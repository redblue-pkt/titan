#ifndef TITHEK_SETTINGS_H
#define TITHEK_SETTINGS_H

void screentithek_settings()
{
	int rcret = 0;
	struct skin* tmp = NULL;

	struct skin* tithek_settings = getscreen("tithek_settings");
	struct skin* listbox = getscreennode(tithek_settings, "listbox");
	struct skin* view = getscreennode(tithek_settings, "view");
	struct skin* cover = getscreennode(tithek_settings, "cover");
	struct skin* pay = getscreennode(tithek_settings, "pay");
	struct skin* picratio = getscreennode(tithek_settings, "picratio");
	struct skin* hidxxx = getscreennode(tithek_settings, "hidxxx");

	addchoicebox(cover, "0", _("show auto entrys"));
	addchoicebox(cover, "1", _("show 2 entrys"));
	addchoicebox(cover, "2", _("show 6 entrys"));
	addchoicebox(cover, "3", _("show 12 entrys"));	
	addchoicebox(cover, "4", _("show 20 entrys"));
	addchoicebox(cover, "5", _("show 30 entrys"));
	addchoicebox(cover, "6", _("show in list"));
	setchoiceboxselection(cover, getconfig("tithek_cover", NULL));

	addchoicebox(view, "0", _("show auto entrys"));
	addchoicebox(view, "1", _("show 2 entrys"));
	addchoicebox(view, "2", _("show 6 entrys"));
	addchoicebox(view, "3", _("show 12 entrys"));	
	addchoicebox(view, "4", _("show 20 entrys"));
	addchoicebox(view, "5", _("show 30 entrys"));
	addchoicebox(view, "6", _("show in list"));
	setchoiceboxselection(view, getconfig("tithek_view", NULL));

	addchoicebox(picratio, "0", _("no"));
	addchoicebox(picratio, "1", _("yes"));	
	setchoiceboxselection(picratio, getconfig("tithek_pic_ratio", NULL));

	addchoicebox(pay, "0", _("no"));
	addchoicebox(pay, "1", _("yes"));	
	setchoiceboxselection(pay, getconfig("tithek_pay", NULL));

	addchoicebox(hidxxx, "0", _("no"));
	addchoicebox(hidxxx, "1", _("yes"));	
	setchoiceboxselection(hidxxx, getconfig("tithek_hid_xxx", NULL));

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
			addconfigscreencheck("tithek_cover", cover, NULL);
			addconfigscreencheck("tithek_pay", pay, NULL);
			addconfigscreencheck("tithek_pic_ratio", picratio, NULL);
			addconfigscreencheck("tithek_hid_xxx", hidxxx, NULL);
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
