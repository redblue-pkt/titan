#ifndef SYSTEM_BACKUP_RESTORE_H
#define SYSTEM_BACKUP_RESTORE_H

void screensystem_backup_restore()
{
	int rcret = 0, ret = 0;
	struct skin* backup_restore = getscreen("system_backup_restore");
	struct skin* b_red = getscreennode(backup_restore, "b1");
	struct skin* b_green = getscreennode(backup_restore, "b2");
	struct skin* b_yellow = getscreennode(backup_restore, "b3");
	struct skin* b_blue = getscreennode(backup_restore, "b4");
	struct skin* info = getscreennode(backup_restore, "info");
	char* tmpstr = NULL, *infotext = NULL, *BACKUPDIR = NULL;

	infotext = _("Press -restore- and your saved settings will be restored from your swapstick / recording hdd! The Box will restart automatically for restoring!\n\nPress -backup- to save your actual settings to swapstick / recording hdd.\nWARNING: The old backup will be deleted!");

	changetext(info, infotext);
	changetitle(backup_restore, _("Backup / Restore Settings"));

	drawscreen(backup_restore, 0, 0);
				
	while(1)
	{
		rcret = waitrc(backup_restore, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
		{
			clearscreen(backup_restore);
			if(textbox(_("Restore Settings"), _("Are you sure you want to restore settings?\n\nOK = start restore\nEXIT = abort restore"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 700, 250, 0, 0) == 1)
			{
				changetext(info, _("Please wait ...\n\nAll Settings are restored.\n\nBox will start in few seconds."));
				drawscreen(backup_restore, 0, 0);
#ifdef OEBUILD
				char* BACKUPDIR = NULL;
				if(isfile("/media/.moviedev") || file_exist("/media/hdd"))
					BACKUPDIR = ostrcat("/media/hdd", NULL, 0, 0);
				else if(isfile("/media/.backupdev") || file_exist("/var/backup"))
					BACKUPDIR = ostrcat("/var/backup", NULL, 0, 0);
				else if(isfile("/media/.swapextensionsdev") || file_exist("/var/swap"))
					BACKUPDIR = ostrcat("/var/swap", NULL, 0, 0);
				if(BACKUPDIR != NULL)
#else
				if(isfile("/tmp/.backupdev") || file_exist("/var/backup"))
#endif
				{
#ifdef OEBUILD
					tmpstr = ostrcat("/sbin/settings.sh restore ", BACKUPDIR, 0, 0);
					tmpstr = ostrcat(tmpstr, " > /tmp/backup.log 2>&1", 1, 0);
					printf("cmd: %s\n", tmpstr);
					ret = system(tmpstr);
					free(tmpstr), tmpstr = NULL;
#else
					ret = system("/sbin/settings.sh restore > /tmp/backup.log 2>&1");
#endif
					if(ret != 0)
						textbox(_("Message"), _("Restore failed, see log"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
					else
					{
#ifdef OEBUILD
						tmpstr = ostrcat(_("Backup restored successfully"), "\n\n", 0, 0);
						tmpstr = ostrcat(tmpstr, BACKUPDIR, 1, 0);		
						textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
						free(tmpstr); tmpstr = NULL;
#else
						textbox(_("Message"), _("Backup restored successfully"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
#endif

						if(textbox(_("Message"), _("Update Plugins to new Version?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
							screenextensions(3, NULL, NULL, 1); 						if(textbox(_("Message"), _("Update Plugins to new Version?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)

						ret = system("init 6");
					}
					free(BACKUPDIR), BACKUPDIR = NULL;
				}
				else
				{
					//clearscreen(backup_restore);
					textbox(_("RESTORE ERROR"), _("No backup folders found!!!\n\nAborting restoring..."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					changetext(info, _(infotext));
					drawscreen(backup_restore, 0, 0);
				}
			}
			else
			{
				drawscreen(backup_restore, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
#ifdef OEBUILD
			if(isfile("/media/.moviedev") || file_exist("/media/hdd"))
				BACKUPDIR = ostrcat("/media/hdd", NULL, 0, 0);
			else if(isfile("/media/.backupdev") || file_exist("/var/backup"))
				BACKUPDIR = ostrcat("/var/backup", NULL, 0, 0);
			else if(isfile("/media/.swapextensionsdev") || file_exist("/var/swap"))
				BACKUPDIR = ostrcat("/var/swap", NULL, 0, 0);
			if(BACKUPDIR != NULL)
#else
			if(isfile("/tmp/.backupdev") || file_exist("/var/backup"))
#endif
			{
				changetitle(backup_restore, _("Backup - Settings"));
				changetext(info, _("Backup started\n\nPlease wait..."));
				info->textposx = 50;
				b_red->hidden = YES; b_green->hidden = YES; b_yellow->hidden = YES; b_blue->hidden = YES;
				drawscreen(backup_restore, 0, 0);

				writeallconfig(1);
#ifdef OEBUILD
				tmpstr = ostrcat("/sbin/settings.sh backup ", BACKUPDIR, 0, 0);
				tmpstr = ostrcat(tmpstr, " > /tmp/backup.log 2>&1", 1, 0);
				printf("cmd: %s\n", tmpstr);
				ret = system(tmpstr);
				free(tmpstr), tmpstr = NULL;
#else
				ret = system("/sbin/settings.sh backup > /tmp/backup.log 2>&1");
#endif
				changetitle(backup_restore, _("Backup / Restore Settings"));
				changetext(info, _(infotext));
				info->textposx = 0;
				b_red->hidden = NO; b_green->hidden = NO; b_yellow->hidden = NO; b_blue->hidden = NO;
				if(ret != 0)
					textbox(_("Message"), _("Backup failed, see log"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
				else
				{
#ifdef OEBUILD
					tmpstr = ostrcat(_("Backup created successfully"), "\n\n", 0, 0);
					tmpstr = ostrcat(tmpstr, BACKUPDIR, 1, 0);

			
					textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
					free(tmpstr); tmpstr = NULL;
#else
					textbox(_("Message"), _("Backup created successfully"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
#endif
				}
				drawscreen(backup_restore, 0, 0);
			}
			else
			{
				clearscreen(backup_restore);
				textbox(_("BACKUP ERROR"), _("A record hdd or a swapstick must be mounted!\n\nAborting backup..."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				drawscreen(backup_restore, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			tmpstr = readfiletomem("/tmp/backup.log", 0);
			textbox(_("BACKUP LOG"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
			free(tmpstr); tmpstr = NULL;
			drawscreen(backup_restore, 0, 0);
		}
	}

	infotext = NULL;
	delownerrc(backup_restore);
	clearscreen(backup_restore);
}
#endif
