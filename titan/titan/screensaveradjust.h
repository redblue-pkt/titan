#ifndef SCREENSAVERADJUST_H
#define SCREENSAVERADJUST_H

void screenscreensaveradjust()
{
	int rcret = 0;
	struct skin* screensaveradjust = getscreen("screensaveradjust");
	struct skin* listbox = getscreennode(screensaveradjust, "listbox");
	struct skin* screensaver = getscreennode(screensaveradjust, "screensaver");
	struct skin* screensaver_delay = getscreennode(screensaveradjust, "screensaver_delay");
	struct skin* screensaver_interval = getscreennode(screensaveradjust, "screensaver_interval");
	struct skin* screensaver_type = getscreennode(screensaveradjust, "screensaver_type");
	struct skin* screensaver_background_color = getscreennode(screensaveradjust, "screensaver_background_color");
	struct skin* screensaver_pic = getscreennode(screensaveradjust, "screensaver_pic");
	struct skin* filelist = getscreennode(screensaveradjust, "filelist");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addchoicebox(screensaver, "0", _("no"));	
	addchoicebox(screensaver, "1", _("yes"));
	setchoiceboxselection(screensaver, getconfig("screensaver", NULL));

	changeinput(screensaver_delay, "5\n6\n7\n8\n9\n10\n15\n20\n30\40\n50\60");
	setchoiceboxselection(screensaver_delay, getconfig("screensaver_delay", NULL));

	changeinput(screensaver_interval, "2\n5\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(screensaver_interval, getconfig("screensaver_interval", NULL));

	addchoicebox(screensaver_background_color, "000000", _("black"));
	addchoicebox(screensaver_background_color, "0000ff", _("blue"));
	addchoicebox(screensaver_background_color, "00ff00", _("red"));
	addchoicebox(screensaver_background_color, "ff0000", _("orange"));
	addchoicebox(screensaver_background_color, "ffff00", _("yellow"));
	addchoicebox(screensaver_background_color, "ffffff", _("white"));
	addchoicebox(screensaver_background_color, "BEBEBE", _("grey"));
	addchoicebox(screensaver_background_color, "FFD39B", _("burlywood1"));
	addchoicebox(screensaver_background_color, "020202", _("lila"));
	addchoicebox(screensaver_background_color, "mixed", _("mixed mode"));
	setchoiceboxselection(screensaver_background_color, getconfig("screensaver_background_color", NULL));

	addchoicebox(screensaver_pic, "skin/screensaver1.png", _("Icon 1"));
	addchoicebox(screensaver_pic, "skin/screensaver2.png", _("Icon 2"));
	addchoicebox(screensaver_pic, "skin/screensaver3.png", _("Icon 3"));
	addchoicebox(screensaver_pic, "skin/screensaver4.png", _("Icon 4"));
	setchoiceboxselection(screensaver_pic, getconfig("screensaver_pic", NULL));

	addchoicebox(screensaver_type, "0000000000", _("Song TitleText"));
	addchoicebox(screensaver_type, "1111111111", _("Logo 1"));
	addchoicebox(screensaver_type, "2222222222", _("Logo 2"));

	char* imdb_directory = getconfig("mediadbpath", NULL);
	if(imdb_directory != NULL)
	{
		char* imdbdir = NULL;
		imdbdir = ostrcat(imdb_directory, "/", 0, 0);
	
		if(file_exist(imdbdir))
			addchoicebox(screensaver_type, imdbdir, _("imdb mvis (network/hdd)"));
		free(imdbdir); imdbdir = NULL;
	}
	
	//TODO: use config instread fix path
	if(file_exist("/var/swap/saver/"))
	{
		char* path = NULL;
		delmarkedscreennodes(screensaveradjust, FILELISTDELMARK);
		changeinput(filelist, "/var/swap/saver");
		createfilelist(screensaveradjust, filelist, 0);
		
		node = filelist;
		while(node != NULL)
		{
			if(node->del == FILELISTDELMARK && node->text != NULL && ostrcmp(node->text, "..") != 0)
			{
				path = ostrcat("/var/swap/saver/" , node->text, 0, 0);
				addchoicebox(screensaver_type, path, node->text);
				free(path); path = NULL;
			}
			node = node->next;
		}
	}
	setchoiceboxselection(screensaver_type, getconfig("screensaver_type", NULL));

	drawscreen(screensaveradjust, 0, 0);
	addscreenrc(screensaveradjust, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(screensaveradjust, tmp);
		rcret = waitrc(screensaveradjust, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("screensaver", screensaver, NULL);
			addconfigscreencheck("screensaver_delay", screensaver_delay, NULL);
			addconfigscreencheck("screensaver_interval", screensaver_interval, NULL);
			addconfigscreencheck("screensaver_type", screensaver_type, NULL);
			addconfigscreencheck("screensaver_background_color", screensaver_background_color, NULL);
			addconfigscreencheck("screensaver_pic", screensaver_pic, NULL);
			break;
		}
	}
	delownerrc(screensaveradjust);
	clearscreen(screensaveradjust);
	writeallconfig(1);
}

#endif
