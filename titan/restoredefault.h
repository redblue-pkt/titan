#ifndef RESTOREDEFAULT_H
#define RESTOREDEFAULT_H

void screenrestoredefault()
{
	int ret = 0;
	char* tmpstr = NULL;
	struct skin* load = getscreen("loading");

	ret = textbox(_("Message"), _("Realy restore default settings ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	if(ret == 1)
	{
		drawscreen(load, 0, 0);
		tmpstr = getconfig("restorecmd", NULL);
		if(tmpstr != NULL)
		{
			system(tmpstr);
			textbox(_("Message"), _("Receiver now reboot !!!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
			oshutdown(2, 2);;
		}
		else
			textbox(_("Message"), _("Can't restore settings"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
	}
	clearscreen(load);
}

void screeneraseswap()
{
	struct skin* load = getscreen("loading");

	if(textbox(_("Message"), _("Do you really want to erase all datas and backups from Swap?\nThe Box will reboot and the erase will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		debug(40, "Erase-Mtd");
		drawscreen(load, 0, 0);
		system("touch /var/etc/.erasemtd");
		sleep(4);
		clearscreen(load);
		oshutdown(2, 2);
	}
}

void screenbackuperaseswap()
{
	struct skin* load = getscreen("loading");

	if(textbox(_("Message"), _("Do you really want to erase Swap (Settings are preserve)?\nThe Box will reboot and the erase will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		debug(40, "Erase-Mtd with settings restore");
		drawscreen(load, 0, 0);
		system("touch /var/etc/.erasemtd");
		system("touch /var/etc/.backupmtd");
		sleep(4);
		clearscreen(load);
		oshutdown(2, 2);
	}
}

void screenrestoredefaultchoice()
{
	struct menulist* mlist = NULL, *mbox = NULL;

	addmenulist(&mlist, "Restore default settings", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Format and restore default settings", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Format and backup/restore settings", NULL, NULL, 0, 0);

	mbox = menulistbox(mlist, NULL, "Restore default settings", NULL, NULL, 1, 0);

	if(mbox != NULL)
	{
		if(ostrcmp(mbox->name, "Restore default settings") == 0)
		{
			screenrestoredefault();
		}
		else if(ostrcmp(mbox->name, "Format and restore default settings") == 0)
		{
			screeneraseswap();
		}
		else if(ostrcmp(mbox->name, "Format and backup/restore settings") == 0)
		{
			screenbackuperaseswap();
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
}

#endif
