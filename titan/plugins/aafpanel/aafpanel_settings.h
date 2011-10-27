#ifndef AAFPANEL_SETTINGS_H
#define AAFPANEL_SETTINGS_H

// mode 0 = Betaconfig
// mode 1 = Automount
// mode 2 = Red Button

void screenaafpanel_settings(int mode)
{
	int rcret = 0;
	struct skin* aaf_config = getscreen("aafpanel_config");
	struct skin* listbox = getscreennode(aaf_config, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	addscreenrc(aaf_config, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	if (mode == 0){
		changetitle(aaf_config, _("AAF Beta Config"));

		node = addlistbox(aaf_config, listbox, node, 1);
		node->type = INPUTBOX;
		changetext(node, _("User"));
		changename(node, "betauser");
		changeinput(node, getownconfigq(node->name));

		node = addlistbox(aaf_config, listbox, node, 1);
		node->type = INPUTBOX;
		changetext(node, _("Password"));
		changename(node, "betapass");
		changeinput(node, getownconfigq(node->name));
	}

	if (mode == 1){
		changetitle(aaf_config, _("AAF Automount Config"));

		node = addlistbox(aaf_config, listbox, node, 1);
		node->type = CHOICEBOX;
		changetext(node, _("Ntfs Write"));
		changename(node, "ntfs_write");
		addchoicebox(node, "0", _("disable"));
		addchoicebox(node, "1", _("ntfs-3g"));
		addchoicebox(node, "2", _("ntfsmount"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	if (mode == 2){
		changetitle(aaf_config, _("AAF Red Button Config"));

		node = addlistbox(aaf_config, listbox, node, 1);
		node->type = CHOICEBOX;
		changetext(node, _("Open AAF-Panel when pressing red button:"));
		changename(node, "redpanel");
		addchoicebox(node, "0", _("no"));
		addchoicebox(node, "1", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	drawscreen(aaf_config, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(aaf_config, tmp);
		rcret = waitrc(aaf_config, 0, 0);
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
	delmarkedscreennodes(aaf_config, 1);
	delownerrc(aaf_config);
	clearscreen(aaf_config);
}

#endif
