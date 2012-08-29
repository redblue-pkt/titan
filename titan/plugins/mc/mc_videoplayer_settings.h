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
	struct skin* autoscan = getscreennode(mc_videoplayer_settings, "autoscan");
//	struct skin* mediadb = getscreennode(mc_videoplayer_settings, "mediadb");
//	struct skin* mediadbscandelall = getscreennode(mc_videoplayer_settings, "mediadbscandelall");
//	struct skin* mediadbscandelnotfound = getscreennode(mc_videoplayer_settings, "mediadbscandelnotfound");	
	struct skin* autostart_playlist = getscreennode(mc_videoplayer_settings, "autostart_playlist");

	addchoicebox(dirsort, "0", _("alpha"));	
	addchoicebox(dirsort, "1", _("reverse alpha"));	
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("mc_vp_dirsort", NULL));

	addchoicebox(view, "0", _("fast"));
	addchoicebox(view, "1", _("big"));	
	addchoicebox(view, "2", _("cover"));
	addchoicebox(view, "3", _("fullcover"));	
	addchoicebox(view, "4", _("size (default)"));
	addchoicebox(view, "5", _("datetime"));
	setchoiceboxselection(view, getconfig("mc_vp_view", NULL));

	addchoicebox(autostart_playlist, "0", _("no"));	
	addchoicebox(autostart_playlist, "1", _("yes"));
	setchoiceboxselection(autostart_playlist, getconfig("mc_vp_autostart_playlist", NULL));

	addchoicebox(autoscan, "0", _("no"));	
	addchoicebox(autoscan, "1", _("yes"));
	setchoiceboxselection(autoscan, getconfig("mc_vp_autoscan", NULL));
/*
	system("ls -al /media/net ; ls -al /media/usb");

	addchoicebox(mediadb, "/var/swap", "/var/swap");

	char* nfs = command("cat /proc/mounts | grep nfs | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory nfs: %s", nfs);
	addchoicebox(mediadb, nfs, nfs);

	char* cifs = command("cat /proc/mounts | grep cifs | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory cifs: %s", cifs);
	addchoicebox(mediadb, cifs, cifs);

	char* vfat = command("cat /proc/mounts | grep vfat | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory vfat: %s", vfat);
	addchoicebox(mediadb, vfat, vfat);

	char* ntfs3g = command("cat /proc/mounts | grep ntfs-3g | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory ntfs-3g: %s", ntfs3g);
	addchoicebox(mediadb, ntfs3g, ntfs3g);

	char* ntfsmount = command("cat /proc/mounts | grep ntfsmount | awk '{print $2}'");	
	debug(50, "[mc_imdb_sync] imdb_directory ntfsmount: %s", ntfsmount);
	addchoicebox(mediadb, ntfsmount, ntfsmount);

	char* jfs = command("cat /proc/mounts | grep jfs | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory jfs: %s", jfs);
	addchoicebox(mediadb, jfs, jfs);

	char* ext2 = command("cat /proc/mounts | grep ext2 | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory ext2: %s", ext2);
	addchoicebox(mediadb, ext2, ext2);

	char* ext3 = command("cat /proc/mounts | grep ext3 | awk '{print $2}'");
	debug(50, "[mc_imdb_sync] imdb_directory ext3: %s", ext3);
	addchoicebox(mediadb, ext3, ext3);

	setchoiceboxselection(mediadb, getconfig("mc_vp_mediadb", NULL));

	addchoicebox(mediadbscandelall, "0", _("no"));
	addchoicebox(mediadbscandelall, "1", _("yes"));
	setchoiceboxselection(mediadbscandelall, getconfig("mc_vp_mediadbscandelall", NULL));

	addchoicebox(mediadbscandelnotfound, "0", _("no"));
	addchoicebox(mediadbscandelnotfound, "1", _("yes"));
	setchoiceboxselection(mediadbscandelnotfound, getconfig("mc_vp_mediadbscandelnotfound", NULL));
*/	
	drawscreen(mc_videoplayer_settings, 0, 0);
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
			addconfigscreencheck("mc_vp_dirsort", dirsort, NULL);
			addconfigscreencheck("mc_vp_view", view, NULL);
			
			addconfigscreencheck("mc_vp_autoscan", autoscan, NULL);			
//			addconfigscreencheck("mc_vp_mediadb", mediadb, NULL);
//			addconfigscreencheck("mc_vp_mediadbscandelall", mediadbscandelall, NULL);
//			addconfigscreencheck("mc_vp_mediadbscandelnotfound", mediadbscandelnotfound, NULL);						

			addconfigscreencheck("vp_autostart_playlist", autostart_playlist, NULL);		
			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			struct skin* screen = getscreen("networkmenu");
			menu(screen);
			drawscreen(mc_videoplayer_settings, 0, 0);
		}		
  }

	delownerrc(mc_videoplayer_settings);
	clearscreen(mc_videoplayer_settings);
//	free(ext2);
//	free(ext3);
//	free(vfat);
//	free(ntfsmount);
//	free(ntfs3g);
//	free(cifs);
//	free(nfs);
}

#endif
