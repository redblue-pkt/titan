#ifndef STANDBY_H
#define STANDBY_H

void screenstandby()
{
	int rcret = 0 /*, voltoff = 1*/;
	struct skin* standbyscreen = getscreen("standby");
	struct stimerthread *epgscan = NULL;
	char* loctime = NULL, *tmpstr = NULL;
	time_t lastrun = 0;
	struct dvbdev* dvbnode = dvbdev;

	rcret = servicestop(status.aktservice, 1, 0);
	if(rcret == 1) 
	{
		status.standby = 0;
		return;
	}

	//streaming, and record on other chained receiver does not work if we power off the tuner
	/*
	//check if all tuner unlocked, if yes set all volt off
	while(dvbnode != NULL)
        {
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == QPSK && dvbnode->felock > 0)
			voltoff = 0;
		dvbnode = dvbnode->next;
	}
	if(voltoff == 1)
	{
		dvbnode = dvbdev;
		while(dvbnode != NULL)
        	{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == QPSK && dvbnode->felock > 0)
				fesetvoltage(dvbnode, SEC_VOLTAGE_OFF, 15);
			dvbnode = dvbnode->next;
		}
	}
	*/
	
	subtitlepause(1);
//	clearfb(skinfb);
//	blitfb();
	setfbtransparent(0);
	setvfdbrightness(getconfigint("vfdstandbybrightness", NULL));
	setoverclockfreq(0);

	status.protecttime = 0;
	status.rcowner = standbyscreen;
	while(1)
	{
		rcret = waitrc(standbyscreen, 10000, 0);

		if(rcret == getrcconfigint("rcpower", NULL)) break;

		//check for epg update
		loctime = gettime("%H:%M");
		if(lastrun + 300 < time(NULL) && ostrcmp(loctime, getconfig("epg_refreshtime", NULL)) == 0)
		{
			//start epg scanlist
			lastrun = time(NULL);
       			epgscan = addtimer(&epgscanlistthread, START, 1000, 1, NULL, NULL, NULL);
		}
		free(loctime); loctime = 0;
	}

	if(gettimer(epgscan) != NULL)
		epgscan->aktion = STOP;

	setoverclockfreq(1);
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	setvfdbrightness(getconfigint("vfdbrightness", NULL));

	/*
	//
	if(status.aktservice->fedev != NULL && voltoff == 1)
	{
		status.aktservice->fedev->felasttransponder = NULL;
		status.aktservice->fedev->feaktpolarization = 0;
		status.aktservice->fedev->feakttransponder = NULL;
	}
	*/

	tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
	servicestart(status.lastservice->channel, tmpstr, NULL, 0);
	free(tmpstr); tmpstr = NULL;
	subtitlepause(0);
	status.standby = 0;
	status.startmode = 2;
	sleep(1);
	status.rcowner = NULL;
}

#endif
