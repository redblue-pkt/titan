#ifndef SETTINGS_REDBUTTON_H
#define SETTINGS_REDBUTTON_H

void screensettings_redbutton()
{
	char* skintitle = _("Red Key Action - Setup");
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;

	addmenulist(&mlist, "Extensions List", _("Extensions List"), NULL, 0, 0);
	addmenulist(&mlist, "Auto Resolution", _("Auto Resolution"), NULL, 0, 0);
	if(checkemu() == 1)
		addmenulist(&mlist, "Softcam Panel", _("Softcam Panel"), NULL, 0, 0);
	addmenulist(&mlist, "TV / Radio Switch", _("TV / Radio Switch"), NULL, 0, 0);
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

	char* redkey = getconfig("redkey", NULL);
	debug(60, "Redkey: %s (default)", redkey);
	
	setmenulistdefault(mlist, redkey);
	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your plugin / menu item from the following list and show it so the red button"), NULL, NULL, 1, 0);

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
	return;
}

#endif
