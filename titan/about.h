#ifndef ABOUT_H
#define ABOUT_H

int screenabout(void)
{
	char *text = NULL, *tmpstr = NULL;
	struct dvbdev* dvbnode = dvbdev;

	text = malloc(MINMALLOC);
	if(text == NULL)
	{
		err("no memory");
		return 1;
	}

	tmpstr = ostrcat(_("Image"), ": ", 0, 0);
	tmpstr = ostrcat(tmpstr, PROGNAME, 1, 0);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Version"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": ", 1, 0);
	tmpstr = ostrcat(tmpstr, OVERSION, 1, 0);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Copyright"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": ", 1, 0);
	tmpstr = ostrcat(tmpstr, COPYRIGHT, 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
		
//	snprintf(text, MINMALLOC, _("Image: %s\nVersion: %s\nCopyright: %s\n\n"), PROGNAME, OVERSION, COPYRIGHT);
	snprintf(text, MINMALLOC, "%s", tmpstr);
	free(tmpstr), tmpstr = NULL;
		
	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			text = ostrcat(text, _("Tuner"), 1, 0);
			text = ostrcat(text, ": ", 1, 0);
			if(dvbnode->feinfo->name != NULL)
				text = ostrcat(text, dvbnode->feinfo->name, 1, 0);
			else
				text = ostrcat(text, _("unknown"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("Tunertype"), 1, 0);
			text = ostrcat(text, ": ", 1, 0);

			tmpstr = fegettypestr(dvbnode);	
			text = ostrcat(text, tmpstr, 1, 1);
			text = ostrcat(text, "\n\n", 1, 0);
		}
		dvbnode = dvbnode->next;
	}

	textbox(_("System-Info"), text, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 500, 0, 0);
	free(text);

	return 0;
}

#endif
