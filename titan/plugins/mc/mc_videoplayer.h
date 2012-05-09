#ifndef MC_VIDEOPLAYER_H
#define MC_VIDEOPLAYER_H

extern struct skin* skin;
extern struct screensaver* screensaver;

void screenmc_videoplayer()
{
	char* filename = NULL;
	char* tmppolicy = NULL;
	char* currentdirectory = NULL;
	char* selectedfile = NULL;
	int rcret = 0, rcwait = 1000, playerret = 0, flag = 1, skip = 0, eof = 0, playinfobarcount = 0, playinfobarstatus = 1, tmpview = 0, playlist = 0, playertype = 0;
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
	
	struct skin* skin_cover = getscreennode(apskin, "cover");
//	skin_cover->hidden = YES;
			
	currentdirectory = ostrcat(currentdirectory, getconfig("mc_videoplayerpath", NULL), 1, 0);
	selectedfile = ostrcat(selectedfile, getconfig("mc_selectedfile", NULL), 1, 0);

	// enable listbox and set hidden
	listbox->aktpage = -1;
	listbox->aktline = 0;
	listbox->hidden = YES;

	// read configs
	int view = getconfigint("vp_view", NULL);
	int skip13 = getconfigint("skip13", NULL);
	int skip46 = getconfigint("skip46", NULL);
	int skip79 = getconfigint("skip79", NULL);

	// save policy
	tmppolicy = getpolicy();
	
	// set allowed filemask
	char* filemask = NULL;
	if((status.expertmodus > 0) || (file_exist("/var/swap/etc/.mcfull")))
		filemask = ostrcat("*.m3u *.pls *.ifo *.rar *.iso *.img *.avi *.dat *.divx *.flv *.mkv *.m4v *.mp4 *.mov *.mpg *.mpeg *.mts *.m2ts *.trp *.ts *.vdr *.vob *.wmv *.rm", NULL, 0, 0);
	else
		filemask = ostrcat("*.m3u *.pls *.avi *.mkv *.mpg *.mpeg *.ts", NULL, 0, 0);
	
	// disable global transparent/hangtime
	setfbtransparent(255);
	status.hangtime = 99999;
	status.playspeed = 0, status.play = 0, status.pause = 0, status.random = 0;

	debug(50, "start screenmc_videoplayer view=%d", view);

	singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);

	if(getconfigint("screensaver", NULL) == 1)
		initscreensaver();

	tmpview = view;
	mc_changeview(view, filelist);

	getfilelist(apskin, filelistpath, filelist, currentdirectory, filemask, tmpview, selectedfile);
	addscreenrc(apskin, filelist);

	char* savecmd = NULL;
			
	while(1)
	{
		rcret = waitrc(apskin, rcwait, 0);
//		debug(50, "while status play=%d", status.play);

		if((status.play == 1) || (status.playspeed != 0))
		{
			playinfobarcount ++;
			if(playinfobarstatus > 0)
			{
				screenplayinfobar(filename, 0, playertype, 0);
			}
			if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
			{
				playinfobarstatus = 0;
				screenplayinfobar(NULL, 1, playertype, 0);
			}
		}
		else if(status.filelistextend == 5 && filelist->select != NULL)
		{
			char* cmd = NULL;
			cmd = ostrcat(cmd, filelist->select->filelist->imdbpath, 1, 0);
			cmd = ostrcat(cmd, ".jpg", 1, 0);			
			changepic(skin_cover, cmd);
			drawscreen(apskin, 0);

			free(cmd), cmd = NULL;
			cmd = ostrcat(cmd, filelist->select->filelist->imdbpath, 1, 0);
			cmd = ostrcat(cmd, ".backdrop.mvi", 1, 0);

			if(!file_exist(cmd)){
				free(cmd), cmd = NULL;
				cmd = ostrcat(cmd, "/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 1, 0);
			}

			if(savecmd == NULL)
			{
				singlepicstart(cmd, 0);
				free(savecmd), savecmd = NULL;
				savecmd = ostrcat(savecmd, cmd, 1, 0);
			}
			else
			{
				if(ostrcmp(savecmd, cmd) != 0)
				{
					singlepicstart(cmd, 0);
					free(savecmd), savecmd = NULL;
					savecmd = ostrcat(savecmd, cmd, 1, 0);
				}
			}
			free(cmd), cmd = NULL;
		}
		
		if(rcret == getrcconfigint("rc1", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc4", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc7", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc3", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc6", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc9", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcleft", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcright", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcdown", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, 600, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcup", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, 600, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}

		else if(rcret == getrcconfigint("rcff", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcff(filename, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}	
		else if(rcret == getrcconfigint("rcfr", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcfr(filename, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcplay", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcplay(filename, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcpause", NULL))
		{
			if((status.play == 1) || (status.pause == 1))
				playrcpause(filename, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcnext", NULL))
		{
			if(status.play == 1)
				eof = 1;
		}
		else if(rcret == getrcconfigint("rcprev", NULL))
		{
			if(status.play == 1)
				eof = 2;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(status.play == 1)
				playrcblue(filename, playinfobarstatus, playertype, flag);
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
				playrcyellow(filename, playinfobarstatus, playertype, flag);						
			else
				playerrandom(apskin, filelist, listbox, b3, playlist, flag);
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			if(status.play == 1)
				playrcred(filename, playinfobarstatus, playertype, flag);
			else
			{
				if(playlist == 0)
				{
					int sort = screenmc_sort();
					debug(50, "rcred: tmpsort=%d", sort);

					addconfiginttmp("dirsort", sort);
					mc_changeview(tmpview, filelist);

					delownerrc(apskin);	
					getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->text);
					addscreenrc(apskin, filelist);
					drawscreen(apskin, 0);
				}
			}
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(status.play == 1)
				playrcgreen(filename, playinfobarstatus, playertype, flag);
			else
			{
				showplaylist(apskin, filelistpath, filelist, listbox, b2, 0, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);
				drawscreen(apskin, 0);
				continue;			
			}
		}
		else if(rcret == getrcconfigint("rctext", NULL) || rcret == getrcconfigint("rcsubtitel", NULL))
		{		
			if(status.play == 1)
				playrctext(filename, playinfobarstatus, playertype, flag);	
		}
		else if(rcret == getrcconfigint("rcmenu", NULL))
		{
			if(status.play == 0 && status.pause == 0)
			{
				debug(50, "rcmenu: settings");
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
				view = getconfigint("vp_view", NULL);
				screenmc_videoplayer_settings();
				
				if(view != getconfigint("vp_view", NULL))
				{
					printf("view changed > change tmpview\n");
					tmpview = getconfigint("vp_view", NULL);
				}
				
				mc_changeview(tmpview, filelist);

				delownerrc(apskin);	
				getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->text);
				addscreenrc(apskin, filelist);
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
				playrcstop(playertype, flag);
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
	
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
				
				if(playertype == 1)
					playerafterendts();
				else
					playerafterend();
			
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
				if(ostrcmp(getconfig("mc_selectedfile", NULL), filelist->select->name) != 0)
					addconfig("mc_selectedfile", filelist->select->name);
			}

			playrcstop(playertype, flag);
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
	
			printf("exit: view=%d tmpview=%d\n", view, tmpview);			
			status.filelistextend = 0;
			break;
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
			{
				playrcok(filename, playinfobarstatus, playertype, flag);
				continue;
			}

			playinfobarcount = 0;
			playinfobarstatus = 1;
			status.playspeed = 0;
			status.pause = 0;
//			status.play = 0;
//			playlist = 0;
			playinfobarcount = 0;

			if(playlist == 1 && listbox->select != NULL)
			{
				debug(50, "listbox->select->name: %s", listbox->select->name);
				filename = ostrcat("", listbox->select->name, 0, 0);

				if(getconfigint("playertype", NULL) == 1 && (cmpfilenameext(filename, ".ts") == 0 || cmpfilenameext(filename, ".mts") == 0 || cmpfilenameext(filename, ".m2ts") == 0))
					playertype = 1;
				else
					playertype = 0;

				changetext(b2, _("Playlist-Mode"));
			
				debug(50, "screensaver title: %s", listbox->select->text);
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = listbox->select->text;

				debug(50, "playerstop");
				playrcstop(playertype, flag);
//				sleep(1);
//				drawscreen(infobar, 0);

				debug(50, "playerstart: %s", filename);
				eof = 0;

				delownerrc(apskin);
				setfbtransparent(255);
//////////
				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0);
				setfbtransparent(255);
				debug(50, "check");
				debug(50, "autostart_playlist: %d", getconfigint("vp_autostart_playlist", NULL));
				debug(50, "status.play: %d", status.play);
				debug(50, "flag: %d", flag);
///////////
				debug(50, "playertype: %d", playertype);
				if(playertype == 1)
					playerret = playerstartts(filename, 0);
				else
					playerret = playerstart(filename);

				playwritevfd(filename);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfd("VideoPlayer Filelist-Mode");
						status.play = 0;
						playlist = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

						changetext(filelistpath, _(getconfig("mc_videoplayerpath", NULL)));
						filelist->hidden = NO;
						listbox->hidden = YES;

						addscreenrc(apskin, filelist);
						drawscreen(apskin, 0);
						continue;
					}
				#endif

				screenplayinfobar(filename, 0, playertype, 0);			
				status.play = 1;
				
				free(status.playfile); status.playfile = NULL;
				status.playfile = ostrcat(filename, "", 0, 0);
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
				filename = createpath(filelistpath->text, filelist->select->name);
				printf("name: %s\n",filelist->select->name);
				printf("text: %s\n",filelist->select->text);


				if(getconfigint("playertype", NULL) == 1 && (cmpfilenameext(filename, ".ts") == 0 || cmpfilenameext(filename, ".mts") == 0 || cmpfilenameext(filename, ".m2ts") == 0))		
					playertype = 1;
				else
					playertype = 0;

				if(!strncmp(".rar",filename+strlen(filename)-4,4) || !strncmp(".iso",filename+strlen(filename)-4,4) || !strncmp(".img",filename+strlen(filename)-4,4))
				{
					debug(50, "mc_mounter_main filename: %s", filename);
					//addconfig("mc_videoplayerpath", filelistpath->text);
					currentdirectory = ostrcat("", getconfig("mc_videoplayerpath", NULL), 0, 0);
					selectedfile = ostrcat(selectedfile, getconfig("mc_selectedfile", NULL), 0, 0);

					mc_mounter_main(0,filename,filelistpath,filelist,apskin,filemask,tmpview,currentdirectory);
					debug(50, "mc_mounter_main done");
	
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
					continue;
				}
				else if(!strncmp(".m3u",filename+strlen(filename)-4,4) || !strncmp(".pls",filename+strlen(filename)-4,4)) 
				{
					showplaylist(apskin, filelistpath, filelist, listbox, b2, 1, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);

					if(getconfigint("vp_autostart_playlist", NULL) == 0)
						drawscreen(apskin, 0);
					continue;

				}

				changetext(b2, _("Filelist-Mode"));
			
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = (void*)filelist->select->text;

				debug(50, "playerstop");
				playrcstop(playertype, flag);
//				sleep(1);

				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0);

				delownerrc(apskin);
				setfbtransparent(255);

				debug(50, "playerstart: %s", filename);
				eof = 0;

				debug(50, "playertype: %d", playertype);
				if(playertype == 1)
					playerret = playerstartts(filename, 0);
				else
					playerret = playerstart(filename);

				playwritevfd(filename);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfd("VideoPlayer Filelist-Mode");
						status.play = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						addscreenrc(apskin, filelist);
//						drawscreen(apskin, 0);
						continue;
					}
				#endif
				screenplayinfobar(filename, 0, playertype, 0);
				status.play = 1;

				free(status.playfile); status.playfile = NULL;
				status.playfile = ostrcat(filename, "", 0, 0);
			}
		}

		if(eof >=1 || (playertype == 0 && playerisplaying() == 0) || (playertype == 1 && playerisplayingts() == 0))
		{
			if(status.play == 1)
			{
				setfbtransparent(0);
				apskin->hidden = NO;
				drawscreen(skin, 0);
				playereof(apskin, filelist, listbox, filelistpath, b2, NULL, NULL, NULL, &skip, &eof, &playlist, playertype, flag);
			}
		}
	}

	deinitscreensaver();
	status.hangtime = getconfigint("hangtime", NULL);
	delconfigtmp("dirsort");

	delmarkedscreennodes(apskin, FILELISTDELMARK);
	delownerrc(apskin);
	clearscreen(apskin);

	free(filename), filename = NULL;
	free(currentdirectory), currentdirectory = NULL;
	free(selectedfile), selectedfile = NULL;

	free(status.playfile); status.playfile = NULL;
	if(tmppolicy != NULL)
	{
		setpolicy(tmppolicy);
		free(tmppolicy);
	}

	if(playertype == 1)
		playerafterendts();
	else
		playerafterend();

			
	writevfd("Mediacenter");
	debug(50, "closed");
}



#endif

