#ifndef MHWEPG_H
#define MHWEPG_H

//channel C+ DEMANDE for test on astra 19.2

struct mhwchannel* getmhwchannel(unsigned char* channelbuf, int channelcount, int id)
{
	struct mhwchannel* mhwchannel = NULL;
	int i = 0;

	if(channelbuf == NULL) return NULL;
	mhwchannel = (struct mhwchannel*)(channelbuf + 4);

	for(i = 0; i < channelcount; i++)
	{
		if(i == id) return mhwchannel;
		mhwchannel++;
	}

	return NULL;
}

int readmhwchannel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* channelbuf)
{
	int readlen = 0;
	struct dvbdev* dmxnode;

	if(channelbuf == NULL) return -1;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return -1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		return -1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0xD3, 0, 16);

	readlen = dvbread(dmxnode, channelbuf, 0, MINMALLOC, 1000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return -1;
	}

	dmxclose(dmxnode, -1);
	return readlen;
}

int readmhwtitle(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int channelcount)
{
	int readlen = 0, first = 1;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	struct mhwtitle* mhwtitle = NULL;
	struct mhwchannel* mhwchannel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0;
	struct channel* tmpchnode = NULL;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || chnode == status.aktservice->channel && status.aktservice->type != CHANNEL)
		return;

	buf = calloc(1, MHWTITLELEN);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}
	mhwtitle = (struct mhwtitle*)buf;

	firstbuf = calloc(1, MHWTITLELEN);
	if(firstbuf == NULL)
	{
		err("no memory");
		free(buf);
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0xD2, 0, 15);

	//Skip also the titles until the next separator
	/*
	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, MHWTITLELEN, 1000000);
		usleep(1000);
		if(readlen != MHWTITLELEN)
		{
			perr("Error while reading titles");
			dmxclose(dmxnode, -1);
			free(buf);
			return 1;
		}
		if(mhwtitle->channel_id == 0xFF) break;
	}
	*/

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, MHWTITLELEN, 1000000);
		usleep(1000);
		if(readlen != MHWTITLELEN)
		{
			perr("Error while reading titles");
			dmxclose(dmxnode, -1);
			free(buf);
			free(firstbuf);
			return 1;
		}
		if(first == 1)
		{
			first = 0;
			memcpy(firstbuf, buf, MHWTITLELEN);
		}
		else
		{
			if(memcmp(firstbuf, buf, MHWTITLELEN) == 0)
			{
				//TODO: set transponder epg time
				break;
			}

			//TODO: check timeout
		}
		//if(mhwtitle->channel_id == 0xFF) break;

		printf("%d\n", mhwtitle->channel_id);
		mhwchannel = getmhwchannel(channelbuf, channelcount, mhwtitle->channel_id);
		if(mhwchannel != NULL)
		{
			transponderid = ((HILO(mhwchannel->network_id) << 16) | HILO(mhwchannel->transponder_id)) & 0xffffffff;
                	if(chnode->transponder != NULL)
			{
				if(chnode->transponder->fetype == FE_QAM)
					transponderid = transponderid | ((uint64_t)1 << 32);
				else if(chnode->transponder->fetype == FE_OFDM)
					transponderid = transponderid | ((uint64_t)2 << 32);
			}

			serviceid = HILO(mhwchannel->channel_id);
			tmpchnode = getchannel(serviceid, transponderid);
			if(tmpchnode == NULL)
			{
				debug(1000, "out -> NULL detect");
				continue;
			}
			printf("kanal: %s\n", tmpchnode->name);

		}
		printf("%s\n", mhwtitle->title);
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return 0;
}

int readmhw(struct stimerthread* self, struct dvbdev* fenode)
{
	int ret = 0, channelcount = 0;
	unsigned char* channelbuf = NULL;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx1\n");

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx2\n");
		return -1;
	}

	channelbuf = calloc(1, MINMALLOC);
	if(channelbuf == NULL)
	{
		err("no memory");
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx2\n");
		return 1;
	}

	channelcount = readmhwchannel(self, fenode, channelbuf);
	if(channelcount <= 0)
	{
		debug(400, "no channels found");
		free(channelbuf); channelbuf = NULL;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx2\n");
		return 1;
	}

	ret = readmhwtitle(self, fenode, NULL, channelbuf, channelcount);
	if(ret != 0)
	{
		free(channelbuf); channelbuf = NULL;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx2\n");
		return 1;
	}

	free(channelbuf); channelbuf = NULL;
printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx2\n");
	return 0;
}

#endif
