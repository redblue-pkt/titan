#ifndef TITHEK_SETTINGS_H
#define TITHEK_SETTINGS_H

void screentithek_settings()
{
	int rcret = 0;
	struct skin* tmp = NULL;

	struct skin* tithek_settings = getscreen("tithek_settings");
	struct skin* listbox = getscreennode(tithek_settings, "listbox");
	struct skin* view = getscreennode(tithek_settings, "view");

	addchoicebox(view, "0", _("show 6 entrys"));
	addchoicebox(view, "1", _("show 12 entrys"));	
	setchoiceboxselection(view, getconfig("tithek_view", NULL));

	drawscreen(tithek_settings, 0, 0);
	addscreenrc(tithek_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tithek_settings, tmp);
		rcret = waitrc(tithek_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("tithek_view", view, NULL);
			break;
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			screenscreensaveradjust();
			drawscreen(tithek_settings, 0, 0);
		}	
  }

	delownerrc(tithek_settings);
	clearscreen(tithek_settings);
}

#endif
