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
	struct skin* oled_sel = getscreennode(vfdisplay, "oled_sel");
	struct skin* blinkoff = getscreennode(vfdisplay, "blinkoff");
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

	if(checkchipset("BCM7424") == 1) // inihdp
	{
		char *tmp1 = NULL, *tmp2 = NULL;		
		int i = 0;		
		if(checkscreen("OLED_nemesis") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis", "v1");		
		for (i=2;i<30;i++)
		{
			tmp1 = ostrcat("OLED_nemesis_v",oitoa(i), 0, 1);
			tmp2 = ostrcat("v",oitoa(i), 0, 1);
			if(checkscreen(tmp1) != status.skinerr)
   				addchoicebox(oled_sel, tmp1,tmp2);
			free(tmp1);tmp1=NULL;
			free(tmp2);tmp2=NULL;
		}
		
		setchoiceboxselection(oled_sel, getskinconfig("OLED_nemesis", NULL));
		addchoicebox(blinkoff, "0", _("on"));
		addchoicebox(blinkoff, "1", _("off"));
		setchoiceboxselection(blinkoff, getconfig("skinblinkoff", NULL));
	}
	else if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("ATEMIO6200") == 1)
	{
		changetext(blinkoff, _("continuous text"));
		addchoicebox(blinkoff, "1", _("on"));
		addchoicebox(blinkoff, "0", _("off"));
		setchoiceboxselection(blinkoff, getconfig("vfd_scroll", NULL));
	}
	else if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
	{
		char *tmp1 = NULL, *tmp2 = NULL;		
		int i = 0;		
		if(checkscreen("OLED_dream1") != status.skinerr)
			addchoicebox(oled_sel, "OLED_dream1", "v1");		
		for (i=2;i<30;i++)
		{
			tmp1 = ostrcat("OLED_dream1_v",oitoa(i), 0, 1);
			tmp2 = ostrcat("v",oitoa(i), 0, 1);
			if(checkscreen(tmp1) != status.skinerr)
   				addchoicebox(oled_sel, tmp1,tmp2);
			free(tmp1);tmp1=NULL;
			free(tmp2);tmp2=NULL;
		}
		
		setchoiceboxselection(oled_sel, getskinconfig("OLED_dream1", NULL));
		addchoicebox(blinkoff, "0", _("on"));
		addchoicebox(blinkoff, "1", _("off"));
		setchoiceboxselection(blinkoff, getconfig("skinblinkoff", NULL));
	}	
	else if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
	{
		char *tmp1 = NULL, *tmp2 = NULL;		
		int i = 0;		
		if(checkscreen("OLED_dream2") != status.skinerr)
			addchoicebox(oled_sel, "OLED_dream2", "v1");		
		for (i=2;i<30;i++)
		{
			tmp1 = ostrcat("OLED_dream2_v",oitoa(i), 0, 1);
			tmp2 = ostrcat("v",oitoa(i), 0, 1);
			if(checkscreen(tmp1) != status.skinerr)
   				addchoicebox(oled_sel, tmp1,tmp2);
			free(tmp1);tmp1=NULL;
			free(tmp2);tmp2=NULL;
		}
		
		setchoiceboxselection(oled_sel, getskinconfig("OLED_dream2", NULL));
		addchoicebox(blinkoff, "0", _("on"));
		addchoicebox(blinkoff, "1", _("off"));
		setchoiceboxselection(blinkoff, getconfig("skinblinkoff", NULL));
	}	
	else if(checkbox("VUDUO4K") == 1 || checkbox("VUUNO4K") == 1  || checkbox("VUULTIMO4K") == 1)
	{
		char *tmp1 = NULL, *tmp2 = NULL;		
		int i = 0;		
		if(checkscreen("OLED_vu1") != status.skinerr)
			addchoicebox(oled_sel, "OLED_vu1", "v1");		
		for (i=2;i<30;i++)
		{
			tmp1 = ostrcat("OLED_vu1_v",oitoa(i), 0, 1);
			tmp2 = ostrcat("v",oitoa(i), 0, 1);
			if(checkscreen(tmp1) != status.skinerr)
   				addchoicebox(oled_sel, tmp1,tmp2);
			free(tmp1);tmp1=NULL;
			free(tmp2);tmp2=NULL;
		}
		
		setchoiceboxselection(oled_sel, getskinconfig("OLED_vu1", NULL));
		addchoicebox(blinkoff, "0", _("on"));
		addchoicebox(blinkoff, "1", _("off"));
		setchoiceboxselection(blinkoff, getconfig("skinblinkoff", NULL));
	}	
	else
	{
		oled_sel->hidden = YES;
		blinkoff->hidden = YES;
	}

	if(checkbox("ATEMIO530") == 0 && checkbox("ATEMIO520") == 0 && checkbox("IPBOX91") == 0 && checkbox("ATEMIO6000") == 0 && checkbox("ATEMIO6100") == 0 && checkbox("SPARK") == 0)
	{
		addchoicebox(vfdnotstandby, "0", _("channel"));
		addchoicebox(vfdnotstandby, "1", _("channel + time"));
		addchoicebox(vfdnotstandby, "2", _("time + channel"));
		addchoicebox(vfdnotstandby, "3", _("time"));
	}
	if(checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1)
		addchoicebox(vfdnotstandby, "3", _("time"));
	addchoicebox(vfdnotstandby, "4", _("time small"));
	addchoicebox(vfdnotstandby, "5", _("channel number"));
	setchoiceboxselection(vfdnotstandby, getconfig("vfdisplay", NULL));

	addchoicebox(vfdstandby, "0", _("time"));
	addchoicebox(vfdstandby, "1", _("off"));
	if(checkbox("ATEMIO530") == 0 && checkbox("ATEMIO520") == 0 && checkbox("IPBOX91") == 0 && checkbox("ATEMIO6000") == 0 && checkbox("ATEMIO6100") == 0 && checkbox("SPARK") == 0)
	{
		addchoicebox(vfdstandby, "2", _("date + time"));
		addchoicebox(vfdstandby, "3", _("vfddate"));
	}
	setchoiceboxselection(vfdstandby, getconfig("vfdisplaystandby", NULL));

//record

	if(checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("VUDUO4K") == 1 || checkbox("VUUNO4K") == 1  || checkbox("VUULTIMO4K") == 1) //inihdp
		vfdrecord->hidden = YES;
	else
	{
		vfdrecord->hidden = NO;
		addchoicebox(vfdrecord, "0", _("off"));
		addchoicebox(vfdrecord, "1", _("blink"));
		if(checkbox("VUZERO4K") != 1)
		{
			addchoicebox(vfdrecord, "2", _("fade out/in"));
			addchoicebox(vfdrecord, "3", _("show record"));
			addchoicebox(vfdrecord, "4", _("star"));
			addchoicebox(vfdrecord, "5", _("blink star"));
		}
		setchoiceboxselection(vfdrecord, getconfig("vfdisplayrecord", NULL));
	}

//powerled

	if(checkbox("ATEVIO7000") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("SPARK") == 1)
	{
		changeinput(at7000frontrun, "15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0");
		setchoiceboxselection(at7000frontrun, getconfig("at7000frontrun", NULL));

		changeinput(at7000frontsleep, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15");
		setchoiceboxselection(at7000frontsleep, getconfig("at7000frontsleep", NULL));
	}

	if(checkbox("ATEVIO7000") == 1 || checkbox("SPARK") == 1)
	{
		at7000frontrun->hidden = NO;
		at7000frontsleep->hidden = NO;
	}
	else if(checkbox("ATEMIO7600") == 1)
		at7000frontrun->hidden = NO;
	else
	{
		at7000frontrun->hidden = YES;
		at7000frontsleep->hidden = YES;
	}

	if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
	{
		addchoicebox(at7000frontrun, "0", _("off"));
		addchoicebox(at7000frontrun, "1", _("red"));
		addchoicebox(at7000frontrun, "2", _("blue"));
		setchoiceboxselection(at7000frontrun, getconfig("dm900frontrun", NULL));
		addchoicebox(at7000frontsleep, "0", _("off"));
		addchoicebox(at7000frontsleep, "1", _("red"));
		addchoicebox(at7000frontsleep, "2", _("blue"));
		setchoiceboxselection(at7000frontsleep, getconfig("dm900frontsleep", NULL));
		changetext(at7000frontrun, _("LED when running"));
		changetext(at7000frontsleep, _("LED when sleeping"));
		at7000frontrun->hidden = NO;
		at7000frontsleep->hidden = NO;
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
		
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) &&  (checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkchipset("BCM7424") == 1 || checkbox("VUDUO4K") == 1 || checkbox("VUUNO4K") == 1  || checkbox("VUULTIMO4K") == 1) && listbox->select != NULL && ostrcmp(listbox->select->name, "oled_sel") == 0) // inihdp
		{
			tmpstr = ostrcat(tmpstr, oled_sel->ret, 0, 0);
			struct skin* OLED_all = getscreen(tmpstr);
			if(status.skinerr == OLED_all)
			{
				if(checkchipset("BCM7424") == 1)
					OLED_all = getscreen("OLED_nemesis");
				else if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
					OLED_all = getscreen("OLED_dream1");
				else if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
					OLED_all = getscreen("OLED_dream2");
				else if(checkbox("VUDUO4K") == 1 || checkbox("VUUNO4K") == 1  || checkbox("VUULTIMO4K") == 1)
					OLED_all = getscreen("OLED_vu1");
			}	
			struct skin* textbox = getscreennode(OLED_all, "textbox");
			changetext(textbox, tmpstr);
			drawscreen(OLED_all, 0, 0);
			free(tmpstr);tmpstr=NULL;
		}
		
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
			debug(10, "add vfdbrightness %d", vfdbrightness);
			debug(10, "add vfdstandbybrightness %d", vfdstandbybrightness);
			
			if(checkbox("ATEVIO7000") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("SPARK") == 1)
			{
				addconfigscreencheck("at7000frontrun", at7000frontrun, "0");
				addconfigscreencheck("at7000frontsleep", at7000frontsleep, "0");
				tmpstr = ostrcat("fp_control -P ",getconfig("at7000frontrun", NULL), 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr=NULL;
			}
			if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
			{
				addconfigscreencheck("dm900frontrun", at7000frontrun, "0");
				addconfigscreencheck("dm900frontsleep", at7000frontsleep, "0");
				addskinconfigscreencheck("OLED_dream2", oled_sel, "0");
				addconfig("skinblinkoff", blinkoff->ret);
				setled(1);
			}
			if(checkbox("VUDUO4K") == 1 || checkbox("VUUNO4K") == 1  || checkbox("VUULTIMO4K") == 1)
			{
				addskinconfigscreencheck("OLED_dream2", oled_sel, "0");
				addconfig("skinblinkoff", blinkoff->ret);
			}
			if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("ATEMIO6200") == 1)
			{
				addconfig("vfd_scroll", blinkoff->ret);
			}
			if(checkchipset("BCM7424") == 1) //inihdp
			{
				addskinconfigscreencheck("OLED_nemesis", oled_sel, "0");
				addconfig("skinblinkoff", blinkoff->ret);
			}
			else if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1) //inihdp
			{
				addskinconfigscreencheck("OLED_dream1", oled_sel, "0");
				addconfig("skinblinkoff", blinkoff->ret);
			}
			break;
		}
	}

	setvfdbrightness(getconfigint("vfdbrightness", NULL));
	delownerrc(vfdisplay);
	clearscreen(vfdisplay);
}

#endif
