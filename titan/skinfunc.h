#ifndef SKINFUNC_H
#define SKINFUNC_H

char* getplaytext(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.pause > 0)
		tmpstr = ostrcat("Pause", NULL, 0, 0);
	else if(status.play > 0)
		tmpstr = ostrcat("Play", NULL, 0, 0);
	else if(status.playspeed == 1)
		tmpstr = ostrcat(">> 2x", NULL, 0, 0);
	else if(status.playspeed == 2)
		tmpstr = ostrcat(">> 4x", NULL, 0, 0);
	else if(status.playspeed == 3)
		tmpstr = ostrcat(">> 8x", NULL, 0, 0);
	else if(status.playspeed == 4)
		tmpstr = ostrcat(">> 16x", NULL, 0, 0);
	else if(status.playspeed == 5)
		tmpstr = ostrcat(">> 32x", NULL, 0, 0);
	else if(status.playspeed == 6)
		tmpstr = ostrcat(">> 64x", NULL, 0, 0);
	else if(status.playspeed == 7)
		tmpstr = ostrcat(">> 128x", NULL, 0, 0);
	else if(status.playspeed == -1)
		tmpstr = ostrcat("<< 2x", NULL, 0, 0);
	else if(status.playspeed == -2)
		tmpstr = ostrcat("<< 4x", NULL, 0, 0);
	else if(status.playspeed == -3)
		tmpstr = ostrcat("<< 8x", NULL, 0, 0);
	else if(status.playspeed == -4)
		tmpstr = ostrcat("<< 16x", NULL, 0, 0);
	else if(status.playspeed == -5)
		tmpstr = ostrcat("<< 32x", NULL, 0, 0);
	else if(status.playspeed == -6)
		tmpstr = ostrcat("<< 64x", NULL, 0, 0);
	else if(status.playspeed == -7)
		tmpstr = ostrcat("<< 128x", NULL, 0, 0);

	return tmpstr;
}

char* getplaypic(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.pause > 0)
		tmpstr = ostrcat("pause.png", NULL, 0, 0);
	else if(status.play > 0)
		tmpstr = ostrcat("play.png", NULL, 0, 0);
	else if(status.playspeed == 1)
		tmpstr = ostrcat("forward1.png", NULL, 0, 0);
	else if(status.playspeed == 2)
		tmpstr = ostrcat("forward2.png", NULL, 0, 0);
	else if(status.playspeed == 3)
		tmpstr = ostrcat("forward3.png", NULL, 0, 0);
	else if(status.playspeed == 4)
		tmpstr = ostrcat("forward4.png", NULL, 0, 0);
	else if(status.playspeed == 5)
		tmpstr = ostrcat("forward5.png", NULL, 0, 0);
	else if(status.playspeed == 6)
		tmpstr = ostrcat("forward6.png", NULL, 0, 0);
	else if(status.playspeed == 7)
		tmpstr = ostrcat("forward7.png", NULL, 0, 0);
	else if(status.playspeed == -1)
		tmpstr = ostrcat("rewind1.png", NULL, 0, 0);
	else if(status.playspeed == -2)
		tmpstr = ostrcat("rewind2.png", NULL, 0, 0);
	else if(status.playspeed == -3)
		tmpstr = ostrcat("rewind3.png", NULL, 0, 0);
	else if(status.playspeed == -4)
		tmpstr = ostrcat("rewind4.png", NULL, 0, 0);
	else if(status.playspeed == -5)
		tmpstr = ostrcat("rewind5.png", NULL, 0, 0);
	else if(status.playspeed == -6)
		tmpstr = ostrcat("rewind6.png", NULL, 0, 0);
	else if(status.playspeed == -7)
		tmpstr = ostrcat("rewind7.png", NULL, 0, 0);

	if(tmpstr != NULL)
	{
		if(path != NULL && tmpstr != NULL)
			tmpstr = ostrcat("/", tmpstr, 0, 1);
		tmpstr = ostrcat(path, tmpstr, 0, 1);
	}
	
	return tmpstr;
}

char* getreccount(struct skin* node)
{
	char* tmpstr = NULL;

	if(status.recording > 0)
		tmpstr = oitoa(status.recording);

	return tmpstr;
}

char* getrec(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.recording > 0)
	{
		tmpstr = ostrcat("rec.png", NULL, 0, 0);

		if(path != NULL)
			tmpstr = ostrcat("/", tmpstr, 0, 1);
		tmpstr = ostrcat(path, tmpstr, 0, 1);
	}
	
	return tmpstr;
}

char* getcrypt(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		if(status.aktservice->channel->crypt > 0)
			tmpstr = ostrcat("crypt.png", NULL, 0, 0);
		else
			tmpstr = ostrcat("nocrypt.png", NULL, 0, 0);

		if(path != NULL)
			tmpstr = ostrcat("/", tmpstr, 0, 1);
		tmpstr = ostrcat(path, tmpstr, 0, 1);
	}
	
	return tmpstr;
}

char* getaspectmode(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	tmpstr = getaspect();
	if(ostrcmp(tmpstr, "4:3") == 0)
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, "4_3.png", 1, 0);
	}
	if(ostrcmp(tmpstr, "16:9") == 0)
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, "16_9.png", 1, 0);
	}

	if(path != NULL)
		tmpstr = ostrcat("/", tmpstr, 0, 1);
	tmpstr = ostrcat(path, tmpstr, 0, 1);

	return tmpstr;
}

char* getsdhd(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	videoreadqwidth(status.aktservice->videodev);

	if(getaktvideosize() == 0) //videosize is ok
	{
		if(status.videosize.w == 720)
			tmpstr = ostrcat("sd.png", NULL, 0, 0);
		else if(status.videosize.w == 1280)
			tmpstr = ostrcat("hdready.png", NULL, 0, 0);
		else if(status.videosize.w == 1920)
			tmpstr = ostrcat("hd.png", NULL, 0, 0);
	}
	
	if(tmpstr == NULL) tmpstr = ostrcat("novideo.png", NULL, 0, 0);

	if(path != NULL)
		tmpstr = ostrcat("/", tmpstr, 0, 1);
	tmpstr = ostrcat(path, tmpstr, 0, 1);

	return tmpstr;
}
	
char* getresolution(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(fb->width == 720 && fb->height == 576)
		tmpstr = ostrcat("576.png", NULL, 0, 0);
	else if(fb->width == 1280 && fb->height == 720)
		tmpstr = ostrcat("720.png", NULL, 0, 0);
	else if(fb->width == 1920 && fb->height == 1080)
		tmpstr = ostrcat("1080.png", NULL, 0, 0);

	if(path != NULL)
		tmpstr = ostrcat("/", tmpstr, 0, 1);
	tmpstr = ostrcat(path, tmpstr, 0, 1);
	
	return tmpstr;
}

char* getteletext(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		if(status.aktservice->channel->txtpid > 0)
			tmpstr = ostrcat("teletext.png", NULL, 0, 0);
		else
			tmpstr = ostrcat("noteletext.png", NULL, 0, 0);

		if(path != NULL)
			tmpstr = ostrcat("/", tmpstr, 0, 1);
		tmpstr = ostrcat(path, tmpstr, 0, 1);
	}
	
	return tmpstr;
}

char* getsoundtype(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		switch(status.aktservice->channel->audiocodec)
		{
			case AC3:
				tmpstr = ostrcat("ac3.png", NULL, 0, 0);
				break;
			case MPEGA:
				tmpstr = ostrcat("mpega.png", NULL, 0, 0);
				break;
			case DTS:
				tmpstr = ostrcat("dts.png", NULL, 0, 0);
				break;
			case LPCM:
				tmpstr = ostrcat("lpcm.png", NULL, 0, 0);
				break;
			case AAC:
				tmpstr = ostrcat("aac.png", NULL, 0, 0);
				break;
			case AACHE:
				tmpstr = ostrcat("aache.png", NULL, 0, 0);
				break;
		}
		if(path != NULL)
			tmpstr = ostrcat("/", tmpstr, 0, 1);
		tmpstr = ostrcat(path, tmpstr, 0, 1);
	}
	
	return tmpstr;
}

char* getprovidername(struct skin* node)
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL && status.aktservice->channel->provider != NULL)
		tmpstr = ostrcat(status.aktservice->channel->provider->name, NULL, 0, 0);

	return tmpstr;
}

char* getsatname(struct skin* node)
{
	struct sat* satnode = NULL;
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL && status.aktservice->channel->transponder != NULL)
	{
		satnode = getsatbyorbitalpos(status.aktservice->channel->transponder->orbitalpos);
		if(satnode != NULL)
			tmpstr = ostrcat(satnode->name, NULL, 0, 0);
	}

	return tmpstr;
}

char* getber(struct skin* node, char* format)
{
	uint32_t ber = 0;
	char* buf = NULL, *buf1 = NULL;

	ber = fereadber(status.aktservice->fedev);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	
	snprintf(buf, MINMALLOC, format, ber);
	buf1 = ostrcat(buf, NULL, 1, 0);

	return buf1;
}

char* getsignal(struct skin* node, char* format)
{
	uint16_t signal = 0;
	char* buf = NULL, *buf1 = NULL;

	signal = fereadsignalstrength(status.aktservice->fedev);
	signal = (signal * 100) / 0xffff;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	
	snprintf(buf, MINMALLOC, format, signal);
	buf1 = ostrcat(buf, NULL, 1, 0);

	return buf1;
}

char* getunc(struct skin* node, char* format)
{
	uint32_t unc = 0;
	char* buf = NULL, *buf1 = NULL;

	unc = fereaduncorrectedblocks(status.aktservice->fedev);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	
	snprintf(buf, MINMALLOC, format, unc);
	buf1 = ostrcat(buf, NULL, 1, 0);

	return buf1;
}

char* getsnr(struct skin* node, char* format)
{
	uint16_t snr = 0;
	char* buf = NULL, *buf1 = NULL;

	
	snr = fereadsnr(status.aktservice->fedev);
	snr = (snr * 100) / 0xffff;
	
	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	
	snprintf(buf, MINMALLOC, format, snr);
	buf1 = ostrcat(buf, NULL, 1, 0);

	return buf1;
}

char* getepgtimeline(struct skin* node, int akt)
{	
	struct epg* epgnode = NULL;
	struct channel* chnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;
	int proz = 0;

	if(akt == 0) chnode = status.markedchannel;
	if(akt == 1) chnode = status.aktservice->channel;
	if(akt == 2 && node != NULL) chnode = (struct channel*)node->handle;

	if(chnode != NULL)
	{
		epgnode = getepgakt(chnode);
		if(epgnode != NULL)
		{
			proz = (time(NULL) - epgnode->starttime) * 100 / (epgnode->endtime - epgnode->starttime);
			tmpnr = oitoa(proz);
			tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
			return tmpstr;
		}
	}
	return NULL;
}

char* getepgmarkedtimeline(struct skin* node)
{
	return getepgtimeline(node, 0);
}

char* getepgakttimeline(struct skin* node)
{
	return getepgtimeline(node, 1);
}

char* getepgchanneltimeline(struct skin* node)
{
	return getepgtimeline(node, 2);
}

char* getaktchannelname(struct skin* node)
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		tmpstr = ostrcat(tmpstr, status.aktservice->channel->name, 1, 0);
		return tmpstr;
	}
	return NULL;
}

char* getepgtime(struct skin* node, char* format, int akt, int type)
{
	// akt 0 = marked channel
	// akt 1 = akt channel now
	// akt 2 = akt channel next

	// type 0 = starttime
	// type 1 = endtime
	// type 2 = remainingtime

	debug(1000, "in");
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	struct channel* chnode = NULL;
	char *buf = NULL, *buf1 = NULL;
	time_t min = 0;
	
	if(format == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	if(akt == 0) chnode = status.markedchannel;
	if(akt == 1 || akt == 2) chnode = status.aktservice->channel;

	if(chnode != NULL)
	{
		epgnode = getepgakt(chnode);

		if(akt == 2)
		{
			if(epgnode != NULL)
				epgnode = epgnode->next;
		}

		if(epgnode != NULL)
		{
			if(type == 0)
				loctime = olocaltime(&epgnode->starttime);
			else if(type == 1)
				loctime = olocaltime(&epgnode->endtime);
			else if(type == 2)
			{
				if(akt == 1)
					min = (epgnode->endtime - (time(NULL) - 60)) / 60;
				else if(akt == 2)
					min = (epgnode->endtime - epgnode->starttime) / 60;
				if(min < 0) min = 0;
			}

			buf = malloc(MINMALLOC);
			if(buf == NULL)
			{
				free(loctime);
				err("no memory");
				return NULL;
			}

			if(type == 2)
				snprintf(buf, MINMALLOC, format, min);
			else if(loctime != NULL)
				ostrftime(buf, MINMALLOC, format, loctime);
			buf1 = ostrcat(buf, NULL, 1, 0);

			free(loctime); loctime = NULL;
			debug(1000, "out");
			return buf1;
		}
	}
	debug(1000, "out");
	return NULL;
}

char* getepgakttimeremaining(struct skin* node, char* format)
{
	return getepgtime(node, format, 1, 2);
}

char* getepgnexttimeremaining(struct skin* node, char* format)
{
	return getepgtime(node, format, 2, 2);
}

char* getepgmarkedstart(struct skin* node, char* format)
{
	return getepgtime(node, format, 0, 0);
}

char* getepgmarkedend(struct skin* node, char* format)
{
	return getepgtime(node, format, 0, 1);
}

char* getepgaktstart(struct skin* node, char* format)
{
	return getepgtime(node, format, 1, 0);
}

char* getepgaktend(struct skin* node, char* format)
{
	return getepgtime(node, format, 1, 1);
}

char* getepgnextstart(struct skin* node, char* format)
{
	return getepgtime(node, format, 2, 0);
}

char* getepgnextend(struct skin* node, char* format)
{
	return getepgtime(node, format, 2, 1);
}

char* getepgaktsubtitle(struct skin* node)
{
	struct epg* epgnode = NULL;
	char* tmpstr = NULL;

	epgnode = getepgakt(status.aktservice->channel);
	if(epgnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, epgnode->subtitle, 1, 0);
		return tmpstr;
	}

	return NULL;
}

char* getepgakttitle(struct skin* node)
{
	struct epg* epgnode = NULL;
	char* tmpstr = NULL;

	epgnode = getepgakt(status.aktservice->channel);
	if(epgnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
		return tmpstr;
	}

	return NULL;
}

char* getepgnexttitle(struct skin* node)
{
	struct epg* epgnode = NULL;
	char* tmpstr = NULL;

	epgnode = getepgakt(status.aktservice->channel);
	if(epgnode != NULL)
	{
		epgnode = epgnode->next;
		if(epgnode != NULL)
		{
			tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
			return tmpstr;
		}
	}

	return NULL;
}

char* getepgaktdesc(struct skin* node)
{
	struct epg* epgnode = NULL;

	if(status.aktservice->channel != NULL)
	{
		epgnode = getepgakt(status.aktservice->channel);
		if(epgnode != NULL)
			return epgdescunzip(epgnode);
	}

	return NULL;
}

char* getepgmarkeddesc(struct skin* node)
{
	struct epg* epgnode = NULL;

	if(status.markedchannel != NULL)
	{
		epgnode = getepgakt(status.markedchannel);
		if(epgnode != NULL)
			return epgdescunzip(epgnode);
	}

	return NULL;
}

char* getepgmarkedlist(struct skin* node, char* ccount)
{
	struct epg* epgnode = NULL;
	char* tmpstr = NULL, *buf = NULL;
	int count = atoi(ccount);
	struct tm* loctime = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(status.markedchannel != NULL)
	{
		epgnode = getepgakt(status.markedchannel);
		while(epgnode != NULL)
		{
			epgnode = epgnode->next;
			if(epgnode != NULL)
			{
				loctime = olocaltime(&epgnode->starttime);
				if(loctime != NULL)
					strftime(buf, MINMALLOC, "%H:%M", loctime);
				free(loctime); loctime = NULL;
				tmpstr = ostrcat(tmpstr, buf, 1, 0);
				tmpstr = ostrcat(tmpstr, " ", 1, 0);
				tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				count--;
			}
			if(count < 1) break;
		}
	}

	free(buf);
	return tmpstr;
}

char* getepgpicon(struct skin* node)
{
	char* tmpstr = NULL;
	
	if(status.epgchannel != NULL)
		tmpstr = createpiconpath(status.epgchannel, 0);
	return tmpstr;
}

char* getpicon(struct skin* node)
{
	char* tmpstr = NULL;
	tmpstr = createpiconpath(status.aktservice->channel, 0);
	return tmpstr;
}

char* getalternatepicon(struct skin* node)
{
	char* tmpstr = NULL;
	tmpstr = createpiconpath(status.aktservice->channel, 1);
	return tmpstr;
}

char* gettime(struct skin* node, char* format)
{
	debug(1000, "in");
	time_t sec;
	struct tm *loctime = NULL;
	char *buf = NULL, *buf1 = NULL;

	if(format == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sec = time(NULL);
	loctime = olocaltime(&sec);

	if(loctime != NULL)
	{
		ostrftime(buf, MINMALLOC, format, loctime);
		buf1 = ostrcat(buf, NULL, 1, 0);
	}

	free(loctime);
	debug(1000, "out");
	return buf1;
}

char* getchannelnr(struct skin* node, struct channel* chnode)
{
	char *tmpstr = NULL, *tmpnr = NULL;

	if(chnode == NULL) chnode = status.aktservice->channel;

	if(status.servicetype == 0)
		tmpstr = getconfig("channellist", NULL);
	else
		tmpstr = getconfig("rchannellist", NULL);
	if(ostrncmp("(BOUQUET)-", tmpstr, 10) == 0 && strlen(tmpstr) > 10)
	{
		struct mainbouquet* mainbouquetnode = NULL;
		int count = 0;

		mainbouquetnode = getmainbouquet(tmpstr + 10);
		if(mainbouquetnode != NULL && mainbouquetnode->bouquet != NULL)
		{
			struct bouquet* bnode = mainbouquetnode->bouquet;

			while(bnode != NULL)
			{
				if(bnode->channel == chnode)
				{
					tmpnr = oitoa(mainbouquetnode->bouquet->nr + count);
					return tmpnr;
				}
				count++;
				bnode = bnode->next;
			}
		}
		return tmpnr;
	} 
	else
		return tmpnr;
}

char* getakttuner(struct skin* node)
{
	char* tmpstr = NULL;

	if(status.aktservice->fedev != NULL)
	{
		tmpstr = oitoa(status.aktservice->fedev->devnr);
		return tmpstr;
	}

	return NULL;
}

char* getchannellistname(struct skin* node)
{
	char* tmpstr = NULL, *tmpstr1 = NULL;

	if(status.servicetype == 0)
                tmpstr = getconfig("channellist", NULL);
        else
                tmpstr = getconfig("rchannellist", NULL);

	if(tmpstr != NULL)
	{
		if(ostrncmp("(BOUQUET)-", tmpstr, 10) == 0 && strlen(tmpstr) > 10)
			tmpstr1 = ostrcat(tmpstr1, tmpstr + 10, 1, 0);

		else if(ostrncmp("(A-Z)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
			tmpstr1 = ostrcat(tmpstr1, tmpstr + 6, 1, 0);

		else if(ostrncmp("(SAT)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
			tmpstr1 = ostrcat(tmpstr1, tmpstr + 6, 1, 0);

		else if(ostrncmp("(PROVIDER)-", tmpstr, 11) == 0 && strlen(tmpstr) > 6)
			tmpstr1 = ostrcat(tmpstr1, tmpstr + 11, 1, 0);
	}

	return tmpstr1;
}

char* getpowerofftime(struct skin* node, char* format)
{
	char* buf = NULL, *buf1 = NULL;
	int resttime = 0;

	if(format == NULL) return NULL;

	if(status.sd_timer != NULL && status.sd_timer->active)
	{
		buf = malloc(MINMALLOC);
		if(buf == NULL)
		{
			err("no mem");
			return NULL;
		}

		resttime = (status.sd_timer->shutdown_time - time(NULL)) / 60;
		if(resttime < 0) resttime = 0;
		snprintf(buf, MINMALLOC, format, resttime);
		buf1 = ostrcat(buf, NULL, 1, 0);
	}

	return buf1;
}

char* gettvpic(struct skin* node, char* pos)
{
	struct splitstr* ret = NULL;
	char* tmpstr = NULL;
	int count = 0, left = 0, top = 0, width = 0;

	if(pos != NULL)
	{
		tmpstr = ostrcat(pos, NULL, 0, 0);
		ret = strsplit(tmpstr, ":", &count);
		if(count == 3)
		{
			if((&ret[0])->part != NULL)
				left = atoi((&ret[0])->part);
			if((&ret[1])->part != NULL)
				top = atoi((&ret[1])->part);
			if((&ret[2])->part != NULL)
				width = atoi((&ret[2])->part);
			
			setvmpegrect(status.aktservice->videodev, left, top, width, 0);
		}
		free(tmpstr); tmpstr = NULL;
	}

	free(ret); ret = NULL;
	return NULL;
}

char* getrecfreesize(struct skin* node)
{
	int ret = 0;

	unsigned long long full = getfullspace(getconfig("rec_path", NULL)) / (1024 * 1024);
	unsigned long long free = getfreespace(getconfig("rec_path", NULL)) / (1024 * 1024);

	if(full > 0 && full > free) ret = ((full - free) * 100) / full;

	if(ret < 0) ret = 0;
	if(ret > 100) ret = 100;

	return oitoa(ret);
}

char* gethbbtv(struct skin* node, char* path)
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
    if(status.aktservice->channel->hbbtvurl != NULL)
      tmpstr = ostrcat("hbbtv_on.png", "", 0, 0);
    else
      tmpstr = ostrcat("hbbtv_off.png", "", 0, 0);

		if(path != NULL)
			tmpstr = ostrcat("/", tmpstr, 0, 1);
		tmpstr = ostrcat(path, tmpstr, 0, 1);
	}
	
	return tmpstr;
}

char* getimgname(struct skin* node)
{
	char* tmpstr = NULL;

	tmpstr = readfiletomem(getconfig("imagenamefile", NULL), 1);

	return tmpstr;
}

char* getoscamcaid(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "caid:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscampid(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "pid:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamprov(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "prov:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamreader(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "reader:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamfrom(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "from:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamprotocol(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "protocol:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamhops(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "hops:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamecmtime(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "ecm time:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamcw0(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "cw0:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamcw1(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "cw1:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamsystem(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "system:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getoscamsource(struct skin* node)
{
	char* buf = NULL, *tmpstr = NULL;

	buf = readfiletomem("/tmp/ecm.info", 1);
	tmpstr = ostrstr(buf, "source:");
	if(tmpstr != NULL)
	{
		tmpstr = stringreplacecharonce(tmpstr, '\n', '\0');
		tmpstr = ostrcat(tmpstr, NULL, 0, 0);
	}
	free(buf); buf = NULL;

	return tmpstr;
}

char* getemu(struct skin* node)
{
	char* tmpstr = NULL;
	tmpstr = string_newline(command("emu.sh infoname"));
	return tmpstr;
}

#endif
