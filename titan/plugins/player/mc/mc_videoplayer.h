#ifndef MC_VIDEOPLAYER_H
#define MC_VIDEOPLAYER_H

extern struct skin* skin;
extern struct screensaver* screensaver;
extern struct mediadb* mediadb;

void screenmc_videoplayer()
{
	int videooff = 0;
	// workaround for grey background mvi
	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);

	readmediadb(getconfig("mediadbfile", NULL), 0, 0);

	char* lastid = NULL, *filename = NULL, *tmppolicy = NULL ,*currentdirectory = NULL, *selectedfile = NULL, *tmpstr = NULL;
	int rcret = 0, rcwait = 1000, playerret = 0, flag = 1, skip = 0, eof = 0, playinfobarcount = 0, playinfobarstatus = 1, tmpview = 0, playlist = 0, playertype = 0, mviwait = 0, mvinum = 0, exit = 0;

	int sort = getconfigint("mc_vp_dirsort", NULL);
	addconfiginttmp("dirsort", sort);

	// main screen
	struct skin* apskin = getscreen("mc_videoplayer");
	struct skin* filelistpath = getscreennode(apskin, "filelistpath");
	struct skin* filelist = getscreennode(apskin, "filelist");
	struct skin* listbox = getscreennode(apskin, "listbox");
	struct skin* b2 = getscreennode(apskin, "b2");
	struct skin* b3 = getscreennode(apskin, "b3");
	struct skin* b4 = getscreennode(apskin, "b4");
	struct skin* plot = getscreennode(apskin, "plot");
	struct skin* title = getscreennode(apskin, "title");
	struct skin* thumb = getscreennode(apskin, "thumb");
	struct skin* stars = getscreennode(apskin, "stars");

//	apskin->bgcol = getskinconfigint("black", NULL);
	
	if(getconfigint("mc_vp_uselastdir", NULL) == 1)
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_vp_path", NULL), 1, 0);
		selectedfile = ostrcat(selectedfile, getconfig("mc_vp_selectedfile", NULL), 1, 0);
	}
	else
	{
		currentdirectory = ostrcat(currentdirectory, getconfig("mc_vp_defaultdir", NULL), 1, 0);		
	}

	// enable listbox and set hidden
	listbox->aktpage = -1;
	listbox->aktline = 0;
	listbox->hidden = YES;

	// read configs
	int view = getconfigint("mc_vp_view", NULL);
	int skip13 = getconfigint("skip13", NULL);
	int skip46 = getconfigint("skip46", NULL);
	int skip79 = getconfigint("skip79", NULL);

	// save policy
	tmppolicy = getpolicy();

	// set allowed filemask
	char* filemask = NULL;
	if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
		filemask = ostrcat("*.avi *.mkv *.mp4 *.ts *.mov *.flv *.mpg *.m2ts *.vob *.evo *.wmv *.asf *.mp2 *.m4v *.fla *.divx *.dat *.mpeg *.trp *.mts *.vdr *.ogg *.wtv *.asx *.mvi *.ram *.rm *.3gp *.rmvb *.rm *.webm *.opus *.m3u8 *.mpd *.m3u *.pls *.ifo *.rar *.iso *.img *.part", NULL, 0, 0);
	else
		filemask = ostrcat("*.m3u *.pls *.avi *.mkv *.mpg *.mpeg *.ts *.mp2t", NULL, 0, 0);
	
	// disable global transparent/hangtime
//	setfbtransparent(255);
	status.hangtime = 99999;
	status.playspeed = 0, status.play = 0, status.pause = 0, status.random = 0;

	debug(50, "start screenmc_videoplayer view=%d", view);

//	singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
	char* defaultmvi = NULL;
	defaultmvi = createpluginpath("/mc/skin/default.mvi", 0);
	singlepicstart(defaultmvi, 0);

	if(getconfigint("screensaver", NULL) == 1)
		initscreensaver();

	tmpview = view;

	mc_changeview(view, filelist, apskin, flag);
	thumb->hidden = YES;
	plot->hidden = YES;
	title->hidden = YES;
	stars->hidden = YES;

//	clearscreen(loadmediadb);	
	getfilelist(apskin, filelistpath, filelist, currentdirectory, filemask, tmpview, selectedfile);
	addscreenrc(apskin, filelist);

	char* savecmd = NULL;

	if(!file_exist("/mnt/swapextensions/player"))   
		mkdir("/mnt/swapextensions/player", 0777);
	
	int refresh = 0;			
	while(1)
	{
		
		rcret = waitrcext(apskin, rcwait, 0, tmpview);
		if(filelist->select != NULL && filelist->select->input != NULL)
			refresh = 1;
		else
			refresh = 0;

#ifdef EXTEPLAYER3
		getsubtext();
#endif
		if(status.play == 1 || status.playspeed != 0)
		{
			playinfobarcount ++;
			if(playinfobarstatus > 0)
			{
				if(videooff == 0) screenplayinfobar(filename, NULL, 0, playertype, 0);
			}
			if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
			{
				playinfobarstatus = 0;
				if(videooff == 0) screenplayinfobar(NULL, NULL, 1, playertype, 0);
				status.writeplayersub = 1;
			}
		}
		else if(exit == 0 && tmpview == 3 && filelist->select != NULL && status.play == 0 && status.pause == 0)
		{
			char* cmd = NULL;
			char* pic = NULL;

			int waittime = 5, foundthumb = 0, foundplot = 0, foundtitle = 0, foundstars = 0;
			waittime = getconfigint("mc_vp_backdrop_interval", NULL);

			if(filelist->select != NULL && filelist->select->input == NULL)
			{
				struct mediadb* mnode = getmediadb(filelistpath->text, filelist->select->name, 0);

				if(mnode != NULL)
				{
					if(mnode->id != NULL)
					{
						if(ostrcmp(lastid, mnode->id) != 0)					
						{
							free(lastid), lastid = NULL;
							lastid = ostrcat(lastid, mnode->id, 1, 0);
							mvinum = 1;
							mviwait = waittime;
						}
						else
						{
							if(mvinum == mnode->backdropcount)
								mvinum = 1;

							if(mviwait > waittime && mvinum < mnode->backdropcount)
								mvinum++;
						}

						tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
						tmpstr = ostrcat(tmpstr, "/", 1, 0);																			
						tmpstr = ostrcat(tmpstr, mnode->id, 1, 0);
	
						pic = ostrcat(tmpstr, "_thumb.jpg", 0, 0);
						
						cmd = ostrcat(tmpstr, "_backdrop", 0, 0);
						cmd = ostrcat(cmd, oitoa(mvinum), 1, 0);
						cmd = ostrcat(cmd, ".mvi", 1, 0);
						free(tmpstr), tmpstr = NULL;
					}
					if(mnode->plot != NULL)
						changetext(plot, mnode->plot);
					if(mnode->title != NULL)
						changetext(title, mnode->title);

					if(mnode->poster == NULL)
					{
						thumb->hidden = YES;
						free(pic), pic = NULL;
						foundthumb = 0;
					}
					else
					{
						thumb->hidden = NO;
						changepic(thumb, pic);
						free(pic), pic = NULL;
						foundthumb = 1;
					}
					
					if(mnode->plot == NULL)
					{
						foundplot = 0;
						plot->hidden = YES;
					}
					else
					{
						foundplot = 1;
						plot->hidden = NO;
					}
					
					if(mnode->title == NULL)
					{
						foundtitle = 0;
						title->hidden = YES;
					}
					else
					{
						foundtitle = 1;
						title->hidden = NO;
					}
					
					if(mnode->rating != 0)
					{
						foundstars = 1;
						int rating = mnode->rating;
						if(rating == 10)
							changepic(stars, "%pluginpath%/mc/skin/stars10.png");
						else if(rating > 9)
							changepic(stars, "%pluginpath%/mc/skin/stars9x.png");
						else if(rating == 9)
							changepic(stars, "%pluginpath%/mc/skin/stars9.png");
						else if(rating > 8)
							changepic(stars, "%pluginpath%/mc/skin/stars8x.png");
						else if(rating == 8)
							changepic(stars, "%pluginpath%/mc/skin/stars8.png");
						else if(rating > 7)
							changepic(stars, "%pluginpath%/mc/skin/stars7x.png");
						else if(rating == 7)
							changepic(stars, "%pluginpath%/mc/skin/stars7.png");
						else if(rating > 6)
							changepic(stars, "%pluginpath%/mc/skin/stars6x.png");
						else if(rating == 6)
							changepic(stars, "%pluginpath%/mc/skin/stars6.png");
						else if(rating > 5)
							changepic(stars, "%pluginpath%/mc/skin/stars5x.png");
						else if(rating == 5)
							changepic(stars, "%pluginpath%/mc/skin/stars5.png");
						else if(rating > 5)
							changepic(stars, "%pluginpath%/mc/skin/stars5x.png");
						else if(rating == 5)
							changepic(stars, "%pluginpath%/mc/skin/stars5.png");
						else if(rating > 4)
							changepic(stars, "%pluginpath%/mc/skin/stars4x.png");
						else if(rating == 4)
							changepic(stars, "%pluginpath%/mc/skin/stars4.png");
						else if(rating > 3)
							changepic(stars, "%pluginpath%/mc/skin/stars3x.png");
						else if(rating == 3)
							changepic(stars, "%pluginpath%/mc/skin/stars3.png");
						else if(rating > 2)
							changepic(stars, "%pluginpath%/mc/skin/stars2x.png");
						else if(rating == 2)
							changepic(stars, "%pluginpath%/mc/skin/stars2.png");
						else if(rating > 1)
							changepic(stars, "%pluginpath%/mc/skin/stars1x.png");
						else if(rating == 1)
							changepic(stars, "%pluginpath%/mc/skin/stars1.png");
						else if(rating > 0)
							changepic(stars, "%pluginpath%/mc/skin/stars0x.png");
						else
							changepic(stars, NULL);
						stars->hidden = NO;
					}
					else
					{
						foundstars = 0;
						changepic(stars, NULL);
						stars->hidden = YES;
					}
				}
				else	
				{
					refresh = 1;				
					foundthumb = 0;
					foundplot = 0;
					foundtitle = 0;
					foundstars = 0;
					thumb->hidden = YES;
					plot->hidden = YES;
					title->hidden = YES;
					stars->hidden = YES;
					free(pic), pic = NULL;
				}
					
				drawscreen(apskin, 0, 0);
			}

			debug(50, "cmd: %s", cmd);	
			if(!file_exist(cmd))
			{
				free(cmd), cmd = NULL;
//				cmd = ostrcat(cmd, "/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 1, 0);
				cmd = ostrcat(cmd, defaultmvi, 1, 0);
				mviwait = waittime;
				if(foundthumb == 0)
					thumb->hidden = YES;
				if(foundplot == 0)
					plot->hidden = YES;
				if(foundtitle == 0)
					title->hidden = YES;
				if(foundstars == 0)					
					stars->hidden = YES;

				drawscreen(apskin, 0, 0);
			}

			if(savecmd == NULL)
			{
				singlepicstart(cmd, 0);
				free(savecmd), savecmd = NULL;
				savecmd = ostrcat(savecmd, cmd, 1, 0);
			}
			else
			{
				if((refresh == 1 ) || (exit == 0 && ostrcmp(savecmd, cmd) != 0 && mviwait > waittime - 1))
				{
					singlepicstart(cmd, 0);
					free(savecmd), savecmd = NULL;
					savecmd = ostrcat(savecmd, cmd, 1, 0);
					mviwait = 0;
				}
				else if(waittime == 0)
				{
					debug(50, "disable Backdrop Interval");
				}
				else
					mviwait++;
			}
			free(cmd), cmd = NULL;
		}

		if (rcret == getrcconfigint("rcrecord", NULL))
		{
			if(getconfigint("mc_vp_recordpicture", NULL) == 1)
            {
            	status.mediadbthread = NULL;
            	if(status.mediadbthread == NULL)
                {
                    if(status.pause == 1)
                        drawscreen(skin, 0, 0);
                	struct stimerthread *recthumbfirstthread = NULL;
                    status.mcaktiv = 2;
		            recthumbfirstthread = addtimer(&createrecthumbfirstthread, START, 1000, 1, (void*)ostrcat(filelistpath->text, NULL, 0, 0), (void*)ostrcat(filelist->select->name, NULL, 0, 0), NULL);

			        //wait for recthumblastthread end before shutdown
			        int count = 0;
			        while(gettimer(recthumbfirstthread) != NULL && count < 60)
			        {
				        sleep(1);
				        count++;
			        }
                }
            }
            char* cmd = NULL;
	        if(checkchipset("3798MV200") == 1)
        		cmd = ostrcat("grab -v -j 100 -r 960 > /tmp/screenshot.jpg", NULL, 0, 0);
            else
        		cmd = ostrcat("grab -v -j 100 -r 160:120 /tmp/screenshot.jpg", NULL, 0, 0);
		    debug(133, "cmd: %s", cmd);
            system(cmd);
            free(cmd), cmd = NULL;

			textbox(_("Message"), _("Shooting Background done !\nSave Screenshoot Path: /tmp/screenshot.jpg"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		}
		else if(rcret == getrcconfigint("rc2", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpto(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc1", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc4", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc7", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc3", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc6", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rc9", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcleft", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcright", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpf(filename, NULL, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcdown", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcjumpr(filename, NULL, 600, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcup", NULL))
		{
			if(status.play == 1 || status.playspeed != 0 || status.pause == 1)
				playrcjumpf(filename, NULL, 600, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}

		else if(rcret == getrcconfigint("rcff", NULL))
		{
			if(status.play == 1 || status.playspeed != 0 || status.pause == 1)
				playrcff(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}	
		else if(rcret == getrcconfigint("rcfr", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
				playrcfr(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		
		else if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
		{
			if((status.play == 1) || (status.pause == 1) || (status.playspeed != 0))
				playrcpause(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcplay", NULL))
		{
			if((status.play == 1) || (status.pause == 1) || (status.playspeed != 0))
				playrcplay(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);
		}
		else if(rcret == getrcconfigint("rcnext", NULL) || rcret == getrcconfigint("rcchdown", NULL))
		{
			if(status.play == 1)
				eof = 1;
		}
		else if(rcret == getrcconfigint("rcprev", NULL) || rcret == getrcconfigint("rcchup", NULL))
		{
			if(status.play == 1)
				eof = 2;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(status.play == 1)
				playrcblue(filename, NULL, playinfobarstatus, playertype, flag);
			else
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
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(status.play == 1)
				playrcyellow(filename, NULL, playinfobarstatus, playertype, flag);						
			else
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
					sort = screendirsort();
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
			if(status.play == 1)
				playrcgreen(filename, NULL, playinfobarstatus, playertype, flag);
			else
			{
				showplaylist(apskin, filelistpath, filelist, listbox, b2, 0, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);
				drawscreen(apskin, 0, 0);
				continue;			
			}
		}
		else if(rcret == getrcconfigint("rctext", NULL) || rcret == getrcconfigint("rcsubtitel", NULL))
		{		
			if(status.play == 1)
				playrctext(filename, NULL, playinfobarstatus, playertype, flag);	
		}
		else if(rcret == getrcconfigint("rcmenu", NULL))
		{
			if(status.play == 0 && status.pause == 0)
			{
				refresh = 1;
				debug(50, "rcmenu: settings");
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
				singlepicstart(defaultmvi, 0);
				view = getconfigint("mc_vp_view", NULL);
				printf("view in: %d\n", view);
				printf("tmpview in: %d\n", tmpview);

				screenmc_videoplayer_settings();
				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);				
				if(view != getconfigint("mc_vp_view", NULL))
				{
					printf("view changed > change tmpview\n");
					tmpview = getconfigint("mc_vp_view", NULL);
				}

				if(getconfig("mc_vp_dirsort", NULL) != getconfig("dirsort", NULL))
				{
					char* tmpstr = NULL;
					tmpstr = ostrcat(getconfig("mc_vp_dirsort", NULL), NULL, 0, 0);
					addconfigtmp("dirsort", tmpstr);
					free(tmpstr), tmpstr = NULL;
				}

				printf("1view in: %d\n", view);
				printf("1tmpview in: %d\n", tmpview);
				
                addconfiginttmp("dirsort", sort);
				mc_changeview(tmpview, filelist, apskin, flag);

				drawscreen(blackscreen, 0, 0);
				drawscreen(loadmediadb, 0, 0);
				printf("2view in: %d\n", view);
				printf("2tmpview in: %d\n", tmpview);
	
				delownerrc(apskin);
				getfilelist(apskin, filelistpath, filelist, filelistpath->text, filemask, tmpview, filelist->select->name);
				addscreenrc(apskin, filelist);
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
					playrcred(filename, NULL, NULL, NULL, playinfobarstatus, playertype, flag);
//				}
			}
			else
				playrcplay(filename, NULL, &playinfobarstatus, &playinfobarcount, playertype, flag);

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
		else if(rcret == getrcconfigint("rcpower", NULL) && status.play == 1 && videooff == 0)
		{
			videooff = 1;
			drawscreen(blackscreen, 0, 0);
		}
		else if(rcret == getrcconfigint("rcexit", NULL) && videooff == 1)
		{
			videooff = 0;
			drawscreen(skin, 0, 0);
		}
		else if(rcret == getrcconfigint("rcstop", NULL) || (rcret == getrcconfigint("rcexit", NULL) && status.play == 1))
		{
//		if(dname != NULL && filename != NULL && getconfigint("recordpicture", NULL) == 1)
//			recthumblastthread = addtimer(&createrecthumblastthread, START, 1000, 1, (void*)ostrcat(filelistpath->text, NULL, 0, 0), (void*)ostrcat(filelist->select->name, NULL, 0, 0), NULL);
/*
				//wait for recthumblastthread end before shutdown
				int count = 0;
				while(gettimer(recthumblastthread) != NULL && count < 60)
				{
					sleep(1);
					count++;
				}
*/
			debug(50, "rcstop: stopplayback");
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);
			if(status.play == 1 && playertype == 0)
			{
				char* tmpfilename = ostrcat(filename, NULL, 0, 0);
				char* fileseek = ostrcat("/mnt/swapextensions/player/", basename(tmpfilename), 0, 0);
				fileseek = ostrcat(fileseek, ".se", 0, 0);
				FILE* fbseek = fopen(fileseek, "w");
				if(fbseek != NULL)
				{
					off64_t pos = playergetpts() / 90000;
					fprintf(fbseek,"%lld", pos);
					fclose(fbseek);
				}
				free(fileseek), fileseek = NULL;
				free(tmpfilename), tmpfilename = NULL;
			}
			
//			servicestop(status.aktservice, 1, 1);
			playrcstop(playertype, flag);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);
			sleep(2);
//			singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
			singlepicstart(defaultmvi, 0);

			// show skin
			setfbtransparent(255);

			apskin->hidden = NO;
			filelist->hidden = NO;
			listbox->hidden = YES;
			changetext(filelistpath, _(getconfig("mc_vp_path", NULL)));
			changetext(b2, _("Filelist-Mode"));

			// switch filelist
			delownerrc(apskin);
			addscreenrc(apskin, filelist);
			// show skin
			drawscreen(apskin, 0, 0);

			setfbtransparent(255);
			clearscreen(loadmediadb);
			clearscreen(blackscreen);

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
		
			writevfdmenu("VideoPlayer Filelist-Mode");
		}
		else if(rcret == getrcconfigint("rcexit", NULL))
		{
			videooff = 0;
			exit = 1;
			debug(50, "exit - save mc_vp_path: %s", filelistpath->text);
			debug(50, "exit - save mc_vp_selectedfile: %s", filelist->select->name);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);

			if(playlist == 0)
			{
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_vp_path", NULL), filelistpath->text) != 0)
					addconfig("mc_vp_path", filelistpath->text);
				if(filelist->select != NULL && filelist->select->name != NULL && ostrcmp(getconfig("mc_vp_selectedfile", NULL), filelist->select->name) != 0)
					addconfig("mc_vp_selectedfile", filelist->select->name);
			}

			if(status.play == 1 && playertype == 0)
			{
				char* tmpfilename = ostrcat(filename, NULL, 0, 0);
				char* fileseek = ostrcat("/mnt/swapextensions/player/", basename(tmpfilename), 0, 0);
				fileseek = ostrcat(fileseek, ".se", 0, 0);
				FILE* fbseek = fopen(fileseek, "w");
				if(fbseek != NULL)
				{
					off64_t pos = playergetpts() / 90000;
					fprintf(fbseek,"%lld", pos);
					fclose(fbseek);
				}
				free(fileseek), fileseek = NULL;
				free(tmpfilename), tmpfilename = NULL;
			}

//			setfbtransparent(0);
			servicestop(status.aktservice, 1, 1);
			playrcstop(playertype, flag);
			drawscreen(blackscreen, 0, 0);
			drawscreen(loadmediadb, 0, 0);
			sleep(2);
//			singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
			singlepicstart(defaultmvi, 0);

			// show skin
			setfbtransparent(255);
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
			unlink("/tmp/.autoscan");
			break;
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if((status.play == 1) || (status.playspeed != 0))
			{
				playrcok(filename, NULL, playinfobarstatus, playertype, flag);
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
//				drawscreen(infobar, 0, 0);

				debug(50, "playerstart: %s", filename);
				eof = 0;

				delownerrc(apskin);
//				setfbtransparent(255);
//////////
				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0, 0);
//				setfbtransparent(255);
				debug(50, "check");
				debug(50, "autostart_playlist: %d", getconfigint("mc_vp_autostart_playlist", NULL));
				debug(50, "status.play: %d", status.play);
				debug(50, "flag: %d", flag);
///////////
				debug(50, "playertype: %d", playertype);
				if(playertype == 1)
					playerret = playerstartts(filename, 0);
				else
					playerret = playerstart(filename);

				playwritevfd(filename, NULL);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfdmenu("VideoPlayer Filelist-Mode");
						status.play = 0;
						playlist = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);

						changetext(filelistpath, _(getconfig("mc_vp_path", NULL)));
						filelist->hidden = NO;
						listbox->hidden = YES;

						addscreenrc(apskin, filelist);
						drawscreen(apskin, 0, 0);
						continue;
					}
				#endif

				if(videooff == 0) screenplayinfobar(filename, NULL, 0, playertype, 0);			
				status.play = 1;
				
				free(status.playfile); status.playfile = NULL;
				status.playfile = ostrcat(filename, "", 0, 0);
			}
			else if(filelist->select != NULL && filelist->select->input != NULL)
			{

				char* tmpfilename = ostrcat(filelistpath->text, NULL, 0, 0);			

				// workaround dont open folder on rcchup
				if(skip == 1)
				{
					drawscreen(apskin, 0, 0);
					writerc(getrcconfigint("rcok", NULL));
					skip = 0;
				}
				else if(!ostrncmp("video_ts", string_tolower(basename(tmpfilename)), 8))
				{
					struct skin* dvdplayer = getplugin("DVD Player");
					if(dvdplayer != NULL)
					{
						if(textbox(_("Message"), _("Found VIDEO_TS Folder, start with DVD-Player ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0) == 1)
						{
							void (*startplugin)(char*, int);
							startplugin = dlsym(dvdplayer->pluginhandle, "screendvdplay");
							if(startplugin != NULL)
							{
								debug(50, "filelist->select->name: %s", filelist->select->name);
								filename = createpath(filelistpath->text, filelist->select->name);
								printf("start dvdplayer plugin\n");
								startplugin(filename,0);
							}
						}
						drawscreen(apskin, 0, 0);
//						singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
						singlepicstart(defaultmvi, 0);
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
				free(tmpfilename), tmpfilename = NULL;
			}
			else if(filelist->select != NULL && filelist->select->input == NULL)
			{
				if(filelistpath->text != NULL && ostrcmp(getconfig("mc_vp_path", NULL), filelistpath->text) != 0)
					addconfig("mc_vp_path", filelistpath->text);

				debug(50, "filelist->select->text: %s", filelist->select->text);
				filename = createpath(filelistpath->text, filelist->select->name);

				if(getconfigint("playertype", NULL) == 1 && (cmpfilenameext(filename, ".ts") == 0 || cmpfilenameext(filename, ".mts") == 0 || cmpfilenameext(filename, ".m2ts") == 0))
					playertype = 1;
				else
					playertype = 0;

				if(cmpfilenameext(filename, ".rar") == 0 || cmpfilenameext(filename, ".iso") == 0 || cmpfilenameext(filename, ".img") == 0)
				{
					debug(50, "mc_mounter_main filename: %s", filename);
					//addconfig("mc_vp_path", filelistpath->text);
					currentdirectory = ostrcat("", getconfig("mc_vp_path", NULL), 0, 0);
					selectedfile = ostrcat(selectedfile, getconfig("mc_vp_selectedfile", NULL), 0, 0);

					mc_mounter_main(0,filename,filelistpath,filelist,apskin,filemask,tmpview,currentdirectory);
					debug(50, "mc_mounter_main done");

//					singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/default.mvi", 0);
					singlepicstart(defaultmvi, 0);
					continue;
				}
				else if(cmpfilenameext(filename, ".m3u") == 0 || cmpfilenameext(filename, ".pls") == 0)
				{
					showplaylist(apskin, filelistpath, filelist, listbox, b2, 1, &playlist, &eof, &filename, &currentdirectory, &playertype, flag);

					if(getconfigint("mc_vp_autostart_playlist", NULL) == 0)
						drawscreen(apskin, 0, 0);
					continue;

				}

				changetext(b2, _("Filelist-Mode"));
			
				if(screensaver != NULL && screensaver->type == 0)
					screensaver->value = (void*)filelist->select->text;

				setfbtransparent(0);

//				debug(50, "playerstop");
//				playrcstop(playertype, flag);

				delownerrc(apskin);

				debug(50, "playerstart: %s", filename);
				eof = 0;

				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0, 0);
				drawscreen(loadmediadb, 0, 0);
				setfbtransparent(255);
	
				debug(50, "playertype: %d", playertype);
				if(playertype == 1)
					playerret = playerstartts(filename, 0);
				else
					playerret = playerstart(filename);

				playwritevfd(filename, NULL);

				#ifndef SIMULATE
					if(playerret != 0)
					{
						writevfdmenu("VideoPlayer Filelist-Mode");
						status.play = 0;
						status.playspeed = 0;
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
						addscreenrc(apskin, filelist);
//						drawscreen(apskin, 0, 0);
						continue;
					}
					else
					{
						if(playertype == 0 && getconfigint("showlastpos", NULL) == 1)
						{
							char* tmpfilename = ostrcat(filename, NULL, 0, 0);
							char* fileseek = ostrcat("/mnt/swapextensions/player/", basename(tmpfilename), 0, 0);
							fileseek = ostrcat(fileseek, ".se", 0, 0);
	
							FILE* fbseek = fopen(fileseek, "r");
							if(fbseek != NULL)
							{
//								int ret = textbox(_("Message"), _("Start at last position ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0);
//printf("ret=%d\n", ret);
//								if(ret == 0 || ret == 1)
								if(textbox(_("Message"), _("Start at last position ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0) == 1)
								{
									char* skip1 = calloc(1, 20); 
									if(skip1 != NULL) 
									{
										fscanf(fbseek,"%s",skip1);
										playrcjumpf(filename, NULL, atoll(skip1), &playinfobarstatus, &playinfobarcount, playertype, flag);		
									}
									free(skip1), skip1 = NULL;
								}
								fclose(fbseek);
							}
							free(fileseek), fileseek = NULL;
							free(tmpfilename), tmpfilename = NULL;
						}
					}
		
				#endif

				clearscreen(loadmediadb);
				if(videooff == 0) screenplayinfobar(filename, NULL, 0, playertype, 0);
				status.play = 1;

				free(status.playfile); status.playfile = NULL;
				status.playfile = ostrcat(filename, "", 0, 0);

				if(status.mediadbfiles == 0 && getconfigint("mc_vp_autoscan", NULL) == 1 && !file_exist("/tmp/.autoscan"))
				{
					if(filelistpath->text != NULL)
					{
						mediadbscan(filelistpath->text, 1000, 1);
						writesys("/tmp/.autoscan", filelistpath->text, 0);
						status.mediadbfiles = findfiles(filelistpath->text, 0, 1, 1, 1); //count only
					}
					else
					{
						mediadbscan(currentdirectory, 1000, 1);
						writesys("/tmp/.autoscan", currentdirectory, 0);
						status.mediadbfiles = findfiles(currentdirectory, 0, 1, 1, 1); //count only
					}
				}
			}
		}

		if(eof >=1 || (playertype == 0 && playerisplaying() == 0) || (playertype == 1 && playerisplayingts() == 0))
		{
			if(status.play == 1)
			{
				char* tmpfilename = ostrcat(filename, NULL, 0, 0);
				char* fileseek = ostrcat("/mnt/swapextensions/player/", basename(tmpfilename), 0, 0);
				fileseek = ostrcat(fileseek, ".se", 0, 0);
				unlink(fileseek);
			
				setfbtransparent(0);
				apskin->hidden = NO;
				drawscreen(skin, 0, 0);
				playereof(apskin, filelist, listbox, filelistpath, b2, NULL, NULL, NULL, &skip, &eof, &playlist, playertype, flag);
			}
		}
		if(videooff == 1) drawscreen(blackscreen, 0, 0);
	}

	deinitscreensaver();
	status.hangtime = getconfigint("hangtime", NULL);
	delconfigtmp("dirsort");
	delconfigtmp("mediadbpath");
	delconfigtmp("mediadbfile");
	delconfigtmp("mediadbscandelall");
	delconfigtmp("mediadbscandelnotfound");	
	delconfigtmp("mediadbscantimeout");
	
	delmarkedscreennodes(apskin, FILELISTDELMARK);
	delownerrc(apskin);
	clearscreen(apskin);

	free(filename), filename = NULL;
	free(currentdirectory), currentdirectory = NULL;
	free(selectedfile), selectedfile = NULL;
	free(lastid), lastid = NULL;

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


	if(status.mediadbthread == NULL)
	{
		if(status.writemediadb == 1)
			writemediadb(getconfig("mediadbfile", NULL), NULL);
		freemediadb(0);
	}

	clearscreen(blackscreen);
	clearscreen(loadmediadb);

	system("/bin/umount -fl `mount | grep fuse | grep rarfs | grep -v '/dev/' | cut -d ' ' -f3` > /dev/null 2>&1; killall -9 rarfs > /dev/null 2>&1");
	system("/bin/umount -fl `mount | grep fuse | grep -v '/dev/' | cut -d ' ' -f3` > /dev/null 2>&1; /bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3`  `mount | grep udf | cut -d ' ' -f3` > /dev/null 2>&1; killall -9 rarfs fusesmb curlftpfs > /dev/null 2>&1");
	system("umount -a -f -t fuse.rarfs,iso9660,udf,fuse.djmount,fuse.fusesmb,fuse.curlftpfs > /dev/null 2>&1");

	free(defaultmvi), defaultmvi = NULL;

	writevfdmenu("Mediacenter");
	debug(50, "closed");
}

#endif
