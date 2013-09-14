#ifndef PANEL_SETTINGS_OVERCLOCKING_H
#define PANEL_SETTINGS_OVERCLOCKING_H

void read_cpufreq(struct skin* label)
{
	if(isfile("/proc/cpu_frequ/pll0_ndiv_mdiv") == 0)
	{
		changetext(label, NULL);
		return;
	}

	char* tmpstr = NULL, *tmpstr1 = NULL, **tmpstr2 = NULL;
	int i = 0;

	tmpstr1 = ostrcat(tmpstr1, command("cat /proc/cpu_frequ/pll0_ndiv_mdiv"), 1, 0);
	if(tmpstr1 == NULL)
	{
		changetext(label, NULL);
		return;
	}
	tmpstr2 = str_split(tmpstr1, "\n");

	if(tmpstr2 != NULL)
	{
		for(i = 0; tmpstr2[i] != NULL; i++)
		{
			if(ostrstr(tmpstr2[i], "CKGA_") != 0)
			{
				free(tmpstr2[i]); tmpstr2[i] = NULL;
			}
			else if(ostrstr(tmpstr2[i], "SYSACLKOUT") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "BOGOMIPS") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "PLL0") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "SH4") != 0){
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "MODULE") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "SLIM") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "PLL1") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "COMMS") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "TMU0") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else if(ostrstr(tmpstr2[i], "TMU0") != 0)
			{
				tmpstr = ostrcat(tmpstr, tmpstr2[i], 1, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			}
			else
			{
				free(tmpstr2[i]); tmpstr2[i] = NULL;
			}
		}
	}
	changetext(label, _(tmpstr));
}

void set_freq(char* inifreq, char* oldfreq, char* newfreq)
{
	if(isfile("/proc/cpu_frequ/pll0_ndiv_mdiv") == 0)
	{
		debug(10, "file not found %s", "/proc/cpu_frequ/pll0_ndiv_mdiv");
		return;
	}

	debug(10, "inifreq=%s oldfreq=%s => newfreq=%s", inifreq, oldfreq, newfreq);

	if(ostrcmp(newfreq, inifreq) == 0 && ostrcmp(newfreq, oldfreq) == 0)
	{
		debug(10, "Nothing to change in FREQ");
		return;
	}

	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, "echo \"", 1, 0);
	tmpstr = ostrcat(tmpstr, newfreq, 1, 0);
	tmpstr = ostrcat(tmpstr, "\" > /proc/cpu_frequ/pll0_ndiv_mdiv", 1, 0);

	debug(10, "Set FREQ to %s", newfreq);
	system(tmpstr);

	if(oldfreq != NULL && newfreq != NULL){
		strcpy(oldfreq, newfreq);
	}
	return;
}

void set_fdma(char* inifdma, char* oldfdma, char* newfdma)
{
	if(checkbox("UFS912") || checkbox("ATEMIO520") || checkbox("ATEMIO530") || checkbox("SPARK") || checkbox("ATEMIO510") || checkbox("ATEMIO7600") || checkbox("WHITEBOX"))
		return;

	if(isfile("/proc/cpu_frequ/pll1_fdma_bypass") == 0)
	{
		debug(10, "file not found %s", "/proc/cpu_frequ/pll1_fdma_bypass");
		return;
	}

	if(ostrcmp(newfdma, inifdma) == 0 && ostrcmp(newfdma, oldfdma) == 0)
	{
		debug(10, "Nothing to change in FDMA");
		return;
	}

	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, "echo \"", 1, 0);
	tmpstr = ostrcat(tmpstr, newfdma, 1, 0);
	tmpstr = ostrcat(tmpstr, "\" > /proc/cpu_frequ/pll1_fdma_bypass", 1, 0);

	debug(10, "Set FDMA to %s", newfdma);
	system(tmpstr);

	if(oldfdma != NULL && newfdma != NULL)
		strcpy(oldfdma, newfdma);

	return;
}

char* getlistvalue(struct skin* mylist, char* name)
{
	struct skin* node = NULL;
	char* tmpstr = NULL;
	int tmpaktpage = mylist->aktpage;
	int tmpaktline = mylist->aktline;

	mylist->aktpage = -1;
	mylist->aktline = 1;
	node = mylist->next;

	while(node != NULL)
	{
		if(ostrcmp(name, node->name) == 0)
		{
			tmpstr = ostrcat(tmpstr, node->ret, 1, 0);
			break;
		}
		node = node->next;
	}

	mylist->aktpage = tmpaktpage;
	mylist->aktline = tmpaktline;
	return tmpstr;
}

void screenpanel_settings_overclocking()
{
	int rcret = 0;
	struct skin* panel_overclock = getscreen("panel_settings_overclocking");
	struct skin* listbox = getscreennode(panel_overclock, "listbox");
	struct skin* label1 = getscreennode(panel_overclock, "label1");
	struct skin* node = NULL;
	struct skin* tmp = NULL;

	char* FreqSelvalue = NULL, *FreqSeltext = NULL;
	char* standbyFreqSelvalue = NULL, *standbyFreqSeltext = NULL;
	char* inifreq = NULL, *oldfreq = NULL;
	char* inifdma = NULL, *oldfdma = NULL;

	addscreenrc(panel_overclock, listbox);

	if(checkbox("UFS912") || checkbox("ATEMIO520") || checkbox("ATEMIO530") || checkbox("SPARK") || checkbox("ATEMIO510") || checkbox("ATEMIO7600") || checkbox("WHITEBOX"))
	{
		FreqSelvalue = "3841\n12803\n5121\n16643\n17923";
		FreqSeltext = "450 (default)\n500\n600\n650\n700";
		standbyFreqSelvalue = "3841\n2561\n5123\n2563";
		standbyFreqSeltext = "450 (default)\n300\n200\n100";
	}
	else
	{
		FreqSelvalue = "15110\n25609\n9475\n31241\n22790";
		FreqSeltext = "266 (default)\n300\n333\n366\n400";
		standbyFreqSelvalue = "15110\n51227\n42523\n34075";
		standbyFreqSeltext = "266 (default)\n200\n166\n133";
	}

	node = addlistbox(panel_overclock, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Only Boot Overclock")); changename(node, "only_boot_overclock");
		addchoicebox(node, "0", _("no")); addchoicebox(node, "1", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	node = addlistbox(panel_overclock, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("CPU frequency")); changename(node, "pll0_ndiv_mdiv");
		addchoicebox(node, FreqSelvalue, _(FreqSeltext));
		setchoiceboxselection(node, getownconfig(node->name));
	}

	if(checkbox("UFS912") == 0 && checkbox("ATEMIO520") == 0 && checkbox("ATEMIO530") == 0 && checkbox("SPARK") == 0 && checkbox("ATEMIO510") == 0 && checkbox("ATEMIO7600") == 0 && checkbox("WHITEBOX") == 0)
	{
		node = addlistbox(panel_overclock, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("FDMA bypass")); changename(node, "pll1_fdma_bypass");
			addchoicebox(node, "0", _("no")); addchoicebox(node, "1", _("yes"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

	node = addlistbox(panel_overclock, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Reduce Standby frequency")); changename(node, "standby_overclock");
		addchoicebox(node, "0", _("no")); addchoicebox(node, "1", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));

		if(ostrcmp(getownconfig(node->name), "1") == 0)
		{
			node = addlistbox(panel_overclock, listbox, node, 1);
			if(node != NULL)
			{
				node->type = CHOICEBOX;
				changetext(node, _("Standby frequency")); changename(node, "standby_freq");
				addchoicebox(node, standbyFreqSelvalue, _(standbyFreqSeltext));
				setchoiceboxselection(node, getownconfig(node->name));
			}
		}
		else
		{
			node = addlistbox(panel_overclock, listbox, node, 1);
			if(node != NULL)
			{
				node->type = CHOICEBOX;
				changetext(node, NULL); changename(node, "standby_freq");
			}
		}
	}

	if(checkbox("UFS912") == 0 && checkbox("ATEMIO520") == 0 && checkbox("ATEMIO530") == 0 && checkbox("SPARK") == 0 && checkbox("ATEMIO510") == 0 && checkbox("ATEMIO7600") == 0 && checkbox("WHITEBOX") == 0)
	{
		node = addlistbox(panel_overclock, listbox, node, 1);
		if(node != NULL)
		{
			node->type = CHOICEBOX;
			changetext(node, _("Use Teletxt Workaround")); changename(node, "tuxtxt_cpufrequ_workaround");
			addchoicebox(node, "0", _("off")); addchoicebox(node, "1", _("frequency")); addchoicebox(node, "1", _("videmode"));
			setchoiceboxselection(node, getownconfig(node->name));
		}
	}

	read_cpufreq(label1);
	drawscreen(panel_overclock, 0, 0);

	inifreq = getlistvalue(listbox,"pll0_ndiv_mdiv");
	oldfreq = getlistvalue(listbox,"pll0_ndiv_mdiv");
	inifdma = getlistvalue(listbox,"pll1_fdma_bypass");
	oldfdma = getlistvalue(listbox,"pll1_fdma_bypass");

	tmp = listbox->select;

	while(1)
	{
		addscreenrc(panel_overclock, tmp);
		rcret = waitrc(panel_overclock, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcred", NULL)){
			set_freq(inifreq, oldfreq, inifreq); // set freq back to initial value if freq <> inifreq
			set_fdma(inifdma, oldfdma, inifdma);
			break;
		}

		if(rcret == getrcconfigint("rcyellow", NULL)){
			set_freq(oldfreq, oldfreq, getownconfig("pll0_ndiv_mdiv"));
			set_fdma(oldfdma, oldfdma, getownconfig("pll1_fdma_bypass"));
			read_cpufreq(label1);
			drawscreen(panel_overclock, 0, 0);
		}

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
			if(ostrcmp(listbox->select->name, "standby_overclock") == 0)
			{
				listbox->select = listbox->select->next;

				if(ostrcmp(getownconfig("standby_overclock"), "1") == 0)
				{
					changetext(listbox->select, _("Standby frequency"));
					addchoicebox(listbox->select, standbyFreqSelvalue, _(standbyFreqSeltext));
					setchoiceboxselection(listbox->select, getownconfig("standby_freq"));
				}
				else
				{
					changetext(listbox->select, NULL);
					changeinput(listbox->select, NULL);
				}
				drawscreen(panel_overclock, 0, 0);
			}
		}

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			set_freq(oldfreq, oldfreq, getownconfig("pll0_ndiv_mdiv"));
			set_fdma(oldfdma, oldfdma, getownconfig("pll1_fdma_bypass"));
			writeownconfigtmp();
			break;
		}
	}

	FreqSelvalue = NULL; FreqSeltext = NULL; standbyFreqSelvalue = NULL; standbyFreqSeltext = NULL; tmp = NULL;
	inifreq = NULL; oldfreq = NULL; inifdma = NULL; oldfdma = NULL;
	delownconfigtmpall();
	delmarkedscreennodes(panel_overclock, 1);
	delownerrc(panel_overclock);
	clearscreen(panel_overclock);
}

#endif
