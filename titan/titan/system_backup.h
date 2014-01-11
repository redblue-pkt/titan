#ifndef SYSTEM_BACKUP_H
#define SYSTEM_BACKUP_H

void screensystem_backup()
{
	int rcret = 0;
	struct skin* backup = getscreen("system_backup");
	struct skin* listbox = getscreennode(backup, "listbox");
	struct skin *listfield = getscreennode(backup, "listfield");
	struct skin* info = getscreennode(backup, "info");
	struct skin* loading = getscreen("loading");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *infotext = NULL;

	infotext = "Backup to /tmp or /var/backup. If the free memory too small can a usb device will never use.\nThere must be a folder backup.";

	changetext(info, _(infotext));
	changetitle(backup, _("Create Backup"));

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
		rcret = waitrc(backup, 0, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->ret != NULL)
			{
				drawscreen(loading, 0, 0);
				status.sec = 0; //deaktivate spinner
				tmpstr = ostrcat(tmpstr, "backup.sh ", 1, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->ret, 1, 0);

				if(checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("UFS912") == 1 || checkbox("ATEMIO7600") == 1)
				{
					if(!file_exist("/mnt/logs"))
						 mkdir("/mnt/logs", 777);
				
					if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
						tmpstr = ostrcat(tmpstr, " > /mnt/logs/backup_debug.log 2>&1", 1, 0);
				}
				else if(file_exist("/var/swap"))
				{
					if(!file_exist("/var/swap/logs"))
						 mkdir("/var/swap/logs", 777);
				
					if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
						tmpstr = ostrcat(cmd, " > /var/swap/logs/backup_debug.log 2>&1", 1, 0);		
				}
	
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;
				clearscreen(loading);
			}
		}
	}

	infotext = NULL;
	delownerrc(backup);
	clearscreen(backup);
}

#endif
