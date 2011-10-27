#ifndef LNBCONFIG_H
#define LNBCONFIG_H

int changeloftype(struct skin* lofl, struct skin* lofh, struct skin* threshold, char *type)
{
	if(ostrcmp(type, "2") == 0)
	{
		lofl->hidden = NO;
		lofh->hidden = NO;
		threshold->hidden = NO;
		return 2;
	}
	else
	{
		lofl->hidden = YES;
		lofh->hidden = YES;
		threshold->hidden = YES;
		return 0;
	}
}

void changelnb(struct skin* voltagemode, struct skin* tonemode, struct skin* loftype, struct skin* lofl, struct skin* lofh, struct skin* threshold, char *lnbnr)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat("lnb_voltagemode", lnbnr, 0, 0);
	changename(voltagemode, tmpstr);
	setchoiceboxselection(voltagemode, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("lnb_tonemode", lnbnr, 0, 0);
	changename(tonemode, tmpstr);
	setchoiceboxselection(tonemode, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("lnb_loftype", lnbnr, 0, 0);
	changename(loftype, tmpstr);
	setchoiceboxselection(loftype, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("lnb_lofl", lnbnr, 0, 0);
	changename(lofl, tmpstr);
	changemask(lofl, "000000");
	changeinput(lofl, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("lnb_lofh", lnbnr, 0, 0);
	changename(lofh, tmpstr);
	changemask(lofh, "000000");
	changeinput(lofh, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("lnb_lofthreshold", lnbnr, 0, 0);
	changename(threshold, tmpstr);
	changemask(threshold, "000000");
	changeinput(threshold, getconfig(tmpstr, NULL));
	free(tmpstr);
}

void screenlnb(char* lnbnr)
{
	int rcret = 0;
	struct skin* lnbscreen = getscreen("lnbscreen");
	struct skin* listbox = getscreennode(lnbscreen, "listbox");
	struct skin* lnb = getscreennode(lnbscreen, "lnb");
	struct skin* voltagemode = getscreennode(lnbscreen, "voltagemode");
	struct skin* tonemode = getscreennode(lnbscreen, "tonemode");
	struct skin* loftype = getscreennode(lnbscreen, "loftype");
	struct skin* lofl = getscreennode(lnbscreen, "lofl");
	struct skin* lofh = getscreennode(lnbscreen, "lofh");
	struct skin* threshold = getscreennode(lnbscreen, "threshold");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = 1;

	if(lnbnr == NULL)
	{
		tmpstr = getmaxsatstring();
		changeinput(lnb, tmpstr);
		lnb->aktpage = 1;
		free(tmpstr);
		lnbnr = "1";
	}
	else
	{
		changeinput(lnb, lnbnr);
		lnb->aktpage = atoi(lnbnr);
	}

	changeinput(voltagemode, NULL);
	addchoicebox(voltagemode, "0", _("Polarization"));
	addchoicebox(voltagemode, "1", _("13V"));
	addchoicebox(voltagemode, "2", _("18V"));

	changeinput(tonemode, NULL);
	addchoicebox(tonemode, "0", _("Band"));
	addchoicebox(tonemode, "1", _("On"));
	addchoicebox(tonemode, "2", _("Off"));

	changeinput(loftype, NULL);
	addchoicebox(loftype, "0", _("universal"));
	addchoicebox(loftype, "1", _("C-Band"));
	addchoicebox(loftype, "2", _("user defined"));

	changelnb(voltagemode, tonemode, loftype, lofl, lofh, threshold, lnbnr);
	changeloftype(lofl, lofh, threshold, getconfig("lnb_loftype", lnbnr));

	drawscreen(lnbscreen, 0);
	addscreenrc(lnbscreen, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(lnbscreen, tmp);
		status.screencalc = 2;
		rcret = waitrc(lnbscreen, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL && ostrcmp(listbox->select->name, "lnb") == 0 && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)))		
		{
			changelnb(voltagemode, tonemode, loftype, lofl, lofh, threshold, listbox->select->ret);
			changeloftype(lofl, lofh, threshold, getconfig("lnb_loftype", listbox->select->ret));
		}
		drawscreen(lnbscreen, 0);

		if(listbox->select != NULL && ostrncmp(listbox->select->name, "lnb_loftype", 11) == 0 && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)))
		{
			changeloftype(lofl, lofh, threshold, loftype->ret);
		}

		addconfigscreentmpcheck(voltagemode->name, voltagemode, "0");
		addconfigscreentmpcheck(tonemode->name, tonemode, "0");
		addconfigscreentmpcheck(loftype->name, loftype, "0");
		addconfigscreentmpcheck(lofl->name, lofl, "000000");
		addconfigscreentmpcheck(lofh->name, lofh, "000000");
		addconfigscreentmpcheck(threshold->name, threshold, "000000");

		status.screencalc = 0;
		drawscreen(lnbscreen, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			writeconfigtmp();
			break;
		}
	}

	changename(voltagemode, "voltagemode");
	changename(tonemode, "tonemode");
	changename(loftype, "loftype");
	changename(lofl, "lofl");
	changename(lofh, "lofh");
	changename(threshold, "threshold");

	status.screencalc = 0;
	delconfigtmpall();
	delownerrc(lnbscreen);
	clearscreen(lnbscreen);
}

#endif
