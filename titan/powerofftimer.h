#ifndef POWEROFFTIMER_H
#define POWEROFFTIMER_H

void checkshutdowntimer()
{
	if(status.sd_timer != NULL && status.sd_timer->active && status.sd_timer->shutdown_time < time(NULL))
	{
		status.sd_timer->active = 0;
		if(status.sd_timer->type == 0) 
			oshutdown(1, 0); 
		else
		{
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
		status.sd_timer=NULL;
	}
}

void screenpowerofftimer(void)
{
	if(status.sd_timer==NULL) //first call
	{
		status.sd_timer=(struct shutdowntimer*) malloc(sizeof(struct shutdowntimer));
		if(!status.sd_timer)
		{
			err("can't allocate memory for variable");
			return;
		}
		status.sd_timer->active=0;
		status.sd_timer->type=0;
		status.sd_timer->shutdown_time=time(NULL);
	}

	int rcret = 0;
	struct skin* screen = getscreen("powerofftimerscreen");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* minutes = getscreennode(screen, "minutes");
	struct skin* mode = getscreennode(screen, "mode");
	

	long long secs=0;
	if(status.sd_timer->active) //calculate minutes until previously scheduled shutdown
	{
		secs=status.sd_timer->shutdown_time-(long long)time(NULL);
		if(secs<=0) secs=1800; //oops, we should already be shut down or at least almost;)
	}
	int min=((float)secs/60 + 0.5);
	if(min>1000) min=999;
	char *tmpstr=(char *) malloc(4*sizeof(char)) ;
	snprintf(tmpstr,4,"%03d",min);
	changemask(minutes, "000");
	changeinput(minutes,tmpstr);
	free(tmpstr);

	if(!status.sd_timer->type) //use old value as default
	{
		addchoicebox(mode, "0", _("DeepStandby"));
		addchoicebox(mode, "1", _("Standby"));
	}
	else
	{
		addchoicebox(mode, "1", _("Standby"));
		addchoicebox(mode, "0", _("DeepStandby"));
	}
	drawscreen(screen,0);
	addscreenrc(screen, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;
	struct skin* tmp = NULL;
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(screen, tmp);
		rcret = waitrc(screen, 0, 0);
		tmp = listbox->select;
		if(rcret==getrcconfigint("rcexit",NULL))  //user pressed exit so no change
		{
			//fprintf(stderr,"shutdowntimerscreen cancelled\n");
			break;
		}
		if(rcret==getrcconfigint("rcred",NULL)) //red means "cancel shutdowntimer"
		{
			//fprintf(stderr,"shutdowntimer cancelled\n");
			status.sd_timer->active=0;
			status.sd_timer->shutdown_time=time(NULL);
			break;
		}
		if(rcret==getrcconfigint("rcok",NULL)) //ok means "activate shutdowntimer"
		{
			if(atoi(minutes->ret)==0) break;
			status.sd_timer->shutdown_time=atoll(minutes->ret)*60+(long long)time(NULL);
			status.sd_timer->active=1;
			if(mode->ret != NULL)
				status.sd_timer->type=atoi(mode->ret);
			fprintf(stderr,"found: time=%s, type=%s, secs=%lld, now=%ld\n",minutes->ret,mode->ret,status.sd_timer->shutdown_time,time(NULL));
			break;
		}
		if(rcret==getrcconfigint("rcblue",NULL)) //blue means "use epg end time of current program"
		{
			struct epg* epgnode = getepgakt(status.aktservice->channel);
			if(epgnode != NULL)
			{
				min = (epgnode->endtime-time(NULL))/60+1;
				char *tmpstr=(char *) malloc(4*sizeof(char)) ;
				snprintf(tmpstr,4,"%03d",min);
				fprintf(stderr,"blue: %s %d\n",tmpstr,min);
				changeinput(minutes,tmpstr);
				free(tmpstr);
				drawscreen(screen,0);
			}
		}
	}

	delownerrc(screen);
	clearscreen(screen);
}

#endif
