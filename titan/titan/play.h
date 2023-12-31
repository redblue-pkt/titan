#ifndef PLAY_H
#define PLAY_H

extern struct screensaver* screensaver;

void screenplaypolicy()
{
	if(checkbit(status.playercan, 0) == 0) return;

	int rcret = 0;
	struct skin* playpolicy = getscreen("playpolicy");
	char* tmpstr = NULL;

start:
	changepolicy();
	tmpstr = getpolicy();
	changetext(playpolicy, _(tmpstr));
	free(tmpstr); tmpstr = NULL;
	drawscreen(playpolicy, 0, 0);

	while(1)
	{
		rcret = waitrc(playpolicy, 1000, 0);
		if(rcret == getrcconfigint("rcok", NULL))
			goto start;
		break;
	}

	clearscreen(playpolicy);
}

void screenplaybufferstatus(struct stimerthread* self)
{
	int rcret = -1, draw = 50;
	if(self == NULL) return;

	struct skin* playbufferstatus = getscreen("playbufferstatus");

	playbufferstatus->progresssize = 0;
	
	while(self->aktion != STOP)
	{
		rcret = waitrc(0, 1, 0);
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			playerstopbuffer();
			break;
		}

		if(draw == 50)
		{
			draw = 0;
			if(playergetbuffersize() > 0)
				drawscreen(playbufferstatus, 0, 0);
		}
		//usleep(10000);
		draw++;
	}
	
	if(playergetbuffersize() > 0)
	{
		clearscreen(playbufferstatus);
		drawscreen(skin, 0, 0);
	}
}

//flag = 4 ---> timeshift
//flag = 5 --> timeshift, not in play mode (only recording)
void screenplayinfobar(char* file, char* showname, int mode, int playertype, int flag)
{
	int change = 1;
	int videodevpts = 1;

	if(checkbit(status.playercan, 14) == 0) return;

	if((flag == 2) || (flag == 3))
	{
		return;
	}

	struct skin* playinfobar = getscreen("playinfobar");
	struct skin* playinfobarpic = getscreen("playinfobarpic");
	if(mode == 1)
	{
		clearscreen(playinfobar);
		clearscreen(playinfobarpic);
		drawscreen(skin, 0, 0);
		return;
	}

	struct skin* title = getscreennode(playinfobar, "title");
	struct skin* spos = getscreennode(playinfobar, "pos");
	struct skin* slen = getscreennode(playinfobar, "len");
	struct skin* sreverse = getscreennode(playinfobar, "reverse");
	struct skin* sprogress = getscreennode(playinfobar, "progress");
	char* tmpstr = NULL;
	unsigned long long pos = 0, dpos = 0, len = 0, reverse = 0;

	// show thumb cover start
	struct skin* playinfobarcover = getscreen("playinfobarcover");
	struct skin* cover = getscreennode(playinfobarcover, "cover");
	cover->hidden = YES;
	if(file != NULL)
	{
		struct mediadb* node = NULL;
		char* dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		node = getmediadb(dname, basename(file), 0);
		free(dname); dname = NULL;
		
		if(node != NULL)
		{
			if(ostrncmp("tt", node->id, 2) == 0)
			{
				tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, node->id, 1, 0);
				tmpstr = ostrcat(tmpstr, "_poster.jpg", 0, 0);
				if(file_exist(tmpstr))
				{
					changepic(cover, tmpstr);
					cover->hidden = NO;
				}
				free(tmpstr), tmpstr = NULL;
			}
		}
	}
	// show thumb cover end

	if(showname == NULL)
	{
		tmpstr = ostrcat(file, NULL, 0, 0);
		if(tmpstr != NULL) changetext(title, basename(tmpstr));
		free(tmpstr); tmpstr = NULL;
	}
	else
		changetext(title, showname);

	videodevpts = 1;
	if(playertype == 1)
	{
		int ret = 0;
		unsigned long long startpos = 0;
		if(flag == 4)
			ret = playergetinfots(&len, &startpos, NULL, &pos, NULL, 1);
		else if(flag == 5)
			ret = playergetinfots(&len, &startpos, NULL, &pos, NULL, 2);
		else
		{
			ret = playergetinfots(&len, &startpos, NULL, &pos, NULL, 0);
			if(ret == 0)
			{
				ret = videogetpts(status.aktservice->videodev, &dpos);
				if(ret == 0)
				{
					pos = dpos;
					videodevpts = 1;
				}
				else
				{
					ret = 0; 
					videodevpts = 0;
				}
			}
		}
		len = len / 90000;
		pos = (pos - startpos) / 90000;
		if(ret != 0) change = 0;
	}
	else if(playertype == 2)
	{
		pos = dvdgetpts() / 90000;
		len = dvdgetlength();
	}
	else
	{
		pos = playergetpts() / 90000;
		len = playergetlength();
	}
	if(pos < 0) pos = 0;
	reverse = len - pos;

	if(change == 1)
	{
		if(len == 0)
			sprogress->progresssize = 0;
		else
			sprogress->progresssize = pos * 100 / len;
	}

	tmpstr = convert_timesec(pos);
	if(videodevpts == 1)
		changetext(spos, tmpstr);
	else
		changetext(spos, " ");
	free(tmpstr); tmpstr = NULL;

	tmpstr = convert_timesec(len);
	changetext(slen, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = convert_timesec(reverse);
	changetext(sreverse, tmpstr);
	free(tmpstr); tmpstr = NULL;

	status.writeplayersub = 0;

	drawscreen(playinfobar, 0, 0);
	drawscreen(playinfobarpic, 0, 0);
	drawscreen(playinfobarcover, 0, 0);
}

void screenplaytracklist(int mode, int playertype, int flag)
{
	//mode 1 = audiotracks
	//mode 2 = subtitle tracks

	if(mode == 1 && checkbit(status.playercan, 1) == 0) return;
	if(mode == 2 && checkbit(status.playercan, 2) == 0) return;
		
	if(playertype == 1)
	{
		screenplayinfobar(NULL, NULL, 1, playertype, flag);
		if(mode == 1)
			playerchangeaudiotrackts();
		else if(mode == 2)
			playerchangesubtitletrackts();
		blitfb(0);
		return;
	}

	int i = 0;
	int rcret = 0, curtrackid = 0;
	struct skin* track = NULL;
	if(mode == 1)
		track = getscreen("audiotrack");
	else if(mode == 2)
		track = getscreen("subtitle");
	struct skin* listbox = getscreennode(track, "listbox");
	struct skin* tmp = NULL;
	char** tracklist = NULL;
	char* curtrackencoding = NULL, *curtrackname = NULL;
	char* tmpstr = NULL;

	playergetcurtrac(mode, &curtrackid, &curtrackencoding, &curtrackname);
	tracklist = playergettracklist(mode);

	if(tracklist != NULL)
	{
		while(tracklist[i] != NULL)
		{
			printf("tracklist[i]: %s\n", tracklist[i]);
			tmp = addlistbox(track, listbox, tmp, 1);
			if(tmp != NULL)
			{
				if(ostrcmp(tracklist[i], "und") == 0)
					tmpstr = ostrcat(tmpstr, _("undefined"), 1, 0);
				else
					tmpstr = ostrcat(tmpstr, _(tracklist[i]), 1, 0);
					
				if(tracklist[i + 1] != NULL && strlen(tracklist[i + 1]) > 0)
				{	
					tmpstr = ostrcat(tmpstr, " (", 1, 0);
					tmpstr = ostrcat(tmpstr, tracklist[i + 1], 1, 0);
					tmpstr = ostrcat(tmpstr, ")", 1, 0);
				}
				
				
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				tmp->type = CHOICEBOX;
				tmp->del = 1;
				if(tracklist[i] != NULL && atoi(tracklist[i]) != 0)
					tmp->handle = (char*)atoi(tracklist[i]);
				else
					tmp->handle = (char*)(i / 2);

#if defined (EXTGST)
				if(getconfigint("extplayer_type", NULL) == 1)
				{
#endif

	#ifdef EPLAYER4	
					if(curtrackid == (i / 2))
					{
						tmp->handle1 = "running";
						changeinput(tmp, _("running"));
					}
					else
						changeinput(tmp, "");
	#else
					if((ostrcmp(curtrackname, tracklist[i]) == 0 && ostrcmp(curtrackencoding, tracklist[i + 1]) == 0) || (tracklist[i] != NULL && curtrackid == atoi(tracklist[i])))
					{
						tmp->handle1 = "running";
						changeinput(tmp, _("running"));
					}
					else
						changeinput(tmp, "");
	#endif
#if defined (EXTGST)
				}
				else
				{
					if((ostrcmp(curtrackname, tracklist[i]) == 0 && ostrcmp(curtrackencoding, tracklist[i + 1]) == 0) || (tracklist[i] != NULL && curtrackid == atoi(tracklist[i])))
					{
						tmp->handle1 = "running";
						changeinput(tmp, _("running"));
					}
					else
						changeinput(tmp, "");
				}				
#endif

			}
			i += 2;
		}
	}

	free(curtrackencoding); curtrackencoding = NULL;
	free(curtrackname); curtrackname = NULL;
	playerfreetracklist(tracklist);
	tracklist = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	screenplayinfobar(NULL, NULL, 1, playertype, flag);
	drawscreen(track, 0, 0);
	addscreenrc(track, listbox);

	while(1)
	{
		rcret = waitrc(track, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcyellow", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				if(mode == 1) {
					playerchangeaudiotrack((int)listbox->select->handle);
#if defined (EXTGST)
					if(getconfigint("extplayer_type", NULL) == 1)
					{
#endif
	#ifdef EPLAYER4
						if(playertype == 0)
							playerseek(-1);
	#endif
#if defined (EXTGST)
					}
#endif
				}
				else if(mode == 2)
				{
					if(listbox->select->handle1 != NULL)
						playerstopsubtitletrack();
					else	
						playerchangesubtitletrack((int)listbox->select->handle);
				}
			}
			break;
		}
	}

	delmarkedscreennodes(track, 1);
	delownerrc(track);
	clearscreen(track);
	blitfb(0);
	
	if(rcret == getrcconfigint("rcyellow", NULL))
		screenplaytracklist(2, playertype, flag);
}

void playrcyellow(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 1) == 0) return;
	status.writeplayersub = 0;

	screenplaytracklist(1, playertype, flag);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);

	status.writeplayersub = 1;
}

void playrctext(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 2) == 0) return;
	status.writeplayersub = 0;

	screenplaytracklist(2, playertype, flag);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);

	status.writeplayersub = 1;
}

void playrcgreen(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 3) == 0) return;
	status.writeplayersub = 0;

	screenplayinfobar(file, showname, 1, playertype, flag);
	if(playertype == 2)
		screenvideomode(2);
	else
		screenvideomode(1);
	drawscreen(skin, 0, 0);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);

	status.writeplayersub = 1;
}

void playrcblue(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
	if(checkbit(status.playercan, 4) == 0) return;
	status.writeplayersub = 0;

	screenplayinfobar(file, showname, 1, playertype, flag);
	screenpowerofftimer();
	drawscreen(skin, 0, 0);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
	status.writeplayersub = 1;
}

void playrcok(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 0) == 0) return;

	free(status.playfile); status.playfile = NULL;
	status.playfile = ostrcat(file, NULL, 0, 0);
	screenplaypolicy(file, 1);
	drawscreen(skin, 0, 0);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
}

void id3tag_info(char* file)
{
	char* tmpstr = NULL;

	if(file == NULL) return;
	if(!filelistflt(".mp3 .flac .ogg .wma .ra .wav", file))
	{
		struct id3tag* id3tag = NULL;
		int hash = gethash(file);
		char* tmphash = olutoa(hash);
				
		id3tag = getid3(file, tmphash, 1);
		free(tmphash); tmphash = NULL;
	
		if(id3tag != NULL)
		{
			tmpstr = ostrcat(tmpstr, _("Title:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->title, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			
			tmpstr = ostrcat(tmpstr, _("Artist:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->artist, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			
			tmpstr = ostrcat(tmpstr, _("Album:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->album, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			
			tmpstr = ostrcat(tmpstr, _("Year:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->year, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
			tmpstr = ostrcat(tmpstr, _("Genre:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->genretext, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);

			tmpstr = ostrcat(tmpstr, _("Comment:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->comment, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		}
		freeid3(id3tag); id3tag = NULL;		
	}
	else
	{
		tmpstr = ostrcat(tmpstr, _("Title:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Title"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Artist:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Artist"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Album:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Album"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Year:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Year"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	
		tmpstr = ostrcat(tmpstr, _("Genre:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Genre"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Comment:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Comment"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Track:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Track"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Copyright:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Copyright"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
	//	tmpstr = ostrcat(tmpstr, _("TestLibEplayer:"), 1, 0);
	//	tmpstr = ostrcat(tmpstr, " ", 1, 0);
	//	tmpstr = ostrcat(tmpstr, playergetinfo("TestLibEplayer"), 1, 1);
	//	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	}
			
	textbox(_("iD3Tag"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 500, 100, 0);
	free(tmpstr), tmpstr = NULL;
}

void imdb_submenu(char* file, int mode)
{
	struct skin* pluginnode = NULL;
	void (*startplugin)(char*, char*, int, char*, char*);

	startplugin = NULL;
	
	if(mode == 0)
		pluginnode = getplugin("IMDb");
	else if(mode == 1)
		pluginnode = getplugin("IMDb-API");
	else if(mode == 2)
		pluginnode = getplugin("TMDb");
			
	if(pluginnode != NULL)
	{			
		if(mode == 0)
			startplugin = dlsym(pluginnode->pluginhandle, "screenimdb");
		else if(mode == 1)
			startplugin = dlsym(pluginnode->pluginhandle, "screenimdbapi");
		else if(mode == 2)
			startplugin = dlsym(pluginnode->pluginhandle, "screentmdb");

		if(startplugin != NULL)
		{
				debug(133, "file=%s", basename(file));
			if(file != NULL)
			{
				if(ostrstr(file, "/") != NULL)
				{
					//create imdb search name
	
					char* dname = ostrcat(file, NULL, 0, 0);
					dname = dirname(dname);
				
					char* shortname = ostrcat(basename(file), NULL, 0, 0);
					string_tolower(shortname);
	//				shortname = string_shortname(shortname, 1);
					shortname = string_shortname(shortname, 2);
					string_removechar(shortname);
					strstrip(shortname);
	
					debug(133, "inputfile=%s", file);
					debug(133, "shortname=%s", shortname);
					debug(133, "dname=%s", dname);
					debug(133, "file=%s", basename(file));
	
					startplugin(shortname, NULL, 2, dname, basename(file));
	
					free(shortname), shortname = NULL;
					free(dname), dname = NULL;
				}
				else
				{
					startplugin(file, NULL, 2, NULL, NULL);
				}
				
			}				
		}
	}
}

void get_mediadb_scan_info()
{
	int videocount = 0, audiocount = 0, picturecount = 0;
	getmediadbcounts(&videocount, &audiocount, &picturecount);

	char* tmpstr = NULL;
	tmpstr = ostrcat(tmpstr, "scanning (", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(videocount), 1, 1);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(status.mediadbfiles), 1, 1);
	tmpstr = ostrcat(tmpstr, ")", 1, 0);
	
	tmpstr = ostrcat(tmpstr, "\n\n ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("MediaDB directory scan started in Background !"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n  ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Delete MediaDB before scan"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t", 1, 0);
	if(ostrcmp(getconfig("mediadbscandelall", NULL), "1") == 0)
		tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);			
	tmpstr = ostrcat(tmpstr, _("Delete unused entrys before scan"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t", 1, 0);		
	if(ostrcmp(getconfig("mediadbscandelnotfound", NULL), "1") == 0)
		tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("scan Directory"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("mc_vp_path", NULL), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
	tmpstr = ostrcat(tmpstr, _("MediaDB place"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);				
	tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("MediaDB Debug"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);
	if(ostrcmp(getconfig("mediadbdebug", NULL), "1") == 0)
		tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
	tmpstr = ostrcat(tmpstr, _("Backdrop Download Count"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t", 1, 0);		
	if(getconfigint("mediadbbackdrop", NULL) == 0)
		tmpstr = ostrcat(tmpstr, _("all"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, oitoa(getconfigint("mediadbbackdrop", NULL)), 1, 1);

	textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 500, 10, 0);
	free(tmpstr), tmpstr = NULL;
}

// flag 100 = tithek
int playrcred(char* file, char* showname, char* menutitle, char* link, int playinfobarstatus, int playertype, int flag)
{
	int ret = 1;
//	if(checkbit(status.playercan, 5) == 0) return;

	status.writeplayersub = 0;

	if(status.play == 1)
		screenplayinfobar(file, showname, 1, playertype, flag);

	int hangtime = status.hangtime;

	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	struct menulist* mlist = NULL, *mbox = NULL;
	char* skintitle = _("Menu");
    char* localparser = NULL, *localparser_input = NULL, *cmd = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;

printf("playrcred link: %s flag: %d\n", link, flag);

    if(link != NULL && !ostrncmp("/", link, 1))
    {
        localparser_input = ostrcat(link, NULL, 0, 0);
        localparser_input = stringreplacecharonce(localparser_input, ' ', '\0');
printf("playrcred set localparser_input: %s\n", localparser_input);

    }
		
	if(flag < 99)
	{
		if(status.play == 1)
		{
			addmenulist(&mlist, "Video Settings", _("Video Settings"), NULL, 0, 0);
			addmenulist(&mlist, "AV Settings", _("AV Settings"), NULL, 0, 0);
			addmenulist(&mlist, "iD3Tag Info", _("iD3Tag Info"), NULL, 0, 0);
		}

		addmenulist(&mlist, "MediaDB Edit", _("MediaDB Edit"), NULL, 0, 0);

printf("status.play %d\n",status.play);
printf("status.actplay %s\n",status.actplay);
printf("status.mcaktiv %d\n",status.mcaktiv);

		if(status.play == 0)
		{
			addmenulist(&mlist, "Delete File", _("Delete File"), NULL, 0, 0);
			addmenulist(&mlist, "Rename File", _("Delete File"), NULL, 0, 0);
			addmenulist(&mlist, "Create Folder", _("Create Folder"), NULL, 0, 0);
//			addmenulist(&mlist, "Update Iptv", _("Update Iptv"), NULL, 0, 0);
    		addmenulist(&mlist, "Search on PornHub", _("Search on PornHub"), NULL, 0, 0);
    		addmenulist(&mlist, "Search on Xvideos", _("Search on Xvideos"), NULL, 0, 0);
		}
		else if(status.play == 1 && status.mcaktiv != 0)
			addmenulist(&mlist, "Delete File", _("Delete File"), NULL, 0, 0);
		else
			addmenulist(&mlist, "Show Streamlink", _("Show Streamlink"), NULL, 0, 0);


		if(status.mediadbfiles > 0)
			addmenulist(&mlist, "MediaDB Scan Info", _("MediaDB Scan Info"), NULL, 0, 0);		
	}
	else
	{
		addmenulist(&mlist, "Create Favorites sub folder", _("Create Favorites sub folder"), NULL, 0, 0);
		addmenulist(&mlist, "Create Favorites sub folder(PIN secured)", _("Create Favorites sub folder(PIN secured)"), NULL, 0, 0);

		if(flag < 199)
		{
			if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
			{
                debug(202, "showname: %s", showname);
                debug(202, "menutitle: %s", menutitle);
                debug(202, "link: %s", link);
    		    debug(202, "localparser_input: %s", localparser_input);
                if(localparser_input != NULL && ostrstr(link, " search ") != NULL)
                {
                    cmd = ostrcat("cat ", localparser_input, 0, 0);
                    cmd = ostrcat(cmd, " | grep '^create_service_bouquets()' | wc -l", 1, 0);
		            debug(202, "cmd: %s", cmd);
                    if(ostrcmp(string_newline(command(cmd)), "1") == 0)
                    {
        				addmenulist(&mlist, "Create Service Bouquets", _("Create Service Bouquets"), NULL, 0, 0);
                    }
           	        free(cmd), cmd = NULL;
                }
				addmenulist(&mlist, "Search on KinoX", _("Search on KinoX"), NULL, 0, 0);
//start
printf("localparser_input: %s flag: %d\n", localparser_input, flag);
                if(localparser_input != NULL)
                {
	                cmd = ostrcat(cmd, "cat /tmp/tithek/mainmenu.local.list | awk '{ print $1 \" \" $2 \" \" $3 \" \" $4 \" \" $5 }'", 1, 0);
                    printf("playrcred cmd: %s\n", cmd);

	                tmpstr1 = command(cmd);
                    free(cmd), cmd= NULL;

                    printf("playrcred tmpstr1: %s\n", tmpstr1);
                //	tmpstr = readfiletomem(filename, 1);

                    if(tmpstr1 != NULL)
                    {
		                struct splitstr* ret1 = NULL;
		                int count = 0;
		                int i = 0;
                        int ret = 0;
		                int updated = 0;

		                ret1 = strsplit(tmpstr1, "\n", &count);
	                
		                for(i = 0; i < count; i++)
		                {
                            //hid kinox.sh
	                        if(ostrstr((&ret1[i])->part, "kinox.sh") != NULL) continue;
			                struct splitstr* ret2 = NULL;
			                int count2 = 0;
			                int i2 = 0;
			                tmpstr2 = ostrcat((&ret1[i])->part, NULL, 0, 0);
                            printf("playrcred tmpstr2: %s\n", tmpstr2);
			                ret2 = strsplit(tmpstr2, "#", &count2);
                            if(ret2 != NULL && count2 >= 1)
                            {
                                localparser = ostrcat((&ret2[1])->part, NULL, 0, 0);
                                printf("playrcred localparser: %s\n", localparser);
                               
	                            if(file_exist(localparser))
                                {
                                    cmd = ostrcat("cat ", localparser, 0, 0);
                                    cmd = ostrcat(cmd, " | grep '^search()' | wc -l", 1, 0);
		                            printf("cmd: %s\n", cmd);
                                    if(ostrcmp(string_newline(command(cmd)), "1") == 0)
                                    {
                                        tmpstr = ostrcat(_("Search on"), " ", 0, 0);
                                        tmpstr = ostrcat(tmpstr, (&ret2[0])->part, 1, 0);

                        				addmenulist(&mlist, _(tmpstr), (&ret2[1])->part, NULL, 0, 0);
                //        				addmenulist(&mlist, "Search on KinoX", _("Search on KinoX"), NULL, 0, 0);
                                        free(tmpstr), tmpstr = NULL;
                                    }
                                    free(cmd), cmd = NULL;
                                }
                                free(localparser), localparser = NULL;
                            }
			                free(ret2), ret2 = NULL;
			                free(tmpstr2), tmpstr2 = NULL;
		                }
		                free(ret1), ret1 = NULL;
                    }
                    free(tmpstr2), tmpstr2 = NULL;
                }
//end

/*
				addmenulist(&mlist, "Search on KinoX (local)", _("Search on KinoX (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Movie4k", NULL, _("Search on Movie4k"), 0, 0);
				addmenulist(&mlist, "Search on Movie4k (local)", _("Search on Movie4k (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Solarmovies (movie)", _("Search on Solarmovies (movie)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Solarmovies (serie)", _("Search on Solarmovies (serie)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Solarmovies (local)", _("Search on Solarmovies (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Mle-HD (local)", _("Search on Mle-HD (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Beeg (local)", _("Search on Beeg (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Xvideos (local)", _("Search on Xvideos (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on FilmOn (local)", _("Search on FilmOn (local)"), NULL, 0, 0);
*/
			}
/*
			addmenulist(&mlist, "Search on Netzkino (local)", _("Search on Netzkino (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on Youtube", _("Search on Youtube"), NULL, 0, 0);
			addmenulist(&mlist, "Search on Youtube (local)", _("Search on Youtube (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on MyVideo", _("Search on MyVideo"), NULL, 0, 0);
			addmenulist(&mlist, "Search on MyVideo (local)", _("Search on MyVideo (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on ARD (local)", _("Search on ARD (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on ZDF (local)", _("Search on ZDF (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on TecTime (local)", _("Search on TecTime (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on Giga (local)", _("Search on Giga (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on NowTv (local)", _("Search on NowTv (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on InternetTv (local)", _("Search on InternetTv (local)"), NULL, 0, 0);
*/
			addmenulist(&mlist, "Search on InternetRadio (local)", _("Search on InternetRadio (local)"), NULL, 0, 0);
//			addmenulist(&mlist, "Search on All (local)", _("Search on All (local)"), NULL, 0, 0);

		}
	}

	addmenulist(&mlist, "Downloads", _("Downloads"), NULL, 0, 0);

	if(flag < 199)
	{
		//add plugins
		while(child != NULL)
		{
			if(child->del == PLUGINDELMARK && (status.security >= 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
			{
				if(ostrcmp(child->name, "Streaminfo") == 0 && status.play == 1)
					addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
				else if(ostrcmp(child->name, "IMDb") == 0)
					addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
				else if(ostrcmp(child->name, "IMDb-API") == 0)
					addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
	//			else if(ostrcmp(child->name, "TMDb") == 0)
	//				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
			}
			child = child->next;
		}
	}

	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your Red-Menu entry from the following list"), NULL, NULL, 1, 0);
	if(mbox != NULL)
	{
printf("mbox->name=%s\n", mbox->name);
printf("mbox->text=%s\n", mbox->text);

		if(ostrcmp(mbox->name, "Video Settings") == 0)
			screenvideosettings();
		else if(ostrcmp(mbox->name, "AV Settings") == 0)
			screenavsettings(0);
		else if(ostrcmp(mbox->name, "IMDb") == 0)
			imdb_submenu(file, 0);
		else if(ostrcmp(mbox->name, "IMDb-API") == 0)
			imdb_submenu(file, 1);
		else if(ostrcmp(mbox->name, "TMDb") == 0)
			imdb_submenu(file, 2);
		else if(ostrcmp(mbox->name, "iD3Tag Info") == 0)
			id3tag_info(file);
		else if(ostrcmp(mbox->name, "MediaDB Scan Info") == 0)
			get_mediadb_scan_info();
		else if(ostrcmp(mbox->name, "MediaDB Edit") == 0)
			screenmediadbedit(file, 0, 0);
//		else if(ostrcmp(mbox->name, "Update Iptv") == 0)
//			ret = update_iptv(file);
		else if(ostrcmp(mbox->name, "Delete File") == 0)
		{
			playcheckdirrcret(file, 1);
			ret = 9999;
		}
		else if(ostrcmp(mbox->name, "Rename File") == 0)
		{
			playcheckdirrcret(file, 6);
			ret = 9999;
		}
		else if(ostrcmp(mbox->name, "Create Folder") == 0)
		{
			playcheckdirrcret(file, 7);
			ret = 9999;
		}
		else if(ostrcmp(mbox->name, "Show Streamlink") == 0)
	       textbox(_(mbox->name), file, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 200, 0, 0);
		else if(ostrcmp(mbox->name, "Downloads") == 0)
			screenbgdownload(1);
		else if(ostrcmp(mbox->name, "Create Favorites sub folder") == 0)
			ret = 0;
		else if(ostrcmp(mbox->name, "Create Favorites sub folder(PIN secured)") == 0)
			ret = 1000;
		else if(ostrcmp(mbox->name, "Search on KinoX") == 0)
			ret = 2;
/*
		else if(ostrcmp(mbox->name, "Search on KinoX (local)") == 0)
			ret = 8;

		else if(ostrcmp(mbox->name, "Search on Solarmovies (movie)") == 0)
			ret = 3;
		else if(ostrcmp(mbox->name, "Search on Solarmovies (serie)") == 0)
			ret = 4;
		else if(ostrcmp(mbox->name, "Search on Solarmovies (local)") == 0)
			ret = 9;
		else if(ostrcmp(mbox->name, "Search on Youtube") == 0)
			ret = 5;
		else if(ostrcmp(mbox->name, "Search on Youtube (local)") == 0)
			ret = 10;
		else if(ostrcmp(mbox->name, "Search on MyVideo") == 0)
			ret = 6;
		else if(ostrcmp(mbox->name, "Search on MyVideo (local)") == 0)
			ret = 11;
		else if(ostrcmp(mbox->name, "Search on Movie4k") == 0)
			ret = 7;
		else if(ostrcmp(mbox->name, "Search on Movie4k (local)") == 0)
			ret = 12;
*/
		else if(ostrcmp(mbox->name, "Search on InternetRadio (local)") == 0)
			ret = 13;
/*
		else if(ostrcmp(mbox->name, "Search on InternetTv (local)") == 0)
			ret = 14;
		else if(ostrcmp(mbox->name, "Search on ARD (local)") == 0)
			ret = 15;
		else if(ostrcmp(mbox->name, "Search on ZDF (local)") == 0)
			ret = 16;
		else if(ostrcmp(mbox->name, "Search on TecTime (local)") == 0)
			ret = 17;
		else if(ostrcmp(mbox->name, "Search on Giga (local)") == 0)
			ret = 18;
		else if(ostrcmp(mbox->name, "Search on Beeg (local)") == 0)
			ret = 19;
		else if(ostrcmp(mbox->name, "Search on NowTv (local)") == 0)
			ret = 20;
		else if(ostrcmp(mbox->name, "Search on All (local)") == 0)
			ret = 21;
		else if(ostrcmp(mbox->name, "Search on Xvideos (local)") == 0)
			ret = 24;
    	else if(ostrcmp(mbox->name, "Search on Mle-HD (local)") == 0)
			ret = 25;
		else if(ostrcmp(mbox->name, "Search on Netzkino (local)") == 0)
			ret = 26;
		else if(ostrcmp(mbox->name, "Search on FilmOn (local)") == 0)
			ret = 27;
*/
		else if(ostrcmp(mbox->name, "Search on PornHub") == 0 || ostrcmp(mbox->name, "Search on Xvideos") == 0)
        {
            localparser = ostrcat(mbox->name, NULL, 0, 0);
            localparser = string_replace_all("Search on ", "/tmp/localparser/", localparser, 1);
            strstrip(localparser);
		    string_tolower(localparser);
            localparser = ostrcat(localparser, ".sh", 1, 0);
		    struct skin* tithekplugin = getplugin("Titan Mediathek");
		    if(tithekplugin != NULL)
		    {
			    struct tithek* (*startplugin)(char*, char*);

			    startplugin = dlsym(tithekplugin->pluginhandle, "localparser_search_file");
			    if(startplugin != NULL)
			    {
				    startplugin(localparser, file);

			    }
		    }
            free(localparser), localparser = NULL;
        }
		else if(ostrcmp(mbox->name, "Create Service Bouquets") == 0)
        {
	        struct skin* load = getscreen("loading");
	        drawscreen(load, 0, 0);

            cmd = ostrcat(link, NULL, 0, 0);
            cmd = string_replace_all(" search ", " create_service_bouquets ", cmd, 1);

		    if(ostrstr(cmd, "%search%") != NULL)
			    cmd = string_replace_all("%search%", file, cmd, 1);

            cmd = string_newline(cmd);
		    debug(202, "cmd2: %s", cmd);

		    tmpstr1 = command(cmd);
            tmpstr1 = string_newline(tmpstr1);

	        free(cmd), cmd = NULL;

    		debug(202, "return from parser: %s", tmpstr1);

	        if(!ostrncmp("errormsg=", tmpstr1, 9))
	        {
//		        tmpstr = ostrcat(_(mbox->name), "\n\n", 0, 0);
		        tmpstr = ostrcat(tmpstr, "\n\n", 0, 0);
		        tmpstr = ostrcat(tmpstr, tmpstr1, 1, 0);
		        tmpstr = ostrcat(tmpstr, " ", 1, 0);
		        tmpstr = ostrcat(tmpstr, menutitle, 1, 0);
		        tmpstr = ostrcat(tmpstr, "-", 1, 0);
		        tmpstr = ostrcat(tmpstr, file, 1, 0);
		        tmpstr = string_replace("errormsg=", "", tmpstr, 1);
		        debug(202, "Found error Msg: %s", tmpstr1);

		        textbox(_(mbox->name), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 0, 2);
            }
            else
            {
//		        tmpstr = ostrcat(_(mbox->name), "\n\n", 0, 0);
		        tmpstr = ostrcat(tmpstr, "\n\n", 0, 0);
		        tmpstr = ostrcat(tmpstr, _("Sure to Save the Service Bouquets"), 1, 0);
		        tmpstr = ostrcat(tmpstr, " ", 1, 0);
		        tmpstr = ostrcat(tmpstr, menutitle, 1, 0);
		        tmpstr = ostrcat(tmpstr, "-", 1, 0);
		        tmpstr = ostrcat(tmpstr, file, 1, 0);
		        tmpstr = ostrcat(tmpstr, "?", 1, 0);

//	            if(textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 0, 0) == 1)
	            if(textbox(_(mbox->name), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 0, 0) == 1)
	            {
                    cmd = ostrcat(localparser_input, " ", 0, 0);
                    cmd = ostrcat(cmd, " ", 1, 0);
                    cmd = ostrcat(cmd, localparser_input, 1, 0);
                    cmd = ostrcat(cmd, " save '' '", 1, 0);
                    cmd = ostrcat(cmd, file, 1, 0);
                    cmd = ostrcat(cmd, "'", 1, 0);

           		    debug(202, "cmd3: %s", cmd);
                    system(cmd);
        	        free(cmd), cmd = NULL;

//            		textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
//        		    debug(202, "Titan will be restarted!");

	                //sync usb
//	                system("sync");
	                //write only config file
//	                writeallconfig(3);
	                //gui restart and write no config
//	                oshutdown(3, 2);

            		textbox(_("Message"), _("Titan will be reloaded Channellist!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
        		    debug(202, "Titan will be reloaded channellist!");
		            freesat();
		            freeallbouquet();
		            freemainbouquet(0);
		            freechannel(0);
		            freetransponder();
		            freeprovider();
	                ret = readsat(getconfig("satfile", NULL));
	                ret = readtransponder(getconfig("transponderfile", NULL));
	                ret = readprovider(getconfig("providerfile", NULL));
	                ret = readchannel(getconfig("channelfile", NULL));
	                ret = readtransponderencoding(getconfig("transponderencodingfile", NULL));
	                ret = readmainbouquet(getconfig("bouquetfile", NULL));
	                ret = readallbouquet();
                }
                else
                {
                    cmd = ostrcat(localparser_input, " ", 0, 0);
                    cmd = ostrcat(cmd, " ", 1, 0);
                    cmd = ostrcat(cmd, localparser_input, 1, 0);
                    cmd = ostrcat(cmd, " remove '' '", 1, 0);
                    cmd = ostrcat(cmd, file, 1, 0);
                    cmd = ostrcat(cmd, "'", 1, 0);
           		    debug(202, "cmd4: %s", cmd);
                    system(cmd);
        	        free(cmd), cmd = NULL;
                }
	        }

            free(tmpstr); tmpstr = NULL;
	        free(tmpstr1); tmpstr1 = NULL;
            free(localparser_input), localparser_input = NULL;
        }
		else if(ostrncmp("/tmp/localparser/", mbox->text,17) == 0 || ostrncmp("/mnt/parser/", mbox->text,12) == 0)
        {
	        struct skin* load = getscreen("loading");
	        drawscreen(load, 0, 0);
            
printf("playrcred mbox->text2: %s\n", mbox->text);
printf("playrcred file: %s\n", file);


            if(file_exist(mbox->text))
            {
		        struct skin* tithekplugin = getplugin("Titan Mediathek");
		        if(tithekplugin != NULL)
		        {
			        struct tithek* (*startplugin)(char*, char*);

			        startplugin = dlsym(tithekplugin->pluginhandle, "localparser_search_file");
			        if(startplugin != NULL)
			        {
				        startplugin(mbox->text, file);

			        }
		        }
            }
//            free(localparser), localparser = NULL;
/*
                cmd = ostrcat("cat ", mbox->text, 0, 0);
                cmd = ostrcat(cmd, " | grep '$SRC $SRC search ' | cut -d'#' -f2 | sed 's!$SRC!", 1, 0);
                cmd = ostrcat(cmd, mbox->text, 1, 0);
                cmd = ostrcat(cmd, "!g'", 1, 0);
//                    cmd = string_replace_all("%search%", (&ret2[1])->part, cmd, 1);

                debug(202, "cmd2: %s", cmd);
                printf("playrcred cmd2: %s\n", cmd);

                tmpstr = command(cmd);
                debug(202, "tmpstr: %s", tmpstr);
                printf("playrcred tmpstr: %s\n", tmpstr);

                free(cmd), cmd = NULL;
                cmd = ostrcat(cmd, tmpstr, 1, 0);

//                if(getconfigint("tithek_servicebouquets_autoupdate_allchannels", NULL) == 1)
//                    cmd = string_replace_all(" search ", " update_all_channels ", cmd, 1);
//                else
//                    cmd = string_replace_all(" search ", " update_service_bouquets ", cmd, 1);

                cmd = string_replace_all("%search%", file, cmd, 1);
                printf("playrcred cmd3: %s\n", cmd);

                tmpstr = command(cmd);
                printf("playrcred tmpstr: %s\n", tmpstr);

                free(cmd), cmd = NULL;
            }
//            free(localparser), localparser = NULL;

            free(tmpstr); tmpstr = NULL;
//            free(localparser), localparser = NULL;
*/

        }
		else if(ostrcmp(mbox->name, "Downloads") == 0)
		{
			if(status.play == 1)
				screenbgdownload(1);
			else
				screenbgdownload(0);
		}			
		else
		{
			pluginnode = getplugin(mbox->name);

			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "start");
				if(startplugin != NULL)
					startplugin();
			}
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
	drawscreen(skin, 0, 0);
	resettvpic();
	if(playinfobarstatus > 0 &&	status.play == 1)
		screenplayinfobar(file, showname, 0, playertype, flag);

	status.hangtime = hangtime;

	status.writeplayersub = 1;

	return ret;
}

void playrcinfo(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(checkbit(status.playercan, 14) == 0) return;

	if(*playinfobarstatus == 0)
	{
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
	else if(*playinfobarstatus == 1)
	{
		*playinfobarstatus = 0;
		screenplayinfobar(NULL, NULL, 1, playertype, flag);
		status.writeplayersub = 1;
	}
}

void playrcstop(int playertype, int flag)
{
	if(checkbit(status.playercan, 6) == 0) return;

	free(status.playfile); status.playfile = NULL;
	
	if(playertype == 1)
		playerstopts(0, 0);
	else if(playertype == 2)
		dvdstop();
	else
		playerstop();

	writevfdmenu("Player");
	screenplayinfobar(NULL, NULL, 1, playertype, flag);
}

//type 0: only from play
//type 1: only from pause
void playrcsf(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int type, int flag)
{
	if(checkbit(status.playercan, 15) == 0) return;

	if(status.playspeed == 0 && ((type == 0 && status.pause == 0) || (type == 1 && status.pause == 1)))
	{
		status.slowspeed++;
		if(status.slowspeed > 6) status.slowspeed = 6;
		if(status.slowspeed > 0)
		{
			status.play = 0;

			if(playertype == 1)
				playerslowts((int)pow(2, status.slowspeed));
			else if(playertype == 0)
				playerslow(status.slowspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.slowspeed == 0)
		{
			status.pause = 0;
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				audioclearbuffer(status.aktservice->audiodev);
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
	}
}

//type 0: only from play
//type 1: only from pause
void playrcsr(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int type, int flag)
{
	if(checkbit(status.playercan, 15) == 0) return;

	if(status.playspeed == 0 && ((type == 0 && status.pause == 0) || (type == 1 && status.pause == 1)))
	{
		status.slowspeed--;
		if(status.slowspeed < 0) status.slowspeed = 0;
		if(status.slowspeed > 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerslowts((int)pow(2, status.slowspeed));
			else if(playertype == 0)
				playerslowts(status.slowspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.slowspeed == 0)
		{
			status.pause = 0;
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				audioclearbuffer(status.aktservice->audiodev);
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
	}
}

void playrcff(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.pause == 1)
	{
		playrcsf(file, showname, playinfobarstatus, playinfobarcount, playertype, 1, flag);
		return;
	}

	if(checkbit(status.playercan, 7) == 0) return;

	if(status.pause == 0 && status.slowspeed == 0)
	{
		status.playspeed++;
		if(status.playspeed > 6) status.playspeed = 6;
		if(status.playspeed > 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerffts((int)pow(2, status.playspeed));
			else if(playertype == 2)
				dvdff(status.playspeed);
			else	
				playerff(status.playspeed);
				
			//Musik
			if(playertype != 0 || flag != 2)
			{		
				*playinfobarstatus = 2;
				*playinfobarcount = 0;
				screenplayinfobar(file, showname, 0, playertype, flag);
			}
		}
		if(status.playspeed < 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerfrts((int)(pow(2, status.playspeed * -1) * -1), 0);
			else if(playertype == 2)
				dvdfr(status.playspeed);
			else
				playerfr(status.playspeed);
			//Musik
			if(playertype != 0 || flag != 2)
			{		
				*playinfobarstatus = 2;
				*playinfobarcount = 0;
				screenplayinfobar(file, showname, 0, playertype, flag);
			}
		}
		if(status.playspeed == 0)
		{
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			//Musik
			if(playertype != 0 || flag != 2)
			{		
				*playinfobarstatus = 1;
				*playinfobarcount = 0;
				screenplayinfobar(file, showname, 0, playertype, flag);
			}
		}
	}
}

void playrcfr(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.pause == 1)
	{
		playrcsr(file, showname, playinfobarstatus, playinfobarcount, playertype, 1, flag);
		return;
	}
	
	if(checkbit(status.playercan, 8) == 0) return;

	if(status.pause == 0 && status.slowspeed == 0)
	{
		status.playspeed--;
		if(status.playspeed < -6) status.playspeed = -6;
		if(status.playspeed > 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerffts((int)pow(2, status.playspeed));
			else if(playertype == 2)
				dvdff(status.playspeed);
			else
				playerff(status.playspeed);
			//Musik
			if(playertype != 0 || flag != 2)
			{		
				*playinfobarstatus = 2;
				*playinfobarcount = 0;
				screenplayinfobar(file, showname, 0, playertype, flag);
			}
		}
		if(status.playspeed < 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerfrts((int)(pow(2, status.playspeed * -1) * -1), 0);
			else if(playertype == 2)
				dvdfr(status.playspeed);
			else
				playerfr(status.playspeed);
			//Musik
			if(playertype != 0 || flag != 2)
			{		
				*playinfobarstatus = 2;
				*playinfobarcount = 0;
				screenplayinfobar(file, showname, 0, playertype, flag);
			}
		}
		if(status.playspeed == 0)
		{
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			//Musik
			if(playertype != 0 || flag != 2)
			{		
				*playinfobarstatus = 1;
				*playinfobarcount = 0;
				screenplayinfobar(file, showname, 0, playertype, flag);
			}
		}
	}
}

void playrcpause(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.playspeed != 0 || status.slowspeed != 0)
	{
		playrcplay(file, showname, playinfobarstatus, playinfobarcount, playertype, flag);
		return;
	}
	
	if(checkbit(status.playercan, 9) == 0) return;

	if(status.pause == 1)
	{
		if(playertype == 1)
		{
			playercontinuets();
		}
		else if(playertype == 2)
			dvdcontinue();
		else
			playercontinue();
		status.slowspeed = 0;
		status.playspeed = 0;
		status.play = 1;
		status.pause = 0;
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
	else
	{
		status.slowspeed = 0;
		status.playspeed = 0;
		status.play = 0;
		status.pause = 1;
		if(playertype == 1)
			playerpausets();
		else if(playertype == 2)
			dvdpause();
		else
			playerpause();
		*playinfobarstatus = 2;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
}

void playrcplay(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(checkbit(status.playercan, 10) == 0) return;

	free(status.playfile); status.playfile = NULL;
	status.playfile = ostrcat(file, NULL, 0, 0);

	debug(10, "playertype: %i, playspeed %i, slowspeed %i", playertype, status.playspeed, status.slowspeed);
	if(playertype == 1)
	{
		
		if(status.playspeed != 0 || status.slowspeed != 0)
		{
			playerpausets();
			if(checkchipset("HI3798MV200") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || vubox1 == 1)
			{
				videoslowmotion(status.aktservice->videodev, 0);
				videofastforward(status.aktservice->videodev, 0);
				if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || vubox1 == 1)
					dmxstart(status.aktservice->dmxaudiodev);
			}
		}
		if(status.slowspeed != 0)
			audioclearbuffer(status.aktservice->audiodev);
		if((status.playspeed != 0 || status.slowspeed != 0) && checkchipset("HI3798MV200") != 1 && checkbox("DM7020HD") != 1 && checkbox("DM7020HDV2") != 1 && vubox1 != 1)
			playerresetts();
		playercontinuets();
	}
	else if(playertype == 2)
		dvdcontinue();
	else
	{
		if(status.playspeed != 0 || status.slowspeed != 0)
			playerpause();
		playercontinue();
	}
	status.slowspeed = 0;
	status.playspeed = 0;
	status.pause = 0;
	status.play = 1;
	*playinfobarstatus = 1;
	*playinfobarcount = 0;
	screenplayinfobar(file, showname, 0, playertype, flag);
}

void playrcjumpr(char* file, char* showname, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
//	if(checkbit(status.playercan, 11) == 0) return;

	unsigned long long pos = 0;
	
	if(status.pause == 0 && status.playspeed == 0 && status.slowspeed == 0)
	{
		//Musik
		if(playertype == 0 && flag == 2)
		{
			pos = playergetpts() / 90000;
			if(pos + 10 > sec)
					playerseek(sec * -1);
			else
			{
				playerstop();
				playerstart(file);
			}
			return;
		}
	
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
		
		//a jump over the beginning of the
		//file, freez the player (type 0)
		if(playertype == 1)
			playerseekts(getservice(RECORDPLAY, 0), sec * -1, 0);
		else if(playertype == 2)
			pos = dvdgetpts() / 90000;
		else
			pos = playergetpts() / 90000;
	
		if(playertype != 1)
		{
			if(pos + 10 > sec)
			{
				if(playertype == 2)
					dvdseek(sec * -1);
				else
					playerseek(sec * -1);
			}
			else
			{
				if(playertype == 2)
				{
					dvdstop();
					dvdstart(file);
				}
				else
				{
					playerstop();
					playerstart(file);
				}
			}
		}

		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		//status.play = 0;
		clearscreen(load);
		screenplayinfobar(file, showname, 0, playertype, flag);
		//status.play = 1;
		sleep(1);
	}
}

void playrcjumpto(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.pause == 0 && status.playspeed == 0 && status.slowspeed == 0)
	{
		char* tmpstr = numinput(_("Jump To (min)"), NULL, "000", 0);
		if(tmpstr != NULL)
		{
			int jump = atoi(tmpstr) * 60;
			playrcjumpf(file, showname, jump, playinfobarstatus, playinfobarcount, playertype, flag);
		}
		free(tmpstr), tmpstr = NULL;
	}
}

void playrcjumpf(char* file, char* showname, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
//	if(checkbit(status.playercan, 12) == 0) return;

	if(status.pause == 0 && status.playspeed == 0 && status.slowspeed == 0)
	{
		//Musik
		if(playertype == 0 && flag == 2)
		{
			playerseek(sec);
			return;
		}
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
		
		if(playertype == 1)
			playerseekts(getservice(RECORDPLAY, 0), sec, 0);
		else if(playertype == 2)
			dvdseek(sec);
		else
			playerseek(sec);
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		//status.play = 0;
		clearscreen(load);
		screenplayinfobar(file, showname, 0, playertype, flag);
		//status.play = 1;
	}
}

void playchangecodec(int playertype)
{
	if(checkbit(status.playercan, 13) == 0) return;

	char** tracklist = NULL;

	if(getconfigint("av_ac3default", NULL) == 1)
	{
		int i = 0;
		
		tracklist = playergettracklist(1);
		if(tracklist != NULL)
		{
			while(tracklist[i] != NULL)
			{
				if(ostrcmp(tracklist[i + 1], "A_AC3") == 0)
				{
					playerchangeaudiotrack(i / 2);
				}
				i += 2;
			}
		}
		playerfreetracklist(tracklist);
		tracklist = NULL;
	}
}

// dirrcret 1 = delete
// dirrcret 2 = 
// dirrcret 3 = epg info
// dirrcret 4 = sort
// dirrcret 5 = exit
// dirrcret 6 = rename
// dirrcret 7 = create folder

int playcheckdirrcret(char* file, int dirrcret)
{
	int ret = 0;
	char* epgfilename = NULL, *tmpstr = NULL;

	if(dirrcret == 4)
	{
		int sort = screendirsort();
		addconfigint("dirsort", sort);
		ret = 1;
	}
	if(dirrcret == 3)
	{
		epgfilename = changefilenameext(file, ".epg");

		tmpstr = readfiletomem(epgfilename, 0);
		if(tmpstr != NULL)
			textbox(_("EPG Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 2);
		else {
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".eit");
			tmpstr = readeittomem(epgfilename);
			if(tmpstr != NULL)
				textbox(_("EPG Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 2);
		}
		free(epgfilename); epgfilename = NULL;
		free(tmpstr); tmpstr = NULL;
		ret = 1;
	}
	if(dirrcret == 1)
	{
		char* subfile = NULL;
		subfile = ostrstr(file, "/movie/");
		if(subfile != NULL)
			subfile = subfile + 7;
		else
			subfile = file;
		if(getservicebyrecname(file, 0, 0) != NULL)
			textbox(_("Message"), _("Record in progress"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		else if(textbox(_("Really Delete ?"), subfile, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
		{
			unlink(file);
			if(file_exist(file))
			{
				char* cmd = ostrcat("rm -rf \"", file, 0, 0);
				cmd = ostrcat(cmd, "\"", 1, 0);
				debug(10, "cmd: %s", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}			
			epgfilename = changefilenameext(file, ".epg");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".se");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".ma");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".as");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
		}
		ret = 1;
	}
	if(dirrcret == 6)
	{
		char* subfile = NULL, *dname = NULL, *searchext = NULL;
		int overwrite;
		subfile = ostrstr(file, "/movie/");
		if(subfile != NULL)
			subfile = subfile + 7;
		else
			subfile = file;
		if(getservicebyrecname(file, 0, 0) != NULL)
			textbox(_("Message"), _("Record in progress"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		else if(textbox(_("Really Rename ?"), subfile, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
		{
			tmpstr = ostrcat(basename(file), NULL, 0, 0);

			char* search = textinput(_("Rename"), tmpstr);
			free(tmpstr), tmpstr = NULL;

			if(search != NULL)
			{
				dname = ostrcat(file, NULL, 0, 0);
				dname = dirname(dname);
				tmpstr = createpath(dname, search);
				if(file_exist(tmpstr))
				{
					if(textbox(_("File Exist, Really Rename and Overwrite exist File ?"), search, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
						overwrite = 1;
					else
						overwrite = 0;
				}
				else
					overwrite = 1;

				if(overwrite == 1)
				{
					debug(10, "rename %s > %s", file, tmpstr);
					ret = rename(file, tmpstr);
					free(tmpstr), tmpstr = NULL;
					free(dname), dname = NULL;
		
					epgfilename = changefilenameext(file, ".epg");
					if(file_exist(epgfilename))
					{
						tmpstr = ostrcat(basename(epgfilename), NULL, 0, 0);
						dname = ostrcat(epgfilename, NULL, 0, 0);
						dname = dirname(dname);
						searchext = changefilenameext(search, ".epg");
						tmpstr = createpath(dname, searchext);
						debug(10, "rename %s > %s", epgfilename, tmpstr);
						ret = rename(epgfilename, tmpstr);
						free(tmpstr), tmpstr = NULL;
						free(dname), dname = NULL;
					}
					free(epgfilename); epgfilename = NULL;
		
					epgfilename = changefilenameext(file, ".se");
					if(file_exist(epgfilename))
					{
						tmpstr = ostrcat(basename(epgfilename), NULL, 0, 0);
						dname = ostrcat(epgfilename, NULL, 0, 0);
						dname = dirname(dname);
						searchext = changefilenameext(search, ".se");
						tmpstr = createpath(dname, searchext);
						debug(10, "rename %s > %s", epgfilename, tmpstr);
						ret = rename(epgfilename, tmpstr);
						free(tmpstr), tmpstr = NULL;
						free(dname), dname = NULL;
					}
					free(epgfilename); epgfilename = NULL;
		
					epgfilename = changefilenameext(file, ".ma");
					if(file_exist(epgfilename))
					{
						tmpstr = ostrcat(basename(epgfilename), NULL, 0, 0);
						dname = ostrcat(epgfilename, NULL, 0, 0);
						dname = dirname(dname);
						searchext = changefilenameext(search, ".ma");
						tmpstr = createpath(dname, searchext);
						debug(10, "rename %s > %s", epgfilename, tmpstr);
						ret = rename(epgfilename, tmpstr);
						free(tmpstr), tmpstr = NULL;
						free(dname), dname = NULL;
					}
					free(epgfilename); epgfilename = NULL;
		
					epgfilename = changefilenameext(file, ".as");
					if(file_exist(epgfilename))
					{
						tmpstr = ostrcat(basename(epgfilename), NULL, 0, 0);
						dname = ostrcat(epgfilename, NULL, 0, 0);
						dname = dirname(dname);
						searchext = changefilenameext(search, ".as");
						tmpstr = createpath(dname, searchext);
						debug(10, "rename %s > %s", epgfilename, tmpstr);
						ret = rename(epgfilename, tmpstr);
						free(tmpstr), tmpstr = NULL;
						free(dname), dname = NULL;
					}
					free(epgfilename); epgfilename = NULL;
				}
			}
		}
//		ret = 1;
	}
	if(dirrcret == 7)
	{
		char* dname = NULL;
		dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		if(textbox(_("Really Create Folder in ?"), dname, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
		{
			tmpstr = ostrcat(basename(file), NULL, 0, 0);

			char* search = textinput(_("Create Folder"), "New Folder");
			free(tmpstr), tmpstr = NULL;

			if(search != NULL)
			{
				tmpstr = createpath(dname, search);
				debug(10, "create %s", tmpstr);
				ret = mkdir(tmpstr, 0777);
			}
		}
//		ret = 1;
	}
	return ret;
}

void playwritevfd(char* file, char* showname)
{
	char* tmpstr = NULL;

	if(showname == NULL)
	{
		tmpstr = ostrcat(file, NULL, 0, 0);
		if(tmpstr != NULL) writevfdmenu(basename(tmpstr));
		free(tmpstr); tmpstr = NULL;
	}
	else
		writevfdmenu(showname);
}


void playstartservice()
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		tmpstr = ostrcat(status.aktservice->channellist, NULL, 0, 0);
		servicecheckret(servicestart(status.aktservice->channel, tmpstr, NULL, 3), 0);
	}
	else
	{
		tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 0), 0);
	}
	free(tmpstr); tmpstr = NULL;
}

// flag 0 = dirlist/playing/infobar
// flag 1 = playing/infobar
// flag 2 = playing
// flag 3 = not stop/start live service
// flag 4 = playing with screensaver
// startfolder 2 = do nothing with playstop/playstart
int screenplay(char* startfile, char* showname, int startfolder, int flag)
{
	int rcret = 0, playertype = 0, ret = 0, rcwait = 1000, screensaver_delay = 0, holdselection = 0, waitofbuffer = 0, videooff = 0;
	char* file = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	char* tmppolicy = NULL, *startdir = NULL;
	char* formats = NULL;
	struct channel* pipchannel = NULL; 
	struct skin* playinfobar = getscreen("playinfobar");
	struct skin* sprogress = getscreennode(playinfobar, "progress");
	struct skin* load = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");

	int oldsort = getconfigint("dirsort", NULL);
	int skip13 = getconfigint("skip13", NULL);
	int skip46 = getconfigint("skip46", NULL);
	int skip79 = getconfigint("skip79", NULL);

	resettvpic();

	if((startfolder == 0 && flag != 3) || (startfolder == 2 && getconfigint("tithek_delservice", NULL) == 0))
	{
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return ret;
	}

	if(status.webplayfile != NULL)
	{
		// stream over tithek enable start
		printf("startfile: %s\n", status.webplayfile);
		struct skin* tithekplugin = getplugin("Titan Mediathek");
		if(tithekplugin != NULL)
		{
			struct tithek* (*startplugin)(char*);

			startplugin = dlsym(tithekplugin->pluginhandle, "list_hoster_streams");
			if(startplugin != NULL)
			{
				startfile = (char*)startplugin(status.webplayfile);
				printf("startfile changed: %s\n", startfile);
			}
		}
		else
			startfile = status.webplayfile;

		// stream over tithek enable end

		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return ret;
	}

	// allowed from atemio avi mkv mpg4 xvid mpg1 mpg2 jpeg png
	if(startfolder == 1)
		formats = ostrcat(formats, ".ts", 1, 0);
	else if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
		formats = ostrcat(formats, "aac .avi .mkv .mp4 .ts .mov .flv .flac .mp3 .mpg .m2ts .vob .evo .wmv .wma .asf .mp2 .m4v .m4a .fla .divx .dat .mpeg .trp .mts .vdr .ogg .wav .wtv .asx .mvi .png .jpg .jpeg .ra .ram .rm .3gp .amr .rmvb .rm .webm .opus .m3u8 .mpd", 1, 0);
	else
		formats = ostrcat(formats, ".avi .mkv .mpg .mpeg .ts .mp3 .flac .ogg", 1, 0);
	
	status.updatevfd = PAUSE;
	tmppolicy = getpolicy();

playerstart:
	//reset timeline
	sprogress->progresssize = 0;

	if(startfolder == 0)
		startdir = getconfig("rec_moviepath", NULL);
	else
		startdir = getconfig("rec_path", NULL);

	status.playspeed = 0, status.play = 0, status.pause = 0, status.slowspeed = 0;
	int playinfobarcount = 0, playinfobarstatus = 1, dirrcret = 0;

	if(startfile == NULL)
	{
		readmediadb(getconfig("mediadbfile", NULL), 0, 0);

		tmpstr = ostrcat(file, NULL, 0, 0);
		char* tmpstartdir = startdir;
		if(file != NULL)
		{
			tmpstr1 = ostrcat(file, NULL, 0, 0);
			startdir = dirname(tmpstr1);
		}
		free(file); file = NULL;
		file = screendirreal(startdir, formats, basename(tmpstr), &dirrcret, ".epg", _("DEL"), getrcconfigint("rcred", NULL), _("SELECT"), 0, _("EPG"), getrcconfigint("rcyellow", NULL), _("SORT"), getrcconfigint("rcblue", NULL), 90, 1, 90, 1, holdselection, 64);
		holdselection = 0;
		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;
		startdir = tmpstartdir;
	}
	else
		file = ostrcat(startfile, NULL, 0, 0);

	if(file == NULL)
	{
		if(playcheckdirrcret(file, dirrcret) == 1)
			goto playerstart;
	}

	if(file != NULL)
	{
//		if(cmpfilenameext(file, ".ts") == 0)
        if(getconfigint("playertype", NULL) == 1 && (cmpfilenameext(file, ".ts") == 0 || cmpfilenameext(file, ".mts") == 0 || cmpfilenameext(file, ".m2ts") == 0))
		{
			playertype = 1;
			addconfig("lastplayertype", "1");
		}
		else
			addconfig("lastplayertype", "0");	

		if(startfile == NULL)
		{
			tmpstr = ostrcat(file, NULL, 0, 0);
			if(tmpstr != NULL && startfolder == 0) addconfig("rec_moviepath", dirname(tmpstr));
			free(tmpstr); tmpstr = NULL;
		
			if(playcheckdirrcret(file, dirrcret) == 1)
			{
				if(dirrcret == 1) holdselection = 1;
				goto playerstart;
			}

			if(startfolder == 1 && flag != 3)
			{
				rcret = servicestop(status.aktservice, 1, 1);
				if(rcret == 1)
				{
					free(tmppolicy);
					free(file);
					free(formats);
					addconfigint("dirsort", oldsort);
					return ret;
				}
			}
		}

		drawscreen(skin, 0, 0);
		drawscreen(load, 0, 0);
		playwritevfd(file, showname);
		printf("player--> playertype: %i\n", playertype);

		if(playertype == 1)
			rcret = playerstartts(file, 0);
		else if(playertype == 2)
			rcret = playerstart(file);
		else
		{
#if defined (EXTGST)
			if(getconfigint("extplayer_type", NULL) == 0)
			{
#endif
	#ifdef EPLAYER3
				if(ostrstr(file, "http://") == file || ostrstr(file, "https://") == file)
				{
					struct stimerthread* bufferstatus = addtimer(&screenplaybufferstatus, START, 1000, 1, NULL, NULL, NULL);
					rcret = playerstart(file);
					if(bufferstatus != NULL && gettimer(bufferstatus) != NULL)
					{
						bufferstatus->aktion = STOP;
						usleep(100000);
					}
				}
				else
					rcret = playerstart(file);
	#else
				rcret = playerstart(file);
	#endif
#if defined (EXTGST)
			}
			else
				rcret = playerstart(file);
#endif
		}

#ifndef SIMULATE
		if(rcret != 0)
		{
			textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
			writevfdmenu("Player");
			
			if(startfile == NULL)
			{
				if(startfolder == 1 && flag != 3) playstartservice();
				goto playerstart;
			}
			else
			{
				ret = 2;
				goto playerend;
			}
		}
#endif
		if(playertype == 0 && getconfigint("showlastpos", NULL) == 1 && status.actplaypts != NULL && file_exist(status.actplaypts))
		{
			FILE* fbseek = fopen(status.actplaypts, "r");
			if(fbseek != NULL)
			{
				if(textbox(_("Message"), _("Start at last position ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0) == 1)
				{
					char* skip1 = calloc(1, 20); 
					if(skip1 != NULL) 
					{
						fscanf(fbseek,"%s",skip1);
						playrcjumpf(file, showname, atoll(skip1), &playinfobarstatus, &playinfobarcount, playertype, flag);

					}
					free(skip1), skip1 = NULL;
				}
				fclose(fbseek);
			}
		}

		clearscreen(load);
		if(status.prefillbuffer == 0)
			screenplayinfobar(file, showname, 0, playertype, flag);
		else
			waitofbuffer = 1;
		
		if(flag == 4 && getconfigint("screensaver", NULL) == 1)
		{
			screensaver_delay = getconfigint("screensaver_delay", NULL);
			initscreensaver();
		}								
		
		//change codec if ac3default and video has ac3
		//deaktivate, freeze player, makes a seek -5
		//see eplayer container_ffmpeg_switch_audio
		//the seek to the beginning of the file freez
		//eplayer.
		//playchangecodec();
		free(status.playfile); status.playfile = NULL;
		status.playfile = ostrcat(file, NULL, 0, 0);
		status.play = 1;
		int count = 0;
		while(1)
		{
			while((playertype == 0 && playerisplaying()) || (playertype == 1 && playerisplayingts()))
			{
				rcret = waitrc(playinfobar, rcwait, 0);
#if defined (EXTGST)
				if(getconfigint("extplayer_type", NULL) == 1)
				{
#endif
	#ifdef EPLAYER4
					debug(150, "waitofbuffer=%d status.cleaninfobar=%d status.prefillbuffer=%d status.prefillbuffercount=%d playinfobarstatus=%d playinfobarcount=%d",waitofbuffer , status.cleaninfobar, status.prefillbuffer, status.prefillbuffercount, playinfobarstatus, playinfobarcount);

					if(waitofbuffer == 1 &&	status.prefillbuffer == 0 && (status.cleaninfobar == 1 || status.prefillbuffercount == 200))
					{
						drawscreen(skin, 0, 0);
						if(videooff == 0) screenplayinfobar(file, showname, 0, playertype, flag);
						waitofbuffer = 0;
						status.cleaninfobar = 0;				
					}
					else if(waitofbuffer == 0 && status.prefillbuffer == 0 && (status.cleaninfobar == 0 || status.prefillbuffercount == 200))
					{
						playinfobarcount++;
						if(playinfobarstatus > 0)
							if(videooff == 0) screenplayinfobar(file, showname, 0, playertype, flag);
						if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
						{
							playinfobarstatus = 0;
							if(videooff == 0) screenplayinfobar(NULL, NULL, 1, playertype, flag);
							status.writeplayersub = 1;
						}
					}
	#else
	#ifdef EXTEPLAYER3
					getsubtext();
	#endif
					playinfobarcount++;
					if(playinfobarstatus > 0)
						if(videooff == 0) screenplayinfobar(file, showname, 0, playertype, flag);
					if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
					{
						playinfobarstatus = 0;
						if(videooff == 0) screenplayinfobar(NULL, NULL, 1, playertype, flag);
						status.writeplayersub = 1;
					}

					if(waitofbuffer == 1 &&	status.prefillbuffer == 0)
					{
						if(videooff == 0) screenplayinfobar(file, showname, 0, playertype, flag);
						waitofbuffer = 0;
					}
	#endif
#if defined (EXTGST)
				}
				else
				{
	#ifdef EXTEPLAYER3
					getsubtext();
	#endif
					playinfobarcount++;
					if(playinfobarstatus > 0)
						if(videooff == 0) screenplayinfobar(file, showname, 0, playertype, flag);
					if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
					{
						playinfobarstatus = 0;
						if(videooff == 0) screenplayinfobar(NULL, NULL, 1, playertype, flag);
						status.writeplayersub = 1;
					}

					if(waitofbuffer == 1 &&	status.prefillbuffer == 0)
					{
						if(videooff == 0) screenplayinfobar(file, showname, 0, playertype, flag);
						waitofbuffer = 0;
					}
				}
#endif
				if(flag == 4)
				{
					if(status.play == 1 && screensaver != NULL)
						count++;
	
					if(count > screensaver_delay && screensaver != NULL)
					{
						showscreensaver();
#if defined (EXTGST)
						if(getconfigint("extplayer_type", NULL) == 1)
						{
#endif

	#ifdef EPLAYER4
							if(screensaver->speed < 50)
								rcwait = screensaver->speed * 10;
							else
								rcwait = screensaver->speed;
	#else
							rcwait = screensaver->speed;
	#endif
#if defined (EXTGST)
						}
#endif

					}
				}

#if defined (EXTGST)
				if(getconfigint("extplayer_type", NULL) == 0)
				{
#endif
	#ifdef EXTEPLAYER3
	//				if(rcret == getrcconfigint("rchelp", NULL))
	//					printf("getsubtext: %s\n", getsubtext());
	#endif
#if defined (EXTGST)
				}
#endif

				if(rcret == getrcconfigint("rcpip", NULL))
				{
					if(status.pipservice->videodev == NULL)
					{
						pipchannel = status.lastservice->channel;
						printf("++++ RC: %i\n",pipstart(pipchannel, NULL, 0));
					}
					else
					{
						pipstop(status.pipservice, 0);
						pipchannel = NULL;
					}
				}				
				
				if(rcret == getrcconfigint("rcyellow", NULL))
					playrcyellow(file, showname, playinfobarstatus, playertype, flag);
				
				if(rcret == getrcconfigint("rctext", NULL) || rcret == getrcconfigint("rcsubtitel", NULL))
					playrctext(file, showname, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcgreen", NULL))
					playrcgreen(file, showname, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcblue", NULL))
					playrcblue(file, showname, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcok", NULL))
					playrcok(file, showname, playinfobarstatus, playertype, flag);
				
				if(rcret == getrcconfigint("rcred", NULL))
					playrcred(file, showname, NULL, NULL, playinfobarstatus, playertype, flag);

				if(rcret == getrcconfigint("rcinfo", NULL))
					playrcinfo(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcpower", NULL) && status.play == 1 && videooff == 0)
				{
					videooff = 1;
					drawscreen(blackscreen, 0, 0);
				}
				else if(rcret == getrcconfigint("rcexit", NULL) && videooff == 1)
				{
					videooff = 0;
					drawscreen(skin, 0, 0);
				}
				else if(rcret == getrcconfigint("rcstop", NULL) || rcret == getrcconfigint("rcexit", NULL))
				{
					if(status.prefillbuffer == 1)
					{
						status.prefillbuffer = 2;
						continue;
					}

					if(playertype == 0 && getconfigint("showlastpos", NULL) == 1 && status.actplaypts != NULL && status.play == 1)
					{
						FILE* fbseek = fopen(status.actplaypts, "w");
						if(fbseek != NULL)
						{
							off64_t pos = playergetpts() / 90000;
							fprintf(fbseek,"%lld", pos);
							fclose(fbseek);
						}
					}

					playrcstop(playertype, flag);
					if(startfile == NULL)
					{						
						if(startfolder == 1 && flag != 3) playstartservice();
						goto playerstart;
					}
					else
					{
						ret = 1;
						goto playerend;
					}
				}
				
				if(rcret == getrcconfigint("rcsf", NULL))
					playrcsf(file, showname, &playinfobarstatus, &playinfobarcount, playertype, 0, flag);
          
        		if(rcret == getrcconfigint("rcsr", NULL))
					playrcsr(file, showname, &playinfobarstatus, &playinfobarcount, playertype, 0, flag);

				if(rcret == getrcconfigint("rcff", NULL))
					playrcff(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcfr", NULL))
					playrcfr(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
				{	
					playrcpause(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
					continue;
				}

				if(rcret == getrcconfigint("rcplay", NULL))
					playrcplay(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcleft", NULL))
					playrcjumpr(file, showname, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc1", NULL))
					playrcjumpr(file, showname, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc4", NULL))
					playrcjumpr(file, showname, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc7", NULL))
					playrcjumpr(file, showname, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rc2", NULL))
					playrcjumpto(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcright", NULL))
					playrcjumpf(file, showname, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc3", NULL))
					playrcjumpf(file, showname, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc6", NULL))
					playrcjumpf(file, showname, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc9", NULL))
					playrcjumpf(file, showname, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcdown", NULL))
					playrcjumpr(file, showname, 300, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcup", NULL))
					playrcjumpf(file, showname, 300, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc0", NULL))
					setmarker();
					
				if(rcret == getrcconfigint("rc5", NULL))
					screenmarker(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcmenu", NULL)) {
					char* asfile = changefilenameext(file, ".as");
					if(status.autoseek == 0) {
						status.autoseek = 1;
						textbox(_("Message"), _("Marker autoseek is started"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 10, 0);
						addtimer(&markerautoseek_thread, START, 10000, 1, NULL, NULL, NULL);
						FILE* testseek = fopen(asfile, "w");
						if(testseek != NULL)
						{
							fprintf(testseek,"autoseek is started\n");
							fclose(testseek);
						}
					}
					else {
						status.autoseek = 0;
						textbox(_("Message"), _("Marker autoseek is stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 10, 0);
						unlink(asfile);
					}
					free(asfile); asfile = NULL;
				}
			}
			//don't change this sleep, without this
			//the player stops to fast, and a last seek can
			//produce a segfault
			
			if(status.repeat == 1)
				goto playerstart;

			if(playertype == 0 && getconfigint("showlastpos", NULL) == 1 && status.actplaypts != NULL && file_exist(status.actplaypts) && status.play == 1)
				unlink(status.actplaypts);

playerend:
			sleep(1);
			if(playertype == 1)
				playerafterendts();
			else
				playerafterend();

			writevfdmenu("Player");
			screenplayinfobar(file, showname, 1, playertype, flag);

			if(startfile == NULL)
			{
				if(startfolder == 1 && flag != 3) playstartservice();
				goto playerstart;
			}
			else
				break;
		}
	}
	if(startfolder == 0 && flag != 3) playstartservice();
	status.updatevfd = START;
	
	if(status.webplayfile != NULL)
	{
		playstartservice();
		free(status.webplayfile); status.webplayfile = NULL;
	}

	if(tmppolicy != NULL)
	{
		setpolicy(tmppolicy);
		free(tmppolicy);
	}
	
	if(flag == 4)
		deinitscreensaver();

	addconfigint("dirsort", oldsort);
	free(status.playfile); status.playfile = NULL;
	status.slowspeed = 0; 
	status.playspeed = 0;
	status.pause = 0;
	status.play = 0;
	free(file);
	free(formats);

	if(status.mediadbthread == NULL)
	{
		if(status.writemediadb == 1)
			writemediadb(getconfig("mediadbfile", NULL), NULL);
		freemediadb(0);
	}

	return ret;
}

void streamplayer(struct channel* chnode, int flag)
{
    char* newkey = NULL, *oldkey = NULL;
    printf("streamplayer playerstart%d name: %s\n", flag, chnode->name);
    printf("streamplayer playerstart%d streamurl: %s\n", flag, chnode->streamurl);
    printf("streamplayer playerstart%d epgurl: %s\n", flag, chnode->epgurl);
    addconfigtmp("playerbuffersize", "0");
    addconfigtmp("playerbufferseektime", "0");
//    if(/*status.play != 2 && getconfigint("lastplayertype", NULL) == 0 && */checkbox("DM900") == 1)
//    sleep(4); //woraround EPG
    servicestop(status.aktservice, 1, 5);
    printf("for console test add streamplayer_use_console=1 to titan.cfg\n");

    if(getconfigint("tithek_servicebouquets_autoupdate", NULL) == 1)
    {
        if(ostrstr(chnode->streamurl, "vavoo_auth=") != NULL)
        {
            if(file_exist("/tmp/vavoo.authkey"))
            {
                oldkey = string_resub("vavoo_auth=", "|User-Agent", chnode->streamurl, 0);
                debug(202, "oldkey: %s", oldkey);
                newkey = readfiletomem("/tmp/vavoo.authkey", 0);
                debug(202, "newkey: %s", newkey);
                if(ostrcmp(newkey, oldkey) != 0)
                {
                    printf("streamplayer playerstart%d set new VaVoo Key\n", flag);
                    debug(202, "set newkey: %s", newkey);
                    chnode->streamurl = string_replace(oldkey, newkey, chnode->streamurl, 1);
                }
                free(newkey), newkey = NULL;
                free(oldkey), oldkey = NULL;
            }
        }
    }

	if(getconfigint("streamplayer_use_console", NULL) == 1)
	{
	    printf("console test:\n");
		printf("gst-launch playbin uri=%s\n", chnode->streamurl);
		printf("gstplayer %s\n", chnode->streamurl);
		printf("eplayer3 %s\n", chnode->streamurl);
		printf("exteplayer3 %s\n", chnode->streamurl);
	}
	else
	{
		playerstart(chnode->streamurl);
#ifdef DREAMBOX
        if(getconfigint("extplayer_type", NULL) == 1)
        {
    		playerpause();
            usleep(1000000);
            playercontinue();
        }
#endif
	}

    status.play = 2;
    delconfigtmp("playerbuffersize");
    delconfigtmp("playerbufferseektime");
}

#endif
