#ifndef EXTENSIONS_H
#define EXTENSIONS_H

void saveipkg()
{
	if(checkbox("ATEMIO510") == 0 && checkbox("AT7500") == 0 && checkbox("UFS912") == 0)
		return;

	int err = 0;
	
	system("rm -rf /mnt/ipkg ; mkdir /mnt/ipkg; cp -a /var/usr/lib/ipkg/info /mnt/ipkg; cp /var/usr/lib/ipkg/status /mnt/ipkg");

	if(file_exist("/var/usr/lib/ipkg/info") && !file_exist("/mnt/ipkg/info"))
		err = 1;

	if(file_exist("/var/usr/lib/ipkg/status") && !file_exist("/mnt/ipkg/status"))
		err = 1;

	if(err == 1)
	{
		system("rm -rf /mnt/ipkg");
		textbox(_("Message"), _("Can't backup ipkg files\nPlease check if partition is full"), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
}

void screenfeed()
{
	char* tmpstr = NULL, *line = NULL, *lastline = NULL;
	char* pos = NULL;

	tmpstr = readsys(getconfig("feed", NULL), 3); //line3
	tmpstr = string_replace("src/gz secret http://", "", tmpstr, 1);

	if(tmpstr != NULL)
		pos = strchr(tmpstr, '/');
	if(pos != NULL)
		pos[0] = '\0';

	if(tmpstr == NULL || ostrcmp(tmpstr, "") == 0 || ostrcmp(tmpstr, "\n") == 0)
		tmpstr = ostrcat(tmpstr, "000.000.000.000", 1, 0);

	lastline = numinput(_("Feed"), tmpstr, "000.000.000.000", 1);

	if(lastline != NULL)
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = fixip(lastline, 1);
		free(lastline); lastline = tmpstr;

		tmpstr = readsys(getconfig("feed", NULL), 1); //line1
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0))
			line = ostrcat(line, "#", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;

		if(line[strlen(line) - 1] != '\n')
			line = ostrcat(line, "\n", 1, 0);

		tmpstr = readsys(getconfig("feed", NULL), 2); //line2
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0))
			line = ostrcat(line, "#\n", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;

		if(line[strlen(line) - 1] == '\n')
			tmpstr = ostrcat(line, "src/gz secret http://", 0, 0);
		else
			tmpstr = ostrcat(line, "\nsrc/gz secret http://", 0, 0);

		if(strlen(lastline) == 0)
		{
			free(tmpstr);
			tmpstr = ostrcat(line, NULL, 0, 0);
		}

		tmpstr = ostrcat(tmpstr, lastline, 1, 0);
		tmpstr = ostrcat(tmpstr, "/svn/ipk/sh4/titan", 1, 0);
		writesys(getconfig("feed", NULL), tmpstr, 0);
	}

	free(tmpstr);
	free(line);
	free(lastline);
}

void screenextensions(int mode, char* path)
{
	char* tmpstr = NULL, *tmpinfo = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	struct menulist* mlist1 = NULL, *mbox1 = NULL;
	
	if(mode == 0)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);

		system("cp -a /mnt/ipkg/* /var/usr/lib/ipkg");
		ipkg_update();
		ipkg_list();

		clearscreen(load);

		mbox = ipkmenulist(mlist, NULL, "Ipk Install - select section", NULL, NULL, 1, 0);

		if(mbox != NULL)
		{
			debug(130, "section: %s", mbox->name);
			mbox1 = ipkmenulist(mlist1, "ipkinstall", "Ipk Install - select file", "/tmp/preview", mbox->name, 2, 1);
			
			if(mbox1 != NULL)
			{
				debug(130, "file: %s", mbox1->name);
				tmpstr = ostrcat("titan-plugin-", mbox->name, 0, 0);
				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, mbox1->param, 1, 0);

				tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
				tmpinfo = ostrcat(tmpinfo, "-", 1, 0);
				tmpinfo = ostrcat(tmpinfo, mbox1->name, 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

				if(textbox(_("Ipk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
				{
					char* log = NULL;
					if(ipkg_install(tmpstr) == 0)
					{
						log = readfiletomem("/tmp/ipkg.log", 0);
						if(log == NULL) log = ostrcat("Install success", NULL, 0, 0);
						textbox(_("Ipk Install Info - Install OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					}
					else
					{
						log = readfiletomem("/tmp/ipkg.log", 0);
						if(log == NULL) log = ostrcat("Install error", NULL, 0, 0);
						textbox(_("Ipk Install Info - Install ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					}
					textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
					loadplugin();
					saveipkg();
					free(log), log = NULL;
					unlink("/tmp/ipkg.log");
				}
			}
		}
			
		freemenulist(mlist1, 1); mlist1 = NULL;
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenextensions(0, path);
	}
	else if(mode == 1)
	{
		system("cp -a /mnt/ipkg/* /var/usr/lib/ipkg");
		ipkg_list_installed();
		mbox = ipkmenulist(mlist, NULL, "Ipk Remove - select file", NULL, NULL, 1, 2);
		
		if(mbox != NULL)
		{
			debug(130, "file: %s", mbox->name);

			tmpstr = ostrcat("titan-plugin-", mbox->name, 0, 0);

			tmpinfo = ostrcat(tmpinfo, "Remove ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Remove Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
			{
				char* log = NULL;
				if(ipkg_remove(tmpstr, 1) == 0)
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					if(log == NULL) log = ostrcat("Remove success", NULL, 0, 0);
					textbox(_("Ipk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					saveipkg();
				}
				else
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					if(log == NULL) log = ostrcat("Remove error", NULL, 0, 0);
					textbox(_("Ipk Remove Info - Remove ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				free(log), log = NULL;
				unlink("/tmp/ipkg.log");
			}
		}
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenextensions(1, path);
	}
	else if(mode == 2)
	{
		if(path == NULL)
			tmpstr = get_ipk_tmplistinstall("/tmp");
		else
			tmpstr = get_ipk_tmplistinstall(path);
    
	    if(tmpstr == NULL || strlen(tmpstr) == 0)
	    {
			textbox(_("Message"), _("No plugin found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	    }
	    else
	    {
			addmenulistall(&mlist, tmpstr, NULL, 0, NULL);
			mbox = menulistbox(mlist, NULL, "Ipk Tmp Install - select file", NULL, "/skin/plugin.png", 1, 0);
	    }
		
		free(tmpstr); tmpstr = NULL;
		
		if(mbox != NULL)
		{
			debug(130, "file: %s", mbox->name);

			tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Ipk Tmp Info"), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
			{
				char* log = NULL;

				if(path == NULL)
					log = get_ipk_tmpinstall("/tmp", mbox->name);
				else
					log = get_ipk_tmpinstall(path, mbox->name);

				if(log == NULL) log = ostrcat("No output found !", NULL, 0, 0);
				textbox(_("Ipk Tmp Info"), log, "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
				free(log); log = NULL;
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				loadplugin();
			}
		}
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		if(mbox != NULL) screenextensions(2, path);
	}
	else if(mode == 3)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
		system("cp -a /mnt/ipkg/* /var/usr/lib/ipkg");
		ipkg_update();
		clearscreen(load);
		ipkg_upgrade();
		saveipkg();
		freeipkg();
		loadplugin();
	}
}

//flag 0: without message
//flag 1: with message
void screenextensions_check(int flag)
{
	int treffer = 0;
	struct hdd *node = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	if(status.security == 1)
	{
		addhddall();
		node = hdd;

		while(node != NULL)
		{
			if(node->partition != 0)
			{
				tmpstr = ostrcat("/autofs/", node->device, 0, 0);
				tmpstr1 = get_ipk_tmplistinstall(tmpstr);

				if(tmpstr1 != NULL)
				{
					treffer = 1;
					screenextensions(2, tmpstr);
				}

				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
			}
			node = node->next;
		}

		if(flag == 1 && treffer == 0)
			textbox(_("Ipk Install Info"), _("No plugin found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
}

#endif
