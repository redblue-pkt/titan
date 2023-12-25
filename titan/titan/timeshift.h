#ifndef TIMESHIFT_H
#define TIMESHIFT_H

//flag 0: pause from rcpause
//flag 1: pause from servicestart
void timeshiftpause(int flag)
{
	int ret = 0;
	int tsextern = getconfigint("tsextern", NULL);
			
	if(flag == 0 && status.timeshift == 1 && status.playing == 0 && status.timeshifttype == 1 && status.timeshiftpos == 0) //stop service
	{
		ret = servicestop(status.aktservice, 0, 2);
		if(ret == 0)
		{
			struct service* snode = getservice(RECORDTIMESHIFT, 0);
			if(snode != NULL)
			{
				status.timeshiftstart = time(NULL);
				status.timeshiftpos = lseek64(snode->recdstfd, 0, SEEK_CUR);
				status.timeshiftpos = status.timeshiftpos - (status.timeshiftpos % snode->tssize);
				if(status.timeshiftpos <= 0) status.timeshiftpos = snode->tssize;
			}		
		}
	}
	else if(status.timeshift == 0) //stop service an start timeshift record
	{
		//reset timeline
		struct skin* playinfobar = getscreen("playinfobar");
		struct skin* sprogress = getscreennode(playinfobar, "progress");
		sprogress->progresssize = 0;

		if(status.timeshifttype == 0)
			ret = servicestop(status.aktservice, 0, 2);
		if(ret == 0)
		{
			ret = recordstart(status.aktservice->channel, -1, 0, RECTIMESHIFT, 0, NULL);
			if(ret != 0)
			{
				status.timeshift = 0;
				if(status.timeshifttype == 0)
					servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 3), 0);
			}
			else
				status.timeshiftstart = time(NULL);
			recordcheckret(NULL, ret, 6);
		}
	}
	else if(status.playspeed == 0 && status.slowspeed == 0 && status.timeshift == 1 && status.playing == 1) //if playing, set pause
	{
		status.slowspeed = 0;
		status.playspeed = 0;
		status.play = 0;
		status.pause = 1;
		if(tsextern == 0)
			playerpausets();
		else
			playerpause();
	}
}

//flag 0: stop from rcstop
//flag 1: stop from servicestop
//flag 2: error in record thread
//flag 3: stop from titan end
void timeshiftstop(int flag)
{
	int i = 0, ret = 0;
	char* file = NULL;
	int tsextern = getconfigint("tsextern", NULL);

	if(flag == 1 && status.timeshifttype == 1 && status.playing == 0 && status.timeshiftpos > 0)
		return;
	if(flag == 0 && status.timeshifttype == 1)
	{
		if(status.timeshiftpos > 0 && status.playing == 0)
			goto startservice;
		else if(status.playing == 1)
		{
			if(status.playspeed != 0 || status.slowspeed != 0)
			{
				if(tsextern == 0)
				{
					playerpausets();
					playercontinuets();
				}
				else
				{
					playerpause();
					playercontinue();
				}
			}
			if(tsextern == 0)
				playerstopts(1, flag);
			else
				playerstop();
			writevfdmenu("Player");
			screenplayinfobar(NULL, NULL, 1, 1, 4);

			goto startservice;
		}
		return;
	}

	struct service* snode = getservice(RECORDTIMESHIFT, flag);
	
	if(status.playspeed != 0 || status.slowspeed != 0)
	{ 
		if(tsextern == 0)
		{
			playerpausets();
			playercontinuets();
		}
		else
		{
			playerpause();
			playercontinue();
		}
	}
	
	if(tsextern == 0)
		playerstopts(1, flag);
	else
		playerstop();

	writevfdmenu("Player");
	screenplayinfobar(NULL, NULL, 1, 1, 4);
	
	if(snode != NULL)
	{
		status.timeshift = 2;
		file = ostrcat(file, snode->recname, 1, 0);
		snode->recendtime = 1;

		ret = getconfigint("deltimeshift", NULL);
		if(ret == 0)
		{
			ret = textbox(_("Message"), _("Should Timeshift File deleted ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 10, 0);
			if(ret == 0) ret = 1; 
		} 

		if(ret == 1) unlink(file);
		
		free(file); file = NULL;
	}

	if(flag == 0 || flag == 3)
	{
		while(status.timeshift > 0)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}
	}
	
startservice:
	if(flag == 0 || flag == 2)
		servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 3), 0);
	
	//if timeshift ends in pause status, we must reactivate continue in player driver
	if(tsextern == 0)
		playercontinuets();
	else
		playercontinue();
	status.timeshiftstart = 0;
	status.timeshiftpos = 0;
	status.playspeed = 0;
	status.slowspeed = 0;
	status.pause = 0;
	status.play = 0;
}

void timeshiftplay(int* playinfobarstatus, int* playinfobarcount)
{
	int ret = 1, waiting = 15;
	int tsextern = getconfigint("tsextern", NULL);

#ifdef MIPSEL
	waiting = 30;
#endif

	struct service* snode = getservice(RECORDTIMESHIFT, 0);
	struct skin* playinfobarpic = getscreen("playinfobarpic");

	if((status.timeshifttype == 0 && status.playing == 0) || (status.timeshifttype == 1 && status.playing == 0 && status.timeshiftpos > 0))
	{

		if(status.timeshiftstart + waiting > time(NULL))
		{
			textbox(_("Message"), _("Timeshift file to short\nplease wait a little and try again"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
			if(status.playpic == 0)
			{	
				drawscreen(playinfobarpic, 0, 0);
				status.playpic = 1;
			}
			return;
		}
	
		if(snode != NULL && tsextern == 0) ret = playerstartts(snode->recname, 1);
		if(snode != NULL && tsextern == 1) ret = playerstart(snode->recname, NULL);
		if(ret != 0)
		{
			textbox(_("Message"), _("Can't start timeshift play !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			if(status.playpic == 0)
			{	
				drawscreen(playinfobarpic, 0, 0);
				status.playpic = 1;
			}
			return;
		}
	}
	else if(status.playing == 0 && status.timeshifttype == 1)
	{
		status.playercan = 0x7FFF;
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		if(snode != NULL) screenplayinfobar(snode->recname, NULL, 0, 1, 5);
	}
	else if(status.playing == 1)
	{
		if(status.playspeed != 0 || status.slowspeed != 0)
		{
			if(tsextern == 0)
				playerpausets();
			else
				playerpause();
		}
		if(status.slowspeed != 0)
			audioclearbuffer(status.aktservice->audiodev);
		
		if(tsextern == 0)
			playercontinuets();
		else
			playercontinue();
				
		if(status.playspeed != 0 || status.slowspeed != 0)
		{
			if(tsextern == 0)
				playerresetts();
			else
				playercontinue();
		}
	}
	
	if(status.playing == 0) return;

	status.slowspeed = 0;
	status.playspeed = 0;
	status.pause = 0;
	status.play = 1;
	status.playercan = 0x7FFF;
	*playinfobarstatus = 1;
	*playinfobarcount = 0;
	if(snode != NULL) screenplayinfobar(snode->recname, NULL, 0, 1, 4);
}

/*
void timeshiftscreen(struct stimerthread* self, struct service* servicenode)
{
	
	off64_t endoffile;
	off64_t currentpos;
	unsigned long long ptsend = 0;
	unsigned long long ptscurrent = 0;
	char* tmpstr = NULL;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* timeshift = getscreen("timeshift");
	struct skin* seek = getscreennode(timeshift, "seek");
	struct skin* timeshiftbar = getscreennode(timeshift, "timeshiftbar");
	char* bg = NULL;
	int seeking = 0, timeout = 0, ret = 0, minuten = 0, sekunden = 0;
	struct service* snode = getservice(RECORDTIMESHIFT, 0);
	int fd = -1;
	
	if(servicenode == NULL)
	{
		err("NULL detect");
		return;
	}
	
	if(snode != NULL)
		fd = open(snode->recname, O_RDONLY | O_LARGEFILE);
	
	if(fd < 0)
	{
		err("timeshift fd not ok");
		return;
	}
	
	timeshiftbar->progresssize = 0;
	setnodeattr(timeshift, framebuffer, 0);
	bg = savescreen(timeshift);
	if(status.timeshiftseek == 999999)
		timeout = 5;

	while(status.timeshiftseek != 0 || seeking != 0)
	{
		currentpos = lseek64(servicenode->recsrcfd, 0, SEEK_CUR);
		currentpos = currentpos - 5000000;
		endoffile = lseek64(fd , 0, SEEK_END);
		
		if(endoffile != 0)
		{
			timeshiftbar->progresssize = currentpos * 100 / endoffile;
			if(status.timeshiftseek < 999999)
			{
				if(status.timeshiftseek > 10000)
				{
					tmpstr = oitoa(status.timeshiftseek - 10000);
					tmpstr = ostrcat(_(">> "), tmpstr, 0, 1);
					tmpstr = ostrcat(tmpstr, "x", 1, 0);
					changetext(seek, tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
				if(status.timeshiftseek < 10000 && status.timeshiftseek > -10000)
				{
					if(seeking == 0)
					{
						seeking = status.timeshiftseek;
						tmpstr = oitoa(status.timeshiftseek);
					}
					else
					{
						tmpstr = oitoa(seeking);
						seeking = 0;
					}
					tmpstr = ostrcat(_("skip "), tmpstr, 0, 1);
					tmpstr = ostrcat(tmpstr, " sec", 1, 0);
					changetext(seek, tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
			}
			else
			{
				ret = getptspos(fd, currentpos, &ptscurrent, &currentpos, 1, servicenode->tssize);
				ret = getpts(fd, 0, 0, 256 * 1024, &ptsend, &endoffile, -1, servicenode->tssize);
				sekunden = (ptsend - ptscurrent) / 90000;
				minuten = sekunden / 60;
				sekunden = sekunden % 60; 
				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(minuten), 1, 1);
				tmpstr = ostrcat(tmpstr, ":", 1, 0);
				
				if(sekunden < 10)
					tmpstr = ostrcat(tmpstr, "0", 1, 0);
					
				tmpstr = ostrcat(tmpstr, oitoa(sekunden), 1, 1);
				tmpstr = ostrcat(tmpstr, " min", 1, 0);
				changetext(seek, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
		}
		
		drawscreen(timeshift, 0, 0);
		sleep (1);
		if(status.timeshiftseek == 999999) {
			timeout = timeout - 1;
			if(timeout == 0)
				status.timeshiftseek = 0;
		} 
	}
	
	restorescreen(bg, timeshift);
	blitfb(0);
	close(fd);
}

//flag 0: normal ff or fr
//flag 1: ff or fr back
void timeshiftseek(int sekunden, int* playinfobarstatus, int* playinfobarcount, int flag)
{
	struct service* snode = getservice(RECORDPLAY, 0);
	
	if(sekunden < 999999)
	{
		status.timeshiftseek = sekunden;
		if(snode != 0)
		{
			if(sekunden > 10000)
			{
				if(sekunden < 20000)
				{
					if(sekunden >= 10128)
					{
						status.timeshiftseek = 10064;
						return;
					}
					status.play = 0;
					if(flag == 0)
						status.playspeed++;
					else
						status.playspeed--;
					sekunden = sekunden - 10000;
					playerffts(sekunden);
				}
				else
				{
					if(sekunden >= 20128)
					{
						status.timeshiftseek = 20064;
						return;
					}
					status.play = 0;
					if(flag == 0)
						status.playspeed--;
					else
						status.playspeed++;
					sekunden = sekunden - 20000;
					playerfrts(sekunden * -1, 1);
					//playerseekts(snode, -10, 0);
				}
				*playinfobarstatus = 2;
				*playinfobarcount = 0;
				screenplayinfobar(status.playfile, NULL, 0, 1, 4);
			}	
			else
			{
				playerseekts(snode, sekunden, 0);
				*playinfobarstatus = 1;
				*playinfobarcount = 0;
				screenplayinfobar(status.playfile, NULL, 0, 1, 4);
			}
		}
	}
	else
	{
		if(*playinfobarstatus == 0)
		{
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(status.playfile, NULL, 0, 1, 4);
		}
		else if(*playinfobarstatus == 1)
		{
			*playinfobarstatus = 0;
			screenplayinfobar(status.playfile, NULL, 1, 1, 4);
		}
	}
}
*/

void timeshiftinfobar(int* playinfobarstatus, int* playinfobarcount)
{
	int tsextern = getconfigint("tsextern", NULL);
	if(status.playing == 0 && status.playspeed == 0)
	{
		*playinfobarstatus = 2;
		//player simmulieren fuer infobar
		status.playercan = 0xFFFF;
		status.pause = 1;
	}
	
	if(status.playing == 1 && status.pause == 1)
	{
		//*playinfobarstatus = 0;
		*playinfobarstatus = 2;
		//screenplayinfobar(NULL, NULL, 1, 1, 4);
	}
	if(status.playing == 1)
	{
		(*playinfobarcount)++;
		if(*playinfobarstatus == 2)
		{
			screenplayinfobar(status.playfile, NULL, 0, 1, 4);
			if(status.playspeed == 0 && status.slowspeed == 0) //used for begin of file with fr
			{
				*playinfobarstatus = 1;
				*playinfobarcount = 0;
			}
		}
		if(*playinfobarstatus == 1)
			screenplayinfobar(status.playfile, NULL, 0, 1, 4);
		if(*playinfobarstatus == 1 && *playinfobarcount >= getconfigint("infobartimeout", NULL) && status.playspeed == 0 && status.slowspeed == 0)
		{
			*playinfobarstatus = 0;
			screenplayinfobar(NULL, NULL, 1, 1, 4);
		}
	}
	else if(*playinfobarstatus > 0) 
	{
		*playinfobarstatus = 0;
		screenplayinfobar(NULL, NULL, 1, 1, 4);
	}
}

void timeshiftposplay(int* playinfobarstatus, int* playinfobarcount)
{
	int tsextern = getconfigint("tsextern", NULL);
	unsigned long long oldpos = status.timeshiftpos;
	struct service* snode = getservice(RECORDTIMESHIFT, 0);
	if(snode != NULL)
	{
		status.timeshiftpos = lseek64(snode->recdstfd, 0, SEEK_CUR) - 10000000;
		status.timeshiftpos = status.timeshiftpos - (status.timeshiftpos % snode->tssize);
		if(status.timeshiftpos <= 0) status.timeshiftpos = snode->tssize;
	}
	timeshiftplay(playinfobarstatus, playinfobarcount);
	if(status.playing == 0)
		status.timeshiftpos = oldpos;
	else
		status.timeshiftpos = 0;
}


#endif
