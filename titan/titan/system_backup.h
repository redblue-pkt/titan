#ifndef SYSTEM_BACKUP_H
#define SYSTEM_BACKUP_H

struct stimerthread* backupthread = NULL;

void backup_thread()
{
	sleep(4);
	while (backupthread->aktion != STOP)
	{
		sleep(3);
		if(file_exist("/tmp/.backup_ok") == 1)
		{
			textbox(_("Message"), _("Backup created successfully"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 500, 200, 0, 0);
			remove("/tmp/.backup_ok");
			break;
		}
		if(file_exist("/tmp/.backup_start") == 0)
		{
			textbox(_("Message"), _("Backup ended with error"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 500, 200, 0, 0);
			remove("/tmp/.backup_ok");
			break;
		}
	}
	if(backupthread->aktion == STOP)
	{
		system("killall backup.sh");
	}
	backupthread = NULL;
}

void screensystem_backup()
{
	int rcret = 0;
	struct skin* backup = getscreen("system_backup");
	struct skin* listbox = getscreennode(backup, "listbox");
	struct skin *listfield = getscreennode(backup, "listfield");
	struct skin* info = getscreennode(backup, "info");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *infotext = NULL;

	infotext = create_backup(NULL, 0);

	changetext(info, infotext);
//	changetitle(backup, _("Create Backup"));

	addscreenrc(backup, listbox);

//	if(checkbox("UFS912"))
//		changeinput(listfield, "kernel\nfw\nroot\nfull");
//	else
//		changeinput(listfield, "kernel\nvar\nroot\nfull");

	changeinput(listfield, "full");

	drawscreen(backup, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(backup, tmp);
		rcret = waitrc(backup, 1000, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->ret != NULL)
			{
				delownerrc(backup);
				clearscreen(backup);
				backupthread = addtimer(&backup_thread, START, 10000, 1, NULL, NULL, NULL);
				tmpstr = create_backup(listbox->select->ret, 2);
				break;
			}
		}
	}

//	infotext = NULL;
	free(tmpstr), tmpstr = NULL;
	free(infotext), infotext = NULL;
	if(rcret != getrcconfigint("rcred", NULL))
	{
		delownerrc(backup);
		clearscreen(backup);
	}
}



#endif
