#ifndef SETTINGS_MEDIABUTTON_H
#define SETTINGS_MEDIABUTTON_H

void screensettings_mediabutton()
{
	char* skintitle = _("Media Key Action - Setup");
	struct menulist* mlist = NULL, *mbox = NULL;
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;

// needed for translation
	char* tmptxt = NULL;
	tmptxt = ostrcat(tmptxt, _("RecordPlayer"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("Media Plugins List"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("MediaPlayer"), 1, 0);
	free(tmptxt), tmptxt = NULL;

	addmenulist(&mlist, "Media Plugins List", NULL, NULL, 0, 0);
	addmenulist(&mlist, "RecordPlayer", NULL, NULL, 0, 0);

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK && (status.security == 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
		{
			if(ostrcmp(child->name, "Media Center") == 0)
			{
				addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);			
			}
			else if(ostrcmp(child->name, "TiTan Mediathek") == 0)
			{
				addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
			}
			else if(ostrcmp(child->name, "Titan Media Center") == 0)
			{
				addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
			}
			else if(ostrcmp(child->name, "GmediaRender") == 0)
			{
				addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
			}
			else if(ostrcmp(child->name, "DVD Player") == 0)
			{
				addmenulist(&mlist, child->name, NULL, child->pic, 0, 0);
			}
		}
		child = child->next;
	}
	
	addmenulist(&mlist, "MediaPlayer", NULL, NULL, 0, 0);

	char* mediakey = getconfig("mediakey", NULL);
	debug(60, "Mediakey: %s (default)", mediakey);
	
	setmenulistdefault(mlist, mediakey);
	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your Media plugin item from the following list and show it so the plugin button"), NULL, NULL, 1, 0);

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
