#ifndef MEDIADBSETTINGS_H
#define MEDIADBSETTINGS_H

void screenmediadbsettings()
{
	int rcret = 0;
	struct skin* mediadbsettings = getscreen("mediadbsettings");
	struct skin* listbox = getscreennode(mediadbsettings, "listbox");
	struct skin* mediadbpath = getscreennode(mediadbsettings, "mediadbpath");
	struct skin* mediadbscandelall = getscreennode(mediadbsettings, "mediadbscandelall");
	struct skin* mediadbscandelnotfound = getscreennode(mediadbsettings, "mediadbscandelnotfound");
	struct skin* mediadbscantimeout = getscreennode(mediadbsettings, "mediadbscantimeout");
	struct skin* mediadbbackdrop = getscreennode(mediadbsettings, "mediadbbackdrop");
	struct skin* mediadbdebug = getscreennode(mediadbsettings, "mediadbdebug");	
	struct skin* createthumb = getscreennode(mediadbsettings, "createthumb");
	struct skin* recordpicture = getscreennode(mediadbsettings, "recordpicture");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	if(checkbox("ATEMIO510") != 1 && checkbox("UFS910") != 1 && checkbox("UFS922") != 1 && checkbox("ATEVIO700") != 1 && checkbox("ATEVIO7000") != 1 && checkbox("IPBOX91") != 1 && checkbox("IPBOX900") != 1 && checkbox("IPBOX910") != 1 && checkbox("IPBOX9000") != 1)
		addchoicebox(mediadbpath, "/mnt/swapextensions/.mediadb", "/mnt/swapextensions/.mediadb");
	addchoicebox(mediadbpath, "/media/hdd/.mediadb", "/media/hdd/.mediadb");
#ifdef OEBUILD
	if(file_exist("/media/.swapextensionsdev") == 1)
#else
	if(file_exist("/tmp/.swapextensionsdev") == 1)
#endif
		addchoicebox(mediadbpath, "/var/swap/.mediadb", "/var/swap/.mediadb");

	setchoiceboxselection(mediadbpath, getconfig("mediadbpath", NULL));

	addchoicebox(mediadbscandelall, "0", _("no"));
	addchoicebox(mediadbscandelall, "1", _("yes"));
	setchoiceboxselection(mediadbscandelall, getconfig("mediadbscandelall", NULL));

	addchoicebox(mediadbscandelnotfound, "0", _("no"));
	addchoicebox(mediadbscandelnotfound, "1", _("yes"));
	setchoiceboxselection(mediadbscandelnotfound, getconfig("mediadbscandelnotfound", NULL));

	addchoicebox(mediadbscantimeout, "0", _("always"));
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
	addchoicebox(mediadbscantimeout, "999999", _("never"));
	setchoiceboxselection(mediadbscantimeout, getconfig("mediadbscantimeout", NULL));

	addchoicebox(createthumb, "0", _("no"));
	addchoicebox(createthumb, "1", _("yes"));
	setchoiceboxselection(createthumb, getconfig("createthumb", NULL));

	addchoicebox(mediadbbackdrop, "0", _("all"));
	addchoicebox(mediadbbackdrop, "1", _("1"));
	addchoicebox(mediadbbackdrop, "2", _("2"));
	addchoicebox(mediadbbackdrop, "3", _("3"));
	addchoicebox(mediadbbackdrop, "4", _("4"));
	addchoicebox(mediadbbackdrop, "5", _("5"));
	addchoicebox(mediadbbackdrop, "10", _("10"));
	addchoicebox(mediadbbackdrop, "15", _("15"));
	addchoicebox(mediadbbackdrop, "20", _("20"));
	setchoiceboxselection(mediadbbackdrop, getconfig("mediadbbackdrop", NULL));

	addchoicebox(mediadbdebug, "0", _("no"));
	addchoicebox(mediadbdebug, "1", _("yes"));
	setchoiceboxselection(mediadbdebug, getconfig("mediadbdebug", NULL));

	addchoicebox(recordpicture, "0", _("no"));
	addchoicebox(recordpicture, "1", _("yes"));
	setchoiceboxselection(recordpicture, getconfig("recordpicture", NULL));
		
	drawscreen(mediadbsettings, 0, 0);
	addscreenrc(mediadbsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mediadbsettings, tmp);
		rcret = waitrc(mediadbsettings, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			break;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("mediadbpath", mediadbpath, NULL);

			if(!file_exist(getconfig("mediadbpath", NULL)))
			{
				if(textbox(_("Message"), _("No MediaDB Directory found\nPress OK to create it"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
					mkdir(getconfig("mediadbpath", NULL), 0777);
			}
			
			free(tmpstr); tmpstr = NULL;
			tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/mediadb", 0, 0);
			addconfig("mediadbfile", tmpstr);
			free(tmpstr); tmpstr = NULL;
			addconfigscreencheck("mediadbscandelall", mediadbscandelall, "0");
			addconfigscreencheck("mediadbscandelnotfound", mediadbscandelnotfound, "0");
			addconfigscreencheck("mediadbscantimeout", mediadbscantimeout, "0");
			addconfigscreencheck("mediadbbackdrop", mediadbbackdrop, "0");
			addconfigscreencheck("mediadbdebug", mediadbdebug, "0");
			addconfigscreencheck("createthumb", createthumb, "0");
			addconfigscreencheck("recordpicture", recordpicture, "0");
			if(ostrcmp(createthumb->ret, "0") == 0)
				startthumb(0);
			else
				startthumb(1);

			writeallconfig(1);

			break;
		}
	}

	delownerrc(mediadbsettings);
	clearscreen(mediadbsettings);
}

#endif
