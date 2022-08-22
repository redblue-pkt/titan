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

		if(node->channel != NULL) printf("%s (%llu)\n", node->channel->name, node->channel->transponderid);
		if(node->transponder != NULL) printf("TransponderID %llu\n", node->transponder->id);
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
	if(chnode == NULL) return;

	freeaudiotrack(chnode);
	freesubtitle(chnode);
	freepmt(chnode);
	freecadesc(chnode);
	freeesinfo(chnode);
	chnode->txtpid = 0;
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
//flag 6: same as 5 but second zap
int servicestartreal(struct channel* chnode, char* channellist, char* pin, int flag)
{
	struct transponder* tpnode = NULL;
	struct dvbdev *fenode = NULL;
	struct dvbdev *dmxaudionode = NULL;
	struct dvbdev *dmxvideonode = NULL;
	struct dvbdev *dmxpcrnode = NULL;
	struct dvbdev *audionode = NULL;
	struct dvbdev *videonode = NULL;
	int ret = 0, festatus = 1, tmpmute = 0, i = 0;
	unsigned char *patbuf = NULL;
	int checkpmt = 0, pincheck = 0, stopflag = 0, ageprotect = 0, tune = 0, secondzap = 0;
	struct epg* epgnode = NULL;

	m_lock(&status.servicemutex, 2);

	status.secondzap = 0;

	if(flag == 4 || flag == 5 || flag == 6) tune = 1;
	if(flag == 6) secondzap = 1;
	if(flag == 4) flag = 0;

	//wakeup hdd work 
	if(flag == 1 || flag == 2) wakeup_record_device(); 
	
	if(flag == 0 && status.aktservice->type == CHANNEL && status.aktservice->channel != NULL && chnode == status.aktservice->channel)
	{
		m_unlock(&status.servicemutex, 2);
		return 20;
	}
	if(flag == 3 || flag == 5 || flag == 6) flag = 0;

	if(chnode == NULL)
	{
		m_unlock(&status.servicemutex, 2);
		return 21;
	}
	debug(200, "servicestartreal... started");
	//stop running autoresolution
	if(status.restimer != NULL)
		status.restimer->aktion = STOP;

	if(secondzap == 0)
	{
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
	}

	//got transponder
	if(flag == 0)
	{
		if(chnode->transponder == NULL)
			tpnode = gettransponder(chnode->transponderid);
		else
			tpnode = chnode->transponder;
	}

	if(flag == 1 || flag == 2)
		stopflag = 2;
	else if(secondzap == 1)
		stopflag = 3;
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

    if(chnode->streamurl != NULL && chnode->epgurl != NULL)
    {
        printf("playerstart1 name: %s\n", chnode->name);
        printf("playerstart1 streamurl: %s\n", chnode->streamurl);
        printf("playerstart1 epgurl: %s\n", chnode->epgurl);
        addconfigtmp("playerbuffersize", "0");
        addconfigtmp("playerbufferseektime", "0");
        if(/*status.play != 2 && getconfigint("lastplayertype", NULL) == 0 && */checkbox("DM900") == 1)
            servicestop(status.aktservice, 1, 1);
        playerstart(chnode->streamurl);
        status.play = 2;
		delconfigtmp("playerbuffersize");
		delconfigtmp("playerbufferseektime");
    }

	//got frontend dev
	if(chnode->epgurl == NULL && flag == 0)
	{
		fenode = fegetfree(tpnode, 0, NULL, chnode);
		if(fenode == NULL)
		{
			m_unlock(&status.servicemutex, 2);
			return 1;
		}
		
#ifdef DREAMBOX
        if(chnode->streamurl == NULL)
        {
		    if(status.aktservice->fedev != fenode)
		    {
			    int fastzap = getconfigint("fastzap", NULL);
			    if(fastzap == 1)
			    {
				    audioclose(status.aktservice->audiodev, -1);
				    status.aktservice->audiodev = NULL;
				    dmxstop(status.aktservice->dmxaudiodev);
				    dmxclose(status.aktservice->dmxaudiodev, -1);
				    status.aktservice->dmxaudiodev = NULL;
			    }
			    if(fastzap == 1 || fastzap == 2)
			    {
				    videoclose(status.aktservice->videodev, -1);
				    status.aktservice->videodev = NULL;
				    dmxstop(status.aktservice->dmxvideodev);
				    dmxclose(status.aktservice->dmxvideodev, -1);
				    status.aktservice->dmxvideodev = NULL;
				    dmxstop(status.aktservice->dmxpcrdev);
				    dmxclose(status.aktservice->dmxpcrdev, -1);
				    status.aktservice->dmxpcrdev = NULL;
				    dmxstop(status.aktservice->dmxsubtitledev);
				    dmxclose(status.aktservice->dmxsubtitledev, -1);
				    status.aktservice->dmxsubtitledev = NULL;
			    }
		    }
        }
#endif
				
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

	if(chnode->streamurl == NULL && fenode == NULL)
	{
		m_unlock(&status.servicemutex, 2);
		return 1;
	}

	//check pmt if not all infos in channellist
	if((chnode->audiopid == -1 || chnode->videopid == -1 || chnode->pcrpid == -1 || chnode->audiocodec == -1 || chnode->videocodec == -1 || (getconfigint("av_ac3default", NULL) == YES && chnode->audiocodec != AC3)))
	{
		//wait for tuner lock
		if(chnode->streamurl == NULL && flag == 0)
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

		debug(200, "1-secondzap=%i", status.secondzap);
		free(status.aktservice->pmtbuf);
		status.aktservice->pmtbuf = NULL;
		status.aktservice->pmtlen = 0;
		if(patbuf != NULL)
			status.aktservice->pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 0);
		else if(chnode->pmtpid > 0)
			status.aktservice->pmtbuf = dvbgetpmt(fenode, NULL, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 1);

		if(status.aktservice->pmtbuf == NULL) status.secondzap = 2;
		debug(200, "2-secondzap=%i", status.secondzap);
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
#ifdef MIPSEL	
	if(status.mute != 0)
	{
		setmute(0);
		tmpmute = 1;
	}
#else
	if(status.mute == 0)
	{
		tmpmute = 1;
		//setmute(1);
		audiosetmute(status.aktservice->audiodev, 1);
	}
#endif

	if(chnode->epgurl == NULL)
    {
    	audiostop(status.aktservice->audiodev);

    	if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkchipset("HI3798MV200") == 1)
    		dmxstop(status.aktservice->dmxaudiodev);

// gost test multibox
    	if(checkchipset("HI3798MV200") == 1  || vubox1 == 1)
    	{
    		videostop(status.aktservice->videodev, 1);
    		dmxstop(status.aktservice->dmxvideodev);
    	}

    	//demux pcr start
    	if(flag == 0 && chnode->pcrpid > 0)
    	{
    		if(status.aktservice->dmxpcrdev != NULL && status.aktservice->dmxpcrdev->fd >= 0 && status.aktservice->dmxpcrdev->adapter == fenode->adapter && status.aktservice->dmxpcrdev->devnr == fenode->devnr)
    			dmxpcrnode = status.aktservice->dmxpcrdev;
    		else
    		{
    			dmxclose(status.aktservice->dmxpcrdev, -1);
    			dmxpcrnode = dmxopen(fenode, 2);
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

	    //demux audio start
	    if(chnode->audiopid > 0)
	    {
		    if(status.aktservice->dmxaudiodev != NULL && status.aktservice->dmxaudiodev->fd >= 0 && status.aktservice->dmxaudiodev->adapter == fenode->adapter && status.aktservice->dmxaudiodev->devnr == fenode->devnr)
			    dmxaudionode = status.aktservice->dmxaudiodev;
		    else
		    {
			    dmxclose(status.aktservice->dmxaudiodev, -1);
			    dmxaudionode = dmxopen(fenode, 2);
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
			    dmxvideonode = dmxopen(fenode, 2);
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
	
	//workaround for some audio channel not playing (for test)
    	usleep(100000);

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
			    audiosetbypassmode(audionode, chnode->audiocodec);
			    if(checkchipset("HI3798MV200") == 1 || vubox1 == 1) //fixt only audio no video.. blackscreen after zap
				    audiopause(audionode);
			    if(status.mute != 1)
			    {
				    // needs for ts playback
				    if(checkchipset("3798MV200") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
					    dmxstart(status.aktservice->dmxaudiodev);
				    audioplay(audionode);
			    }
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
			    videonode = videoopen(fenode->adapter, 0);
			    status.aktservice->videodev = videonode;
		    }
		    if(videonode != NULL)
		    {
			    videocontinue(videonode);
			    videoselectsource(videonode, VIDEO_SOURCE_DEMUX);
			    setencoding(chnode, videonode);

			    // needs for ts playpack
			    if(checkchipset("3798MV200") == 1)
				    dmxstart(status.aktservice->dmxvideodev);

			    if(vubox1 == 1 ) //fixt only audio no video.. blackscreen after zap
				    videofreeze(videonode);

			    if(videoplay(videonode)!= 0) {
				    usleep(500000);
				    videoplay(videonode);
			    }
		    }
		    else
			    err("can't get free video dev");
		    if(checkchipset("HI3798MV200") == 1 || vubox1 == 1)
		    {
			    videoslowmotion(videonode, 0);
			    videofastforward(videonode, 0);
			    videocontinue(videonode);
		    }
	    }
#ifdef MIPSEL
	    if(checkchipset("HI3798MV200") == 1 || vubox1 == 1)
	    {
		    audiocontinue(audionode);
	    }

	    if(tmpmute == 1)
	    {
		    tmpmute = 0;
		    setmute(1);
	    }
#else	
	    //unset mute if set here
	    if(tmpmute == 1)
	    {
		    tmpmute = 0;
		    audiosetmute(status.aktservice->audiodev, 0);
		    //setmute(0);
	    }
	    if(status.mute != 1)
	    {
		    if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
			    dmxstart(status.aktservice->dmxaudiodev);
		    audioplay(status.aktservice->audiodev);
	    }
#endif

        if(chnode->streamurl != NULL)
        {
            printf("playerstart2 name: %s\n", chnode->name);
            printf("playerstart2 streamurl: %s\n", chnode->streamurl);
            printf("playerstart2 epgurl: %s\n", chnode->epgurl);
            addconfigtmp("playerbuffersize", "0");
            addconfigtmp("playerbufferseektime", "0");
            if(/*status.play != 2 && getconfigint("lastplayertype", NULL) == 0 && */checkbox("DM900") == 1)
                servicestop(status.aktservice, 1, 1);
            playerstart(chnode->streamurl);
            status.play = 2;
			delconfigtmp("playerbuffersize");
			delconfigtmp("playerbufferseektime");
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
			    {
				    festatus = fegetunlock(fenode);
				    if(festatus != 0)
				    {
					    debug(200, "fegetunlock rc:%d ... now fewait", festatus);	
					    festatus = fewait(fenode);
				    }
			    }

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
		    debug(200, "3-secondzap=%i", status.secondzap);	
		    free(status.aktservice->pmtbuf);
		    status.aktservice->pmtbuf = NULL;
		    status.aktservice->pmtlen = 0;
		    if(patbuf != NULL)
			    status.aktservice->pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 0);
		    else if(chnode->pmtpid > 0)
			    status.aktservice->pmtbuf = dvbgetpmt(fenode, NULL, chnode->serviceid, &chnode->pmtpid, &status.aktservice->pmtlen, -1, 1);

		    if(status.aktservice->pmtbuf == NULL) status.secondzap = 4;
		    debug(200, "4-secondzap=%i", status.secondzap);
		    if(dvbgetinfo(status.aktservice->pmtbuf, chnode) == 1)
		    {
			    //audio or video pid or codec changed
			    free(status.aktservice->pmtbuf);
			    status.aktservice->pmtbuf = NULL;
			    status.aktservice->pmtlen = -1;
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
	    if(flag == 0 && chnode->aitpid > 0)
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
    }

	if(flag == 0)
	{
		//add channel to history
		if(chnode->streamurl != NULL || status.aktservice->type == CHANNEL)
		{
    		addchannelhistory(chnode, status.aktservice->channellist);
			if(chnode->streamurl == NULL && checkbox("DM900") == 1 && status.servicetype == 0) //only for tv
				createmostzap(chnode->serviceid, chnode->transponderid);
		}
		festatus = fewait(fenode);
		if(chnode->streamurl == NULL && festatus != 0)
		{
			m_unlock(&status.servicemutex, 2);
			err("festatus=%i", festatus );
			return 2;
		}
	}

    //wait for epg thread stops
    if(flag == 0 && status.epgthread != NULL)
    {
	    i = 0;
	    while(status.epgthread->status != INPAUSE)
	    {
		    usleep(10000);
		    i++; if(i > 300) break;
	    }
	    status.epgthread->aktion = START;
    }

    status.videosizevalid = time(NULL);
    m_unlock(&status.servicemutex, 2);
    
    //auto change channel name
    if(flag == 0 && status.autochangechannelname == 1)
	    addtimer(&autochangechannelname, START, 1000, 1, NULL, NULL, NULL);
    
    //autoresolution
    if(flag == 0 && ostrcmp(getconfig("av_videomode_autores", NULL), "auto") == 0)
    {
	    int sec = 7;
	    char* av_videomode_autores_ts = getconfig("av_videomode_autores_ts", NULL);
	    if(av_videomode_autores_ts != NULL)
		    sec = atoi(av_videomode_autores_ts);
	    if(status.restimer == NULL)
		    status.restimer = addtimer(&setaktres, START, 1000, 1, (void*)sec, NULL, NULL);
	    else
	    {
		    status.restimer->aktion = STOP;
 			status.restimer = addtimer(&setaktres, START, 1000, 1, (void*)sec, NULL, NULL);
	    }			
    }

    if(flag == 0 && status.autosubtitle == 1) subtitlestartlast(); //start subtitle

    if(flag == 0 && status.timeshifttype == 1)
    {
	    i = 0;
	    while(status.timeshift > 0)
	    {
		    usleep(100000);
		    i++; if(i > 20) break;
	    }
	    timeshiftpause(); //start permanent timeshift record
    }

	debug(200, "servicestartreal... ended with 0");
	return 0;
}

//second zap on failure
int servicestart(struct channel* chnode, char* channellist, char* pin, int flag)
{
	int ret = 0;
	
	ret = servicestartreal(chnode, channellist, pin, flag);
	debug(200, "servicestart... started");
	if(status.secondzap != 0 && ret == 0 && (flag == 0 || flag > 2))
	{
		debug(200, "first zap not ok, make second zap (%d)", status.secondzap);
		ret = servicestartreal(chnode, channellist, pin, 6);
	}
	debug(200, "servicestart... ended");
	return ret;
}

//flag 0: lock
//flag 1: no lock
struct service* getservicebyrecname(char* file, int type, int flag)
{
	if(file == NULL) return NULL;

	if(flag == 0) m_lock(&status.servicemutex, 2);
	struct service* snode = service;

	while(snode != NULL)
	{
		if(ostrcmp(snode->recname, file) == 0 && (type == 0 || (type == 1 && (snode->type == RECORDDIRECT || snode->type == RECORDTIMER))))
		{
			if(flag == 0) m_unlock(&status.servicemutex, 2);
			return snode;
		}
		snode = snode->next;
	}
	if(flag == 0) m_unlock(&status.servicemutex, 2);
	return NULL;
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

//flag 0: lock
//flag 1: no lock
struct service* getservicebyrectimestamp(char* timestamp, int flag)
{
	if(timestamp == 0) return NULL;

	if(flag == 0) m_lock(&status.servicemutex, 2);
	struct service* snode = service;

	while(snode != NULL)
	{
		if(ostrcmp(snode->rectimestamp, timestamp) == 0)
		{
			if(flag == 0) m_unlock(&status.servicemutex, 2);
			return snode;
		}
		snode = snode->next;
	}
	if(flag == 0) m_unlock(&status.servicemutex, 2);
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
//flag 3: same as 0 but no akttolast
//flag 4: showiframe
int servicestop(struct service *node, int clear, int flag)
{
	int rcret = 0;

	if(node != NULL)
	{
		status.tvpic = 0;

        if(status.play == 2 /*&& getconfigint("lastplayertype", NULL) == 0*/)
        {
            printf("servicestop playerstop IpTV flag=%d\n", flag);
            playerstop();
            status.play = 0;
        }

		if(status.timeshift == 1 && flag != 2)
		{
			if(status.timeshifttype == 0 && status.asktimeshift == 0)
				rcret = textbox(_("Message"), _("Timeshift is running !!!\nStop it and switch ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 10, 0);

			if(rcret == 2) return 1;
			timeshiftstop(1);
		}
		if(flag != 2 && node->type != NOTHING && node->type != STILLPIC) caservicedel(node, NULL);


		truncate("/tmp/ecm.info", 0);
		unlink("/tmp/pid.info");
		unlink("/tmp/caids.info");
		
		status.videosizevalid = 0;

		if(status.epgthread != NULL) status.epgthread->aktion = PAUSE;
		subtitlestop(0);


		if(node->type == CHANNEL && flag < 2) akttolast();

		if(flag != 2) node->type = NOTHING;
		if(flag == 4) node->type = STILLPIC;
	
		audiostop(node->audiodev);
		if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkchipset("HI3798MV200") == 1)
			dmxstop(status.aktservice->dmxaudiodev);

		if(checkbox("VUSOLO2_old") == 1)
		{
//			videofreeze(status.aktservice->videodev);
//			dmxstart(status.aktservice->dmxaudiodev);
//			audioplay(status.aktservice->audiodev);
//			audiopause(status.aktservice->audiodev);
			videoclearbuffer(status.aktservice->videodev);
			audioclearbuffer(status.aktservice->audiodev);
//			videoslowmotion(status.aktservice->videodev, 0);
//			videofastforward(status.aktservice->videodev, 0);
		}
		else if(vubox1 == 1)
		{
			videofreeze(node->videodev);
			videoclearbuffer(node->videodev);
			audioclearbuffer(node->audiodev);
		}
		else
		{

			dmxstop(node->dmxaudiodev);
			videostop(node->videodev, clear);
		}

		int	fastzap = getconfigint("fastzap", NULL);

		if(flag == 3) flag = 0;
		if(flag == 4 || flag == 1 || (flag == 0 && (fastzap == 0 || fastzap == 2)))
		{
			audioclose(node->audiodev, -1);
			node->audiodev = NULL;
			dmxstop(node->dmxaudiodev);
			dmxclose(node->dmxaudiodev, -1);
			node->dmxaudiodev = NULL;
		}
		
		if(flag == 4 || flag == 1 || (flag == 0 && fastzap == 0))
		{
			printf("[titan] service.h -> servicestop close all\n");
			if(checkchipset("HI3798MV200") == 1)
			{
				videocontinue(node->videodev);
			}
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
		err("NULL detect");
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
	if((checkchipset("HI3798MV200") == 1) && status.aktservice->audiodev != NULL)
		audiopause(status.aktservice->audiodev);

	//don't start audio play, if we are in timeshift record, but not playing mode
	if(status.timeshifttype == 0 && status.timeshift == 1 && status.playing == 0) return;
	if(status.timeshifttype == 1 && status.timeshift == 1 && status.playing == 0 && status.timeshiftpos > 0) return;

	if(status.mute != 1)
		audioplay(status.aktservice->audiodev);
}

struct service* addservice(struct service* last)
{
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

			free(node->rectimestamp);
			node->rectimestamp = NULL;

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
}

void freeservice()
{
	struct service *node = service, *prev = service;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delservice(prev, 0);
	}
}

char* servicecheckret(int ret, int flag)
{ 
	char* tmpstr = NULL;

	if(ret != 0)
	{
		switch(ret)
		{
			case 1:
				tmpstr = ostrcat(_("Can't find a Tuner.\nAll Tuners in use or no Tuner defined."), NULL, 0, 0);
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

#ifdef MIPSEL
void servicefullHDMIin_start()
{
	struct dvbdev *videonode = NULL;
	struct dvbdev *audionode = NULL;
	struct channel *chnode = NULL;
	
	servicestop(status.aktservice, 1, 1);
	
	audionode = audioopen(0);
	if(audionode != NULL)
	{
		audioselectsource(audionode, AUDIO_SOURCE_HDMI);
		audioplay(audionode); 
	}
	videonode = videoopen(0, 0);
	if(videonode != NULL)
	{
		videoselectsource(videonode, VIDEO_SOURCE_HDMI);
		videosetstreamtype(videonode, 0);
		videoplay(videonode); 
	}
	status.aktservice->videodev = videonode;
	status.aktservice->audiodev = audionode;
	status.aktservice->type = HDMIIN;
	chnode = getchannel(65535, 0);
	if(chnode == NULL)
		//chnode = createchannel("HDMIIN", 0, 0, 65535, 99, 0, -1, -1, -1, -1, 0, -1);
		chnode = createchannel("HDMIIN", 0, 0, 65535, 0, 0, -1, -1, -1, -1, 0, -1, NULL, NULL);
	status.aktservice->channel = chnode;
}
#endif

#endif
