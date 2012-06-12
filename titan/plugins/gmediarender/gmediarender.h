#ifndef GMEDIARENDER_H
#define GMEDIARENDER_H

extern struct skin* skin;
struct fb *fb = NULL;

void screengmediarender()
{
	int rcret = -1, ret = 0;
	char* cmd = NULL;
	char* tmpstr = NULL;
	
	ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;

	drawscreen(skin, 0, 0);

	cmd = ostrcat(cmd, "/var/swap/titanplugins/gmediarender/gmediarender.sh gmediarender-", 1, 0);
	cmd = ostrcat(cmd, status.boxtype, 1, 0);
	cmd = ostrcat(cmd, " &", 1, 0);
	
	//start renderer
	system(cmd);
	free(cmd), cmd = NULL;

	tmpstr = ostrcat(tmpstr, _("Servername"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": gmediarender-", 1, 0);
	tmpstr = ostrcat(tmpstr, status.boxtype, 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Wait for connect or press EXIT"), 1, 0);

	textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
	free(tmpstr), tmpstr = NULL;

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

 		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	//stop renderer
	system("killall -9 gmediarender");
			
	if(status.lastservice != NULL)
		servicestart(status.lastservice->channel, NULL, NULL, 0);
}

#endif
