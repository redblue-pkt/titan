#ifndef PIN_H
#define PIN_H

//type = 0 (allways)
//type = 1 (only if channelprotect yes)
//type = 2 (only if menuprotect yes)
int screenpincheck(int type, char* pin)
{
	if(type == 1)
	{
		if(getconfigint("channelprotect", NULL) == 0) return 0;
		if(time(NULL) < status.protecttime) return 0;
		if(pin != NULL && ostrcmp(pin, getconfig("pincode", NULL)) == 0) return 0;
	}
	if(type == 2 && getconfigint("menuprotect", NULL) == 0) return 0;

	int rcret = 0, ret = 1;
	struct skin* pincheck = getscreen("pincheck");

	changemask(pincheck, "****");
	changeinput(pincheck, "****");

	drawscreen(pincheck, 0, 0);
	addscreenrc(pincheck, pincheck);

	while(1)
	{
		rcret = waitrc(pincheck, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(type == 1)
			{
				if(ostrcmp(pincheck->ret, getconfig("pincode", NULL)) == 0)
				{
					status.protecttime = time(NULL) + (getconfigint("protecttime", NULL) * 60);
					ret = 0;
				}
			}
			else
			{
				if(ostrcmp(pincheck->ret, getconfig("pincodemenu", NULL)) == 0)
					ret = 0;
			}
			break;
		}
	}

	delownerrc(pincheck);
	clearscreen(pincheck);
	drawscreen(skin, 0, 0);
	return ret;
}

void screenmenuprotect()
{
	int rcret = 0;
	struct skin* node = skin;
	struct skin* child = NULL;
	struct skin* menuprotect = getscreen("menuprotect");
	struct skin* listbox = getscreennode(menuprotect, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	while(node != NULL)
	{
		child = node->child;
		while(child != NULL)
		{
			if(child->type & MENU)
			{
				tmp = addlistbox(menuprotect, listbox, tmp, 1);
				if(tmp != NULL)
				{
					changetext(tmp, child->text);
					changename(tmp, child->name);
					tmp->type = CHOICEBOX;
					tmp->del = 1;
					tmp->height = listbox->fontsize + 2;
					addchoicebox(tmp, "0", _("no"));
					addchoicebox(tmp, "1", _("yes"));
					addchoicebox(tmp, "2", _("hidden"));
					tmpstr = ostrcat("protect_", tmp->name, 0, 0);
					setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
					free(tmpstr);
				}
			}
			child = child->next;
		}
		node = node->next;
	}

	drawscreen(menuprotect, 0, 0);
	addscreenrc(menuprotect, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(menuprotect, tmp);
		rcret = waitrc(menuprotect, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			node = menuprotect->child;
			while(node != NULL)
			{
				tmpstr = ostrcat("protect_", node->name, 0, 0);
				addconfigscreencheck(tmpstr, node, "0");
				free(tmpstr); tmpstr = NULL;
				node = node->next;
			}
			break;
		}
	}

	delmarkedscreennodes(menuprotect, 1);
	delownerrc(menuprotect);
	clearscreen(menuprotect);
}

void screenpin()
{
	char*tmpstr = NULL;

	tmpstr = ostrcat("protect_", "childprotection", 0, 0);
	if(getconfigint(tmpstr, NULL) == 0)
	{
		int pinret = screenpincheck(0, NULL);
		if(pinret == 1) return;
	}
	free(tmpstr); tmpstr = NULL;

	int rcret = 0;
	struct skin* pin = getscreen("pin");
	struct skin* listbox = getscreennode(pin, "listbox");
	struct skin* pincode = getscreennode(pin, "pincode");
	struct skin* pincodemenu = getscreennode(pin, "pincodemenu");
	struct skin* channelprotect = getscreennode(pin, "channelprotect");
	struct skin* ageprotect = getscreennode(pin, "ageprotect");
	struct skin* menuprotect = getscreennode(pin, "menuprotect");
	struct skin* protecttime = getscreennode(pin, "protecttime");
	struct skin* tmp = NULL;

	changemask(pincodemenu, "0000");
	changeinput(pincodemenu, getconfig("pincodemenu", NULL));

	changemask(pincode, "0000");
	changeinput(pincode, getconfig("pincode", NULL));

	addchoicebox(channelprotect, "0", _("no"));
	addchoicebox(channelprotect, "1", _("yes"));
	setchoiceboxselection(channelprotect, getconfig("channelprotect", NULL));
	
	addchoicebox(protecttime, "0", _("no"));
	addchoicebox(protecttime, "30", "30");
	addchoicebox(protecttime, "60", "60");
	addchoicebox(protecttime, "90", "90");
	addchoicebox(protecttime, "120", "120");
	addchoicebox(protecttime, "150", "150");
	addchoicebox(protecttime, "180", "180");
	addchoicebox(protecttime, "210", "210");
	addchoicebox(protecttime, "30000000", _("endless"));
	setchoiceboxselection(protecttime, getconfig("protecttime", NULL));

	changeinput(ageprotect, "0\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18");
	setchoiceboxselection(ageprotect, getconfig("ageprotect", NULL));

	addchoicebox(menuprotect, "0", _("no"));
	addchoicebox(menuprotect, "1", _("yes"));
	setchoiceboxselection(menuprotect, getconfig("menuprotect", NULL));

	drawscreen(pin, 0, 0);
	addscreenrc(pin, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(pin, tmp);
		rcret = waitrc(pin, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			clearscreen(pin);
			screenmenuprotect();
			drawscreen(pin, 0, 0);
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreen("pincodemenu", pincodemenu);
			addconfigscreen("pincode", pincode);
			addconfigscreencheck("channelprotect", channelprotect, "0");
			addconfigscreencheck("ageprotect", ageprotect, "0");
			addconfigscreencheck("menuprotect", menuprotect, "0");
			addconfigscreencheck("protecttime", protecttime, "0");
			break;
		}
	}

	delownerrc(pin);
	clearscreen(pin);
}

#endif
