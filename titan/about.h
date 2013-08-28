#ifndef ABOUT_H
#define ABOUT_H

int screenabout(void)
{	STARTFUNC
	char *text = NULL, *tmpstr = NULL;
	struct dvbdev* dvbnode = dvbdev;

	text = ostrcat(_("Image"), ": ", 0, 0);
	text = ostrcat(text, PROGNAME, 1, 0);
	text = ostrcat(text, "\n", 1, 0);
	text = ostrcat(text, _("Version"), 1, 0);
	text = ostrcat(text, ": ", 1, 0);
	text = ostrcat(text, OVERSION, 1, 0);
	text = ostrcat(text, "\n", 1, 0);
	text = ostrcat(text, _("Copyright"), 1, 0);
	text = ostrcat(text, ": ", 1, 0);
	text = ostrcat(text, COPYRIGHT, 1, 0);
	text = ostrcat(text, "\n\n", 1, 0);
		
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
	
	char* flog = readfiletomem("/tmp/.firmware.log", 0);
	text = ostrcat(text, flog, 1, 1);

	textbox(_("System-Info"), text, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 600, 0, 0);
	free(text);

	return 0;
}

#endif
