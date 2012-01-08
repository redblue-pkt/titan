#ifndef PANEL_SETTINGS_REDBUTTON_H
#define PANEL_SETTINGS_REDBUTTON_H

void screenpanel_settings_redbutton()
{
	debug(1000, "in");
	char* skintitle = "Red Key Action - Setup";
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	tmpstr = ostrcat(tmpstr, "Extensions List\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Auto Resolution\n", 1, 0);
	if(checkemu() == 1)
		tmpstr = ostrcat(tmpstr, "Softcam Panel\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "TV / Radio Switch\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Multi EPG\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Graphic Multi EPG\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Sleep Timer\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Child Protection\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Subchannel\n", 1, 0);

	char* redkey = getconfig("redkey", NULL);
	int skip = 0;

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK)
		{
			if(status.securety == 0)
			{
				if(ostrcmp(child->name, "Softcam Panel") == 0) skip = 1;
				if(ostrcmp(child->name, "Media Center") == 0) skip = 1;
				if(ostrcmp(child->name, "Internet Browser") == 0) skip = 1;
				if(ostrcmp(child->name, "CallMonitor") == 0) skip = 1;
				if(ostrcmp(child->name, "Imdb") == 0) skip = 1;
				if(ostrcmp(child->name, "KeyLock") == 0) skip = 1;
				if(ostrcmp(child->name, "Mbox Info") == 0) skip = 1;
				if(ostrcmp(child->name, "Networkbrowser") == 0) skip = 1;
				if(ostrcmp(child->name, "Permanent Time") == 0) skip = 1;	
				if(ostrcmp(child->name, "TopfieldVFD") == 0) skip = 1;
				if(ostrcmp(child->name, "Hello") == 0) skip = 1;
			}
			if(skip == 0)
			{
				tmpstr = ostrcat(tmpstr, child->name, 1, 0);
				debug(60, "Redkey: %s", child->name);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			skip = 0;
		}
		child = child->next;
	}
	debug(60, "Redkey: %s (default)", redkey);

	char* mlistbox = menulistbox(redkey, tmpstr, NULL, skintitle, NULL, 1, 0);

	if(mlistbox == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		return;
	}
	debug(60, "(new) Redkey=%s", mlistbox);

	if(ostrcmp(mlistbox, "Extensions List") == 0)
		delconfig("redkey");
	else
		addconfig("redkey", mlistbox);

	free(mlistbox); mlistbox = NULL;
	free(tmpstr); tmpstr = NULL;
	debug(1000, "out");
	return;
}

#endif
