#ifndef CAM_H
#define CAM_H

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


void sendcapmttosock(struct service* node, unsigned char* buf, int pos)
{
	int ret = 0;

	if(node->camsockfd < 0 || socksend(&node->camsockfd, buf, pos, -1) != 0)
	{
		ret = sockopen(&node->camsockfd, "/tmp/camd.socket", 0, -1);
		if(ret == 0)
		{
			if(socksend(&node->camsockfd, buf, pos, -1) == 0)
			{
				debug(250, "send ca pmt -> ok");
			}
		}
	}
	else
		debug(250, "send ca pmt -> ok");
}

void sendcapmtend(struct service* node)
{
	unsigned char* buf = NULL;

	if(node->fedev == 0)
	{
		debug(250, "no frontend");
		return;
	}
	if(node == NULL)
	{
		debug(250, "service empty");
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
	sendcapmttosock(node, buf, 8);

	free(buf);
}

//flag 0 = close old socket
//flag 1 = don't close old socket
//flag 2 = send to ca
void sendcapmt(struct service* node, int flag)
{
	int pos = 10, i = 0, lenbytes = 0, nok = 0;
	unsigned char* buf = NULL;

	if(node == NULL)
	{
		debug(250, "service empty");
		return;
	}

	if(node->channel == NULL)
	{
		debug(250, "channel empty");
		nok = 1;
	}
	if(node->channel->pmt == NULL)
	{
		debug(250, "pmt empty");
		nok = 1;
	}
	if(node->channel->cadesc == NULL)
	{
		debug(250, "cadesc empty");
		nok = 1;
	}
	if(node->channel->esinfo == NULL)
	{
		debug(250, "esinfo empty");
		nok = 1;
	}
	if(node->channel->crypt == 0)
	{
		debug(250, "channel not crypr");
		nok = 1;
	}
	if(node->fedev == 0)
	{
		debug(250, "no frontend");
		nok = 1;
	}

	struct cadesc* cadescnode = node->channel->cadesc;
	struct esinfo* esinfonode = node->channel->esinfo;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		nok = 1;
	}

	if(nok == 1)
	{
		free(buf);
		sockclose(&node->camsockfd);
		return;
	}
	if(flag == 0) sockclose(&node->camsockfd);

	memset(buf, 0, MINMALLOC);

	buf[0] = 0x9F; // ca_pmt_tag
        buf[1] = 0x80; // ca_pmt_tag
        buf[2] = 0x32; // ca_pmt_tag

	buf[pos++] = 3; //ca_pmt_lst_management: fixme 5 for update
	buf[pos++] = node->channel->pmt->programnumber >> 8;
	buf[pos++] = node->channel->pmt->programnumber & 0xff;

	buf[pos++] = (node->channel->pmt->reserved1 << 6) | (node->channel->pmt->versionnumber << 1) | node->channel->pmt->currentnextindicator;

	buf[pos++] = 0x00; //len from here (programinfo len) 
	buf[pos++] = 0x00; //len from here (programinfo len)
	buf[pos++] = 0x01; //ca_pmt_cmd_id: ok_descrambling=1

	if(flag < 2)
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

	while(cadescnode != NULL)
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
		buf[pos++] = (esinfonode->reserved1 << 5) | (esinfonode->pid >> 8);
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

	if(debug_level == 250)
	{
		printf("CA-PMT: ");
		for(i = 0; i < pos; i++)
			printf("%02x ", buf[i] & 0xff);
		printf("\n");
	}

	if(flag == 2)
	{
#ifdef CAMSUPP
		sendcapmttocam(buf, pos);
#endif
	}
	else
		sendcapmttosock(node, buf, pos);

	free(buf);
}

void checkcam()
{
	int ret = 1; 
	struct service* node = NULL;

	if(status.pmtmode == 1) return;

	//struct can change from another thread
	m_lock(&status.servicemutex, 2);
	node = service;
	while(node != NULL)
	{
		if((node->type == CHANNEL || node->type == RECORDDIRECT || node ->type == RECORDTIMER || node->type == RECORDTIMESHIFT || node->type == RECORDSTREAM) && node->channel != NULL && node->channel->crypt == 1 && node->fedev != NULL && node->fedev->type != FRONTENDDEVDUMMY)
		{
			ret = sockcheck(&node->camsockfd);

			if(checkservice(node) == NULL)
			{
				node = service;
				continue;
			}
			if(ret != 0) // socket not connected
			{
				debug(250, "socket not connected, try connect");
				ret = sockopen(&node->camsockfd, "/tmp/camd.socket", 1, -1);
				if(ret == 0)
					sendcapmt(node, 1);
			}
		}
		node = node->next;
	}
	m_unlock(&status.servicemutex, 2);
}

struct pmt* addpmt(struct channel* chnode, int programnumber, int reserved1, int versionnumber, int currentnextindicator, int reserved2)
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
	newnode->reserved1 = reserved1;
	newnode->versionnumber = versionnumber;
	newnode->currentnextindicator = currentnextindicator;
	newnode->reserved2 = reserved2;

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

struct esinfo* addesinfo(struct channel* chnode, int streamtype, int reserved1, int pid, int reserved2, struct esinfo* last)
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
	newnode->reserved1 = reserved1;
	newnode->pid = pid;
	newnode->reserved2 = reserved2;

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
