#ifndef SYSTEM_BACKUP_H
#define SYSTEM_BACKUP_H

void screensystem_backup()
{
	int rcret = 0;
	struct skin* backup = getscreen("system_backup");
	struct skin* listbox = getscreennode(backup, "listbox");
	struct skin *listfield = getscreennode(backup, "listfield");
	struct skin* b_red = getscreennode(backup, "b1");
	struct skin* b_green = getscreennode(backup, "b2");
	struct skin* info = getscreennode(backup, "info");
	struct skin* loading = getscreen("loading");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *infotext = NULL;

	infotext = "You find this Backup with timestamp on /tmp\nConnecting per ftp and copy this backup to your Pc !\nPlease Waiting 2 Min ... and relax\n \nNext infos on LCD Display!";

	changetext(info, _(infotext));
	changetext(b_red, _("Cancel"));
	changetext(b_green, _("Backup"));
	b_red->hidden = NO; b_green->hidden = NO;

	changetitle(backup, _("Create Backup"));

	addscreenrc(backup, listbox);

	if(checkbox("UFS912"))
		changeinput(listfield, "kernel\nfw\nroot\nfull");
	else
		changeinput(listfield, "kernel\nvar\nroot\nfull");

	drawscreen(backup, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(backup, tmp);
		rcret = waitrc(backup, 0, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL)) break;

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(isfile("/sbin/backup.sh"))
			{
				b_red->hidden = YES; b_green->hidden = YES;
				drawscreen(backup, 0, 0);
				if(listbox->select != NULL && listbox->select->ret != NULL)
				{
					drawscreen(loading, 0, 0);
					status.sec = 0; //deaktivate spinner
					tmpstr = ostrcat(tmpstr, "backup.sh ", 1, 0);
					tmpstr = ostrcat(tmpstr, listbox->select->ret, 1, 0);
					system(tmpstr);
					free(tmpstr); tmpstr = NULL;
					clearscreen(loading);
					//should only reached if system call fails
					textbox(_("Message"), _("Backup failed"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
				}
			}
			break;
		}
	}

	infotext = NULL;
	delownerrc(backup);
	clearscreen(backup);
}

#endif
