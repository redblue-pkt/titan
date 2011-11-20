#ifndef PANEL_SETTINGS_BLUEBUTTON_H
#define PANEL_SETTINGS_BLUEBUTTON_H

void screenpanel_settings_bluebutton()
{
	debug(1000, "in");
	char* skintitle = "Blue Key Action - Setup";
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	tmpstr = ostrcat(tmpstr, "Auto Resolution (default)\n", 1, 0);
	if(checkemu() == 1)
		tmpstr = ostrcat(tmpstr, "Softcam Panel\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Extentions List\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Multi EPG\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Graphic Multi EPG\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Sleep Timer\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "Child Protection\n", 1, 0);
	tmpstr = ostrcat(tmpstr, "TV / Radio Switch\n", 1, 0);

	char* bluekey = getconfig("bluekey", NULL);

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK)
		{
			tmpstr = ostrcat(tmpstr, child->name, 1, 0);
			debug(60, "Bluekey: %s", child->name);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		}
		child = child->next;
	}
	debug(60, "Bluekey: %s (default)", bluekey);

	char* mlistbox = menulistbox(bluekey, tmpstr, NULL, skintitle, NULL, 1);

	if(mlistbox == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		return;
	}
	debug(60, "(new) Bluekey=%s", mlistbox);

	if(ostrcmp(mlistbox, "Auto Resolution (default)") == 0)
		delconfig("bluekey");
	else
		addconfig("bluekey", mlistbox);

	free(mlistbox); mlistbox = NULL;
	free(tmpstr); tmpstr = NULL;
	debug(1000, "out");
	return;
}

#endif
