#ifndef EXTENSIONS_H
#define EXTENSIONS_H

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

	//for devs, who have secret feed not in mind
	if(ostrcmp("999.999.999.999", lastline) == 0)
	{
		free(lastline); lastline = NULL;
		lastline = ostrcat("097.074.032.010", NULL, 0, 0);
	}

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
		tmpstr = ostrcat(tmpstr, "/svn/tpk/sh4", 1, 0);
		writesys(getconfig("feed", NULL), tmpstr, 0);
	}

	free(tmpstr);
	free(line);
	free(lastline);
}

//flag 0: install ok
//flag 1: install err
//flag 2: remove ok
//flag 3: remove err
char* gettpklog(char* installpath, int flag)
{
	char* tmpstr = NULL;

	if(flag == 0 || flag == 1)
		tmpstr = ostrcat(tmpstr, _("Installation start"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("Remove start"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
	
	tmpstr = ostrcat(tmpstr, readfiletomem(TPKLOG, 0), 1, 1);
	
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
	
	if(installpath != NULL)
	{
		tmpstr = ostrcat(tmpstr, _("New free space (KB): "), 1, 0);
		tmpstr = ostrcat(tmpstr, ollutoa(getfreespace(installpath) / 1024), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	}
	
	if(flag == 0)
		tmpstr = ostrcat(tmpstr, _("Install success"), 1, 0);
	if(flag == 1)
		tmpstr = ostrcat(tmpstr, _("Install error"), 1, 0);
	if(flag == 2)
		tmpstr = ostrcat(tmpstr, _("Remove success"), 1, 0);
	if(flag == 3)
		tmpstr = ostrcat(tmpstr, _("Remove error"), 1, 0);
		
	if(installpath == NULL || ostrcmp("/var/swap", installpath) == 0)
		sync();
	
	return tmpstr;
}

char* getinstallpath(char* path, char* size)
{
	int count = 0, isize = 0;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmlist = NULL;
	
	if(size != NULL) isize = atoi(size);
	
	if(path == NULL || path[0] == '*' || ostrstr(path, "mnt") != NULL)
	{
		if(tpkchecksize(NULL, "/mnt/swapextensions", isize) == 0)
		{
			tmpmlist = addmenulist(&mlist, "Install to FLASH", NULL, NULL, 0, 0);
			changemenulistparam(tmpmlist, "/mnt/swapextensions", NULL, NULL, NULL);
			free(tmpstr); tmpstr = NULL;
			tmpstr = ostrcat("/mnt/swapextensions", NULL, 0, 0);
			count++;
		}
	}
	
	if(path == NULL || path[0] == '*' || ostrstr(path, "var") != NULL)
	{
		if(tpkchecksize(NULL, "/var", isize) == 0)
		{
			tmpmlist = addmenulist(&mlist, "Install to MNT", NULL, NULL, 0, 0);
			changemenulistparam(tmpmlist, "/var", NULL, NULL, NULL);
			free(tmpstr); tmpstr = NULL;
			tmpstr = ostrcat("/var", NULL, 0, 0);
			count++;
		}
	}
	
	if(path == NULL || path[0] == '*' || ostrstr(path, "swap") != NULL)
	{
		if(file_exist("/tmp/.swapextensionsdev") == 1)
		{
			if(tpkchecksize(NULL, "/var/swap", isize) == 0)
			{
				tmpmlist = addmenulist(&mlist, "Install to SWAP", NULL, NULL, 0, 0);
				changemenulistparam(tmpmlist, "/var/swap", NULL, NULL, NULL);
				free(tmpstr); tmpstr = NULL;
				tmpstr = ostrcat("/var/swap", NULL, 0, 0);
				count++;
			}
		}
	}
	
	//if(count > 1) // if only 1 installpath, don't show choicebox
	if(count > 0) // show always choicebox
	{
		free(tmpstr); tmpstr = NULL;
		mbox = menulistbox(mlist, "tpkinstallinfo", "Choice Install Medium", NULL, NULL, 0, 0);
		if(mbox != NULL)
			tmpstr = ostrcat(mbox->param, NULL, 0, 0);
	}
	else if(count == 0)
	{
		textbox(_("Tpk Install Info"), _("Can't install Package. Package to big."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
		free(tmpstr); tmpstr = NULL;
	}
	
	freemenulist(mlist, 0); mlist = NULL;
	return tmpstr;
}

void screenextensions(int mode, char* path, char* defentry, int first)
{
	char* tmpstr = NULL, *tmpinfo = NULL, *installpath = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	struct menulist* mlist1 = NULL, *mbox1 = NULL;
	struct skin* load = getscreen("loading");
		
	status.hangtime = 99999;
	unlink(TPKLOG);
	
	if(mode == 0)
	{
		drawscreen(load, 0, 0);

		if(first == 1) tpkgetindex(0);
		tpklist();

		clearscreen(load);

		mbox = tpkmenulist(mlist, NULL, "Tpk Install - select section", NULL, NULL, 1, defentry, 0);

		if(mbox != NULL)
		{
			debug(130, "section: %s", mbox->name);
			mbox1 = tpkmenulist(mlist1, "tpkinstall", "Tpk Install - select file", "/tmp/tpk", mbox->name, 2, NULL, 1);
			
			if(mbox1 != NULL && mbox1->param != NULL && mbox1->param1 != NULL)
			{
				debug(130, "file: %s", mbox1->name);
				
				installpath = getinstallpath(mbox1->param2, mbox1->param3);
        if(installpath != NULL)
        {
					tmpinfo = ostrcat(tmpinfo, _("Installing"), 1, 0);
					tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
					tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
					tmpinfo = ostrcat(tmpinfo, "-", 1, 0);
					tmpinfo = ostrcat(tmpinfo, mbox1->name, 1, 0);
					tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
					tmpinfo = ostrcat(tmpinfo, _("started"), 1, 0);
					tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);
		
					if(textbox(_("Tpk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
					{
						drawscreen(load, 0, 0);
						resettvpic();
						char* log = NULL;
						if(tpkgetpackage(mbox1->param, mbox1->param1, installpath) == 0)
						{
							log = gettpklog(installpath, 0);
							textbox(_("Tpk Install Info - Install OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
						}
						else
						{
							log = gettpklog(installpath, 1);
							textbox(_("Tpk Install Info - Install ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
						}
						textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
						loadplugin();
						free(log), log = NULL;
						unlink(TPKLOG);
						if(file_exist("/tmp/.tpk_needs_reboot"))
						{
							textbox(_("Message"), _("TPK Install done, your system will reboot !"), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
							system("init 6");
						}
					}
				}
			}
		}
		free(installpath); installpath = NULL;
		free(tmpstr); tmpstr = NULL;	
		freemenulist(mlist1, 0); mlist1 = NULL;
		if(mbox != NULL) tmpstr = ostrcat(mbox->name, NULL, 0, 0);
		freemenulist(mlist, 0); mlist = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freetpk();
		if(mbox != NULL) screenextensions(0, path, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(mode == 1)
	{
		tpklistinstalled(0);
		mbox = tpkmenulist(mlist, NULL, "Tpk Remove - select file", NULL, NULL, 1, defentry, 2);
		
		if(mbox != NULL && mbox->param != NULL)
		{
			debug(130, "file: %s", mbox->name);

			tmpinfo = ostrcat(tmpinfo, _("Removeing"), 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Tpk Remove Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
			{
				drawscreen(load, 0, 0);
				resettvpic();
				char* log = NULL;
				if(tpkremove(mbox->param, 0, 0) == 0)
				{
					log = gettpklog(NULL, 2);
					textbox(_("Tpk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
					//del plugin from memory if Titanname is defined in plugin control file
					if(mbox->param1 != NULL && mbox->param1[0] != '*') delplugin(mbox->param1);
				}
				else
				{
					log = gettpklog(NULL, 3);
					textbox(_("Tpk Remove Info - Remove ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
				}
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				free(log); log = NULL;
				unlink(TPKLOG);
				if(file_exist("/tmp/.tpk_needs_reboot"))
				{
					textbox(_("Message"), _("TPK Remove done, your system will reboot !"), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
					system("init 6");
				}
			}
		}
		free(tmpstr); tmpstr = NULL;
		freemenulist(mlist, 0); mlist = NULL;
		if(mbox != NULL) tmpstr = ostrcat(mbox->name, NULL, 0, 0);
		free(tmpinfo); tmpinfo = NULL;
		freetpk();
		if(mbox != NULL) screenextensions(1, path, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(mode == 2)
	{
		char* text1 = "Tpk Tmp Install - select file";
		char* text2 = "Tpk Tmp Info";

		if(path == NULL)
			tmpstr = gettpktmplist("/tmp");
		else
		{
			tmpstr = gettpktmplist(path);
			text1 = "Tpk Media Install - select file";
			text2 = "Tpk Media Info";
		}
    
		if(tmpstr == NULL || strlen(tmpstr) == 0)
		{
			textbox(_("Message"), _("No plugin found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		else
		{
			addmenulistall(&mlist, tmpstr, NULL, 0, defentry);
			mbox = menulistbox(mlist, NULL, text1, NULL, "/skin/plugin.png", 1, 0);
		}
		
		free(tmpstr); tmpstr = NULL;
		
		if(mbox != NULL)
		{
			installpath = getinstallpath(NULL, 0);
			if(installpath != NULL)
			{
				debug(130, "file: %s", mbox->name);
	
				tmpinfo = ostrcat(tmpinfo, _("Installing"), 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, _("started"), 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);
	
				if(textbox(_(text2), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 2)
				{
					char* log = NULL;
					int ret = 0;				
					drawscreen(load, 0, 0);
					resettvpic();				
					if(path == NULL)
					{
						tmpstr = ostrcat(tmpstr, "/tmp", 1, 0);
						tmpstr = ostrcat(tmpstr, "/", 1, 0);
						tmpstr = ostrcat(tmpstr, mbox->name, 1, 0);
						ret = tpkinstall(tmpstr, installpath);
						free(tmpstr); tmpstr = NULL;
					}
					else
					{
						tmpstr = ostrcat(tmpstr, path, 1, 0);
						tmpstr = ostrcat(tmpstr, "/", 1, 0);
						tmpstr = ostrcat(tmpstr, mbox->name, 1, 0);
						ret = tpkinstall(tmpstr, installpath);
						free(tmpstr); tmpstr = NULL;
					}
	
					if(ret != 0) log = gettpklog(installpath, 1);
					if(ret == 0) log = gettpklog(installpath, 0);
					textbox(_(text2), log, "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					free(log); log = NULL;
					unlink(TPKLOG);
					textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
					loadplugin();
					if(file_exist("/tmp/.tpk_needs_reboot"))
					{
						textbox(_("Message"), _("TPK Tmp Install done, your system will reboot !"), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
						system("init 6");
					}
				}
			}
		}
		free(installpath); installpath = NULL;
		free(tmpstr); tmpstr = NULL;
		freemenulist(mlist, 0); mlist = NULL;
		if(mbox != NULL) tmpstr = ostrcat(mbox->name, NULL, 0, 0);
		free(tmpinfo); tmpinfo = NULL;
		if(mbox != NULL) screenextensions(2, path, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(mode == 3)
	{
		drawscreen(load, 0, 0);
		resettvpic();
		if(first == 1)
		{
			if(tpkgetindex(0) != 0)
				textbox(_("Tpk Update Info - Update ERROR"), _("Can't get all TPK index !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		writesys("/tmp/.tpk_upgrade_start", "0", 0);
		if(tpkupdate() != 0)
			textbox(_("Tpk Update Info - Update ERROR"), _("Can't update all packages !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		loadplugin();
		clearscreen(load);
		drawscreen(skin, 0, 0);
		unlink(TPKLOG);

		if(file_exist("/tmp/.tpk_needs_reboot"))
		{
			textbox(_("Message"), _("TPK Upgrade done, your system will reboot !"), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			system("init 6");
		}
		unlink("/tmp/.tpk_upgrade_start");
	}

	if(first == 1) tpkcleantmp(0);
	status.hangtime = getconfigint("hangtime", NULL);
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
				tmpstr1 = gettpktmplist(tmpstr);

				if(tmpstr1 != NULL)
				{
					treffer = 1;
					screenextensions(2, tmpstr, NULL, 1);
				}

				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
			}
			node = node->next;
		}

		if(flag == 1 && treffer == 0)
			textbox(_("Tpk Install Info"), _("No plugin found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
}

#endif
