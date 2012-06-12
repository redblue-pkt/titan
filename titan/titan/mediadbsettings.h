#ifndef MEDIADBSETTINGS_H
#define MEDIADBSETTINGS_H

void screenmediadbsettings()
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* mediadbsettings = getscreen("mediadbsettings");
	struct skin* listbox = getscreennode(mediadbsettings, "listbox");
	struct skin *mediadbpath = getscreennode(mediadbsettings, "mediadbpath");
	struct skin* mediadbscandelall = getscreennode(mediadbsettings, "mediadbscandelall");
	struct skin* mediadbscandelnotfound = getscreennode(mediadbsettings, "mediadbscandelnotfound");
	struct skin* mediadbscantimeout = getscreennode(mediadbsettings, "mediadbscantimeout");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	changeinput(mediadbpath, getconfig("mediadbpath", NULL));

	addchoicebox(mediadbscandelall, "0", _("no"));
	addchoicebox(mediadbscandelall, "1", _("yes"));
	setchoiceboxselection(mediadbscandelall, getconfig("mediadbscandelall", NULL));

	addchoicebox(mediadbscandelnotfound, "0", _("no"));
	addchoicebox(mediadbscandelnotfound, "1", _("yes"));
	setchoiceboxselection(mediadbscandelnotfound, getconfig("mediadbscandelnotfound", NULL));


	addchoicebox(mediadbscantimeout, "0", _("never"));
	addchoicebox(mediadbscantimeout, "1", _("older 1 day"));
	addchoicebox(mediadbscantimeout, "2", _("older 2 day"));
	addchoicebox(mediadbscantimeout, "3", _("older 3 day"));
	addchoicebox(mediadbscantimeout, "4", _("older 4 day"));
	addchoicebox(mediadbscantimeout, "5", _("older 5 day"));
	addchoicebox(mediadbscantimeout, "10", _("older 10 day"));
	addchoicebox(mediadbscantimeout, "15", _("older 15 day"));
	addchoicebox(mediadbscantimeout, "20", _("older 20 day"));
	addchoicebox(mediadbscantimeout, "30", _("older 30 day"));
	addchoicebox(mediadbscantimeout, "40", _("older 40 day"));
	addchoicebox(mediadbscantimeout, "50", _("older 50 day"));
	setchoiceboxselection(mediadbscantimeout, getconfig("mediadbscantimeout", NULL));

	drawscreen(mediadbsettings, 0, 0);
	addscreenrc(mediadbsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mediadbsettings, tmp);
		rcret = waitrc(mediadbsettings, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreen("mediadbpath", mediadbpath);
			tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/mediadb", 0, 0);
			addconfig("mediadbfile", tmpstr);
			free(tmpstr); tmpstr = NULL;
			addconfigscreencheck("mediadbscandelall", mediadbscandelall, "0");
			addconfigscreencheck("mediadbscandelnotfound", mediadbscandelnotfound, "0");
			addconfigscreencheck("mediadbscantimeout", mediadbscantimeout, "0");
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "mediadbpath") == 0)
			{
				clearscreen(mediadbsettings);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 700, 0, 650, 0, 0);
				if(ret != NULL)
				{
					if(ostrstr(ret, "/mediadb") != NULL)
						changeinput(listbox->select, ret);
					else
					{
						tmpstr = ostrcat(ret, "/mediadb", 0, 0);
						if(mkdir(tmpstr, 777) > -1)
							changeinput(listbox->select, tmpstr);
						free(tmpstr); tmpstr = NULL;
					}
				}
				free(ret);

				drawscreen(mediadbsettings, 0, 0);
			}
		}
	}

	delownerrc(mediadbsettings);
	clearscreen(mediadbsettings);
}

#endif
