#ifndef MC_INTERNETBROWSER_H
#define MC_INTERNETBROWSER_H

void screenmc_internetbrowser()
{
	if(status.security == 1)
	{
		int treffer = 0;
		struct skin* plugin = getscreen("plugin");
		struct skin* child = plugin->child;

		while(child != NULL)
		{
			if(child->del == PLUGINDELMARK)
			{
				if(ostrcmp(child->name, "Internet Browser") == 0)
					treffer = 1;
				child = child->next;
			}
		}

		if(treffer == 1)
		{
//			if(textbox(_("Message"), _("start Internet browser now ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1){	
				char* tmpstr = NULL;
				int rcret = 0;
	
				status.sec = 0; //deaktivate spinner
				setfbtransparent(255);
				rcret = servicestop(status.aktservice, 1, 0);
				if(rcret == 1) return;
	
				tmpstr = ostrcat("nsfb -f linux ", getconfig("browserhome", NULL), 0, 0);
				system(tmpstr);
				free(tmpstr);
				setosdtransparent(getskinconfigint("osdtransparent", NULL));
				drawscreen(skin, 0, 0);
				servicestart(status.lastservice->channel, NULL, NULL, 0);
				sleep(2);
//			}
		}
		else
			textbox(_("Message"), _("Install netsurf Internetbrowser Ipk first !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
	}
	else
		textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
}

#endif
