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

	addmenulist(&mlist, "MediaCenter List", NULL, NULL, 0, 0);
	addmenulist(&mlist, "VideoPlayer", NULL, NULL, 0, 0);
	addmenulist(&mlist, "AudioPlayer", NULL, NULL, 0, 0);
	addmenulist(&mlist, "PicturePlayer", NULL, NULL, 0, 0);
	addmenulist(&mlist, "MediaThek", NULL, NULL, 0, 0);
	addmenulist(&mlist, "MiniPlayer", NULL, NULL, 0, 0);
	addmenulist(&mlist, "RecordPlayer", NULL, NULL, 0, 0);

	char* mediakey = getconfig("mediakey", NULL);
	debug(60, "Mediakey: %s (default)", mediakey);
	
	setmenulistdefault(mlist, mediakey);
	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your Media plugin item from the following list and show it so the plugin button"), NULL, NULL, 1, 0);

	if(mbox == NULL)
	{
		return;
	}
	debug(60, "(new) Mediakey=%s", mbox->name);

	freemenulist(mlist, 1); mlist = NULL;
	return;
}

#endif
