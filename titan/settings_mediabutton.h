#ifndef SETTINGS_MEDIABUTTON_H
#define SETTINGS_MEDIABUTTON_H

void screensettings_mediabutton()
{
	char* skintitle = _("Media Key Action - Setup");
	struct menulist* mlist = NULL, *mbox = NULL;
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;

	addmenulist(&mlist, "Media Plugins List", _("Media Plugins List"), NULL, 0, 0);
	addmenulist(&mlist, "Record Player", _("Record Player"), NULL, 0, 0);

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK && (status.security >= 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
		{
			if(ostrcmp(child->name, "Media Center") == 0)
				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);			
			else if(ostrcmp(child->name, "Titan Mediathek") == 0)
				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
			else if(ostrcmp(child->name, "Titan Media Center") == 0)
				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
			else if(ostrcmp(child->name, "GmediaRender") == 0)
				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
			else if(ostrcmp(child->name, "DVD Player") == 0)
				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
			else if(ostrcmp(child->name, "KodiStarter") == 0)
				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
		}
		child = child->next;
	}
	
	addmenulist(&mlist, "Media Player", _("Media Player"), NULL, 0, 0);

	char* mediakey = getconfig("mediakey", NULL);
	debug(60, "Mediakey: %s (default)", mediakey);
	
	setmenulistdefault(mlist, mediakey);
	mbox = menulistbox(mlist, NULL, skintitle, _("Select a function for the Media button"), NULL, NULL, 1, 0);

	if(mbox == NULL)
	{
		return;
	}
	debug(60, "(new) Mediakey=%s", mbox->name);

	addconfig("mediakey", mbox->name);

	freemenulist(mlist, 1); mlist = NULL;
	return;
}

#endif
