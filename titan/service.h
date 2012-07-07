#ifndef SERVICE_H
#define SERVICE_H

void debugservice()
{
	struct service* node = service;
	int count = 0;

	while(node != NULL)
	{
		printf("Service %d\n", count);
		count++;

		if(node->channel != NULL) printf("%s (%lu)\n", node->channel->name, node->channel->transponderid);
		if(node->transponder != NULL) printf("TransponderID %lu\n", node->transponder->id);
		if(node->fedev != NULL) printf("%s (%d)\n", node->fedev->dev, node->fedev->fd);
		if(node->dmxaudiodev != NULL) printf("%s (%d)\n", node->dmxaudiodev->dev, node->dmxaudiodev->fd);
		if(node->dmxvideodev != NULL) printf("%s (%d)\n", node->dmxvideodev->dev, node->dmxvideodev->fd);
		if(node->dmxpcrdev != NULL) printf("%s (%d)\n", node->dmxpcrdev->dev, node->dmxpcrdev->fd);
		if(node->audiodev != NULL) printf("%s (%d)\n", node->audiodev->dev, node->audiodev->fd);
		if(node->videodev != NULL) printf("%s (%d)\n", node->videodev->dev, node->videodev->fd);
		node = node->next;
	}
}

void serviceresetchannelinfo(struct channel* chnode)
{
	freeaudiotrack(chnode);
	freesubtitle(chnode);
	freelinkedchannel(chnode);
	freepmt(chnode);
	freecadesc(chnode);
	freeesinfo(chnode);
	chnode->txtpid = 0;
	chnode->pcrpid = 0;
	chnode->pmtpid = 0;
	chnode->crypt = 0;
}

void akttolast()
{
	if(status.aktservice->fedev != NULL && status.aktservice->fedev->type == FRONTENDDEVDUMMY) return;
	status.lastservice->fedev = status.aktservice->fedev;
	status.lastservice->dmxaudiodev = status.aktservice->dmxaudiodev;
	status.lastservice->dmxvideodev = status.aktservice->dmxvideodev;
	status.lastservice->dmxpcrdev = status.aktservice->dmxpcrdev;
	status.lastservice->dmxsubtitledev = status.aktservice->dmxsubtitledev;
	status.lastservice->audiodev = status.aktservice->audiodev;
	status.lastservice->videodev = status.aktservice->videodev;
	status.lastservice->transponder = status.aktservice->transponder;
	status.lastservice->channel = status.aktservice->channel;
	free(status.lastservice->channellist);
	status.lastservice->channellist = ostrcat(status.aktservice->channellist, NULL, 0, 0);
}

//flag 0: channel
//flag 1: playback
//flag 2: timeshift
//flag 3: same as 0 but don't check chnode
//flag 4: same as 0 but new tuning
//flag 5: same as 3 but new tuning
int servicestart(struct channel* chnode, char* channellist, char* pin, int flag)
{
	debug(1000, "in");
	struct transponder* tpnode = NULL;
	struct dvbdev *fenode = NULL;
	struct dvbdev *dmxaudionode = NULL;
	struct dvbdev *dmxvideonode = NULL;
	//struct dvbdev *dmxpcrnode = NULL;
	struct dvbdev *audionode = NULL;
	struct dvbdev *videonode = NULL;
	int ret = 0, festatus = 1, tmpmute = 0;
	unsigned char *patbuf = NULL;
	int checkpmt = 0, pincheck = 0, stopflag = 0, ageprotect = 0, tune = 0;
	struct epg* epgnode = NULL;

	m_lock(&status.servicemutex, 2);

	if(flag == 4 || flag == 5) tune = 1;
	if(flag == 4) flag = 0;

	if(flag == 0 && status.aktservice->type == CHANNEL && status.aktservice->channel != NULL && chnode == status.aktservice->channel)
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

	if(flag == 1 || flag == 2) stopflag = 2;
	if(getconfigint("nozapclear", NULL) == 1)
		ret = servicestop(status.aktservice, 0, stopflag);
	else
		ret = servicestop(status.aktservice, 1, stopflag);

	//can't stop service (timeshift ??)
	if(ret == 1)
	{
		m_unlock(&status.servicemutex, 2);
		return 22;
	}
	
	//reset channel info
	if(flag == 0) serviceresetchannelinfo(chnode);

	status.aktservice->transponder = tpnode;
	status.aktservice->channel = chnode;
	status.aktservice->type = CHANNEL;
	if(status.epgthread != NULL) status.epgthread->aktion = PAUSE;

	if(flag == 0 && status.aktservice->type == CHANNEL)
		changechannellist(chnode, channellist);

	//got frontend dev
	if(flag == 0)
	{
		fenode = fegetfree(tpnode, 0, NULL);
		if(fenode == NULL)
		{
			m_unlock(&status.servicemutex, 2);
			return 1;
		}

		status.aktservice->fedev = fenode;

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
	else if(flag == 1 || flag == 2)
	{
		fenode = fegetdummy();
		status.aktservice->fedev = fenode;
	}

	//check pmt if not all infos in channellist
	if(chnode->audiopid == -1 || chnode->videopid == -1 || chnode->audiocodec == -1 || chnode->videocodec == -1 || (getconfigint("av_ac3default", NULL) == YES && chnode->audiocodec != AC3))
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
		free(status.aktservice->pmtbuf);
		status.aktservice->pmtbuf = NULL;
		status.aktservice->pmtlen = 0;
		if(patbuf != NULL)
			status.aktservice->pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 0);
		else if(chnode->pmtpid > 0)
			status.aktservice->pmtbuf = dvbgetpmt(fenode, NULL, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 1);

		dvbgetinfo(status.aktservice->pmtbuf, chnode);

		if(flag == 0)
		{
			if(status.pmtmode == 1)
			{
				if(recordcheckcrypt(fenode, CHANNEL) == 0)
					dvbwritepmt(status.aktservice, status.aktservice->pmtbuf);
				else
					debug(200, "don't write pmt.tmp, another crypt channel use this frontend");
			}
			else
				sendcapmt(status.aktservice, 0, 0);
		}
		free(patbuf);
	}

	if(flag != 0) checkpmt = 1;

	//set mute for scat problem
	if(status.mute == 0)
	{
		tmpmute = 1;
		status.mute = 1;
		setmute(1);
	}

	//demux audio start
	if(chnode->audiopid > 0)
	{
		if(status.aktservice->dmxaudiodev != NULL && status.aktservice->dmxaudiodev->fd >= 0 && status.aktservice->dmxaudiodev->adapter == fenode->adapter && status.aktservice->dmxaudiodev->devnr == fenode->devnr)
			dmxaudionode = status.aktservice->dmxaudiodev;
		else
		{
			dmxclose(status.aktservice->dmxaudiodev, -1);
			dmxaudionode = dmxopen(fenode);
			if(dmxsetbuffersize(dmxaudionode, getconfigint("dmxaudiobuffersize", NULL)) != 0)
			{
				dmxclose(dmxaudionode, -1);
				dmxaudionode = NULL;
			}
		}
		if(dmxaudionode != NULL)
		{
			if(dmxsetsource(dmxaudionode, fenode->fedmxsource) != 0)
			{
				dmxclose(dmxaudionode, -1);
				dmxaudionode = NULL;
			}
			if(dmxsetpesfilter(dmxaudionode, chnode->audiopid, -1, DMX_OUT_DECODER, DMX_PES_AUDIO, 0) != 0)
			{
				dmxclose(dmxaudionode, -1);
				dmxaudionode = NULL;
			}
		}
		else
			err("demux audio dev not ok");
	}
	else
	{
		err("dmx audiopid not valid (%d)", chnode->audiopid);
		dmxclose(status.aktservice->dmxaudiodev, -1);
	}

	status.aktservice->dmxaudiodev = dmxaudionode;

	//demux video start
	if(chnode->videopid > 0)
	{
		if(status.aktservice->dmxvideodev != NULL && status.aktservice->dmxvideodev->fd >= 0 && status.aktservice->dmxvideodev->adapter == fenode->adapter && status.aktservice->dmxvideodev->devnr == fenode->devnr)
			dmxvideonode = status.aktservice->dmxvideodev;
		else
		{
			dmxclose(status.aktservice->dmxvideodev, -1);
			dmxvideonode = dmxopen(fenode);
			if(dmxsetbuffersize(dmxvideonode, getconfigint("dmxvideobuffersize", NULL)) != 0)
			{
				dmxclose(dmxvideonode, -1);
				dmxvideonode = NULL;
			}
			status.aktservice->dmxvideodev = dmxvideonode;
		}
		if(dmxvideonode != NULL)
		{
			if(dmxsetsource(dmxvideonode, fenode->fedmxsource) != 0)
			{
				dmxclose(dmxvideonode, -1);
				dmxvideonode = NULL;
			}
			if(dmxsetpesfilter(dmxvideonode, chnode->videopid, -1, DMX_OUT_DECODER, DMX_PES_VIDEO, 0) != 0)
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
		dmxclose(status.aktservice->dmxvideodev, -1);
	}

	status.aktservice->dmxvideodev = dmxvideonode;

	//audio start
	if(dmxaudionode != NULL)
	{
		if(status.aktservice->audiodev != NULL && status.aktservice->audiodev->fd >= 0 && status.aktservice->audiodev->adapter == fenode->adapter) 
			audionode = status.aktservice->audiodev;
		else
		{
			audioclose(status.aktservice->audiodev, -1);
			audionode = audioopen(fenode->adapter);
			status.aktservice->audiodev = audionode;
		}
		if(audionode != NULL)
		{
			audioselectsource(audionode, AUDIO_SOURCE_DEMUX);
			//workaround for no audio on zap if old channel is mpeg audio and new
			//channel has only ac3 audio
			audiosetbypassmode(audionode, chnode->audiocodec);
			audioplay(audionode);
			audiostop(audionode);
			audioplay(audionode);
		}
		else
			err("can't get free audio dev");
	}
	
	//video start
	if(dmxvideonode != NULL)
	{
		if(status.aktservice->videodev != NULL && status.aktservice->videodev->fd >= 0 && status.aktservice->videodev->adapter == fenode->adapter)
			videonode = status.aktservice->videodev;
		else
		{
			videoclose(status.aktservice->videodev, -1);
			videonode = videoopen(fenode->adapter);
			status.aktservice->videodev = videonode;
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

	//unset mute if set here
	if(tmpmute == 1)
	{
		tmpmute = 0;
		status.mute = 0;
		setmute(0);
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
		free(status.aktservice->pmtbuf);
		status.aktservice->pmtbuf = NULL;
		status.aktservice->pmtlen = 0;
		if(patbuf != NULL)
			status.aktservice->pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 0);
		else if(chnode->pmtpid > 0)
			status.aktservice->pmtbuf = dvbgetpmt(fenode, NULL, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 1);

		if(dvbgetinfo(status.aktservice->pmtbuf, chnode) == 1)
		{
			//audio or video pid or codec changed
			free(status.aktservice->pmtbuf);
			status.aktservice->pmtbuf = NULL;
			status.aktservice->pmtlen = 0;
		}

		if(flag == 0)
		{
			if(status.pmtmode == 1)
			{
				if(recordcheckcrypt(fenode, CHANNEL) == 0)
					dvbwritepmt(status.aktservice, status.aktservice->pmtbuf);
				else
					debug(200, "don't write pmt.tmp, another crypt channel use this frontend");
			}
			else
				sendcapmt(status.aktservice, 0, 0);
		}
		free(patbuf);
	}

	//get ait and parse it for hbbtv url
	if(chnode->aitpid > 0)
	{
		unsigned char* aitbuf = NULL;
		aitbuf = dvbgetait(fenode, chnode->aitpid, 0, -1);
    if(aitbuf != NULL)
    {
      free(chnode->hbbtvurl); chnode->hbbtvurl = NULL;
      chnode->hbbtvurl = dvbgethbbtvurl(aitbuf);
    }

		debug(200, "hbbtvurl=%s", chnode->hbbtvurl);
		free(aitbuf); aitbuf = NULL;
	}

	//demux pcr start
	//i think this is not needed
	/*
	if(flag == 0 && chnode->pcrpid > 0)
	{
		if(status.aktservice->dmxpcrdev != NULL && status.aktservice->dmxpcrdev->fd >= 0 && status.aktservice->dmxpcrdev->adapter == fenode->adapter && status.aktservice->dmxpcrdev->devnr == fenode->devnr)
			dmxpcrnode = status.aktservice->dmxpcrdev;
		else
		{
			dmxclose(status.aktservice->dmxpcrdev, -1);
			dmxpcrnode = dmxopen(fenode);
		}
		if(dmxpcrnode != NULL)
		{
			if(dmxsetsource(dmxpcrnode, fenode->fedmxsource) != 0)
			{
				dmxclose(dmxpcrnode, -1);
				dmxpcrnode = NULL;
			}
			if(dmxsetpesfilter(dmxpcrnode, chnode->pcrpid, -1, DMX_OUT_DECODER, DMX_PES_PCR, 0) != 0)
			{
				dmxclose(dmxpcrnode, -1);
				dmxpcrnode = NULL;
			}
		}
		else
			err("demux pcr dev not ok");
	}
	else
	{
		err("dmx pcrpid not valid (%d)", chnode->pcrpid);
		dmxclose(status.aktservice->dmxpcrdev, -1);
	}

	status.aktservice->dmxpcrdev = dmxpcrnode;
	*/

	if(flag == 0)
	{
		//add channel to history
		if(status.aktservice->type == CHANNEL)
		{
			addchannelhistory(chnode, status.aktservice->channellist);
			if(status.servicetype == 0) //only for tv
				createmostzap(chnode->serviceid, chnode->transponderid);
		}
		festatus = fewait(fenode);
		if(festatus != 0)
		{
			m_unlock(&status.servicemutex, 2);
			return 2;
		}
	}

	if(status.epgthread != NULL)
	{
		int i = 0;
		while(status.epgthread->status != INPAUSE)
		{
			usleep(10000);
			i++; if(i > 200) break;
		}
		status.epgthread->aktion = START;
	}

	status.videosizevalid = time(NULL);
	m_unlock(&status.servicemutex, 2);
	
	//auto change channel name
	if(flag == 0 && getconfigint("autochangechannelname", NULL) == 1)
		addtimer(&autochangechannelname, START, 1000, 1, NULL, NULL, NULL);
	
	//autoresolution
	if(flag == 0 && ostrcmp(getconfig("av_videomode_autores", NULL), "auto") == 0)
	{
		int sec = 7;
		if(getconfig("av_videomode_autores_ts", NULL) != NULL)
			sec = atoi(getconfig("av_videomode_autores_ts", NULL));
		if(status.restimer == NULL)
			status.restimer = addtimer(&setaktres, START, 10000, 1, (void*)sec, NULL, NULL);
		else
		{
			status.restimer->aktion = STOP;
 			sleep(1);
 			status.restimer = addtimer(&setaktres, START, 10000, 1, (void*)sec, NULL, NULL);
		}			
	}
	
	debug(1000, "out");
	return 0;
}

struct service* getservicebychannel(struct channel* chnode)
{
	m_lock(&status.servicemutex, 2);
	struct service* snode = service;

	while(snode != NULL)
	{
		if(snode->channel == chnode)
		{
			m_unlock(&status.servicemutex, 2);
			return snode;
		}
		snode = snode->next;

	}
	m_unlock(&status.servicemutex, 2);
	return NULL;
}

struct service* getservicebyservice(struct service* node, int flag)
{
	if(flag == 0) m_lock(&status.servicemutex, 2);
	struct service* snode = service;

	while(snode != NULL)
	{
		if(snode != status.lastservice && snode != node && snode->channel == node->channel)
		{
			if(flag == 0) m_unlock(&status.servicemutex, 2);
			return snode;
		}
		snode = snode->next;

	}
	if(flag == 0) m_unlock(&status.servicemutex, 2);
	return NULL;
}

//flag 0: with mutex
//flag 1: without mutex
struct service* getservice(int type, int flag)
{
	if(flag == 0) m_lock(&status.servicemutex, 2);
	struct service* snode = service;

	while(snode != NULL)
	{
		if(snode->type == type)
		{
			if(flag == 0) m_unlock(&status.servicemutex, 2);
			return snode;
		}
		snode = snode->next;

	}
	if(flag == 0) m_unlock(&status.servicemutex, 2);
	return NULL;
}

//flag 0: faststop depends on param faststop
//flag 1: always normal stop
//flag 2: from timeshift/player
int servicestop(struct service *node, int clear, int flag)
{
	int rcret = 0;

	if(node != NULL)
	{
		if(status.timeshift == 1 && flag != 2)
		{
			rcret = textbox(_("Message"), _("Timeshift is running !!!\nStop it and switch ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 10, 0);

			if(rcret == 2) return 1;
			timeshiftstop(1);
		}
		if(flag != 2) caservicedel(node, NULL);

		truncate("/tmp/ecm.info", 0);
		unlink("/tmp/pid.info");
		unlink("/tmp/caids.info");
		
		status.videosizevalid = 0;

		if(status.epgthread != NULL) status.epgthread->aktion = PAUSE;
		subtitlestop(0);

		if(node->type == CHANNEL && flag != 2) akttolast();
		if(flag != 2) node->type = NOTHING;

		audiostop(node->audiodev);
		videostop(node->videodev, clear);
		
		int fastzap = getconfigint("fastzap", NULL);

		if(flag == 1 || (flag == 0 && (fastzap == 0 || fastzap == 2)))
		{
			audioclose(node->audiodev, -1);
			node->audiodev = NULL;
			dmxstop(node->dmxaudiodev);
			dmxclose(node->dmxaudiodev, -1);
			node->dmxaudiodev = NULL;
		}
		
		if(flag == 1 || (flag == 0 && fastzap == 0))
		{
			videoclose(node->videodev, -1);
			node->videodev = NULL;
			dmxstop(node->dmxvideodev);
			dmxclose(node->dmxvideodev, -1);
			node->dmxvideodev = NULL;
			dmxstop(node->dmxpcrdev);
			dmxclose(node->dmxpcrdev, -1);
			node->dmxpcrdev = NULL;
			dmxstop(node->dmxsubtitledev);
			dmxclose(node->dmxsubtitledev, -1);
			node->dmxsubtitledev = NULL;
		}
		return 0;
	}
	return 1;
}

void servicechangeaudio(struct channel* chnode, struct audiotrack* tracknode)
{
	if(chnode == NULL || tracknode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

	if(chnode->audiopid == tracknode->audiopid && chnode->audiocodec == tracknode->audiocodec)
		return;

	chnode->audiopid = tracknode->audiopid;
	chnode->audiocodec = tracknode->audiocodec;

	status.writechannel = 1;
	audiostop(status.aktservice->audiodev);
	audiosetbypassmode(status.aktservice->audiodev, chnode->audiocodec);
	//clear videobuffer on playback for syncing video / audio
	if(status.playing == 1) videoclearbuffer(status.aktservice->videodev);
	dmxsetpesfilter(status.aktservice->dmxaudiodev, chnode->audiopid, -1, DMX_OUT_DECODER, DMX_PES_AUDIO, 0);
	if(!(status.timeshift == 1 && status.playing == 0))
		audioplay(status.aktservice->audiodev);
}

struct service* addservice(struct service* last)
{
	debug(1000, "in");
	struct service *newnode = NULL, *node = NULL;

	newnode = (struct service*)calloc(1, sizeof(struct service));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->recdstfd = -1;
	newnode->recsrcfd = -1;
	newnode->tssize = 188;

	m_lock(&status.servicemutex, 2);
	node = service;
	if(node != NULL)
	{
		if(last == NULL)
		{
			while(node->next != NULL)
				node = node->next;
			node->next = newnode;
		}
		else
			last->next = newnode;
	}
	else
		service = newnode;

	debug(1000, "out");
	m_unlock(&status.servicemutex, 2);
	return newnode;
}

struct service* checkservice(struct service* node)
{
	struct service* snode = service;

	while(snode != NULL)
	{
		if(snode == node)
			return snode;
		snode = snode->next;
	}
	return NULL;
}

//flag 0: set mutex
//flag 1: not set mutex
void delservice(struct service* snode, int flag)
{
	debug(1000, "in");
	m_lock(&status.servicemutex, 2);
	struct service *node = service, *prev = service;
	struct rectimer *rectimernode = NULL;

	while(node != NULL)
	{
		if(node == snode)
		{
			if(node == service)
				service = node->next;
			else
				prev->next = node->next;

			dmxclose(node->dmxaudiodev, -1);
			dmxclose(node->dmxvideodev, -1);
			dmxclose(node->dmxpcrdev, -1);
			dmxclose(node->dmxsubtitledev, -1);
			audioclose(node->audiodev, -1);
			videoclose(node->videodev, -1);
			close(node->recdstfd);
			close(node->recsrcfd);
			caservicedel(node, NULL);

			//check if a rectimer is joined with a service
			if(node->type == RECORDTIMER)
			{
				if(flag == 0)
					m_lock(&status.rectimermutex, 1);

				rectimernode = getrectimerbyservice(node);
				if(rectimernode != NULL)
				{
					rectimernode->servicenode = NULL;
					if(rectimernode->status == 0 || rectimernode->status == 1)
					{
						rectimernode->status = 2;
						status.writerectimer = 1;
						writerectimer(getconfig("rectimerfile", NULL), 1);
					}
				}

				if(flag == 0)
					m_unlock(&status.rectimermutex, 1);
			}

			free(node->channellist);
			node->channellist = NULL;

			free(node->recname);
			node->recname = NULL;

			free(node->pmtbuf);
			node->pmtbuf = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	m_unlock(&status.servicemutex, 2);
	debug(1000, "out");
}

void freeservice()
{
	debug(1000, "in");
	struct service *node = service, *prev = service;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delservice(prev, 0);
	}
	debug(1000, "out");
}

char* servicecheckret(int ret, int flag)
{ 
	char* tmpstr = NULL;

	if(ret != 0)
	{
		switch(ret)
		{
			case 1:
				tmpstr = ostrcat(_("Can't find a Tuner.\nAlle Tuners in use or no Tuner defined."), NULL, 0, 0);
				break;
			case 2:
				tmpstr = ostrcat(_("Tuning to Channel failed!"), NULL, 0, 0);
				break;
			case 3:
				tmpstr = ostrcat(_("Can't open frontend dev or Frontend Type unknown!"), NULL, 0, 0);
				break;
			case 20:
				break;
			case 21:
				tmpstr = ostrcat(_("Channellist empty or corrupt (channel not found)!"), NULL, 0, 0);
				break;
			case 22:
				break;
		}
		if(tmpstr != NULL)
			textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
	}

	if(flag != 1)
	{
		free(tmpstr);
		tmpstr = NULL;
	}

	return tmpstr;
}

#endif
