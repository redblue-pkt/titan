#ifndef MC_VIDEOPLAYER_H
#define MC_VIDEOPLAYER_H

extern struct skin* skin;
extern struct screensaver* screensaver;

void screenmc_videoplayer()
{
	char* filename = NULL;
	char* currentdirectory = NULL;
	int rcret = 0, rcwait = 1000, playerret = 0, flag = 1, skip = 0, eof = 0, playinfobarcount = 0, playinfobarstatus = 1, tmpview = 1, playlist = 0;
	// workaround for grey background mvi
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0);

	// main screen
	struct skin* apskin = getscreen("mc_videoplayer");
	struct skin* filelistpath = getscreennode(apskin, "mc_filelistpath");
	struct skin* filelist = getscreennode(apskin, "mc_filelist");
	struct skin* listbox = getscreennode(apskin, "listbox");
	struct skin* b2 = getscreennode(apskin, "b2");
	struct skin* b3 = getscreennode(apskin, "b3");
	struct skin* b4 = getscreennode(apskin, "b4");

	currentdirectory = ostrcat(currentdirectory, getconfig("mc_videoplayerpath", NULL), 1, 0);

	// enable listbox and set hidden
	listbox->aktpage = -1;
	listbox->aktline = 0;
	listbox->hidden = YES;

	// read configs
	int style = getconfigint("style", NULL);
	int view = getconfigint("view", NULL);
	int skip13 = getconfigint("skip13", NULL);
	int skip46 = getconfigint("skip46", NULL);
	int skip79 = getconfigint("skip79", NULL);

	// set allowed filemask
	char* filemask = "*.m3u *.ifo *.rar *.iso *.img *.avi *.dat *.divx *.flv *.mkv *.m4v *.mp4 *.mov *.mpg *.mpeg *.mts *.m2ts *.pls *.trp *.ts *.vdr *.vob *.wmv *.rm";
	
	// disable global transparent/hangtime
	setfbtransparent(255);
	status.hangtime = 99999;
	status.playspeed = 0, status.play = 0, status.pause = 0, status.random = 0;

	debug(50, "start screenmc_videoplayer style=%d, view=%d", style, view);

	singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi");

	if(getconfigint("screensaver", NULL) == 1)
		initscreensaver();

	mc_changeview(view, filelist);

	getfilelist(apskin, filelistpath, filelist, currentdirectory, filemask, tmpview, NULL);
	addscreenrc(apskin, filelist);

	while(1)
	{
		rcret = waitrc(apskin, rcwait, 0);
		debug(50, "while status play=%d", status.play);

		if((status.play == 1) || (status.playspeed != 0))
		{
			playinfobarcount ++;
			if(playinfobarstatus > 0)
			{
				screenplayinfobar(filename, 0, 0);
			}
			if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
			{
				playinfobarstatus = 0;
				screenplayinfobar(NULL, 1, 0);
			}
		}

		if(rcret == getrcconfigint("rc1", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, skip13, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rc4", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, skip46, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rc7", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, skip79, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rc3", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, skip13, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rc6", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, skip46, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rc9", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, skip79, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcleft", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, -60, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcright", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, 60, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcdown", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, -600, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcup", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, 600, &playinfobarstatus, &playinfobarcount, flag);
		}

		else if(rcret == getrcconfigint("rcff", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcff(filename, &playinfobarstatus, &playinfobarcount, flag);
		}	
		else if(rcret == getrcconfigint("rcfr", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcfr(filename, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcplay", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcplay(filename, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcpause", NULL))
		{
			if((status.play == 1) || (status.pause == 1))
				playrcpause(filename, &playinfobarstatus, &playinfobarcount, flag);
		}
		else if(rcret == getrcconfigint("rcchdown", NULL))
		{
			if(status.play == 1)
				eof = 1;
		}
		else if(rcret == getrcconfigint("rcchup", NULL))
		{
			if(status.play == 1)
				eof = 2;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(status.play == 1)
				playrcblue(filename, playinfobarstatus, flag);
			else
			{
				if(status.repeat == 0)
				{
					changetext(b4, _("Repeat-On"));
					drawscreen(apskin, 0);
					status.repeat = 1;
				}
				else
				{
					status.repeat = 0;
					changetext(b4, _("Repeat"));
					drawscreen(apskin, 0);
				}
			}
		}

		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(status.play == 1)
				playrcyellow(filename, playinfobarstatus, flag);						
			else
				playerrandom(apskin, filelist, listbox, b3, playlist, flag);
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			if(status.play == 1)
				playrcred(filename, playinfobarstatus, flag);
			else
			{
				if(playlist == 0)
				{
					int sort = screenmc_sort();
					debug(50, "rcred: tmpsort=%d", sort);

					addconfiginttmp("dirsort", sort);
					mc_changeview(view, filelist);
					getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->text);
				}
			}
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(status.play == 1)
				playrcgreen(filename, playinfobarstatus, flag);
			else
			{
				showplaylist(apskin, filelistpath, filelist, listbox, b2, 0, &playlist, &eof, &filename, &currentdirectory, flag);
				drawscreen(apskin, 0);
				continue;			
			}
		}
		else if(rcret == getrcconfigint("rctext", NULL))
		{		
			if(status.play == 1)
				playrctext(filename, playinfobarstatus, flag);	
		}
		else if(rcret == getrcconfigint("rcmenu", NULL))
		{
			if(status.play == 0 && status.pause == 0)
			{
				debug(50, "rcmenu: settings");

				screenmc_videoplayer_settings();
				view = getconfigint("view", NULL);
				mc_changeview(view, filelist);

				drawscreen(apskin, 0);
			}
		}
		else if(rcret == getrcconfigint("rcinfo", NULL))
		{
			printf("Title: %s\n", playergetinfo("Title"));
			printf("Artist: %s\n", playergetinfo("Artist"));
			printf("Album: %s\n", playergetinfo("Album"));
			printf("Year: %s\n", playergetinfo("Year"));
			printf("Genre: %s\n", playergetinfo("Genre"));
			printf("Comment: %s\n", playergetinfo("Comment"));
			printf("Track: %s\n", playergetinfo("Track"));
			printf("Copyright: %s\n", playergetinfo("Copyright"));
			printf("TestLibEplayer: %s\n", playergetinfo("TestLibEplayer"));
		}
		else if(rcret == getrcconfigint("rcstop", NULL))
		{

//			if((status.play == 1) || (status.pause == 1))
//			{
				debug(50, "rcstop: stopplayback");
				playerstop();
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi");
	
				apskin->hidden = NO;
				filelist->hidden = NO;
				listbox->hidden = YES;
				changetext(filelistpath, _(getconfig("mc_videoplayerpath", NULL)));
				changetext(b2, _("Filelist-Mode"));

				// switch filelist
				delownerrc(apskin);
				addscreenrc(apskin, filelist);
				// show skin
				setfbtransparent(255);
				drawscreen(apskin, 0);

				sleep(1);

				playinfobarcount = 0;
				playinfobarstatus = 1;
				status.playspeed = 0;
				status.pause = 0;
				status.play = 0;
				playlist = 0;
				playinfobarcount = 0;
				writevfd("VideoPlayer Filelist-Mode");
//			}
		}
		else if(rcret == getrcconfigint("rcexit", NULL))
		{
			debug(50, "exit - save mc_videoplayerpath: %s", filelistpath->text);
			if(playlist == 0)
			{
				if(ostrcmp(getconfig("mc_videoplayerpath", NULL), filelistpath->text) != 0)
					addconfig("mc_videoplayerpath", filelistpath->text);
			}

			playerstop();
			// show skin
			setfbtransparent(255);
			sleep(1);
			apskin->hidden = NO;
			filelist->hidden = NO;
			listbox->hidden = YES;
			changetext(b2, _("Filelist-Mode"));
			changetext(b3, _("Random"));
			changetext(b4, _("Repeat"));										
			status.playspeed = 0;
			status.pause = 0;
			status.play = 0;
			status.random = 0;
			status.repeat = 0;
			playlist = 0;
			writevfd("Mediacenter");
			playinfobarcount = 0;
			break;
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
			{
				playrcok(filename, playinfobarstatus, flag);
				continue;
			}

			if(playlist == 1 && listbox->select != NULL)
			{
				filename = ostrcat("", listbox->select->name, 0, 0);

				changetext(b2, _("Playlist-Mode"));
			
				debug(50, "screensaver title: %s", listbox->select->text);
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = listbox->select->text;

				debug(50, "playerstop");
				playerstop();
//				sleep(1);
//				drawscreen(infobar, 0);

				debug(50, "playerstart: %s", filename);
				eof = 0;

				delownerrc(apskin);
				setfbtransparent(255);

				playerret = playerstart(filename);
				playwritevfd(filename);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfd("VideoPlayer Filelist-Mode");
						status.play = 0;
						playlist = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);

						changetext(filelistpath, _(getconfig("mc_videoplayerpath", NULL)));
						filelist->hidden = NO;
						listbox->hidden = YES;

						addscreenrc(apskin, filelist);
						drawscreen(apskin, 0);
						continue;
					}
				#endif

				screenplayinfobar(filename, 0, 0);			
				status.play = 1;
			}
			else if(filelist->select != NULL && filelist->select->input != NULL)
			{
				// workaround dont open folder on rcchup
				if(skip == 1)
				{
					drawscreen(apskin, 0);
					writerc(getrcconfigint("rcok", NULL));
					skip = 0;
				}
				else
				{
					debug(50, "mc_mounter_chk start");
					mc_mounter_chk(filelistpath);
					debug(50, "mc_mounter_chk done");
				}
			}
			else if(filelist->select != NULL && filelist->select->input == NULL)
			{
				if(ostrcmp(getconfig("mc_videoplayerpath", NULL), filelistpath->text) != 0)
					addconfig("mc_videoplayerpath", filelistpath->text);

				debug(50, "filelist->select->text: %s", filelist->select->text);
				filename = createpath(filelistpath->text, filelist->select->text);

				if(!strncmp(".rar",filename+strlen(filename)-4,4) || !strncmp(".iso",filename+strlen(filename)-4,4) || !strncmp(".img",filename+strlen(filename)-4,4))
				{
					debug(50, "mc_mounter_main filename: %s", filename);
					//addconfig("mc_videoplayerpath", filelistpath->text);
					currentdirectory = ostrcat("", getconfig("mc_videoplayerpath", NULL), 0, 0);

					mc_mounter_main(0,filename,filelistpath,filelist,apskin,filemask,tmpview,currentdirectory);
					debug(50, "mc_mounter_main done");
	
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi");
					continue;
				}
				else if(!strncmp(".m3u",filename+strlen(filename)-4,4) || !strncmp(".pls",filename+strlen(filename)-4,4)) 
				{
					showplaylist(apskin, filelistpath, filelist, listbox, b2, 1, &playlist, &eof, &filename, &currentdirectory, flag);
//					drawscreen(apskin, 0);
					continue;

				}

				changetext(b2, _("Filelist-Mode"));
			
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = (void*)filelist->select->text;

				debug(50, "playerstop");
				playerstop();
//				sleep(1);

				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0);

				delownerrc(apskin);
				setfbtransparent(255);

				debug(50, "playerstart: %s", filename);
				eof = 0;
				playerret = playerstart(filename);
				playwritevfd(filename);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfd("VideoPlayer Filelist-Mode");
						status.play = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
						addscreenrc(apskin, filelist);
//						drawscreen(apskin, 0);
						continue;
					}
				#endif
				screenplayinfobar(filename, 0, 0);
				status.play = 1;
			}
		}

		if(eof >=1 || playerisplaying() == 0)
		{
			if(status.play == 1)
			{
				setfbtransparent(0);
				apskin->hidden = NO;
				drawscreen(skin, 0);
				playereof(apskin, filelist, listbox, filelistpath, b2, NULL, NULL, NULL, &skip, &eof, &playlist, flag);
			}
		}
	}

	deinitscreensaver();
	status.hangtime = getconfigint("hangtime", NULL);
	delconfigtmp("dirsort");

	delmarkedscreennodes(apskin, FILELISTDELMARK);
	delownerrc(apskin);
	clearscreen(apskin);
	if(style == 1)
	{
		delmarkedpic(1010);
		delmarkedpic(1011);
		delmarkedpic(1012);
		delmarkedpic(1013);
	}

	free(filename), filename = NULL;
	free(currentdirectory), currentdirectory = NULL;

	writevfd("Mediacenter");
	debug(50, "closed");
}



#endif

