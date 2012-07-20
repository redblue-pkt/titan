#ifndef INFO_H
#define INFO_H

void screengetserial()
{
	char* cpu = NULL;
	char* tmpstr = NULL;

	cpu = getcpuid();
	if(cpu == NULL) return;

	tmpstr = ostrcat("Board-ID SerialNr: ", cpu, 0, 0);
	writesys("/tmp/atemio.log", tmpstr, 1);

	char* cmd = NULL;
	cmd = ostrcat(cmd, "/var/backup/atemio.", 1, 0);
	cmd = ostrcat(cmd, cpu, 1, 0);
	cmd = ostrcat(cmd, ".log", 1, 0);
	writesys(cmd, tmpstr, 1);
	free(tmpstr); tmpstr = NULL;
	free(cmd); cmd = NULL;

	tmpstr = ostrcat(_("For next OnlineUpdate please contact Atemio and send this Serial Number\nand your Atemio Serial Number !!\n\nBoard-ID SerialNr:"), " ", 0, 0);
	tmpstr = ostrcat(tmpstr, cpu, 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Email  		info@atemio.de"), 1, 0);
	textbox(_("Info"), _(tmpstr), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 400, 0, 1);

	free(cpu); cpu = NULL;
	free(tmpstr); tmpstr = NULL;
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

		tmpstr1 = readfiletomem("/etc/motd", 0);
		if(tmpstr1 != NULL) tmpstr2 = strstr(tmpstr1, "wElc0me");
		tmpstr3 = readfiletomem("/etc/imageinfo", 0);
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
