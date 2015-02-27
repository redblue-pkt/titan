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
		changetitle(system_info, _("System Status"));
	else if(mode == 1)
		changetitle(system_info, _("Free Space"));
	else if(mode == 2)
		changetitle(system_info, _("Kernel"));
	else if(mode == 3)
		changetitle(system_info, _("Mounts"));
	else if(mode == 4)
		changetitle(system_info, _("Network"));
	else if(mode == 5)
		changetitle(system_info, _("Ram"));

	drawscreen(system_info, 0, 0);

	tmpstr = system_infos(mode);
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
