#ifndef VFDISPLAY_H
#define VFDISPLAY_H

void screenvfdisplay()
{
	char* tmpstr = NULL;
	int rcret = 0, vfdbrightness = 0, vfdstandbybrightness = 0;
	struct skin* vfdisplay = getscreen("vfdisplay");
	struct skin* listbox = getscreennode(vfdisplay, "listbox");
	struct skin* brightness = getscreennode(vfdisplay, "brightness");
	struct skin* standby = getscreennode(vfdisplay, "standby");
	struct skin* vfdnotstandby = getscreennode(vfdisplay, "vfdnotstandby");
	struct skin* vfdstandby = getscreennode(vfdisplay, "vfdstandby");
	struct skin* vfdrecord = getscreennode(vfdisplay, "vfdrecord");
	struct skin* at7000frontrun = getscreennode(vfdisplay, "at7000frontrun");
	struct skin* at7000frontsleep = getscreennode(vfdisplay, "at7000frontsleep");
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

	if(checkbox("ATEMIO530") == 0 && checkbox("ATEMIO520") == 0)
	{
		addchoicebox(vfdnotstandby, "0", _("channel"));
		addchoicebox(vfdnotstandby, "1", _("channel + time"));
		addchoicebox(vfdnotstandby, "2", _("time + channel"));
		addchoicebox(vfdnotstandby, "3", _("time"));
		addchoicebox(vfdnotstandby, "4", _("time small"));
	}
	addchoicebox(vfdnotstandby, "5", _("channel number"));
	setchoiceboxselection(vfdnotstandby, getconfig("vfdisplay", NULL));

	addchoicebox(vfdstandby, "0", _("time"));
	addchoicebox(vfdstandby, "1", _("off"));
	if(checkbox("ATEMIO530") == 0 && checkbox("ATEMIO520") == 0)
	{
		addchoicebox(vfdstandby, "2", _("date + time"));
		addchoicebox(vfdstandby, "3", _("date"));
	}
	setchoiceboxselection(vfdstandby, getconfig("vfdisplaystandby", NULL));
	
	addchoicebox(vfdrecord, "0", _("off"));
	addchoicebox(vfdrecord, "1", _("blink"));
	addchoicebox(vfdrecord, "2", _("fade out/in"));
	addchoicebox(vfdrecord, "3", _("show record"));
	addchoicebox(vfdrecord, "4", _("star"));
	addchoicebox(vfdrecord, "5", _("blink star"));
	setchoiceboxselection(vfdrecord, getconfig("vfdisplayrecord", NULL));

	changeinput(at7000frontrun, "15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
	setchoiceboxselection(at7000frontrun, getconfig("at7000frontrun", NULL));
	
	changeinput(at7000frontsleep, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15");
	setchoiceboxselection(at7000frontsleep, getconfig("at7000frontsleep", NULL));

	if(checkbox("ATEVIO7000") == 1)
	{
		at7000frontrun->hidden = NO;
		at7000frontsleep->hidden = NO;
	}
	else
	{
		at7000frontrun->hidden = YES;
		at7000frontsleep->hidden = YES;
	}

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
			addconfigscreencheck("vfdisplayrecord", vfdrecord, "0");
			addconfigint("vfdbrightness", vfdbrightness);
			addconfigint("vfdstandbybrightness", vfdstandbybrightness);

			if(checkbox("ATEVIO7000") == 1)
			{
				addconfigscreencheck("at7000frontrun", at7000frontrun, "0");
				addconfigscreencheck("at7000frontsleep", at7000frontsleep, "0");
				tmpstr = ostrcat("fp_control -P ",getconfig("at7000frontrun", NULL), 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr=NULL;
			}

			break;
		}
	}

	setvfdbrightness(getconfigint("vfdbrightness", NULL));
	delownerrc(vfdisplay);
	clearscreen(vfdisplay);
}

#endif
