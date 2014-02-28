#ifndef SYSTEM_INFOS_H
#define SYSTEM_INFOS_H

void screensystem_infos(int mode)
{
	char* tmpstr = NULL;
	int rcret = 0;
	struct skin* system_info = getscreen("system_infos");
	struct skin* info = getscreennode(system_info, "info");

	addscreenrc(system_info, info);

	if(mode == 0)
	{
		char* tmpstr1 = NULL, *tmpstr2 = NULL;

		changetitle(system_info, _("System_Status"));

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
		changetitle(system_info, _("Free Space"));
		system("ls /media/usb/* >/dev/null");
		system("ls /media/net/* >/dev/null");
		system("ls /var/swap/* >/dev/null");
		system("ls /mnt/swapextension/* >/dev/null");
		system("ls /var/backup/* >/dev/null");
		system("ls /media/hdd/* >/dev/null");
		tmpstr = command("df -h");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 2)
	{
		changetitle(system_info, _("Kernel"));

		tmpstr = command("cat /proc/version");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 3)
	{
		changetitle(system_info, _("Mounts"));
		system("ls /media/usb/* >/dev/null");
		system("ls /media/net/* >/dev/null");
		system("ls /var/swap/* >/dev/null");
		system("ls /mnt/swapextension/* >/dev/null");
		system("ls /var/backup/* >/dev/null");
		system("ls /media/hdd/* >/dev/null");
		tmpstr = command("mount");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if(mode == 4)
	{
		char* tmpstr1 = NULL, *tmpstr2 = NULL;

		changetitle(system_info, _("Network"));

		tmpstr = command("ifconfig");
		tmpstr1 = command("route -n");
		tmpstr2 = ostrcat(tmpstr, tmpstr1, 1, 1);
		changetext(info, tmpstr2);

		free(tmpstr2); tmpstr2 = NULL;
	}

	if(mode == 5)
	{
		changetitle(system_info, _("Ram"));

		tmpstr = command("free");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	drawscreen(system_info, 0, 0);

	while(1)
	{
		rcret = waitrc(system_info, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(system_info);
	clearscreen(system_info);
	//delscreen(system_info);
}

#endif
