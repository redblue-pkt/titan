#ifndef DISEQCCONFIG_H
#define DISEQCCONFIG_H

int changeuncommittedcmd(struct skin* cmdorder, char* type)
{
	if(type == NULL || ostrcmp(type, "0") == 0)
	{
		changeinput(cmdorder, NULL);
		addchoicebox(cmdorder, "0", _("committed, toneburst"));
		addchoicebox(cmdorder, "1", _("toneburst, committed"));
		return 0;
	}
	else
	{
		changeinput(cmdorder, NULL);
		addchoicebox(cmdorder, "0", _("committed, toneburst"));
		addchoicebox(cmdorder, "1", _("toneburst, committed"));
		addchoicebox(cmdorder, "2", _("committed, uncommitted, toneburst"));
		addchoicebox(cmdorder, "3", _("toneburst, committed, uncommitted"));
		addchoicebox(cmdorder, "4", _("uncommitted, committed, toneburst"));
		addchoicebox(cmdorder, "5", _("toneburst, uncommitted, committed"));
		return 1;
	}

}

void changediseqcmode(struct skin* committedcmd, struct skin* cmdorder, struct skin* uncommittedcmd, struct skin* rotorpos, struct skin* diseqcrepeat, struct skin* latitude, struct skin* longitude, struct skin* eastwest, char *type)
{
	committedcmd->hidden = YES;
	cmdorder->hidden = YES;
	uncommittedcmd->hidden = YES;
	rotorpos->hidden = YES;
	diseqcrepeat->hidden = YES;
	latitude->hidden = YES;
	longitude->hidden = YES;
	eastwest->hidden = YES;

	if(type == NULL || ostrcmp(type, "0") == 0 || ostrcmp(type, "1") == 0 || ostrcmp(type, "2") == 0 || ostrcmp(type, "3") == 0)
	{
		committedcmd->hidden = NO;
		cmdorder->hidden = NO;
		diseqcrepeat->hidden = NO;

		if(ostrcmp(type, "1") == 0 || ostrcmp(type, "2") == 0)
			uncommittedcmd->hidden = NO;
		if(ostrcmp(type, "2") == 0)
			rotorpos->hidden = NO;
		if(ostrcmp(type, "3") == 0)
		{
			latitude->hidden = NO;
			longitude->hidden = NO;
			eastwest->hidden = NO;
		}
	}
}

void changediseqc(struct skin* diseqcmode, struct skin* toneburst, struct skin* committedcmd, struct skin* cmdorder, struct skin* uncommittedcmd, struct skin* diseqcrepeat, struct skin* rotorpos, struct skin* latitude, struct skin* longitude, struct skin* eastwest, char* feshortname, char *diseqcnr)
{
	char* tmpstr = NULL, *tmpnr = NULL;
	int i = 0;

	tmpstr = ostrcat(feshortname, "_diseqc_mode", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(diseqcmode, tmpstr);
	setchoiceboxselection(diseqcmode, getconfig(tmpstr, NULL));
	changeuncommittedcmd(cmdorder, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_toneburst", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(toneburst, tmpstr);
	setchoiceboxselection(toneburst, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_committedcmd", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(committedcmd, tmpstr);
	setchoiceboxselection(committedcmd, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_cmdorder", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(cmdorder, tmpstr);
	setchoiceboxselection(cmdorder, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_uncommittedcmd", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(uncommittedcmd, tmpstr);
	setchoiceboxselection(uncommittedcmd, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_repeate", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(diseqcrepeat, tmpstr);
	setchoiceboxselection(diseqcrepeat, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_rotorpos", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(rotorpos, tmpstr);
	for(i = 1; i < 255; i++)
	{
		tmpnr = oitoa(i);
		addchoicebox(rotorpos, tmpnr, tmpnr);
		free(tmpnr); tmpnr = NULL;
	}
	setchoiceboxselection(rotorpos, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_latitude", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(latitude, tmpstr);
	changeinput(latitude, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_longitude", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(longitude, tmpstr);
	changeinput(longitude, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat(feshortname, "_diseqc_eastwest", 0, 0);
	tmpstr = ostrcat(tmpstr, diseqcnr, 1, 0);
	changename(eastwest, tmpstr);
	setchoiceboxselection(eastwest, getconfig(tmpstr, NULL));
	free(tmpstr);
}

void screendiseqc(char* feshortname, char* diseqcnr)
{
	int rcret = 0;
	struct skin* diseqcscreen = getscreen("diseqcscreen");
	struct skin* listbox = getscreennode(diseqcscreen, "listbox");
	struct skin* diseqcmode = getscreennode(diseqcscreen, "diseqcmode");
	struct skin* toneburst = getscreennode(diseqcscreen, "toneburst");
	struct skin* committedcmd = getscreennode(diseqcscreen, "committedcmd");
	struct skin* cmdorder = getscreennode(diseqcscreen, "cmdorder");

	struct skin* uncommittedcmd = getscreennode(diseqcscreen, "uncommittedcmd");
	struct skin* diseqcrepeat = getscreennode(diseqcscreen, "diseqcrepeat");
	struct skin* rotorpos = getscreennode(diseqcscreen, "rotorpos");
	struct skin* latitude = getscreennode(diseqcscreen, "latitude");
	struct skin* longitude = getscreennode(diseqcscreen, "longitude");
	struct skin* eastwest = getscreennode(diseqcscreen, "eastwest");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = 1;

	if(diseqcnr != NULL)
		diseqcnr = oitoa((int)diseqcnr);
	else
		return;

	changeinput(diseqcmode, NULL);
	addchoicebox(diseqcmode, "0", _("DiSEqC 1.0"));
	addchoicebox(diseqcmode, "1", _("DiSEqC 1.1"));
	addchoicebox(diseqcmode, "2", _("DiSEqC 1.2"));
	addchoicebox(diseqcmode, "3", _("DiSEqC 1.3 (USALS)"));
	addchoicebox(diseqcmode, "100", _("Tonburst A/B"));

	changeinput(toneburst, NULL);
	addchoicebox(toneburst, "0", _("none"));
	addchoicebox(toneburst, "1", _("A"));
	addchoicebox(toneburst, "2", _("B"));

	changeinput(committedcmd, NULL);
	addchoicebox(committedcmd, "0", _("none"));
	addchoicebox(committedcmd, "1", _("Input 1"));
	addchoicebox(committedcmd, "2", _("Input 2"));
	addchoicebox(committedcmd, "3", _("Input 3"));
	addchoicebox(committedcmd, "4", _("Input 4"));
	addchoicebox(committedcmd, "5", _("Input 5"));
	addchoicebox(committedcmd, "6", _("Input 6"));
	addchoicebox(committedcmd, "7", _("Input 7"));
	addchoicebox(committedcmd, "8", _("Input 8"));
	addchoicebox(committedcmd, "9", _("Input 9"));
	addchoicebox(committedcmd, "10", _("Input 10"));
	addchoicebox(committedcmd, "11", _("Input 11"));
	addchoicebox(committedcmd, "12", _("Input 12"));
	addchoicebox(committedcmd, "13", _("Input 13"));
	addchoicebox(committedcmd, "14", _("Input 14"));
	addchoicebox(committedcmd, "15", _("Input 15"));
	addchoicebox(committedcmd, "16", _("Input 16"));

	changeinput(uncommittedcmd, NULL);
	addchoicebox(uncommittedcmd, "0", _("none"));
	addchoicebox(uncommittedcmd, "1", _("Input 1"));
	addchoicebox(uncommittedcmd, "2", _("Input 2"));
	addchoicebox(uncommittedcmd, "3", _("Input 3"));
	addchoicebox(uncommittedcmd, "4", _("Input 4"));
	addchoicebox(uncommittedcmd, "5", _("Input 5"));
	addchoicebox(uncommittedcmd, "6", _("Input 6"));
	addchoicebox(uncommittedcmd, "7", _("Input 7"));
	addchoicebox(uncommittedcmd, "8", _("Input 8"));
	addchoicebox(uncommittedcmd, "9", _("Input 9"));
	addchoicebox(uncommittedcmd, "10", _("Input 10"));
	addchoicebox(uncommittedcmd, "11", _("Input 11"));
	addchoicebox(uncommittedcmd, "12", _("Input 12"));
	addchoicebox(uncommittedcmd, "13", _("Input 13"));
	addchoicebox(uncommittedcmd, "14", _("Input 14"));
	addchoicebox(uncommittedcmd, "15", _("Input 15"));
	addchoicebox(uncommittedcmd, "16", _("Input 16"));

	changeinput(diseqcrepeat, "1\n2\n3\n4\n5\n6");

	changemask(latitude, "000");
	changemask(longitude, "000");

	changeinput(eastwest, NULL);
	addchoicebox(eastwest, "0", _("east"));
	addchoicebox(eastwest, "1", _("west"));

	changediseqc(diseqcmode, toneburst, committedcmd, cmdorder, uncommittedcmd, diseqcrepeat, rotorpos, latitude, longitude, eastwest, feshortname, diseqcnr);

	tmpstr = ostrcat(feshortname, "_diseqc_mode", 0, 0);
	changediseqcmode(committedcmd, cmdorder, uncommittedcmd, rotorpos, diseqcrepeat, latitude, longitude, eastwest, getconfig(tmpstr, diseqcnr));
	free(tmpstr); tmpstr = NULL;
	free(diseqcnr); diseqcnr = NULL;

	drawscreen(diseqcscreen, 0);
	addscreenrc(diseqcscreen, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(diseqcscreen, tmp);
		status.screencalc = 2;
		rcret = waitrc(diseqcscreen, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL && listbox->select->name != NULL && strstr(listbox->select->name, "diseqc_mode") != NULL && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)))
		{
			changediseqcmode(committedcmd, cmdorder, uncommittedcmd, rotorpos, diseqcrepeat, latitude, longitude, eastwest, diseqcmode->ret);
			changeuncommittedcmd(cmdorder, diseqcmode->ret);
		}

		addconfigscreentmpcheck(diseqcmode->name, diseqcmode, "0");
		addconfigscreentmpcheck(toneburst->name, toneburst, "0");
		addconfigscreentmpcheck(committedcmd->name, committedcmd, "0");
		addconfigscreentmpcheck(cmdorder->name, cmdorder, "0");
		addconfigscreentmpcheck(diseqcrepeat->name, diseqcrepeat, "1");
		addconfigscreentmpcheck(uncommittedcmd->name, uncommittedcmd, "0");
		addconfigscreentmpcheck(rotorpos->name, rotorpos, "000");
		addconfigscreentmpcheck(latitude->name, latitude, "000");
		addconfigscreentmpcheck(longitude->name, longitude, "000");
		addconfigscreentmpcheck(eastwest->name, eastwest, "0");

		status.screencalc = 0;
		drawscreen(diseqcscreen, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			writeconfigtmp();
			break;
		}
	}

	changename(diseqcmode, "diseqcmode");
	changename(toneburst, "toneburst");
	changename(committedcmd, "committedcmd");
	changename(cmdorder, "cmdorder");
	changename(uncommittedcmd, "uncommittedcmd");
	changename(diseqcrepeat, "diseqcrepeat");
	changename(rotorpos, "rotorpos");
	changename(latitude, "latitude");
	changename(longitude, "longitude");
	changename(eastwest, "eastwest");

	status.screencalc = 0;
	delconfigtmpall();
	delownerrc(diseqcscreen);
	clearscreen(diseqcscreen);
}

#endif
