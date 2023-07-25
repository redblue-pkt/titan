#ifndef XUPNPD_H
#define XUPNPD_H

void screenxupnpd()
{
	int rcret = -1, ret = 0;
	struct skin* xupnpd = getscreen("xupnpdsettings");
	struct skin* listbox = getscreennode(xupnpd, "listbox");
	char* tmpstr = NULL, *xupnpdstop = NULL, *xupnpdstart = NULL;
	struct skin* tmp = NULL;
	struct skin* node = NULL;

	tmpstr = createpluginpath("/xupnpd/xupnpd.sh", 0);
	xupnpdstop = ostrcat(tmpstr, " stop ", 0, 0);
	xupnpdstart = ostrcat(tmpstr, " start ", 0, 0);
	printf("tmpstr1: %s\n", tmpstr);
	tmpstr = string_replace("/usr/local/share/titan/plugins/network/xupnpd/xupnpd.sh", "/etc/xupnpd", tmpstr, 1);
	printf("tmpstr2: %s\n", tmpstr);
	xupnpdstop = ostrcat(xupnpdstop, tmpstr, 1, 0);
	xupnpdstart = ostrcat(xupnpdstart, tmpstr, 1, 0);

	addscreenrc(xupnpd, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	node = addlistbox(xupnpd, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("autostart")); changename(node, "xupnpd");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	drawscreen(xupnpd, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(xupnpd, tmp);
		rcret = waitrc(xupnpd, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			debug(10, "cmd: %s", xupnpdstop);
			system(xupnpdstop);
			debug(10, "cmd: %s", xupnpdstart);
			ret = system(xupnpdstart);
			if(ret == 0)
				textbox(_("Message"), _("xupnpd started and config saved"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("xupnpd not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(xupnpd, 0, 0);			
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			debug(10, "cmd: %s", xupnpdstop);
			system(xupnpdstop);
			textbox(_("Message"), _("xupnpd now stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			drawscreen(xupnpd, 0, 0);
		}

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			debug(10, "cmd: %s", xupnpdstop);
			system(xupnpdstop);
			debug(10, "cmd: %s", xupnpdstart);
			ret = system(xupnpdstart);
			if(ret == 0)
				textbox(_("Message"), _("xupnpd started."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("xupnpd not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(xupnpd, 0, 0);
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(xupnpd, 1);
	delownerrc(xupnpd);
	clearscreen(xupnpd);
}

#endif
