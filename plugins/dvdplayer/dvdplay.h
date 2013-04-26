#ifndef DVDPLAY_H
#define DVDPLAY_H

extern struct skin* skin;

void screendvdplay(char* startfile, int flag)
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
	if(startfile == NULL)
	{
		tmpstr = ostrcat(file, "", 1, 0); file = NULL;
		file = screendir(startdir, NULL, basename(tmpstr), &dirrcret, NULL, _("EJECT"), getrcconfigint("rcred", NULL), _("SELECT"), 0, NULL, 0, NULL, 0, 90, 1, 90, 1, 2);
		free(tmpstr); tmpstr = NULL;
	}
	else
		file = ostrcat(startfile, NULL, 0, 0);
	
	if(startfile == NULL && dirrcret == 1)
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
		playwritevfd(file, NULL);
		rcret = dvdstart(file);
#ifndef SIMULATE
		if(rcret != 0)
		{
			textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			writevfdmenu("DVD");
			
			if(startfile == NULL)
			{
				playstartservice();
				goto playerstart;
			}
		}
#endif

		//screenplayinfobar(file, NULL, 0, playertype, flag);

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
					screenplayinfobar(file, NULL, 0, playertype, flag);
				if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL) * 10)
				{
					playinfobarstatus = 0;
					screenplayinfobar(NULL, NULL, 1, playertype, flag);
				}

				if(rcret == getrcconfigint("rcstop", NULL))
				{
					playrcstop(playertype, flag);
					if(startfile == NULL)
					{
						playstartservice();
						goto playerstart;
					}
					else
						goto playerend;
				}

				if(dvdmenuopen() == 1)
				{
					if(playinfobarstatus == 1)
					{
						screenplayinfobar(NULL, NULL, 1, playertype, flag);
						playinfobarstatus = 0;
					}
					continue;
				}

				if(rcret == RCTIMEOUT) continue;

				//if(rcret == getrcconfigint("rcyellow", NULL))
				//	playrcyellow(file, NULL, playinfobarstatus, playertype, flag);
				
				//if(rcret == getrcconfigint("rctext", NULL) || rcret == getrcconfigint("rcsubtitel", NULL))
				//	playrctext(file, NULL, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcgreen", NULL))
				{
					playrcgreen(file, NULL, playinfobarstatus, playertype, flag);
					dvdchangevideo();
				}
					
				if(rcret == getrcconfigint("rcred", NULL))
					playrcred(file, NULL, playinfobarstatus, playertype, flag);

				if(rcret == getrcconfigint("rcinfo", NULL))
					playrcinfo(file, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcff", NULL))
					playrcff(file, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcfr", NULL))
					playrcfr(file, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcpause", NULL))
					playrcpause(file, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcplay", NULL))
					playrcplay(file, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rc1", NULL))
					playrcjumpr(file, NULL, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc4", NULL))
					playrcjumpr(file, NULL, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc7", NULL))
					playrcjumpr(file, NULL, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc3", NULL))
					playrcjumpf(file, NULL, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc6", NULL))
					playrcjumpf(file, NULL, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc9", NULL))
					playrcjumpf(file, NULL, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
			}
			//don't change this sleep, without this
			//the player stops to fast, and a last seek can
			//produce a segfault
playerend:
			sleep(1);
			dvdafterend();

			writevfdmenu("DVD");
			screenplayinfobar(file, NULL, 1, playertype, flag);

			if(startfile == NULL)
			{
				playstartservice();
				goto playerstart;
			}
			else
				break;
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
