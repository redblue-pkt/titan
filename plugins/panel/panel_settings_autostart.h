#ifndef PANEL_SETTINGS_AUTOSTART_H
#define PANEL_SETTINGS_AUTOSTART_H

// mode 0 = Defaults
// mode 1 = Audio/Video/Sat
// mode 2 = USB Device
// mode 3 = Cam/Emu
// mode 4 = Network
// mode 5 = Child safety

void screenpanel_settings_autostart(int mode)
{
	int rcret = 0;
	struct skin* panel_config = getscreen("panel_config");
	struct skin* listbox = getscreennode(panel_config, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(panel_config, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	if (mode == 0){
		int i = 0;

		changetitle(panel_config, _("Autostart Defaults"));

		if(file_exist("/etc/.usbimage"))
		{
			node = addlistbox(panel_config, listbox, node, 1);
			if(node != NULL)
			{
				node->type = CHOICEBOX;
				changetext(node, _("fsckroot")); changename(node, "fsckroot");
				addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
				setchoiceboxselection(node, getownconfig(node->name));
			}
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("bootstop")); changename(node, "bootstop");
			for(i=0; i <= 20; i++)
			{
				char* tmpnr = NULL;
				tmpnr = oitoa(i);
				addchoicebox(node, tmpnr, _(tmpnr));
				free(tmpnr); tmpnr = NULL;
			}
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("debug")); changename(node, "debug");
			addchoicebox(node, "high", _("high")); addchoicebox(node, "low", _("low"));
			addchoicebox(node, "off", _("off"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("showip")); changename(node, "showip");
			addchoicebox(node, "status", _("Status")); addchoicebox(node, "ip", _("ip"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("updatelist")); changename(node, "updatelist");
			for(i=0; i <= 20; i++)
			{
				char* tmpnr = NULL;
				tmpnr = oitoa(i);
				addchoicebox(node, tmpnr, _(tmpnr));
				free(tmpnr); tmpnr = NULL;
			}
			setchoiceboxselection(node, getownconfig(node->name));
		}
		
		if(checkbox("UFS910") == 1)
		{
			node = addlistbox(panel_config, listbox, node, 1);
			if(node != NULL)
			{
				node->type = CHOICEBOX;
				changetext(node, _("useUnknown910")); changename(node, "useUnknown910");
				addchoicebox(node, "1", _("yes")); addchoicebox(node, "0", _("no"));
				setchoiceboxselection(node, getownconfig(node->name));
			}
		}
	}

	if (mode == 1){

		changetitle(panel_config, _("Autostart Defaults"));

		if(isfile("/var/bin/audio.elf") || isfile("/boot/audio.old.elf"))
		{
			node = addlistbox(panel_config, listbox, node, 1);
			if(node != NULL)
			{
				node->type = CHOICEBOX;
				changetext(node, _("Old Audiofw")); changename(node, "oldaudiofw");
				addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
				setchoiceboxselection(node, getownconfig(node->name));
			}
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("No Audiosync")); changename(node, "noaudiosync");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Boot Resolution")); changename(node, "BootResolution");
			addchoicebox(node, "low", _("low")); addchoicebox(node, "high", _("high"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Bootlogo")); changename(node, "bootlogo");
			addchoicebox(node, "y", _("yes")); addchoicebox(node, "n", _("no"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Poweroff")); changename(node, "poweroff");
			addchoicebox(node, "y", _("yes")); addchoicebox(node, "n", _("no"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Scart On Standby")); changename(node, "scartonstandby");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("HighSR")); changename(node, "HighSR");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Boot RGB Fix")); changename(node, "bootrgbfix");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

	if (mode == 2){
		int i = 0;
		int tmpsize = 0;

		changetitle(panel_config, _("Autostart USB"));

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Load Partition")); changename(node, "loadpartition");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Fsck Record")); changename(node, "fsckrecord");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Swap")); changename(node, "swap");
			addchoicebox(node, "y", _("yes")); addchoicebox(node, "n", _("no"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
		
		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Swap Size")); changename(node, "swapsize");
			tmpsize = 16384;
			for(i=0; i < 7; i++)
			{
				char* tmpnr = NULL;
				tmpnr = oitoa(tmpsize);
				addchoicebox(node, tmpnr, _(tmpnr));
				free(tmpnr); tmpnr = NULL;
				tmpsize = tmpsize * 2;
			}
			setchoiceboxselection(node, getownconfig(node->name));
		 }

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Fsck Swap")); changename(node, "fsckswap");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Record Max Sectors")); changename(node, "RecordMaxSectors");
			tmpsize = 80;
			for(i=0; i < 13; i++)
			{
				char* tmpnr = NULL;
				tmpnr = oitoa(tmpsize);
				addchoicebox(node, tmpnr, _(tmpnr));
				free(tmpnr); tmpnr = NULL;
				tmpsize = tmpsize + 80;
			}
			setchoiceboxselection(node, getownconfig(node->name));
		}
		
		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Swap Max Sectors")); changename(node, "SwapMaxSectors");
			tmpsize = 80;
			for(i=0; i < 13; i++)
			{
				char* tmpnr = NULL;
				tmpnr = oitoa(tmpsize);
				addchoicebox(node, tmpnr, _(tmpnr));
				free(tmpnr); tmpnr = NULL;
				tmpsize = tmpsize + 80;
			}
			setchoiceboxselection(node, getownconfig(node->name));
		}

	}

	if (mode == 3){

		changetitle(panel_config, _("Autostart EMU"));

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Emu Control")); changename(node, "emucontrol");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("ftdi")); changename(node, "ftdi");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("pl2303")); changename(node, "pl2303");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

	}

	if (mode == 4){

		changetitle(panel_config, "Autostart NETWORK");

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("nfs server")); changename(node, "nfsserver");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Samba Server")); changename(node, "sambaserver");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("scan wlan")); changename(node, "wlan");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

	if (mode == 5){

		changetitle(panel_config, _("Autostart Child Safety"));

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Parental")); changename(node, "Parental");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}

		node = addlistbox(panel_config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = INPUTBOXNUM;
			changetext(node, _("Pin")); changename(node, "Pin");
			addchoicebox(node, "n", _("no"));
			if(getownconfig(node->name) != NULL)
				changeinput(node, getownconfig(node->name));
			else
				changeinput(node, "0000");
		}

	}

	drawscreen(panel_config, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(panel_config, tmp);
		rcret = waitrc(panel_config, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcgreen", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(panel_config, 1);
	delownerrc(panel_config);
	clearscreen(panel_config);
}

#endif
