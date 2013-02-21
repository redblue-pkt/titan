#ifndef PANEL_SETTINGS_H
#define PANEL_SETTINGS_H

// mode 0 = Betaconfig
// mode 1 = Automount
// mode 2 = Red Button

void screenpanel_settings(int mode)
{
	int rcret = 0;
	struct skin* config = getscreen("panel_config");
	struct skin* listbox = getscreennode(config, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(config, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	if (mode == 0){
		changetitle(config, _("Automount Config"));

		node = addlistbox(config, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Ntfs Write"));
			changename(node, "ntfs_write");
			addchoicebox(node, "0", _("disable"));
			addchoicebox(node, "1", _("ntfs-3g"));
			addchoicebox(node, "2", _("ntfsmount"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

	drawscreen(config, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(config, tmp);
		rcret = waitrc(config, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			if(mode == 1 || mode == 2){
				addownconfigscreentmpcheck(listbox->select->name, listbox->select, "0");
			}else{
				addownconfigscreenqtmp(listbox->select->name, listbox->select);
			}
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {

			break;
		}
		if(rcret == getrcconfigint("rcgreen", NULL)) {
			writeownconfigtmp();
			break;
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(config, 1);
	delownerrc(config);
	clearscreen(config);
}

#endif
