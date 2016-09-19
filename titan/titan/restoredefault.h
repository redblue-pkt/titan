#ifndef RESTOREDEFAULT_H
#define RESTOREDEFAULT_H

void screenrestoredefault()
{
	int ret = 0;
	char* tmpstr = NULL;
	struct skin* load = getscreen("loading");

	ret = textbox(_("Message"), _("Really restore default ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	if(ret == 1)
	{
		drawscreen(load, 0, 0);
		tmpstr = getconfig("restorecmd", NULL);
		if(tmpstr != NULL)
		{
			system(tmpstr);
			textbox(_("Message"), _("Receiver reboots now !!!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
			oshutdown(2, 2);;
		}
		else
			textbox(_("Message"), _("Can't restore default"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
	}
	clearscreen(load);
}

void screenrestoredefault_without_channelsettings()
{
	int ret = 0;
	char* tmpstr = NULL;
	struct skin* load = getscreen("loading");

	ret = textbox(_("Message"), _("Really restore default without Channelsettings ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	if(ret == 1)
	{
		drawscreen(load, 0, 0);
		tmpstr = getconfig("restorecmd", NULL);
		if(tmpstr != NULL)
		{
			tmpstr = ostrcat(tmpstr, " without_channelsettings", 1, 0);
			printf("cmd: %s\n", tmpstr);
			system(tmpstr);
			textbox(_("Message"), _("Receiver reboots now !!!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
			oshutdown(2, 2);;
		}
		else
			textbox(_("Message"), _("Can't restore default"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
	}
	clearscreen(load);
}

void screeneraseswap()
{
	struct skin* load = getscreen("loading");

	if(textbox(_("Message"), _("Do you really want to erase all data and backups from Swap?\nThe Box will reboot and the erase will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
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

	addmenulist(&mlist, "Restore default", _("Restore default"), NULL, 0, 0);
	addmenulist(&mlist, "Restore default without Channelsettings", _("Restore default without Channelsettings"), NULL, 0, 0);
	addmenulist(&mlist, "Format MNT with Backup/Restore", _("Format MNT with Backup/Restore"), NULL, 0, 0);
	addmenulist(&mlist, "Format MNT (all)", _("Format MNT (all)"), NULL, 0, 0);

	mbox = menulistbox(mlist, NULL, _("Restore default settings"), _("Choose your Restore Mode from the following list."), NULL, NULL, 1, 0);

	if(mbox != NULL)
	{
		if(ostrcmp(mbox->name, "Restore default") == 0)
		{
			screenrestoredefault();
		}
		else if(ostrcmp(mbox->name, "Restore default without Channelsettings") == 0)
		{
			screenrestoredefault_without_channelsettings();
		}
		else if(ostrcmp(mbox->name, "Format MNT with Backup/Restore") == 0)
		{
			screenbackuperaseswap();
		}
		else if(ostrcmp(mbox->name, "Format MNT (all)") == 0)
		{
			screeneraseswap();
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
}

#endif
