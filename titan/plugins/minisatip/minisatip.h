#ifndef MINISATIP_H
#define MINISATIP_H

void screenminisatip()
{
	int rcret = -1, ret = 0;
	struct skin* minisatip = getscreen("minisatipsettings");
	struct skin* listbox = getscreennode(minisatip, "listbox");
	char* tmpstr = NULL, *minisatipstop = NULL, *minisatipstart = NULL;
	struct skin* tmp = NULL;
	struct skin* node = NULL;

	minisatipstop = ostrcat("/etc/init.d/minisatip stop", NULL, 0, 0);
	minisatipstart = ostrcat("/etc/init.d/minisatip start", NULL, 0, 0);
	
	addscreenrc(minisatip, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	node = addlistbox(minisatip, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("autostart")); changename(node, "minisatip");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	drawscreen(minisatip, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(minisatip, tmp);
		rcret = waitrc(minisatip, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			debug(10, "cmd: %s", minisatipstop);
			system(minisatipstop);
			debug(10, "cmd: %s", minisatipstart);
			ret = system(minisatipstart);
			if(ret == 0)
				textbox(_("Message"), _("minisatip started and config saved"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("minisatip not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(minisatip, 0, 0);			
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			debug(10, "cmd: %s", minisatipstop);
			system(minisatipstop);
			textbox(_("Message"), _("minisatip now stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			drawscreen(minisatip, 0, 0);
		}

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			debug(10, "cmd: %s", minisatipstop);
			system(minisatipstop);
			debug(10, "cmd: %s", minisatipstart);
			ret = system(minisatipstart);
			if(ret == 0)
				textbox(_("Message"), _("minisatip started."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("minisatip not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(minisatip, 0, 0);
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(minisatip, 1);
	delownerrc(minisatip);
	clearscreen(minisatip);
}

#endif
