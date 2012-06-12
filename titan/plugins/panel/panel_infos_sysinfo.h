#ifndef PANEL_INFOS_SYSINFO_H
#define PANEL_INFOS_SYSINFO_H

void screenpanel_infos_sysinfo(int mode)
{

	char* tmpstr = NULL;
	int rcret = 0;
	struct skin* panel_info = getscreen("panel_infos");
	struct skin* info = getscreennode(panel_info, "info");

	addscreenrc(panel_info, info);

	if (mode == 0){
		changetitle(panel_info, _("CPU info"));

		tmpstr = command("cat /proc/cpuinfo | sed 's/\t\t/\t/'");
		changetext(info, _(tmpstr));

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 1){
		changetitle(panel_info, _("Memory info"));

		tmpstr = command("cat /proc/meminfo");
		changetext(info, _(tmpstr));

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 2){
		changetitle(panel_info, _("MTD info"));

		tmpstr = command("cat /proc/mtd");
		changetext(info, _(tmpstr));

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 3){
		changetitle(panel_info, _("Module info"));

		tmpstr = command("cat /proc/modules");
		changetext(info, _(tmpstr));

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 4){
		changetitle(panel_info, _("Devices"));

		tmpstr = command("cat /proc/devices");
		changetext(info, _(tmpstr));

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 5){
		char* tmpstr1 = NULL, **tmpstr2 = NULL, **tmpstr3 = NULL;
		int i = 0, ii = 0;
		char* swap[] = {"Name: ", "Type: ", "Size: ", "Used: ", "Prio: "};

		changetitle(panel_info, _("Swap info"));

		tmpstr1 = command("cat /proc/swaps | sed 's/\t/ /g; s/[ ]* / /g'");
		tmpstr2 = str_split(tmpstr1, "\n");
		if(tmpstr2 != NULL)
		{
			free(tmpstr2[0]); tmpstr2[0] = NULL;

			for(i = 1; tmpstr2[i] != NULL; i++)
			{
				tmpstr3 = str_split(tmpstr2[i], " ");
				if(tmpstr3 != NULL)
				{

					for(ii = 0; tmpstr3[ii] != NULL; ii++)
					{
						tmpstr = ostrcat(tmpstr, swap[ii], 1, 0);
						tmpstr = ostrcat(tmpstr, tmpstr3[ii], 1, 1);
						tmpstr = ostrcat(tmpstr, "\n", 1, 0);
					}
				}

				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				free(tmpstr2[i]); tmpstr2[i] = NULL;
				free(tmpstr3); tmpstr3 = NULL;
			}
		}

		changetext(info, _(tmpstr));

		free(tmpstr3); tmpstr3 = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr1); tmpstr1 = NULL;
		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 6){
		changetitle(panel_info, _("Top Info"));

		tmpstr = command("top -b -n1");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	drawscreen(panel_info, 0, 0);

	while(1)
	{
		rcret = waitrc(panel_info, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(panel_info);
	clearscreen(panel_info);
	//delscreen(panel_info);
}

#endif
