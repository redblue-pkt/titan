#ifndef KEYACTIONS_H
#define KEYACTIONS_H

//key 0: blue
//key 1: red
//key 2: plugin
//flag 0: without menulist
//flag 1: with menulist
void screenkeyactions(int key, int flag)
{
	char* keyconf = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	char* tmpstr = NULL;
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	struct menulist* mlist = NULL, *mbox = NULL;

// needed for translation
	char* tmptxt = NULL;
	tmptxt = ostrcat(tmptxt, _("Extensions List"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Auto Resolution"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("TV / Radio Switch"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Softcam Panel"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Multi EPG"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Graphic Multi EPG"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Sleep Timer"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Child Protection"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Subchannel"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Downloads"), 1, 0);
	free(tmptxt), tmptxt = NULL;
		
	if(flag == 1)
	{
		char* skintitle = _("Key Action - Plugins");

		if(status.mediadbfiles > 0)
			addmenulist(&mlist, _("MediaDB Scan Info"), NULL, NULL, 0, 0);
			
		if(key == 1)
		{
			while(child != NULL)
			{
				if(child->del == PLUGINDELMARK && (status.security == 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
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
						addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
						debug(60, "key: %s", child->name);
					}
				}
				child = child->next;
			}
		}
	
		//addmenulist(&mlist, "Extensions List", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Auto Resolution", NULL, "resolution.png", 0, 0);
		if(checkemu() == 1)
			addmenulist(&mlist, "Softcam Panel", NULL, NULL, 0, 0);
		//addmenulist(&mlist, "TV / Radio Switch", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Multi EPG", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Graphic Multi EPG", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Sleep Timer", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Child Protection", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Subchannel", NULL, NULL, 0, 0);
		addmenulist(&mlist, "Downloads", NULL, NULL, 0, 0);
	
		mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, 1, 0);
		if(mbox != NULL) keyconf = mbox->name;
		free(tmpstr); tmpstr = NULL;
	}
	else
	{
		if(key == 0) keyconf = getconfig("bluekey", NULL);
		if(key == 1) keyconf = getconfig("redkey", NULL);
		if(key == 2) keyconf = getconfig("pluginkey", NULL);
	}
	
	if(flag == 0 && keyconf == NULL)
	{
		if(key == 1) screenkeyactions(1, 1);
		if(key == 2) screenkeyactions(2, 1);
		freemenulist(mlist, 1); mlist = NULL;
		return;
	}

	if(status.security == 0 && checkpluginskip(keyconf) == 1) return;
						
	debug(60, "key=%s", keyconf);
	if(ostrcmp(keyconf, "Softcam") == 0)
	{
		screensoftcam();
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Subchannel") == 0)
	{
		screenlinkedchannel();
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Auto Resolution") == 0)
	{
		keyactions_setres();
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Extensions List") == 0)
	{
		screenkeyactions(1, 1);
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Multi EPG") == 0)
	{
		screenmultiepg(NULL, NULL, 0);
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Graphic Multi EPG") == 0)
	{
		screengmultiepg(NULL, NULL, 0);
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Sleep Timer") == 0)
	{
		screenpowerofftimer();
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Child Protection") == 0)
	{
		screenpin();
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "Downloads") == 0)
	{
		screenbgdownload(0);
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}
	else if(ostrcmp(keyconf, "MediaDB Scan Info") == 0)
	{
		get_mediadb_scan_info();
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}

	pluginnode = getplugin(keyconf);

	if(pluginnode != NULL)
	{
		startplugin = dlsym(pluginnode->pluginhandle, "start");
		if(startplugin != NULL)
		{
			resettvpic();
			startplugin();
		}
	}
	else if(keyconf != NULL)
	{
		screenkeyactions(1, 1);
		freemenulist(mlist, 1); mlist = NULL;
		resettvpic();
		return;
	}

	resettvpic();
	freemenulist(mlist, 1); mlist = NULL;
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
