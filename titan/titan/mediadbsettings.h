#ifndef MEDIADBSETTINGS_H
#define MEDIADBSETTINGS_H

void screenmediadbsettings()
{
	int rcret = 0;
	struct skin* mediadbsettings = getscreen("mediadbsettings");
	struct skin* listbox = getscreennode(mediadbsettings, "listbox");
	struct skin* mediadbdevice = getscreennode(mediadbsettings, "mediadbpath");
	struct skin* mediadbscandelall = getscreennode(mediadbsettings, "mediadbscandelall");
	struct skin* mediadbscandelnotfound = getscreennode(mediadbsettings, "mediadbscandelnotfound");
	struct skin* mediadbscantimeout = getscreennode(mediadbsettings, "mediadbscantimeout");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	system("ls -al /media/net/ ; ls -al /media/usb/");

	addchoicebox(mediadbdevice, "/var/swap", "/var/swap");

	char* nfs = command("cat /proc/mounts | grep nfs | awk '{print $2}'");
	debug(50, "mediadbdevice nfs: %s", nfs);
	addchoicebox(mediadbdevice, nfs, nfs);

	char* cifs = command("cat /proc/mounts | grep cifs | awk '{print $2}'");
	debug(50, "mediadbdevice cifs: %s", cifs);
	addchoicebox(mediadbdevice, cifs, cifs);

	char* vfat = command("cat /proc/mounts | grep vfat | awk '{print $2}'");
	debug(50, "mediadbdevice vfat: %s", vfat);
	addchoicebox(mediadbdevice, vfat, vfat);

	char* ntfs3g = command("cat /proc/mounts | grep ntfs-3g | awk '{print $2}'");
	debug(50, "mediadbdevice ntfs-3g: %s", ntfs3g);
	addchoicebox(mediadbdevice, ntfs3g, ntfs3g);

	char* ntfsmount = command("cat /proc/mounts | grep ntfsmount | awk '{print $2}'");	
	debug(50, "mediadbdevice ntfsmount: %s", ntfsmount);
	addchoicebox(mediadbdevice, ntfsmount, ntfsmount);

	char* jfs = command("cat /proc/mounts | grep jfs | awk '{print $2}'");
	debug(50, "mediadbdevice jfs: %s", jfs);
	addchoicebox(mediadbdevice, jfs, jfs);

	char* ext2 = command("cat /proc/mounts | grep ext2 | awk '{print $2}'");
	debug(50, "mediadbdevice ext2: %s", ext2);
	addchoicebox(mediadbdevice, ext2, ext2);

	char* ext3 = command("cat /proc/mounts | grep ext3 | awk '{print $2}'");
	debug(50, "mediadbdevice ext3: %s", ext3);
	addchoicebox(mediadbdevice, ext3, ext3);

	setchoiceboxselection(mediadbdevice, getconfig("mediadbdevice", NULL));


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
			addconfigscreencheck("mediadbdevice", mediadbdevice, NULL);
			tmpstr = ostrcat(getconfig("mediadbdevice", NULL), "/mediadb", 0, 0);
			addconfig("mediadbpath", tmpstr);

// not working
//			if(!file_exist(tmpstr))
//				mkdir(tmpstr, 777);

			if(!file_exist(tmpstr))
			{
				char* cmd = NULL;
				cmd = ostrcat("mkdir ", tmpstr, 0, 0);
				system(cmd);
				free(cmd), cmd = NULL;
			}
			
			free(tmpstr), tmpstr = NULL;
			tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/mediadb", 0, 0);
			addconfig("mediadbfile", tmpstr);
			free(tmpstr); tmpstr = NULL;
			addconfigscreencheck("mediadbscandelall", mediadbscandelall, "0");
			addconfigscreencheck("mediadbscandelnotfound", mediadbscandelnotfound, "0");
			addconfigscreencheck("mediadbscantimeout", mediadbscantimeout, "0");
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
