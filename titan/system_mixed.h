#ifndef SYSTEM_MIXED_H
#define SYSTEM_MIXED_H

void screen_system_eraseswap()
{
	if(textbox(_("Message"), _("Do you really want to erase all datas and backups from Swap?\nThe Box will reboot and the erase will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		debug(40, "Erase-Mtd");
		system("touch /var/etc/.erasemtd");
		sleep(4);
		oshutdown(2, 1);
	}       
}

void screen_system_wizard()
{
	if(textbox(_("Message"), _("Do you really want to start the Wizard?\nThe Box will reboot and the Wizard will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		debug(40, "Wizzard");
		system("touch /var/etc/.firstszap");
		system("touch /var/etc/.firstwizard");
		sleep(4);
		oshutdown(2, 1);
	}       
}

#endif
