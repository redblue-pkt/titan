#ifndef AAFPANEL_INFOS_H
#define AAFPANEL_INFOS_H

void screenaafpanel_infos(int mode)
{

	char* tmpstr = NULL;
	int rcret = 0;
	struct skin* aaf_info = getscreen("aafpanel_infos");
	struct skin* info = getscreennode(aaf_info, "info");

	addscreenrc(aaf_info, info);

	if (mode == 0){
		char* tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;

		changetitle(aaf_info, _("AAF Infos"));

		tmpstr1 = command("cat /etc/motd");
		if(tmpstr1 != NULL) tmpstr2 = strstr(tmpstr1, "wElc0me");
		tmpstr3 = command("cat /etc/imageinfo");
		if(tmpstr2 == NULL)
			tmpstr = ostrcat(tmpstr3, "", 1, 0);
		else
			tmpstr = ostrcat(tmpstr2, tmpstr3, 0, 1);
		changetext(info, tmpstr);

		free(tmpstr1); tmpstr1 = NULL;
		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 1){
		char* tmpstr1 = NULL, *tmpstr2 = NULL;

		changetitle(aaf_info, _("AAF Default"));

		tmpstr1 = ostrcat("Date = ", "", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, gettime("%d %B %Y"), 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nTime = ", "", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, gettime("%H:%M:%S"), 1, 1);
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

	if (mode == 2){
		changetitle(aaf_info, _("AAF Free Space"));

		tmpstr = command("df -h");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 3){
		changetitle(aaf_info, _("AAF Kernel"));

		tmpstr = command("cat /proc/version");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 4){
		changetitle(aaf_info, _("AAF Mounts"));

		tmpstr = command("mount");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	if (mode == 5){
		char* tmpstr1 = NULL, *tmpstr2 = NULL;

		changetitle(aaf_info, _("AAF Network"));

		tmpstr = command("ifconfig");
		tmpstr1 = command("route -n");
		tmpstr2 = ostrcat(tmpstr, tmpstr1, 1, 1);
		changetext(info, tmpstr2);

		free(tmpstr2); tmpstr2 = NULL;
	}

	if (mode == 6){
		changetitle(aaf_info, _("AAF Ram"));

		tmpstr = command("free");
		changetext(info, tmpstr);

		free(tmpstr); tmpstr = NULL;
	}

	drawscreen(aaf_info, 0);

	while(1)
	{
		rcret = waitrc(aaf_info, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(aaf_info);
	clearscreen(aaf_info);
	//delscreen(aaf_info);
}

#endif
