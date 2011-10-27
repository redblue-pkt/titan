#ifndef MC_PICTUREPLAYER_SETTINGS_H
#define MC_PICTUREPLAYER_SETTINGS_H

void screenmc_pictureplayer_settings()
{
	int rcret = 0;
	struct skin* tmp = NULL;

	struct skin* mc_pictureplayer_settings = getscreen("mc_pictureplayer_settings");
	struct skin* listbox = getscreennode(mc_pictureplayer_settings, "listbox");
	struct skin* dirsort = getscreennode(mc_pictureplayer_settings, "sort");
	struct skin* view = getscreennode(mc_pictureplayer_settings, "view");
	struct skin* sound = getscreennode(mc_pictureplayer_settings, "sound");
	struct skin* pp_interval = getscreennode(mc_pictureplayer_settings, "interval");

	addchoicebox(dirsort, "0", _("alpha"));	
	addchoicebox(dirsort, "1", _("reverse alpha"));	
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("dirsort", NULL));

	addchoicebox(view, "0", _("default"));	
	addchoicebox(view, "1", _("big"));	
	addchoicebox(view, "2", _("fullcover"));
	addchoicebox(view, "3", _("details"));
	addchoicebox(view, "4", _("smal"));
	addchoicebox(view, "5", _("fast"));
	setchoiceboxselection(view, getconfig("view", NULL));

	addchoicebox(sound, "http://stream.mth-house.de:8500/", _("MTH House"));
	addchoicebox(sound, "http://stream.laut.fm:80/radiofunclub", _("Radio Funclub"));
	addchoicebox(sound, "http://stream.laut.fm:80/radiofunclub80", _("Radio Funclub 80s"));	
	addchoicebox(sound, "http://www.antennestream.at:8002/", _("Antenne Kaernten"));	
	addchoicebox(sound, "http://webhost110.gtk.hu:9008/", _("Onkelz Rock Radio"));
	addchoicebox(sound, "http://c22033-l.i.core.cdn.streamfarm.net/22004mdrjump/live/3087mdr_jump/live_de_128.mp3", _("Jump"));
	addchoicebox(sound, "http://scfire-ntc-aa01.stream.aol.com:80/stream/1081", _("Raute Musik FM"));
	addchoicebox(sound, "off", _("off"));
	setchoiceboxselection(sound, getconfig("sound", NULL));

	changeinput(pp_interval, "10\n15\n20\n30\n40\n50\n60\n70\n80\n90\n100\n2\n5");
	setchoiceboxselection(pp_interval, getconfig("pp_interval", NULL));

	drawscreen(mc_pictureplayer_settings, 0);
	addscreenrc(mc_pictureplayer_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_pictureplayer_settings, tmp);
		rcret = waitrc(mc_pictureplayer_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreen("dirsort", dirsort);
			addconfigscreen("view", view);
			addconfigscreen("sound", sound);
			addconfigscreen("pp_interval", pp_interval);
			break;
		}
  }

	delownerrc(mc_pictureplayer_settings);
	clearscreen(mc_pictureplayer_settings);
}

#endif
