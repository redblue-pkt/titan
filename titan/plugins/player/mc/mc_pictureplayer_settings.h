#ifndef MC_PICTUREPLAYER_SETTINGS_H
#define MC_PICTUREPLAYER_SETTINGS_H

void screenmc_pictureplayer_settings()
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* tmp = NULL;

	struct skin* mc_pictureplayer_settings = getscreen("mc_pictureplayer_settings");
	struct skin* listbox = getscreennode(mc_pictureplayer_settings, "listbox");
	struct skin* dirsort = getscreennode(mc_pictureplayer_settings, "sort");
	struct skin* view = getscreennode(mc_pictureplayer_settings, "view");
	struct skin* sound = getscreennode(mc_pictureplayer_settings, "sound");
	struct skin* interval = getscreennode(mc_pictureplayer_settings, "interval");
	struct skin* showpictitle = getscreennode(mc_pictureplayer_settings, "showpictitle");
	struct skin* picdenom = getscreennode(mc_pictureplayer_settings, "picdenom");
	struct skin* picquality = getscreennode(mc_pictureplayer_settings, "picquality");
	struct skin* pichwdecode = getscreennode(mc_pictureplayer_settings, "pichwdecode");
	struct skin* defaultdir = getscreennode(mc_pictureplayer_settings, "defaultdir");
	struct skin* uselastdir = getscreennode(mc_pictureplayer_settings, "uselastdir");
	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	
	changeinput(picdenom, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16");
	setchoiceboxselection(picdenom, getskinconfig("mc_pp_picdenom", NULL));

//	changeinput(picquality, "0\n1\n2\n3");
	addchoicebox(picquality, "0", _("low"));
	addchoicebox(picquality, "1", _("middle"));
	addchoicebox(picquality, "2", _("best"));
	setchoiceboxselection(picquality, getconfig("mc_pp_picquality", NULL));

	addchoicebox(pichwdecode, "0", _("off"));	
	addchoicebox(pichwdecode, "1", _("on"));	
	setchoiceboxselection(pichwdecode, getconfig("mc_pp_pichwdecode", NULL));

	addchoicebox(showpictitle, "0", _("off"));	
	addchoicebox(showpictitle, "1", _("on"));	
	setchoiceboxselection(showpictitle, getconfig("mc_pp_showpictitle", NULL));
	
	addchoicebox(dirsort, "0", _("alpha"));	
	addchoicebox(dirsort, "1", _("reverse alpha"));	
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("mc_pp_dirsort", NULL));

	addchoicebox(view, "0", _("fast"));
	addchoicebox(view, "1", _("big"));	
	addchoicebox(view, "2", _("cover"));
//	addchoicebox(view, "3", _("fullcover"));		
	addchoicebox(view, "4", _("size (default)"));
	addchoicebox(view, "5", _("datetime"));

	setchoiceboxselection(view, getconfig("mc_pp_view", NULL));

	addchoicebox(sound, "http://mp3.89.0rtl.de:80/", _("Jump"));
	addchoicebox(sound, "http://stream.mth-house.de:8500/", _("MTH House"));
	addchoicebox(sound, "http://stream.laut.fm:80/radiofunclub", _("Radio Funclub"));
	addchoicebox(sound, "http://stream.laut.fm:80/radiofunclub80", _("Radio Funclub 80s"));	
	addchoicebox(sound, "http://www.antennestream.at:8002/", _("Antenne Kaernten"));	
	addchoicebox(sound, "http://webhost110.gtk.hu:9008/", _("Onkelz Rock Radio"));
	addchoicebox(sound, "http://c22033-l.i.core.cdn.streamfarm.net/22004mdrjump/live/3087mdr_jump/live_de_128.mp3", _("Jump"));
	addchoicebox(sound, "http://scfire-ntc-aa01.stream.aol.com:80/stream/1081", _("Raute Musik FM"));
	addchoicebox(sound, "off", _("off"));
	setchoiceboxselection(sound, getconfig("mc_pp_sound", NULL));

	changeinput(interval, "10\n15\n20\n30\n40\n50\n60\n70\n80\n90\n100\n2\n5");
	setchoiceboxselection(interval, getconfig("mc_pp_interval", NULL));

	changeinput(defaultdir, getconfig("mc_pp_defaultdir", NULL));

	addchoicebox(uselastdir, "0", _("no"));
	addchoicebox(uselastdir, "1", _("yes"));
	setchoiceboxselection(uselastdir, getconfig("mc_pp_uselastdir", NULL));

	drawscreen(mc_pictureplayer_settings, 0, 0);
	addscreenrc(mc_pictureplayer_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_pictureplayer_settings, tmp);
		rcret = waitrc(mc_pictureplayer_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("mc_pp_dirsort", dirsort, NULL);
			addconfigscreencheck("mc_pp_view", view, NULL);
			addconfigscreencheck("mc_pp_sound", sound, NULL);
			addconfigscreencheck("mc_pp_interval", interval, NULL);
			addconfigscreencheck("mc_pp_showpictitle", showpictitle, NULL);
			addconfigscreen("mc_pp_picdenom", picdenom);			
			addconfigscreencheck("mc_pp_picquality", picquality, NULL);
			addconfigscreencheck("mc_pp_pichwdecode", pichwdecode, NULL);
			addconfigscreen("mc_pp_defaultdir", defaultdir);
			addconfigscreencheck("mc_pp_uselastdir", uselastdir, NULL);
			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			struct skin* screen = getscreen("networkmenu");
			menu(screen);
			drawscreen(mc_pictureplayer_settings, 0, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "defaultdir") == 0)
			{
				clearscreen(mc_pictureplayer_settings);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 750, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(mc_pictureplayer_settings, 0, 0);
			}
		}
	}

	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);
	writeallconfig(0);
	delownerrc(mc_pictureplayer_settings);
	clearscreen(mc_pictureplayer_settings);
	resettvpic();
}

#endif
