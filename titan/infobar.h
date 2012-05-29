#ifndef INFOBAR_H
#define INFOBAR_H

void screeninfobar()
{
	int rcret = 0, ret = 0, infobartimeout = 0, rcwait = 1000, count = 0;
	struct skin* infobar1 = getscreen("infobar");
	struct skin* infobar2 = getscreen("infobar2");
	struct skin* infobar = infobar1;
	struct skin* standbymenu = NULL;
	char* tmpstr = NULL; char* tmpnr = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);

	status.mute = 0;
	status.infobar = 2;

	addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
	addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
	addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);

	status.infobaraktiv = 1;
	status.mcaktiv = 0;
	
	while(1)
	{
    //check if picmem times out and must freed
    if(time(NULL) > status.picmemtimeout + 60)
    {
      checkpictimeout();
      status.picmemtimeout = time(NULL);
    }
  
		if(status.standby == 1)
			screenstandby();
		if(status.infobar == 1)
		{
			rcret = waitrc(infobar, 1000, 0);
			infobartimeout++;
		}
		else if(status.infobar == 2)
		{
			if(getconfigint("infobarsleep", NULL) > 0)
				rcret = waitrc(infobar, getconfigint("infobarsleep", NULL) * 1000, 0);
			else
				rcret = RCTIMEOUT;
			if(rcret == RCTIMEOUT)
			{
				status.infobar = 1;
				infobar = infobar1;
				drawscreen(infobar, 4);
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
			if(screensaver == NULL) rcwait = 0;
			while(rcret == 0 || rcret == RCTIMEOUT)
			{
				rcret = waitrc(infobar, rcwait, 0);
				count++;
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
			drawscreen(skin, 0);
			infobartimeout = 0;
		}

		if(rcret == getrcconfigint("rcpause", NULL))
		{
			//timeshift
			timeshiftpause();
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
				timeshiftplay();
				continue;
			}
			//TODO: ff, fr, seek
			if(rcret == getrcconfigint("rcff", NULL))
			{
				if(status.playing == 1) { 
					if(status.timeshiftseek > 10000) {
						timeshiftseek((status.timeshiftseek - 10000) * 2 + 10000);
					}
					else
					timeshiftseek(10002);
				}
				continue;
			}
			if(rcret == getrcconfigint("rcfr", NULL))
			{
				if(status.playing == 1) {
					if(status.timeshiftseek > 102)
						timeshiftplay();
					timeshiftseek(-10);
				}
				continue;
			}
			if(rcret == getrcconfigint("rc3", NULL))
			{
				if(status.timeshiftseek > 102)
						timeshiftplay();
				timeshiftseek(60);
				continue;
			}
			if(rcret == getrcconfigint("rc6", NULL))
			{
				if(status.timeshiftseek > 102)
						timeshiftplay();
				timeshiftseek(300);
				continue;
			}
			if(rcret == getrcconfigint("rc9", NULL))
			{
				if(status.timeshiftseek > 102)
						timeshiftplay();
				timeshiftseek(600);
				continue;
			}	
			if(rcret == getrcconfigint("rc1", NULL))
			{
				if(status.timeshiftseek > 102)
						timeshiftplay();
				timeshiftseek(-60);
				continue;
			}
			if(rcret == getrcconfigint("rc4", NULL))
			{
				if(status.timeshiftseek > 102)
						timeshiftplay();
				timeshiftseek(-300);
				continue;
			}
			if(rcret == getrcconfigint("rc7", NULL))
			{
				if(status.timeshiftseek > 102)
						timeshiftplay();
				timeshiftseek(-600);
				continue;
			}
			if(rcret == getrcconfigint("rcinfo", NULL))
			{
				if(status.playing == 1) {
					if(status.timeshiftseek > 102 && status.timeshiftseek < 999999)
							timeshiftplay();
					if(status.timeshiftseek == 999999)
						status.timeshiftseek = 0;
					else {	
						timeshiftseek(999999);
					continue;
					}
				}	
			}
		}
		if(rcret == getrcconfigint("rcstop", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenrecordstop();
			status.updatevfd = START;
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcblue", NULL) && getconfig("bluekey", NULL) == NULL)
			rcret = getrcconfigint("rctvradio", NULL);
		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rctvradio", NULL) || rcret == getrcconfigint("rcfav", NULL))
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
			drawscreen(skin, 0);
			if(rcret == getrcconfigint("rcfav", NULL))	
				ret = screenchannellist(NULL, NULL, 2);
			else
				ret = screenchannellist(NULL, NULL, 0);
			if(ret >= 20 || ret < 0)
				status.servicetype = tmpservicetype;
			drawscreen(skin, 0);
			status.infobaraktiv = 1;
			status.infobar = 2;
			continue;
		}
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(status.timeshiftseek == 999999)
					status.timeshiftseek = 0;
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			drawscreen(skin, 0);
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
				drawscreen(skin, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL; tmpnr = NULL;
				drawscreen(skin, 0);
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
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(status.timeshift == 0 && (rcret == getrcconfigint("rcplay", NULL) || rcret == getrcconfigint("rcarchive", NULL)))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			screenplay(NULL, 1, 0);
			status.infobaraktiv = 1;
			status.updatevfd = START;
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenvideomode(0);
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcsubchannel", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenlinkedchannel();
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcsleep", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenpowerofftimer();
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcsubtitel", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screensubtitle();
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			clearscreen(infobar);
			screenaudiotrack();
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcmedia", NULL) && status.security == 1)
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
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
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
					drawscreen(skin, 0);
					subtitlepause(0);
				}
				else if(getconfigint("secondinfobar", NULL) == 0 || infobar2 == status.skinerr || infobar == infobar2)
				{
					status.infobar = 0;
					drawscreen(skin, 0);
					subtitlepause(0);
				}
				else
				{
					infobar = infobar2;
					drawscreen(infobar, 0);
				}
			}
			else
			{
				subtitlepause(1);
				status.infobar = 1;
				infobar = infobar1;
				drawscreen(infobar, 4);
			}
			continue;
		}
		if(rcret == getrcconfigint("rcpower", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(infobar);
			drawscreen(skin, 0);
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
			drawscreen(skin, 0);
			status.infobaraktiv = 1;
			subtitlepause(0);
			continue;
		}
		if(rcret == getrcconfigint("rcrecall", NULL))
		{
			subtitlepause(1);
			clearscreen(infobar);
			drawscreen(skin, 0);
			screenchannelhistory();
			infobartimeout = 0;
			infobar = infobar1;
			if(status.infobar == 0)
				drawscreen(infobar, 4);
			else
				drawscreen(infobar, 0);
			status.infobar = 1;
			continue;
		}
		if(rcret == getrcconfigint("rc0", NULL))
		{
			subtitlepause(1);
			clearscreen(infobar);
			drawscreen(skin, 0);
			if(status.lastservice->channel != NULL)
			{
				tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
				servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 0), 0);
				free(tmpstr); tmpstr = NULL;
			}
			infobartimeout = 0;
			infobar = infobar1;
			if(status.infobar == 0)
				drawscreen(infobar, 4);
			else
				drawscreen(infobar, 0);
			status.infobar = 1;
			continue;
		}
		if(rcret == getrcconfigint("rcchup", NULL))
		{
			subtitlepause(1);
			zapup();
			clearscreen(infobar);
			drawscreen(skin, 0);
			infobartimeout = 0;
			status.infobar = 2;
			continue;
		}
		if(rcret == getrcconfigint("rcchdown", NULL))
		{
			subtitlepause(1);
			zapdown();
			clearscreen(infobar);
			drawscreen(skin, 0);
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
		if(rcret == RCTIMEOUT)
		{
			if(getconfigint("infobartimeout", NULL) > infobartimeout)
			{
				clearscreen(infobar);
				drawscreen(infobar, 0);
			}
			else
			{
				status.infobar = 0;
				drawscreen(skin, 0);
				subtitlepause(0);
			}
		}
	}
}

#endif
