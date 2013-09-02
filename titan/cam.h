#ifndef CAM_H
#define CAM_H

void debugcaservice()
{
	int i = 0;

	for(i = 0; i < MAXCASERVICE; i++)
	{
		if(caservice[i].service != NULL)
		{
			printf("number: %d\n", i);
			printf("service: %p\n", caservice[i].service);
			if(caservice[i].channel != NULL)
				printf("channel: %s\n", caservice[i].channel->name);
			printf("camsockfd: %d\n", caservice[i].camsockfd);
			printf("caslot: %p\n", caservice[i].caslot);
			printf("count: %d\n", caservice[i].count);
		}
	}
}

int getcaservicebyslot(struct caslot* caslot, int flag)
{
	int i = 0;

	for(i = 0; i < MAXCASERVICE; i++)
	{
		if(caslot == caservice[i].caslot)
		{
			if(flag == 1)
			{
				if(caservice[i].service != NULL && (caservice[i].service->type == RECORDDIRECT || caservice[i].service->type == RECORDTIMER))
					return i;
			}
			else
				return i;
		}
	}
	return -1;
}

//flag 0 = from zap
//flag 1 = from watchthread
//flag 2 = from cathread / caservicedel
//flag 3 = from recordthread
int caserviceadd(struct service* snode, int flag)
{
	int i = 0, first = -1;

	if(snode == NULL) return -1;

	for(i = 0; i < MAXCASERVICE; i++)
	{
		if(caservice[i].service != NULL && caservice[i].channel == snode->channel)
		{
			if((flag == 0 || flag == 3))
			{
				caservice[i].service = snode;
				caservice[i].count++;
				return -1;
			}
			return i;
		}
		if(caservice[i].service == NULL && first == -1)
			first = i;
	}

	if(first != -1)
	{
		caservice[first].service = snode;
		caservice[first].channel = snode->channel;
		caservice[first].count = 1;
		caservice[first].camsockfd = -1;
		if(caservice[first].caslot != NULL && caservice[first].camanager > -1 && caservice[first].caslot->casession[caservice[first].camanager].inuse > 0) caservice[first].caslot->casession[caservice[first].camanager].inuse = 1;
		caservice[first].caslot = NULL;
		caservice[first].camanager = -1;
		free(caservice[first].capmt); caservice[first].capmt = NULL;
		caservice[first].capmtlen = 0;
		caservice[first].cmdpos = 0;
		return first;
	}

	return -1;
}

void caservicedel(struct service* snode, struct caslot* caslot)
{
	int i = 0;

	for(i = 0; i < MAXCASERVICE; i++)
	{
		if(snode != NULL && caservice[i].service != NULL && caservice[i].channel == snode->channel)
		{
			caservice[i].count--;
			if(caservice[i].count < 1)
			{
				if(caservice[i].camsockfd > -1)
					sockclose(&caservice[i].camsockfd);
				if(caservice[i].caslot != NULL)
				{
					//sendcapmt(caservice[i].service, i + 1, 2);
					if(caservice[i].caslot != NULL && caservice[i].camanager > -1 && caservice[i].caslot->casession[caservice[i].camanager].inuse > 0) caservice[i].caslot->casession[caservice[i].camanager].inuse = 1;
					caservice[i].caslot = NULL;
					caservice[i].camanager = -1;
				}
				free(caservice[i].capmt); caservice[i].capmt = NULL;
				caservice[i].count = 0;
				caservice[i].capmtlen = 0;
				caservice[i].cmdpos = 0;
				caservice[i].service = NULL;
				caservice[i].channel = NULL;
			}
			else
				caservice[i].service = getservicebyservice(snode, 1);
		}
		//remove cam from slot
		if(caservice[i].service != NULL && caservice[i].caslot == caslot)
		{
			if(caservice[i].caslot != NULL && caservice[i].camanager > -1 && caservice[i].caslot->casession[caservice[i].camanager].inuse > 0) caservice[i].caslot->casession[caservice[i].camanager].inuse = 1;
			caservice[i].caslot = NULL;
			caservice[i].camanager = -1;
		}
	}
}

void freecaservice()
{
	int i = 0;

	for(i = 0; i < MAXCASERVICE; i++)
	{
		free(caservice[i].capmt);
		caservice[i].capmt = NULL;

		caservice[i].caslot = NULL;
		caservice[i].camanager = -1;
		caservice[i].capmtlen = 0;
		caservice[i].cmdpos = 0;
		caservice[i].service = NULL;
		caservice[i].channel = NULL;

		if(caservice[i].camsockfd > -1)
			sockclose(&caservice[i].camsockfd);
	}
}

void dvbwritepmt(struct service* node, unsigned char* pmtbuf)
{
	int length;
	FILE *fd;
	char* filename = NULL, *tmpnr = NULL;

	if(node == NULL || pmtbuf == NULL)
	{
		debug(1000, "NULL detect");
		return;
	}

	if(node->fedev == NULL || (node->fedev != NULL && node->fedev->devnr == 0))
	{
		filename = ostrcat(filename, "/tmp/pmt.tmp", 0, 0);
	}
	else
	{
		tmpnr = oitoa(node->fedev->devnr);
		filename = ostrcat(filename, "/tmp/pmt", 1, 0);
		filename = ostrcat(filename, tmpnr, 1, 1);
		filename = ostrcat(filename, ".tmp", 1, 0);
	}

	unlink(filename);
	length =((pmtbuf[1] & 0xf) << 8) + ((pmtbuf[2] + 3) & 0xff);
	if(filename != NULL)
	{
		fd=fopen(filename, "wbt");
		if(fd != NULL)
		{
			if((int) fwrite(pmtbuf, sizeof(unsigned char), length, fd) != length)
				unlink(filename);
			else
				debug(200, "write %s", filename);
			fclose(fd);
		}
	}
	free(filename);
}

void sendcapmttosock(struct service* node, unsigned char* buf, int pos, int caservicenr)
{
	int ret = 0, i = 0;

	if(caservice[caservicenr].camsockfd < 0 || socksend(&caservice[caservicenr].camsockfd, buf, pos, -1) != 0)
	{
		ret = sockopen(&caservice[caservicenr].camsockfd, "/tmp/camd.socket", 0, -1);
		if(ret == 0)
			ret = socksend(&caservice[caservicenr].camsockfd, buf, pos, -1);
	}

	if(ret == 0 && debug_level == 620)
	{
		printf("CA-PMT: ");
		for(i = 0; i < pos; i++)
			printf("%02x ", buf[i] & 0xff);
		printf("\n");
	}
}

//flag 0: cam capmt
//flag 1: emu capmt
//flag 2: clear capmt
int createcapmt(struct dvbdev* dvbnode, struct service* node, unsigned char* buf, int* lenbytes, int flag)
{
	if(buf == NULL || node == NULL || node->channel == NULL) return -1;
	if(node->fedev == NULL || node->channel->pmt == NULL) return -1;
	if(node->channel->cadesc == NULL || node->channel->esinfo == NULL) return -1;

	int pos = 10, i = 0;
	struct cadesc* cadescnode = node->channel->cadesc;
	struct esinfo* esinfonode = node->channel->esinfo;

	memset(buf, 0, MINMALLOC);

	buf[0] = 0x9F; // ca_pmt_tag
	buf[1] = 0x80; // ca_pmt_tag
	buf[2] = 0x32; // ca_pmt_tag

	buf[pos++] = 0x03; //ca_pmt_lst_management: 0=more, 1=first, 2=last, 3=only, 4=add, 5=update
	buf[pos++] = node->channel->pmt->programnumber >> 8;
	buf[pos++] = node->channel->pmt->programnumber & 0xff;

	buf[pos++] = (node->channel->pmt->versionnumber << 1) | node->channel->pmt->currentnextindicator;

	buf[pos++] = 0x00; //len from here (programinfo len)
	buf[pos++] = 0x00; //len from here (programinfo len)
	buf[pos++] = 0x01; //ca_pmt_cmd_id: ok_descrambling=1

	if(flag == 1)
	{
		buf[pos++] = 0x81; //id (fix)
		buf[pos++] = 0x08; //len
		buf[pos++] = 0x00; //namespace >> 24
		buf[pos++] = 0x00; //(namespace >> 16) & 0xff
		buf[pos++] = 0x00; //(namespace >> 8) & 0xff
		buf[pos++] = 0x00; //namespace & 0xff
		buf[pos++] = 0x00; //transportstreamid >> 8
		buf[pos++] = 0x00; //transportstreamid & 0xff
		buf[pos++] = 0x00; //original network id >> 8
		buf[pos++] = 0x00; //original network id & 0xff

		buf[pos++] = 0x82; //id (fix)
		buf[pos++] = 0x02; //len

/*
		while(tmpnode != NULL)
		{
			if(tmpnode->fedev != NULL)
			{
				//(demuxmask |= (1 << dmxdev->devnr)
				buf[pos] |= (1 << tmpnode->fedev->devnr);
			}
			tmpnode = tmpnode->next;
		}
		pos++;
*/
		buf[pos++] |= (1 << node->fedev->devnr); //cadev_nr
		buf[pos++] = node->fedev->devnr; //demux_dev_nr

		buf[pos++] = 0x84; //id (fix)
		buf[pos++] = 0x02; //len
		buf[pos++] = node->channel->pmtpid >> 8; //pmtpid >> 8
		buf[pos++] = node->channel->pmtpid & 0xff; //pmtpid & 0xff
	}

	while(cadescnode != NULL && flag < 2)
	{
		//remove caids, cam can't
		if(flag == 0 && status.casendallcaids == 0)
		{
			char* tmpnr = NULL;

			if(dvbnode != NULL)
			{
				tmpnr = oitoa(cadescnode->systemid);
				if(ostrstr(dvbnode->caslot->caids, tmpnr) == NULL)
				{
					free(tmpnr); tmpnr = NULL;
					cadescnode = cadescnode->next;
					continue;
				}
				free(tmpnr); tmpnr = NULL;
			}
		}

		if(cadescnode->len > 0 && cadescnode->pid == 0)
		{
			int cadesclen = 0;

			buf[pos++] = 0x09; //ca desc tag
			if(flag == 0 && status.caskipprivat == 1) //workaround for to long capmt
			{
				buf[pos++] = 4;
				cadesclen = cadesclen;
			}
			else
			{
				cadesclen = cadescnode->len;
				buf[pos++] = cadesclen;
			}
			buf[pos++] = cadescnode->systemid >> 8;
			buf[pos++] = cadescnode->systemid & 0xff;
			buf[pos++] = (cadescnode->reserved << 5) | (cadescnode->capid >> 8);
			buf[pos++] = cadescnode->capid & 0xff;

			if(cadesclen > 4)
			{
				memcpy(&buf[pos], cadescnode->privat, cadesclen - 4);
				pos += cadesclen - 4;
			}
		}
		cadescnode = cadescnode->next;
	}

	int tmppos = pos;

	while(esinfonode != NULL)
	{
		buf[pos++] = esinfonode->streamtype;
		buf[pos++] = esinfonode->pid >> 8;
		buf[pos++] = esinfonode->pid & 0xff;

		int eslenpos = pos;
		int eslen = 0;
		pos += 2;

		cadescnode = node->channel->cadesc;
		while(cadescnode != NULL && flag < 2)
		{
			if(cadescnode->len > 0 && cadescnode->pid == esinfonode->pid)
			{
				//remove caids, cam can't
				if(flag == 0 && status.casendallcaids == 0)
				{
					char* tmpnr = NULL;

					if(dvbnode != NULL)
					{
						tmpnr = oitoa(cadescnode->systemid);
						if(ostrstr(dvbnode->caslot->caids, tmpnr) == NULL)
						{
							free(tmpnr); tmpnr = NULL;
							cadescnode = cadescnode->next;
							continue;
						}
						free(tmpnr); tmpnr = NULL;
					}
				}

				if(eslen == 0)
				{
					eslen = 1;
					buf[pos++] = 0x01; //ca_pmt_cmd_id: ok_descrambling=1
				}
				eslen = eslen + cadescnode->len + 2;
				buf[pos++] = 0x09; //ca desc tag
				buf[pos++] = cadescnode->len;
				buf[pos++] = cadescnode->systemid >> 8;
				buf[pos++] = cadescnode->systemid & 0xff;
				buf[pos++] = (cadescnode->reserved << 5) | (cadescnode->capid >> 8);
				buf[pos++] = cadescnode->capid & 0xff;
				if(cadescnode->len > 4)
				{
					memcpy(&buf[pos], cadescnode->privat, cadescnode->len - 4);
					pos += cadescnode->len - 4;
				}
			}
			cadescnode = cadescnode->next;
		}

		buf[eslenpos + 1] = eslen & 0xff;
		buf[eslenpos] = (eslen >> 8) & 0x0f;

		esinfonode = esinfonode->next;
	}

	*lenbytes = writelengthfield(&buf[3], pos - 10);
	for(i = 0; i < pos + 10; i++)
	{
		buf[(*lenbytes) + 3 + i] = buf[10 + i];
	}
	pos = pos - 10 + (*lenbytes) + 3;
	tmppos = tmppos - 10 + (*lenbytes) + 3;

	//programinfo len
	buf[8 + (*lenbytes)] = (tmppos - 9 - (*lenbytes)) & 0xff;
	buf[7 + (*lenbytes)] = ((tmppos - 9 - (*lenbytes)) >> 8) & 0x0f;

	return pos;
}

//flag 0 = from zap
//flag 1 = from watchthread
//flag 2 = from cathread / caservicedel
//flag 3 = from recordthread
void sendcapmt(struct service* node, int clear, int flag)
{
	int len = 0, i = 0, caservicenr = 0, lenbytes = 0;
	unsigned char* buf = NULL;
	struct dvbdev* dvbnode = dvbdev;

	//check if service should decrypt
	if(node == NULL)
	{
		debug(620, "service empty");
		return;
	}
	if(node->channel == NULL)
	{
		debug(620, "channel empty");
		return;
	}
	if(node->fedev == NULL)
	{
		debug(620, "no frontend");
		return;
	}
	
	if(node->channel->crypt == 0)
	{
		debug(620, "channel not crypt");

		//check if we can change input sources
		int doswitch = 1;
		for(i = 0; i < MAXCASERVICE; i++)
		{
			if(caservice[i].caslot != NULL && caservice[i].service != NULL && caservice[i].service->fedev != NULL)
			{
				if(caservice[i].service->fedev->devnr == node->fedev->devnr && (caservice[i].service->type == RECORDDIRECT || caservice[i].service->type == RECORDTIMER))
				{
					debug(620, "can't change input sources");
					doswitch = 0;
				}
			}
		}

		if(doswitch == 1)
		{
			debug(200, "reset routing tuner %d", node->fedev->devnr);
			switch(node->fedev->devnr)
			{
				case 0: setcisource(node->fedev->devnr, "A"); break;
				case 1: setcisource(node->fedev->devnr, "B"); break;
				case 2: setcisource(node->fedev->devnr, "C"); break;
				case 3: setcisource(node->fedev->devnr, "D"); break;
			}
		}
		
		return;
	}
	
	if(node->channel->pmt == NULL)
	{
		debug(620, "pmt empty");
		return;
	}
	if(node->channel->cadesc == NULL)
	{
		debug(620, "cadesc empty");
		return;
	}
	if(node->channel->esinfo == NULL)
	{
		debug(620, "esinfo empty");
		return;
	}
	if(node->fedev->type == FRONTENDDEVDUMMY)
	{
		debug(620, "dummy frontend not crypt");
		return;
	}
	if(flag != 3 && node->type != CHANNEL && node->type != RECORDDIRECT && node->type != RECORDTIMER && node->type != RECORDTIMESHIFT && node->type != RECORDSTREAM)
	{
		debug(620, "service type should not decrypt");
		return;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}

	if(clear == 0)
	{
		caservicenr = caserviceadd(node, flag);
		if(caservicenr < 0)
		{
			debug(620, "service is decrypt");
			free(buf);
			return;
		}

		while(dvbnode != NULL)
		{
			if(dvbnode->type == CIDEV && dvbnode->fd > -1 && dvbnode->caslot != NULL && dvbnode->caslot->status == 2 && dvbnode->caslot->caids != NULL)
			{
				if(caservice[caservicenr].caslot == NULL)
				{
					lenbytes = 0;
					len = createcapmt(dvbnode, node, buf, &lenbytes, 0);
					if(len > -1)
					{
						if(sendcapmttocam(dvbnode, node, buf, len, caservicenr, lenbytes + 9, clear) == 0) break;
					}
				}
			}
			dvbnode = dvbnode->next;
		}

		if(caservice[caservicenr].camsockfd < 0)
		{
			lenbytes = 0;
			len = createcapmt(NULL, node, buf, &lenbytes, 1);
			if(len > -1)
				sendcapmttosock(node, buf, len, caservicenr);
		}
	}

	if(clear > 0)
	{
		while(dvbnode != NULL)
		{
			if(dvbnode->type == CIDEV && dvbnode->fd > -1 && dvbnode->caslot != NULL && dvbnode->caslot->status == 2 && dvbnode->caslot->caids != NULL)
			{
				if(caservice[clear - 1].caslot == dvbnode->caslot)
				{
					lenbytes = 0;
					len = createcapmt(dvbnode, node, buf, &lenbytes, 2);
					if(len > -1)
						sendcapmttocam(dvbnode, node, buf, len, caservicenr, lenbytes + 9, clear);
					break;
				}
			}
			dvbnode = dvbnode->next;
		}
	}

	free(buf);
}

void checkcam()
{
	int i = 0; 

	if(status.pmtmode == 1) return;
 
	//struct can change from another thread
	m_lock(&status.servicemutex, 2);
	for(i = 0; i < MAXCASERVICE; i++)
	{
		if(caservice[i].service != NULL)
		{
			sockcheck(&caservice[i].camsockfd);
			if(caservice[i].camsockfd < 0 || caservice[i].caslot == NULL)
				sendcapmt(caservice[i].service, 0, 1);
		}
	}
	m_unlock(&status.servicemutex, 2);
}

struct pmt* addpmt(struct channel* chnode, int programnumber, int versionnumber, int currentnextindicator)
{
	struct pmt *newnode = NULL;

	if(chnode == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	newnode = (struct pmt*)malloc(sizeof(struct pmt));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct pmt));

	newnode->programnumber = programnumber;
	newnode->versionnumber = versionnumber;
	newnode->currentnextindicator = currentnextindicator;

	chnode->pmt = newnode;

	return newnode;
}

struct cadesc* addcadesc(struct channel* chnode, int pid, unsigned char* buf, struct cadesc* last)
{
	struct cadesc *newnode = NULL, *prev = NULL, *node = NULL;

	if(chnode == NULL || buf == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	node = chnode->cadesc;

	newnode = (struct cadesc*)malloc(sizeof(struct cadesc));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct cadesc));

	newnode->pid = pid;
	newnode->len = buf[1];
	newnode->systemid = (buf[2] << 8) | buf[3];
	newnode->reserved = buf[4] >> 5;
	newnode->capid = ((buf[4] & 0x1F) << 8) | buf[5];

	if(newnode->len > 4)
	{
		newnode->privat = malloc(newnode->len - 4);
		if(newnode->privat == NULL)
		{
			err("no mem");
		}
		else
			memcpy(newnode->privat, &buf[6], newnode->len - 4);
	}

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		chnode->cadesc = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	return newnode;
}

struct esinfo* addesinfo(struct channel* chnode, int streamtype, int pid, struct esinfo* last)
{
	struct esinfo *newnode = NULL, *prev = NULL, *node = NULL;

	if(chnode == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	node = chnode->esinfo;

	newnode = (struct esinfo*)malloc(sizeof(struct esinfo));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct esinfo));

	newnode->streamtype = streamtype;
	newnode->pid = pid;

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		chnode->esinfo = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	return newnode;
}

void freepmt(struct channel* chnode)
{
	if(chnode == NULL || chnode->pmt == NULL) return;

	free(chnode->pmt);
	chnode->pmt = NULL;
}

void freecadesc(struct channel* chnode)
{
	struct cadesc *node = NULL, *prev = NULL;

	if(chnode == NULL) return;

	node = chnode->cadesc;
	prev = chnode->cadesc;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		chnode->cadesc = node;

		free(prev->privat);
		prev->privat = NULL;

		free(prev);
		prev = NULL;

	}
}

void freeesinfo(struct channel* chnode)
{
	struct esinfo *node = NULL, *prev = NULL;

	if(chnode == NULL) return;

	node = chnode->esinfo;
	prev = chnode->esinfo;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		chnode->esinfo = node;

		free(prev);
		prev = NULL;

	}
}

#endif
