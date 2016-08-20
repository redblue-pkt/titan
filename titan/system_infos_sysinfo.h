#ifndef SYSTEM_INFOS_SYSINFO_H
#define SYSTEM_INFOS_SYSINFO_H

void screensystem_infos_sysinfo(int mode)
{

	char* tmpstr = NULL;
	int rcret = 0;
	struct skin* system_info = getscreen("system_infos");
	struct skin* info = getscreennode(system_info, "info");

	addscreenrc(system_info, info);

	if(mode == 0)
		changetitle(system_info, _("CPU"));
	else if(mode == 1)
		changetitle(system_info, _("Memory"));
	else if(mode == 2)
		changetitle(system_info, _("MTD"));
	else if(mode == 3)
		changetitle(system_info, _("Kernel Modules"));
	else if(mode == 4)
		changetitle(system_info, _("Devices"));
	else if(mode == 5)
		changetitle(system_info, _("Swap"));
	else if(mode == 6)
		changetitle(system_info, _("Top"));
	else if(mode == 7)
		changetitle(system_info, _("Processlist"));
	else if(mode == 8)
		changetitle(system_info, _("USB"));

	drawscreen(system_info, 0, 0);

	tmpstr = system_infos_sysinfo(mode);

	changetext(info, tmpstr);
	free(tmpstr); tmpstr = NULL;
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
