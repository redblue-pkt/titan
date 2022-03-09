#ifndef MC_INTERNETBROWSER_H
#define MC_INTERNETBROWSER_H

void screenmc_internetbrowser()
{
	int treffer = 0;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK)
		{
			if(ostrcmp(child->name, "Internet Browser") == 0)
				treffer = 1;
		}
		child = child->next;
	}

	if(treffer == 1)
	{
		pluginnode = getplugin("Internet Browser");
	
		if(pluginnode != NULL)
		{
			startplugin = dlsym(pluginnode->pluginhandle, "start");
			if(startplugin != NULL)
			{
				startplugin();
			}
		}
	}
	else
		textbox(_("Message"), _("Install netsurf Internetbrowser Tpk first !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
}

#endif
