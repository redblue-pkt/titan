#ifndef ERASESWAP_H
#define ERASESWAP_H

void screeneraseswap()
{
	struct skin* load = getscreen("loading");

	if(textbox(_("Message"), _("Do you really want to erase all datas and backups from Swap?\nThe Box will reboot and the erase will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		debug(40, "Erase-Mtd");
		drawscreen(load, 0, 0);
		system("touch /var/etc/.erasemtd");
		sleep(4);
		clearscreen(load);
		oshutdown(2, 1);
	}       
}

#endif
