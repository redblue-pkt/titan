#ifndef KEYACTIONS_H
#define KEYACTIONS_H

//key 0: blue
//key 1: red
//flag 0: without menulist
//flag 1: with menulist
void screenkeyactions(int key, int flag)
{
	debug(1000, "in");

	char* keyconf = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	char* tmpstr = NULL, *mlistbox = NULL;
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	
	if(flag == 1)
	{
		char* skintitle = "Key Action - Plugins";
	
		if(key == 1)
		{
			while(child != NULL)
			{
				if(child->del == PLUGINDELMARK)
				{
					tmpstr = ostrcat(tmpstr, child->name, 1, 0);
					debug(60, "key: %s", child->name);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				child = child->next;
			}
		}
	
		//tmpstr = ostrcat(tmpstr, "Extensions List\n", 1, 0);
		tmpstr = ostrcat(tmpstr, "Auto Resolution\n", 1, 0);
		if(checkemu() == 1)
			tmpstr = ostrcat(tmpstr, "Softcam Panel\n", 1, 0);
		//tmpstr = ostrcat(tmpstr, "TV / Radio Switch\n", 1, 0);
		tmpstr = ostrcat(tmpstr, "Multi EPG\n", 1, 0);
		tmpstr = ostrcat(tmpstr, "Graphic Multi EPG\n", 1, 0);
		tmpstr = ostrcat(tmpstr, "Sleep Timer\n", 1, 0);
		tmpstr = ostrcat(tmpstr, "Child Protection\n", 1, 0);
		tmpstr = ostrcat(tmpstr, "Subchannel\n", 1, 0);
	
		mlistbox = menulistbox(NULL, tmpstr, NULL, skintitle, NULL, 1, 0);
		keyconf = mlistbox;
		free(tmpstr); tmpstr = NULL;
	}
	else
	{
		if (key == 0) keyconf = getconfig("bluekey", NULL);
		if (key == 1) keyconf = getconfig("redkey", NULL);
	}
	
	if(flag == 0 && keyconf == NULL)
	{
		if(key == 1) screenkeyactions(1, 1);
		free(mlistbox); mlistbox = NULL;
		return;
	}
	
	debug(60, "key=%s", keyconf);
	if(ostrcmp(keyconf, "Softcam Panel") == 0)
	{
		pluginnode = getplugin("Panel");
		
		if(pluginnode != NULL)
		{
			startplugin = dlsym(pluginnode->pluginhandle, "screenpanel_softcam");
			if(startplugin != NULL){
				startplugin();
			}
			free(mlistbox); mlistbox = NULL;
			return;
		}
	}
	else if(ostrcmp(keyconf, "Subchannel") == 0)
	{
		screenlinkedchannel();
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(keyconf, "Auto Resolution") == 0)
	{
		keyactions_setres();
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(keyconf, "Extensions List") == 0)
	{
		screenkeyactions(1, 1);
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(keyconf, "Multi EPG") == 0)
	{
		screenmultiepg(NULL, NULL, 0);
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(keyconf, "Graphic Multi EPG") == 0)
	{
		screengmultiepg(NULL, NULL, 0);
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(keyconf, "Sleep Timer") == 0)
	{
		screenpowerofftimer();
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(keyconf, "Child Protection") == 0)
	{
		screenpin();
		free(mlistbox); mlistbox = NULL;
		return;
	}
			
	pluginnode = getplugin(keyconf);

	if(pluginnode != NULL)
	{
		startplugin = dlsym(pluginnode->pluginhandle, "start");
		if(startplugin != NULL)
			startplugin();
	}

	free(mlistbox); mlistbox = NULL;
	debug(1000, "out");
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
				res_def = getconfig("av_videomode_default", NULL);
				if(res_def == NULL)
					 res_def = res_act;
				setvideomode(res_def);
				changefbresolution(res_def);
				sleep(2);
				screenautores(res_def, 5, 0);		 	
			}
			else
			{
				setvideomode(res);
				changefbresolution(res);
				sleep(2);
				screenautores(res, 5, 0);
			}
		}			
	}
	else
	{
		textbox(_("Message"), _("ERROR cant read res"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);			
	}
	free(res);
	res = NULL;
	
	return;
}				

#endif