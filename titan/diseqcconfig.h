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

void changediseqcmode(struct skin* committedcmd, struct skin* cmdorder, struct skin* uncommittedcmd, struct skin* rotorpos, struct skin* diseqcrepeat, char *type)
{
	committedcmd->hidden = YES;
	cmdorder->hidden = YES;
	uncommittedcmd->hidden = YES;
	rotorpos->hidden = YES;
	diseqcrepeat->hidden = YES;

	if(type == NULL || ostrcmp(type, "0") == 0 || ostrcmp(type, "1") == 0 || ostrcmp(type, "2") == 0)
	{
		committedcmd->hidden = NO;
		cmdorder->hidden = NO;
		diseqcrepeat->hidden = NO;

		if(ostrcmp(type, "1") == 0 || ostrcmp(type, "2") == 0)
			uncommittedcmd->hidden = NO;
		if(ostrcmp(type, "2") == 0)
			rotorpos->hidden = NO;
	}
}

void changediseqc(struct skin* diseqcmode, struct skin* toneburst, struct skin* committedcmd, struct skin* cmdorder, struct skin* uncommittedcmd, struct skin* diseqcrepeat, struct skin* rotorpos, char *diseqcnr)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat("diseqc_mode", diseqcnr, 0, 0);
	changename(diseqcmode, tmpstr);
	setchoiceboxselection(diseqcmode, getconfig(tmpstr, NULL));
	changeuncommittedcmd(cmdorder, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("diseqc_toneburst", diseqcnr, 0, 0);
	changename(toneburst, tmpstr);
	setchoiceboxselection(toneburst, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("diseqc_committedcmd", diseqcnr, 0, 0);
	changename(committedcmd, tmpstr);
	setchoiceboxselection(committedcmd, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("diseqc_cmdorder", diseqcnr, 0, 0);
	changename(cmdorder, tmpstr);
	setchoiceboxselection(cmdorder, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("diseqc_uncommittedcmd", diseqcnr, 0, 0);
	changename(uncommittedcmd, tmpstr);
	setchoiceboxselection(uncommittedcmd, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("diseqc_repeat", diseqcnr, 0, 0);
	changename(diseqcrepeat, tmpstr);
	setchoiceboxselection(diseqcrepeat, getconfig(tmpstr, NULL));
	free(tmpstr);

	tmpstr = ostrcat("diseqc_rotorpos", diseqcnr, 0, 0);
	changename(rotorpos, tmpstr);
	changemask(rotorpos, "000");
	changeinput(rotorpos, getconfig(tmpstr, NULL));
	free(tmpstr);
}

void screendiseqc(char* diseqcnr)
{
	int rcret = 0;
	struct skin* diseqcscreen = getscreen("diseqcscreen");
	struct skin* listbox = getscreennode(diseqcscreen, "listbox");
	struct skin* diseqc = getscreennode(diseqcscreen, "diseqc");
	struct skin* diseqcmode = getscreennode(diseqcscreen, "diseqcmode");
	struct skin* toneburst = getscreennode(diseqcscreen, "toneburst");
	struct skin* committedcmd = getscreennode(diseqcscreen, "committedcmd");
	struct skin* cmdorder = getscreennode(diseqcscreen, "cmdorder");

	struct skin* uncommittedcmd = getscreennode(diseqcscreen, "uncommittedcmd");
	struct skin* diseqcrepeat = getscreennode(diseqcscreen, "diseqcrepeat");
	struct skin* rotorpos = getscreennode(diseqcscreen, "rotorpos");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = 1;

	if(diseqcnr == NULL)
	{
		tmpstr = getmaxsatstring();
		changechoiceboxvalue(diseqc, tmpstr);
		tmpstr = stringreplacecharonce(tmpstr, '1', 'A');
		tmpstr = stringreplacecharonce(tmpstr, '2', 'B');
		tmpstr = stringreplacecharonce(tmpstr, '3', 'C');
		tmpstr = stringreplacecharonce(tmpstr, '4', 'D');
		tmpstr = stringreplacecharonce(tmpstr, '5', 'E');
		tmpstr = stringreplacecharonce(tmpstr, '6', 'F');
		tmpstr = stringreplacecharonce(tmpstr, '7', 'G');
		tmpstr = stringreplacecharonce(tmpstr, '8', 'H');
		changeinput(diseqc, tmpstr);
		diseqc->aktpage = 1;
		free(tmpstr); tmpstr = NULL;
		diseqcnr = "1";
	}
	else
	{
		changechoiceboxvalue(diseqc, diseqcnr);
		if(ostrcmp(diseqcnr, "1") == 0) changeinput(diseqc, "A");
		else if(ostrcmp(diseqcnr, "2") == 0) changeinput(diseqc, "B");
		else if(ostrcmp(diseqcnr, "3") == 0) changeinput(diseqc, "C");
		else if(ostrcmp(diseqcnr, "4") == 0) changeinput(diseqc, "D");
		else if(ostrcmp(diseqcnr, "5") == 0) changeinput(diseqc, "E");
		else if(ostrcmp(diseqcnr, "6") == 0) changeinput(diseqc, "F");
		else if(ostrcmp(diseqcnr, "7") == 0) changeinput(diseqc, "G");
		else if(ostrcmp(diseqcnr, "8") == 0) changeinput(diseqc, "H");
		else changeinput(diseqc, diseqcnr);
		diseqc->aktpage = atoi(diseqcnr);
	}


	changeinput(diseqcmode, NULL);
	addchoicebox(diseqcmode, "0", _("DiSEqC 1.0"));
	addchoicebox(diseqcmode, "1", _("DiSEqC 1.1"));
	//addchoicebox(diseqcmode, "2", _("DiSEqC 1.2"));
	addchoicebox(diseqcmode, "100", _("Tonburst A/B"));

	changeinput(toneburst, NULL);
	addchoicebox(toneburst, "0", _("None"));
	addchoicebox(toneburst, "1", _("A"));
	addchoicebox(toneburst, "2", _("B"));

	changeinput(committedcmd, NULL);
	addchoicebox(committedcmd, "0", _("None"));
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
	addchoicebox(uncommittedcmd, "0", _("None"));
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

	changediseqc(diseqcmode, toneburst, committedcmd, cmdorder, uncommittedcmd, diseqcrepeat, rotorpos, diseqcnr);
	changediseqcmode(committedcmd, cmdorder, uncommittedcmd, rotorpos, diseqcrepeat, getconfig("diseqc_mode", diseqcnr));

	drawscreen(diseqcscreen, 0);
	addscreenrc(diseqcscreen, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(diseqcscreen, tmp);
		status.screencalc = 2;
		rcret = waitrc(diseqcscreen, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL && ostrcmp(listbox->select->name, "diseqc") == 0 && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)))
		{
			changediseqc(diseqcmode, toneburst, committedcmd, cmdorder, uncommittedcmd, diseqcrepeat, rotorpos, listbox->select->ret);
			changediseqcmode(committedcmd, cmdorder, uncommittedcmd, rotorpos, diseqcrepeat, getconfig("diseqc_mode", listbox->select->ret));
		}

		drawscreen(diseqcscreen, 0);

		if(listbox->select != NULL && ostrncmp(listbox->select->name, "diseqc_mode", 11) == 0 && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)))
		{
			changediseqcmode(committedcmd, cmdorder, uncommittedcmd, rotorpos, diseqcrepeat, diseqcmode->ret);
			changeuncommittedcmd(cmdorder, diseqcmode->ret);
		}

		addconfigscreentmpcheck(diseqcmode->name, diseqcmode, "0");
		addconfigscreentmpcheck(toneburst->name, toneburst, "0");
		addconfigscreentmpcheck(committedcmd->name, committedcmd, "0");
		addconfigscreentmpcheck(cmdorder->name, cmdorder, "0");
		addconfigscreentmpcheck(diseqcrepeat->name, diseqcrepeat, "1");
		addconfigscreentmpcheck(uncommittedcmd->name, uncommittedcmd, "0");
		addconfigscreentmpcheck(rotorpos->name, rotorpos, "000");

		status.screencalc = 0;
		drawscreen(diseqcscreen, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			writeconfigtmp();
			break;
		}
	}

	changename(diseqc, "diseqc");
	changename(diseqcmode, "diseqcmode");
	changename(toneburst, "toneburst");
	changename(committedcmd, "committedcmd");
	changename(cmdorder, "cmdorder");
	changename(uncommittedcmd, "uncommittedcmd");
	changename(diseqcrepeat, "diseqcrepeat");
	changename(rotorpos, "rotorpos");

	status.screencalc = 0;
	delconfigtmpall();
	delownerrc(diseqcscreen);
	clearscreen(diseqcscreen);
}

#endif
