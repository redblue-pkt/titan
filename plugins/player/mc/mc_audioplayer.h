#ifndef MC_AUDIOPLAYER_H
#define MC_AUDIOPLAYER_H

extern struct skin* skin;
extern struct screensaver* screensaver;

void screenmc_audioplayer()
{
	// workaround for grey background mvi
	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);

	readmediadb(getconfig("mediadbfile", NULL), 0, 0);

	char* filename = NULL, *currentdirectory = NULL, *selectedfile = NULL, *lastid = NULL, *tmpstr = NULL;
	int rcret = 0, rcwait = 1000, playerret = 0, flag = 2, skip = 0, eof = 0, playinfobarcount = 0, playinfobarstatus = 1, count = 0, tmpview = 0, playlist = 0, playertype = 0;

	tmpstr = ostrcat(getconfig("mc_ap_dirsort", NULL), NULL, 0, 0);
	addconfigtmp("dirsort", tmpstr);
	free(tmpstr), tmpstr = NULL;

	// main screen
	struct skin* apskin = getscreen("mc_audioplayer");
	struct skin* filelistpath = getscreennode(apskin, "filelistpath");
	struct skin* filelist = getscreennode(apskin, "filelist");
	struct skin* listbox = getscreennode(apskin, "listbox");
	struct skin* b2 = getscreennode(apskin, "b2");
	struct skin* b3 = getscreennode(apskin, "b3");
	struct skin* b4 = getscreennode(apskin, "b4");

//	apskin->bgcol = getskinconfigint("black", NULL);

	struct skin* title = getscreennode(apskin, "title");
	struct skin* thumb = getscreennode(apskin, "thumb");
	struct skin* actorstext = getscreennode(apskin, "actorstext");
	struct skin* actors = getscreennode(apskin, "actors");
	struct skin* genretext = getscreennode(apskin, "genretext");
	struct skin* genre = getscreennode(apskin, "genre");
	struct skin* yeartext = getscreennode(apskin, "yeartext");
	struct skin* year = getscreennode(apskin, "year");
	struct skin* realnametext = getscreennode(apskin, "realnametext");
	struct skin* realname = getscreennode(apskin, "realname");
	struct skin* albumtext = getscreennode(apskin, "albumtext");
	struct skin* album = getscreennode(apskin, "album");
	
	// infobar screen
	struct skin* infobar = getscreen("mc_audioplayer_infobar");
	struct skin* spos = getscreennode(infobar, "pos");
	struct skin* slen = getscreennode(infobar, "len");
	struct skin* sreverse = getscreennode(infobar, "reverse");
	struct skin* sprogress = getscreennode(infobar, "progress");
	struct skin* b12 = getscreennode(infobar, "b12");
	struct skin* b13 = getscreennode(infobar, "b13");

	if(getconfigint("mc_ap_uselastdir", NULL) == 1)
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_ap_path", NULL), 1, 0);
		selectedfile = ostrcat(selectedfile, getconfig("mc_ap_selectedfile", NULL), 1, 0);
	}
	else
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_ap_defaultdir", NULL), 1, 0);		
	}

	// enable listbox and set hidden
	listbox->aktpage = -1;
	listbox->aktline = 0;
	listbox->hidden = YES;

	// read configs
	int view = getconfigint("mc_ap_view", NULL);
	int screensaver_delay = getconfigint("screensaver_delay", NULL);

	// set allowed filemask
	char* filemask = NULL;
	if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
		filemask = ostrcat("*.aac *.flac *.mp3 *.wma *.m4a *.ogg *.wav *.ra *.amr *.m3u *.pls", NULL, 0, 0);
	else
		filemask = ostrcat("*.m3u *.pls *.mp3 *.flac *.ogg", NULL, 0, 0);
	
	// disable global transparent/hangtime
	setfbtransparent(255);
	status.hangtime = 99999;
	status.playspeed = 0, status.play = 0, status.pause = 0, status.random = 0;

	debug(50, "start screenmc_audioplayer view=%d", view);

	char* defaultmvi = NULL;
	defaultmvi = createpluginpath("/mc/skin/default.mvi", 0);
	singlepicstart(defaultmvi, 0);

	if(getconfigint("screensaver", NULL) == 1)
		initscreensaver();

	tmpview = view;
	mc_changeview(view, filelist, apskin, flag);
	thumb->hidden = YES;
	album->hidden = YES;
	title->hidden = YES;
	actors->hidden = YES;
	year->hidden = YES;
	realname->hidden = YES;
	genre->hidden = YES;
	albumtext->hidden = YES;
	actorstext->hidden = YES;
	yeartext->hidden = YES;
	realnametext->hidden = YES;
	genretext->hidden = YES;
	free(musicdat.thumb); musicdat.thumb = NULL;
	//test for SH4
	system("rm /tmp/mccover.jpg");
	// ende test
	free(musicdat.title); musicdat.title = NULL;
	free(musicdat.album); musicdat.album = NULL;
	free(musicdat.realname); musicdat.realname = NULL;
	free(musicdat.actors); musicdat.actors = NULL;
	free(musicdat.genre); musicdat.genre = NULL;
	free(musicdat.year); musicdat.year = NULL;
	
	musicdat.act = 1;

//	clearscreen(loadmediadb);
	getfilelist(apskin, filelistpath, filelist, currentdirectory, filemask, tmpview, selectedfile);
	addscreenrc(apskin, filelist);

	while(1)
	{
		if(status.play == 1 && count <= screensaver_delay)
			mc_audioplayer_infobar(apskin, infobar, spos, slen, sreverse, sprogress, b12, b13, NULL);

		rcret = waitrcext(apskin, rcwait, 0, tmpview);
		debug(50, "while status play=%d", status.play);

		if(rcret != RCTIMEOUT && rcret != getrcconfigint("rcvolup", NULL) && rcret != getrcconfigint("rcvoldown", NULL) && rcret != getrcconfigint("rcmute", NULL) && rcret != getrcconfigint("rcmute1", NULL))
		{
			if(count > screensaver_delay && screensaver != NULL)
			{
				if((rcret == getrcconfigint("rcpause", NULL)) || (rcret == getrcconfigint("rc1", NULL)) || (rcret == getrcconfigint("rc3", NULL)) || (rcret == getrcconfigint("rc4", NULL)) || (rcret == getrcconfigint("rc6", NULL)) || (rcret == getrcconfigint("rc7", NULL)) || (rcret == getrcconfigint("rc9", NULL)))
					drawscreen(infobar, 0, 0);
				else
				{
					if(screensaver != NULL && screensaver->type == 3)
						singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
					drawscreen(apskin, 0, 0);
				}
			}
			count = 0;
			rcwait = 1000;
		}
		else if(status.play == 1 && screensaver != NULL)
			count++;

		if(status.play == 1 && count > screensaver_delay && screensaver != NULL)
		{
			showscreensaver();
			rcwait = screensaver->speed;
		}

		if(tmpview == 3 && filelist->select != NULL && count < screensaver_delay)
		{
			char* pic = NULL;
			int len1 = 0;

			if(ostrcmp(lastid, filelist->select->name) != 0) 
			{                                
				free(lastid), lastid = NULL; 
				lastid = ostrcat(lastid, filelist->select->name, 1, 0);  
				if(filelist->select != NULL && filelist->select->input == NULL)
				{
					struct mediadb* mnode = getmediadb(filelistpath->text, filelist->select->name, 0);
					if(mnode != NULL)
					{
						if(mnode->id != NULL)
						{
							tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
							tmpstr = ostrcat(tmpstr, "/", 1, 0);																			
							tmpstr = ostrcat(tmpstr, mnode->id, 1, 0);
		
							pic = ostrcat(tmpstr, "_cover.jpg", 0, 0);
							free(tmpstr), tmpstr = NULL;
						}
	
						len1 = strlen(mnode->plot);
						if(mnode->plot != NULL && len1 != 0)
						{
							changetext(album, mnode->plot);
							album->hidden = NO;
							albumtext->hidden = NO;
							free(musicdat.album); musicdat.album = NULL;
							musicdat.album = ostrcat(mnode->plot, "", 0, 0);
						}
						else
						{
							album->hidden = YES;
							albumtext->hidden = YES;
							free(musicdat.album); musicdat.album = NULL;
						}
	
						len1 = strlen(mnode->plot);
						if(mnode->title != NULL && len1 != 0)
						{
							len1 = strlen(mnode->actors);
							if(mnode->actors != NULL && len1 != 0)					
							{
								tmpstr = ostrcat(tmpstr, mnode->actors, 1, 0);
								tmpstr = ostrcat(tmpstr, " - ", 1, 0);
								actors->hidden = NO;
								actorstext->hidden = NO;
							}
							else
							{
								actors->hidden = YES;
								actorstext->hidden = YES;
								free(musicdat.actors); musicdat.actors = NULL;
							}
							tmpstr = ostrcat(tmpstr, mnode->title, 1, 0);
	
							if(mnode->year != 0)
							{
								tmpstr = ostrcat(tmpstr, " (", 1, 0);
								tmpstr = ostrcat(tmpstr, oitoa(mnode->year), 1, 0);
								tmpstr = ostrcat(tmpstr, ")", 1, 0);
								year->hidden = NO;
								yeartext->hidden = NO;
							}
							else
							{
								year->hidden = YES;
								yeartext->hidden = YES;
								free(musicdat.year); musicdat.year = NULL;
							}
							if(tmpstr != NULL)
							{
								changetext(title, tmpstr);
								title->hidden = NO;
								free(musicdat.title); musicdat.title = NULL;
								musicdat.title = ostrcat(tmpstr, "", 0, 0);
							}
							else
							{
								changetext(title, filelist->select->name);
								title->hidden = NO;
								if(filelist != NULL)
								{
									free(musicdat.title); musicdat.title = NULL;
									musicdat.title = ostrcat(filelist->select->name, "", 0, 0);
								}
							}
							free(tmpstr), tmpstr = NULL;
	
							changetext(realname, filelist->select->name);
							realname->hidden = NO;
							realnametext->hidden = NO;
							if(filelist != NULL)
							{
								free(musicdat.realname); musicdat.realname = NULL;
								musicdat.realname = ostrcat(filelist->select->name, "", 0, 0);
							}
						}
						else
						{
							realname->hidden = YES;
							realnametext->hidden = YES;
							free(musicdat.realname); musicdat.realname = NULL;
							changetext(title, filelist->select->name);
							title->hidden = NO;
							if(filelist != NULL)
							{
								free(musicdat.title); musicdat.title = NULL;
								musicdat.title = ostrcat(filelist->select->name, "", 0, 0);
							}
						}					
	
						len1 = strlen(mnode->actors);
						if(mnode->actors != NULL && len1 != 0)
						{
							changetext(actors, mnode->actors);
							actors->hidden = NO;
							actorstext->hidden = NO;
							free(musicdat.actors); musicdat.actors = NULL;
							musicdat.actors = ostrcat(mnode->actors, "", 0, 0);
						}
						else
						{
							actors->hidden = YES;
							actorstext->hidden = YES;
							free(musicdat.actors); musicdat.actors = NULL;
						}
	
						len1 = strlen(mnode->genre);
						if(mnode->genre != NULL && len1 != 0)
						{
							changetext(genre, mnode->genre);
							genre->hidden = NO;
							genretext->hidden = NO;
							free(musicdat.genre); musicdat.genre = NULL;
							musicdat.genre = ostrcat(mnode->genre, "", 0, 0);
						}
						else
						{
							genre->hidden = YES;
							genretext->hidden = YES;
							free(musicdat.genre); musicdat.genre = NULL;
						}
	
						if(mnode->year != 0)
						{
							changetext(year, oitoa(mnode->year));
							year->hidden = NO;
							yeartext->hidden = NO;
							free(musicdat.year); musicdat.year = NULL;
							musicdat.year = ostrcat(oitoa(mnode->year), "", 0, 0);
						}
						else
						{
							year->hidden = YES;
							yeartext->hidden = YES;
							free(musicdat.year); musicdat.year = NULL;
						}
					}
					else	
					{
						thumb->hidden = YES;
						album->hidden = YES;
						title->hidden = YES;
						actors->hidden = YES;
						year->hidden = YES;
						realname->hidden = YES;
						genre->hidden = YES;
						albumtext->hidden = YES;
						actorstext->hidden = YES;
						yeartext->hidden = YES;
						realnametext->hidden = YES;
						genretext->hidden = YES;
						free(pic), pic = NULL;
						free(musicdat.thumb); musicdat.thumb = NULL;
						//test for SH4
						system("rm /tmp/mccover.jpg");
						// ende test
						free(musicdat.title); musicdat.title = NULL;
						free(musicdat.album); musicdat.album = NULL;
						free(musicdat.realname); musicdat.realname = NULL;
						free(musicdat.actors); musicdat.actors = NULL;
						free(musicdat.genre); musicdat.genre = NULL;
						free(musicdat.year); musicdat.year = NULL;
					}
	
					if(file_exist(pic))
					{
						free(musicdat.thumb); musicdat.thumb = NULL;
						musicdat.thumb = ostrcat(pic, NULL, 0, 0);
						//test for SH4
						tmpstr = ostrcat("cp ", musicdat.thumb, 0, 0);
						tmpstr = ostrcat(tmpstr, " /tmp/mccover.jpg", 1, 0);
						system(tmpstr);
						free(tmpstr); tmpstr=NULL;
						// ende test
						changepic(thumb, pic);
						thumb->hidden = NO;
					}
					else
					{
						thumb->hidden = YES;
						free(musicdat.thumb); musicdat.thumb = NULL;
						//test for SH4
						system("rm /tmp/mccover.jpg");
						// ende test
					}
	
					free(pic), pic = NULL;				
					drawscreen(apskin, 0, 0);
				}
				else
				{
					free(musicdat.thumb); musicdat.thumb = NULL;
					//test for SH4
					system("rm /tmp/mccover.jpg");
					// ende test
					free(musicdat.title); musicdat.title = NULL;
					free(musicdat.album); musicdat.album = NULL;
					free(musicdat.realname); musicdat.realname = NULL;
					free(musicdat.actors); musicdat.actors = NULL;
					free(musicdat.genre); musicdat.genre = NULL;
					free(musicdat.year); musicdat.year = NULL;
					thumb->hidden = YES;
					album->hidden = YES;
					title->hidden = YES;
					actors->hidden = YES;
					year->hidden = YES;
					realname->hidden = YES;
					genre->hidden = YES;
					albumtext->hidden = YES;
					actorstext->hidden = YES;
					yeartext->hidden = YES;
					realnametext->hidden = YES;
					genretext->hidden = YES;
					free(pic), pic = NULL;
					drawscreen(apskin, 0, 0);
	
				}
				if(status.play == 1)
					drawscreen(infobar, 0, 0);
			}
		}

		if (rcret == getrcconfigint("rcrecord", NULL))
		{
			system("grab -j 100");
			textbox(_("Message"), _("Shooting Background done !\nSave Screenshoot Path: /tmp/screenshot.jpg"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
		}
		else if(rcret == getrcconfigint("rc1", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, 10, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc4", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, 30, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc7", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc3", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, 10, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc6", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, 30, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc9", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcff", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcff(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, 2);
		}	
		else if(rcret == getrcconfigint("rcfr", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcfr(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, 2);
		}
		else if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
		{
			if(status.pause == 1)
			{
				if(screensaver != NULL && screensaver->type == 3)
					singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
				drawscreen(apskin, 0, 0);
			}
			if((status.play == 1) || (status.pause == 1) || (status.playspeed != 0))
				playrcpause(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcplay", NULL))
		{
			if((status.play == 1) || (status.pause == 1) || (status.playspeed != 0))
				playrcplay(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
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
			if(playlist == 0)
			{
				int sort = screendirsort();
				debug(50, "rcred: tmpsort=%d", sort);

				addconfiginttmp("dirsort", sort);
				mc_changeview(tmpview, filelist, apskin, flag);

				delownerrc(apskin);	
				getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->name);
				addscreenrc(apskin, filelist);
				drawscreen(apskin, 0, 0);
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

				view = getconfigint("mc_ap_view", NULL);
				printf("view in: %d\n", view);
				printf("tmpview in: %d\n", tmpview);

				screenmc_audioplayer_settings();
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);	
			
				if(view != getconfigint("mc_ap_view", NULL))
				{
					printf("view changed > change tmpview\n");
					tmpview = getconfigint("mc_ap_view", NULL);
				}

				if(getconfig("mc_ap_dirsort", NULL) != getconfig("dirsort", NULL))
				{
					char* tmpstr = NULL;
					tmpstr = ostrcat(getconfig("mc_ap_dirsort", NULL), NULL, 0, 0);
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
		else if(rcret == getrcconfigint("rcstop", NULL) || (rcret == getrcconfigint("rcexit", NULL) && status.play == 1))
		{

//			if((status.play == 1) || (status.pause == 1))
//			{
				debug(50, "rcstop: stopplayback");
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);
				playerstop();
				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
			
				filelist->hidden = NO;
				listbox->hidden = YES;
				changetext(filelistpath, _(getconfig("mc_ap_path", NULL)));
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

				writevfdmenu("AudioPlayer Filelist-Mode");
//			}
		}
		else if(rcret == getrcconfigint("rcexit", NULL))
		{
			debug(50, "exit - save mc_ap_path: %s", filelistpath->text);
			debug(50, "exit - save mc_ap_selectedfile: %s", filelistpath->text);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);

			if(playlist == 0)
			{
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_ap_path", NULL), filelistpath->text) != 0)
					addconfig("mc_ap_path", filelistpath->text);
				if(filelist->select != NULL && filelist->select->name != NULL && ostrcmp(getconfig("mc_ap_selectedfile", NULL), filelist->select->name) != 0)
					addconfig("mc_ap_selectedfile", filelist->select->name);

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
			if(playlist == 1 && listbox->select != NULL)
			{
				debug(50, "listbox->select->name: %s", listbox->select->name);
				filename = ostrcat("", listbox->select->name, 0, 0);

				changetext(b2, _("Playlist-Mode"));
			
				debug(50, "screensaver title: %s", listbox->select->text);
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = listbox->select->text;

				debug(50, "playerstop");
				playerstop();
				sleep(1);
				drawscreen(infobar, 0, 0);
				debug(50, "playerstart: %s", filename);
				eof = 0;
				
				free(status.playfile); status.playfile = NULL;
				status.playfile = ostrcat(filename, "", 0, 0);
				playerret = playerstart(filename);
				playwritevfd(filename, NULL);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
/*
						writevfdmenu("AudioPlayer Filelist-Mode");
						status.play = 0;
						playlist = 0;
						status.playspeed = 0;

						changetext(filelistpath, _(getconfig("mc_ap_path", NULL)));
						filelist->hidden = NO;
						listbox->hidden = YES;

						delownerrc(apskin);
						addscreenrc(apskin, filelist);
						drawscreen(apskin, 0, 0);
*/
// test
						status.play = 1;
						eof = 1;
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
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_ap_path", NULL), filelistpath->text) != 0)
					addconfig("mc_ap_path", filelistpath->text);
			
				debug(50, "filelist->select->text: %s", filelist->select->text);
				filename = createpath(filelistpath->text, filelist->select->name);
				printf("name: %s\n",filelist->select->name);
				printf("text: %s\n",filelist->select->text);

				if(cmpfilenameext(filename, ".rar") == 0 || cmpfilenameext(filename, ".iso") == 0 || cmpfilenameext(filename, ".img") == 0)
				{
					debug(50, "mc_mounter_main filename: %s", filename);
					//addconfig("mc_ap_path", filelistpath->text);
					currentdirectory = ostrcat("", getconfig("mc_ap_path", NULL), 0, 0);
					selectedfile = ostrcat(selectedfile, getconfig("mc_ap_selectedfile", NULL), 0, 0);

					mc_mounter_main(0,filename,filelistpath,filelist,apskin,filemask,tmpview,currentdirectory);
					debug(50, "mc_mounter_main done");
	
					singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
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

				drawscreen(infobar, 0, 0);
				debug(50, "playerstart: %s", filename);
				eof = 0;
#ifdef MIPSEL
				// gstplayer cancelt wenn man showiframe benutzt
				// mvi showiframe screensaver not working and playback ok
				servicestop(status.aktservice, 0, 4);
				// mvi showiframe screensaver working and playback canceld
				//servicestop(status.aktservice, 1, 1);	
#endif
				free(status.playfile); status.playfile = NULL;
				status.playfile = ostrcat(filename, "", 0, 0);
				playerret = playerstart(filename);
				playwritevfd(filename, NULL);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfdmenu("AudioPlayer Filelist-Mode");
						status.play = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						continue;
					}
				#endif

				status.play = 1;

				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
					
				if(status.mediadbfiles == 0 && getconfigint("mc_ap_autoscan", NULL) == 1 && !file_exist("/tmp/.autoscan"))
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
	
	musicdat.act = 0;
	
	deinitscreensaver();
		
	status.hangtime = getconfigint("hangtime", NULL);
	delconfigtmp("dirsort");

	delmarkedscreennodes(apskin, FILELISTDELMARK);
	delownerrc(apskin);
	clearscreen(apskin);

	free(lastid), lastid = NULL;
	free(filename), filename = NULL;
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

	writevfdmenu("Mediacenter");
	debug(50, "closed");
}

#endif

