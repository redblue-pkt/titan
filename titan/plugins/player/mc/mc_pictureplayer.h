#ifndef MC_PICTUREPLAYER_H
#define MC_PICTUREPLAYER_H

extern struct skin* skin;
extern struct screensaver* screensaver;

void screenmc_pictureplayer()
{
	char* filename = NULL;
	char* currentdirectory = NULL;
	char* selectedfile = NULL;

	int nextpic = 0, rcret = 0, rcwait = 1000, playerret = 0, flag = 3, skip = 0, eof = 0, playinfobarcount = 0, playinfobarstatus = 1, tmpview = 0, playlist = 0, playertype = 0, stopradio = 0;

	char* tmpstr = NULL;
	tmpstr = ostrcat(getconfig("mc_pp_dirsort", NULL), NULL, 0, 0);
	addconfigtmp("dirsort", tmpstr);
	free(tmpstr), tmpstr = NULL;

	// disable buffer
	addconfigtmp("playerbuffersize", "0");
				
	// workaround for grey background mvi
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0, 0);

	struct skin* loadmediadb = getscreen("loading");
	drawscreen(loadmediadb, 0, 0);

	// main screen
	struct skin* apskin = getscreen("mc_pictureplayer");
	struct skin* filelistpath = getscreennode(apskin, "filelistpath");
	struct skin* filelist = getscreennode(apskin, "filelist");
	struct skin* listbox = getscreennode(apskin, "listbox");
	struct skin* b2 = getscreennode(apskin, "b2");
	struct skin* b3 = getscreennode(apskin, "b3");
	struct skin* b4 = getscreennode(apskin, "b4");

//	apskin->bgcol = getskinconfigint("black", NULL);

	// pic screen
	struct skin* picscreen = getscreen("picscreen");
	struct skin* picture = getscreennode(picscreen, "picture");
	struct skin* picname = getscreennode(picscreen, "picname");

	if(getconfigint("mc_pp_uselastdir", NULL) == 1)
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_pp_path", NULL), 1, 0);
		selectedfile = ostrcat(selectedfile, getconfig("mc_pp_selectedfile", NULL), 1, 0);
	}
	else
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_pp_defaultdir", NULL), 1, 0);		
	}

	// enable listbox and set hidden
	listbox->aktpage = -1;
	listbox->aktline = 0;
	listbox->hidden = YES;

	// read configs
	int view = getconfigint("mc_pp_view", NULL);
	char* sound = getconfig("mc_pp_sound", NULL);

	// set allowed filemask
	char* filemask = "*.jpg *.png";
	
	// disable global transparent/hangtime
	setfbtransparent(255);
	status.hangtime = 99999;
	status.playspeed = 0, status.play = 0, status.pause = 0, status.random = 0;

	debug(50, "start screenmc_pictureplayer view=%d", view);

	char* defaultmvi = NULL;
	defaultmvi = createpluginpath("/mc/skin/default.mvi", 0);
	singlepicstart(defaultmvi, 0);

	if(getconfigint("screensaver", NULL) == 1)
		initscreensaver();

	tmpview = view;
	mc_changeview(view, filelist, apskin, flag);
//	clearscreen(loadmediadb);
	getfilelist(apskin, filelistpath, filelist, currentdirectory, filemask, tmpview, selectedfile);
	addscreenrc(apskin, filelist);

	// start radio musik on pictureplayer
	char* track = NULL;
	track = ostrcat(track, sound, 1, 0);

	if(stopradio == 0 && getconfig("mc_pp_sound", NULL) != NULL && ostrcmp(sound, "off") != 0)
	{
		playerret = playerstart(track);
		playwritevfd(track, NULL);

		#ifndef SIMULATE
			if(playerret != 0)
			{
				textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				playerstop();
				playerret = 0;
				stopradio = 1;
			}
		#endif
	}
	
	while(1)
	{
		rcret = waitrcext(apskin, rcwait, 0, tmpview);
		debug(50, "while status play=%d", status.play);
		nextpic = 0;
		if(playinfobarcount < getconfigint("mc_pp_interval", NULL) && status.play == 1)
			playinfobarcount ++;
		else if(status.play == 1)
		{
//			picplayer(picscreen, picture, picname, NULL, 0);	
			nextpic = 1;
		}

		if (rcret == getrcconfigint("rcrecord", NULL))
		{
			system("grab -j 100");
			textbox(_("Message"), _("Shooting Background done !\nSave Screenshoot Path: /tmp/screenshot.jpg"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
		}
		else if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
		{
			if((status.play == 1) || (status.pause == 1))
				playrcpause(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcplay", NULL))
		{
			if((status.play == 1) || (status.pause == 1))
				playrcplay(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if((rcret == getrcconfigint("rcchdown", NULL)) || (rcret == getrcconfigint("rcprev", NULL)))
		{
			if(status.play == 1)
				eof = 1;
		}
		else if((rcret == getrcconfigint("rcchup", NULL)) || (rcret == getrcconfigint("rcnext", NULL)))
		{
			if(status.play == 1)
				eof = 2;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(status.repeat == 0)
			{
				changetext(b4, _("Repeat-On"));
				drawscreen(apskin, 0, 0);
				status.repeat = 1;
			}
			else
			{
				status.repeat = 0;
				changetext(b4, _("Repeat"));
				drawscreen(apskin, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			playerrandom(apskin, filelist, listbox, b3, playlist, flag);
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			if(status.play == 1)
				playrcred(filename, NULL, NULL, NULL, playinfobarstatus, playertype, flag);
			else
			{
				if(playlist == 0)
				{
					int sort = screendirsort();
					debug(50, "rcred: tmpsort=%d", sort);
					addconfiginttmp("dirsort", sort);

					mc_changeview(view, filelist, apskin, flag);
					delownerrc(apskin);
					getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->name);
					addscreenrc(apskin, filelist);
					drawscreen(apskin, 0, 0);
				}
			}
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(status.play == 1)
				playrcgreen(filename, NULL, playinfobarstatus, playertype, flag);
			else
			{
				showplaylist(apskin, filelistpath, filelist, listbox, b2, 0, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);
				drawscreen(apskin, 0, 0);
			}

		}
		else if(rcret == getrcconfigint("rctext", NULL))
		{		
			if(status.play == 1)
				playrctext(filename, NULL, playinfobarstatus, playertype, flag);	
		}
		else if(rcret == getrcconfigint("rcmenu", NULL))
		{
			if(status.play == 0 && status.pause == 0)
			{
				debug(50, "rcmenu: settings");

				view = getconfigint("mc_pp_view", NULL);
				screenmc_pictureplayer_settings();
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);	

				if(view != getconfigint("mc_pp_view", NULL))
				{
					printf("view changed > change tmpview\n");
					tmpview = getconfigint("mc_pp_view", NULL);
				}

				if(getconfig("mc_pp_dirsort", NULL) != getconfig("dirsort", NULL))
				{
					char* tmpstr = NULL;
					tmpstr = ostrcat(getconfig("mc_pp_dirsort", NULL), NULL, 0, 0);
					addconfigtmp("dirsort", tmpstr);
					free(tmpstr), tmpstr = NULL;
				}

				sound = getconfig("mc_pp_sound", NULL);
				stopradio = 0;

				// start radio musik on pictureplayer
				playerstop();
				free(track), track = NULL;
				track = ostrcat("", sound, 0, 0);

				if(stopradio == 0 && getconfig("mc_pp_sound", NULL) != NULL && ostrcmp(sound, "off") != 0)
				{
					playerret = playerstart(track);
					playwritevfd(filename, NULL);

					#ifndef SIMULATE
						if(playerret != 0)
						{
							textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
							stopradio = 1;
						}
					#endif
				}
				else
					playerstop();

				mc_changeview(tmpview, filelist, apskin, flag);
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);	

				delownerrc(apskin);	
				getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->name);
				addscreenrc(apskin, filelist);

				drawscreen(apskin, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcstop", NULL) || (rcret == getrcconfigint("rcexit", NULL) && status.play == 1))
		{
			debug(50, "rcstop: stopplayback");

			picplayer(picscreen, picture, picname, NULL, 0);
			clearscreen(picscreen);
			eof = 0;

			singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
	
			apskin->hidden = NO;
			filelist->hidden = NO;
			listbox->hidden = YES;
			changetext(filelistpath, _(getconfig("mc_pp_path", NULL)));
			changetext(b2, _("Filelist-Mode"));

			// switch filelist
			delownerrc(apskin);
			addscreenrc(apskin, filelist);
			setfbtransparent(255);
			drawscreen(apskin, 0, 0);

			sleep(1);

			playinfobarcount = 0;
			playinfobarstatus = 1;
			status.playspeed = 0;
			status.pause = 0;
			status.play = 0;
			playlist = 0;
			playinfobarcount = 0;
			writevfdmenu("pictureplayer Filelist-Mode");
		}
		else if(rcret == getrcconfigint("rcexit", NULL))
		{
			debug(50, "exit - save mc_pp_path: %s", filelistpath->text);
			debug(50, "exit - save mc_pp_selectedfile: %s", filelistpath->text);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);

			if(playlist == 0)
			{
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_pp_path", NULL), filelistpath->text) != 0)
					addconfig("mc_pp_path", filelistpath->text);
				if(filelist->select != NULL && filelist->select->name != NULL && ostrcmp(getconfig("mc_pp_selectedfile", NULL), filelist->select->name) != 0)
					addconfig("mc_pp_selectedfile", filelist->select->name);
			}

			playerstop();
			picplayer(picscreen, picture, picname, NULL, 0);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);
			eof = 0;

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
			writevfdmenu("Mediacenter");
			playinfobarcount = 0;

			printf("exit: view=%d tmpview=%d\n", view, tmpview);			
			break;
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if(playlist == 1 && listbox->select != NULL)
			{
				filename = ostrcat("", listbox->select->name, 0, 0);

				changetext(b2, _("Playlist-Mode"));
			
				debug(50, "screensaver title: %s", listbox->select->text);
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = listbox->select->text;

				debug(50, "playerstop");
				playerstop();

				debug(50, "playerstart: %s", filename);
				eof = 0;

				delownerrc(apskin);
				setfbtransparent(255);

				if(stopradio == 0 && getconfig("mc_pp_sound", NULL) != NULL && ostrcmp(sound, "off") != 0)
				{
					playerret = playerstart(filename);
					playwritevfd(filename, NULL);

					#ifndef SIMULATE
						if(playerret != 0)
						{
							writevfdmenu("pictureplayer Filelist-Mode");
							status.play = 0;
							playlist = 0;
							status.playspeed = 0;
							textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

							changetext(filelistpath, _(getconfig("mc_pp_path", NULL)));
							filelist->hidden = NO;
							listbox->hidden = YES;
	
							addscreenrc(apskin, filelist);
							drawscreen(apskin, 0, 0);
							stopradio = 1;
							continue;
						}
					#endif
				}

				screenplayinfobar(filename, NULL, 0, playertype, 0);			
				status.play = 1;
			}
			else if(filelist->select != NULL && filelist->select->input != NULL)
			{
				// workaround dont open folder on rcchup
				if(skip == 1)
				{
					drawscreen(apskin, 0, 0);
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
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_pp_path", NULL), filelistpath->text) != 0)
					addconfig("mc_pp_path", filelistpath->text);

				debug(50, "filelist->select->text: %s", filelist->select->text);
				filename = createpath(filelistpath->text, filelist->select->name);
				printf("name: %s\n",filelist->select->name);
				printf("text: %s\n",filelist->select->text);

				if(cmpfilenameext(filename, ".rar") == 0 || cmpfilenameext(filename, ".iso") == 0 || cmpfilenameext(filename, ".img") == 0)
				{
					debug(50, "mc_mounter_main filename: %s", filename);
					//addconfig("mc_pp_path", filelistpath->text);
					currentdirectory = ostrcat("", getconfig("mc_pp_path", NULL), 0, 0);
					selectedfile = ostrcat(selectedfile, getconfig("mc_pp_selectedfile", NULL), 0, 0);

					mc_mounter_main(0,filename,filelistpath,filelist,apskin,filemask,tmpview,currentdirectory);
					debug(50, "mc_mounter_main done");
	
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
					continue;
				}
				else if(cmpfilenameext(filename, ".m3u") == 0 || cmpfilenameext(filename, ".pls") == 0)
				{
					showplaylist(apskin, filelistpath, filelist, listbox, b2, 1, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);
					continue;
				}

				changetext(b2, _("Filelist-Mode"));
			
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = (void*)filelist->select->text;

				delownerrc(apskin);
//				setfbtransparent(255);

				debug(50, "playerstart: %s", filename);
				eof = 0;

				playinfobarcount = 0, playinfobarstatus = 1;
				picplayer(picscreen, picture, picname, filename, 1);
				servicestop(status.aktservice, 1, 1);

				status.play = 1;
			}
		}
		
		if(playerisplaying() == 0)
		{
			playerstop();

			if(stopradio == 0 && getconfig("mc_pp_sound", NULL) != NULL && ostrcmp(sound, "off") != 0)
			{
				playerret = playerstart(track);
				playwritevfd(filename, NULL);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						stopradio = 1;
					}
				#endif
			}
		}

		if((eof >= 1) || (nextpic == 1))
		{
			if(status.play == 1)
			{
//				setfbtransparent(0);
//				apskin->hidden = NO;
//				drawscreen(skin, 0, 0);
				playereof(apskin, filelist, listbox, filelistpath, picscreen, picture, picname, b2, &skip, &eof, &playlist, playertype, flag);
			}
		}
	}

	// remove disable buffer
	delconfigtmp("playerbuffersize");

	deinitscreensaver();
	status.hangtime = getconfigint("hangtime", NULL);
	delconfigtmp("dirsort");

	delmarkedscreennodes(apskin, FILELISTDELMARK);
	delownerrc(apskin);
	clearscreen(apskin);
	clearscreen(picscreen);

	free(track), track = NULL;
	free(filename), filename = NULL;
	free(currentdirectory), currentdirectory = NULL;
	free(selectedfile), selectedfile = NULL;

	clearscreen(blackscreen);
	clearscreen(loadmediadb);
	
	writevfdmenu("Mediacenter");
	debug(50, "closed");
}

#endif
