#ifndef KEYACTIONS_H
#define KEYACTIONS_H

//key 0: blue
//key 1: red
//key 2: plugin
//key 3: media
//flag 0: without menulist
//flag 1: with menulist
void screenkeyactions(int key, int flag)
{
	char* keyconf = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	struct menulist* mlist = NULL, *mbox = NULL;

	if(flag == 1)
	{
		char* skintitle = _("Key Action - Plugins");

		if(key != 3 && status.mediadbfiles > 0)
			addmenulist(&mlist, "MediaDB Scan Info", _("MediaDB Scan Info"), NULL, 0, 0);
			
		if(key == 1)
		{
			while(child != NULL)
			{
				if(child->del == PLUGINDELMARK && (status.security >= 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
				{
					if(!ostrncmp("TMDb", child->name, 4))
					{
						debug(60, "skip key: %s", child->name);			
					}
					else if(!ostrncmp("Reader Config", child->name, 13))
					{
						debug(60, "skip key: %s", child->name);			
					}
					else
					{
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
						debug(60, "key: %s", child->name);
					}
				}
				child = child->next;
			}
			addmenulist(&mlist, "Media Player", _("Media Player"), NULL, 0, 0);
			addmenulist(&mlist, "Record Player", _("Record Player"), NULL, 0, 0);
		}
	
		if(key == 3)
		{
			addmenulist(&mlist, "Record Player", _("Record Player"), NULL, 0, 0);
			while(child != NULL)
			{
				if(child->del == PLUGINDELMARK && (status.security >= 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
				{
					if(ostrcmp(child->name, "Media Center") == 0)
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);			
					else if(ostrcmp(child->name, "Titan Mediathek") == 0)
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
					else if(ostrcmp(child->name, "Titan Media Center") == 0)
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
					else if(ostrcmp(child->name, "GmediaRender") == 0)
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
					else if(ostrcmp(child->name, "DVD Player") == 0)
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
					else if(ostrcmp(child->name, "KodiStarter") == 0)
						addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);						
				}
				child = child->next;
			}
			addmenulist(&mlist, "Media Player", _("Media Player"), NULL, 0, 0);
		}
		else
		{
			//addmenulist(&mlist, "Extensions List", _("Extensions List"), NULL, 0, 0);
			addmenulist(&mlist, "Auto Resolution", _("Auto Resolution"), "resolution.png", 0, 0);
			if(checkemu() == 1)
				addmenulist(&mlist, "Softcam Panel", _("Softcam Panel"), NULL, 0, 0);
			//addmenulist(&mlist, "TV / Radio Switch", _("TV / Radio Switch"), NULL, 0, 0);
			addmenulist(&mlist, "Bouquet List", _("Bouquet List"), NULL, 0, 0);
			addmenulist(&mlist, "Multi EPG", _("Multi EPG"), NULL, 0, 0);
			addmenulist(&mlist, "Graphic Multi EPG", _("Graphic Multi EPG"), NULL, 0, 0);
			addmenulist(&mlist, "Sleep Timer", _("Sleep Timer"), NULL, 0, 0);
			addmenulist(&mlist, "Child Protection", _("Child Protection"), NULL, 0, 0);
			addmenulist(&mlist, "Subchannel", _("Subchannel"), NULL, 0, 0);
			addmenulist(&mlist, "Downloads", _("Downloads"), NULL, 0, 0);
		}
	
		mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, NULL, 1, 0);
		if(mbox != NULL) keyconf = ostrcat(mbox->name, NULL, 0, 0);
		freemenulist(mlist, 1); mlist = NULL;
	}
	else
	{
		if(key == 0) keyconf = ostrcat(getconfig("bluekey", NULL), NULL, 0, 0);
		if(key == 1) keyconf = ostrcat(getconfig("redkey", NULL), NULL, 0, 0);
		if(key == 2) keyconf = ostrcat(getconfig("pluginkey", NULL), NULL, 0, 0);
		if(key == 3) keyconf = ostrcat(getconfig("mediakey", NULL), NULL, 0, 0);
	}
	
	if(flag == 0 && keyconf == NULL)
	{
		if(key == 1) screenkeyactions(1, 1);
		if(key == 2) screenkeyactions(2, 1);
		if(key == 3) screenkeyactions(3, 1);
	}
	else
	{
		if(status.security == 0 && checkpluginskip(keyconf) == 1) return;
		resettvpic();
							
		debug(60, "key=%s", keyconf);
		if(ostrcmp(keyconf, "Softcam Panel") == 0)
			screensoftcam();
		else if(ostrcmp(keyconf, "Subchannel") == 0)
			screenlinkedchannel();
		else if(ostrcmp(keyconf, "Auto Resolution") == 0)
			keyactions_setres();
		else if(ostrcmp(keyconf, "Extensions List") == 0)
			screenkeyactions(1, 1);
		else if(ostrcmp(keyconf, "Multi EPG") == 0)
			screenmultiepg(NULL, NULL, 0);
		else if(ostrcmp(keyconf, "Graphic Multi EPG") == 0)
			screengmultiepg(NULL, NULL, 0);
		else if(ostrcmp(keyconf, "Sleep Timer") == 0)
			screenpowerofftimer();
		else if(ostrcmp(keyconf, "Child Protection") == 0)
			screenpin();
		else if(ostrcmp(keyconf, "Downloads") == 0)
			screenbgdownload(0);
		else if(ostrcmp(keyconf, "MediaDB Scan Info") == 0)
			get_mediadb_scan_info();
		else if(ostrcmp(keyconf, "Record Player") == 0)
		{
			int oldsort = getconfigint("dirsort", NULL);
			addconfigint("dirsort", 4);
			screenplay(NULL, NULL, 1, 0);
			addconfigint("dirsort", oldsort);
		}
		else if(ostrcmp(keyconf, "Media Player") == 0)
		{
			int oldsort = getconfigint("dirsort", NULL);
			screenplay(NULL, NULL, 0, 0);
			addconfigint("dirsort", oldsort);
		}
		else if(ostrcmp(keyconf, "Media Plugins List") == 0)
			screenkeyactions(3, 1);
		else if(ostrcmp(keyconf, "Bouquet List") == 0)
			screenchannellist(NULL, NULL, 2);
		else
		{
			pluginnode = getplugin(keyconf);
		
			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "start");
				if(startplugin != NULL)
					startplugin();
			}
			else if(keyconf != NULL)
				screenkeyactions(1, 1);
		}
	}

	free(keyconf), keyconf = NULL;
	resettvpic();
	return;
}

void keyactions_setres()
{
	int m_width;
	char* res = NULL;
	char* res_act = NULL;
	char* res_def = NULL;
	char* res_sd = NULL;
	
	if(getaktvideosize() == 0)
	{
		m_width = status.videosize.w;
		if(m_width == 720)
		{
			res_sd = getconfig("av_videomode_autores_sd", NULL);
			if(res_sd == NULL) 
				res = ostrcat(res, "576i50", 1, 0);
			else
				res = ostrcat(res, res_sd, 1, 0);
		}
		else if(m_width == 1280)
			res = ostrcat(res, "720p50", 1, 0);
		else if(m_width == 1920)
			res = ostrcat(res, "1080i50", 1, 0);	
		else
			m_width = 0;
		
		if(m_width > 0)
		{	
			res_act = getvideomode();
			if(ostrcmp(res_act, res) == 0) 
			{
				res_def = getconfig("av_videomode", NULL);
				if(res_def == NULL)
					 res_def = res_act;
				setvideomode(res_def, 1);
				changefbresolution(res_def, 0);
				screenautores(res_def, 5, 0);
			}
			else
			{
				setvideomode(res, 1);
				changefbresolution(res, 0);
				screenautores(res, 5, 0);
			}
		}			
	}
	else
	{
		textbox(_("Message"), _("ERROR cant read res"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);			
	}
	free(res);
	res = NULL;
	
	return;
}				

#endif
