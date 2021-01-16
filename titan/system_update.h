#ifndef SYSTEM_UPDATE_H 
#define SYSTEM_UPDATE_H

void screensystem_update(int mode)
{
	debug(50, "(start) mode=%d", mode);
	int rcret = 0, ret = 0;

	status.hangtime = 99999;
	struct skin* load = getscreen("loading");
	drawscreen(load, 0, 0);
	
	char* tmpstr = NULL;
	char* cmd = NULL;

	struct update* node = createupdatelist(mode);
		
	struct skin* systemupdate = getscreen(node->skinname);
	struct skin* filelistpath = getscreennode(systemupdate, "filelistpath");
	struct skin* filelist = getscreennode(systemupdate, "filelist");
	struct skin* device = getscreennode(systemupdate, "device");
	struct skin* b6 = getscreennode(systemupdate, "b6");
	
	if(node->imgtype == 1)
		changetext(b6, _("stable"));
	else
		changetext(b6, _("unstable"));
		
	if(!file_exist("/etc/.beta")) b6->hidden = YES;

	if(mode == 2 || mode == 3)
	{
#ifdef OEBUILD
		char* devicelist = command("cat /boot/STARTUP* | sed -nr 's/.*root=\\/dev\\/([^\\/]+) rootfstype.*/\\1/p' | sed 's! rootsubdir=!/!g' | sort -u");
		char* rootpart = string_newline(command("cat /proc/cmdline | sed -nr 's/.*root=\\/dev\\/([^\\/]+) rootfstype.*/\\1/p' | sed 's! rootsubdir=!/!g'"));
#else
		char* devicelist = command("cat /proc/diskstats | awk {'print $3'} | grep 'sd[a-z][0-9]'");
		char* rootpart = string_newline(command("cat /proc/cmdline | sed 's/^.*root=//;s/ .*$//' | sed 's!/dev/!!'"));
#endif
printf("devicelist: %s\n", devicelist);
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
#ifdef OEBUILD
				label= ostrcat("STARTUP_", oitoa(count), 0, 1);
#else
				label = get_label(pch);
#endif
				if(ostrstr(label, "MINI") != NULL || ostrstr(label, "STARTUP") != NULL)
				{
					cmd = ostrcat("cat /autofs/", pch, 0, 0);
					cmd = ostrcat(cmd, "/etc/version-svn", 1, 0);
					version = command(cmd);

					showname = ostrcat(label, " ", 0, 0);

					if(version == NULL)
					{
						showname = ostrcat(label, " (", 0, 0);
						showname = ostrcat(showname, pch, 1, 0);
						showname = ostrcat(showname, ") ", 1, 0);
						showname = ostrcat(showname, _("non-version"), 1, 0);
					}
					else
						showname = ostrcat(showname, strstrip(version), 1, 0);
printf("rootpart: %s\n", rootpart);
printf("pch: %s\n", pch);

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
	getfilelist(systemupdate, filelistpath, filelist, node->filepath, node->filemask, 0, NULL);
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
			if(mode == 0 || mode == 2)
			{
				char* cmd = NULL;

				drawscreen(load, 0, 0);
				cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
				cmd = ostrcat(cmd, node->auth, 1, 0);
#ifdef OEBUILD
				if(node->imgtype == 1)
					cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release titannit.dyndns.tv", 1, 0);	
#else
				if(node->imgtype == 1)
					cmd = ostrcat(cmd, " dev beta.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);	
#endif
				system(cmd);
				free(cmd),cmd = NULL;
				clearscreen(load);
			}

			drawscreen(systemupdate, 0, 0);
			getfilelist(systemupdate, filelistpath, filelist, node->filepath, node->filemask, 0, NULL);
			addscreenrc(systemupdate, filelist);
			continue;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(mode == 0 || mode == 2)
			{
				if(node->imgtype == 0)
				{
					int pinret = 0;
					
					if(!file_exist("/etc/.beta"))
						pinret = screenpincheck(3, NULL);
					
					if(pinret == 0)
					{
						node->imgtype = 1;
						changetext(b6, _("stable"));
					}
				}
				else
				{
					node->imgtype = 0;
					changetext(b6, _("unstable"));
				}
			
				char* cmd = NULL;

				drawscreen(load, 0, 0);
				cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
				cmd = ostrcat(cmd, node->auth, 1, 0);
#ifdef OEBUILD
				if(node->imgtype == 1)
					cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release titannit.dyndns.tv", 1, 0);	
#else
				if(node->imgtype == 1)
					cmd = ostrcat(cmd, " dev beta.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);	
#endif
				system(cmd);
				free(cmd),cmd = NULL;
				clearscreen(load);
			}

			drawscreen(systemupdate, 0, 0);
			getfilelist(systemupdate, filelistpath, filelist, node->filepath, node->filemask, 0, NULL);
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
//				if(checkrealbox("DM900") == 1 || checkrealbox("HD51") == 1)
//				if(checkrealbox("HD51") == 1)
//					cmd = ostrcat(cmd, "/sbin/update2.sh ", 1, 0);
//				else
					cmd = ostrcat(cmd, "/sbin/update.sh ", 1, 0);
				cmd = ostrcat(cmd, node->type, 1, 0);
				cmd = ostrcat(cmd, " ", 1, 0);
			
				char* msgtxt = NULL;
				writeallconfig(1);

				debug(40, "Update: update with log");
				if(ostrstr(filelist->select->text, "_USB_") != NULL || ostrstr(filelist->select->text, "_FULL_") != NULL || ostrstr(filelist->select->text, "_FULLBACKUP.") != NULL || ostrstr(filelist->select->text, "_UPDATENFI_") != NULL || ostrstr(filelist->select->text, "_UPDATEUSB_") != NULL || ostrstr(filelist->select->text, ".zip") != NULL)
				{
					if(ostrstr(filelist->select->text, "_FULL_") != NULL)
						cmd = ostrcat(cmd, "full ", 1, 0);
					else if(ostrstr(filelist->select->text, "_FULLBACKUP.") != NULL)
						cmd = ostrcat(cmd, "fullbackup ", 1, 0);
					else if(ostrstr(filelist->select->text, "_UPDATENFI_") != NULL)
						cmd = ostrcat(cmd, "updatenfi ", 1, 0);
					else if(ostrstr(filelist->select->text, "_UPDATEUSB_") != NULL)
						cmd = ostrcat(cmd, "updateusb ", 1, 0);
					else if(ostrstr(filelist->select->text, ".tar.gz") != NULL)
					{
						cmd = ostrcat(cmd, device->ret, 1, 0);
						cmd = ostrcat(cmd, " ", 1, 0);
					}
					else if(ostrstr(filelist->select->text, ".zip") != NULL)
#ifdef OEBUILD
						cmd = ostrcat(cmd, "updateusb ", 1, 0);
#else
						cmd = ostrcat(cmd, "full ", 1, 0);
#endif

					cmd = ostrcat(cmd, tmpstr, 1, 0);
					cmd = ostrcat(cmd, node->auth, 1, 0);
#ifdef OEBUILD
					if(node->imgtype == 1)
						cmd = ostrcat(cmd, " dev titannit.dyndns.tv", 1, 0);
					else
						cmd = ostrcat(cmd, " release titannit.dyndns.tv", 1, 0);

					if(mode == 2 || mode == 3)
					{
						// send multiboot label
						cmd = ostrcat(cmd, " ", 1, 0);
						cmd = ostrcat(cmd, device->ret, 1, 0);
					}
#else
					if(node->imgtype == 1)
						cmd = ostrcat(cmd, " dev beta.dyndns.tv", 1, 0);
					else
						cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);
#endif
#ifdef OEBUILD
					if(file_exist("/media/hdd") && isdir("/media/hdd"))
					{
						if(!file_exist("/media/hdd/logs"))
							 mkdir("/media/hdd/logs", 777);

						if(file_exist("/media/hdd/logs"))
						{
							cmd = ostrcat(cmd, " > /media/hdd/logs/update_", 1, 0);		
							cmd = ostrcat(cmd, getboxtype(), 1, 0);
							cmd = ostrcat(cmd, "_debug.log 2>&1", 1, 0);		
						}
					}
					else if(file_exist("/var/backup"))
					{
						if(!file_exist("/var/backup/logs"))
							 mkdir("/var/backup/logs", 777);
					
						if(file_exist("/var/backup/logs"))
						{
							cmd = ostrcat(cmd, " > /var/backup/logs/update_", 1, 0);		
							cmd = ostrcat(cmd, getboxtype(), 1, 0);
							cmd = ostrcat(cmd, "_debug.log 2>&1", 1, 0);		
						}
					}
					else if(file_exist("/var/swap") && !isdir("/var/swap"))
					{
						if(!file_exist("/var/swap/logs"))
							 mkdir("/var/swap/logs", 777);
					
						if(file_exist("/var/swap/logs"))
						{
							cmd = ostrcat(cmd, " > /var/swap/logs/update_", 1, 0);		
							cmd = ostrcat(cmd, getboxtype(), 1, 0);
							cmd = ostrcat(cmd, "_debug.log 2>&1", 1, 0);		
						}
					}
#else
					if(file_exist("/var/swap"))
					{
						if(!file_exist("/var/swap/logs"))
							 mkdir("/var/swap/logs", 777);
					
						if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
							cmd = ostrcat(cmd, " > /var/swap/logs/update_debug.log 2>&1", 1, 0);		
					}
					else if(checkbox("ATEMIO510") != 1 && checkbox("UFS910") != 1 && checkbox("UFS922") != 1 && checkbox("ATEVIO700") != 1 && checkbox("ATEVIO7000") != 1 && checkbox("IPBOX91") != 1 && checkbox("IPBOX900") != 1 && checkbox("IPBOX910") != 1 && checkbox("IPBOX9000") != 1 && checkbox("DM520") != 1&& checkbox("DM525") != 1)
					{
						if(!file_exist("/mnt/logs"))
							 mkdir("/mnt/logs", 777);
					
						if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
							cmd = ostrcat(cmd, " > /mnt/logs/update_debug.log 2>&1", 1, 0);
					}
#endif
					if(ostrstr(filelist->select->text, "_FULL_") != NULL)
						msgtxt = ostrcat(msgtxt, _("starting Full Update ?"), 1, 0);
					else if(ostrstr(filelist->select->text, "_FULLBACKUP.") != NULL)
						msgtxt = ostrcat(msgtxt, _("starting Full Update (from backup) ?"), 1, 0);
					else if(ostrstr(filelist->select->text, "_UPDATENFI_") != NULL)
						msgtxt = ostrcat(msgtxt, _("starting Nfi Update ?"), 1, 0);
					else if(ostrstr(filelist->select->text, ".tar.gz") != NULL)
						msgtxt = ostrcat(msgtxt, _("starting Usb Update ?"), 1, 0);
					else if(ostrstr(filelist->select->text, ".zip") != NULL)
#ifdef OEBUILD	
					msgtxt = ostrcat(msgtxt, _("starting Usb Update ?"), 1, 0);
#else
					msgtxt = ostrcat(msgtxt, _("starting Full Update ?"), 1, 0);
#endif
					else if(ostrstr(filelist->select->text, ".nfi") != NULL)
						msgtxt = ostrcat(msgtxt, _("starting Nfi Update ?"), 1, 0);
					else if(ostrstr(filelist->select->text, "_UPDATEUSB_") != NULL)
						msgtxt = ostrcat(msgtxt, _("starting Full Update ?"), 1, 0);
				}

				clearscreen(systemupdate);
				resettvpic();
				if(msgtxt == NULL)
				{
					textbox(_("Message"), _("Error file not supported"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(systemupdate, 0, 0);
					getfilelist(systemupdate, filelistpath, filelist, node->filepath, node->filemask, 0, NULL);
					addscreenrc(systemupdate, filelist);
				}

				if(textbox(_("Message"), msgtxt, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
				{
					debug(40, "update started cmd: %s", cmd);
					status.sec = 0; //deactivate spinner

					ret = checkshutdown(6);
					if(ret == 0)
					{
						system(cmd);
#ifdef OEBUILD
						sleep(3);
						textbox(_("Message"), _("Multiboot installation completed successfully\nActivate your new startup"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
#else
						//should only reached if system fails
						textbox(_("Message"), _("Can't start system update\nSyntax Error on updatefile"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
						debug(40, "update error cmd: %s", cmd);
#endif
					}
					drawscreen(systemupdate, 0, 0);
					getfilelist(systemupdate, filelistpath, filelist, node->filepath, node->filemask, 0, NULL);
					addscreenrc(systemupdate, filelist);
				}
				else
				{
					debug(40, "update canceled cmd: %s", cmd);
					drawscreen(systemupdate, 0, 0);
					getfilelist(systemupdate, filelistpath, filelist, node->filepath, node->filemask, 0, NULL);
					addscreenrc(systemupdate, filelist);
				}

				free(cmd); cmd = NULL;
				free(msgtxt); msgtxt = NULL;
				free(tmpstr); tmpstr = NULL;
				continue;
			}
		}
	}
	
	freeupdatelist(node);
//	free(auth); auth = NULL;
	delownerrc(systemupdate);
	clearscreen(systemupdate);

	delownerrc(systemupdate);
	clearscreen(systemupdate);

	status.hangtime = getconfigint("hangtime", NULL);
	debug(40, "end");
}


#endif
