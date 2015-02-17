#ifndef SETTINGS_BLUEBUTTON_H
#define SETTINGS_BLUEBUTTON_H

void screensettings_bluebutton()
{
	char* skintitle = _("Blue Key Action - Setup");
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;

	addmenulist(&mlist, "TV / Radio Switch", _("TV / Radio Switch"), NULL, 0, 0);
	if(checkemu() == 1)
		addmenulist(&mlist, "Softcam Panel", _("Softcam Panel"), NULL, 0, 0);
	addmenulist(&mlist, "Extensions List", _("Extensions List"), NULL, 0, 0);
	addmenulist(&mlist, "Multi EPG", _("Multi EPG"), NULL, 0, 0);
	addmenulist(&mlist, "Graphic Multi EPG", _("Graphic Multi EPG"), NULL, 0, 0);
	addmenulist(&mlist, "Sleep Timer", _("Sleep Timer"), NULL, 0, 0);
	addmenulist(&mlist, "Child Protection", _("Child Protection"), NULL, 0, 0);
	addmenulist(&mlist, "Subchannel", _("Subchannel"), NULL, 0, 0);
	addmenulist(&mlist, "Downloads", _("Downloads"), NULL, 0, 0);

	while(child != NULL)
	{
	if(child->del == PLUGINDELMARK && (status.security >= 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
			addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
		child = child->next;
	}
	
	char* bluekey = getconfig("bluekey", NULL);
	debug(60, "Bluekey: %s (default)", bluekey);

	setmenulistdefault(mlist, bluekey);
	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your plugin / menu item from the following list and show it so the blue button"), NULL, NULL, 1, 0);

	if(mbox == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		return;
	}
	debug(60, "(new) Bluekey=%s", mbox->name);

	if(ostrcmp(mbox->name, "TV / Radio Switch") == 0)
		delconfig("bluekey");
	else
		addconfig("bluekey", mbox->name);

	freemenulist(mlist, 1); mlist = NULL;
	free(tmpstr); tmpstr = NULL;
	return;
}

#endif
