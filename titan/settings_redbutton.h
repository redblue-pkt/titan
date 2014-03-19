#ifndef SETTINGS_REDBUTTON_H
#define SETTINGS_REDBUTTON_H

void screensettings_redbutton()
{
	char* skintitle = _("Red Key Action - Setup");
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;

// needed for translation
	char* tmptxt = NULL;
	tmptxt = ostrcat(tmptxt, _("Extensions List"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Auto Resolution"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Softcam Panel"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Extensions List"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Multi EPG"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Graphic Multi EPG"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Sleep Timer"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Child Protection"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Subchannel"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Downloads"), 1, 0);
	free(tmptxt), tmptxt = NULL;

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
	addmenulist(&mlist, "Downloads", NULL, NULL, 0, 0);

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

	if(ostrcmp(mbox->name, "Extensions List") == 0)
		delconfig("redkey");
	else
		addconfig("redkey", mbox->name);

	freemenulist(mlist, 1); mlist = NULL;
	free(tmpstr); tmpstr = NULL;
	return;
}

#endif
