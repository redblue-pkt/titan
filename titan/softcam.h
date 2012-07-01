#ifndef SOFTCAM_H
#define SOFTCAM_H

char* swhichcam = NULL;
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
	if(strstr(check, "checkemu running") != 0) running = 1;
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
	swhichcam = whichcam();

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
				debug(100, "active emu = %s\n", emush);
				active = 1;
			}

			// check if emu is running
			cmd = string_quote(emush);
			cmd = ostrcat("emu.sh check ", cmd, 0, 1);
			checkstate = command(cmd);
			free(cmd); cmd = NULL;
			if(strstr(checkstate, "checkemu running") != 0)
			{
				debug(100, "running emu = %s\n", emush);
				check = 1;
			}

			// update emuname with status
			if(active == 1 && check ==1)
				emuname = ostrcat(emuname, "  (running) (active)", 1, 0);
			else if(check ==1)
				emuname = ostrcat(emuname, "  (running)", 1, 0);
			else if(active == 1)
				emuname = ostrcat(emuname, "  (active)", 1, 0);

			debug(100, "emuname = %s\n", emuname);
			addlist(emulist, emuname, emush);
			free(emuname); emuname = NULL;
			emush = strtok(NULL, "\n");
			count++;
		}
	}
	else
		addlist(emulist, "Emu not found", "emu0");

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
					node->height = 22;
	
					// change font color, depending emu is running/active
					if(strstr(node->text, "active") != 0)
					{
						node->fontcol = convertcol("emuaktivecol");
						if(setselection == 0)
						{
							aktemu = elist->value;
							setselection = 1;
						}
					}
					else if(strstr(node->text, "running") != 0)
					{
						node->fontcol = convertcol("emurunningcol");
						if(setselection == 0 || setselection == 1)
						{
							aktemu = elist->value;
							setselection = 2;
						}
					}
					else
						node->fontcol = convertcol("fontcol");
				}
			}
			elist = elist->next;
		}
	}
	setlistboxselection(listbox, aktemu);
	debug(100, "set selection to %s\n", aktemu);
}

void activate(char* emu)
{
	debug(100, "active emu= %s\n",emu);
	char* cmd = NULL;
	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh activate \"" , cmd, 0, 1);
	system(cmd);
	free(cmd);
	readmenu();
	fillmenubox();
}

void deactivate(char* emu)
{
	debug(100, "deaktivate emu= %s\n",emu);
	char* cmd = NULL;
	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh deactivate \"" , cmd, 0, 1);
	system(cmd);
	free(cmd);
	readmenu();
	fillmenubox();
}

void startcam(char* emu)
{
	debug(100, "startcam emu= %s\n",emu);
	char* cmd = NULL;
	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh start \"" , cmd, 0, 1);
	system(cmd);
	free(cmd);
	readmenu();
	fillmenubox();
}

void stopcam(char* emu)
{
	debug(100, "stopcam emu= %s\n",emu);
	char* cmd = NULL;
	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh stop \"" , cmd, 0, 1);
	system(cmd);
	free(cmd);
	readmenu();
	fillmenubox();
}

void restartcam(char* emu)
{
	debug(100, "restart emu= %s\n",emu);
	char* cmd = NULL;
	cmd = ostrcat(emu, "\"" , 0, 0);
	cmd = ostrcat("emu.sh restart \"" , cmd, 0, 1);
	system(cmd);
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
	struct skin* loading = getscreen("loading");
	struct skin* softcam = getscreen("softcam");
	struct skin* listbox = getscreennode(softcam, "listbox");
	struct skin* labelecminfo = getscreennode(softcam, "ecminfo");
	struct skin* menutitle = getscreennode(softcam, "menutitle");
	struct skin* b_red = getscreennode(softcam, "b1");
	struct skin* b_green = getscreennode(softcam, "b2");
	struct skin* b_yellow = getscreennode(softcam, "b3");
	struct skin* b_blue = getscreennode(softcam, "b4");
	struct skin* tmp = NULL;

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

	drawscreen(softcam, 0, 0);
	addscreenrc(softcam, listbox);

	tmp = listbox->select;

	while(1)
	{
		addscreenrc(softcam, tmp);
		rcret = waitrc(softcam, 4000, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			// deactivate emu
			drawscreen(loading, 0, 0);
			deactivate(tmp->name);
			drawscreen(softcam, 0, 0);
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			// restart emu
			drawscreen(loading, 0, 0);
			restartcam(tmp->name);
			drawscreen(softcam, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			// refrech screen
			drawscreen(loading, 0, 0);
			readecminfo(labelecminfo);
			readmenu();
			fillmenubox();
			drawscreen(softcam, 0, 0);
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			// activate emu
			drawscreen(loading, 0, 0);
			activate(tmp->name);
			drawscreen(softcam, 0, 0);
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			// start/stop emu, depending if emu already runs
			drawscreen(loading, 0, 0);
			if(checkrunningcam(tmp->name) == 1)
			{
				stopcam(tmp->name);
			}
			else
			{
				startcam(tmp->name);
			}
			drawscreen(softcam, 0, 0);
		}
		if(rcret == RCTIMEOUT)
		{
			// update ecminfo
			readecminfo(labelecminfo);
			drawscreen(softcam, 0, 0);
		}
	}

	if(emulist != NULL)
		freelist(emulist);

	free(swhichcam);
	delmarkedscreennodes(softcam, 1);
	delownerrc(softcam);
	clearscreen(softcam);
}

#endif
