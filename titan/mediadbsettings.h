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
	struct skin* createthumb = getscreennode(mediadbsettings, "createthumb");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	addchoicebox(mediadbpath, "/media/hdd/mediadb", "/media/hdd/mediadb");
	tmpstr = realpath("/var/swap", NULL);
	if(tmpstr != NULL && ostrcmp(tmpstr, "/mnt/swapextensions") != 0)
		addchoicebox(mediadbpath, "/var/swap/mediadb", "/var/swap/mediadb");
	free(tmpstr); tmpstr = NULL;
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
			addconfigscreencheck("mediadbpath", mediadbpath, NULL);

			if(!file_exist(getconfig("mediadbpath", NULL)))
				mkdir(getconfig("mediadbpath", NULL), 0777);
			
			free(tmpstr), tmpstr = NULL;
			tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/mediadb", 0, 0);
			addconfig("mediadbfile", tmpstr);
			free(tmpstr); tmpstr = NULL;
			addconfigscreencheck("mediadbscandelall", mediadbscandelall, "0");
			addconfigscreencheck("mediadbscandelnotfound", mediadbscandelnotfound, "0");
			addconfigscreencheck("mediadbscantimeout", mediadbscantimeout, "0");

			addconfigscreencheck("createthumb", createthumb, "0");
			if(ostrcmp(createthumb->ret, "0") == 0)
				startthumb(0);
			else
				startthumb(1);

			writeallconfig(1);
			break;
		}
	}
	free(ext2);
	free(ext3);
	free(vfat);
	free(ntfsmount);
	free(ntfs3g);
	free(cifs);
	free(nfs);
	delownerrc(mediadbsettings);
	clearscreen(mediadbsettings);
}

#endif
