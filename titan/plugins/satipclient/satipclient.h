#ifndef SATIPCLIENT_H
#define SATIPCLIENT_H

void screensatipclient()
{
	int rcret = -1, ret = 0;
	struct skin* satipclient = getscreen("satipclientsettings");
	struct skin* listbox = getscreennode(satipclient, "listbox");
	char* tmpstr = NULL, *satipclientstop = NULL, *satipclientstart = NULL, *satipclientscan = NULL;
	struct skin* tmp = NULL;
	struct skin* node = NULL;

	satipclientstop = ostrcat("/etc/init.d/satipclient stop", NULL, 0, 0);
	satipclientstart = ostrcat("/etc/init.d/satipclient start", NULL, 0, 0);
	satipclientscan = ostrcat(createpluginpath("/satipclient/satipclient.sh", 0), NULL, 0, 0);

	addscreenrc(satipclient, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	node = addlistbox(satipclient, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("autostart")); changename(node, "satipclient");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	drawscreen(satipclient, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(satipclient, tmp);
		rcret = waitrc(satipclient, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			debug(10, "cmd: %s", satipclientstop);
			system(satipclientstop);
			debug(10, "cmd: %s", satipclientstart);
			ret = system(satipclientstart);
			if(ret == 0)
				textbox(_("Message"), _("SAT-IP Client started and config saved"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("SAT-IP Client not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(satipclient, 0, 0);			
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			debug(10, "cmd: %s", satipclientstop);
			system(satipclientstop);
			textbox(_("Message"), _("SAT-IP Client now stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			drawscreen(satipclient, 0, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			debug(10, "cmd: %s", satipclientstop);
			system(satipclientstop);
			debug(10, "cmd: %s", satipclientstart);
			ret = system(satipclientstart);
			if(ret == 0)
				textbox(_("Message"), _("SAT-IP Client started."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("SAT-IP Client not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(satipclient, 0, 0);
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			debug(10, "cmd: %s", satipclientscan);
			tmpstr = command(satipclientscan);
			debug(10, "tmpstr: %s", tmpstr);
			textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 500, 0, 0);
			free(tmpstr), tmpstr = NULL;
			drawscreen(satipclient, 0, 0);
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			screentunerconfig();
			writeallconfig(1);
			drawscreen(satipclient, 0, 0);
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(satipclient, 1);
	delownerrc(satipclient);
	clearscreen(satipclient);
}

#endif
