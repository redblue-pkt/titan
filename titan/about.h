#ifndef ABOUT_H
#define ABOUT_H

int screenabout(void)
{
	char* tmpstr = NULL;
	tmpstr = getabout();

	textbox(_("System-Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
	free(tmpstr);

	return 0;
}

#endif
