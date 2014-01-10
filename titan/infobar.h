#ifndef INFOBAR_H
#define INFOBAR_H

void screeninfobar()
{
	int rcret = 0, ret = 0, infobartimeout = 0, rcwait = 1000, count = 0, first = 1;
	struct skin* infobar1 = getscreen("infobar");
	struct skin* infobar2 = getscreen("infobar2");
	struct skin* infobar = infobar1;
	struct skin* standbymenu = NULL;
	char* tmpstr = NULL; char* tmpnr = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	time_t lasttime = 0;
	int mark = 0;

	status.mute = 0;
	status.infobar = 2;

	addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
	addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
	addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);

	status.infobaraktiv = 1;
	status.mcaktiv = 0;
	
	int playinfobarcount = 0, playinfobarstatus = 0;
	
	while(1)
	{
		mark = 0;

		if(status.crosscontrol > 0 && status.play == 0 && status.pause == 0 && rcret == getrcconfigint("rcright", NULL))
			writerc(getrcconfigint("rcvolup", NULL));
		if(status.crosscontrol > 0 && status.play == 0 && status.pause == 0 && rcret == getrcconfigint("rcleft", NULL))
			writerc(getrcconfigint("rcvoldown", NULL));

		//check if mediadb can freed
		if(status.mediadbthread == NULL)
		{
			if(status.writemediadb == 1)
				writemediadb(getconfig("mediadbfile", NULL), NULL);
			freemediadb(0);
		}
		//check if picmem times out and must freed
		if(time(NULL) > status.picmemtimeout + 60)
		{
			checkpictimeout();
			status.picmemtimeout = time(NULL);
		}

		if(status.standby == 1)
			screenstandby();
		if(status.infobar == 1 )
		{
			rcret = waitrc(infobar, 1000, 0);
			infobartimeout++;
		}
		else if(status.infobar == 0 && status.timeshift == 1)
		{
			rcret = waitrc(infobar, 1000, 0);
			if(rcret == RCTIMEOUT)
			{
				timeshiftinfobar(&playinfobarstatus, &playinfobarcount);
				continue;
			}
		}
		else if(status.infobar == 2)
		{
			if(getconfigint("infobarsleep", NULL) > 0)
				rcret = waitrc(infobar, getconfigint("infobarsleep", NULL) * 1000, 0);
			else if(first == 1) //only on first start wait a little for epg
			{
				first = 0;
				rcret = waitrc(infobar, 1000, 0);
			}
			else
				rcret = RCTIMEOUT;

			if(rcret == RCTIMEOUT)
			{
				status.infobar = 1;
				infobar = infobar1;
				drawscreen(infobar, 0, 4);
				continue;
			}
			else
				status.infobar = 0;
		}
		else
		{
			int screensaver_delay = getconfigint("screensaver_delay", NULL);
			rcret = 0; count = 0; rcwait = 1000;
			if(status.servicetype == 1 && getconfigint("screensaver", NULL) == 1)
			{
				initscreensaver();
				if(screensaver != NULL) screensaver->flag = 1;
			}
			if(screensaver == NULL)
			{
				if(status.infobarprogram == 1) rcwait = 2000;
				else rcwait = 0;
			}
			while(rcret == 0 || rcret == RCTIMEOUT)
			{
				rcret = waitrc(infobar, rcwait, 0);
				count++;
				if(screensaver == NULL && status.infobarprogram == 1 && (rcret == 0 || rcret == RCTIMEOUT))
				{
					infobartimeout = 99999;
					mark = 1;
					break;
				}
				if(rcret == RCTIMEOUT && screensaver != NULL && count > screensaver_delay)
				{
					if(status.aktservice->channel != NULL)
						screensaver->value = ostrcat(status.aktservice->channel->name, NULL, 0, 0); 
					else
						screensaver->value = ostrcat("Radio", NULL, 0, 0); 
					showscreensaver();
					free(screensaver->value);
					rcwait = screensaver->speed;
				}
			}
			if(screensaver != NULL) screensaver->flag = 0;
			deinitscreensaver();
			if(mark == 0)
			{
				drawscreen(skin, 0, 0);
				infobartimeout = 0;
			}
		}

		if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
		{
			//timeshift
			if(status.timeshift == 1 && (status.playing == 0 || status.slowspeed != 0 || status.playspeed != 0 || status.pause != 0))
			{
				if(status.timeshifttype == 1 && status.timeshiftpos == 0)
				{
					timeshiftpause(0);
					timeshiftinfobar(&playinfobarstatus, &playinfobarcount);
				}
				else
					timeshiftplay(&playinfobarstatus, &playinfobarcount);
			}
			else
			{
				timeshiftpause(0);
				timeshiftinfobar(&playinfobarstatus, &playinfobarcount);
			}
			
			continue;
		}
		if(status.timeshift == 1)
		{
					
			if(rcret == getrcconfigint("rcstop", NULL))
			{
				timeshiftstop(0);
				continue;
			}
			if(rcret == getrcconfigint("rcplay", NULL))
			{
				if((checkbox("ATEMIO520") != 1 && checkbox("ATEMIO530") != 1 && status.timeshifttype == 1) || ostrcmp(getconfig("remotecontrol", NULL), "1") == 1)
				{
					if(status.playing == 0 || (status.playspeed == 0 && status.slowspeed == 0 && status.pause == 0))
					{
						subtitlepause(1);
						status.infobar = 0;
						status.infobaraktiv = 0;
						clearscreen(infobar);
						screenplay(NULL, NULL, 1, 0);
						status.infobaraktiv = 1;
						status.updatevfd = START;
						drawscreen(skin, 0, 0);
						subtitlepause(0);
						continue;
					}
				}
				timeshiftplay(&playinfobarstatus, &playinfobarcount);
				continue;
			}
			if(status.timeshifttype == 1 && status.playing == 0)
			{
				if(rcret == getrcconfigint("rcff", NULL) || (getconfig("timeshiftnumkeys", NULL) == 0 && (rcret == getrcconfigint("rc3", NULL) || rcret == getrcconfigint("rc6", NULL) || rcret == getrcconfigint("rc9", NULL))))
				{
					struct service* snode = getservice(RECORDTIMESHIFT, 0);
					if(snode != NULL)
					{
						status.playercan = 0x7FFF;
						playinfobarstatus = 1;
						playinfobarcount = 0;
						screenplayinfobar(snode->recname, NULL, 0, 1, 5);
					}
					continue;
				}
				if(rcret == getrcconfigint("rcfr", NULL))
				{
					timeshiftposplay(&playinfobarstatus, &playinfobarcount);
					if(status.playing == 1)
						playrcfr(status.playfile, NULL, &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(getconfig("timeshiftnumkeys", NULL) == 0)
				{
					if(rcret == getrcconfigint("rc1", NULL))
					{
						timeshiftposplay(&playinfobarstatus, &playinfobarcount);
						if(status.playing == 1)
							playrcjumpr(status.playfile, NULL, getconfigint("skip13", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
						continue;
					}
					if(rcret == getrcconfigint("rc4", NULL))
					{
						timeshiftposplay(&playinfobarstatus, &playinfobarcount);
						if(status.playing == 1)
							playrcjumpr(status.playfile, NULL, getconfigint("skip46", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
						continue;
					}
					if(rcret == getrcconfigint("rc7", NULL))
					{
						timeshiftposplay(&playinfobarstatus, &playinfobarcount);
						if(status.playing == 1)
							playrcjumpr(status.playfile, NULL, getconfigint("skip79", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
						continue;
					}
				}
			}
			if(status.playing == 1)
			{
				if(rcret == getrcconfigint("rcff", NULL))
				{
					playrcff(status.playfile, NULL, &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(rcret == getrcconfigint("rcfr", NULL))
				{
					playrcfr(status.playfile, NULL, &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(rcret == getrcconfigint("rc3", NULL))
				{
					playrcjumpf(status.playfile, NULL, getconfigint("skip13", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(rcret == getrcconfigint("rc6", NULL))
				{
					playrcjumpf(status.playfile, NULL, getconfigint("skip46", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(rcret == getrcconfigint("rc9", NULL))
				{				
					playrcjumpf(status.playfile, NULL, getconfigint("skip79", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}	
				if(rcret == getrcconfigint("rc1", NULL))
				{
					playrcjumpr(status.playfile, NULL, getconfigint("skip13", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(rcret == getrcconfigint("rc4", NULL))
				{
					playrcjumpr(status.playfile, NULL, getconfigint("skip46", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(rcret == getrcconfigint("rc7", NULL))
				{
					playrcjumpr(status.playfile, NULL, getconfigint("skip79", NULL), &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;
				}
				if(status.timeshifttype == 0 && rcret == getrcconfigint("rcinfo", NULL))
				{
					playrcinfo(status.playfile, NULL, &playinfobarstatus, &playinfobarcount, 1, 4);
					continue;	
				}
			}
		}
		if(rcret == getrcconfigint("rcstop", NULL) && status.recording > 0)
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenrecordstop();
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcblue", NULL) && getconfig("bluekey", NULL) == NULL)
			rcret = getrcconfigint("rctvradio", NULL);
		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rctvradio", NULL) || rcret == getrcconfigint("rcfav", NULL) || (status.crosscontrol == 0 && status.play == 0 && status.pause == 0 && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))))
		{
			int tmpservicetype = status.servicetype;
			status.infobaraktiv = 0;
			subtitlepause(1);
			clearscreen(infobar);
			if(rcret == getrcconfigint("rctvradio", NULL))
			{
				if(status.servicetype == 0)
					status.servicetype = 1;
				else
					status.servicetype = 0;
			}
			drawscreen(skin, 0, 0);
			if(rcret == getrcconfigint("rcfav", NULL))	
				ret = screenchannellist(NULL, NULL, 2);
			else
				ret = screenchannellist(NULL, NULL, 0);
			if(ret >= 20 || ret < 0)
				status.servicetype = tmpservicetype;
			drawscreen(skin, 0, 0);
			status.infobaraktiv = 1;
			status.infobar = 2;
			infobartimeout = 0;
			continue;
		}
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(status.timeshift == 1 && status.playing == 1)
				screenplayinfobar(NULL, NULL, 1, 1, 4);
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rctext", NULL))
		{
			if(status.aktservice->channel != NULL && status.aktservice->channel->txtpid > 0)
			{
				subtitlepause(1);
				status.infobar = 0;
				status.sec = 0;
				clearscreen(infobar);
				tmpstr = ostrcat(getconfig("tuxtxtfile", NULL), " ", 0, 0);
				tmpnr = oitoa(status.aktservice->channel->txtpid);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
        
				if(status.aktservice->fedev != NULL)
				{
					tmpnr = oitoa(status.aktservice->fedev->devnr);
					tmpstr = ostrcat(tmpstr, " ", 1, 0);
					tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
				}
        
				drawscreen(skin, 0, 0);
				status.tuxtxt = 1;
				system(tmpstr);
				status.tuxtxt = 0;
				free(tmpstr); tmpstr = NULL; tmpnr = NULL;
				drawscreen(skin, 0, 0);
				subtitlepause(0);
			}
			continue;
		}
		if(rcret == getrcconfigint("rcrecord", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenrecorddirect();
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if((status.timeshift == 0 && rcret == getrcconfigint("rcplay", NULL)) || rcret == getrcconfigint("rcarchive", NULL) || rcret == getrcconfigint("rcwebplay", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			screenplay(NULL, NULL, 1, 0);
			status.infobaraktiv = 1;
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			screenkeyactions(1, 0);
			status.infobaraktiv = 1;
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenvideomode(0);
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcsubchannel", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenlinkedchannel();
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcsleep", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenpowerofftimer();
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcsubtitel", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screensubtitle();
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenaudiotrack();
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenkeyactions(0, 0);
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rchbbtv", NULL) && status.aktservice->channel != NULL && status.aktservice->channel->hbbtvurl != NULL)
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);

			pluginnode = getplugin("hbbtv Browser");
			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "starturl");
				if(startplugin != NULL)
					startplugin();
			}

			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcmedia", NULL))
		{
			struct skin* pluginnode = getplugin("Media Center");
			void (*startplugin)(void);
			status.infobaraktiv = 0;
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "start");
				if(startplugin != NULL)
				startplugin();
			}
			status.infobaraktiv = 1;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			menu(getscreen("mainmenu"), 1);
			status.menurelease = 0;
			status.updatevfd = START;
			status.infobaraktiv = 1;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcepg", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			epgchoice(NULL);
			drawscreen(skin, 0, 0);
			status.infobaraktiv = 1;
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcresolution", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			switchvideomode();
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcinfo", NULL))
		{
			if(status.infobar == 1)
			{
				if(getconfigint("secondinfobar", NULL) == 2)
				{
					clearscreen(infobar);
					epgchoice(NULL);
					status.infobar = 0;
					drawscreen(skin, 0, 0);
					subtitlepause(0);
				}
				else if(getconfigint("secondinfobar", NULL) == 3)
				{
					clearscreen(infobar);
					screenepg(NULL, NULL, 0);
					status.infobar = 0;
					drawscreen(skin, 0, 0);
					subtitlepause(0);
				}
				else if(getconfigint("secondinfobar", NULL) == 0 || infobar2 == status.skinerr || infobar == infobar2)
				{
					status.infobar = 0;
					drawscreen(skin, 0, 0);
					subtitlepause(0);
				}
				else
				{
					infobar = infobar2;
					drawscreen(infobar, 0, 0);
				}
			}
			else
			{
				subtitlepause(1);
				status.infobar = 1;
				infobar = infobar1;
				drawscreen(infobar, 0, 4);
			}
			continue;
		}
		if(rcret == getrcconfigint("rcpower", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			drawscreen(skin, 0, 0);
			switch(getconfigint("poweraktion", NULL))
			{
				case 1: //power off
					oshutdown(1, 1);
					break;
				case 2: //standby
					status.standby = 1;
					break;
				case 3: //restart
					oshutdown(2, 1);
					break;
				case 4: //Gui restart
					oshutdown(3, 1);
					break;
				default:
					standbymenu = getscreen("standbymenu");
					menu(standbymenu, 1);
					break;
			}
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			status.infobaraktiv = 1;
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcrecall", NULL))
		{
			subtitlepause(1);
			clearscreen(infobar);
			drawscreen(skin, 0, 0);
			screenchannelhistory();
			infobartimeout = 0;
			infobar = infobar1;
			if(status.infobar == 0)
				drawscreen(infobar, 0, 4);
			else
				drawscreen(infobar, 0, 0);
			status.infobar = 1;
			continue;
		}
		if(rcret == getrcconfigint("rc0", NULL))
		{
			subtitlepause(1);
			clearscreen(infobar);
			drawscreen(skin, 0, 0);
			if(status.lastservice->channel != NULL)
			{
				tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
				servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 0), 0);
				free(tmpstr); tmpstr = NULL;
			}
			infobartimeout = 0;
			infobar = infobar1;
			if(status.infobar == 0)
				drawscreen(infobar, 0, 4);
			else
				drawscreen(infobar, 0, 0);
			status.infobar = 1;
			continue;
		}
		if(rcret == getrcconfigint("rcchup", NULL) || (status.crosscontrol > 0 && status.play == 0 && status.pause == 0 && rcret == getrcconfigint("rcdown", NULL)))
		{
			subtitlepause(1);
			if(status.crosscontrol > 1 && status.virtualzap == 0 && rcret == getrcconfigint("rcdown", NULL))
			{
				status.virtualzap = status.crosscontrol - 1;
				zapup();
				status.virtualzap = 0;
			}
			else 
				zapup();
			clearscreen(infobar);
			drawscreen(skin, 0, 0);
			infobartimeout = 0;
			status.infobar = 2;
			continue;
		}
		if(rcret == getrcconfigint("rcchdown", NULL) || (status.crosscontrol > 0 && status.play == 0 && status.pause == 0 && rcret == getrcconfigint("rcup", NULL)))
		{
			subtitlepause(1);
			if(status.crosscontrol > 1 && status.virtualzap == 0 && rcret == getrcconfigint("rcup", NULL))
			{
				status.virtualzap = status.crosscontrol - 1;
				zapdown();
				status.virtualzap = 0;
			}
			else
				zapdown();
			clearscreen(infobar);
			drawscreen(skin, 0, 0);
			infobartimeout = 0;
			status.infobar = 2;
			continue;
		}
		if(rcret == rcnumber(rcret))
		{
			subtitlepause(1);
			clearscreen(infobar);
			screenchannelbynr(rcret);
			status.infobar = 2;
			continue;
		}
		//show infobar on program switch (only tv)
		if(status.infobarprogram == 1 && status.infobar == 0 && status.servicetype == 0)
		{
			time_t akttime = time(NULL);
			//show infobar only all 60 sec
			if(lasttime < akttime)
			{
				struct epg* tmpepg = getepgakt(status.aktservice->channel);
				if(tmpepg != NULL && akttime - 1 <= tmpepg->starttime && akttime + 1 >= tmpepg->starttime)
				{
					lasttime = akttime + 10;
					infobartimeout = 0;
					subtitlepause(1);
					status.infobar = 1;
					infobar = infobar1;
					drawscreen(infobar, 0, 4);
					continue;
				}
			}
		}
    /*
		if(rcret == getrcconfigint("rcpip", NULL))
		{
			struct channel* tmp = getchannel(12003, 66625); //for test RTL Television
			pipstart(tmp, NULL, 0);
		}
    */
		if(rcret == RCTIMEOUT && mark == 0)
		{
			if(getconfigint("infobartimeout", NULL) > infobartimeout)
			{
				clearscreen(infobar);
				drawscreen(infobar, 0, 0);
			}
			else
			{
				status.infobar = 0;
				drawscreen(skin, 0, 0);
				subtitlepause(0);
			}
		}
	}
}

#endif
