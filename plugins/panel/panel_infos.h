#ifndef PANEL_INFOS_H
#define PANEL_INFOS_H

void screenpanel_infos(int mode)
{

	char* tmpstr = NULL;
	int rcret = 0;
	struct skin* panel_info = getscreen("panel_infos");
	struct skin* info = getscreennode(panel_info, "info");

	addscreenrc(panel_info, info);

	if(mode == 0)
  {
		char* tmpstr1 = NULL, *tmpstr2 = NULL;

		changetitle(panel_info, _("Default"));

		tmpstr1 = ostrcat("Date = ", "", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, gettime(NULL, "%d %B %Y"), 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nTime = ", "", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, gettime(NULL, "%H:%M:%S"), 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);
		
		tmpstr1 = ostrcat("\nUptime = ", "", 0, 0);
		tmpstr2 = command("uptime");
		if(tmpstr2 != NULL) tmpstr2 = strtok(tmpstr2, ",");
		tmpstr1 = ostrcat(tmpstr1, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nBoxtype = ", "", 0, 0);
		tmpstr2 = string_toupper(command("cat /etc/model"));
		tmpstr1 = ostrcat(tmpstr1, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nLoad = ", "", 0, 0);
		tmpstr2 = command("cat /proc/loadavg");
		tmpstr1 = ostrcat(tmpstr1, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		changetext(info, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 1)
  {
		changetitle(panel_info, _("Free Space"));

		tmpstr = command("df -h");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 2)
  {
		changetitle(panel_info, _("Kernel"));

		tmpstr = command("cat /proc/version");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 3)
  {
		changetitle(panel_info, _("Mounts"));

		tmpstr = command("mount");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 4)
  {
		char* tmpstr1 = NULL, *tmpstr2 = NULL;

		changetitle(panel_info, _("Network"));

		tmpstr = command("ifconfig");
		tmpstr1 = command("route -n");
		tmpstr2 = ostrcat(tmpstr, tmpstr1, 1, 1);
		changetext(info, tmpstr2);

		free(tmpstr2); tmpstr2 = NULL;
	}

	if(mode == 5)
  {
		changetitle(panel_info, _("Ram"));

		tmpstr = command("free");
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
