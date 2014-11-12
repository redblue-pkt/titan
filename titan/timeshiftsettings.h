#ifndef TIMESHIFTSETTINGS_H
#define TIMESHIFTSETTINGS_H

void timeshiftplaydisable()
{
	int end = 0;
	struct skin* node = skin;
	struct skin* child = NULL;

	while(node != NULL)
	{
		child = node->child;
		while(child != NULL)
		{
			if(ostrcmp("mediaplayer", child->name) == 0)
			{
				if(status.timeshifttype == 0)
					child->locked = 1;
				else if(status.timeshifttype == 1)
					child->locked = 0;
				end = 1;
				break;
			}
			child = child->next;
		}
		if(end == 1) break;
		node = node->next;
	}
}

void screentimeshiftsettings()
{
	int rcret = 0, ret = 0, change = 0;
	
	struct skin* timeshiftsettings = getscreen("timeshiftsettings");
	struct skin* listbox = getscreennode(timeshiftsettings, "listbox");
	struct skin* timeshifttype = getscreennode(timeshiftsettings, "timeshifttype");	
	struct skin* deltimeshift = getscreennode(timeshiftsettings, "deltimeshift");
	struct skin* asktimeshift = getscreennode(timeshiftsettings, "asktimeshift");
  struct skin* timeshiftnumkeys = getscreennode(timeshiftsettings, "timeshiftnumkeys");
	struct skin* tmp = NULL;
	
	if(status.timeshifttype == 0 && status.timeshift > 0)
	{
		textbox(_("Message"), _("Timeshiftsettings not allowed\nif timeshift is running !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		return;
	}
	
	addchoicebox(timeshifttype, "0", _("normal"));
	addchoicebox(timeshifttype, "1", _("permanent"));
	setchoiceboxselection(timeshifttype, getconfig("timeshifttype", NULL));

	addchoicebox(deltimeshift, "0", _("ask"));
	addchoicebox(deltimeshift, "1", _("allways del file"));
	addchoicebox(deltimeshift, "2", _("never del file"));
	setchoiceboxselection(deltimeshift, getconfig("deltimeshift", NULL));

	addchoicebox(asktimeshift, "0", _("yes"));
	addchoicebox(asktimeshift, "1", _("no"));
	setchoiceboxselection(asktimeshift, getconfig("asktimeshift", NULL));
  
	addchoicebox(timeshiftnumkeys, "0", _("yes"));
	addchoicebox(timeshiftnumkeys, "1", _("no"));
	setchoiceboxselection(timeshiftnumkeys, getconfig("timeshiftnumkeys", NULL));
	
	drawscreen(timeshiftsettings, 0, 0);
	addscreenrc(timeshiftsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(timeshiftsettings, tmp);
		rcret = waitrc(timeshiftsettings, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("deltimeshift", deltimeshift, "0");
			
			//check if timeshifttype change
			if(ostrcmp(timeshifttype->ret, getconfig("timeshifttype", NULL)) != 0)
				change = 1;
			if(ostrcmp(timeshifttype->ret, "0") == 0 && getconfig("timeshifttype", NULL) == NULL)
				change = 0;
			
			addconfigscreencheck("timeshifttype", timeshifttype, "0");
			status.timeshifttype = getconfigint("timeshifttype", NULL);
			addconfigscreencheck("asktimeshift", asktimeshift, "0");
			status.asktimeshift = getconfigint("asktimeshift", NULL);
			addconfigscreencheck("timeshiftnumkeys", timeshiftnumkeys, "0");
			break;
		}
	}

	timeshiftplaydisable();
	delownerrc(timeshiftsettings);
	clearscreen(timeshiftsettings);
	drawscreen(skin, 0, 0);

	//zap on change so timeshift is ended or started
	if(change == 1)
	{
		resettvpic();

		ret = servicestop(status.aktservice, 1, 1);
		if(ret == 0)
		{
			status.aktservice->transponder = NULL;
			servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 5), 0);
		}
	}
}

#endif
