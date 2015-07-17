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
		rcret = waitrc(backup, 0, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->ret != NULL)
			{
				drawscreen(loading, 0, 0);
				tmpstr = create_backup(listbox->select->ret, 1);
				clearscreen(loading);
				changetext(info, tmpstr);
				sleep(30);
			}
		}
	}

//	infotext = NULL;
	free(tmpstr), tmpstr = NULL;
	free(infotext), infotext = NULL;

	delownerrc(backup);
	clearscreen(backup);
}

#endif
