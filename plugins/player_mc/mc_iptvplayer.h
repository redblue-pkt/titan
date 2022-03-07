#ifndef MC_IPTVPLAYER_H
#define MC_IPTVPLAYER_H

extern struct skin* skin;
extern struct screensaver* screensaver;

void screenmc_iptvplayer()
{
	// workaround for grey background mvi
	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);

	addconfigtmp("playerbuffersize", "0");

	readmediadb(getconfig("mediadbfile", NULL), 0, 0);

	char* filename = NULL, *title = NULL, *currentdirectory = NULL, *selectedfile = NULL, *lastid = NULL, *tmpstr = NULL, *showname = NULL;
	int rcret = 0, rcwait = 1000, playerret = 0, flag = 4, skip = 0, eof = 0, playinfobarcount = 0, playinfobarstatus = 1, count = 0, tmpview = 0, playlist = 0, playertype = 0;
	int videooff = 0;

	tmpstr = ostrcat(getconfig("mc_ip_dirsort", NULL), NULL, 0, 0);
	addconfigtmp("dirsort", tmpstr);
	free(tmpstr), tmpstr = NULL;

	// main screen
	struct skin* apskin = getscreen("mc_iptvplayer");
	struct skin* filelistpath = getscreennode(apskin, "filelistpath");
	struct skin* filelist = getscreennode(apskin, "filelist");
	struct skin* listbox = getscreennode(apskin, "listbox");
	struct skin* b2 = getscreennode(apskin, "b2");
	struct skin* b3 = getscreennode(apskin, "b3");
	struct skin* b4 = getscreennode(apskin, "b4");

	if(getconfigint("mc_ip_uselastdir", NULL) == 1)
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_ip_path", NULL), 1, 0);
		selectedfile = ostrcat(selectedfile, getconfig("mc_ip_selectedfile", NULL), 1, 0);
	}
	else
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_ip_defaultdir", NULL), 1, 0);		
	}

	// enable listbox and set hidden
	listbox->aktpage = -1;
	listbox->aktline = 0;
	listbox->hidden = YES;

	// read configs
	int view = getconfigint("mc_ip_view", NULL);
	int screensaver_delay = getconfigint("screensaver_delay", NULL);

	// set allowed filemask
	char* filemask = NULL;
	if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
		filemask = ostrcat("*.m3u *.pls *.mpg", NULL, 0, 0);
	else
		filemask = ostrcat("*.m3u *.pls *.mpg", NULL, 0, 0);
	
	// disable global transparent/hangtime
	setfbtransparent(255);
	status.hangtime = 99999;
	status.playspeed = 0, status.play = 0, status.pause = 0, status.random = 0;

	debug(50, "start screenmc_iptvplayer view=%d", view);

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

	while(1)
	{
//		if(status.play == 1 && count <= screensaver_delay)
//			mc_iptvplayer_infobar(apskin, infobar, spos, slen, sreverse, sprogress, b12, b13, NULL);

		rcret = waitrcext(apskin, rcwait, 0, tmpview);
//		debug(50, "while status play=%d", status.play);
		if(status.play == 1 || status.playspeed != 0)
		{
			playinfobarcount ++;
			if(playinfobarstatus > 0)
			{
				if(videooff == 0) screenplayinfobar(filename, showname, 0, playertype, 0);
			}
			if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
			{
				playinfobarstatus = 0;
				if(videooff == 0) screenplayinfobar(NULL, NULL, 1, playertype, 0);
			}
		}
		if (rcret == getrcconfigint("rcrecord", NULL))
		{
			system("grab -j 100");
			textbox(_("Message"), _("Shooting Background done !\nSave Screenshoot Path: /tmp/screenshot.jpg"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
		}
		else if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
		{
			if(status.pause == 1)
			{
				if(screensaver != NULL && screensaver->type == 3)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
				drawscreen(apskin, 0, 0);
			}
			if((status.play == 1) || (status.pause == 1) || (status.playspeed != 0))
				playrcpause(filename, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcplay", NULL))
		{
			if((status.play == 1) || (status.pause == 1) || (status.playspeed != 0))
				playrcplay(filename, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
			//showplaylist(1);
		}
		else if((rcret == getrcconfigint("rcchdown", NULL)) || (rcret == getrcconfigint("rcnext", NULL)))
		{
			if(status.play == 1)
				eof = 1;
		}
		else if((rcret == getrcconfigint("rcchup", NULL)) || (rcret == getrcconfigint("rcprev", NULL)))
		{
			if(status.play == 1)
				eof = 2;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			debug(50, "rcblue: repeat");

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
				playrcred(filename, NULL, playinfobarstatus, playertype, flag);
			else
			{
				if(playlist == 0)
				{
					int sort = screendirsort();
					debug(50, "rcred: tmpsort=%d", sort);

					// fixt grey if sorting >10s
					drawscreen(skin, 0, 0);
	
					addconfiginttmp("dirsort", sort);
					mc_changeview(tmpview, filelist, apskin, flag);

					delownerrc(apskin);	
					getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->name);
					addscreenrc(apskin, filelist);
					drawscreen(apskin, 0, 0);
				}
			}
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			showplaylist(apskin, filelistpath, filelist, listbox, b2, 0, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);
			drawscreen(apskin, 0, 0);
		}
		else if(rcret == getrcconfigint("rcmenu", NULL))
		{
			if(status.play == 0 && status.pause == 0)
			{
				debug(50, "rcmenu: settings");

				view = getconfigint("mc_ip_view", NULL);
				printf("view in: %d\n", view);
				printf("tmpview in: %d\n", tmpview);

				screenmc_iptvplayer_settings();
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);	
			
				if(view != getconfigint("mc_ip_view", NULL))
				{
					printf("view changed > change tmpview\n");
					tmpview = getconfigint("mc_ip_view", NULL);
				}

				if(getconfig("mc_ip_dirsort", NULL) != getconfig("dirsort", NULL))
				{
					char* tmpstr = NULL;
					tmpstr = ostrcat(getconfig("mc_ip_dirsort", NULL), NULL, 0, 0);
					addconfigtmp("dirsort", tmpstr);
					free(tmpstr), tmpstr = NULL;
				}

				printf("1view in: %d\n", view);
				printf("1tmpview in: %d\n", tmpview);
				
				mc_changeview(tmpview, filelist, apskin, flag);
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);	

				printf("2view in: %d\n", view);
				printf("2tmpview in: %d\n", tmpview);

				delownerrc(apskin);	
				getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->name);
				addscreenrc(apskin, filelist);
	
				screensaver_delay = getconfigint("screensaver_delay", NULL);
				deinitscreensaver();
				if(getconfigint("screensaver", NULL) == 1)
					initscreensaver();

				drawscreen(apskin, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcinfo", NULL))
		{
			if(status.play == 0 && status.pause == 0)
			{
				drawscreen(blackscreen, 0, 0);
//				if(filelist->select != NULL && filelist->select->input == NULL)
//				{
					filename = createpath(filelistpath->text, filelist->select->name);
					debug(133, "filename: %s", filename);				
					int check = playrcred(filename, NULL, playinfobarstatus, playertype, flag);
					if(check == 1)
						textbox(_("Message"), _("Update Iptv M3U ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);

//				}
			}
			else
				playrcplay(filename, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);

			if(status.play == 0 && status.pause == 0)
			{
				drawscreen(skin, 0, 0);
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

//			if((status.play == 1) || (status.pause == 1))
//			{
				debug(50, "rcstop: stopplayback");
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);
				playerstop();
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
			
				filelist->hidden = NO;
				listbox->hidden = YES;
				changetext(filelistpath, _(getconfig("mc_ip_path", NULL)));
				changetext(b2, _("Filelist-Mode"));

				// switch filelist
				delownerrc(apskin);
				addscreenrc(apskin, filelist);

				drawscreen(apskin, 0, 0);
				clearscreen(loadmediadb);
				clearscreen(blackscreen);
			
				sleep(1);
				status.playspeed = 0;
				status.pause = 0;
				status.play = 0;
				playlist = 0;

				writevfdmenu("iptvplayer Filelist-Mode");
//			}
		}
		else if(rcret == getrcconfigint("rcexit", NULL))
		{
			debug(50, "exit - save mc_ip_path: %s", filelistpath->text);
			debug(50, "exit - save mc_ip_selectedfile: %s", filelistpath->text);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);

			if(playlist == 0)
			{
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_ip_path", NULL), filelistpath->text) != 0)
					addconfig("mc_ip_path", filelistpath->text);
				if(filelist->select != NULL && filelist->select->name != NULL && ostrcmp(getconfig("mc_ip_selectedfile", NULL), filelist->select->name) != 0)
					addconfig("mc_ip_selectedfile", filelist->select->name);

			}

			servicestop(status.aktservice, 1, 1);	

			playerstop();
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);
	
			sleep(1);
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
			
			printf("exit: view=%d tmpview=%d\n", view, tmpview);
			unlink("/tmp/.autoscan");			
			break;
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
////////////
			playinfobarcount = 0;
			playinfobarstatus = 1;
			status.playspeed = 0;
			status.pause = 0;
//			status.play = 0;
//			playlist = 0;
			playinfobarcount = 0;
////////////
			if(playlist == 1 && listbox->select != NULL)
			{
				debug(50, "listbox->select->name: %s", listbox->select->name);
				filename = ostrcat(listbox->select->name, NULL, 0, 0);
				showname = ostrcat(listbox->select->text, NULL, 0, 0);

				changetext(b2, _("Playlist-Mode"));
			
				debug(50, "screensaver title: %s", listbox->select->text);
//				if(screensaver != NULL && screensaver->type == 0)
//					screensaver->value = listbox->select->text;

				debug(50, "playerstop");
				playerstop();
				sleep(1);
//				drawscreen(infobar, 0, 0);
				debug(50, "playerstart: %s", filename);
				eof = 0;

//////
				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0, 0);
				drawscreen(loadmediadb, 0, 0);
				setfbtransparent(255);
//////

                if (ostrstr(filename, "&tslivemode=1") == NULL)
                    filename = ostrcat(filename, "&tslivemode=1", 1, 0);		

				debug(50, "playerstart change: %s", filename);

				playerret = playerstart(filename);
				playwritevfd(filename, NULL);

//////
				clearscreen(loadmediadb);
				if(videooff == 0) screenplayinfobar(listbox->select->name, listbox->select->text, 0, playertype, 0);

				status.play = 1;
//////

				#ifndef SIMULATE
					if(playerret != 0)
					{
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
/*
						writevfdmenu("iptvplayer Filelist-Mode");
						status.play = 0;
						playlist = 0;
						status.playspeed = 0;

						changetext(filelistpath, _(getconfig("mc_ip_path", NULL)));
						filelist->hidden = NO;
						listbox->hidden = YES;

						delownerrc(apskin);
						addscreenrc(apskin, filelist);
						drawscreen(apskin, 0, 0);
*/
// test
						status.play = 1;
						eof = 2;
// test
						continue;
					}
				#endif
			
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

					char* checkautoscan = NULL;
					checkautoscan = readfiletomem("/tmp/.autoscan", 0);

					if(filelistpath->text != NULL && ostrcmp(checkautoscan, filelistpath->text) != 0)
						unlink("/tmp/.autoscan");

					free(checkautoscan), checkautoscan = NULL;
				}				
			}
			else if(filelist->select != NULL && filelist->select->input == NULL)
			{
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_ip_path", NULL), filelistpath->text) != 0)
					addconfig("mc_ip_path", filelistpath->text);
			
				debug(50, "filelist->select->text: %s", filelist->select->text);
				filename = createpath(filelistpath->text, filelist->select->name);
				printf("name: %s\n",filelist->select->name);
				printf("text: %s\n",filelist->select->text);

				if(cmpfilenameext(filename, ".rar") == 0 || cmpfilenameext(filename, ".iso") == 0 || cmpfilenameext(filename, ".img") == 0)
				{
					debug(50, "mc_mounter_main filename: %s", filename);
					//addconfig("mc_ip_path", filelistpath->text);
					currentdirectory = ostrcat("", getconfig("mc_ip_path", NULL), 0, 0);
					selectedfile = ostrcat(selectedfile, getconfig("mc_ip_selectedfile", NULL), 0, 0);

					mc_mounter_main(0,filename,filelistpath,filelist,apskin,filemask,tmpview,currentdirectory);
					debug(50, "mc_mounter_main done");
	
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
					continue;
				}
				else if(cmpfilenameext(filename, ".m3u") == 0 || cmpfilenameext(filename, ".pls") == 0)
				{
					showplaylist(apskin, filelistpath, filelist, listbox, b2, 1, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);
					drawscreen(apskin, 0, 0);
					continue;
				}

				changetext(b2, _("Filelist-Mode"));
			
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = (void*)filelist->select->text;

				debug(50, "playerstop");
				if((status.play == 1) || (status.pause == 1))
				{
					playerstop();
					sleep(1);
				}

//				drawscreen(infobar, 0, 0);
				debug(50, "playerstart: %s", filename);
				eof = 0;
#ifdef MIPSEL
				// gstplayer cancelt wenn man showiframe benutzt
				// mvi showiframe screensaver not working and playback ok
				servicestop(status.aktservice, 0, 4);
				// mvi showiframe screensaver working and playback canceld
				//servicestop(status.aktservice, 1, 1);	
#endif

				playerret = playerstart(filename);
				playwritevfd(filename, NULL);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfdmenu("iptvplayer Filelist-Mode");
						status.play = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						continue;
					}
				#endif

				status.play = 1;

				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
					
				if(status.mediadbfiles == 0 && getconfigint("mc_ip_autoscan", NULL) == 1 && !file_exist("/tmp/.autoscan"))
				{
					if(filelistpath->text != NULL)
					{
						mediadbscan(filelistpath->text, 1001, 1);
						writesys("/tmp/.autoscan", filelistpath->text, 0);
						status.mediadbfiles = findfiles(currentdirectory, 1, 1, 1, 1); //count only
					}
					else
					{
						mediadbscan(currentdirectory, 1001, 1);
						writesys("/tmp/.autoscan", currentdirectory, 0);
						status.mediadbfiles = findfiles(currentdirectory, 1, 1, 1, 1); //count only
					}
					printf("status.mediadbfiles: %d\n",status.mediadbfiles);
				}
			}
		}
		if(eof >=1 || playerisplaying() == 0)
		{
			if(status.play == 1)
				playereof(apskin, filelist, listbox, filelistpath, b2, NULL, NULL, NULL, &skip, &eof, &playlist, playertype, flag);
		}
	}

	deinitscreensaver();
		
	status.hangtime = getconfigint("hangtime", NULL);
	delconfigtmp("dirsort");

	delmarkedscreennodes(apskin, FILELISTDELMARK);
	delownerrc(apskin);
	clearscreen(apskin);

	free(lastid), lastid = NULL;
	free(filename), filename = NULL;
	free(showname), showname = NULL;
	free(currentdirectory), currentdirectory = NULL;
	free(selectedfile), selectedfile = NULL;

	if(status.mediadbthread == NULL)
	{
		if(status.writemediadb == 1)
			writemediadb(getconfig("mediadbfile", NULL), NULL);
		freemediadb(0);
	}

	clearscreen(blackscreen);
	clearscreen(loadmediadb);

	delconfigtmp("playerbuffersize");

	writevfdmenu("Mediacenter");
	debug(50, "closed");
}

#endif

