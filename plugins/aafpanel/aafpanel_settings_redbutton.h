#ifndef AAFPANEL_SETTINGS_REDBUTTON_H
#define AAFPANEL_SETTINGS_REDBUTTON_H

void screenaafpanel_settings_redbutton()
{
	debug(1000, "in");
	char* skintitle = "Red Key Action - Setup";
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	tmpstr = ostrcat(tmpstr, "Extentions List (default)\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Softcam Panel\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Auto Resolution\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Multi EPG\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Sleep Timer\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Child Protection\n", 1, 0);

	char* redkey = getconfig("redkey", NULL);

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK)
		{
			tmpstr = ostrcat(tmpstr, child->name, 1, 0);
			debug(60, "Redkey: %s", child->name);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		}
		child = child->next;
	}
	debug(60, "Redkey: %s (default)", redkey);

	char* mlistbox = menulistbox(redkey, tmpstr, NULL, skintitle, NULL, 1);

	if(mlistbox == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		return;
	}
	debug(60, "(new) Redkey=%s", mlistbox);

	if(ostrcmp(mlistbox, "Extentions List (default)") == 0)
		delconfig("redkey");
	else
		addconfig("redkey", mlistbox);

	free(mlistbox); mlistbox = NULL;
	free(tmpstr); tmpstr = NULL;
	debug(1000, "out");
	return;
}

#endif
