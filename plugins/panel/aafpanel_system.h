#ifndef AAFPANEL_SYSTEM_H
#define AAFPANEL_SYSTEM_H

void screenaafpanel_system_update(int mode)
{
	debug(50, "(start) mode=%d", mode);
	int rcret = 0;

	status.hangtime = 99999;
	struct skin* aafpanel_system = getscreen("aafpanel_system_main");
	drawscreen(aafpanel_system, 0);

	char* tmpstr = NULL;

	char* skinname = NULL;
	char* filemask = NULL;
	char* filepath = NULL;
	char* type = NULL;
	char* cmd = NULL;
	
	if (mode == 0){
		system("/sbin/update.sh getfilelist");
		skinname = "aafpanel_system_update_flash_online_menu";
		filemask = ostrcat(filemask, "*.img", 1, 0);
		filepath = ostrcat(filepath, "/tmp/online", 1, 0);
		type = ostrcat(type, "online", 1, 0);
	} else if (mode == 1){
		skinname = "aafpanel_system_update_flash_tmp_menu";
		filemask = ostrcat(filemask, "*.img", 1, 0);
		filepath = ostrcat(filepath, "/tmp", 1, 0);
		type = ostrcat(type, "tmp", 1, 0);
	} else if (mode == 2){
		system("/sbin/update.sh getfilelist");
		skinname = "aafpanel_system_update_usb_online_menu";
		filemask = ostrcat(filemask, "*.tar.gz", 1, 0);
		filepath = ostrcat(filepath, "/tmp/online", 1, 0);
		type = ostrcat(type, "online", 1, 0);
	} else if (mode == 3){
		skinname = "aafpanel_system_update_usb_tmp_menu";
		filemask = ostrcat(filemask, "*.tar.gz", 1, 0);
		filepath = ostrcat(filepath, "/tmp", 1, 0);
		type = ostrcat(type, "tmp", 1, 0);
	}

	aafpanel_system = getscreen(skinname);
	struct skin* filelistpath = getscreennode(aafpanel_system, "aafpanel_filelistpath");
	struct skin* filelist = getscreennode(aafpanel_system, "aafpanel_filelist");

	struct skin* device = getscreennode(aafpanel_system, "device");

	if (mode == 2 || mode == 3){

		char* devicelist = command("cat /proc/diskstats | awk {'print $3'} | grep 'sd[a-z][0-9]'");
		char* rootpart = command("cat /proc/cmdline | sed 's/^.*root=//;s/ .*$//' | sed 's!/dev/!!'");

		if(file_exist("/var/baroot/ba/bainit")){
			addchoicebox(device,"BA-IMAGE","BA-IMAGE");
		}	
		if(devicelist != NULL && strlen(devicelist) != 0){
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

				if(string_find("MINI",label)){
					cmd = ostrcat("cat /media/autofs/", pch, 0, 0);
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

					debug(40, "addchoicebox: device=%s, label=%s showname=%s",pch, label, showname);
					addchoicebox(device, label, _(showname));

					free(cmd), cmd = NULL;
					free(showname), showname = NULL;
					free(version), version = NULL;
				}

				pch = strtok (NULL, "\n");
				free(label), label = NULL;
			}
			free(pch), pch = NULL;

		} else {
			addchoicebox(device, "no device found", "no device found");
		}

		free(devicelist), devicelist = NULL;
		free(rootpart), rootpart = NULL;
	}

	setchoiceboxselection(device, getconfig("device", NULL));

	getfilelist(aafpanel_system, filelistpath, filelist, filepath, filemask, 1, NULL);
	addscreenrc(aafpanel_system, filelist);
	if(mode == 2 || mode == 3)
	{
		delrc(getrcconfigint("rcright", NULL), aafpanel_system, filelist);
		delrc(getrcconfigint("rcleft", NULL), aafpanel_system, filelist);
		addscreenrc(aafpanel_system, device);
	}

	while(1)
	{
		rcret = waitrc(aafpanel_system, 0, 0);
		debug(40, "[aafpanel_system] while status");


		if(rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcred", NULL)){
			break;
		} else if(rcret == getrcconfigint("rcyellow", NULL)){
			if (mode == 0 || mode == 2){
				system("/sbin/update.sh getfilelist");
			}
			drawscreen(aafpanel_system, 0);
			getfilelist(aafpanel_system, filelistpath, filelist, filepath, filemask, 1, NULL);
			addscreenrc(aafpanel_system, filelist);
			continue;
		} else if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcgreen", NULL)){
			if(filelist->select != NULL && filelist->select->input != NULL){
				continue;
			} else if(filelist->select != NULL && filelist->select->input == NULL){
				tmpstr = createpath(filelistpath->text, filelist->select->text);
				debug(40, "[aafpanel_system] tmpstr: %s", tmpstr);

				char* cmd = NULL;
				cmd = ostrcat(cmd, "/sbin/update.sh ", 1, 0);
				cmd = ostrcat(cmd, type, 1, 0);
				cmd = ostrcat(cmd, " ", 1, 0);

				char* msgtxt = NULL;

				if(file_exist("/tmp/.swapextensionsdev")){
					printf("[AafPanel] Update: update with log\n");
					if(string_find("_KERNEL_",filelist->select->text)){
						cmd = ostrcat(cmd, "kernel ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Kernel Update starten ?", 1, 0);
					} else if(string_find("_FW_",filelist->select->text)){
						cmd = ostrcat(cmd, "fw ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Fw Update starten ?", 1, 0);
					} else if(string_find("_ROOT_",filelist->select->text)){
						cmd = ostrcat(cmd, "root ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Root Update starten ?", 1, 0);
					} else if(string_find("_VAR_",filelist->select->text)){
						cmd = ostrcat(cmd, "var ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Var Update starten ?", 1, 0);
					} else if(string_find("_FULL_",filelist->select->text)){
						cmd = ostrcat(cmd, "full ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Full Update starten ?", 1, 0);
					} else if(string_find("_USB_",filelist->select->text)){
						//char* device = getconfig("device", NULL);
						cmd = ostrcat(cmd, device->ret, 1, 0);
						cmd = ostrcat(cmd, " ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Usb Update starten ?", 1, 0);
					} else if(string_find("_FULLUSB_",filelist->select->text)){
						//char* device = getconfig("device", NULL);
						cmd = ostrcat(cmd, device->ret, 1, 0);
						cmd = ostrcat(cmd, " ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						cmd = ostrcat(cmd, " > /var/swap/update_debug.log 2>&1", 1, 0);
						msgtxt = ostrcat(msgtxt, "Fullusb Update starten ?", 1, 0);
					}
				} else {
					printf("[AafPanel] Update: update without log\n");

					if(string_find("_KERNEL_",filelist->select->text)){
						cmd = ostrcat(cmd, "kernel ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Kernel Update starten ?", 1, 0);
					} else if(string_find("_FW_",filelist->select->text)){
						cmd = ostrcat(cmd, "fw ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Fw Update starten ?", 1, 0);
					} else if(string_find("_ROOT_",filelist->select->text)){
						cmd = ostrcat(cmd, "root ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Root Update starten ?", 1, 0);
					} else if(string_find("_VAR_",filelist->select->text)){
						cmd = ostrcat(cmd, "var ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Var Update starten ?", 1, 0);
					} else if(string_find("_FULL_",filelist->select->text)){
						cmd = ostrcat(cmd, "full ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Full Update starten ?", 1, 0);
					} else if(string_find("_USB_",filelist->select->text)){
						//char* device = getconfig("device", NULL);
						cmd = ostrcat(cmd, device->ret, 1, 0);
						cmd = ostrcat(cmd, " ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Usb Update starten ?", 1, 0);
					} else if(string_find("_FULLUSB_",filelist->select->text)){
						//char* device = getconfig("device", NULL);
						cmd = ostrcat(cmd, device->ret, 1, 0);
						cmd = ostrcat(cmd, " ", 1, 0);
						cmd = ostrcat(cmd, tmpstr, 1, 0);
						msgtxt = ostrcat(msgtxt, "Fullusb Update starten ?", 1, 0);
					}
				}
				clearscreen(aafpanel_system);
				if(msgtxt == NULL){
					textbox(_("Message"), _("Error file not supported"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(aafpanel_system, 0);
					getfilelist(aafpanel_system, filelistpath, filelist, filepath, filemask, 1, NULL);
					addscreenrc(aafpanel_system, filelist);
					continue;
				}

				if(textbox(_("Message"), _(msgtxt), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2){
					debug(40, "[aafpanel_system] update started cmd: %s", cmd);
					system(cmd);
				} else {
					debug(40, "[aafpanel_system] update canceled cmd: %s", cmd);
					drawscreen(aafpanel_system, 0);
					getfilelist(aafpanel_system, filelistpath, filelist, filepath, filemask, 1, NULL);
					addscreenrc(aafpanel_system, filelist);
					continue;
				}

				free(cmd); cmd = NULL;
				free(msgtxt); msgtxt = NULL;
				free(tmpstr); tmpstr = NULL;
				continue;
			}
		}
	}
	delownerrc(aafpanel_system);
	clearscreen(aafpanel_system);

	delownerrc(aafpanel_system);
	clearscreen(aafpanel_system);

	status.hangtime = getconfigint("hangtime", NULL);
	debug(40, "[screenaafpanel_system_update] closed:");
}

#endif
