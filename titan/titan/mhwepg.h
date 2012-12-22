#ifndef MHWEPG_H
#define MHWEPG_H

//channel C+ DEMANDE for test on astra 19.2

unsigned long getquad(void *ptr)
{
	unsigned char *data = ptr;
	return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

unsigned short getdoub(void *ptr)
{
	unsigned char *data = ptr;
	return (data[0] << 8) | data[1];
}

time_t localtime2utc(time_t t)
{
	struct tm *temp;

	temp = gmtime(&t);
	temp->tm_isdst = -1;
	return mktime(temp);
}

time_t utc2localtime(time_t t)
{
	return 2 * t - localtime2utc(t);
}

int mhwtimeoffset(int *yesterday, time_t *yesterdayepoch)
{
	struct tm *curtime;
	time_t yesterdaytime = 0;

	// Get day of the week of yesterday (provider local time)
	yesterdaytime = utc2localtime(time(NULL) - 86400);
	curtime = gmtime(&yesterdaytime);
	*yesterday = curtime->tm_wday;

 	// Get epoch of yesterday 00:00 (provider local time)
	curtime->tm_hour = 0;
	curtime->tm_min = 0;
	curtime->tm_sec = 0;
	curtime->tm_isdst = -1;
	*yesterdayepoch = utc2localtime(mktime(curtime));

	return 0;
}

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

struct mhw2channel* getmhw2channel(unsigned char* channelbuf, int id)
{
	struct mhw2channel* mhw2channel = NULL;
	int channelcount = 0;
	int i = 0;

	if(channelbuf == NULL) return NULL;
	mhw2channel = (struct mhw2channel*)(channelbuf + 121);

	channelcount = channelbuf[120];

	for(i = 0; i < channelcount; i++)
	{
		if(i == id) return mhw2channel;
		mhw2channel++;
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

int readmhw2channel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* channelbuf)
{
	int readlen = 0, count = 0;
	unsigned short len = 0;
	struct dvbdev* dmxnode;

	if(channelbuf == NULL) return 1;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0x231, 0, 19);

start:
	readlen = dvbread(dmxnode, channelbuf, 0, 3, 1000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return 1;
	}
	readlen = 0;
	len = channelbuf[2] | ((channelbuf[1] & 0x0f) << 8);
	if(len + 3 <= MINMALLOC)
		readlen = dvbread(dmxnode, channelbuf, 3, len, 1000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return 1;
	}

	count++;
	if(channelbuf[3] != 0 && count < 6) goto start;

	dmxclose(dmxnode, -1);

	if(count >= 6)
		return 1;
	else
		return 0;
}

int readmhwtitle(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int channelcount)
{
	int readlen = 0, first = 1;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	struct mhwtitle* mhwtitle = NULL;
	struct mhwchannel* mhwchannel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0, day = 0, hours = 0, yesterday = 0;
	int nolongdesc = 0;
	struct channel* tmpchnode = NULL;
	struct epg* epgnode = NULL;
	char* zbuf = NULL;
	int zlen = 0, ret = 0;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0, yesterdayepoch = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	mhwtimeoffset(&yesterday, &yesterdayepoch);

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
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		nolongdesc = 0;

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
				debug(400, "mhwepg no more new data, wait for next run");

				if(chnode != NULL && chnode->transponder != NULL)
					chnode->transponder->lastepg = time(NULL) + 7700;
				break;
			}

			//stop epgscan after 2 min
			if(akttime + 120 > time(NULL))
			{
				debug(400, "mhwepg timeout");
				break;
			}
		}

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

			//look in epglist if channel exists
			//only if epglist is marked as whitelist
			if(status.epglistmode == 2 || status.epglistmode == 3)
			{
				if(getepgscanlist(serviceid, transponderid) == NULL)
					continue;
			}

			m_lock(&status.epgmutex, 4);

			eventid = (mhwtitle->channel_id << 16) | (mhwtitle->day << 13) | (mhwtitle->hours << 8) | mhwtitle->minutes;
			epgnode = getepg(tmpchnode, eventid, 1);

			//get start/endtime
			day = mhwtitle->day;
			hours = mhwtitle->hours;
	    if(day == 7) day = 0;
			if(hours > 15) hours = hours - 4;
			else if(hours > 7) hours = hours - 2;
			else day = (day == 6) ? 0 : day + 1;

			dvbtime = (day - yesterday) * 86400 + hours * 3600 + mhwtitle->minutes * 60;
			if(dvbtime < 6 * 3600) dvbtime += 7 * 86400;	/* Next week */
			dvbtime += yesterdayepoch;
			starttime = localtime2utc(dvbtime);
			endtime = starttime + (HILO(mhwtitle->duration) * 60);

#ifndef SIMULATE
			if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
			{
				m_unlock(&status.epgmutex, 4);
				continue;
			}
#endif

			if(epgnode == NULL)
				epgnode = addepg(tmpchnode, eventid, 0, starttime, endtime, NULL, 1);
			else
			{
				updateepg(tmpchnode, epgnode, eventid, 0, starttime, endtime, 1);
				if(epgnode->desc != NULL)
					nolongdesc = 1;
			}

			if(epgnode == NULL)
			{
				debug(1000, "out -> NULL detect");
				m_unlock(&status.epgmutex, 4);
				continue;
			}

			epgnode->title = ostrcat((char*)mhwtitle->title, NULL, 0, 0);

			//TODO: get long descr;
			//compress long desc
			if(nolongdesc == 0 && epgnode->desc != NULL)
			{
				ret = ozip(epgnode->desc, strlen(epgnode->desc) + 1, &zbuf, &zlen, 1);
				if(ret == 0)
				{
					free(epgnode->desc); epgnode->desc = NULL;
					epgnode->desc = zbuf;
					epgnode->desccomplen = zlen;
				}
			}
			m_unlock(&status.epgmutex, 4);
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return 0;
}

int readmhw2title(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf)
{
	int readlen = 0, pos = 0, len = 0;
	unsigned char *buf = NULL;
	struct dvbdev* dmxnode;
	struct mhw2channel* mhw2channel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0;
	struct channel* tmpchnode = NULL;
	unsigned long quad = 0, quad0 = 0;
	int nolongdesc = 0;
	struct epg* epgnode = NULL;
	char* zbuf = NULL;
	int zlen = 0, ret = 0;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	char tmpstr[65];

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0x234, 0, 18);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		nolongdesc = 0;

		readlen = dvbread(dmxnode, buf, 0, 3, 1000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return -1;
		}
		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 1000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return -1;
		}

		//check for end
		quad = getquad(buf + 3);
		if(quad0 == 0) quad0 = quad;
		else if(quad == quad0)
		{
			debug(400, "mhw2epg no more new data, wait for next run");

			if(chnode != NULL && chnode->transponder != NULL)
				chnode->transponder->lastepg = time(NULL) + 7700;
			break;
		}

		//stop epgscan after 2 min
		if(akttime + 120 > time(NULL))
		{
			debug(400, "mhw2epg timeout");
			break;
		}

		pos = 18;
		while(pos < len)
 		{
			int channelid = buf[pos];

			//get start/endtime
			dvbtime = (getdoub(buf + pos + 3) - 40587) * 86400
			 + (((buf[pos + 5] & 0xf0) >> 4) * 10 + (buf[pos + 5] & 0x0f)) * 3600
			 + (((buf[pos + 6] & 0xf0) >> 4) * 10 + (buf[pos + 6] & 0x0f)) * 60;

			starttime = dvbtime;
			endtime = starttime + (getdoub(buf + pos + 8) >> 4) * 60;

			int lgr = buf[pos + 10] & 0x3f;
			if(lgr < 65)
			{
				memcpy(tmpstr, buf + pos + 11, lgr);
				tmpstr[lgr] = '\0';
			}
			else
				tmpstr[0] = '\0';
			pos += lgr + 11;
			//program->id = getdoub(buf + pos + 1);

			mhw2channel = getmhw2channel(channelbuf, channelid);
			if(mhw2channel != NULL)
			{
				transponderid = ((HILO(mhw2channel->network_id) << 16) | HILO(mhw2channel->transponder_id)) & 0xffffffff;
				if(chnode->transponder != NULL)
				{
					if(chnode->transponder->fetype == FE_QAM)
						transponderid = transponderid | ((uint64_t)1 << 32);
					else if(chnode->transponder->fetype == FE_OFDM)
						transponderid = transponderid | ((uint64_t)2 << 32);
				}

				serviceid = HILO(mhw2channel->channel_id);
				tmpchnode = getchannel(serviceid, transponderid);
				if(tmpchnode == NULL)
				{
					debug(1000, "out -> NULL detect");
					pos += 4;
					continue;
				}

				//look in epglist if channel exists
				//only if epglist is marked as whitelist
				if(status.epglistmode == 2 || status.epglistmode == 3)
				{
					if(getepgscanlist(serviceid, transponderid) == NULL)
					{
						pos += 4;
						continue;
					}
				}

				m_lock(&status.epgmutex, 4);

				//eventid = (mhwtitle->channel_id << 16) | (mhwtitle->day << 13) | (mhwtitle->hours << 8) | mhwtitle->minutes;
				epgnode = getepg(tmpchnode, eventid, 1);

#ifndef SIMULATE
				if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
				{
					m_unlock(&status.epgmutex, 4);
					pos += 4;
					continue;
				}
#endif

				if(epgnode == NULL)
					epgnode = addepg(tmpchnode, eventid, 0, starttime, endtime, NULL, 1);
				else
				{
					updateepg(tmpchnode, epgnode, eventid, 0, starttime, endtime, 1);
					if(epgnode->desc != NULL)
						nolongdesc = 1;
				}

				if(epgnode == NULL)
				{
					debug(1000, "out -> NULL detect");
					m_unlock(&status.epgmutex, 4);
					pos += 4;
					continue;
				}

				epgnode->title = ostrcat(tmpstr, NULL, 0, 0);

				//TODO: get long descr;
				//compress long desc
				if(nolongdesc == 0 && epgnode->desc != NULL)
				{
					ret = ozip(epgnode->desc, strlen(epgnode->desc) + 1, &zbuf, &zlen, 1);
					if(ret == 0)
					{
						free(epgnode->desc); epgnode->desc = NULL;
						epgnode->desc = zbuf;
						epgnode->desccomplen = zlen;
					}
				}
				m_unlock(&status.epgmutex, 4);

			}
			pos += 4;
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	return 0;
}

int readmhw2summarie(struct stimerthread* self, struct dvbdev* fenode)
{
	int readlen = 0, len = 0;
	unsigned char *buf = NULL;
	struct dvbdev* dmxnode;
	unsigned long quad = 0, quad0 = 0;
	time_t akttime = 0;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0x236, 0, 20);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, 3, 1000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return -1;
		}
		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 1000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return -1;
		}

		//check for end
		quad = getquad(buf + 3);
		if(quad0 == 0) quad0 = quad;
		else if(quad == quad0) break;

		//stop epgscan after 2 min
		if(akttime + 120 > time(NULL))
		{
			debug(400, "mhw2epg timeout");
			break;
		}

/*
      summary = malloc (sizeof (struct summary));
      summaries[nb_summaries++] = summary;

      summary->id = getdoub(buf + 3);
      nb = buf[12];
      int pos = 13 + nb;
      nb = buf[pos] & 0x0f;
      pos += 1;
      len=0;
      int len_line;
      for(; nb > 0; nb--)
      {
				len_line = buf[pos + len];
				buf[pos + len] = '|';
				len += len_line+1;
      }
      if(len > 0)
        buf[pos + len] = 0;
      else
        buf[pos + 1] = 0;
      summary->text = malloc(len + 1);
      if(summary->text == NULL)
      {
         fprintf (stderr,"Memory allocation error %d\n", len + 1);
         exit(1);
      }
      memcpy(summary->text, buf + pos + 1, len + 1);
*/
	}

	dmxclose(dmxnode, -1);
	free(buf);
	return 0;
}

int readmhw(struct stimerthread* self, struct dvbdev* fenode)
{
	int ret = 0, channelcount = 0;
	unsigned char* channelbuf = NULL;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return -1;
	}

	channelbuf = calloc(1, MINMALLOC);
	if(channelbuf == NULL)
	{
		err("no memory");
		return 1;
	}

	channelcount = readmhwchannel(self, fenode, channelbuf);
	if(channelcount <= 0)
	{
		debug(400, "mhwepg no channels found");
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	ret = readmhwtitle(self, fenode, NULL, channelbuf, channelcount);
	if(ret != 0)
	{
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	free(channelbuf); channelbuf = NULL;
	return 0;
}

int readmhw2(struct stimerthread* self, struct dvbdev* fenode)
{
	int ret = 0;
	unsigned char* channelbuf = NULL;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return -1;
	}

	channelbuf = calloc(1, MINMALLOC);
	if(channelbuf == NULL)
	{
		err("no memory");
		return 1;
	}

	ret = readmhw2channel(self, fenode, channelbuf);
	if(ret != 0)
	{
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	ret = readmhw2title(self, fenode, NULL, channelbuf);
	if(ret != 0)
	{
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	free(channelbuf); channelbuf = NULL;
	return 0;
}

#endif
