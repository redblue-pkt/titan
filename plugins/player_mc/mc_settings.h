#ifndef MC_SETTINGS_H
#define MC_SETTINGS_H

void screenmc_settings()
{
	int rcret = 0;
	struct skin* mc_settings = getscreen("mc_settings");
	struct skin* listbox = getscreennode(mc_settings, "listbox");
	struct skin* style = getscreennode(mc_settings, "style");

	struct skin* tmp = NULL;

	addchoicebox(style, "0", _("transparent (mvi)"));	
	addchoicebox(style, "1", _("skin (picmem jpg)"));
	setchoiceboxselection(style, getconfig("style", NULL));

	drawscreen(mc_settings, 0, 0);
	addscreenrc(mc_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_settings, tmp);
		rcret = waitrc(mc_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("style", style, "0");
			break;
		}
  }

	delownerrc(mc_settings);
	clearscreen(mc_settings);
}

#endif
