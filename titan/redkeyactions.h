#ifndef REDKEYACTIONS_H
#define REDKEYACTIONS_H

void screenredkeyactions()
{
	debug(1000, "in");

	char* redkey = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);

	redkey = getconfig("redkey", NULL);
	if(redkey == NULL)
	{
		redkeyactions_extensions();
		return;
	}
	
	debug(60, "redkey=%s", redkey);
	if(ostrcmp(redkey, "Softcam Panel") == 0)
	{
		pluginnode = getplugin("Panel");
		
		if(pluginnode != NULL)
		{
			startplugin = dlsym(pluginnode->pluginhandle, "screenpanel_softcam");
			if(startplugin != NULL){
				startplugin();
			}
			return;
		}
	}
	else if(ostrcmp(redkey, "Auto Resolution") == 0)
	{
		bluekeyactions_setres();
		return;
	}
	else if(ostrcmp(redkey, "Extensions List") == 0)
	{
		redkeyactions_extensions();
		return;
	}
	else if(ostrcmp(redkey, "Multi EPG") == 0)
	{
		screenmultiepg(NULL, NULL, 0);
		return;
	}
	else if(ostrcmp(redkey, "Sleep Timer") == 0)
	{
		screenpowerofftimer();
		return;
	}
	else if(ostrcmp(redkey, "Child Protection") == 0)
	{
		screenpin();
		return;
	}
			
	pluginnode = getplugin(redkey);

	if(pluginnode != NULL)
	{
		startplugin = dlsym(pluginnode->pluginhandle, "start");
		if(startplugin != NULL)
			startplugin();
	}

	debug(1000, "out");
	return;
}

void redkeyactions_extensions()
{
	debug(1000, "in");
	char* skintitle = "Red Key Action - Plugins";

	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;

	struct skin* pluginnode = NULL;
	void (*startplugin)(void);

	char* tmpstr = NULL;
	if(checkemu() == 1)
		tmpstr = ostrcat(tmpstr, "Softcam Panel\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Auto Resolution\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Multi EPG\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Sleep Timer\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Child Protection\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Subchannel\n", 1, 0);
				
	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK)
		{
			tmpstr = ostrcat(tmpstr, child->name, 1, 0);
			debug(60, "[screenredkeyactions] Redkey: %s", child->name);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		}
		child = child->next;
	}

	char* mlistbox = menulistbox(NULL, tmpstr, NULL, skintitle, NULL, 1, 0);

	if(ostrcmp(mlistbox, "Softcam Panel") == 0)
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
	else if(ostrcmp(bluekey, "Subchannel") == 0)
	{
		screenlinkedchannel();
		return;
	}
	else if(ostrcmp(mlistbox, "Auto Resolution") == 0)
	{
		bluekeyactions_setres();
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(mlistbox, "Multi EPG") == 0)
	{
		screenmultiepg(NULL, NULL, 0);
		free(mlistbox); mlistbox = NULL;
		return;
	}
	else if(ostrcmp(mlistbox, "Sleep Timer") == 0)
	{
		screenpowerofftimer();
		free(mlistbox); mlistbox= NULL;
		return;
	}
	else if(ostrcmp(mlistbox, "Child Protection") == 0)
	{
		screenpin();
		free(mlistbox); mlistbox= NULL;
		return;
	}

	pluginnode = getplugin(mlistbox);

	if(pluginnode != NULL)
	{
		startplugin = dlsym(pluginnode->pluginhandle, "start");
		if(startplugin != NULL)
			startplugin();
	}

	free(tmpstr); tmpstr = NULL;
	free(mlistbox); mlistbox = NULL;

	debug(1000, "out");
	return;
}

#endif
