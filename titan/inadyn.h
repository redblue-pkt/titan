#ifndef INADYN_H
#define INADYN_H

int readinadyn(const char* filename, char** user, char** pw, char** host)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *tmpstr = NULL;
	int len = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		tmpstr = ostrstrcase(fileline, "--username ");
		if(tmpstr != NULL)
		{
			tmpstr += 11;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(*user); *user = NULL;
				*user = ostrcat(tmpstr, NULL, 0, 0);
			}				
		}
		tmpstr = ostrstrcase(fileline, "--password ");
		if(tmpstr != NULL)
		{
			tmpstr += 11;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(*pw); *pw = NULL;
				*pw = ostrcat(tmpstr, NULL, 0, 0);
			}				
		}
		tmpstr = ostrstrcase(fileline, "--alias ");
		if(tmpstr != NULL)
		{
			tmpstr += 8;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(*host); *host = NULL;
				*host = ostrcat(tmpstr, NULL, 0, 0);
			}				
		}
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int writeinadyn(const char* filename, char* user, char* pw, char* host)
{
	char* savesettings = NULL;
	
	savesettings = ostrcat(savesettings, "--username ", 1, 0);
	savesettings = ostrcat(savesettings, user, 1, 0);
	savesettings = ostrcat(savesettings, "\n", 1, 0);

	savesettings = ostrcat(savesettings, "--password ", 1, 0);
	savesettings = ostrcat(savesettings, pw, 1, 0);
	savesettings = ostrcat(savesettings, "\n", 1, 0);

	savesettings = ostrcat(savesettings, "--alias ", 1, 0);
	savesettings = ostrcat(savesettings, host, 1, 0);
	savesettings = ostrcat(savesettings, "\n", 1, 0);
	
	savesettings = ostrcat(savesettings, "update_period 60000\n", 1, 0);

	FILE* fd = fopen(filename, "w");
	if(fd)
	{
		fprintf(fd, "%s\n", savesettings);
		fclose(fd);
	}
	free(savesettings); savesettings = NULL;

	return 0;
}

void screennetwork_inadyn()
{
	int rcret = -1, ret = 0;
	struct skin* inadyn = getscreen("inadynsettings");
	struct skin* listbox = getscreennode(inadyn, "listbox");
	struct skin* startmode = getscreennode(inadyn, "startmode");
	struct skin* user = getscreennode(inadyn, "user");
	struct skin* pw = getscreennode(inadyn, "pw");
	struct skin* host = getscreennode(inadyn, "host");
	struct skin* tmp = NULL;

	char* iuser = NULL, *ipw = NULL, *ihost = NULL;

	readinadyn("/var/etc/inadyn.conf", &iuser, &ipw, &ihost);
	
	addchoicebox(startmode, "n", _("no"));
	addchoicebox(startmode, "y", _("yes"));
	setchoiceboxselection(startmode, getownconfig("inadyn"));

	changeinput(user, iuser);
	free(iuser); iuser = NULL;

	changeinput(pw, ipw);
	free(ipw); ipw = NULL;

	changeinput(host, ihost);
	free(ihost); ihost = NULL;

	drawscreen(inadyn, 0);
	addscreenrc(inadyn, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(inadyn, tmp);
		rcret = waitrc(inadyn, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcgreen", NULL))
		{
			writeinadyn("/var/etc/inadyn.conf", user->ret, pw->ret, host->ret);
			if(startmode->ret != NULL) addownconfig("inadyn", startmode->ret);
			if(rcret == getrcconfigint("rcok", NULL)) break;
			if(rcret == getrcconfigint("rcgreen", NULL))
			{
				system("killall inadyn; sleep 2; killall -9 inadyn");
				ret = system("inadyn --input_file /var/etc/inadyn.conf &");
				if(ret == 0)
					textbox(_("Message"), _("DYNDNS started."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				else
					textbox(_("Message"), _("DYNDNS not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				drawscreen(inadyn, 0);
			}
		}

		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			system("killall inadyn; sleep 2; killall -9 inadyn");
			textbox(_("Message"), _("DYNDNS now stopped"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
			drawscreen(inadyn, 0);
		}
	}

	delownerrc(inadyn);
	clearscreen(inadyn);
}

#endif
