#ifndef SYSTEM_UPDATE_H
#define SYSTEM_UPDATE_H

void screensystem_update(int mode)
{
	debug(50, "(start) mode=%d", mode);
	int rcret = 0, imgtype = 0;

	status.hangtime = 99999;
	struct skin* load = getscreen("loading");
	drawscreen(load, 0, 0);

	char* tmpstr = NULL;

	char* skinname = NULL;
	char* filemask = NULL;
	char* filepath = NULL;
	char* type = NULL;
	char* cmd = NULL;
		
	char* auth = NULL;
	auth = ostrcat(auth, " aUtzhFRTzuDFa", 1, 0);
	auth = ostrcat(auth, " JNHZbghnjuz", 1, 0);
			
	if(file_exist("/etc/.beta")) imgtype = 1;	

	if (mode == 0)
	{	
		char* cmd = NULL;
		cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
		cmd = ostrcat(cmd, auth, 1, 0);
		if(imgtype == 1)
			cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
		else
			cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);		
		system(cmd);
		free(cmd),cmd = NULL;

		skinname = "systemupdate_flash_online_menu";
		filemask = ostrcat(filemask, "*.img", 1, 0);
		filepath = ostrcat(filepath, "/tmp/online", 1, 0);
		type = ostrcat(type, "online", 1, 0);
	}
	else if (mode == 1)
	{
		skinname = "systemupdate_flash_tmp_menu";
		filemask = ostrcat(filemask, "*.img", 1, 0);
		if(file_exist("/var/backup"))
			filepath = ostrcat(filepath, "/var/backup", 1, 0);
		else
			filepath = ostrcat(filepath, "/tmp", 1, 0);		
		type = ostrcat(type, "tmp", 1, 0);
	}
	else if (mode == 2)
	{
		char* cmd = NULL;
		cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
		cmd = ostrcat(cmd, auth, 1, 0);
		if(imgtype == 1)
			cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
		else
			cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);	
		system(cmd);
		free(cmd),cmd = NULL;
		skinname = "systemupdate_usb_online_menu";
		filemask = ostrcat(filemask, "*.tar.gz", 1, 0);
		filepath = ostrcat(filepath, "/tmp/online", 1, 0);
		type = ostrcat(type, "online", 1, 0);
	}
	else if (mode == 3)
	{
		skinname = "systemupdate_usb_tmp_menu";
		filemask = ostrcat(filemask, "*.tar.gz", 1, 0);
		if(file_exist("/var/backup"))
			filepath = ostrcat(filepath, "/var/backup", 1, 0);
		else
			filepath = ostrcat(filepath, "/tmp", 1, 0);	
		type = ostrcat(type, "tmp", 1, 0);
	}

	struct skin* systemupdate = getscreen(skinname);
	struct skin* filelistpath = getscreennode(systemupdate, "filelistpath");
	struct skin* filelist = getscreennode(systemupdate, "filelist");
	struct skin* device = getscreennode(systemupdate, "device");
	struct skin* b6 = getscreennode(systemupdate, "b6");
	
	if(imgtype == 1)
		changetext(b6, _("stable"));
	else
		changetext(b6, _("unstable"));
		
	if(!file_exist("/etc/.beta")) b6->hidden = YES;

	if(mode == 2 || mode == 3)
	{
		char* devicelist = command("cat /proc/diskstats | awk {'print $3'} | grep 'sd[a-z][0-9]'");
		char* rootpart = command("cat /proc/cmdline | sed 's/^.*root=//;s/ .*$//' | sed 's!/dev/!!'");

		if(devicelist != NULL && strlen(devicelist) != 0)
		{
			char* pch;
			char* label = NULL;
			char* showname = NULL;
			char* version = NULL;
			pch = strtok (devicelist, "\n");
			int count = 0;
			while(pch != NULL)
			{
				count += 1;
				label = get_label(pch);

				if(ostrstr(label, "MINI") != NULL)
				{
					cmd = ostrcat("cat /autofs/", pch, 0, 0);
					cmd = ostrcat(cmd, "/etc/version", 1, 0);
					version = command(cmd);

					showname = ostrcat(label, "-", 0, 0);
					showname = ostrcat(showname, pch, 1, 0);
					showname = ostrcat(showname, "-", 1, 0);

					if(version == NULL)
						showname = ostrcat(showname, _("non-version"), 1, 0);
					else
						showname = ostrcat(showname, strstrip(version), 1, 0);

					if(ostrcmp(pch, rootpart) == 0)
						showname = ostrcat(showname, " (active)", 1, 0);

					debug(40, "addchoicebox: device=%s, label=%s showname=%s", pch, label, showname);
					addchoicebox(device, label, _(showname));

					free(cmd), cmd = NULL;
					free(showname), showname = NULL;
					free(version), version = NULL;
				}

				pch = strtok (NULL, "\n");
				free(label), label = NULL;
			}
			free(pch), pch = NULL;

		}
		else
			addchoicebox(device, "no device found", "no device found");

		free(devicelist), devicelist = NULL;
		free(rootpart), rootpart = NULL;
	}

	setchoiceboxselection(device, getconfig("device", NULL));

	clearscreen(load);
	getfilelist(systemupdate, filelistpath, filelist, filepath, filemask, 0, NULL);
	addscreenrc(systemupdate, filelist);


	if(mode == 2 || mode == 3)
	{
		delrc(getrcconfigint("rcright", NULL), systemupdate, filelist);
		delrc(getrcconfigint("rcleft", NULL), systemupdate, filelist);
		addscreenrc(systemupdate, device);
	}

	while(1)
	{
		rcret = waitrc(systemupdate, 0, 0);
		debug(40, "while status");


		if(rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcred", NULL))
			break;
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if (mode == 0 || mode == 2)
			{
				char* cmd = NULL;

				drawscreen(load, 0, 0);
				cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
				cmd = ostrcat(cmd, auth, 1, 0);
				if(imgtype == 1)
					cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);	
				system(cmd);
				free(cmd),cmd = NULL;
				clearscreen(load);
			}

			drawscreen(systemupdate, 0, 0);
			getfilelist(systemupdate, filelistpath, filelist, filepath, filemask, 0, NULL);
			addscreenrc(systemupdate, filelist);
			continue;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			if (mode == 0 || mode == 2)
			{
				if(imgtype == 0)
				{
					int pinret = 0;
					
					if(!file_exist("/etc/.beta"))
						pinret = screenpincheck(3, NULL);
					
					if(pinret == 0)
					{
						imgtype = 1;
						changetext(b6, _("stable"));
					}
				}
				else
				{
					imgtype = 0;
					changetext(b6, _("unstable"));
				}
			
				char* cmd = NULL;

				drawscreen(load, 0, 0);
				cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
				cmd = ostrcat(cmd, auth, 1, 0);
				if(imgtype == 1)
					cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);	
				system(cmd);
				free(cmd),cmd = NULL;
				clearscreen(load);
			}

			drawscreen(systemupdate, 0, 0);
			getfilelist(systemupdate, filelistpath, filelist, filepath, filemask, 0, NULL);
			addscreenrc(systemupdate, filelist);
			continue;
		}
		else if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcgreen", NULL))
		{
			if(filelist->select != NULL && filelist->select->input != NULL)
				continue;
			else if(filelist->select != NULL && filelist->select->input == NULL)
			{
				tmpstr = createpath(filelistpath->text, filelist->select->text);
				debug(40, "tmpstr: %s", tmpstr);

				char* cmd = NULL;
				cmd = ostrcat(cmd, "/sbin/update.sh ", 1, 0);
				cmd = ostrcat(cmd, type, 1, 0);
				cmd = ostrcat(cmd, " ", 1, 0);
			
				char* msgtxt = NULL;
				writeallconfig(1);

				debug(40, "Update: update with log");
				if(ostrstr(filelist->select->text, "_FULL_") != NULL)
				{
					cmd = ostrcat(cmd, "full ", 1, 0);
					cmd = ostrcat(cmd, tmpstr, 1, 0);
					cmd = ostrcat(cmd, auth, 1, 0);
					if(imgtype == 1)
						cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
					else
						cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);

					if(checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("UFS912") == 1 || checkbox("ATEMIO7600") == 1)
					{
						if(!file_exist("/mnt/logs"))
							 mkdir("/mnt/logs", 777);
					
						if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
							cmd = ostrcat(cmd, " > /mnt/logs/update_debug.log 2>&1", 1, 0);
					}
					else if(file_exist("/var/swap"))
					{
						if(!file_exist("/var/swap/logs"))
							 mkdir("/var/swap/logs", 777);
					
						if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
							cmd = ostrcat(cmd, " > /var/swap/logs/update_debug.log 2>&1", 1, 0);		
					}
		
					msgtxt = ostrcat(msgtxt, _("starting Full Update ?"), 1, 0);
				}

				if(ostrstr(filelist->select->text, "_FULLBACKUP.") != NULL)
				{
					cmd = ostrcat(cmd, "fullbackup ", 1, 0);
					cmd = ostrcat(cmd, tmpstr, 1, 0);
					cmd = ostrcat(cmd, auth, 1, 0);
					if(imgtype == 1)
						cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
					else
						cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);

					if(checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("UFS912") == 1 || checkbox("ATEMIO7600") == 1)
					{
						if(!file_exist("/mnt/logs"))
							 mkdir("/mnt/logs", 777);
					
						if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
							cmd = ostrcat(cmd, " > /mnt/logs/update_debug.log 2>&1", 1, 0);
					}
					else if(file_exist("/var/swap"))
					{
						if(!file_exist("/var/swap/logs"))
							 mkdir("/var/swap/logs", 777);
					
						if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
							cmd = ostrcat(cmd, " > /var/swap/logs/update_debug.log 2>&1", 1, 0);		
					}

					msgtxt = ostrcat(msgtxt, _("starting Full Update (from backup) ?"), 1, 0);
				}

				clearscreen(systemupdate);
				resettvpic();
				if(msgtxt == NULL)
				{
					textbox(_("Message"), _("Error file not supported"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(systemupdate, 0, 0);
					getfilelist(systemupdate, filelistpath, filelist, filepath, filemask, 0, NULL);
					addscreenrc(systemupdate, filelist);
				}

				if(textbox(_("Message"), msgtxt, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
				{
					debug(40, "update started cmd: %s", cmd);
					status.sec = 0; //deactivate spinner
					
					system(cmd);
					//should only reached if system fails
					textbox(_("Message"), _("Can't start system update\nSyntax Error on updatefile"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
					debug(40, "update error cmd: %s", cmd);
					drawscreen(systemupdate, 0, 0);
					getfilelist(systemupdate, filelistpath, filelist, filepath, filemask, 0, NULL);
					addscreenrc(systemupdate, filelist);
				}
				else
				{
					debug(40, "update canceled cmd: %s", cmd);
					drawscreen(systemupdate, 0, 0);
					getfilelist(systemupdate, filelistpath, filelist, filepath, filemask, 0, NULL);
					addscreenrc(systemupdate, filelist);
				}

				free(cmd); cmd = NULL;
				free(msgtxt); msgtxt = NULL;
				free(tmpstr); tmpstr = NULL;
				continue;
			}
		}
	}
	free(auth); auth = NULL;
	delownerrc(systemupdate);
	clearscreen(systemupdate);

	delownerrc(systemupdate);
	clearscreen(systemupdate);

	status.hangtime = getconfigint("hangtime", NULL);
	debug(40, "end");
}

#endif
