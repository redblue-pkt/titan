#ifndef LANGUAGE_H
#define LANGUAGE_H

//flag 0: with reboot
//flag 1: without reboot
//flag 2: reload skin
int screenlanguage(int flag)
{
	int rcret = 0, change = 0, reloadskin = 0, check = 0;
	struct skin* language = getscreen("language");
	struct skin* listbox = getscreennode(language, "listbox");

	if(listbox == NULL || language == NULL)
	{
		err("NULL detect");
		return 1;
	}

	setlistboxselection(listbox, getconfig("lang", NULL));

	drawscreen(language, 0, 0);
	addscreenrc(language, listbox);

	while(1)
	{
		rcret = waitrc(language, 0, 0);

		if (rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, getconfig("lang", NULL)) != 0)
			{
				if(ostrcmp(listbox->select->name, "po/en") == 0)
				{
					setlang(listbox->select->name);
					initlocale(getconfig("localepath", NULL));
					addconfig("lang", listbox->select->name);
					change = 1;
				}
				else if(setlang(listbox->select->name) == 0 && initlocale(getconfig("localepath", NULL)) == 0)
				{
					addconfig("lang", listbox->select->name);
					change = 1;
				}

				if(flag == 0 && change == 1)
				{
					check = 1;
					if(checkbox("UFS910") == 1 || checkbox("UFS922") == 1)
					{
						char* file = ostrcat("/mnt/swapextensions/usr/local/share/titan/", listbox->select->name, 0, 0);
						if(!file_exist(file))
							check = 0;
						free(file), file = NULL;
					}
					if(check == 1)
					{
						textbox(_("Message"), _("Change language needs reboot"), NULL, 0, NULL, 0, NULL, 0, NULL, 0, 1000, 200, 5, 0);
						resettvpic();
						oshutdown(3, 1);
					}
					else
						textbox(_("Message"), _("Install language Package"), NULL, 0, NULL, 0, NULL, 0, NULL, 0, 1000, 200, 5, 0);
				}
				if(flag == 2 && change == 1)
					reloadskin = 1;
			}
			break;
		}
	}

	delownerrc(language);
	clearscreen(language);

	if(reloadskin == 1)
	{
		status.skinerr = NULL;
		freescreen();
		if(createstartscreen() != 0) return 2;
		readscreen(getconfig("skinfile", NULL), 0, 0);
	}
	
	return 0;
}

#endif
