#ifndef SETTINGS_MEDIABUTTON_H
#define SETTINGS_MEDIABUTTON_H

void screensettings_mediabutton()
{
	char* skintitle = _("Media Key Action - Setup");
	struct menulist* mlist = NULL, *mbox = NULL;

// needed for translation
	char* tmptxt = NULL;
	tmptxt = ostrcat(tmptxt, _("MediaCenter"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("VideoPlayer"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("AudioPlayer"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("PicturePlayer"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("MediaThek"), 1, 0);
	tmptxt = ostrcat(tmptxt, _("MiniPlayer"), 1, 0);
	free(tmptxt), tmptxt = NULL;

	addmenulist(&mlist, "RecordPlayer", NULL, NULL, 0, 0);

	while(child != NULL)
	{
		if(child->del == PLUGINDELMARK && (status.security == 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
		{
			if(!ostrncmp("Media Center", child->name, 12))
			{
				addmenulist(&mlist, "MediaCenter", NULL, child->pic, 0, 0);			
			}
			else if(!ostrncmp("TiTan Mediathek", child->name, 15))
			{
				addmenulist(&mlist, "MediaThek", NULL, child->pic, 0, 0);
			}
			else if(!ostrncmp("Titan Media Center", child->name, 15))
			{
				addmenulist(&mlist, "TitanMediaCenter", NULL, child->pic, 0, 0);
			}
		}
		child = child->next;
	}
	
	addmenulist(&mlist, "MiniPlayer", NULL, NULL, 0, 0);

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
