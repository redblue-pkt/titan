#ifndef SETTINGS_AUTOSTART_H
#define SETTINGS_AUTOSTART_H

void screensettings_autostart_default()
{
	int rcret = 0;
	struct skin* settings_autostart = getscreen("settings_autostart_default");
	struct skin* listbox = getscreennode(settings_autostart, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(settings_autostart, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

//	int i = 0;
//	changetitletext(settings_autostart, _("Autostart Defaults"));

	if(file_exist("/etc/.usbimage"))
	{
		node = addlistbox(settings_autostart, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("fsckroot")); changename(node, "fsckroot");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

/*
	node = addlistbox(settings_autostart, listbox, node, 1);
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
*/
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("debug")); changename(node, "debug");
		addchoicebox(node, "high", _("high")); addchoicebox(node, "low", _("low"));
		addchoicebox(node, "off", _("off"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("showip")); changename(node, "showip");
		addchoicebox(node, "status", _("Status")); addchoicebox(node, "ip", _("ip"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
/*
	node = addlistbox(settings_autostart, listbox, node, 1);
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
*/

	drawscreen(settings_autostart, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(settings_autostart, tmp);
		rcret = waitrc(settings_autostart, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(settings_autostart, 1);
	delownerrc(settings_autostart);
	clearscreen(settings_autostart);
}

void screensettings_autostart_audiovideo()
{
	int rcret = 0;
	struct skin* settings_autostart = getscreen("settings_autostart_audiovideo");
	struct skin* listbox = getscreennode(settings_autostart, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(settings_autostart, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

//		changetitletext(settings_autostart, _("Autostart Defaults"));

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Bootlogo")); changename(node, "bootlogo");
		addchoicebox(node, "y", _("yes")); addchoicebox(node, "n", _("no"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Boot RGB Fix")); changename(node, "bootrgbfix");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
/*
	if(isfile("/var/bin/audio.elf") || isfile("/boot/audio.old.elf"))
	{
		node = addlistbox(settings_autostart, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Old Audiofw")); changename(node, "oldaudiofw");
			addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}
*/
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("No Audiosync")); changename(node, "noaudiosync");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
/*
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Boot Resolution")); changename(node, "BootResolution");
		addchoicebox(node, "low", _("low")); addchoicebox(node, "high", _("high"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Poweroff")); changename(node, "poweroff");
		addchoicebox(node, "y", _("yes")); addchoicebox(node, "n", _("no"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Scart On Standby")); changename(node, "scartonstandby");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
*/
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("HighSR")); changename(node, "HighSR");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("swts")); changename(node, "swts");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Cam Routing")); changename(node, "camrouting");
		addchoicebox(node, "0", _("no")); addchoicebox(node, "1", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	if(checkbox("UFS910") == 1)
	{
		node = addlistbox(settings_autostart, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("useUnknown910")); changename(node, "useUnknown910");
			addchoicebox(node, "1", _("yes")); addchoicebox(node, "0", _("no"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

	drawscreen(settings_autostart, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(settings_autostart, tmp);
		rcret = waitrc(settings_autostart, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(settings_autostart, 1);
	delownerrc(settings_autostart);
	clearscreen(settings_autostart);
}

void screensettings_autostart_usb()
{
	int rcret = 0;
	struct skin* settings_autostart = getscreen("settings_autostart_usb");
	struct skin* listbox = getscreennode(settings_autostart, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(settings_autostart, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	int i = 0;
	int tmpsize = 0;

//		changetitletext(settings_autostart, _("Autostart USB"));

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Load Partition")); changename(node, "loadpartition");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
/*
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Fsck Record")); changename(node, "fsckrecord");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
*/

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Ntfs Write"));
		changename(node, "ntfs_write");
		addchoicebox(node, "0", _("no"));
		addchoicebox(node, "1", _("ntfs-3g"));
		addchoicebox(node, "2", _("ntfsmount"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
		
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Swap")); changename(node, "swap");
		addchoicebox(node, "y", _("yes")); addchoicebox(node, "n", _("no"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
	
	node = addlistbox(settings_autostart, listbox, node, 1);
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
/*
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Fsck Swap")); changename(node, "fsckswap");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
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
	
	node = addlistbox(settings_autostart, listbox, node, 1);
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
*/
	drawscreen(settings_autostart, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(settings_autostart, tmp);
		rcret = waitrc(settings_autostart, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(settings_autostart, 1);
	delownerrc(settings_autostart);
	clearscreen(settings_autostart);
}

void screensettings_autostart_emu()
{
	int rcret = 0;
	struct skin* settings_autostart = getscreen("settings_autostart_emu");
	struct skin* listbox = getscreennode(settings_autostart, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(settings_autostart, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

//		changetitletext(settings_autostart, _("Autostart EMU"));

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Emu Control")); changename(node, "emucontrol");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("ftdi")); changename(node, "ftdi");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("pl2303")); changename(node, "pl2303");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	drawscreen(settings_autostart, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(settings_autostart, tmp);
		rcret = waitrc(settings_autostart, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(settings_autostart, 1);
	delownerrc(settings_autostart);
	clearscreen(settings_autostart);
}

void screensettings_autostart_safety()
{
	int rcret = 0;
	struct skin* settings_autostart = getscreen("settings_autostart_safety");
	struct skin* listbox = getscreennode(settings_autostart, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(settings_autostart, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

//		changetitletext(settings_autostart, _("Autostart Child Safety"));

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Parental")); changename(node, "Parental");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
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

	drawscreen(settings_autostart, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(settings_autostart, tmp);
		rcret = waitrc(settings_autostart, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(settings_autostart, 1);
	delownerrc(settings_autostart);
	clearscreen(settings_autostart);
}

void screensettings_autostart_network()
{
	int rcret = 0;
	struct skin* settings_autostart = getscreen("settings_autostart_network");
	struct skin* listbox = getscreennode(settings_autostart, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(settings_autostart, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

//		changetitletext(settings_autostart, "Autostart NETWORK");

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("nfs server")); changename(node, "nfsserver");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Samba Server")); changename(node, "sambaserver");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("scan wlan")); changename(node, "wlan");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("ntpdate")); changename(node, "ntpdate");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
	
	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("OpenVPN")); changename(node, "openvpn");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(settings_autostart, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Ipsec")); changename(node, "ipsec");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	drawscreen(settings_autostart, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(settings_autostart, tmp);
		rcret = waitrc(settings_autostart, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcok", NULL)) {
			writeownconfigtmp();
			writeallconfig(1);
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(settings_autostart, 1);
	delownerrc(settings_autostart);
	clearscreen(settings_autostart);
}

#endif
