#ifndef SOFTCAM_H
#define SOFTCAM_H

//char* swhichcam = NULL;
struct clist *emulist[LISTHASHSIZE] = {NULL};

char* whichcam()
{
	return command("emu.sh active");
}

int checkrunningcam(char* emu)
{
	char* check = NULL;
	char* cmd = NULL;
	int running = 0;

	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh check \"" , cmd, 0, 1);
	check = command(cmd);
	free(cmd);
	if(ostrstr(check, "checkemu running") != 0) running = 1;
	free(check);
	return running;
}

void readmenu()
{
	char* emuname = NULL;
	char* emuline = NULL;
	char* emush = NULL;
	char* cmd = NULL;
	char* checkactive = NULL;
	char* checkstate = NULL;
	int count = 0;
	int active = 0;
	int check = 0;

	// clear the list
	if(emulist != NULL)
		freelist(emulist);

	// search active cam
	//free(swhichcam); swhichcam = NULL;
	//swhichcam = whichcam();

	// get the emu list
	emuline = command("emu.sh list");
	if(emuline != NULL)
	{
		// split emulist lines
		emush = strtok(emuline, "\n");

		while(emush != NULL)
		{
			active = 0;
			check = 0;

			// read emu infoname
			cmd = string_quote(emush);
			cmd = ostrcat("emu.sh infoname ", cmd, 0, 1);
			emuname = string_newline(command(cmd));
			free(cmd); cmd = NULL;

			// check if cam is active
			cmd = string_quote(emush);
			cmd = ostrcat("emu.sh active ", cmd, 0, 1);
			checkactive = string_newline(command(cmd));
			free(cmd); cmd = NULL;
			if(ostrcmp(emush, checkactive) == 0)
			{
				debug(100, "active emu = %s", emush);
				active = 1;
			}

			// check if emu is running
			cmd = string_quote(emush);
			cmd = ostrcat("emu.sh check ", cmd, 0, 1);
			checkstate = command(cmd);
			free(cmd); cmd = NULL;
			if(ostrstr(checkstate, "checkemu running") != 0)
			{
				debug(100, "running emu = %s", emush);
				check = 1;
			}

			// update emuname with status
			if(active == 1 && check == 1)
			{
				emuname = ostrcat(emuname, "  (", 1, 0);
				emuname = ostrcat(emuname, _("started"), 1, 0);
				emuname = ostrcat(emuname, ") (", 1, 0);
				emuname = ostrcat(emuname, _("active"), 1, 0);
				emuname = ostrcat(emuname, ")", 1, 0);
			}
			else if(check == 1)
			{
				emuname = ostrcat(emuname, "  (", 1, 0);
				emuname = ostrcat(emuname, _("started"), 1, 0);
				emuname = ostrcat(emuname, ")", 1, 0);
			}
			else if(active == 1)
			{
				emuname = ostrcat(emuname, "  (", 1, 0);
				emuname = ostrcat(emuname, _("active"), 1, 0);
				emuname = ostrcat(emuname, ")", 1, 0);
			}

			debug(100, "emuname = %s", emuname);
			addlist(emulist, emuname, emush);
			free(emuname); emuname = NULL;
			free(checkactive); checkactive = NULL;
			free(checkstate); checkstate = NULL;
			emush = strtok(NULL, "\n");
			count++;
		}
	}
	else
		addlist(emulist, _("Emu not found"), "emu0");

	free(emuname);
	free(emuline);
	free(emush);
	free(cmd);
	free(checkactive);
	free(checkstate);
}

void fillmenubox()
{
	struct clist* elist = NULL;
	struct skin* screen = getscreen("softcam");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* node = NULL;
	char* aktemu = NULL;
	int setselection = 0, i;

	delmarkedscreennodes(screen, 1);

	int height = 22;
	height += status.fontsizeadjust;
					
	for(i = 0; i < LISTHASHSIZE; i++)
	{
		elist = emulist[i];

		while(elist != NULL)
		{
			if(elist->value != NULL)
			{
				node = addlistbox(screen, listbox, node, 1);
				if(node != NULL)
				{
					changetext(node, _(elist->key));
					changename(node, elist->value);
					node->height = height;
	
					// change font color, depending emu is running/active
					if(ostrstr(node->text, "active") != 0)
					{
						node->fontcol = convertcol("emuaktivecol");
						if(setselection == 0)
						{
							aktemu = elist->value;
							setselection = 1;
						}
					}
					else if(ostrstr(node->text, "started") != 0)
					{
						node->fontcol = convertcol("emurunningcol");
						if(setselection == 0 || setselection == 1)
						{
							aktemu = elist->value;
							setselection = 2;
						}
					}
					else
						node->fontcol = listbox->fontcol;
				}
			}
			elist = elist->next;
		}
	}
	setlistboxselection(listbox, aktemu);
	debug(100, "set selection to %s", aktemu);
}

void activate(char* emu)
{
	char* cmd = NULL;

	debug(100, "active emu= %s", emu);
	if(emu == NULL) return;

	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh activate \"" , cmd, 0, 1);
	osystem(cmd, 20);
	free(cmd);
	readmenu();
	fillmenubox();
}

void deactivate(char* emu)
{
	char* cmd = NULL;

	debug(100, "deaktivate emu= %s", emu);
	if(emu == NULL) return;

	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh deactivate \"" , cmd, 0, 1);
	osystem(cmd, 20);
	free(cmd);
	readmenu();
	fillmenubox();
}

void startcam(char* emu)
{
	char* cmd = NULL;

	debug(100, "startcam emu= %s", emu);
	if(emu == NULL) return;

	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh start \"" , cmd, 0, 1);
	osystem(cmd, 20);
	free(cmd);
	readmenu();
	fillmenubox();
}

void stopcam(char* emu)
{
	char* cmd = NULL;

	debug(100, "stopcam emu= %s", emu);
	if(emu == NULL) return;

	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh stop \"" , cmd, 0, 1);
	osystem(cmd, 20);
	free(cmd);
	readmenu();
	fillmenubox();
}

void restartcam(char* emu)
{
	char* cmd = NULL;

	debug(100, "restart emu= %s", emu);
	if(emu == NULL) return;

	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh restart \"" , cmd, 0, 1);
	osystem(cmd, 20);
	free(cmd);
	readmenu();
	fillmenubox();
}

void readecminfo(struct skin* labelecminfo)
{
	char* tmpstr = NULL;

	tmpstr = command("emu.sh ecminfo");
	changetext(labelecminfo, _(tmpstr));
	free(tmpstr);
}

void screensoftcam()
{
	int rcret = 0;
	char* tmpstr = NULL;
	struct skin* loading = getscreen("loading");
	struct skin* softcam = getscreen("softcam");
	struct skin* listbox = getscreennode(softcam, "listbox");
	struct skin* labelecminfo = getscreennode(softcam, "ecminfo");
	struct skin* menutitle = getscreennode(softcam, "menutitle");
	struct skin* b_red = getscreennode(softcam, "b1");
	struct skin* b_green = getscreennode(softcam, "b2");
	struct skin* b_yellow = getscreennode(softcam, "b3");
	struct skin* b_blue = getscreennode(softcam, "b4");
	struct skin* b_menu = getscreennode(softcam, "b7");

	struct skin* pluginnode = NULL;
	void (*startplugin)(char*);

	drawscreen(loading, 0, 0);

	// show labels
	changetext(menutitle, _("Softcam selection:"));
	changetext(b_red, _("Deactivate"));
	changetext(b_green, _("Restart"));
	changetext(b_yellow, _("Refresh"));
	changetext(b_blue, _("Activate"));

	// read ecm inf0
	readecminfo(labelecminfo);

	// read emulist
	readmenu();

	// add emu's to selectionbox
	fillmenubox();

	drawscreen(softcam, 2, 0);
	addscreenrc(softcam, listbox);
	int found = 0;

	if(listbox->select != NULL)
	{
		free(tmpstr), tmpstr = NULL;
		tmpstr = ostrcat(listbox->select->name, NULL, 0, 0);
		string_tolower(tmpstr);

		if(ostrstr(tmpstr, "oscam") != NULL)
		{
			b_menu->hidden = NO;
			found = 1;
		}
		else
		{
			b_menu->hidden = YES;
			found = 0;
		}
		free(tmpstr), tmpstr = NULL;
	}

	drawscreen(softcam, 0, 0);
		
	while(1)
	{
		rcret = waitrc(softcam, 4000, 2);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(listbox->select != NULL)
		{
			tmpstr = ostrcat(listbox->select->name, NULL, 0, 0);
			string_tolower(tmpstr);
			if(ostrstr(tmpstr, "oscam") != NULL)
			{
				b_menu->hidden = NO;
				found = 1;
			}
			else
			{
				b_menu->hidden = YES;
				found = 0;
			}
			free(tmpstr), tmpstr = NULL;
		}
		if(listbox->select != NULL && rcret == getrcconfigint("rcred", NULL))
		{
			// deactivate or stop emu
			drawscreen(loading, 0, 0);

			// check if cam is active
			char* cmd = string_quote(listbox->select->name);
			cmd = ostrcat("emu.sh active ", cmd, 0, 1);
			if(ostrcmp(listbox->select->name, string_newline(command(cmd))) == 0)
				deactivate(listbox->select->name);
			else
				stopcam(listbox->select->name);
			free(cmd); cmd = NULL;
			startinternreader(1);
		}
		if(listbox->select != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			startinternreader(0);
			// restart emu
			drawscreen(loading, 0, 0);
			restartcam(listbox->select->name);
			startinternreader(1);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			// refrech screen
			drawscreen(loading, 0, 0);
			readecminfo(labelecminfo);
			readmenu();
			fillmenubox();
		}
		if(listbox->select != NULL && rcret == getrcconfigint("rcblue", NULL))
		{
			startinternreader(0);
			// activate emu
			drawscreen(loading, 0, 0);
			activate(listbox->select->name);
			startinternreader(1);
		}
		if(listbox->select != NULL && rcret == getrcconfigint("rcok", NULL))
		{
			startinternreader(0);
			// start/stop emu, depending if emu already runs
			drawscreen(loading, 0, 0);
			if(checkrunningcam(listbox->select->name) == 1)
				stopcam(listbox->select->name);
			else
				startcam(listbox->select->name);
			startinternreader(1);
		}
		if(rcret == getrcconfigint("rcmenu", NULL) && found == 1)
		{
			pluginnode = getplugin("Reader Config");

			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "screenoscam");
				if(startplugin != NULL)
					startplugin(listbox->select->name);
			}
			else
				textbox(_("Message"), _("Reader Config Plugin not installed !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
		}
		if(rcret == getrcconfigint("rcinfo", NULL))
		{
			int pinret = 0;
			pinret = screenpincheck(4, NULL);
			if(pinret == 1) continue;

			char* extract = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *cmd = NULL;

			tmpstr = ostrcat(listbox->select->name, NULL, 0, 0);

			string_tolower(tmpstr);
			tmpstr = stringreplacecharonce(tmpstr, '-', '\0');
			tmpstr = stringreplacecharonce(tmpstr, '_', '\0');
			tmpstr = stringreplacecharonce(tmpstr, '.', '\0');

			if(listbox->select->name == NULL)
			{
				if(file_exist("/mnt/swapextensions/keys"))
					tmpstr1 = ostrcat("/mnt/swapextensions", NULL, 0, 0);
				else if(file_exist("/var/swap/keys"))
					tmpstr1 = ostrcat("/var/swap", NULL, 0, 0);
				else if(file_exist("/var/keys"))
					tmpstr1 = ostrcat("/var/keys", NULL, 0, 0);

				if(tmpstr1 == NULL) return;
				extract = ostrcat("tar -zxvf /tmp/.tmp.tar.gz -C ", tmpstr1, 0, 0);
				extract = ostrcat(extract, "/keys/", 1, 0);
				extract = ostrcat(extract, " > /dev/null 2>&1", 1, 0);
				free(tmpstr1), tmpstr1 = NULL; 
			}
			else
			{
				cmd = ostrcat("/sbin/emu.sh keydir ", listbox->select->name, 0, 0);
				tmpstr1 = string_newline(command(cmd));
				extract = ostrcat("tar -zxvf /tmp/.tmp.tar.gz -C ", tmpstr1, 0, 0);
				extract = ostrcat(extract, "/", 1, 0);
				extract = ostrcat(extract, " > /dev/null 2>&1", 1, 0);
				free(cmd), cmd = NULL;
				free(tmpstr1), tmpstr1 = NULL;
			}

			tmpstr1 = ostrcat("/svn/auth/", tmpstr, 0, 0);
			tmpstr1 = ostrcat(tmpstr1, ".tar.gz", 0, 0);

			tmpstr2 = ostrcat(tmpstr, " ", 0, 0);
			tmpstr2 = ostrcat(tmpstr2, _("Keys Updated !"), 1, 0);

			tmpstr3 = ostrcat(_("Restart"), " ", 0, 0);
			tmpstr3 = ostrcat(tmpstr3, tmpstr, 1, 0);
			tmpstr3 = ostrcat(tmpstr3, " ?", 1, 0);

			int ret = 1;

			debug(696, "tmpstr1: %s", tmpstr1);
			gethttp("openaaf.dyndns.tv", tmpstr1, 80, "/tmp/.tmp.tar.gz", HTTPAUTH, 5000, NULL, 0);		

			debug(696, "extract: %s", extract);
			ret = system(extract);

			if(ret == 0)
			{
				textbox(_("Message"), tmpstr2, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				if(textbox(_("Message"), tmpstr3, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
				{
					cmd = ostrcat("emu.sh restart" , NULL, 0, 0);
					ret = system(cmd);
					free(cmd);
				}
			}
			system("rm -rf /tmp/.tmp.tar.gz > /dev/null 2>&1");

			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
			free(tmpstr2); tmpstr2 = NULL;
			free(tmpstr3); tmpstr3 = NULL;
			free(extract); extract = NULL;
		}
		if(rcret == RCTIMEOUT)
		{
			// update ecminfo
			readecminfo(labelecminfo);
		}
		drawscreen(softcam, 0, 0);
	}

	if(emulist != NULL)
		freelist(emulist);

	//free(swhichcam); swhichcam = NULL;
	delmarkedscreennodes(softcam, 1);
	delownerrc(softcam);
	clearscreen(softcam);
}

#endif
