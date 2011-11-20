#ifndef PANEL_SYSTEM_BACKUP_H
#define PANEL_SYSTEM_BACKUP_H

void screenpanel_system_backup()
{
	int rcret = 0;
	struct skin* panel_backup = getscreen("panel_system_backup");
	struct skin* listbox = getscreennode(panel_backup, "listbox");
	struct skin *listfield = getscreennode(panel_backup, "listfield");
	struct skin* b_red = getscreennode(panel_backup, "b1");
	struct skin* b_green = getscreennode(panel_backup, "b2");
	struct skin* info = getscreennode(panel_backup, "info");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *infotext = NULL;

	infotext = "You find this Backup with timestamp on /tmp\nConnecting per ftp and copy this backup to your Pc !\nPlease Waiting 2 Min ... and relax\n \nNext infos on LCD Display!";

	changetext(info, _(infotext));
	changetext(b_red, _("Cancel"));
	changetext(b_green, _("Backup"));
	b_red->hidden = NO; b_green->hidden = NO;

	changetitle(panel_backup, _("Create Backup"));

	addscreenrc(panel_backup, listbox);

	if(checkbox("UFS912") || checkbox("AT7500"))
	{
		changeinput(listfield, "kernel\nfw\nroot\nfull");
	}
	else
	{
		changeinput(listfield, "kernel\nvar\nroot\nfull");
	}

	drawscreen(panel_backup, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(panel_backup, tmp);
		rcret = waitrc(panel_backup, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL)) break;

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(isfile("/sbin/backup.sh"))
			{
				b_red->hidden = YES; b_green->hidden = YES;
				drawscreen(panel_backup, 0);
				tmpstr = ostrcat(tmpstr, "backup.sh ", 1, 0);
				tmpstr = ostrcat(tmpstr, getownconfig(listbox->select->name), 1, 0);
				system(tmpstr);
			}
			break;
		}

	}

	infotext = NULL;
	delownerrc(panel_backup);
	clearscreen(panel_backup);
}

#endif
