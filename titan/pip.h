#ifndef PIP_H
#define PIP_H

//flag 0: channel
//flag 1: playback
//flag 3: same as 0 but don't check chnode
//flag 4: same as 0 but new tuning
//flag 5: same as 3 but new tuning
int pipstartreal(struct channel* chnode, char* pin, int flag)
{
	struct transponder* tpnode = NULL;
	struct dvbdev *fenode = NULL;
	struct dvbdev *dmxvideonode = NULL;
	struct dvbdev *videonode = NULL;
	int ret = 0, festatus = 1;
	unsigned char *patbuf = NULL;
	int checkpmt = 0, pincheck = 0, ageprotect = 0, tune = 0;
	struct epg* epgnode = NULL;

	m_lock(&status.servicemutex, 2);

	status.secondzap = 0;

	if(flag == 4 || flag == 5) tune = 1;
	if(flag == 4) flag = 0;

	if(flag == 0 && status.pipservice->type == CHANNEL && status.pipservice->channel != NULL && chnode == status.pipservice->channel)
	{
		m_unlock(&status.servicemutex, 2);
		return 20;
	}
	if(flag == 3 || flag == 5) flag = 0;

	if(chnode == NULL)
	{
		m_unlock(&status.servicemutex, 2);
		return 21;
	}
	
	ageprotect = getconfigint("ageprotect", NULL);
	if(ageprotect > 0) epgnode = getepgakt(chnode);
	if(chnode->protect > 0 || (epgnode != NULL && epgnode->parentalrating >= ageprotect))
	{
		pincheck = screenpincheck(1, pin);
		if(pincheck == 1)
		{
			m_unlock(&status.servicemutex, 2);
			return 22;
		}
	}

	//got transponder
	if(flag == 0)
	{
		if(chnode->transponder == NULL)
			tpnode = gettransponder(chnode->transponderid);
		else
			tpnode = chnode->transponder;
	}

	ret = pipstop(status.pipservice, 0);

	//can't stop service (timeshift ??)
	if(ret == 1)
	{
		m_unlock(&status.servicemutex, 2);
		return 22;
	}
	
	status.pipservice->transponder = tpnode;
	status.pipservice->channel = chnode;
	status.pipservice->type = CHANNEL;
	free(status.pipservice->channellist);
	status.pipservice->channellist = ostrcat(status.aktservice->channellist, NULL, 0, 0);
	

	//got frontend dev
	if(flag == 0)
	{
		fenode = fegetfree(tpnode, 0, NULL, NULL);
		if(fenode == NULL)
		{
			m_unlock(&status.servicemutex, 2);
			return 1;
		}

		status.pipservice->fedev = fenode;
		
		//frontend tune
		if(fenode->felasttransponder != tpnode || tune == 1)
		{
			if(fenode->feinfo->type == FE_QPSK)
			{
				feset(fenode, tpnode);
				fetunedvbs(fenode, tpnode);
			}
			else if(fenode->feinfo->type == FE_QAM)
				fetunedvbc(fenode, tpnode);
			else if(fenode->feinfo->type == FE_OFDM)
				fetunedvbt(fenode, tpnode);
			else
			{
				m_unlock(&status.servicemutex, 2);
				return 3;
			}
		}
	}
	else if(flag == 1)
	{
		fenode = fegetdummy();
		status.pipservice->fedev = fenode;
	}

	if(fenode == NULL)
	{
		m_unlock(&status.servicemutex, 2);
		return 1;
	}

	//check pmt if not all infos in channellist
	if(chnode->videopid == -1 || chnode->videocodec == -1)
	{
		//wait for tuner lock
		if(flag == 0)
		{
			if(fenode->felasttransponder != tpnode)
				festatus = fewait(fenode);
			else
				festatus = fegetunlock(fenode);

			if(debug_level == 200)	
			{
				fereadstatus(fenode);
				fegetfrontend(fenode);
			}
			if(festatus != 0)
			{
				m_unlock(&status.servicemutex, 2);
				return 2;
			}
		}

		checkpmt = 1;
		if(flag != 1 || (flag == 1 && chnode->pmtpid == 0))
		{
			patbuf = dvbgetpat(fenode, -1);
			if(patbuf == NULL) status.secondzap = 1;
		}
		free(status.pipservice->pmtbuf);
		status.pipservice->pmtbuf = NULL;
		status.pipservice->pmtlen = 0;
		if(patbuf != NULL)
			status.pipservice->pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, &status.pipservice->pmtlen, -1, 0);
		else if(chnode->pmtpid > 0)
			status.pipservice->pmtbuf = dvbgetpmt(fenode, NULL, chnode->serviceid, &chnode->pmtpid, &status.pipservice->pmtlen, -1, 1);

		if(status.pipservice->pmtbuf == NULL) status.secondzap = 2;
		dvbgetinfo(status.pipservice->pmtbuf, chnode);

		if(flag == 0) sendcapmt(status.pipservice, 0, 0);
		free(patbuf);
	}

	if(flag != 0) checkpmt = 1;

	//demux video start
	if(chnode->videopid > 0)
	{
		if(status.pipservice->dmxvideodev != NULL && status.pipservice->dmxvideodev->fd >= 0 && status.pipservice->dmxvideodev->adapter == fenode->adapter && status.pipservice->dmxvideodev->devnr == fenode->devnr)
			dmxvideonode = status.pipservice->dmxvideodev;
		else
		{
			dmxclose(status.pipservice->dmxvideodev, -1);
			dmxvideonode = dmxopen(fenode, 2);
			if(dmxsetbuffersize(dmxvideonode, getconfigint("dmxvideobuffersize", NULL)) != 0)
			{
				dmxclose(dmxvideonode, -1);
				dmxvideonode = NULL;
			}
			status.pipservice->dmxvideodev = dmxvideonode;
		}
		if(dmxvideonode != NULL)
		{
			if(dmxsetsource(dmxvideonode, fenode->fedmxsource) != 0)
			{
				dmxclose(dmxvideonode, -1);
				dmxvideonode = NULL;
			}
			if(dmxsetpesfilter(dmxvideonode, chnode->videopid, -1, DMX_OUT_DECODER, DMX_PES_VIDEO1, 0) != 0)
			{
				dmxclose(dmxvideonode, -1);
				dmxvideonode = NULL;
			}
		}
		else
			err("demux video dev not ok");
	}
	else
	{
		err("dmx videopid not valid (%d)", chnode->videopid);
		dmxclose(status.pipservice->dmxvideodev, -1);
	}

	status.pipservice->dmxvideodev = dmxvideonode;

	//video start
	if(dmxvideonode != NULL)
	{
		if(status.pipservice->videodev != NULL && status.pipservice->videodev->fd >= 0 && status.pipservice->videodev->adapter == fenode->adapter)
			videonode = status.pipservice->videodev;
		else
		{
			videoclose(status.pipservice->videodev, -1);
			videonode = videoopen(fenode->adapter, 1);
			status.pipservice->videodev = videonode;
		}
		if(videonode != NULL)
		{
			videoselectsource(videonode, VIDEO_SOURCE_DEMUX);
			if(chnode->videocodec == VC1)
			{
				videosetstreamtype(videonode, 1); //transportstream
				videosetencoding(videonode, chnode->videocodec);
			}
			else
				videosetstreamtype(videonode, chnode->videocodec);
			videoplay(videonode);
		}
		else
			err("can't get free video dev");
	}

	//check pmt if not done
	if(checkpmt == 0)
	{
		//wait for tuner lock
		if(flag == 0)
		{
			if(fenode->felasttransponder != tpnode)
				festatus = fewait(fenode);
			else
				festatus = fegetunlock(fenode);

			if(debug_level == 200)
			{
				fereadstatus(fenode);
				fegetfrontend(fenode);
			}
			if(festatus != 0)
			{
				m_unlock(&status.servicemutex, 2);
				return 2;
			}
		}
		checkpmt = 1;
		patbuf = dvbgetpat(fenode, -1);
		if(patbuf == NULL) status.secondzap = 3;
		free(status.pipservice->pmtbuf);
		status.pipservice->pmtbuf = NULL;
		status.pipservice->pmtlen = 0;
		if(patbuf != NULL)
			status.pipservice->pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, &status.pipservice->pmtlen, -1, 0);
		else if(chnode->pmtpid > 0)
			status.pipservice->pmtbuf = dvbgetpmt(fenode, NULL, chnode->serviceid, &chnode->pmtpid, &status.pipservice->pmtlen, -1, 1);

		if(status.pipservice->pmtbuf == NULL) status.secondzap = 4;
		if(dvbgetinfo(status.pipservice->pmtbuf, chnode) == 1)
		{
			//audio or video pid or codec changed
			free(status.pipservice->pmtbuf);
			status.pipservice->pmtbuf = NULL;
			status.pipservice->pmtlen = 0;
		}

		if(flag == 0) sendcapmt(status.pipservice, 0, 0);
		free(patbuf);
	}
	if(flag == 0)
	{
		festatus = fewait(fenode);
		if(festatus != 0)
		{
			m_unlock(&status.servicemutex, 2);
			return 2;
		}
	}
	m_unlock(&status.servicemutex, 2);
	return 0;
}

int setvmpeg2(struct dvbdev* node, int value, int flag)
{
	debug(4440, "in");
	char* vmpegdev = NULL, *tmpstr = NULL, *buf = NULL;
	int ret = 0;

	if(getconfig("vmpegleftdev", NULL) == NULL)
	{
		addconfig("vmpegleftdev", "/proc/stb/vmpeg/%d/dst_left");
		addconfig("vmpegtopdev", "/proc/stb/vmpeg/%d/dst_top");
		addconfig("vmpegwidthdev", "/proc/stb/vmpeg/%d/dst_width");
		addconfig("vmpegheightdev", "/proc/stb/vmpeg/%d/dst_height");
		addconfig("vmpegapplydev", "/proc/stb/vmpeg/%d/dst_apply");
	}
	
	if(node == NULL) return 1;
	if(flag == 0)  vmpegdev = getconfig("vmpegleftdev", NULL);
	if(flag == 1)  vmpegdev = getconfig("vmpegtopdev", NULL);
	if(flag == 2)  vmpegdev = getconfig("vmpegwidthdev", NULL);
	if(flag == 3)  vmpegdev = getconfig("vmpegheightdev", NULL);
	if(flag == 99) vmpegdev = getconfig("vmpegapplydev", NULL);

	if(vmpegdev != NULL)
	{
		buf = malloc(MINMALLOC);
		if(buf == NULL)
		{
			err("no mem");
			return 1;
		}
		
		tmpstr = malloc(10);
		if(tmpstr == NULL)
		{
			err("no mem");
			free(buf);
			return 1;
		}
		
		snprintf(buf, MINMALLOC, vmpegdev, node->devnr);
		snprintf(tmpstr, 10, "%x", value);
		debug(444, "set %s to %s", buf, tmpstr);
		status.tvpic = 1;
		ret = writesys(buf, tmpstr, 1);
		
		free(tmpstr);
		free(buf);
		return ret;
	}

	debug(4440, "out");
	return 0;
}

// flag = 0 --> nicht aktivieren
// flag = 1 --> aktiviere Einstellungen
int pippos(struct dvbdev* node, int dst_width, int dst_height, int dst_left, int dst_top, int flag)
{
	int ret = 0;
	
	if(node == NULL) return 1;
	
	ret = setvmpeg2(node, dst_left, 0);
	ret = setvmpeg2(node, dst_top, 1);
	ret = setvmpeg2(node, dst_width, 2);
	ret = setvmpeg2(node, dst_height, 3);
	
	if(flag == 1) ret = setvmpeg2(node, 0, 99);
	
	return ret;
}


//second zap on failure
//TODO: use flag 6 (same as 5 but no pin question on second tune)
int pipstart(struct channel* chnode, char* pin, int flag)
{
	int ret = 0;

	int dst_width = getconfigint("pip_dst_width", NULL);
	int dst_height = getconfigint("pip_dst_height", NULL);
	int dst_left = getconfigint("pip_dst_left", NULL);
	int dst_top = getconfigint("pip_dst_top", NULL);

	if(dst_width == 0) dst_width = 180;
	if(dst_height == 0) dst_height = 144;
	if(dst_left == 0) dst_left = 505;
	if(dst_top == 0) dst_top = 36;

	if(status.pipservice->type == CHANNEL)
		pipstop(status.pipservice, 1);
	
	ret = pipstartreal(chnode, pin, flag);
	
	if(status.secondzap != 0 && ret == 0 && (flag == 0 || flag > 2))
	{
		debug(200, "first zap not ok, make second zap (%d)", status.secondzap);
		ret = pipstartreal(chnode, pin, 5);
	}

	if(ret == 0)
	{
		status.pipservice->fedev->felock++;
		deltranspondertunablestatus();
		ret = pippos(status.pipservice->videodev, dst_width, dst_height, dst_left, dst_top, 1);
		status.pipzap = getconfigint("pip_zap", NULL);
	}

	return ret;
}

int pipstop(struct service *node, int flag)
{

	if(node != NULL)
	{
		if(node->dmxvideodev != NULL)
		{
			node->fedev->felock--;
			deltranspondertunablestatus();
			if(node->type != NOTHING && node->type != STILLPIC) caservicedel(node, NULL);
		}

		node->type = NOTHING;

		videostop(node->videodev, 1);
		videoclose(node->videodev, -1);
		
		if(flag == 0 && checkbox("DM900") != 1 && checkbox("DM920") != 1)
			pippos(node->videodev, 0, 0, 0, 0, 1);
		
		node->videodev = NULL;
		
		if(node->dmxvideodev != NULL)
		{
			dmxstop(node->dmxvideodev);
			dmxclose(node->dmxvideodev, -1);
			node->dmxvideodev = NULL;
		}
		status.pipzap = 0;		
		return 0;
	}
	return 1;
}

int piphdmi(struct service *node, int flag)
{
	struct dvbdev *videonode = NULL;
	
	if(node != NULL && node->type != NOTHING && node->type != HDMIIN)
		pipstop(status.pipservice, 1);
	
	videonode = videoopen(0, 1);
	node->videodev = videonode;
	node->type = HDMIIN;
	
	if(videonode != NULL)
	{
		videoselectsource(videonode, VIDEO_SOURCE_HDMI);
		videosetstreamtype(videonode, 0);
		videoplay(videonode); 
	}
	status.pipzap = 0;
	
	return 0;
}

int pipswap(struct service *node)
{
	int hdmi = 0;
	char* tmpstr = NULL;
	struct channel* chnodeP = node->channel;
	struct channel* chnodeT = status.aktservice->channel;
	if(node->type == CHANNEL && chnodeP != NULL && chnodeP != chnodeT)
	{
		if(status.aktservice->type == HDMIIN)
		{
			hdmi = 1;
			pipstop(node, 1);
		}
		else
			pipstop(node, 0);
		tmpstr = ostrcat(node->channellist, NULL, 0, 0);
		servicecheckret(servicestart(chnodeP, tmpstr, NULL, 0), 0);
		free(tmpstr); tmpstr = NULL;
		if(hdmi == 1)
		{
			piphdmi(node, 0);
			free(node->channellist);
			node->channellist = NULL;
			node->channellist = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		}
		else
		{
			status.aktservice->fedev->felock++;
			pipstart(chnodeT, NULL, 0);
			status.aktservice->fedev->felock--;
			free(node->channellist);
			node->channellist = NULL;
			node->channellist = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		}
	}
#ifdef MIPSEL
	else if(node->type == HDMIIN)
	{
		pipstop(node, 0);
		servicestop(status.aktservice, 1, 1);
		servicefullHDMIin_start();
		pipstart(chnodeT, NULL, 0);
		free(node->channellist);
		node->channellist = ostrcat(status.lastservice->channellist, NULL, 0, 0);
	}
#endif
	else
	{
		return 1;
	}
	return 0;
}
	
	

void pipmenu()
{
	struct skin* pipscreen = getscreen("pipscreen");
	struct skin* hdmi = getscreennode(pipscreen, "hdmi");
	struct skin* b3 = getscreennode(pipscreen, "b3");
	int rcret = 0;
	
	int dst_width = getconfigint("pip_dst_width", NULL);
	int dst_height = getconfigint("pip_dst_height", NULL);
	int dst_left = getconfigint("pip_dst_left", NULL);
	int dst_top = getconfigint("pip_dst_top", NULL);

	if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
	{
		b3->hidden = YES;
		hdmi->hidden = YES;
	}
	
	if(dst_width == 0)
	{
		dst_width = 180;
		dst_height = 144;
	}
	if(dst_height == 0)
	{
		dst_width = 180;
		dst_height = 144;
	}
	if(dst_left == 0) dst_left = 505;
	if(dst_top == 0) dst_top = 36;
		
	if(status.aktservice->type == HDMIIN)
		changetext(hdmi, "no function");
	else if(status.pipservice->type == HDMIIN)
		changetext(hdmi, "live TV");
	else
		changetext(hdmi, "HDMI-in");
	
	while(1)
	{
		drawscreen(pipscreen, 0, 0);
		rcret = waitrc(pipscreen, 0, 0);
		clearscreen(pipscreen);
		
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			pipstop(status.pipservice, 0);
			break;
		}
			
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigint("pip_dst_width", dst_width);
			addconfigint("pip_dst_height", dst_height);
			addconfigint("pip_dst_left", dst_left);
			addconfigint("pip_dst_top", dst_top);
			break;
		}
		if(checkbox("DM900") != 1 && checkbox("DM920") != 1 && checkbox("DM520") != 1 && checkbox("DM525") != 1)
		{
			if(rcret == getrcconfigint("rcred", NULL))
			{
				if(status.aktservice->type == HDMIIN)
					continue;
				if(status.pipservice->type == HDMIIN)
				{
					pipstop(status.pipservice, 1);
					pipstart(status.aktservice->channel, NULL, 0);
					changetext(hdmi, "HDMI-in");
				}
				else
				{
					piphdmi(status.pipservice, 0);
					changetext(hdmi, "live TV");
				}
				continue;
			}
		}
		
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			pipswap(status.pipservice);
			if(status.aktservice->type == HDMIIN)
				changetext(hdmi, "no function");
			else if(status.pipservice->type != HDMIIN)
				changetext(hdmi, "HDMI-in");
			else
				changetext(hdmi, "live TV");
			continue;
		}
			
		if(rcret == getrcconfigint("rcdown", NULL))
		{
			if(dst_top+1 < 576-dst_height) dst_top++;
		}
		if(rcret == getrcconfigint("rcup", NULL))
		{
			if(dst_top-1 > 0) dst_top--;
		}
		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(dst_left-1 > 0) dst_left--;
		}
		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(dst_left+1 < 720-dst_width) dst_left++;
		}
		
		if(rcret == getrcconfigint("rcchdown", NULL))
		{
			if(dst_width-4 > 10)
			{
				dst_width = dst_width - 4;
				dst_height = 576 * dst_width / 720;
			}
		}
		if(rcret == getrcconfigint("rcchup", NULL))
		{
			if(dst_width+4 < 720)
			{
				dst_width = dst_width + 4;
				dst_height = 576 * dst_width / 720;
				
				if(dst_width + dst_left > 720)
					dst_left = 720 - dst_width;
					
				if(dst_height + dst_top > 576)
					dst_top = 576 - dst_height;
			}
		}
		pippos(status.pipservice->videodev, dst_width, dst_height, dst_left, dst_top, 1);
	}
}
	
void pipsetup()
{
	struct skin* pipmenu = getscreen("pipsetup");
	struct skin* kchupdown = getscreennode(pipmenu, "pipzap");
	struct skin* kpip = getscreennode(pipmenu, "pip");
	struct skin* k0 = getscreennode(pipmenu, "null");
	
	struct skin* listbox = getscreennode(pipmenu, "listbox");
	struct skin* tmp = NULL;

	int rcret = 0;
	
	addchoicebox(k0, "0", _("default"));
	addchoicebox(k0, "1", _("->swap<-"));
	setchoiceboxselection(k0, getconfig("pip_swap", NULL));
	
	addchoicebox(kpip, "0", _("pip menu"));
	addchoicebox(kpip, "1", _("pip end"));
	setchoiceboxselection(kpip, getconfig("pip_screen", NULL));
	
	addchoicebox(kchupdown, "0", _("default"));
	addchoicebox(kchupdown, "1", _("pip zap"));
	setchoiceboxselection(kchupdown, getconfig("pip_zap", NULL));
	
	drawscreen(pipmenu, 0, 0);
	addscreenrc(pipmenu, listbox);
	tmp = listbox->select;
	
	while(1)
	{
		addscreenrc(pipmenu, tmp);
		rcret = waitrc(pipmenu, 0, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcok", NULL))	
		{
			addconfig("pip_swap", k0->ret);
			addconfig("pip_screen", kpip->ret);
			addconfig("pip_zap", kchupdown->ret);
			break;
		}
		drawscreen(pipmenu, 0, 0);
	}
	delownerrc(pipmenu);
	clearscreen(pipmenu);
}


#endif
