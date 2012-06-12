#ifndef DVDPLAY_H
#define DVDPLAY_H

extern struct skin* skin;

void screendvdplay(int flag)
{
	int rcret = 0, playertype = 2, dirrcret = 0;
	char* tmpstr = NULL, *startdir = NULL;
	char* file = NULL, *tmppolicy = NULL;
	struct skin* playinfobar = getscreen("playinfobar");

	int skip13 = getconfigint("skip13", NULL);
	int skip46 = getconfigint("skip46", NULL);
	int skip79 = getconfigint("skip79", NULL);
	
	status.updatevfd = PAUSE;
	tmppolicy = getpolicy();

playerstart:
	startdir = getconfig("dvdpath", NULL);
	if(startdir == NULL)
		startdir = getconfig("rec_moviepath", NULL);

	status.playspeed = 0, status.play = 0, status.pause = 0;
	int playinfobarcount = 0, playinfobarstatus = 0;

	tmpstr = ostrcat(file, "", 1, 0); file = NULL;
	file = screendir(startdir, NULL, basename(tmpstr), &dirrcret, NULL, _("EJECT"), getrcconfigint("rcred", NULL), _("SELECT"), 0, NULL, 0, NULL, 0, 90, 1, 90, 1, 2);
	free(tmpstr); tmpstr = NULL;
	
	if(dirrcret == 1)
	{
		system("umount /media/dvd");
		system("eject /dev/sr0");
		goto playerstart;
	}

	if(file != NULL)
	{
		tmpstr = ostrcat(file, "", 0, 0);
		if(tmpstr != NULL) addconfig("dvdpath", dirname(tmpstr));
		free(tmpstr); tmpstr = NULL;
		
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1)
		{
			free(tmppolicy);
			free(file);
			return;
		}

		drawscreen(skin, 0, 0);
		playwritevfd(file);

		rcret = dvdstart(file);
#ifndef SIMULATE
		if(rcret != 0)
		{
			textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			writevfd("DVD");
			
			playstartservice();
			goto playerstart;
		}
#endif

		//screenplayinfobar(file, 0, playertype, flag);

		//change codec if ac3default and video has ac3
		//deaktivate, freeze player, makes a seek -5
		//see eplayer container_ffmpeg_switch_audio
		//the seek to the beginning of the file freez
		//eplayer.
		//playchangecodec();
		free(status.playfile); status.playfile = NULL;
		status.playfile = ostrcat(file, "", 0, 0);
		status.play = 1;
		while(1)
		{
			while(dvdisplaying())
			{
				dvdgotmessage();
				rcret = waitrc(playinfobar, 100, 0);
				dvdkeypress(rcret);
				playinfobarcount++;
				if(playinfobarstatus > 0)
					screenplayinfobar(file, 0, playertype, flag);
				if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL) * 10)
				{
					playinfobarstatus = 0;
					screenplayinfobar(NULL, 1, playertype, flag);
				}

				if(rcret == getrcconfigint("rcstop", NULL))
				{
					playrcstop(playertype, flag);
					playstartservice();
					goto playerstart;
				}

				if(dvdmenuopen() == 1)
				{
					if(playinfobarstatus == 1)
					{
						screenplayinfobar(NULL, 1, playertype, flag);
						playinfobarstatus = 0;
					}
					continue;
				}

				if(rcret == RCTIMEOUT) continue;

				//if(rcret == getrcconfigint("rcyellow", NULL))
				//	playrcyellow(file, playinfobarstatus, playertype, flag);
				
				//if(rcret == getrcconfigint("rctext", NULL) || rcret == getrcconfigint("rcsubtitel", NULL))
				//	playrctext(file, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcgreen", NULL))
				{
					playrcgreen(file, playinfobarstatus, playertype, flag);
					dvdchangevideo();
				}
					
				if(rcret == getrcconfigint("rcred", NULL))
					playrcred(file, playinfobarstatus, playertype, flag);

				if(rcret == getrcconfigint("rcinfo", NULL))
					playrcinfo(file, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcff", NULL))
					playrcff(file, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcfr", NULL))
					playrcfr(file, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcpause", NULL))
					playrcpause(file, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcplay", NULL))
					playrcplay(file, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rc1", NULL))
					playrcjumpr(file, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc4", NULL))
					playrcjumpr(file, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc7", NULL))
					playrcjumpr(file, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc3", NULL))
					playrcjumpf(file, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc6", NULL))
					playrcjumpf(file, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc9", NULL))
					playrcjumpf(file, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
			}
			//don't change this sleep, without this
			//the player stops to fast, and a last seek can
			//produce a segfault
			sleep(1);
			dvdafterend();

			writevfd("DVD");
			screenplayinfobar(file, 1, playertype, flag);

			playstartservice();
			goto playerstart;
		}
	}

	status.updatevfd = START;

	if(tmppolicy != NULL)
	{
		setpolicy(tmppolicy);
		free(tmppolicy);
	}
	
	free(status.playfile); status.playfile = NULL; 
	status.playspeed = 0;
	status.pause = 0;
	status.play = 0;
	free(file);
}

#endif
