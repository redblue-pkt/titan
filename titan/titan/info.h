#ifndef INFO_H
#define INFO_H

void screengetserial()
{
	getserial();
}

void screensystem_info(int mode)
{
	char* tmpstr = NULL;
	int rcret = 0;
	struct skin* systeminfo = getscreen("systeminfo");
	struct skin* info = getscreennode(systeminfo, "info");

	addscreenrc(systeminfo, info);

	if(mode == 0)
  {
		char* tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;

		changetitle(systeminfo, _("Infos"));

		tmpstr1 = command("cat /etc/motd");
		if(tmpstr1 != NULL) tmpstr2 = strstr(tmpstr1, "wElc0me");
		tmpstr3 = command("cat /etc/imageinfo");
		if(tmpstr2 == NULL)
			tmpstr = ostrcat(tmpstr3, NULL, 1, 0);
		else
			tmpstr = ostrcat(tmpstr2, tmpstr3, 0, 1);
		changetext(info, tmpstr);

		free(tmpstr1); tmpstr1 = NULL;
		free(tmpstr); tmpstr = NULL;
	}
	drawscreen(systeminfo, 0, 0);

	while(1)
	{
		rcret = waitrc(systeminfo, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(systeminfo);
	clearscreen(systeminfo);
}

#endif
