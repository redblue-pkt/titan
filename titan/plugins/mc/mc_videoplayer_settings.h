#ifndef MC_VIDEOPLAYER_SETTINGS_H
#define MC_VIDEOPLAYER_SETTINGS_H

void screenmc_videoplayer_settings()
{
	int rcret = 0;
	struct skin* tmp = NULL;

	struct skin* mc_videoplayer_settings = getscreen("mc_videoplayer_settings");
	struct skin* listbox = getscreennode(mc_videoplayer_settings, "listbox");
	struct skin* dirsort = getscreennode(mc_videoplayer_settings, "sort");
	struct skin* view = getscreennode(mc_videoplayer_settings, "view");
	struct skin* imdb_directory = getscreennode(mc_videoplayer_settings, "imdb_directory");
	struct skin* vp_autostart_playlist = getscreennode(mc_videoplayer_settings, "autostart_playlist");

	addchoicebox(dirsort, "0", _("alpha"));	
	addchoicebox(dirsort, "1", _("reverse alpha"));	
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("dirsort", NULL));

	addchoicebox(view, "0", _("fast"));
	addchoicebox(view, "1", _("big"));	
	addchoicebox(view, "2", _("cover"));	
	addchoicebox(view, "3", _("default"));
	addchoicebox(view, "4", _("details"));
	addchoicebox(view, "5", _("fullcover"));
	setchoiceboxselection(view, getconfig("vp_view", NULL));

	addchoicebox(vp_autostart_playlist, "0", _("no"));	
	addchoicebox(vp_autostart_playlist, "1", _("yes"));
	setchoiceboxselection(vp_autostart_playlist, getconfig("vp_autostart_playlist", NULL));

	system("ls -al /media/net ; ls -al /media/usb");

	addchoicebox(imdb_directory, "/var/swap", "/var/swap");

	char* nfs = command("cat /proc/mounts | grep nfs | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory nfs: %s", nfs);
	addchoicebox(imdb_directory, nfs, nfs);

	char* cifs = command("cat /proc/mounts | grep cifs | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory cifs: %s", cifs);
	addchoicebox(imdb_directory, cifs, cifs);

	char* vfat = command("cat /proc/mounts | grep vfat | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory vfat: %s", vfat);
	addchoicebox(imdb_directory, vfat, vfat);

	char* ntfs3g = command("cat /proc/mounts | grep ntfs-3g | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory ntfs-3g: %s", ntfs3g);
	addchoicebox(imdb_directory, ntfs3g, ntfs3g);

	char* ntfsmount = command("cat /proc/mounts | grep ntfsmount | awk '{print $2}'");	
	debug(50, "[mc_imdb_sync] imdb_directory ntfsmount: %s", ntfsmount);
	addchoicebox(imdb_directory, ntfsmount, ntfsmount);

	char* jfs = command("cat /proc/mounts | grep jfs | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory jfs: %s", jfs);
	addchoicebox(imdb_directory, jfs, jfs);

	char* ext2 = command("cat /proc/mounts | grep ext2 | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory ext2: %s", ext2);
	addchoicebox(imdb_directory, ext2, ext2);

	char* ext3 = command("cat /proc/mounts | grep ext3 | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory ext3: %s", ext3);
	addchoicebox(imdb_directory, ext3, ext3);

	setchoiceboxselection(imdb_directory, getconfig("imdb_directory", NULL));

	drawscreen(mc_videoplayer_settings, 0);
	addscreenrc(mc_videoplayer_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_videoplayer_settings, tmp);
		rcret = waitrc(mc_videoplayer_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("dirsort", dirsort, NULL);
			addconfigscreencheck("vp_view", view, NULL);
			addconfigscreencheck("imdb_directory", imdb_directory, NULL);
			addconfigscreencheck("vp_autostart_playlist", vp_autostart_playlist, NULL);		
			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			struct skin* screen = getscreen("networkmenu");
			menu(screen);
			drawscreen(mc_videoplayer_settings, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			mediadbscan("/media", 0);
		}		
  }

	delownerrc(mc_videoplayer_settings);
	clearscreen(mc_videoplayer_settings);
	free(ext2);
	free(ext3);
	free(vfat);
	free(ntfsmount);
	free(ntfs3g);
	free(cifs);
	free(nfs);
}

#endif
