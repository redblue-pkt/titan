#ifndef SETTINGS_REDBUTTON_H
#define SETTINGS_REDBUTTON_H

void screensettings_redbutton()
{
	char* skintitle = _("Red Key Action - Setup");
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	
	addmenulist(&mlist, _("Extensions List"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Auto Resolution"), NULL, NULL, 0, 0);
	if(checkemu() == 1)
		addmenulist(&mlist, _("Softcam Panel"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("TV / Radio Switch"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Multi EPG"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Graphic Multi EPG"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Sleep Timer"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Child Protection"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Subchannel"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Downloads"), NULL, NULL, 0, 0);

	while(child != NULL)
	{
	if(child->del == PLUGINDELMARK && (status.security == 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
			addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
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

	if(ostrcmp(mbox->name, _("Extensions List")) == 0)
		delconfig("redkey");
	else
		addconfig("redkey", mbox->name);

	freemenulist(mlist, 1); mlist = NULL;
	free(tmpstr); tmpstr = NULL;
	return;
}

#endif
