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

	//wenn ON dann startet die GUI bei einer Aufnahme neu
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkchipset("BCM7424") == 1)
		addconfig("standbytuneroff", "0");

	//check if all tuner unlocked, if yes set all volt off
	if(getconfigint("standbytuneroff", NULL) == 1 && checkbox("DM7020HD") == 0 && checkbox("DM7020HDV2") == 0 && checkbox("DM900") == 0 && checkbox("DM920") == 0 && checkbox("DM520") == 0 && checkbox("DM525") == 0)
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
	debug(10, "[standby] set vfdstandbybrightness");
	setvfdbrightness(getconfigint("vfdstandbybrightness", NULL));
	setoverclockfreq(0);
	
	if(checkchipset("BCM7424") == 1) //inihdp
	{
		if(getconfigint("vfdisplaystandby", NULL) == 1)
		  	setvfdbrightness(0);
	}
	
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
	else if(checkchipset("BCM7424") == 1) // inihdp
	{	
		if(getconfigint("fanmode", NULL) == 3)
			writesys("/proc/stb/fp/fan", "1", 1);
	}
	else if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
		setled(1);
			

	status.protecttime = 0;
	status.rcstandby = standbyscreen;
	
	setcecstandby(1);
	if(file_exist("/bin/vdstandby") == 1)
		system("vdstandby -a");

// mipsel work set unknown videomod = display > off	(set)
#ifdef MIPSEL
	char* savevideomode = NULL;
	savevideomode = getvideomode();
	//setvideomode("720p24", 1);
	writesys("/proc/stb/avs/0/input", "aux", 1);
	if(checkchipset("3798MV200") == 1 || checkchipset("HI3798MV200") == 1)
		writesys("/proc/stb/hdmi/output", "off", 1);
#endif

	//workaround..  sometimes reboot
	writerc(getrcconfigint("rcexit", NULL));
	printf("standby -> start");
	while(1)
	{
		rcret = waitrc(standbyscreen, 10000, 0);

		if(rcret == getrcconfigint("rcpower", NULL) || rcret == getrcconfigint("rcfrontpower", NULL)) break;

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
	//workaround after standby poweroff menu reboot
	printf("standby -> stop");
	writerc(getrcconfigint("rcexit", NULL));
	rcret = waitrc(standbyscreen, 1000, 0);
	//
	status.updatevfd = STOP;
	m_lock(&status.vfdmutex, 3);
// mipsel work set unknown videomode = display > off (reset)
#ifdef MIPSEL
	writesys("/proc/stb/avs/0/input", "encoder", 1);
	setvideomode(savevideomode, 1);
	free(savevideomode); savevideomode = NULL;
	if(checkchipset("3798MV200") == 1 || checkchipset("HI3798MV200") == 1)
		writesys("/proc/stb/hdmi/output", "on", 1);
#endif

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
	else if(checkchipset("BCM7424") == 1)	//inihdp
		writesys("/proc/stb/fp/fan", getconfig("fanmode", NULL), 1);
	
	setcecstandby(0);
	if(file_exist("/bin/vdstandby") == 1)
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

	if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
		setled(1);
	
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
	m_unlock(&status.vfdmutex, 3);
	status.updatevfd = START;
	putmsgbuffer();
	addtimer(&setdate, START, 10000, 1, NULL, NULL, NULL);
	printf("standby -> ende");
#ifdef MIPSEL
	cecwakeup();
#endif
	addtimer(&servicebouquetsthread, START, 1000, 1, (void*)1, NULL, NULL);
}

#endif
