#ifndef STANDBY_H
#define STANDBY_H

void screenstandby()
{
	int rcret = 0;
	struct skin* standbyscreen = getscreen("standby");
	struct stimerthread *epgscan = NULL;
	char* loctime = NULL, *tmpstr = NULL;
	time_t lastrun = 0;

	rcret = servicestop(status.aktservice, 1, 0);
	if(rcret == 1) 
	{
		status.standby = 0;
		return;
	}
	
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
