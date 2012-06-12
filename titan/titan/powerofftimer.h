#ifndef POWEROFFTIMER_H
#define POWEROFFTIMER_H

void calcfixpowerofftime()
{
	char* tmpstr = NULL, *fixtime = NULL;
	time_t akttime = time(NULL);
	struct tm* loctime = NULL;

	if(akttime < 1072224000) // 01.01.2004
		return;

	fixtime = getconfig("fixshutdowntime", NULL);
	if(fixtime != NULL)
	{
		loctime = olocaltime(&akttime);
		tmpstr = strptime(fixtime, "%H:%M", loctime);
		if(tmpstr != NULL)
		{
			status.fixpowerofftime = mktime(loctime);
			if(status.fixpowerofftime <= akttime)
				status.fixpowerofftime += 86400;
		}
	}
	else
		status.fixpowerofftime = 1;
	free(loctime); loctime = NULL;
}

void checkshutdowntimer()
{
	int mode = 0;
	if(status.standby != 0) return;

	if(status.fixpowerofftime == 0)
		calcfixpowerofftime();

	time_t akttime = time(NULL);

	if(status.sd_timer != NULL && status.sd_timer->active && status.sd_timer->shutdown_time < akttime)
		mode = 1;
		
	if(status.fixpowerofftime > 1 && status.fixpowerofftime > akttime - 180 && status.fixpowerofftime < akttime)
		mode = 2;
		
	if(mode > 0)
	{
		if(status.fixpowerofftime > 1) status.fixpowerofftime += 86400;
		status.sd_timer->active = 0;
		if(getconfigint("shutdowntimetype", NULL) == 0)
		{
			if(mode == 2)
			{
				mode = 0;
				oshutdown(1, 4);
			}
			else
			{
				mode = 0;
				oshutdown(1, 3);
			} 
		}
		else
		{
			mode = 0;
			status.standby = 2;
			screenstandby();
		}
	}
}

void free_shutdowntimervar()
{
	if(status.sd_timer)
	{
		free(status.sd_timer);
		status.sd_timer = NULL;
	}
}

void screenpowerofftimer(void)
{
	if(status.sd_timer == NULL) //first call
	{
		status.sd_timer = (struct shutdowntimer*) malloc(sizeof(struct shutdowntimer));
		if(!status.sd_timer)
		{
			err("can't allocate memory for variable");
			return;
		}
		status.sd_timer->active = 0;
		status.sd_timer->shutdown_time = time(NULL);
	}

	int rcret = 0;
	struct skin* screen = getscreen("powerofftimerscreen");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* minutes = getscreennode(screen, "minutes");
	struct skin* fixshutdowntime = getscreennode(screen, "fixshutdowntime");
	struct skin* shutdowntimetype = getscreennode(screen, "mode");
	long long secs = 0;

	if(status.sd_timer->active) //calculate minutes until previously scheduled shutdown
	{
		secs = status.sd_timer->shutdown_time - (long long)time(NULL);
		if(secs <= 0) secs = 1800; //oops, we should already be shut down or at least almost;)
	}
	int min = ((float)secs / 60 + 0.5);
	if(min > 1000) min = 999;
	char *tmpstr = (char*)malloc(4 * sizeof(char)) ;
	snprintf(tmpstr,4,"%03d", min);
	changemask(minutes, "000");
	changeinput(minutes, tmpstr);
	free(tmpstr);

	addchoicebox(shutdowntimetype, "0", _("DeepStandby"));
	addchoicebox(shutdowntimetype, "1", _("Standby"));
	setchoiceboxselection(shutdowntimetype, getconfig("shutdowntimetype", NULL));

	changeinput(fixshutdowntime, "deaktiv\n22:00\n22:15\n22:30\n22:45\n23:00\n23:15\n23:30\n23:45\n00:00\n00:15\n00:30\n00:45\n01:00\n01:15\n01:30\n01:45\n02:00\n02:15\n02:30\n02:45\n03:00\n03:15\n03:30\n03:45\n04:00");
	changechoiceboxvalue(fixshutdowntime, "0\n22:00\n22:15\n22:30\n22:45\n23:00\n23:15\n23:30\n23:45\n00:00\n00:15\n00:30\n00:45\n01:00\n01:15\n01:30\n01:45\n02:00\n02:15\n02:30\n02:45\n03:00\n03:15\n03:30\n03:45\n04:00");
	setchoiceboxselection(fixshutdowntime, getconfig("fixshutdowntime", NULL));

	listbox->aktline = 1;
	listbox->aktpage = 1;

	drawscreen(screen, 0, 0);
	addscreenrc(screen, listbox);

	struct skin* tmp = NULL;
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(screen, tmp);
		rcret = waitrc(screen, 0, 0);
		tmp = listbox->select;
		if(rcret == getrcconfigint("rcexit", NULL))  //user pressed exit so no change
		{
			//fprintf(stderr,"shutdowntimerscreen cancelled\n");
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL)) //red means "cancel shutdowntimer"
		{
			//fprintf(stderr,"shutdowntimer cancelled\n");
			status.sd_timer->active = 0;
			status.sd_timer->shutdown_time = time(NULL);
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) //ok means "activate shutdowntimer"
		{
			if(minutes->ret != NULL)
			{
				addconfigscreencheck("fixshutdowntime", fixshutdowntime, "0");
				status.fixpowerofftime = 0;
				addconfigscreencheck("shutdowntimetype", shutdowntimetype, "0");
				if(atoi(minutes->ret) == 0) break;
				status.sd_timer->shutdown_time = atoll(minutes->ret) * 60 + (long long)time(NULL);
				status.sd_timer->active = 1;
				debug(100, "found: time=%s, type=%d, secs=%lld, now=%ld", minutes->ret, getconfigint("shutdowntimetype", NULL), status.sd_timer->shutdown_time, time(NULL));
				break;
			}
		}
		if(rcret == getrcconfigint("rcgreen",NULL)) //blue means "use epg end time of current program"
		{
			struct epg* epgnode = getepgakt(status.aktservice->channel);
			if(epgnode != NULL)
			{
				min = (epgnode->endtime - time(NULL)) / 60 + 1;
				char *tmpstr = (char*)malloc(4 * sizeof(char));
				snprintf(tmpstr, 4, "%03d", min);
				debug(100, "blue: %s %d", tmpstr, min);
				changeinput(minutes, tmpstr);
				free(tmpstr);
				drawscreen(screen, 0, 0);
			}
		}
	}

	delownerrc(screen);
	clearscreen(screen);
}

#endif
