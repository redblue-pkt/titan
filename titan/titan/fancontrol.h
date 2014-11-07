#ifndef FANCONTROL_H
#define FANCONTROL_H

int getspeedWert(char* speedWert)
{
	int speed = 0;
	
	if (speedWert != NULL) 
	{
		if(ostrcmp(speedWert, "115") == 0)
			speed = 0;
		else if(ostrcmp(speedWert, "130") == 0)
			speed = 25;
		else if(ostrcmp(speedWert, "145") == 0)
			speed = 50;
		else if(ostrcmp(speedWert, "155") == 0)
			speed = 75;
		else if(ostrcmp(speedWert, "170") == 0)
			speed = 100;
	}
	else 
		speed = 100;
	return speed;
}

void screensettings_fancontrol()
{
	int rcret = 0;
	int speed = 0;
	int type = 0;
	char* speedWert = NULL;
	
	struct skin* fancontrol = getscreen("settings_fancontrol");
	struct skin* fanprogress = getscreennode(fancontrol, "fanprogress");
	struct skin* titletext = getscreennode(fancontrol, "titletext");
	struct skin* b3 = getscreennode(fancontrol, "b3");

	speedWert = getconfig("fanspeed", NULL);
	speed = getspeedWert(speedWert);
		
	type = 1; //speed normal
	changetext(titletext, "FanControl-normal");
	fanprogress->progresssize = speed;
	drawscreen(fancontrol, 0, 0);

	while(1)
	{
		rcret = waitrc(fancontrol, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			setfanspeed(-1, 0);
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			setfanspeed(speed, type);
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL)){
			if(type == 1)
			{
				type = 2;
				changetext(titletext, _("FanControl-standby"));
				changetext(b3, _("normal"));
				speed = getspeedWert(getconfig("fanspeedstandby", NULL));
			}
			else
			{
				type = 1;
				changetext(titletext, _("FanControl-normal"));
				changetext(b3, _("standby"));
				speed = getspeedWert(getconfig("fanspeed", NULL));
			}
			fanprogress->progresssize = speed;
			drawscreen(fancontrol, 0, 0);
		}
		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(speed > 0)
				speed = speed - 25;
			fanprogress->progresssize = speed;
			setfanspeed(speed, 0);
			drawscreen(fancontrol, 0, 0);
		}
		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(speed < 100)
				speed = speed + 25;
			fanprogress->progresssize = speed;
			setfanspeed(speed, 0);
			drawscreen(fancontrol, 0, 0);
		}
	}
	clearscreen(fancontrol);
}

#ifdef MIPSEL
void screensettings_fancontrol2()
{
	int rcret = 0;
	
	struct skin* tmp = NULL;
	struct skin* fancontrol = getscreen("settings_fancontrol2");
	struct skin* fanon = getscreennode(fancontrol, "fanon");
	struct skin* listbox = getscreennode(fancontrol, "listbox");

	addchoicebox(fanon, "3", _("auto"));
	addchoicebox(fanon, "2", _("on"));
	addchoicebox(fanon, "1", _("off"));
	setchoiceboxselection(fanon, getconfig("fanmode", NULL));

	addscreenrc(fancontrol, listbox);
	tmp = listbox->select;
	drawscreen(fancontrol, 0, 0);

	while(1)
	{
		addscreenrc(fancontrol, tmp);
		rcret = waitrc(fancontrol, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfig("fanmode", fanon->ret);
			break;
		}
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "fanon") == 0)
			writesys("/proc/stb/fp/fan", fanon->ret, 1);
	}
	writesys("/proc/stb/fp/fan", getconfig("fanmode", NULL), 1);
	clearscreen(fancontrol);
}
#endif

#endif