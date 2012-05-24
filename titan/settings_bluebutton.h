#ifndef SETTINGS_BLUEBUTTON_H
#define SETTINGS_BLUEBUTTON_H

void screensettings_bluebutton()
{
	debug(1000, "in");
	char* skintitle = "Blue Key Action - Setup";
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	
	addmenulist(&mlist, "TV / Radio Switch", NULL, NULL, 0, 0);
	if(checkemu() == 1)
		addmenulist(&mlist, "Softcam Panel", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Extensions List", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Multi EPG", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Graphic Multi EPG", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Sleep Timer", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Child Protection", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Subchannel", NULL, NULL, 0, 0);

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK && status.security == 0 && checkpluginskip(child->name) == 0)
			addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
		child = child->next;
	}
	
	char* bluekey = getconfig("bluekey", NULL);
	debug(60, "Bluekey: %s (default)", bluekey);

	setmenulistdefault(mlist, bluekey);
	mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, 1, 0);

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
	debug(1000, "out");
	return;
}

#endif
