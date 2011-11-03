#ifndef AAFPANEL_SYSTEM_RESTORE_H
#define AAFPANEL_SYSTEM_RESTORE_H

void screenaafpanel_system_restore()
{
	int rcret = 0;
	struct skin* aaf_restore = getscreen("aafpanel_system_restore");
	struct skin* b_red = getscreennode(aaf_restore, "b1");
	struct skin* b_green = getscreennode(aaf_restore, "b2");
	struct skin* b_yellow = getscreennode(aaf_restore, "b3");
	struct skin* info = getscreennode(aaf_restore, "info");
	char* tmpstr = NULL, *infotext = NULL;

	infotext = "Press -restore- and your saved settings will be restored from your swapstick / recording hdd! The Box will restart automatically for restoring!\n\nPress -backup- to save your actual settings to swapstick / recording hdd.\nWARNING: The old backup will be deleted!\n\nWhile using the update function of the AAF Panel, your settings will be saved and restored automatically, if possible!";

	changetext(info, _(infotext));
	changetitle(aaf_restore, _("Backup / Restore Settings"));

	drawscreen(aaf_restore, 0);

	while(1)
	{
		rcret = waitrc(aaf_restore, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL)) break;

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(aaf_restore);
			if(textbox(_("Restore Settings"), _("Are you sure you want to restore settings?\n\nOK = start restore\nEXIT = abort restore\n\n"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 700, 250, 0, 0) == 2)
			{
				if(isfile("/var/backup/.actbackup"))
				{
					FILE *fd; fd=fopen("/var/backup/.firstrun", "w"); fclose(fd);
					oshutdown(2, 1);
				}
				else if(isfile("/tmp/.backupdev"))
				{
					//this is a small workaround until the file .actbackup exists everywhere....
					FILE *fd; fd=fopen("/var/backup/.firstrun", "w"); fclose(fd);
					oshutdown(2, 1);
				}
				else
				{
					//clearscreen(aaf_restore);
					textbox(_("RESTORE ERROR"), _("No backup folders found!!!\n\nAborting restoring..."), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(aaf_restore, 0);
				}
			}else
			{
				drawscreen(aaf_restore, 0);
			}
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(isfile("/tmp/.backupdev"))
			{
				changetitle(aaf_restore, _("Backup - Settings"));
				changetext(info, _("Backup started\n\nPlease wait..."));
				info->textposx = 50;
				b_red->hidden = YES; b_green->hidden = YES; b_yellow->hidden = YES;
				drawscreen(aaf_restore, 0);

				writeallconfig(1);
				tmpstr = command("/sbin/settings.sh titan backup");
				free(tmpstr); tmpstr = NULL;

				changetitle(aaf_restore, _("Backup / Restore Settings"));
				changetext(info, _(infotext));
				info->textposx = 0;
				b_red->hidden = NO; b_green->hidden = NO; b_yellow->hidden = NO;
				drawscreen(aaf_restore, 0);
			}
			else
			{
				clearscreen(aaf_restore);
				textbox(_("BACKUP ERROR"), _("A record hdd or a swapstick must be mounted!\n\nAborting backup..."), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				drawscreen(aaf_restore, 0);
			}
		}
	}

	infotext = NULL;
	delownerrc(aaf_restore);
	clearscreen(aaf_restore);
}
#endif
