#ifndef MC_MEDIATHEK_H
#define MC_MEDIATHEK_H

void screenmc_mediathek()
{
	if(status.security == 1)
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
				if(ostrcmp(child->name, "TiTan Mediathek") == 0)
					treffer = 1;
			}
			child = child->next;
		}

		if(treffer == 1)
		{
//			if(textbox(_("Message"), _("start Tithek now ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1){	
				char* tmpstr = NULL;
				int rcret = 0;
	
				status.sec = 0; //deaktivate spinner
//				setfbtransparent(255);
//				rcret = servicestop(status.aktservice, 1, 0);
//				if(rcret == 1) return;
	
//				screentithekplay("http://atemio.dyndns.tv/mediathek/mainmenu.list", "Tithek - Mainmenu", 1)
				pluginnode = getplugin("TiTan Mediathek");
			
				if(pluginnode != NULL)
				{
					startplugin = dlsym(pluginnode->pluginhandle, "start");
					if(startplugin != NULL)
					{
//						resettvpic();
						startplugin();
					}
				}
				printf("gggggggggggggg\n");

//				setosdtransparent(getskinconfigint("osdtransparent", NULL));
//				drawscreen(skin, 0, 0);
//				servicestart(status.lastservice->channel, NULL, NULL, 0);
//				sleep(2);
//			}
		}
		else
			textbox(_("Message"), _("Install Tithek Ipk first !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
	}
	else
		textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
}

#endif
