#ifndef PANEL_SETTINGS_REDBUTTON_H
#define PANEL_SETTINGS_REDBUTTON_H

void screenpanel_settings_redbutton()
{
	debug(1000, "in");
	char* skintitle = "Red Key Action - Setup";
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	
	addmenulist(&mlist, "Extensions List", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Auto Resolution", NULL, NULL, 0, 0);
	if(checkemu() == 1)
		addmenulist(&mlist, "Softcam Panel", NULL, NULL, 0, 0);
	addmenulist(&mlist, "TV / Radio Switch", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Multi EPG", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Graphic Multi EPG", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Sleep Timer", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Child Protection", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Subchannel", NULL, NULL, 0, 0);

	int skip = 0;

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK)
		{
			if(status.security == 0)
			{
				if(ostrcmp(child->name, "Softcam Panel") == 0) skip = 1;
//				if(ostrcmp(child->name, "Media Center") == 0) skip = 1;
				if(ostrcmp(child->name, "Internet Browser") == 0) skip = 1;
				if(ostrcmp(child->name, "CallMonitor") == 0) skip = 1;
				if(ostrcmp(child->name, "Imdb") == 0) skip = 1;
				if(ostrcmp(child->name, "KeyLock") == 0) skip = 1;
				if(ostrcmp(child->name, "Mbox Info") == 0) skip = 1;
				if(ostrcmp(child->name, "Networkbrowser") == 0) skip = 1;
				if(ostrcmp(child->name, "Permanent Time") == 0) skip = 1;	
				if(ostrcmp(child->name, "TopfieldVFD") == 0) skip = 1;
				if(ostrcmp(child->name, "Hello") == 0) skip = 1;
				if(ostrcmp(child->name, "LCD Pearl") == 0) skip = 1;
			}
			if(skip == 0)
			{
				addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
			}
			skip = 0;
		}
		child = child->next;
	}

	char* redkey = getconfig("redkey", NULL);
	debug(60, "Redkey: %s (default)", redkey);
	
	setmenulistdefault(mlist, redkey);
	mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, 1, 0);

	if(mbox == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		return;
	}
	debug(60, "(new) Redkey=%s", mbox->name);

	if(ostrcmp(mbox->name, "Extensions List") == 0)
		delconfig("redkey");
	else
		addconfig("redkey", mbox->name);

	freemenulist(mlist, 1); mlist = NULL;
	free(tmpstr); tmpstr = NULL;
	debug(1000, "out");
	return;
}

#endif
