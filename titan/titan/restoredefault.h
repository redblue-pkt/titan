#ifndef RESTOREDEFAULT_H
#define RESTOREDEFAULT_H

void screenrestoredefault()
{
	int ret = 0;
	char* tmpstr = NULL;

	ret = textbox(_("Message"), _("Realy restore default settings ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	if(ret == 2)
	{
		tmpstr = getconfig("restorecmd", NULL);
		if(tmpstr != NULL)
		{
			system(tmpstr);
			textbox(_("Message"), _("Receiver now reboot !!!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
			oshutdown(3, 2);
		}
		else
			textbox(_("Message"), _("Can't restore settings"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
	}
}

#endif
