#ifndef VFDISPLAY_H
#define VFDISPLAY_H

void screenvfdisplay()
{
	int rcret = 0, vfdbrightness = 0, vfdstandbybrightness = 0;
	struct skin* vfdisplay = getscreen("vfdisplay");
	struct skin* listbox = getscreennode(vfdisplay, "listbox");
	struct skin* brightness = getscreennode(vfdisplay, "brightness");
	struct skin* standby = getscreennode(vfdisplay, "standby");
	struct skin* vfdnotstandby = getscreennode(vfdisplay, "vfdnotstandby");
	struct skin* vfdstandby = getscreennode(vfdisplay, "vfdstandby");
	struct skin* tmp = NULL;

	if(brightness != NULL)
	{
		vfdbrightness = getconfigint("vfdbrightness", NULL);
		brightness->progresssize = (int)ceil(((float)vfdbrightness * 14.28));
	}
	if(standby != NULL)
	{
		vfdstandbybrightness = getconfigint("vfdstandbybrightness", NULL);
		standby->progresssize = (int)ceil(((float)vfdstandbybrightness * 14.28));
	}

	addchoicebox(vfdnotstandby, "0", _("channel"));
	addchoicebox(vfdnotstandby, "1", _("channel + time"));
	addchoicebox(vfdnotstandby, "2", _("time + channel"));
	addchoicebox(vfdnotstandby, "3", _("time"));
	addchoicebox(vfdnotstandby, "4", _("time small"));
	setchoiceboxselection(vfdnotstandby, getconfig("vfdisplay", NULL));

	addchoicebox(vfdstandby, "0", _("time"));
	addchoicebox(vfdstandby, "1", _("off"));
	setchoiceboxselection(vfdstandby, getconfig("vfdisplaystandby", NULL));

	drawscreen(vfdisplay, 0, 0);
	addscreenrc(vfdisplay, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(vfdisplay, tmp);
		rcret = waitrc(vfdisplay, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(listbox->select != NULL && ostrcmp(listbox->select->name, "brightness") == 0)
		{
			if(rcret == getrcconfigint("rcright", NULL))
			{
				vfdbrightness++;
				brightness->progresssize = (int)ceil(((float)vfdbrightness * 14.28));
				if(brightness->progresssize > 100) brightness->progresssize = 100;
				if(vfdbrightness > 7) vfdbrightness = 7;
				setvfdbrightness(vfdbrightness);
				drawscreen(vfdisplay, 0, 0);
			}
			if(rcret == getrcconfigint("rcleft", NULL))
			{
				vfdbrightness--;
				brightness->progresssize = (int)ceil(((float)vfdbrightness * 14.28));
				if(brightness->progresssize < 0) brightness->progresssize = 0;
				if(vfdbrightness < 0) vfdbrightness = 0;
				setvfdbrightness(vfdbrightness);
				drawscreen(vfdisplay, 0, 0);
			}
		}
		if(listbox->select != NULL && ostrcmp(listbox->select->name, "standby") == 0)
		{
			if(rcret == getrcconfigint("rcright", NULL))
			{
				vfdstandbybrightness++;
				standby->progresssize = (int)ceil(((float)vfdstandbybrightness * 14.28));
				if(standby->progresssize > 100) standby->progresssize = 100;
				if(vfdstandbybrightness > 7) vfdstandbybrightness = 7;
				drawscreen(vfdisplay, 0, 0);
			}
			if(rcret == getrcconfigint("rcleft", NULL))
			{
				vfdstandbybrightness--;
				standby->progresssize = (int)ceil(((float)vfdstandbybrightness * 14.28));
				if(standby->progresssize < 0) standby->progresssize = 0;
				if(vfdstandbybrightness < 0) vfdstandbybrightness = 0;
				drawscreen(vfdisplay, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("vfdisplay", vfdnotstandby, "0");
			addconfigscreencheck("vfdisplaystandby", vfdstandby, "0");
			addconfigint("vfdbrightness", vfdbrightness);
			addconfigint("vfdstandbybrightness", vfdstandbybrightness);
			break;
		}
	}

	setvfdbrightness(getconfigint("vfdbrightness", NULL));
	delownerrc(vfdisplay);
	clearscreen(vfdisplay);
}

#endif
