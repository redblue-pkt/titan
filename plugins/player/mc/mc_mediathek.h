#ifndef MC_MEDIATHEK_H
#define MC_MEDIATHEK_H

void screenmc_mediathek()
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
			if(ostrcmp(child->name, "Titan Mediathek") == 0)
				treffer = 1;
		}
		child = child->next;
	}

	if(treffer == 1)
	{
		pluginnode = getplugin("Titan Mediathek");
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
		textbox(_("Message"), _("Install Tithek Tpk first !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
}

#endif
