#ifndef EXTENSIONS_H
#define EXTENSIONS_H

//flag 0: inputbox
//flag 1: auto
void screenfeed(int flag)
{
	char* tmpstr = NULL, *line = NULL, *lastline = NULL;
	char* pos = NULL;

	tmpstr = readsys(getconfig("feed", NULL), 4); //line4
	tmpstr = string_replace("src/gz secret http://", "", tmpstr, 1);

	if(tmpstr != NULL)
		pos = strchr(tmpstr, '/');
	if(pos != NULL)
		pos[0] = '\0';

	if(tmpstr == NULL || ostrcmp(tmpstr, "") == 0 || ostrcmp(tmpstr, "\n") == 0)
		tmpstr = ostrcat(tmpstr, "000.000.000.000", 1, 0);

	if(flag == 1)
		lastline = ostrcat("999.999.999.999", NULL, 0, 0);
	else	
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
/////
		if(line[strlen(line) - 1] != '\n')
			line = ostrcat(line, "\n", 1, 0);

		tmpstr = readsys(getconfig("feed", NULL), 3); //line3
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0))
			line = ostrcat(line, "#\n", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;
/////
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
//		tmpstr = ostrcat(tmpstr, "/svn/tpk/sh4", 1, 0);
#ifdef SH4
		tmpstr = ostrcat(tmpstr, "/svn/tpk/nightly-sh4-secret", 1, 0);	
#else
#ifdef ARM
		tmpstr = ostrcat(tmpstr, "/svn/tpk/nightly-arm-secret", 1, 0);	
#else
		tmpstr = ostrcat(tmpstr, "/svn/tpk/nightly-mipsel-secret", 1, 0);	
#endif
#endif
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
			tmpmlist = addmenulist(&mlist, _("Install to MNT"), NULL, NULL, 0, 0);
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
			tmpmlist = addmenulist(&mlist, _("Install to FLASH"), NULL, NULL, 0, 0);
			changemenulistparam(tmpmlist, "/var", NULL, NULL, NULL);
			free(tmpstr); tmpstr = NULL;
			tmpstr = ostrcat("/var", NULL, 0, 0);
			count++;
		}
	}
	
	if(path == NULL || path[0] == '*' || ostrstr(path, "swap") != NULL)
	{
#ifdef OEBUILD
		if(file_exist("/media/.swapextensionsdev") == 1 || (file_exist("/var/swap") && !isdir("/var/swap")))
#else
		if(file_exist("/tmp/.swapextensionsdev") == 1 || file_exist("/var/swap"))
#endif
		{
			if(tpkchecksize(NULL, "/var/swap", isize) == 0)
			{
				tmpmlist = addmenulist(&mlist, _("Install to SWAP"), NULL, NULL, 0, 0);
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
		mbox = menulistbox(mlist, "tpkinstallinfo", _("Choice Install Medium"), NULL, NULL, NULL, 0, 0);
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

		mbox = tpkmenulist(mlist, NULL, _("Tpk Install - select section"), NULL, NULL, 1, defentry, 0);

		if(mbox != NULL)
		{
			debug(130, "section: %s", mbox->name);
			mbox1 = tpkmenulist(mlist1, "tpkinstall", _("Tpk Install - select file"), "/tmp/tpk", mbox->name, 2, NULL, 1);
			
			if(mbox1 != NULL && mbox1->param != NULL && mbox1->param1 != NULL)
			{
				debug(130, "file: %s", mbox1->name);
				if(ostrstr(mbox1->name, _("There may only be one package installed from this section. To install another package from this section, remove the installed one. If the package is not visible after a software update, perform a TPK update to: ")) == NULL)
				{
					installpath = getinstallpath(mbox1->param2, mbox1->param3);
					if(installpath != NULL)
					{
						tmpinfo = ostrcat(tmpinfo, _("Installing"), 1, 0);
						tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
						tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
						tmpinfo = ostrcat(tmpinfo, "-", 1, 0);
						tmpinfo = ostrcat(tmpinfo, mbox1->name, 1, 0);
						tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
						tmpinfo = ostrcat(tmpinfo, _("starting"), 1, 0);
						tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);
			
						if(textbox(_("Tpk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
						{
							drawscreen(load, 0, 0);
							resettvpic();
							char* log = NULL;
							int tpkret = tpkgetpackage(mbox1->param, mbox1->param1, installpath, 0, 0);
							if(tpkret == 0)
							{
								log = gettpklog(installpath, 0);
								textbox(_("Tpk Install Info - Install OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
							}
							else if(tpkret == 2)
							{
								textbox(_("Tpk Install Info - Install ERROR"), _("There may only be one package installed from this section. To install another package from this section, remove the installed one. If the package is not visible after a software update, perform a TPK update to: "), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
							}
							else
							{
								log = gettpklog(installpath, 1);
								textbox(_("Tpk Install Info - Install ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 2);
							}
	//						textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
							loadplugin();
							free(log), log = NULL;
							unlink(TPKLOG);
							if(file_exist("/tmp/.tpk_needs_reboot"))
							{
								unlink("/tmp/.tpk_needs_reboot");
								textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
								//sync usb
								system("sync");
								//write only config file
								writeallconfig(3);
								//gui restart and write no config
								oshutdown(3, 2);
							}
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
		mbox = tpkmenulist(mlist, NULL, _("Tpk Remove - select file"), NULL, NULL, 1, defentry, 2);
		
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
//				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				free(log); log = NULL;
				unlink(TPKLOG);
				if(file_exist("/tmp/.tpk_needs_reboot"))
				{
					unlink("/tmp/.tpk_needs_reboot");
					textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					//sync usb
					system("sync");
					//write only config file
					writeallconfig(3);
					//gui restart and write no config
					oshutdown(3, 2);
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
			mbox = menulistbox(mlist, NULL, text1, NULL, NULL, "/skin/plugin.png", 1, 0);
		}
		
		free(tmpstr); tmpstr = NULL;
		
		if(mbox != NULL)
		{
			installpath = getinstallpath(NULL, 0);
			debug(130, "installpath: %s", installpath);
			if(installpath != NULL)
			{
				debug(130, "file: %s", mbox->name);
	
				tmpinfo = ostrcat(tmpinfo, _("Installing"), 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, _("starting"), 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);
				debug(130, "tmpinfo: %s", tmpinfo);
	
				if(textbox(_(text2), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
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
						debug(130, "tmpstr: %s", tmpstr);
						ret = tpkinstall(tmpstr, installpath, 0);
						free(tmpstr); tmpstr = NULL;
					}
					else
					{
						tmpstr = ostrcat(tmpstr, path, 1, 0);
						tmpstr = ostrcat(tmpstr, "/", 1, 0);
						tmpstr = ostrcat(tmpstr, mbox->name, 1, 0);
						debug(130, "tmpstr: %s", tmpstr);
						ret = tpkinstall(tmpstr, installpath, 0);
						free(tmpstr); tmpstr = NULL;
					}
	
					if(ret == 0) 
						log = gettpklog(installpath, 0);
					else if(ret == 2)
						log = ostrcat(_("There may only be one package installed from this section. To install another package from this section, remove the installed one. If the package is not visible after a software update, perform a TPK update to: "), NULL, 0, 0);
					else
						log = gettpklog(installpath, 1);
					textbox(_(text2), log, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					free(log); log = NULL;
					unlink(TPKLOG);
					textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
					loadplugin();
					if(file_exist("/tmp/.tpk_needs_reboot"))
					{
						unlink("/tmp/.tpk_needs_reboot");
						textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
						//sync usb
						system("sync");
						//write only config file
						writeallconfig(3);
						//gui restart and write no config
						oshutdown(3, 2);
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
		if(tpkupdate(0) != 0)
			textbox(_("Tpk Update Info - Update ERROR"), _("Can't update all packages !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		loadplugin();
		clearscreen(load);
		drawscreen(skin, 0, 0);
		unlink(TPKLOG);

		if(file_exist("/tmp/.tpk_needs_reboot"))
		{
			unlink("/tmp/.tpk_needs_reboot");
			textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			//sync usb
			system("sync");
			//write only config file
			writeallconfig(3);
			//gui restart and write no config
			oshutdown(3, 2);
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

	if(status.security >= 1)
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

#if defined(OVBUILD) || defined (OEBUILD)
void screenfeed_ipk(int flag)
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
		tmpstr = ostrcat(tmpstr, "/svn/ipk/sh4/titan", 1, 0);
		writesys(getconfig("feed", NULL), tmpstr, 0);
	}

	free(tmpstr);
	free(line);
	free(lastline);
}

// mode=0 install 
// mode=1 remove 
// mode=2 tmp install 
// mode=3 upgrade 

void screenextensions_ipk(int mode, char* path, char* defentry, int first)
{
	char* tmpstr = NULL, *tmpinfo = NULL, *installpath = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	struct menulist* mlist1 = NULL, *mbox1 = NULL;
	struct skin* load = getscreen("loading");
		
	status.hangtime = 99999;
	
//    needed for usepath
    delallfiles(IPKGTMP, ".png");

	if(mode == 0)
	{
		drawscreen(load, 0, 0);

		if(first == 1) ipkggetindex(0);

		ipkg_update();
		ipkg_list();

		clearscreen(load);

		mbox = ipkmenulist(mlist, NULL, _("Ipk Install - select section"), NULL, NULL, 1, defentry, 0);

		if(mbox != NULL)
		{
			debug(130, "section: %s", mbox->name);
//			mbox1 = ipkmenulist(mlist1, "ipkinstall", "Ipk Install - select file", "/tmp/preview", mbox->name, 2, NULL, 1);
			mbox1 = ipkmenulist(mlist1, "ipkinstall", "Ipk Install - select file", IPKGTMP, mbox->name, 2, NULL, 1);
			
			if(mbox1 != NULL)
			{
				debug(130, "file: %s", mbox1->name);
//				tmpstr = ostrcat("titan-plugin-", mbox->name, 0, 0);
//				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, mbox1->param, 1, 0);
				debug(130, "tmpstr: %s", tmpstr);

//					installpath = getinstallpath(mbox1->param2, mbox1->param3);
//					if(installpath != NULL)
//					{
//              char* getinstallpath(char* path, char* size)
//			    changemenulistparam(tmpmlist, node->filename, node->url, node->usepath, size);

//				installpath = getinstallpath(node->usepath, size);

//				installpath = getinstallpath(NULL, 0);
//				installpath = getinstallpath(node->usepath, 0);

printf("mbox1->param2: %s\n", mbox1->param2);
printf("mbox1->param3: %s\n", mbox1->param3);

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

				    if(textbox(_("Ipk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
				    {
					    drawscreen(load, 0, 0);
					    resettvpic();
					    char* log = NULL;

					    log = get_ipk_install(tmpstr, installpath);

			            if(log == NULL) log = ostrcat("Install error", NULL, 0, 0);
			            textbox(_("Ipk Install Info - Install OK"), _(log), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
        		        free(log); log = NULL;
    /*
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
    */
    //					textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);

				        loadplugin();
				        free(log), log = NULL;
        				unlink(IPKGLOG);
				        if(file_exist("/tmp/.ipkg_needs_reboot"))
				        {
					        unlink("/tmp/.ipkg_needs_reboot");
					        textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					        //sync usb
					        system("sync");
					        //write only config file
					        writeallconfig(3);
					        //gui restart and write no config
					        oshutdown(3, 2);
				        }
				    }
                }
			}
		}
/*
		freemenulist(mlist1, 1); mlist1 = NULL;
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenextensions_ipk(0, path);
*/

		free(installpath); installpath = NULL;
		free(tmpstr); tmpstr = NULL;	
		freemenulist(mlist1, 0); mlist1 = NULL;
		if(mbox != NULL) tmpstr = ostrcat(mbox->name, NULL, 0, 0);
		freemenulist(mlist, 0); mlist = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenextensions_ipk(0, path, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(mode == 1)
	{
		ipkg_list_installed();
		mbox = ipkmenulist(mlist, NULL, _("Ipk Remove - select file"), NULL, NULL, 1, defentry, 2);

		if(mbox != NULL)
		{
			debug(130, "file: %s", mbox->name);

			tmpstr = ostrcat("titan-plugin-", mbox->name, 0, 0);

			tmpinfo = ostrcat(tmpinfo, _("Removeing"), 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, _("started"), 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Ipk Remove Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
			{
				drawscreen(load, 0, 0);
				resettvpic();
				char* log = NULL;
				log = get_ipk_remove(tmpstr);

				if(log == NULL) log = ostrcat("Remove error", NULL, 0, 0);
				textbox(_("Ipk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
		        free(log); log = NULL;
/*
				if(ipkg_remove(tmpstr, 1) == 0)
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					if(log == NULL) log = ostrcat("Remove success", NULL, 0, 0);
					textbox(_("Ipk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
				else
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					if(log == NULL) log = ostrcat("Remove error", NULL, 0, 0);
					textbox(_("Ipk Remove Info - Remove ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
*/
//				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
                loadplugin();
				free(log); log = NULL;
				unlink(IPKGLOG);
				if(file_exist("/tmp/.ipkg_needs_reboot"))
				{
					unlink("/tmp/.ipkg_needs_reboot");
					textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					//sync usb
					system("sync");
					//write only config file
					writeallconfig(3);
					//gui restart and write no config
					oshutdown(3, 2);
				}
			}
		}
/*
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenextensions_ipk(1, path);
*/
		free(tmpstr); tmpstr = NULL;
		freemenulist(mlist, 0); mlist = NULL;
		if(mbox != NULL) tmpstr = ostrcat(mbox->name, NULL, 0, 0);
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenextensions_ipk(1, path, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(mode == 2)
	{
		char* text1 = "Ipk Tmp Install - select file";
		char* text2 = "Ipk Tmp Info";

		if(path == NULL)
			tmpstr = get_ipk_tmplistinstall("/tmp");
		else
		{
			tmpstr = get_ipk_tmplistinstall(path);
			text1 = "Ipk Media Install - select file";
			text2 = "Ipk Media Info";
		}
    
		if(tmpstr == NULL || strlen(tmpstr) == 0)
		{
			textbox(_("Message"), _("No plugin found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		else
		{
			addmenulistall(&mlist, tmpstr, NULL, 0, defentry);
			mbox = menulistbox(mlist, NULL, text1, NULL, NULL, "/skin/plugin.png", 1, 0);
		}
		
		free(tmpstr); tmpstr = NULL;
		
		if(mbox != NULL)
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
				drawscreen(load, 0, 0);
				resettvpic();				
				if(path == NULL)
					log = get_ipk_tmpinstall("/tmp", mbox->name);
				else
					log = get_ipk_tmpinstall(path, mbox->name);

				if(log == NULL) log = ostrcat("No output found !", NULL, 0, 0);
				textbox(_(text2), log, "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);

				free(log); log = NULL;
				unlink(IPKGLOG);
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
				loadplugin();
				if(file_exist("/tmp/.ipkg_needs_reboot"))
				{
					unlink("/tmp/.ipkg_needs_reboot");
					textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					//sync usb
					system("sync");
					//write only config file
					writeallconfig(3);
					//gui restart and write no config
					oshutdown(3, 2);
				}

			}
		}
/*
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		if(mbox != NULL) screenextensions_ipk(2, path);
*/
//		free(installpath); installpath = NULL;
		free(tmpstr); tmpstr = NULL;
		freemenulist(mlist, 0); mlist = NULL;
		if(mbox != NULL) tmpstr = ostrcat(mbox->name, NULL, 0, 0);
		free(tmpinfo); tmpinfo = NULL;
		if(mbox != NULL) screenextensions_ipk(2, path, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(mode == 3)
	{
		drawscreen(load, 0, 0);
		resettvpic();
		unlink(IPKGLOG);
		writesys("/tmp/.ipkg_upgrade_start", "0", 0);
		ipkg_update();
//		ipkg_upgrade();
		freeipkg();
		loadplugin();
		clearscreen(load);
		drawscreen(skin, 0, 0);

		tmpinfo = ostrcat(tmpinfo, _("Do you really want to start the Upgrade?"), 1, 0);

		if(textbox(_("Ipk Upgrade"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
		{
			drawscreen(load, 0, 0);
			resettvpic();
			char* log = NULL;
			log = ipkg_upgrade_cmd();

			if(log == NULL) log = ostrcat("Upgrade error", NULL, 0, 0);
			textbox(_("Ipk Upgrade Info - Upgrade OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
	        free(log); log = NULL;
/*
			if(ipkg_remove(tmpstr, 1) == 0)
			{
				log = readfiletomem("/tmp/ipkg.log", 0);
				if(log == NULL) log = ostrcat("Remove success", NULL, 0, 0);
				textbox(_("Ipk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
			}
			else
			{
				log = readfiletomem("/tmp/ipkg.log", 0);
				if(log == NULL) log = ostrcat("Remove error", NULL, 0, 0);
				textbox(_("Ipk Remove Info - Remove ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
			}
*/
//				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
            loadplugin();
			free(log); log = NULL;
			unlink(IPKGLOG);
			if(file_exist("/tmp/.ipkg_needs_reboot"))
			{
				unlink("/tmp/.ipkg_needs_reboot");
				textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				//sync usb
				system("sync");
				//write only config file
				writeallconfig(3);
				//gui restart and write no config
				oshutdown(3, 2);
			}
		}
		free(tmpinfo); tmpinfo = NULL;

		unlink("/tmp/.ipkg_upgrade_start");
	}
	status.hangtime = getconfigint("hangtime", NULL);
}

//flag 0: without message
//flag 1: with message
void screenextensions_check_ipk(int flag)
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
                    screenextensions_ipk(2, tmpstr, NULL, 1);
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

#endif
