#ifndef MC_GLOBAL_H
#define MC_GLOBAL_H

/*
 ------------------------------------------
	mc_changeview start
 ------------------------------------------
*/
// flag 1 = vp
// flag 2 = ap
// flag 3 = pp
void mc_changeview(int view, struct skin* filelist, struct skin* apskin, int flag)
{
	struct skin* node = filelist;

	struct skin* plot = getscreennode(apskin, "plot");
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
	struct skin* stars = getscreennode(apskin, "stars");

	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);
			
	while(node != NULL)
	{
		if(node->del == FILELISTDELMARK)
		{
			node->fontsize = 0;
			node->height = 0;
			node->width = 0;
			node->height = 0;
			node->bgcol = 0;
			node->transparent = 0;
			node->prozwidth = 0;
			node->posx = 0;
		}
		node = node->next;
	}

	if(view == 0)
	{
		filelist->fontsize = 25;//, fontspace = 5;
		filelist->height = 460;
		filelist->width = 1160;
		filelist->prozwidth = 0;
		filelist->posx = 40;
		filelist->transparent = -1;
		filelist->bgcol = -1;
		thumb->hidden = YES;
		plot->hidden = YES;
		title->hidden = YES;
		actorstext->hidden = YES;
		actors->hidden = YES;
		genretext->hidden = YES;
		genre->hidden = YES;
		yeartext->hidden = YES;
		year->hidden = YES;
		realnametext->hidden = YES;
		realname->hidden = YES;
		albumtext->hidden = YES;
		album->hidden = YES;
		stars->hidden = YES;
	}
	else if(view == 1)
	{
		filelist->fontsize = 35;//, fontspace = 5;
		filelist->height = 460;
		filelist->width = 1160;
		filelist->prozwidth = 0;
		filelist->posx = 40;
		filelist->transparent = -1;
		filelist->bgcol = -1;
		filelist->prozwidth = 0;
		thumb->hidden = YES;
		plot->hidden = YES;
		title->hidden = YES;	
		actorstext->hidden = YES;
		actors->hidden = YES;
		genretext->hidden = YES;
		genre->hidden = YES;
		yeartext->hidden = YES;
		year->hidden = YES;
		realnametext->hidden = YES;
		realname->hidden = YES;
		albumtext->hidden = YES;
		album->hidden = YES;
		stars->hidden = YES;
	}
	else if(view == 2)
	{
		filelist->fontsize = 20;//, fontspace = 5;
		filelist->height = 480;
		filelist->width = 1160;
		filelist->prozwidth = 0;	
		filelist->posx = 40;
		filelist->transparent = -1;
		filelist->bgcol = -1;
		thumb->hidden = YES;
		plot->hidden = YES;
		title->hidden = YES;
		actorstext->hidden = YES;
		actors->hidden = YES;
		genretext->hidden = YES;
		genre->hidden = YES;
		yeartext->hidden = YES;
		year->hidden = YES;
		realnametext->hidden = YES;
		realname->hidden = YES;
		albumtext->hidden = YES;
		album->hidden = YES;
		stars->hidden = YES;
	}
	else if(view == 3)
	{
		filelist->fontsize = 25;//, fontspace = 10;
		filelist->height = 350;
		if(flag == 2)
			filelist->width = 800;
		else
			filelist->width = 600;
		filelist->prozwidth = 0;		
		filelist->bgcol = 0x474747;
		filelist->transparent = 40;
		filelist->posx = 20;
		thumb->hidden = NO;
		title->hidden = NO;

		if(flag == 2)
		{
			plot->hidden = YES;
			actorstext->hidden = NO;
			actors->hidden = NO;
			genretext->hidden = NO;
			genre->hidden = NO;
			yeartext->hidden = NO;
			year->hidden = NO;
			realnametext->hidden = NO;
			realname->hidden = NO;
			albumtext->hidden = NO;
			album->hidden = NO;
			stars->hidden = YES;
		}
		else
		{
			plot->hidden = NO;
			actorstext->hidden = YES;
			actors->hidden = YES;
			genretext->hidden = YES;
			genre->hidden = YES;
			yeartext->hidden = YES;
			year->hidden = YES;
			realnametext->hidden = YES;
			realname->hidden = YES;
			albumtext->hidden = YES;
			album->hidden = YES;
			stars->hidden = NO;
		}
	}	
	else if(view == 4)
	{
		filelist->fontsize = 30;//, fontspace = 5;
		filelist->height = 460;
		filelist->width = 1160;
		filelist->prozwidth = 0;
		filelist->posx = 40;
		filelist->transparent = -1;
		filelist->bgcol = -1;
		filelist->prozwidth = 0;
		thumb->hidden = YES;
		plot->hidden = YES;
		title->hidden = YES;		
		actorstext->hidden = YES;
		actors->hidden = YES;
		genretext->hidden = YES;
		genre->hidden = YES;
		yeartext->hidden = YES;
		year->hidden = YES;
		realnametext->hidden = YES;
		realname->hidden = YES;
		albumtext->hidden = YES;
		album->hidden = YES;
		stars->hidden = YES;
	}
	else if(view == 5)
	{
		filelist->fontsize = 25;//, fontspace = 5;
		filelist->height = 460;
		filelist->width = 1160;
		filelist->prozwidth = 0;
		filelist->posx = 40;
		filelist->transparent = -1;
		filelist->bgcol = -1;
		filelist->prozwidth = 0;
		thumb->hidden = YES;
		plot->hidden = YES;
		title->hidden = YES;	
		actorstext->hidden = YES;
		actors->hidden = YES;
		genretext->hidden = YES;
		genre->hidden = YES;
		yeartext->hidden = YES;
		year->hidden = YES;
		realnametext->hidden = YES;
		realname->hidden = YES;
		albumtext->hidden = YES;
		album->hidden = YES;
		stars->hidden = YES;
	}

	filelist->fontsize += status.fontsizeadjust;
}

/*
 ------------------------------------------
	mc_changeratio start
 ------------------------------------------
*/
int mc_changeratio()
{
	debug(50, "[mc_changeratio] start");
	char *tmppolicy = NULL, *tmpstr = NULL;
	debug(50, "[mc_changeratio] getpolicy");
 	tmppolicy = getpolicy();

	debug(50, "[mc_changeratio] from: %s", tmppolicy);

	if(!strncmp("letterbox", tmppolicy, 8)){
		tmpstr = ostrcat(tmpstr, "panscan", 1, 0);
	} else if(!strncmp("panscan", tmppolicy, 7)){
		tmpstr = ostrcat(tmpstr, "non", 1, 0);
	} else if(!strncmp("non", tmppolicy, 3)){
		tmpstr = ostrcat(tmpstr, "bestfit", 1, 0);
	} else if(!strncmp("bestfit", tmppolicy, 7)){
		tmpstr = ostrcat(tmpstr, "letterbox", 1, 0);
	}

	debug(50, "[mc_changeratio] to: %s", tmpstr);
	setpolicy(tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(tmppolicy); tmppolicy = NULL;
	return 0;
}

extern struct skin* skin;
extern struct screensaver* screensaver;

void playereof(struct skin* apskin, struct skin* filelist, struct skin* listbox, struct skin* filelistpath, struct skin* b2, struct skin* picscreen, struct skin* picture, struct skin* picname, int* skip, int* eof, int* playlist, int playertype, int flag)
{
		debug(50, "--------------eof-------------");
		status.playspeed = 0;
		status.pause = 0;
		status.play = 0;
		int skiprcok = 0;
		
		//playlist
		if(*playlist == 1 && flag != 4)
		{
//			changetext(b2, _("Playlist-Mode"));
			changetext(b2, _("Playlist"));

			if(screensaver != NULL && screensaver->type == 3 && flag == 2)
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
		
			drawscreen(apskin, 0, 0);
		
			int switchtofilelist = 0;
						
			if(status.random == 1 && status.repeat == 0)
			{
				int maxdirs = 0, maxfiles = 0;
				getplaylistmaxold(listbox, &maxdirs, &maxfiles);
				struct skin* tmplist = getplaylistrandomold(listbox, maxdirs, maxfiles);
				debug(50, "struct skin xtext=%s", tmplist->text);
				if(tmplist != NULL)
				{
					debug(50, "struct skin randomtext=%s", tmplist->text);
					setlistboxselection(listbox, tmplist->name);
				}
			}
			else
			{
				if(status.repeat == 0)
				{
					debug(50, "writerc: rcdown next file");

					debug(50, "listbox->aktline: %d", listbox->aktline);
					debug(50, "listbox->linecount: %d", listbox->linecount);
					debug(50, "*eof: %d", *eof);
					debug(50, "flag: %d", flag);
			

					if(*eof == 0)
					{
						if(listbox->aktline < listbox->linecount)
						{
							listbox->aktline ++;
//							if(flag == 1)
//							{
//								apskin->hidden = YES;
//								delownerrc(apskin);
//								drawscreen(skin, 0, 0);
//							}
						}
						else
						{
							if(flag != 3)
								playrcstop(playertype, flag);
							if((flag == 1) || (flag == 3))
							{
								apskin->hidden = NO;
								filelist->hidden = NO;
								listbox->hidden = YES;

								if(flag == 1)
									changetext(filelistpath, _(getconfig("mc_vp_path", NULL)));
								else if(flag == 2)
									changetext(filelistpath, _(getconfig("mc_ap_path", NULL)));
								else if(flag == 3)
								{
									changetext(filelistpath, _(getconfig("mc_pp_path", NULL)));
									picplayer(picscreen, picture, picname, NULL, 0);
									clearscreen(picscreen);
								}

								singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
//								changetext(b2, _("Filelist-Mode"));
								changetext(b2, _("Playlist"));
//								writevfdmenu("Filelist-Mode");
								writevfdmenu("Playlist");

								switchtofilelist = 1;
	
								drawscreen(skin, 0, 0);
								
								// show skin
								setfbtransparent(255);

								*playlist = 0;
							}
							else
							{
								if(screensaver != NULL && screensaver->type == 3 && flag == 2)
									singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
							}
							skiprcok = 1;
						}
					}
					else if(*eof == 1)
					{
						if(listbox->aktline < listbox->linecount)
							listbox->aktline ++;
							if(flag == 1)
							{
								//apskin->hidden = YES;
								//delownerrc(apskin);
								drawscreen(skin, 0, 0);
							}
						else
						{
							status.play = 1;
							skiprcok = 1;
						}
					}
					else if(*eof == 2)
					{
						if(listbox->aktline == 1)
						{
							status.play = 1;
							if(screensaver != NULL && screensaver->type == 3 && flag == 2)
								singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
							skiprcok = 1;
						}
						else
						{
							listbox->aktline --;
							if(flag == 1)
							{
								//apskin->hidden = YES;
								//delownerrc(apskin);
								drawscreen(skin, 0, 0);
							}
							// workaround dont open folder on rcchup
							*skip = 1;
						}
					}

					*eof = 0;

					debug(50, "listbox->aktline: %d", listbox->aktline);
					debug(50, "listbox->linecount: %d", listbox->linecount);
				}
			}
			delownerrc(apskin);
			if(switchtofilelist == 0)
				addscreenrc(apskin, listbox);
			else
				addscreenrc(apskin, filelist);			
		}
		else if (flag != 4)
		{
//			changetext(b2, _("Filelist-Mode"));
			changetext(b2, _("Playlist"));
			
		
			if(screensaver != NULL && screensaver->type == 3 && flag == 2)
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);

			if(flag != 3)
				drawscreen(apskin, 0, 0);

			if(status.random == 1 && status.repeat == 0)
			{
				int maxdirs = 0, maxfiles = 0;
				getfilelistmax(filelist, &maxdirs, &maxfiles);
				struct skin* tmplist = getfilelistrandom(filelist, maxdirs, maxfiles);
				debug(50, "struct skin xtext=%s", tmplist->text);
				if(tmplist != NULL)
				{
					debug(50, "struct skin randomtext=%s", tmplist->text);
					setlistboxselection(filelist, tmplist->name);
				}
			}
			else
			{
				if(status.repeat == 0)
				{
					debug(50, "writerc: rcdown next file");

					debug(50, "filelist->aktline: %d", filelist->aktline);
					debug(50, "filelist->linecount: %d", filelist->linecount);

					if(*eof == 0)
					{
						if(filelist->aktline < filelist->linecount)
						{
							filelist->aktline ++;
						}
						else
						{							
							if(flag != 3)
								playrcstop(playertype, flag);
							if((flag == 1) || (flag == 3)) 
							{
								if(flag == 3)
								{
									picplayer(picscreen, picture, picname, NULL, 0);
									clearscreen(picscreen);
								}

								singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);

								apskin->hidden = NO;
								filelist->hidden = NO;
								listbox->hidden = YES;

								// switch filelist
								delownerrc(apskin);
								addscreenrc(apskin, filelist);

								setfbtransparent(255);
							}
							else
							{
								if(screensaver != NULL && screensaver->type == 3 && flag == 2)
									singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
							}
							skiprcok = 1;
						}

					}
					else if(*eof == 1)
					{
						if(filelist->aktline < filelist->linecount)
							filelist->aktline ++;
						else
						{
							status.play = 1;
							skiprcok = 1;
						}
					}
					else if(*eof == 2)
					{
						if(filelist->aktline == 1)
						{
							status.play = 1;
							if(screensaver != NULL && screensaver->type == 3 && flag == 2) 
								singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
							skiprcok = 1;
						}
						else
						{
							filelist->aktline --;
							// workaround dont open folder on rcchup
							*skip = 1;
						}
					}

					*eof = 0;

					debug(50, "filelist->aktline: %d", filelist->aktline);
					debug(50, "filelist->linecount: %d", filelist->linecount);
				}
			}
			delownerrc(apskin);
			addscreenrc(apskin, filelist);
		}

		debug(50, "flag: %d", flag);
		debug(50, "skiprcok: %d", skiprcok);					

		if(flag == 4)
		{
			skiprcok = 1;
			singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/default.mvi", 0);
			drawscreen(apskin, 0, 0);
		}
		else if(flag == 3 && skiprcok == 0)
			drawscreen(apskin, 2, 0);
		else
			drawscreen(apskin, 0, 0);
			
		if(skiprcok == 0)
			writerc(getrcconfigint("rcok", NULL));
}

void playerrandom(struct skin* apskin, struct skin* filelist, struct skin* listbox, struct skin* b3, int playlist, int flag)
{
	if(playlist == 0)
	{
		debug(50, "rcyellow: random");
		if(status.random == 0)
		{
			int maxdirs = 0, maxfiles = 0;
			getfilelistmax(filelist, &maxdirs, &maxfiles);
			struct skin* tmplist = getfilelistrandom(filelist, maxdirs, maxfiles);

			if(tmplist != NULL)
			{
				debug(50, "struct skin randomtext=%s", tmplist->text);
				setlistboxselection(filelist, tmplist->name);
//				if(flag == 2)
					changetext(b3, _("Random-On"));
				drawscreen(apskin, 0, 0);
				status.random = 1;
				debug(50, "writerc rcok");
				writerc(getrcconfigint("rcok", NULL));
			}
		}
		else
		{
			status.random = 0;
//			if(flag == 2)
				changetext(b3, _("Random"));
			drawscreen(apskin, 0, 0);
		}
	}
	else
	{
		debug(50, "rcyellow: random");
		if(status.random == 0)
		{
			int maxdirs = 0, maxfiles = 0;
			getplaylistmaxold(listbox, &maxdirs, &maxfiles);
			struct skin* tmplist = getplaylistrandomold(listbox, maxdirs, maxfiles);
			if(tmplist != NULL)
			{
				debug(50, "struct skin randomtext=%s", tmplist->text);
				setlistboxselection(listbox, tmplist->name);
//				if(flag == 2)
					changetext(b3, _("Random-On"));
				drawscreen(apskin, 0, 0);
				status.random = 1;
				debug(50, "writerc rcok");
				writerc(getrcconfigint("rcok", NULL));
			}
		}
		else
		{
			status.random = 0;
//			if(flag == 2)
				changetext(b3, _("Random"));
			drawscreen(apskin, 0, 0);
		}
	}
}

void showplaylist(struct skin* apskin, struct skin* filelistpath, struct skin* filelist, struct skin* listbox, struct skin* b2, int mode, int* playlist, int* eof, char** filename, char** currentdirectory, int* playertype, int flag)
{
	int playerret = 0;
	struct skin* tmp = NULL;
	struct mainplaylist* mplaylist = NULL;
	struct playlist* playlistnode = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	char* title = NULL;
	int count = 0;

	if(mode == 0)
	{
		debug(50, "rcgreenss: playlist menu");

		addmenulist(&mlist, _("Load Playlist"), NULL, NULL, 0, 0);
		addmenulist(&mlist, _("Edit Playlist"), NULL, NULL, 0, 0);
		
		mbox = menulistbox(mlist, "playlistmenu", _("Playlist Menu"), _("Choose your Playlist Mode from the following list"), "%pluginpath%/mc/skin", NULL, 1, 0);

		drawscreen(apskin, 0, 0);

		if(mbox != NULL)
		{
			if(ostrcmp(mbox->name, _("Load Playlist")) == 0)
			{
//				*count = 0;
//				*rcwait = 1000;
	
				status.playspeed = 0;
				status.pause = 0;
				status.play = 0;

				mplaylist = screenmainplaylist(1);
				drawscreen(apskin, 0, 0);

				if(mplaylist != NULL && mplaylist->playlist != NULL)
				{
					playlistnode = NULL;
					playlistnode = mplaylist->playlist;
					if(playlistnode->file != NULL)
					{
						*filename = ostrcat("", playlistnode->file, 0, 0);
						playrcstop(*playertype, flag);

						count = 0;

						// clear listbox, not working ?
						delmarkedscreennodes(apskin, 2);

						char* firsttitle = NULL;
						while(playlistnode != NULL)
						{
							tmp = addlistbox(apskin, listbox, tmp, 2);
							if(tmp != NULL)
							{
								int count1 = 0;
								char* tmpstr1 = NULL;
								tmpstr1 = ostrcat("", playlistnode->file, 0, 0);

								int i = 0;
								struct splitstr* ret1 = NULL;
								ret1 = strsplit(tmpstr1, "/", &count1);
								if(count1 >= 1)
									i = count1 - 1;
								count ++;
								debug(50, "addlistbox (%d) %s", count, playlistnode->file);

								title = ostrcat("(", oitoa(count), 0, 1);
								if(count < 10)
									title = ostrcat(title, ")    ", 1, 0);
								else if(count < 100)
									title = ostrcat(title, ")   ", 1, 0);
								else
									title = ostrcat(title, ")  ", 1, 0);

								title = ostrcat(title, (&ret1[i])->part, 1, 0);
								changetext(tmp, _(title));

								if(firsttitle == NULL)
									firsttitle = ostrcat("", title, 0, 0);			

								free(ret1), ret1 = NULL;
								free(tmpstr1), tmpstr1 = NULL;

								// new

								changename(tmp, playlistnode->file);
								tmp->handle = (char*)playlistnode;
								tmp->del = 2;
							}
							playlistnode = playlistnode->next;
						}

						*currentdirectory = ostrcat("Playlist: ", mplaylist->name, 0, 0);
						changetext(filelistpath, _(*currentdirectory));
						filelist->hidden = YES;
						listbox->hidden = NO;

						// workaround playereof
						playlistnode = NULL;
						playlistnode = mplaylist->playlist;

						setlistboxselection(listbox, *filename);
						if(firsttitle == NULL)
							firsttitle = ostrcat("", "dummy no read title", 0, 0);						

						title = ostrcat("", firsttitle, 0, 0);
//						changetext(b2, _("Playlist-Mode"));
						changetext(b2, _("Playlist"));

						if(screensaver != NULL && screensaver->type == 0)
							screensaver->value = title;

						delownerrc(apskin);
						addscreenrc(apskin, listbox);
						drawscreen(apskin, 0, 0);
						// show playlist end	
						sleep(1);

						if(flag == 1  && status.play == 1)
						{
							servicestop(status.aktservice, 1, 1);
							drawscreen(skin, 0, 0);
							setfbtransparent(255);
							debug(50, "check");
							debug(50, "autostart_playlist: %d", getconfigint("mc_vp_autostart_playlist", NULL));
							debug(50, "status.play: %d", status.play);
							debug(50, "flag: %d", flag);
						}
						else if(flag == 2  && status.play == 1)
						{
							addscreenrc(apskin, listbox);
							drawscreen(apskin, 0, 0);
							debug(50, "check");
							debug(50, "autostart_playlist: %d", getconfigint("mc_ap_autostart_playlist", NULL));
							debug(50, "status.play: %d", status.play);				
							debug(50, "flag: %d", flag);
						}
			
						if((getconfigint("mc_vp_autostart_playlist", NULL) == 1 && flag == 1 && status.play == 0) || (getconfigint("mc_ap_autostart_playlist", NULL) == 1 && flag == 2 && status.play == 0))
						{
							debug(50, "-------------- check ok titan playlist pls --------------");
							debug(50, "playerstart: %s", *filename);
							debug(50, "flag: %d", flag);
							debug(50, "playertype: %d", *playertype);	
											
							if(flag == 1)
							{
								servicestop(status.aktservice, 1, 1);
								drawscreen(skin, 0, 0);
								setfbtransparent(255);
								debug(50, "check");
								debug(50, "autostart_playlist: %d", getconfigint("mc_vp_autostart_playlist", NULL));
								debug(50, "status.play: %d", status.play);
							}
							else if(flag == 2)
							{
								addscreenrc(apskin, listbox);
								drawscreen(apskin, 0, 0);
								debug(50, "check");
								debug(50, "autostart_playlist: %d", getconfigint("mc_ap_autostart_playlist", NULL));
								debug(50, "status.play: %d", status.play);				
							}
			
//							playerret = playerstart(*filename);
							if(getconfigint("playertype", NULL) == 1 && cmpfilenameext(*filename, ".ts") == 0 && ostrstr(*filename, "://") == NULL)
								*playertype = 1;
							else
								*playertype = 0;
					
							if(*playertype == 1)
								playerret = playerstartts(*filename, 0);
							else
								playerret = playerstart(*filename);
					
							playwritevfd(*filename, NULL);
				
	
							#ifndef SIMULATE
							if(playerret != 0)
							{
								textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
								*eof = 1;
							}
							#endif
	
							status.play = 1;
						}
						*playlist = 1;
						free(firsttitle), firsttitle = NULL;
					}
				}
			}
			else
			{
				screenmainplaylist(0);
				writeallconfig(0);
				drawscreen(apskin, 0, 0);
			}
			freemenulist(mlist, 1);
		}
	}
	else
	{
		FILE *fd = NULL;
		char *fileline = NULL;
		int linecount = 0;
		int httpstream = 0;

		fileline = malloc(MINMALLOC);
		if(fileline == NULL)
		{
			err("no memory");
			return;
		}

		fd = fopen(*filename, "r");
		if(fd == NULL)
		{
			perr("can't open %s", *filename);
			free(fileline);
			return;
		}
		
		// clear listbox, not working ?
		delmarkedscreennodes(apskin, 2);

		char* firstfile = NULL;
		char* firsttitle = NULL;
		char* extra = NULL;
		while(fgets(fileline, MINMALLOC, fd) != NULL)
		{
			int addcurrdir = 0;

			if(ostrstr(fileline, "﻿#EXTM3U") != NULL)
				continue;
			if(fileline[0] == '#' || fileline[0] == '\n' || fileline[0] == '\r')
			{

				if(fileline[strlen(fileline) - 1] == '\n')
					fileline[strlen(fileline) - 1] = '\0';
				if(fileline[strlen(fileline) - 1] == '\r')
					fileline[strlen(fileline) - 1] = '\0';

				printf("fileline: %s\n", fileline);

//				extra = ostrcat(fileline, NULL, 0, 0);
//				extra = stringreplacechar(extra, '\n', '\0');
				extra = oregex(".*,(.*).*", fileline);
				printf("set extra: %s\n", extra);
				continue;
			}
			if(fileline[0] == '/')
				addcurrdir = 1;
//			if(strlen(fileline) >= 6 && fileline[4] == ':' && fileline[5] == '/' && fileline[6] == '/')
			if(ostrstr(fileline, "://") != NULL)
			{
				addcurrdir = 1;
				httpstream = 1;
			}
			if(fileline[strlen(fileline) - 1] == '\n')
				fileline[strlen(fileline) - 1] = '\0';
			if(fileline[strlen(fileline) - 1] == '\r')
				fileline[strlen(fileline) - 1] = '\0';

			linecount++;

			tmp = addlistbox(apskin, listbox, tmp, 2);
			if(tmp != NULL)
			{
				int count1 = 0;
				char* tmpstr1 = NULL;
				tmpstr1 = ostrcat("", fileline, 0, 0);

				int i = 0;
				struct splitstr* ret1 = NULL;
				ret1 = strsplit(tmpstr1, "/", &count1);
				if(count1 >= 1)
					i = count1 - 1;
				count ++;

				debug(50, "title1: %s", title);

				title = ostrcat("(", oitoa(count), 0, 1);
				debug(50, "title2: %s", title);

				if(count < 10)
					title = ostrcat(title, ")    ", 1, 0);
				else if(count < 100)
					title = ostrcat(title, ")   ", 1, 0);
				else
					title = ostrcat(title, ")  ", 1, 0);

				debug(50, "title3: %s", title);

//				struct splitstr* ret2 = NULL;
//				title = ostrcat(title, extra, 1, 0);
//				ret2 = strsplit(tmpstr2, ",", &count2);

				if(extra != NULL)
					title = ostrcat(title, extra, 1, 0);
				else if(ret1 != NULL)
					title = ostrcat(title, (&ret1[i])->part, 1, 0);

				debug(50, "title4: %s", title);

				printf("extra2: %s\n", extra);

				if(title != NULL)
				{
					title = string_replace("#EXTINF:-1,", "", title, 1);
					title = string_replace("#EXTINF:,", "", title, 1);
					title = string_replace("#EXTINF,", "", title, 1);
					title = string_replace("#", "", title, 1);
				}

				debug(50, "#########################");

				debug(50, "title5: %s", title);
				debug(50, "fileline: %s", fileline);
				debug(50, "#########################");

				debug(50, "addlistbox (%d) %s: %s", count, title, fileline);

				changetext(tmp, _(title));

				if(extra != NULL)
				{
					free(extra), extra = NULL;
					free(title), title = NULL;
				}

				if(firstfile == NULL)
				{
					if(addcurrdir == 0)
					{
						if(flag == 1)
							firstfile = ostrcat(getconfig("mc_vp_path", NULL), "/", 0, 0);
						else if(flag == 2)
							firstfile = ostrcat(getconfig("mc_ap_path", NULL), "/", 0, 0);

						firstfile = ostrcat(firstfile, fileline, 1, 0);
					}
					else
						firstfile = ostrcat("", fileline, 0, 0);

					firsttitle = ostrcat("", title, 0, 0);			
				}
				free(ret1), ret1 = NULL;
				free(tmpstr1), tmpstr1 = NULL;

				if(addcurrdir == 0)
				{					
					char* tmpstr3 = NULL;

					if(flag == 1)
						tmpstr3 = ostrcat(getconfig("mc_vp_path", NULL), "/", 0, 0);
					else if(flag == 2)
						tmpstr3 = ostrcat(getconfig("mc_ap_path", NULL), "/", 0, 0);

					tmpstr3 = ostrcat(tmpstr3, fileline, 1, 0);
					changename(tmp, tmpstr3);
					free(tmpstr3), tmpstr3 = NULL;
				}
				else
					changename(tmp, fileline);


				//tmp->handle = (char*)fileline;
				tmp->del = 2;
			}
		}

		if(*filename[0] == '/')
		{
			int count3 = 0;
			char* tmpstr3 = NULL;
			char* title = NULL;
			tmpstr3 = ostrcat("", *filename, 0, 0);

			struct splitstr* ret3 = NULL;
			ret3 = strsplit(tmpstr3, "/", &count3);
			count = count3 - 1;
			title = ostrcat("", (&ret3[count])->part, 0, 0);
			*currentdirectory= ostrcat("Playlist: ", title, 0, 0);
			free(ret3), ret3 = NULL;
			free(tmpstr3), tmpstr3 = NULL;
			free(title), title = NULL;
		}
		else
		{
			*currentdirectory= ostrcat("Playlist: ", *filename, 0, 0);
		}

		changetext(filelistpath, _(*currentdirectory));

		filelist->hidden = YES;
		listbox->hidden = NO;

//		changetext(b2, _("Playlist-Mode"));
		changetext(b2, _("Playlist"));
		delownerrc(apskin);
		addscreenrc(apskin, listbox);
		drawscreen(apskin, 0, 0);
		// show playlist end	

		if(firstfile != NULL)
		{
			playrcstop(*playertype, flag);
			setlistboxselection(listbox, *filename);
			*filename = ostrcat("", firstfile, 0, 0);
			title = ostrcat("", firsttitle, 0, 0);

			//if(flag == 2)
//				changetext(b2, _("Playlist-Mode"));
				changetext(b2, _("Playlist"));

			if(screensaver != NULL && screensaver->type == 0 && flag == 2)
				screensaver->value = title;

			sleep(1);

			if(flag == 1  && status.play == 1)
			{
				servicestop(status.aktservice, 1, 1);
				drawscreen(skin, 0, 0);
				setfbtransparent(255);
				debug(50, "check");
				debug(50, "autostart_playlist: %d", getconfigint("mc_vp_autostart_playlist", NULL));
				debug(50, "status.play: %d", status.play);
				debug(50, "flag: %d", flag);
			}
			else if(flag == 2  && status.play == 1)
			{
				addscreenrc(apskin, listbox);
				drawscreen(apskin, 0, 0);
				debug(50, "check");
				debug(50, "autostart_playlist: %d", getconfigint("mc_ap_autostart_playlist", NULL));
				debug(50, "status.play: %d", status.play);				
				debug(50, "flag: %d", flag);
			}

			// show playlist end
			if((getconfigint("mc_vp_autostart_playlist", NULL) == 1 && flag == 1 && status.play == 0) || (getconfigint("mc_ap_autostart_playlist", NULL) == 1 && flag == 2 && status.play == 0))
			{
				delownerrc(apskin);
				debug(50, "-------------- check ok m3u --------------");
				debug(50, "playerstart: %s", *filename);
				debug(50, "flag: %d", flag);
				debug(50, "playertype: %d", *playertype);

				if(flag == 1)
				{
					servicestop(status.aktservice, 1, 1);
					drawscreen(skin, 0, 0);
					setfbtransparent(255);
					debug(50, "check");
					debug(50, "autostart_playlist: %d", getconfigint("mc_vp_autostart_playlist", NULL));
					debug(50, "status.play: %d", status.play);
				}
				else if(flag == 2)
				{
					addscreenrc(apskin, listbox);
					drawscreen(apskin, 0, 0);
					debug(50, "check");
					debug(50, "autostart_playlist: %d", getconfigint("mc_ap_autostart_playlist", NULL));
					debug(50, "status.play: %d", status.play);				
				}

//				playerret = playerstart(*filename);
				if(getconfigint("playertype", NULL) == 1 && cmpfilenameext(*filename, ".ts") == 0)
					*playertype = 1;
				else
					*playertype = 0;
		
				if(*playertype == 1)
					playerret = playerstartts(*filename, 0);
				else
					playerret = playerstart(*filename);

				playwritevfd(*filename, NULL);
				
				//playwritevfd(*filename, NULL);
				#ifndef SIMULATE
					if(playerret != 0)
					{
						textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

						*eof = 1;
					}
				#endif
	
				status.play = 1;
				if(flag == 1)
					screenplayinfobar(*filename, NULL, 0, 0, 0);
			}				

			*playlist = 1;

			free(fileline), fileline = NULL;
			free(firstfile), firstfile = NULL;
			free(firsttitle), firsttitle = NULL;
		}
		fclose(fd);
	}
}

void mc_audioplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, struct skin* b12, struct skin* b13, char* filename)
{
	debug(50, "infobar start");

	char* tmpstr = NULL;
	unsigned long long int pos = 0, len = 0, reverse = 0;

	pos = playergetpts() / 90000;
	len = playergetlength();
	reverse = len - pos;

	debug(50, "pos: %llu", pos);
	debug(50, "len: %llu", len);
	debug(50, "reverse: %llu", reverse);

	if(len == 0)
		sprogress->progresssize = 0;
	else
		sprogress->progresssize = pos * 100 / len;

	tmpstr = convert_timesec(pos);
	changetext(spos, _(tmpstr));
	free(tmpstr); tmpstr = NULL;

	tmpstr = convert_timesec(len);
	changetext(slen, _(tmpstr));
	free(tmpstr); tmpstr = NULL;

	tmpstr = convert_timesec(reverse);
	changetext(sreverse, _(tmpstr));
	free(tmpstr); tmpstr = NULL;

	if(status.random == 1)
		changetext(b12, _("Random-On"));
	else
		changetext(b12, _("Random"));	

	if(status.repeat == 1)
		changetext(b13, _("Repeat-On"));
	else
		changetext(b13, _("Repeat"));

	drawscreen(infobar, 0, 0);
}

void picplayer(struct skin* picscreen, struct skin* picture, struct skin* picname, char* filename, int flag)
{
	if(filename == NULL)
	{
		clearscreen(picscreen);
		drawscreen(skin, 0, 0);
		return;
	}

	char* tmpstr = NULL;

	tmpstr = ostrcat(filename, "", 0, 0);

	if(getconfigint("mc_pp_showpictitle", NULL) == 1 && tmpstr != NULL) changetext(picname,_((basename(tmpstr))));
	free(tmpstr); tmpstr = NULL;

	changepic(picture, filename);
	picture->picwidth = 1;
	picture->picheight = 1;
	picture->bgcol = 0;
	picture->picquality = getconfigint("mc_pp_picquality", NULL);

	drawscreen(picscreen, 0, 4);
}

#include <stdio.h>
#include <curl/curl.h>

/* This callback is, currently, a simple wrapper around fwrite(). You
   could get it to write to memory, or do anything else you'd like
   with the output. For more info, see
   http://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 */

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
	/* out of memory! */ 
	printf("not enough memory (realloc returned NULL)\n");
	return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

static size_t writeCallback(void *contents, size_t size, size_t nitems, FILE *file) {
  return fwrite(contents, size, nitems, file);
}

// flag = 0 (without header in output)
// flag = 1 (with header in output)
char* gethttps(char* url, char* localfile, char* data, char* user, char* pass, char* referer, int flag)
{
	debug(99, "url: %s", url);

	int debuglevel = getconfigint("debuglevel", NULL);

	char* tmpstr = NULL;
    	FILE *fp;

	CURL *curl_handle;
	CURLcode res;
	
	struct MemoryStruct chunk;
	
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	chunk.size = 0;    /* no data at this point */
	
	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */ 
	curl_handle = curl_easy_init();
	if(curl_handle)
	{
	    	if(localfile != NULL)
		    fp = fopen(localfile,"wb");
	       
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);

		if(user != NULL && pass != NULL)
		{
			curl_easy_setopt(curl_handle, CURLOPT_USERNAME, user);
			curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, pass);
			curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		}
		if(data == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
		else
		{
			curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

			/* example.com is redirected, so we tell libcurl to send POST on 301, 302 and
		     	303 HTTP response codes */
			curl_easy_setopt(curl_handle, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
		}
		if(flag == 1)
			curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 3);
//		curl_easy_setopt(curl_handle, CURLOPT_RETURNTRANSFER, 1);

	    	if(localfile == NULL)
		{
			/* send all data to this function  */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		}
		else
		{
			/* When data arrives, curl will call writeCallback. */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
		}



	    	if(localfile == NULL)
		{
			/* we pass our 'chunk' struct to the callback function */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		}
		else
		{
			/* The last argument to writeCallback will be our file: */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)fp);
		}

		/* some servers don't like requests that are made without a user-agent field, so we provide one */
//		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36");

		// This is occassionally required to stop CURL from verifying the peers certificate.
		// CURLOPT_SSL_VERIFYHOST may also need to be TRUE or FALSE if
		// CURLOPT_SSL_VERIFYPEER is disabled (it defaults to 2 - check the existence of a
		// common name and also verify that it matches the hostname provided)
#ifdef MIPSEL
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
#else
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
		if(debuglevel == 99)
			curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "/mnt/network/cookies");
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "/mnt/network/cookies");
		/* enable redirect following */
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
		/* allow three redirects */
		curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 3L);


		/* enable all supported built-in compressions */
		curl_easy_setopt(curl_handle, CURLOPT_ACCEPT_ENCODING, "");
  
		if(referer == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1L);
		else
			curl_easy_setopt(curl_handle, CURLOPT_REFERER, referer);


		/* get it! */
		res = curl_easy_perform(curl_handle);

		/* check for errors */
		if(res != CURLE_OK)
		{
			err("failed: %s", curl_easy_strerror(res));
			printf("curl error\n");
		}
		else
		{
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */
			printf("%lu bytes retrieved\n", (long)chunk.size);
		}
		
		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);
		if(localfile != NULL)
			fclose(fp);
	}

	if(localfile == NULL)
		tmpstr = ostrcat(chunk.memory, NULL, 0, 0);

  	free(chunk.memory);
	/* we're done with libcurl, so clean it up */

	curl_global_cleanup();

	if(localfile != NULL)
		free(tmpstr), tmpstr = NULL;

	return tmpstr;
}

int update_iptv(char* file)
{
	char* tmpstr = NULL, *link = NULL;
	int ret = 0;

	tmpstr = dirname(file);
	link = getconfig("iptvserver", NULL);

	if(link != NULL)
	{
		debug(10, "update %s", link);

		if(!file_exist(tmpstr))
			mkdir(tmpstr, 0777);

		tmpstr = ostrcat(tmpstr, "/iptv.m3u", 1, 0);
		if(file_exist(tmpstr))
			unlink(tmpstr);

		if(!file_exist(tmpstr))
			gethttps(link, tmpstr, NULL, NULL, NULL, NULL, 0);

		if(file_exist(tmpstr)) ret = 1;

	}

	free(tmpstr), tmpstr = NULL;
	free(link), link = NULL;

	return ret;
}

#endif
