#ifndef SYSTEM_BACKUP_RESTORE_H
#define SYSTEM_BACKUP_RESTORE_H

void screensystem_backup_restore()
{
	int rcret = 0, ret = 0;
	struct skin* backup_restore = getscreen("system_backup_restore");
	struct skin* b_red = getscreennode(backup_restore, "b1");
	struct skin* b_green = getscreennode(backup_restore, "b2");
	struct skin* b_yellow = getscreennode(backup_restore, "b3");
	struct skin* info = getscreennode(backup_restore, "info");
	char* tmpstr = NULL;

	infotext = "Press -restore- and your saved settings will be restored from your swapstick / recording hdd! The Box will restart automatically for restoring!\n\nPress -backup- to save your actual settings to swapstick / recording hdd.\nWARNING: The old backup will be deleted!\n\nWhile using the update function, your settings will be saved and restored automatically, if possible!";

	changetext(info, _(infotext));
	changetitle(backup_restore, _("Backup / Restore Settings"));

	drawscreen(backup_restore, 0, 0);

	while(1)
	{
		rcret = waitrc(backup_restore, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL)) break;

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(backup_restore);
			if(textbox(_("Restore Settings"), _("Are you sure you want to restore settings?\n\nOK = start restore\nEXIT = abort restore"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 700, 250, 0, 0) == 1)
			{
				changetext(info, _("Please wait ...\n\nAll Settings are saved.\n\nBox will start in few seconds."));
				drawscreen(backup_restore, 0, 0);
			
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
					//clearscreen(backup_restore);
					textbox(_("RESTORE ERROR"), _("No backup folders found!!!\n\nAborting restoring..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					changetext(info, _(infotext));
					drawscreen(backup_restore, 0, 0);
				}
			}else
			{
				drawscreen(backup_restore, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			tmpstr = readfiletomem("/tmp/backup.log", 0);
			textbox(_("BACKUP LOG"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
			free(tmpstr); tmpstr = NULL;
			drawscreen(backup_restore, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(isfile("/tmp/.backupdev"))
			{
				changetitle(backup_restore, _("Backup - Settings"));
				changetext(info, _("Backup started\n\nPlease wait..."));
				info->textposx = 50;
				b_red->hidden = YES; b_green->hidden = YES; b_yellow->hidden = YES;
				drawscreen(backup_restore, 0, 0);

				writeallconfig(1);
				
				ret = system("/sbin/settings.sh titan backup > /tmp/backup.log 2>&1");
				
				changetitle(backup_restore, _("Backup / Restore Settings"));
				changetext(info, _(infotext));
				info->textposx = 0;
				b_red->hidden = NO; b_green->hidden = NO; b_yellow->hidden = NO;
				if(ret != 0)
					textbox(_("Message"), _("Backup failed, see log"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
				else
					textbox(_("Message"), _("Backup created successfully"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
				drawscreen(backup_restore, 0, 0);
			}
			else
			{
				clearscreen(backup_restore);
				textbox(_("BACKUP ERROR"), _("A record hdd or a swapstick must be mounted!\n\nAborting backup..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				drawscreen(backup_restore, 0, 0);
			}
		}
	}

	infotext = NULL;
	delownerrc(backup_restore);
	clearscreen(backup_restore);
}
#endif
