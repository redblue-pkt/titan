#ifndef LNBCONFIG_H
#define LNBCONFIG_H

int changeloftype(struct skin* lofl, struct skin* lofh, struct skin* threshold, struct skin* satcr, struct skin* satcrfrequ1, struct skin* satcrfrequ2, char *type)
{
	if(ostrcmp(type, "4") == 0)
	{
		lofl->hidden = NO;
		lofh->hidden = NO;
		threshold->hidden = NO;
		satcr->hidden = NO;
		satcrfrequ1->hidden = YES;
		satcrfrequ2->hidden = NO;
		return 4;
	}
	if(ostrcmp(type, "3") == 0)
	{
		lofl->hidden = YES;
		lofh->hidden = YES;
		threshold->hidden = YES;
		satcr->hidden = NO;
		satcrfrequ1->hidden = NO;
		satcrfrequ2->hidden = YES;
		return 3;
	}
	if(ostrcmp(type, "2") == 0)
	{
		lofl->hidden = NO;
		lofh->hidden = NO;
		threshold->hidden = NO;
		satcr->hidden = YES;
		satcrfrequ1->hidden = YES;
		satcrfrequ2->hidden = YES;
		return 2;
	}
	else
	{
		lofl->hidden = YES;
		lofh->hidden = YES;
		threshold->hidden = YES;
		satcr->hidden = YES;
		satcrfrequ1->hidden = YES;
		satcrfrequ2->hidden = YES;
		return 0;
	}
}

void changelnb(struct skin* voltagemode, struct skin* tonemode, struct skin* loftype, struct skin* lofl, struct skin* lofh, struct skin* threshold, struct skin* satcr, struct skin* satcrfrequ1, struct skin* satcrfrequ2, char* feshortname, char *lnbnr)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(feshortname, "_lnb_voltagemode", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(voltagemode, tmpstr);
	setchoiceboxselection(voltagemode, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_tonemode", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(tonemode, tmpstr);
	setchoiceboxselection(tonemode, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_loftype", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(loftype, tmpstr);
	setchoiceboxselection(loftype, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_lofl", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(lofl, tmpstr);
	changemask(lofl, "000000");
	changeinput(lofl, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_lofh", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(lofh, tmpstr);
	changemask(lofh, "000000");
	changeinput(lofh, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_lofthreshold", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(threshold, tmpstr);
	changemask(threshold, "000000");
	changeinput(threshold, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_satcr", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(satcr, tmpstr);
	setchoiceboxselection(satcr, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_satcrfrequ", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(satcrfrequ1, tmpstr);
	setchoiceboxselection(satcrfrequ1, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_lnb_satcrfrequ", 0, 0);
	tmpstr = ostrcat(tmpstr, lnbnr, 1, 0);
	changename(satcrfrequ2, tmpstr);
	changemask(satcrfrequ2, "0000");
	changeinput(satcrfrequ2, getconfig(tmpstr, NULL));
	free(tmpstr);
}

void screenlnb(char* feshortname, char* lnbnr)
{
	int rcret = 0;
	struct skin* lnbscreen = getscreen("lnbscreen");
	struct skin* listbox = getscreennode(lnbscreen, "listbox");
	struct skin* voltagemode = getscreennode(lnbscreen, "voltagemode");
	struct skin* tonemode = getscreennode(lnbscreen, "tonemode");
	struct skin* loftype = getscreennode(lnbscreen, "loftype");
	struct skin* lofl = getscreennode(lnbscreen, "lofl");
	struct skin* lofh = getscreennode(lnbscreen, "lofh");
	struct skin* threshold = getscreennode(lnbscreen, "threshold");
	struct skin* satcr = getscreennode(lnbscreen, "satcr");
	struct skin* satcrfrequ1 = getscreennode(lnbscreen, "satcrfrequ1");
	struct skin* satcrfrequ2 = getscreennode(lnbscreen, "satcrfrequ2");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	if(lnbnr != NULL)
		lnbnr = oitoa((int)lnbnr);
	else
		return;

	listbox->aktline = 1;
	listbox->aktpage = 1;

	changeinput(voltagemode, NULL);
	addchoicebox(voltagemode, "0", _("Polarization"));
	addchoicebox(voltagemode, "1", _("13V"));
	addchoicebox(voltagemode, "2", _("18V"));

	changeinput(tonemode, NULL);
	addchoicebox(tonemode, "0", _("Band"));
	addchoicebox(tonemode, "1", _("on"));
	addchoicebox(tonemode, "2", _("off"));

	changeinput(loftype, NULL);
	addchoicebox(loftype, "0", _("universal"));
	addchoicebox(loftype, "1", _("C-Band"));
	addchoicebox(loftype, "2", _("user defined"));
	addchoicebox(loftype, "3", _("unicable"));
	addchoicebox(loftype, "4", _("user unicable"));

	changeinput(satcr, NULL);
	addchoicebox(satcr, "1", "1");
	addchoicebox(satcr, "2", "2");
	addchoicebox(satcr, "3", "3");
	addchoicebox(satcr, "4", "4");
	addchoicebox(satcr, "5", "5");
	addchoicebox(satcr, "6", "6");
	addchoicebox(satcr, "7", "7");
	addchoicebox(satcr, "8", "8");
	
	addchoicebox(satcr, "9", "9");
	addchoicebox(satcr, "10", "10");
	addchoicebox(satcr, "11", "11");
	addchoicebox(satcr, "12", "12");
	addchoicebox(satcr, "13", "13");
	addchoicebox(satcr, "14", "14");
	addchoicebox(satcr, "15", "15");
	addchoicebox(satcr, "16", "16");
	addchoicebox(satcr, "17", "17");
	addchoicebox(satcr, "18", "18");
	
	addchoicebox(satcr, "19", "19");
	addchoicebox(satcr, "20", "20");
	addchoicebox(satcr, "21", "21");
	addchoicebox(satcr, "22", "22");
	addchoicebox(satcr, "23", "23");
	addchoicebox(satcr, "24", "24");
	addchoicebox(satcr, "25", "25");
	addchoicebox(satcr, "26", "26");
	addchoicebox(satcr, "27", "27");
	addchoicebox(satcr, "28", "28");
	addchoicebox(satcr, "29", "25");
	addchoicebox(satcr, "30", "30");
	addchoicebox(satcr, "31", "31");
	addchoicebox(satcr, "32", "32");
	

	changeinput(satcrfrequ1, NULL);
	/*addchoicebox(satcrfrequ1, "1284", "1284");
	addchoicebox(satcrfrequ1, "1400", "1400");
	addchoicebox(satcrfrequ1, "1516", "1516");
	addchoicebox(satcrfrequ1, "1632", "1632");
	addchoicebox(satcrfrequ1, "1748", "1748");
	addchoicebox(satcrfrequ1, "1864", "1864");
	addchoicebox(satcrfrequ1, "1980", "1980");
	addchoicebox(satcrfrequ1, "2096", "2096");*/
	
	addchoicebox(satcrfrequ1, "984", "984");
	addchoicebox(satcrfrequ1, "1020", "1020");
	addchoicebox(satcrfrequ1, "1056", "1056");
	addchoicebox(satcrfrequ1, "1092", "1092");
	addchoicebox(satcrfrequ1, "1210", "1210");
	addchoicebox(satcrfrequ1, "1420", "1420");
	addchoicebox(satcrfrequ1, "1680", "1680");
	addchoicebox(satcrfrequ1, "2040", "2040");
	
	addchoicebox(satcrfrequ1, "1128", "1128");
	addchoicebox(satcrfrequ1, "1164", "1164");
	addchoicebox(satcrfrequ1, "1256", "1256");
	addchoicebox(satcrfrequ1, "1292", "1292");
	addchoicebox(satcrfrequ1, "1328", "1328");
	addchoicebox(satcrfrequ1, "1364", "1364");
	addchoicebox(satcrfrequ1, "1458", "1458");
	addchoicebox(satcrfrequ1, "1494", "1494");
	addchoicebox(satcrfrequ1, "1530", "1530");
	addchoicebox(satcrfrequ1, "1566", "1566");
	
	addchoicebox(satcrfrequ1, "1602", "1602");
	addchoicebox(satcrfrequ1, "1638", "1638");
	addchoicebox(satcrfrequ1, "1716", "1716");
	addchoicebox(satcrfrequ1, "1752", "1752");
	addchoicebox(satcrfrequ1, "1788", "1788");
	addchoicebox(satcrfrequ1, "1824", "1824");
	addchoicebox(satcrfrequ1, "1860", "1860");
	addchoicebox(satcrfrequ1, "1896", "1896");
	addchoicebox(satcrfrequ1, "1932", "1932");
	addchoicebox(satcrfrequ1, "1968", "1968");
	addchoicebox(satcrfrequ1, "2004", "2004");
	addchoicebox(satcrfrequ1, "2076", "2076");
	addchoicebox(satcrfrequ1, "2112", "2112");
	addchoicebox(satcrfrequ1, "2148", "2148");
	

	
	

	changelnb(voltagemode, tonemode, loftype, lofl, lofh, threshold, satcr, satcrfrequ1, satcrfrequ2, feshortname, lnbnr);

	tmpstr = ostrcat(feshortname, "_lnb_loftype", 0, 0);
	changeloftype(lofl, lofh, threshold, satcr, satcrfrequ1, satcrfrequ2, getconfig(tmpstr, lnbnr));
        free(tmpstr); tmpstr = NULL;
	free(lnbnr); lnbnr = NULL;

	drawscreen(lnbscreen, 0, 0);
	addscreenrc(lnbscreen, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(lnbscreen, tmp);
		rcret = waitrc(lnbscreen, 0, 2);
		tmp = listbox->select;

		if(listbox->select != NULL && listbox->select->name != NULL && ostrstr(listbox->select->name, "lnb_loftype") != NULL && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)))
		{
			changeloftype(lofl, lofh, threshold, satcr, satcrfrequ1, satcrfrequ2, loftype->ret);
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			screenunicable(loftype, lofl, lofh, satcr, satcrfrequ2);
			changeloftype(lofl, lofh, threshold, satcr, satcrfrequ1, satcrfrequ2, loftype->ret);
		}

		addconfigscreentmpcheck(voltagemode->name, voltagemode, "0");
		addconfigscreentmpcheck(tonemode->name, tonemode, "0");
		addconfigscreentmpcheck(loftype->name, loftype, "0");
		addconfigscreentmpcheck(lofl->name, lofl, "000000");
		addconfigscreentmpcheck(lofh->name, lofh, "000000");
		addconfigscreentmpcheck(threshold->name, threshold, "000000");
		addconfigscreentmpcheck(satcr->name, satcr, "0");
		if(loftype != NULL)
		{
			if(ostrcmp(loftype->ret, "3") == 0)
				addconfigscreentmpcheck(satcrfrequ1->name, satcrfrequ1, "0000");
			if(ostrcmp(loftype->ret, "4") == 0)
				addconfigscreentmpcheck(satcrfrequ2->name, satcrfrequ2, "0000");
		}

		drawscreen(lnbscreen, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
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
	changename(satcr, "satcr");
	changename(satcrfrequ1, "satcrfrequ1");
	changename(satcrfrequ2, "satcrfrequ2");

	delconfigtmpall();
	delownerrc(lnbscreen);
	clearscreen(lnbscreen);
}

#endif
