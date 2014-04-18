#ifndef TMC_H
#define TMC_H

extern struct mediadbfilter* mediadbfilter;
extern struct mediadbcategory* mediadbcategory;
extern struct skin* skin;
struct mediadbfilter* mediadbfilterpos = NULL;
int mediadbfiltercount = 0;

//Mainmenu
int menu0pos = 0;
char* menu0[] = {"skin/tmcsettings.png", "skin/tmcpicture.png", "skin/tmcvideo.png", "skin/tmcaudio.png", "skin/tmcdirplay.png", "skin/tmcplaylist.png", "skin/tmcexit.png"};
char* menu0txt[] = {"Settings", "Picture", "Video", "Audio", "Play", "Playlist", "Exit"};

//Settings
int menu1pos = 0;
char* menu1[] = {"skin/tmcscan.png", "skin/tmcscandir.png", "skin/tmcsettings.png", "skin/tmcdb.png", "skin/tmceditplaylist.png", "skin/tmcback.png"};
char* menu1txt[] = {"Scan All", "Scan Dir", "Main", "Database Info", "Playlist Edit", "Back"};

//Picture
int menu2pos = 0;
char* menu2[] = {"skin/tmccategory.png", "skin/tmcrandom.png", "skin/tmcall.png", "skin/tmcone.png", "skin/tmcaz.png", "skin/tmcback.png"};
char* menu2txt[] = {"Category", "Random", "All", "Single", "A-Z", "Back"};

//Video
int menu3pos = 0;
char* menu3[] = {"skin/tmcgenre.png", "skin/tmccategory.png", "skin/tmcall.png", "skin/tmcyear.png", "skin/tmcdirector.png", "skin/tmcactors.png", "skin/tmcrating.png", "skin/tmcaz.png", "skin/tmcnoimdb.png", "skin/tmcback.png"};
char* menu3txt[] = {"Genre", "Category", "All", "Year", "Director", "Actors", "Rating", "A-Z", "No IMDB", "Back"};

//Audio
int menu4pos = 0;
char* menu4[] = {"skin/tmccategory.png", "skin/tmcrandom.png", "skin/tmcall.png", "skin/tmcone.png", "skin/tmcgenre.png", "skin/tmcyear.png", "skin/tmcactors.png", "skin/tmcaz.png", "skin/tmcback.png"};
char* menu4txt[] = {"Category", "Random", "All", "Single", "Genre", "Year", "Actors", "A-Z", "Back"};


//flag 0: blit
//flag 1: don't blit
void tmcchangestatus(struct skin* tmcstatus, char* tmcstatusbg, int flag)
{
	int pos = 0;
	char* tmpstr = NULL;

	if(mediadbfilterpos != NULL) pos = mediadbfilterpos->count + 1;

	tmpstr = ostrcat(oitoa(pos), " / ", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(mediadbfiltercount), 1, 1);

	restorescreennofree(tmcstatusbg, tmcstatus);
	changetext(tmcstatus, tmpstr);
	drawscreen(tmcstatus, 0, flag);
}

//flag 0: don't draw
//flag 1: draw
void tmcpicdel(struct skin* tmcpictitle, struct skin* tmcpicstar, struct skin* tmcstatus, struct skin* tmcpic1, struct skin* tmcpic2, struct skin* tmcpic3, struct skin* tmcpic4, struct skin* tmcpic5, char* tmcpictitlebg, char* tmcpicstarbg, char* tmcstatusbg, int flag)
{
	changepic(tmcpic1, NULL);
	changepic(tmcpic2, NULL);
	changepic(tmcpic3, NULL);
	changepic(tmcpic4, NULL);
	changepic(tmcpic5, NULL);
	changeret(tmcpic3, NULL);
	changetext(tmcpictitle, NULL);
	changepic(tmcpicstar, NULL);
	changetext(tmcstatus, NULL);
	restorescreennofree(tmcpictitlebg, tmcpictitle);
	restorescreennofree(tmcpicstarbg, tmcpicstar);
	restorescreennofree(tmcstatusbg, tmcstatus);

	if(flag == 1)
	{
		drawscreen(tmcpic1, 0, 1);
		drawscreen(tmcpic2, 0, 1);
		drawscreen(tmcpic3, 0, 1);
		drawscreen(tmcpic4, 0, 1);
		drawscreen(tmcpic5, 0, 0);
	}
}

//flag 0: draw only
//flag 1: left
//flag 2: right
void tmcpicscroll(int menuid, struct skin* tmcpictitle, struct skin* tmcpicstar, struct skin* tmcstatus, struct skin* tmcpic1, struct skin* tmcpic2, struct skin* tmcpic3, struct skin* tmcpic4, struct skin* tmcpic5, char* tmcpictitlebg, char* tmcpicstarbg, char* tmcstatusbg, int flag)
{
	int count = 0;
	struct mediadbfilter* node = mediadbfilterpos;
	char* tmpstr = NULL, *tmpfile = NULL;

	tmcpicdel(tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);

	while(1)
	{
		if(flag == 0 && count == 0)
		{
			if(node == NULL) node = mediadbfilter;
			mediadbfilterpos = node;
		}
		if(flag == 1 && count == 0)
		{
			if(node == NULL) node = mediadbfilter;
			if(node != NULL) node = node->next;
			if(node == NULL) node = mediadbfilter;
			mediadbfilterpos = node;
		}
		if(flag == 2 && count == 0)
		{
			if(node == NULL) node = getlastmediadbfilter(mediadbfilter, 0);
			if(node != NULL) node = node->prev;
			if(node == NULL) node = getlastmediadbfilter(mediadbfilter, 0);
			mediadbfilterpos = node;
		}
		if(count != 0)
		{
			if(node == NULL) node = mediadbfilter;
			if(node != NULL) node = node->next;
			if(node == NULL) node = mediadbfilter;
		}

		if(node != NULL && node->node != NULL)
		{
			if(menuid == 2) //picture
			{
				if(node->node->poster != NULL && strlen(node->node->poster) > 0)
				{
					tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					tmpstr = ostrcat(tmpstr, node->node->poster, 1, 0);
          if(getconfigint("tmcpreview", NULL) == 0)
            tmpstr = ostrcat(tmpstr, "_thumb.jpg", 1, 0);
          else
            tmpstr = ostrcat(tmpstr, "_cover.jpg", 1, 0);
				}
				else
				{
          if(getconfigint("tmcpreview", NULL) == 2)
            tmpstr = ostrcat(node->node->path, "/", 0, 0);
          else
            tmpstr = ostrcat(node->node->path, "/.Thumbnails/", 0, 0);
					tmpstr = ostrcat(tmpstr, node->node->file, 1, 0);
					tmpstr = addmountpart(tmpstr, 1);
				}
			}
			else if(menuid == 3) //video
			{
				tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				tmpstr = ostrcat(tmpstr, node->node->poster, 1, 0);
				if(getconfigint("tmcpreview", NULL) == 0)
          tmpstr = ostrcat(tmpstr, "_thumb.jpg", 1, 0);
        else
          tmpstr = ostrcat(tmpstr, "_cover.jpg", 1, 0);
			}
			else if(menuid == 4) //audio
			{
				tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				tmpstr = ostrcat(tmpstr, node->node->poster, 1, 0);
				if(getconfigint("tmcpreview", NULL) == 0)
          tmpstr = ostrcat(tmpstr, "_thumb.jpg", 1, 0);
        else
          tmpstr = ostrcat(tmpstr, "_cover.jpg", 1, 0);
			}
			
			if(!file_exist(tmpstr))
			{
				free(tmpstr);
				if(menuid == 2)
					tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcnopic.jpg", NULL, 0, 0);
				else if(menuid == 3)
					tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcnovideo.jpg", NULL, 0, 0);
				else if(menuid == 4)
					tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcnoaudio.jpg", NULL, 0, 0);
			}
			
			if(count == 0) changepic(tmcpic1, tmpstr);
			if(count == 1) changepic(tmcpic2, tmpstr);
			if(count == 2)
			{
				changepic(tmcpic3, tmpstr);
				tmpfile = ostrcat(node->node->path, "/", 0, 0);
				tmpfile = ostrcat(tmpfile, node->node->file, 1, 0);
				tmpfile = addmountpart(tmpfile, 1);
				changeret(tmcpic3, tmpfile);
				free(tmpfile); tmpfile = NULL;
				changetext(tmcpictitle, node->node->title);
				
				int rating = node->node->rating;
				
				if(rating > 9)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar5.png");
				else if(rating > 8)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar4x.png");
				else if(rating > 7)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar4.png");
				else if(rating > 6)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar3x.png");
				else if(rating > 5)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar3.png");
				else if(rating > 4)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar2x.png");
				else if(rating > 3)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar2.png");
				else if(rating > 2)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar1x.png");
				else if(rating > 1)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar1.png");
				else if(rating > 0)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar0x.png");
				else
					changepic(tmcpicstar, NULL);
			}
			if(count == 3) changepic(tmcpic4, tmpstr);
			if(count == 4) changepic(tmcpic5, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		count++; if(count > 4) break;
	}

	tmcchangestatus(tmcstatus, tmcstatusbg, 1);

	drawscreen(tmcpictitle, 0, 1);
	drawscreen(tmcpicstar, 0, 1);
	drawscreen(tmcpic1, 0, 1);
	drawscreen(tmcpic2, 0, 1);
	drawscreen(tmcpic3, 0, 1);
	drawscreen(tmcpic4, 0, 1);
	drawscreen(tmcpic5, 0, 0);
}

//flag 0: draw only
//flag 1: left
//flag 2: right
void tmcmenuscroll(int menuid, int active, struct skin* tmcmenutxt, struct skin* tmcmenu1, struct skin* tmcmenu2, struct skin* tmcmenu3, struct skin* tmcmenu3p2, struct skin* tmcmenu4, struct skin* tmcmenu5, int flag)
{
	int id = 0, max = 0;
	char** menu = NULL, **menutxt = NULL;
	int *menupos = 0;
	char* tmpstr = NULL;

	if(menuid == 0)
	{
		menu = menu0;
		menutxt = menu0txt;
		max = (sizeof(menu0) / sizeof(char*)) - 1;
		menupos = &menu0pos;
	}
	else if(menuid == 1)
	{
		menu = menu1;
		menutxt = menu1txt;
		max = (sizeof(menu1) / sizeof(char*)) - 1;
		menupos = &menu1pos;
	}
	else if(menuid == 2)
	{
		menu = menu2;
		menutxt = menu2txt;
		max = (sizeof(menu2) / sizeof(char*)) - 1;
		menupos = &menu2pos;
	}
	else if(menuid == 3)
	{
		menu = menu3;
		menutxt = menu3txt;
		max = (sizeof(menu3) / sizeof(char*)) - 1;
		menupos = &menu3pos;
	}
	else if(menuid == 4)
	{
		menu = menu4;
		menutxt = menu4txt;
		max = (sizeof(menu4) / sizeof(char*)) - 1;
		menupos = &menu4pos;
	}

	if(flag == 1) (*menupos)++;
	if(flag == 2) (*menupos)--;

	if(*menupos < 0) *menupos = max;
	if(*menupos > max) *menupos = 0;

	id = *menupos;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu1, tmpstr);
	free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu2, tmpstr);
	free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu3p2, tmpstr);
	free(tmpstr); tmpstr = NULL;
	changetext(tmcmenutxt, _(menutxt[id]));
	changeret(tmcmenutxt, menutxt[id]);
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu4, tmpstr);
	free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu5, tmpstr);
	free(tmpstr); tmpstr = NULL;

	if(active == 1)
	{
		changetext(tmcmenu1, NULL);
		changetext(tmcmenu2, NULL);
		changetext(tmcmenu3p2, NULL);
		changetext(tmcmenu4, NULL);
		changetext(tmcmenu5, NULL);
	}
	else
	{
		changetext(tmcmenu1, "X");
		changetext(tmcmenu2, "X");
		changetext(tmcmenu3p2, "X");
		changetext(tmcmenu4, "X");
		changetext(tmcmenu5, "X");
	}

	drawscreen(tmcmenutxt, 0, 1);
	drawscreen(tmcmenu1, 0, 1);
	drawscreen(tmcmenu2, 0, 1);
	drawscreen(tmcmenu3, 0, 1);
	drawscreen(tmcmenu4, 0, 1);
	drawscreen(tmcmenu5, 0, 0);
}

char* screentmcdirplay()
{
	struct skin* dir = getscreen("dir");
	char* ret = NULL, *bg = NULL, *formats = NULL;

	dir->width = 700;
	dir->prozwidth = 0;
	dir->height = 650;
	dir->prozheight = 0;

	drawscreen(dir, 2, 0);
	bg = savescreen(dir);

	if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
		formats = ostrcat(formats, "*.flac *.ogg *.mp3 *.avi *.dat *.divx *.flv *.mkv *.m4v *.mp4 *.mov *.mpg *.mpeg *.mts *.m2ts *.trp *.ts *.vdr *.vob *.wmv *.rm", 1, 0);
	else
		formats = ostrcat(formats, "*.mp3 *.flac *.ogg *.avi *.mkv *.mpg *.mpeg *.ts", 1, 0);

	ret = screendir(getconfig("rec_moviepath", NULL), formats, NULL, NULL, NULL, NULL, 0, "SELECT", getrcconfigint("rcgreen", NULL), NULL, 0, NULL, 0, dir->width, dir->prozwidth, dir->height, dir->prozheight, 0);

	restorescreen(bg, dir);
	free(formats); formats = NULL;
	blitfb(0);
	return ret;
}

void screentmcdb()
{
	int rcret = 0, videocount = 0, audiocount = 0, picturecount = 0;
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* listbox = getscreennode(tmcpic3, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *tmppic = NULL;

	tmppic = ostrcat(tmcpic3->pic, NULL, 0, 0);
	changepic(tmcpic3, NULL);
	tmcpic3->bgcol = 0;

	addscreenrc(tmcpic3, listbox);

	while(1)
	{
		tmp = NULL;
		videocount = 0, audiocount = 0, picturecount = 0;
		getmediadbcounts(&videocount, &audiocount, &picturecount);
		delmarkedscreennodes(tmcpic3, 1);

		tmp = addlistbox(tmcpic3, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(_("Status"), ": ", 0, 0);
			if(status.mediadbthread != NULL)
				tmpstr = ostrcat(tmpstr, _("scan"), 1, 0);
			else
				tmpstr = ostrcat(tmpstr, _("idle"), 1, 0);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		tmp = addlistbox(tmcpic3, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(_("Video"), ": ", 0, 0);
			tmpstr = ostrcat(tmpstr, oitoa(videocount), 1, 1);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		tmp = addlistbox(tmcpic3, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(_("Audio"), ": ", 0, 0);
			tmpstr = ostrcat(tmpstr, oitoa(audiocount), 1, 1);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		tmp = addlistbox(tmcpic3, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(_("Picture"), ": ", 0, 0);
			tmpstr = ostrcat(tmpstr, oitoa(picturecount), 1, 1);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		
		drawscreen(tmcpic3, 0, 0);
		rcret = waitrc(tmcpic3, 1000, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(tmcpic3);
	delmarkedscreennodes(tmcpic3, 1);

	changepic(tmcpic3, tmppic);
	tmcpic3->bgcol = -1;
	free(tmppic); tmppic = NULL;
	drawscreen(tmcpic3, 0, 0);
}

char* screentmccategory(int type, char* category)
{
	int rcret = 0;
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* listbox = getscreennode(tmcpic3, "listbox");
	struct skin* tmp = NULL;
	struct mediadbcategory* node = NULL;
	char* tmpstr = NULL, *tmppic = NULL, *ret = NULL;

	tmppic = ostrcat(tmcpic3->pic, NULL, 0, 0);
	changepic(tmcpic3, NULL);
	tmcpic3->bgcol = 0;

	tmpstr = ostrcat(getconfig("mediadbfile", NULL), category, 0, 0);

	if(status.mediadbthreadstatus != 3)
	{
		freemediadbcategory(0);
		readmediadb(tmpstr, type, 1);

		node = mediadbcategory;
		while(node != NULL)
		{
			tmp = addlistbox(tmcpic3, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, node->name);
			}
			node = node->next;
		}
		freemediadbcategory(0);
	}
	free(tmpstr); tmpstr = NULL;

	addscreenrc(tmcpic3, listbox);
	drawscreen(tmcpic3, 0, 0);

	while(1)
	{
		rcret = waitrc(tmcpic3, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			ret = ostrcat(listbox->select->text, NULL, 0, 0);
			break;
		}
	}

	delownerrc(tmcpic3);
	delmarkedscreennodes(tmcpic3, 1);

	changepic(tmcpic3, tmppic);
	tmcpic3->bgcol = -1;
	free(tmppic); tmppic = NULL;
	drawscreen(tmcpic3, 0, 0);

	return ret;
}

int screentmcsettings()
{
	int rcret = 0, ret = 0;
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* listbox = getscreennode(tmcpic3, "listbox");
	struct skin* pictimeout = getscreennode(tmcpic3, "pictimeout");
	struct skin* picfull = getscreennode(tmcpic3, "picfull");
	struct skin* picname = getscreennode(tmcpic3, "picname");
	struct skin* scan = getscreennode(tmcpic3, "scan");
	struct skin* bgpic = getscreennode(tmcpic3, "bgpic");
  struct skin* preview = getscreennode(tmcpic3, "preview");
	struct skin* tmp = NULL;
	char* tmppic = NULL;

	tmppic = ostrcat(tmcpic3->pic, NULL, 0, 0);
	changepic(tmcpic3, NULL);
	tmcpic3->bgcol = 0;

	pictimeout->hidden = NO;
	picfull->hidden = NO;
	picname->hidden = NO;
	scan->hidden = NO;
	bgpic->hidden = NO;
  preview->hidden = NO;

	addchoicebox(pictimeout, "5", "5");
	addchoicebox(pictimeout, "10", "10");
	addchoicebox(pictimeout, "15", "15");
	addchoicebox(pictimeout, "20", "20");
	setchoiceboxselection(pictimeout, getconfig("tmcpictimeout", NULL));

	addchoicebox(picfull, "0", _("no"));
	addchoicebox(picfull, "1", _("yes"));
	setchoiceboxselection(picfull, getconfig("tmcpicfull", NULL));

	addchoicebox(picname, "0", _("no"));
	addchoicebox(picname, "1", _("yes"));
	setchoiceboxselection(picname, getconfig("tmcpicname", NULL));

	addchoicebox(scan, "0", _("all"));
	addchoicebox(scan, "1", _("video"));
	addchoicebox(scan, "2", _("audio"));
	addchoicebox(scan, "3", _("picture"));
	addchoicebox(scan, "91", _("video / audio"));
	addchoicebox(scan, "92", _("video / picture"));
	addchoicebox(scan, "93", _("audio / picture"));
	setchoiceboxselection(scan, getconfig("tmcscan", NULL));

	addchoicebox(bgpic, "0", _("pic1"));
	addchoicebox(bgpic, "1", _("pic2"));
	addchoicebox(bgpic, "2", _("pic3"));
	addchoicebox(bgpic, "3", _("pic4"));
	addchoicebox(bgpic, "4", _("pic5"));
	addchoicebox(bgpic, "1000", _("no pic"));
	setchoiceboxselection(bgpic, getconfig("tmcbgpic", NULL));
  
  addchoicebox(preview, "0", _("slow"));
	addchoicebox(preview, "1", _("normal"));
  addchoicebox(preview, "2", _("high"));
	setchoiceboxselection(preview, getconfig("tmcpreview", NULL));

	addscreenrc(tmcpic3, listbox);
	drawscreen(tmcpic3, 0, 0);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tmcpic3, tmp);
		rcret = waitrc(tmcpic3, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			addconfigscreencheck("tmcpictimeout", pictimeout, "5");
			addconfigscreencheck("tmcpicname", picname, "0");
			addconfigscreencheck("tmcpicfull", picfull, "0");
			addconfigscreencheck("tmcscan", scan, "0");
			if(ostrcmp(bgpic->ret, getconfig("tmcbgpic", NULL)) != 0) ret = 1;
			addconfigscreencheck("tmcbgpic", bgpic, "0");
      addconfigscreencheck("tmcpreview", preview, "0");
			break;
		}
	}

	pictimeout->hidden = YES;
	picfull->hidden = YES;
	picname->hidden = YES;
	scan->hidden = YES;
	bgpic->hidden = YES;
  preview->hidden = YES;

	delownerrc(tmcpic3);

	changepic(tmcpic3, tmppic);
	tmcpic3->bgcol = -1;
	free(tmppic); tmppic = NULL;
	drawscreen(tmcpic3, 0, 0);

	return ret;
}

char* tmcscreenscandir()
{
	struct skin* dir = getscreen("dir");
	char* ret = NULL, *bg = NULL;

	dir->width = 700;
	dir->prozwidth = 0;
	dir->height = 650;
	dir->prozheight = 0;

	drawscreen(dir, 2, 0);
	bg = savescreen(dir);

	ret = screendir(getconfig("rec_moviepath", NULL), "", NULL, NULL, NULL, NULL, 0, "SELECT", getrcconfigint("rcgreen", NULL), NULL, 0, NULL, 0, dir->width, dir->prozwidth, dir->height, dir->prozheight, 0);

	restorescreen(bg, dir);
	blitfb(0);
	return ret;
}

void screentmcepg(char* file)
{
	int rcret = -1;
	char* bg = NULL;
	struct skin* tmcepg = getscreen("tmcepg");

	if(file != NULL)
	{
		drawscreen(tmcepg, 2, 0);
		bg = savescreen(tmcepg);

		readlabelext(tmcepg, file, ".epg");
		drawscreen(tmcepg, 0, 0);

		while(1)
		{
			rcret = waitrc(tmcepg, 0, 0);

			if(rcret == getrcconfigint("rcexit", NULL)) break;
			if(rcret == getrcconfigint("rcok", NULL)) break;
		}

		clearscreen(tmcepg);
		restorescreen(bg, tmcepg);
	}
}

//flag 0: editmode
//flag 1: showmode
struct mainplaylist* screentmcplaylist(int flag)
{
//TODO:
	struct mainplaylist* mplaylist = NULL;
	struct skin* playlist = getscreen("playlist");
	char* bg = NULL;

	drawscreen(playlist, 2, 0);
	bg = savescreen(playlist);

	mplaylist = screenmainplaylist(flag);

	restorescreen(bg, playlist);
	blitfb(0);

	return mplaylist;
}

int screentmcdelete(char* file)
{
	int ret = 0, tret = 0, width = 800, height = 300;
	struct skin* tbox = getscreen("messagebox");
	char* tmpstr = NULL, *bg = NULL, *tmpfile = NULL;
	struct mediadb* node = NULL;

	if(file != NULL)
	{
		char* dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		node = getmediadb(dname, basename(file), 0);
		free(dname); dname = NULL;
	}

	if(node != NULL)
	{
		tbox->width = width;
		tbox->height = height;

		drawscreen(tbox, 2, 0);
		bg = savescreen(tbox);

		tmpfile = ostrcat(node->path, "/", 0, 0);
		tmpfile = ostrcat(tmpfile, node->file, 1, 0);
		tmpfile = addmountpart(tmpfile, 1);

		if(tmpfile != NULL)
		{
			tmpstr = ostrcat(_("Delete selected entry ?"), "\n\n", 0, 0);
			tmpstr = ostrcat(tmpstr, tmpfile, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n\nOK = delete DB only\nRED = delete DB and file", 1, 0);
			tret = textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("RED"), getrcconfigint("rcred", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, width, height, 0, 3);
			free(tmpstr); tmpstr = NULL;

			if(tret == 1) //del only DB
			{
				struct mediadbfilter* mf = mediadbfilterpos;
				if(mediadbfilterpos != NULL) mediadbfilterpos = mediadbfilterpos->next;
				delmediadbfilter(mf, 0);
				delmediadb(node, 0);
				status.writemediadb = 1;
				ret = 1;
			}
			if(tret == 2) //del file and DB
			{
				struct mediadbfilter* mf = mediadbfilterpos;
				if(mediadbfilterpos != NULL) mediadbfilterpos = mediadbfilterpos->next;
				unlink(tmpfile);
				delmediadbfilter(mf, 0);
				delmediadb(node, 0);
				status.writemediadb = 1;
				ret = 1;
			}
		}

		free(tmpfile); tmpfile = NULL;
		restorescreen(bg, tbox);
	}

	return ret;
}

void screentmcimdbsearch(char* file, int menuid)
{
	int type = 0;
	struct tmdb* tmdb = NULL;
	struct mediadb* node = NULL;
	struct skin* tmdbplugin = NULL;
	struct skin* tmcinfo = getscreen("tmcinfo");
	char* bg = NULL;

	if(file != NULL)
	{
		char* dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		node = getmediadb(dname, basename(file), 0);
		free(dname); dname = NULL;
	}

	if(node == NULL) return;

	if(menuid == 3) type = 0; //video
	if(menuid == 4) type = 1; //audio
	if(menuid == 2) type = 2; //picture

	//create imdb search name
	char* shortname = ostrcat(file, NULL, 0, 0);
	string_tolower(shortname);
//	shortname = string_shortname(shortname, 1);
	shortname = string_shortname(shortname, 2);
	string_removechar(shortname);
	strstrip(shortname);
	
	char* fileinfo = ostrcat(getfilenameext(file), NULL , 0, 1);			

	tmdbplugin = getplugin("TMDb");
	if(tmdbplugin != NULL)
	{
		struct tmdb* (*startplugin)(char*, char*, int, char*, char*);
		startplugin = dlsym(tmdbplugin->pluginhandle, "screentmdb");
		if(startplugin != NULL)
		{
			drawscreen(tmcinfo, 2, 0);
			bg = savescreen(tmcinfo);

			tmdb = startplugin(shortname, "tmcinfo", 1, NULL, NULL);

			if(tmdb != NULL)
			{
				node->flag = setbit(node->flag, 31);
				node = createmediadb(node, tmdb->id, type, tmdb->title, tmdb->year, tmdb->released, tmdb->runtime, tmdb->genre, NULL, NULL, NULL, tmdb->plot, tmdb->mvi, tmdb->rating, tmdb->votes, node->path, node->file, shortname, fileinfo, node->flag, node->backdropcount);
			}

			clearscreen(tmcinfo);
			restorescreen(bg, tmcinfo);
			blitfb(0);
		}
	}

	free(shortname), shortname = NULL;
	free(fileinfo), fileinfo = NULL;
}

void screentmcinfo(char* file, int menuid)
{
	int rcret = 0;
	struct skin* tmcinfo = getscreen("tmcinfo");
	struct skin* title = getscreennode(tmcinfo, "title");
	struct skin* released = getscreennode(tmcinfo, "released");
	struct skin* runtime = getscreennode(tmcinfo, "rating");
	struct skin* genre = getscreennode(tmcinfo, "genre");
	struct skin* director = getscreennode(tmcinfo, "orgname");
	struct skin* plot = getscreennode(tmcinfo, "plot");
	struct skin* votes = getscreennode(tmcinfo, "votes");
	struct skin* cover = getscreennode(tmcinfo, "cover");
	char* tmpstr = NULL, *bg = NULL;
	struct mediadb* node = NULL;
	
	if(file != NULL)
	{
		char* dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		node = getmediadb(dname, basename(file), 0);
		free(dname); dname = NULL;
	}

	if(node != NULL)
	{
		changetext(title, node->title);
		changetext(released, node->released);
		changetext(runtime, node->runtime);
		changetext(genre, node->genre);
		changetext(director, node->director);

		tmpstr = oitoa(node->votes);
		changeinput(votes, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(plot, node->plot);

		tmpstr = ostrcat(getconfig("mediadb", NULL), "/", 0, 0);
		tmpstr = ostrcat(node->poster, "_cover.jpg", 0, 0);
		changepic(cover, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	drawscreen(tmcinfo, 2, 0);
	bg = savescreen(tmcinfo);
	
	drawscreen(tmcinfo, 0, 0);

	while(1)
	{
		rcret = waitrc(tmcinfo, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	clearscreen(tmcinfo);
	restorescreen(bg, tmcinfo);
	blitfb(0);
}

void screentmcdbmenu(char* file, int menuid)
{
	int rcret = 0;
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* listbox = getscreennode(tmcpic3, "listbox");
	struct skin* edit = getscreennode(tmcpic3, "edit");
	struct skin* delete = getscreennode(tmcpic3, "delete");
	struct skin* imdbsearch = getscreennode(tmcpic3, "imdbsearch");
	struct skin* info = getscreennode(tmcpic3, "imdbsearch");
	char* tmppic = NULL;

	tmppic = ostrcat(tmcpic3->pic, NULL, 0, 0);
	changepic(tmcpic3, NULL);
	tmcpic3->bgcol = 0;

	edit->hidden = NO;
	delete->hidden = NO;
	if(menuid == 3)
		imdbsearch->hidden = NO;
	info->hidden = NO;

	addscreenrc(tmcpic3, listbox);
	drawscreen(tmcpic3, 0, 0);

	while(1)
	{
		rcret = waitrc(tmcpic3, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			if(ostrcmp(listbox->select->name, "edit") == 0)
				screenmediadbedit(file, menuid, 1);
			if(ostrcmp(listbox->select->name, "delete") == 0)
			{
				if(screentmcdelete(file) == 1)
					break;
			}
			if(menuid == 3 && ostrcmp(listbox->select->name, "imdbsearch") == 0)
				screentmcimdbsearch(file, menuid);
			if(ostrcmp(listbox->select->name, "info") == 0)
				screentmcinfo(file, menuid);

			drawscreen(tmcpic3, 0, 0);
		}
	}

	edit->hidden = YES;
	delete->hidden = YES;
	imdbsearch->hidden = YES;
	info->hidden = YES;

	delownerrc(tmcpic3);

	changepic(tmcpic3, tmppic);
	tmcpic3->bgcol = -1;
	free(tmppic); tmppic = NULL;
	drawscreen(tmcpic3, 0, 0);
}

int screentmcpicplay(char* picture)
{
	int rcret = 0, timeout = 0, ret = 0;
	struct skin* picplay = getscreen("tmcpicplay");
	struct skin* picname = getscreennode(picplay, "picname");
	struct skin* pic = getscreennode(picplay, "pic");

	timeout = getconfigint("tmcpictimeout", NULL) * 1000;
	if(timeout == 0) timeout = 5000;

	if(getconfigint("tmcpicname", NULL) == 1)
		changetext(picname, picture);
	else
		changetext(picname, NULL);

	if(getconfigint("tmcpicfull", NULL) == 1)
	{
		pic->picheight = 720;
		pic->picwidth = 1280;
	}
	else
	{
		pic->picheight = 1;
		pic->picwidth = 1;
	}

	changepic(pic, picture);

	drawscreen(picplay, 0, 0);

	while(1)
	{
		rcret = waitrc(picplay, timeout, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			ret = 1;
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == RCTIMEOUT) break;
	}

	return ret;
}

void tmcsetbutton(int picplaytype, int audioplaytype)
{
	menu2[1] = "skin/tmcrandom.png";
	menu2[3] = "skin/tmcone.png";
	if(picplaytype == 1) menu2[3] = "skin/tmcone_act.png";
	if(picplaytype == 2) menu2[1] = "skin/tmcrandom_act.png";
	
	menu4[1] = "skin/tmcrandom.png";
	menu4[3] = "skin/tmcone.png";
	if(audioplaytype == 1) menu4[3] = "skin/tmcone_act.png";
	if(audioplaytype == 2) menu4[1] = "skin/tmcrandom_act.png";
}

void screentmcmenu()
{
	int rcret = 0, menuid = 0, active = 1;
	int audioplaytype = 0, picplaytype = 0, playlistplaytype = 0;
	struct skin* tmcbg = getscreen("tmcbg");
	struct skin* load = getscreen("loading");
	struct skin* tmcmenutxt = getscreen("tmcmenutxt");
	struct skin* tmcmenu1 = getscreen("tmcmenu1");
	struct skin* tmcmenu2 = getscreen("tmcmenu2");
	struct skin* tmcmenu3 = getscreen("tmcmenu3");
	struct skin* tmcmenu3p2 = getscreennode(tmcmenu3, "tmcmenu3p2");
	struct skin* tmcmenu4 = getscreen("tmcmenu4");
	struct skin* tmcmenu5 = getscreen("tmcmenu5");
	struct skin* tmcpictitle = getscreen("tmcpictitle");
	struct skin* tmcpicstar = getscreen("tmcpicstar");
	struct skin* tmcstatus = getscreen("tmcstatus");
	struct skin* tmcpic1 = getscreen("tmcpic1");
	struct skin* tmcpic2 = getscreen("tmcpic2");
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* tmcpic4 = getscreen("tmcpic4");
	struct skin* tmcpic5 = getscreen("tmcpic5");
	char* tmpstr = NULL;
	char* tmcpictitlebg = NULL, *tmcpicstarbg = NULL, *tmcstatusbg = NULL;

	setfbtransparent(255);

	//checks
	if(!file_exist(getconfig("mediadbpath", NULL)))
	{
		tmpstr = ostrcat(_("MediaDB Path not found!\nConfigure it in MediaDB Settings"), "\n\n", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Path: "), 1, 0);
		tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
	else
	{
		if(!file_exist(getconfig("mediadbfile", NULL)))
			textbox(_("Message"), _("MediaDB not found!\nScan your HDD in TMC Settings"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}

	//TODO: TMDB and so on
 if(getplugin("IMDb-API") == NULL && getplugin("IMDb") == NULL && getplugin("TMDb") == NULL)
		textbox(_("Message"), _("IMDB Plugin not found!\nCan't get detail media infos\nPlease install it"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	drawscreen(load, 0, 0);
	readmediadb(getconfig("mediadbfile", NULL), 0, 0);
	mediadbfilterpos = NULL;
	mediadbfiltercount = 0;

	menu0pos = 0;
	menu1pos = 0;
	menu2pos = 0;
	menu3pos = 0;
	menu4pos = 0;

	tmcsetbutton(picplaytype, audioplaytype);
	tmcpicdel(tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);

	if(getconfigint("tmcbgpic", NULL) != 1000)
	{
		char* tmpbgpic = getconfig("tmcbgpic", NULL);
		if(tmpbgpic == NULL)
			tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcbg", "0", 0, 0);
		else
			tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcbg", tmpbgpic, 0, 0);
		tmpstr = ostrcat(tmpstr, ".jpg", 1, 0);
		changepic(tmcbg, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}
	else
		changepic(tmcbg, NULL);

	drawscreen(tmcbg, 0, 1);
	drawscreen(tmcpic1, 0, 1);
	drawscreen(tmcpic2, 0, 1);
	drawscreen(tmcpic3, 0, 1);
	drawscreen(tmcpic4, 0, 1);
	drawscreen(tmcpic5, 0, 1);
	tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
	
	//save backgrounds
	drawscreen(tmcpictitle, 2, 0);
	drawscreen(tmcpicstar, 2, 0);
	drawscreen(tmcstatus, 2, 0);

	tmcpictitlebg = savescreen(tmcpictitle);
	tmcpicstarbg = savescreen(tmcpicstar);
	tmcstatusbg = savescreen(tmcstatus);

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(menuid > 1 && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
		{
			if(active == 1)
			{
				active = 0;
				tmcpic3->bordersize += 10;
			}
			else
			{
				active = 1;
				tmcpic3->bordersize -= 10;
			}
			drawscreen(tmcpic3, 0, 0);

			tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(active == 1)
			{
				if(menuid == 0) break;
				menuid = 0;
				tmcpicdel(tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 1);

				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				continue;
			}
		}

		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 1);
			else
				tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 1);
			continue;
		}

		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 2);
			else
				tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 2);
			continue;
		}

		if(rcret == getrcconfigint("rcmenu", NULL) && active == 0)
		{
			screentmcdbmenu(tmcpic3->ret, menuid);
			tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
		}

		if(rcret == getrcconfigint("rcinfo", NULL) && active == 0)
			screentmcinfo(tmcpic3->ret, menuid);

		if(rcret == getrcconfigint("rcepg", NULL) && active == 1 && menuid == 3) //only video
			screentmcepg(tmcpic3->ret);

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(active == 1)
			{
				if(ostrcmp("Exit", tmcmenutxt->ret) == 0)
					break;
				else if(ostrcmp("Back", tmcmenutxt->ret) == 0)
				{
					menuid = 0;
					tmcpicdel(tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 1);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(ostrcmp("Settings", tmcmenutxt->ret) == 0) //mainmenu settings
				{
					menuid = 1;
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 1 && ostrcmp("Scan All", tmcmenutxt->ret) == 0)
				{
					int scantype = getconfigint("tmcscan", NULL);

					if(scantype == 0)
						scantype = 100;
					else
						scantype--;

					mediadbscan(NULL, scantype, 0);
					screentmcdb();
				}
				else if(menuid == 1 && ostrcmp("Scan Dir", tmcmenutxt->ret) == 0)
				{
					char* scanpath = tmcscreenscandir();

					if(scanpath != NULL)
					{
						int scantype = getconfigint("tmcscan", NULL);

						if(scantype == 0)
							scantype = 100;
						else
							scantype--;

						mediadbscan(scanpath, scantype, 0);
						free(scanpath); scanpath = NULL;
						screentmcdb();
					}
				}
				else if(menuid == 1 && ostrcmp("Database Info", tmcmenutxt->ret) == 0)
				{
					screentmcdb();
				}
				else if(menuid == 1 && ostrcmp("Playlist Edit", tmcmenutxt->ret) == 0)
				{
					screentmcplaylist(0);
				}
				else if(menuid == 1 && ostrcmp("Main", tmcmenutxt->ret) == 0)
				{
					if(screentmcsettings() == 1)
					{
						if(getconfigint("tmcbgpic", NULL) != 1000)
						{
							char* tmpbgpic = getconfig("tmcbgpic", NULL);
							if(tmpbgpic == NULL)
								tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcbg", "0", 0, 0);
							else
								tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcbg", tmpbgpic, 0, 0);
							tmpstr = ostrcat(tmpstr, ".jpg", 1, 0);
							changepic(tmcbg, tmpstr);
							free(tmpstr); tmpstr = NULL;
						}
						else
							changepic(tmcbg, NULL);

						drawscreen(tmcbg, 0, 1);
						free(tmcpictitlebg); tmcpictitlebg = savescreen(tmcpictitle);
						free(tmcpicstarbg); tmcpicstarbg = savescreen(tmcpicstar);
						free(tmcstatusbg); tmcstatusbg = savescreen(tmcstatus);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
						tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
					}
				}
				else if(ostrcmp("Picture", tmcmenutxt->ret) == 0) //mainmenu picture
				{
					menuid = 2;
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(2, NULL, 0);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 2 && ostrcmp("Category", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(2, ".category");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(2, tmpstr, 4);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 2 && ostrcmp("A-Z", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(2, ".az");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(2, tmpstr, 7);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 2 && ostrcmp("All", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(2, NULL, 0);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
				}
				else if(menuid == 2 && ostrcmp("Random", tmcmenutxt->ret) == 0)
				{
					if(picplaytype == 2)
						picplaytype = 0;
					else
						picplaytype = 2;
					
					tmcsetbutton(picplaytype, audioplaytype);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 2 && ostrcmp("Single", tmcmenutxt->ret) == 0)
				{
					if(picplaytype == 1)
						picplaytype = 0;
					else
						picplaytype = 1;
						
					tmcsetbutton(picplaytype, audioplaytype);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(ostrcmp("Video", tmcmenutxt->ret) == 0) //mainmenu video
				{
					menuid = 3;
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(0, NULL, 0);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 3 && ostrcmp("Genre", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".genre");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 6);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Category", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".category");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 4);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("A-Z", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".az");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 7);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("All", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(0, NULL, 0);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
				}
				else if(menuid == 3 && ostrcmp("Year", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".year");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 1);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Director", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".director");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 2);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Actors", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".actors");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 3);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Rating", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".rating");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(0, tmpstr, 5);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("No IMDB", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(0, tmpstr, 8);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
				}
				else if(ostrcmp("Audio", tmcmenutxt->ret) == 0) //mainmenu audio
				{
					menuid = 4;
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(1, NULL, 0);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 4 && ostrcmp("Category", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(1, ".category");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(1, tmpstr, 4);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 4 && ostrcmp("Genre", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(1, ".genre");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(1, tmpstr, 6);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
        else if(menuid == 4 && ostrcmp("Year", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(1, ".year");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(1, tmpstr, 1);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
        else if(menuid == 4 && ostrcmp("Actors", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(1, ".actors");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(1, tmpstr, 3);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 4 && ostrcmp("A-Z", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(1, ".az");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						mediadbfiltercount = createmediadbfilter(1, tmpstr, 7);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 4 && ostrcmp("All", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					mediadbfiltercount = createmediadbfilter(1, NULL, 0);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
				}
				else if(menuid == 4 && ostrcmp("Random", tmcmenutxt->ret) == 0)
				{
					if(audioplaytype == 2)
						audioplaytype = 0;
					else
						audioplaytype = 2;
						
					tmcsetbutton(picplaytype, audioplaytype);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 4 && ostrcmp("Single", tmcmenutxt->ret) == 0)
				{
					if(audioplaytype == 1)
						audioplaytype = 0;
					else
						audioplaytype = 1;
						
					tmcsetbutton(picplaytype, audioplaytype);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(ostrcmp("Playlist", tmcmenutxt->ret) == 0) //playlist
				{
					int playlistret = 0;
					struct mainplaylist* mplaylist = NULL;

					mplaylist = screentmcplaylist(1);
					if(mplaylist != NULL)
					{
						struct playlist* plist = mplaylist->playlist;
						drawscreen(skin, 0, 0);
						if(servicestop(status.aktservice, 1, 1) == 0)
						{
							while(plist != NULL)
							{
								//TODO: play playlist
								playlistret = screenplay(plist->file, NULL, 0, 0);
								if(playlistret == 1 || playlistret == 2) break;

								if(playlistplaytype == 2) //random
									plist = getplaylistrandom(mplaylist->playlist, getplaylistmax(mplaylist->playlist));
								else //next
									plist = plist->next;
							}

							playstartservice();
						}

						drawscreen(tmcbg, 0, 1);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
						tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
					}
				}
				else if(ostrcmp("Play", tmcmenutxt->ret) == 0) //directory play
				{
					char* file = screentmcdirplay();

					if(file != NULL)
					{
						drawscreen(skin, 0, 0);
						screenplay(file, NULL, 0, 0);

						free(file); file = NULL;
						drawscreen(tmcbg, 0, 1);
						tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
						tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
					}
				}

				continue;
			}
			
			if(active == 0 && tmcpic3->ret != NULL)
			{
				if(menuid == 3) //video
				{
					drawscreen(skin, 0, 0);

					screenplay(tmcpic3->ret, NULL, 0, 0);

					drawscreen(tmcbg, 0, 1);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				if(menuid == 4) //audio
				{
					int audioret = 0;
					struct mediadbfilter* mfilter = NULL;

					drawscreen(skin, 0, 0);
					mfilter = mediadbfilterpos;

					if(servicestop(status.aktservice, 1, 1) == 0)
					{
						while(mfilter != NULL && mediadbfilter != NULL)
						{
							if(mfilter->node != NULL)
							{
								char* tmpfile = ostrcat(mfilter->node->path, "/", 0, 0);
								tmpfile = ostrcat(tmpfile, mfilter->node->file, 1, 0);
								tmpfile = addmountpart(tmpfile, 1);
								audioret = screenplay(tmpfile, NULL, 0, 3);
								free(tmpfile); tmpfile = NULL;
								mediadbfilterpos = mfilter;
							}
							if(audioret == 1 || audioret == 2) break;

							if(audioplaytype == 1) //single
								break;
							else if(audioplaytype == 2) //random
								mfilter = getmediadbfilterrandom(getmediadbfiltercount());
							else //next
								mfilter = mfilter->next;

							if(mfilter == NULL) mfilter = mediadbfilter;
						}

						playstartservice();
					}

					drawscreen(tmcbg, 0, 1);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 2) //picture
				{
					int picret = 0;
					struct mediadbfilter* mfilter = NULL;

					drawscreen(skin, 0, 0);
					mfilter = mediadbfilterpos;
					
					while(mfilter != NULL && mediadbfilter != NULL)
					{
						if(mfilter->node != NULL)
						{
							char* tmpfile = ostrcat(mfilter->node->path, "/", 0, 0);
							tmpfile = ostrcat(tmpfile, mfilter->node->file, 1, 0);
							tmpfile = addmountpart(tmpfile, 1);
							picret = screentmcpicplay(tmpfile);
							free(tmpfile); tmpfile = NULL;
							mediadbfilterpos = mfilter;
						}
						if(picret == 1) break;

						if(picplaytype == 1) //single
							break;
						else if(picplaytype == 2) //random
							mfilter = getmediadbfilterrandom(getmediadbfiltercount());
						else //next
							mfilter = mfilter->next;

						if(mfilter == NULL) mfilter = mediadbfilter;
					}

					drawscreen(tmcbg, 0, 1);
					tmcpicscroll(menuid, tmcpictitle, tmcpicstar, tmcstatus, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, tmcstatusbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu3p2, tmcmenu4, tmcmenu5, 0);
				}
			}
		}
	}

	free(tmcpictitlebg); tmcpictitlebg = NULL;
	free(tmcpicstarbg); tmcpicstarbg = NULL;
	free(tmcstatusbg); tmcstatusbg = NULL;
	freemediadbfilter(0);
	if(status.mediadbthread == NULL)
	{
		if(status.writemediadb == 1)
			writemediadb(getconfig("mediadbfile", NULL), NULL);
		freemediadb(0);
	}
	clearscreen(tmcbg);
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
}

#endif
