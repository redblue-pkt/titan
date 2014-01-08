#ifndef STANDBY_H
#define STANDBY_H

void screenstandby()
{
	int rcret = 0, voltoff = 1;
	struct skin* standbyscreen = getscreen("standby");
	char* loctime = NULL, *tmpstr = NULL;
	time_t lastrun = 0;
	struct dvbdev* dvbnode = dvbdev;

	rcret = servicestop(status.aktservice, 1, 0);
	if(rcret == 1) 
	{
		status.standby = 0;
		return;
	}

	//check if all tuner unlocked, if yes set all volt off
	if(getconfigint("standbytuneroff", NULL) == 1)
	{
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == FE_QPSK && dvbnode->felock > 0)
				voltoff = 0;
			dvbnode = dvbnode->next;
		}
		if(voltoff == 1)
		{
			dvbnode = dvbdev;
			while(dvbnode != NULL)
			{
				if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == FE_QPSK && dvbnode->felock == 0)
					fesetvoltage(dvbnode, SEC_VOLTAGE_OFF, 15);
				dvbnode = dvbnode->next;
			}
		}
	}
	else
		voltoff = 0;
	
	subtitlepause(1);
//	clearfb(skinfb);
//	blitfb(0);
	setfbtransparent(0);
	setvfdbrightness(getconfigint("vfdstandbybrightness", NULL));
	setoverclockfreq(0);

	if(checkbox("ATEVIO7000") == 1)
	{
		if(getconfig("at7000frontsleep", NULL) != NULL)
			tmpstr = ostrcat("fp_control -P ",getconfig("at7000frontsleep", NULL), 0, 0);
		else
			tmpstr = ostrcat("fp_control -P ", "0", 0, 0);
		system(tmpstr);
		free(tmpstr); tmpstr=NULL;
	}
	if(checkbox("UFS922") == 1)
		setfanspeed(-2, 0);

	status.protecttime = 0;
	status.rcstandby = standbyscreen;
	
	setcecstandby(1);
	system("vdstandby -a");
	while(1)
	{
		rcret = waitrc(standbyscreen, 10000, 0);

		if(rcret == getrcconfigint("rcpower", NULL)) break;

		//check for epg update
		loctime = gettime(NULL, "%H:%M");
		if(lastrun + 300 < time(NULL) && ostrcmp(loctime, getconfig("epg_refreshtime", NULL)) == 0)
		{
			voltoff = 1; //is not a real voltoff, but after standby this makes a new channel tuning
			//start epg scanlist
			lastrun = time(NULL);
			if(status.epgscanlistthread == NULL)
				status.epgscanlistthread = addtimer(&epgscanlistthread, START, 1000, 1, NULL, NULL, NULL);
		}
		free(loctime); loctime = 0;
	}

	if(status.epgscanlistthread != NULL)
	{
		int i = 0;
		status.epgscanlistthread->aktion = STOP;
		while(status.epgscanlistthread != NULL)
		{
			usleep(100000);
			i++; if(i > 50) break;
		}
	}
	if(checkbox("UFS922") == 1)
		setfanspeed(-1, 0);
	
	setcecstandby(0);
	system("vdstandby -d");

	setoverclockfreq(1);
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	setvfdbrightness(getconfigint("vfdbrightness", NULL));
	
	if(checkbox("ATEVIO7000") == 1)
	{
		if(getconfig("at7000frontrun", NULL) != NULL)
			tmpstr = ostrcat("fp_control -P ",getconfig("at7000frontrun", NULL), 0, 0);
		else
			tmpstr = ostrcat("fp_control -P ", "15", 0, 0);
		system(tmpstr);
		free(tmpstr); tmpstr=NULL;
	}
	
	//set waittime for unicable new
	status.firstunicablewait = 0;
	status.standby = 0;
	status.startmode = 1;
	status.rcstandby = NULL;

	if(status.aktservice->channel != NULL)
	{
		tmpstr = ostrcat(status.aktservice->channellist, NULL, 0, 0);
		if(voltoff == 1)
			servicecheckret(servicestart(status.aktservice->channel, tmpstr, NULL, 5), 0);
		else
			servicecheckret(servicestart(status.aktservice->channel, tmpstr, NULL, 3), 0);
	}
	else
	{
		tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		if(voltoff == 1)
			servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 4), 0);
		else
			servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 0), 0);
	}
	free(tmpstr); tmpstr = NULL;

	subtitlepause(0);
}

#endif
