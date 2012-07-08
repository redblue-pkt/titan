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
		return first;
	}

	return -1;
}

void caservicedel(struct service* snode, struct caslot* caslot)
{
	int i = 0;

printf("a -> %d\n", i);

	for(i = 0; i < MAXCASERVICE; i++)
	{
printf("b -> %d\n", i);
		if(snode != NULL && caservice[i].service != NULL && caservice[i].channel == snode->channel)
		{
printf("c -> %d\n", i);
			caservice[i].count--;
printf("d -> %d\n", i);
			if(caservice[i].count < 1)
			{
printf("e -> %d\n", i);
				if(caservice[i].camsockfd > -1)
					sockclose(&caservice[i].camsockfd);
printf("f -> %d\n", i);
				if(caservice[i].caslot != NULL)
				{
printf("g -> %d\n", i);
					sendcapmt(caservice[i].service, 1, 2);
printf("h -> %d\n", i);
					if(caservice[i].caslot != NULL && caservice[i].camanager > -1 && caservice[i].caslot->casession[caservice[i].camanager].inuse > 0) caservice[i].caslot->casession[caservice[i].camanager].inuse = 1;
printf("i -> %d\n", i);
					caservice[i].caslot = NULL;
					caservice[i].camanager = -1;
				}
				caservice[i].service = NULL;
				caservice[i].channel = NULL;
			}
			else
				caservice[i].service = getservicebyservice(snode, 1);
		}
printf("j -> %d\n", i);
		//remove cam from slot
		if(caservice[i].service != NULL && caservice[i].caslot == caslot)
		{
printf("k -> %d\n", i);
			if(caservice[i].caslot != NULL && caservice[i].camanager > -1 && caservice[i].caslot->casession[caservice[i].camanager].inuse > 0) caservice[i].caslot->casession[caservice[i].camanager].inuse = 1;
printf("l -> %d\n", i);
			caservice[i].caslot = NULL;
			caservice[i].camanager = -1;
printf("m -> %d\n", i);
		}
	}
printf("n -> %d\n", i);
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

void sendcapmtend(struct service* node)
{
	unsigned char* buf = NULL;

	if(node == NULL)
	{
		debug(620, "service empty");
		return;
	}
	if(node->fedev == 0)
	{
		debug(620, "no frontend");
		return;
	}

	buf = malloc(8);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}
	memset(buf, 0, 8);

	buf[0] = 0x9F; // ca_pmt_tag
        buf[1] = 0x80; // ca_pmt_tag
        buf[2] = 0x3f; // ca_pmt_tag
        buf[3] = 0x04; // ca_pmt_tag
        buf[4] = 0x83; // ca_pmt_tag
        buf[5] = 0x02; // ca_pmt_tag
        buf[6] = 0x00; // ca_pmt_tag
	buf[7] = node->fedev->devnr; //demux_dev_nr
	//sendcapmttosock(node, buf, 8);

	free(buf);
}

//flag 0 = from zap
//flag 1 = from watchthread
//flag 2 = from cathread / caservicedel
//flag 3 = from recordthread
void sendcapmt(struct service* node, int clear, int flag)
{
	int pos = 10, i = 0, lenbytes = 0, round = 0, caservicenr = 0;
	unsigned char* buf = NULL;

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

		debug(200, "set ci slot %d to tuner %d", node->fedev->devnr, node->fedev->devnr);
		switch(node->fedev->devnr)
		{
			case 0: setcisource(node->fedev->devnr, "A"); break;
			case 1: setcisource(node->fedev->devnr, "B"); break;
			case 2: setcisource(node->fedev->devnr, "C"); break;
			case 3: setcisource(node->fedev->devnr, "D"); break;
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
	}

	struct cadesc* cadescnode = node->channel->cadesc;
	struct esinfo* esinfonode = node->channel->esinfo;

start:
	pos = 10, lenbytes = 0, i = 0;
	cadescnode = node->channel->cadesc;
	esinfonode = node->channel->esinfo;
	memset(buf, 0, MINMALLOC);

	buf[0] = 0x9F; // ca_pmt_tag
        buf[1] = 0x80; // ca_pmt_tag
        buf[2] = 0x32; // ca_pmt_tag

	buf[pos++] = 3; //ca_pmt_lst_management: fixme 5 for update
	buf[pos++] = node->channel->pmt->programnumber >> 8;
	buf[pos++] = node->channel->pmt->programnumber & 0xff;

	buf[pos++] = (node->channel->pmt->versionnumber << 1) | node->channel->pmt->currentnextindicator;

	buf[pos++] = 0x00; //len from here (programinfo len) 
	buf[pos++] = 0x00; //len from here (programinfo len)
	buf[pos++] = 0x01; //ca_pmt_cmd_id: ok_descrambling=1

	if(round == 1)
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

	while(cadescnode != NULL && clear == 0)
	{
		if(cadescnode->len > 0)
		{
			buf[pos++] = 0x09; //ca desc tag
			buf[pos++] = cadescnode->len;
			buf[pos++] = cadescnode->systemid >> 8;
			buf[pos++] = cadescnode->systemid & 0xff;
			buf[pos++] = (cadescnode->reserved << 5) | (cadescnode->pid >> 8);
			buf[pos++] = cadescnode->pid & 0xff;
			if(cadescnode->len > 4)
			{
				memcpy(&buf[pos], cadescnode->privat, cadescnode->len - 4);
				pos += cadescnode->len - 4;
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
		pos += 2;
		esinfonode = esinfonode->next;
	}

	lenbytes = writelengthfield(&buf[3], pos - 10);
	for(i = 0; i < pos + 10; i++)
	{
		buf[lenbytes + 3 + i] = buf[10 + i];
	}
	pos = pos - 10 + lenbytes + 3;
	tmppos = tmppos - 10 + lenbytes + 3;

	//programinfo len
	buf[8 + lenbytes] = (tmppos - 9 - lenbytes) & 0xff;
	buf[7 + lenbytes] = ((tmppos - 9 - lenbytes) >> 8) & 0xff;

	if(round == 0)
	{
#ifdef CAMSUPP
		if(caservice[caservicenr].caslot == NULL || clear == 1)
			sendcapmttocam(node, buf, pos, caservicenr);
#endif
		if(clear == 0)
		{
			round = 1;
			goto start;
		}
	}
	else
	{
		if(caservice[caservicenr].camsockfd < 0)
			sendcapmttosock(node, buf, pos, caservicenr);
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
	debug(1000, "in");
	struct pmt *newnode = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
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

	debug(1000, "out");
	return newnode;
}

struct cadesc* addcadesc(struct channel* chnode, unsigned char* buf, struct cadesc* last)
{
	debug(1000, "in");
	struct cadesc *newnode = NULL, *prev = NULL, *node = NULL;

	if(chnode == NULL || buf == NULL)
	{
		debug(1000, "out -> NULL detect");
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

	newnode->len = buf[1];
	newnode->systemid = (buf[2] << 8) | buf[3];
	newnode->reserved = buf[4] >> 5;
	newnode->pid = ((buf[4] & 0x1F) << 8) | buf[5];

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

	debug(1000, "out");
	return newnode;
}

struct esinfo* addesinfo(struct channel* chnode, int streamtype, int pid, struct esinfo* last)
{
	debug(1000, "in");
	struct esinfo *newnode = NULL, *prev = NULL, *node = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
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

	debug(1000, "out");
	return newnode;
}

void freepmt(struct channel* chnode)
{
	debug(1000, "in");

	if(chnode == NULL || chnode->pmt == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

	free(chnode->pmt);
	chnode->pmt = NULL;

	debug(1000, "out");
}

void freecadesc(struct channel* chnode)
{
	debug(1000, "in");
	struct cadesc *node = NULL, *prev = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

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

	debug(1000, "out");
}

void freeesinfo(struct channel* chnode)
{
	debug(1000, "in");
	struct esinfo *node = NULL, *prev = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

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

	debug(1000, "out");
}

#endif
