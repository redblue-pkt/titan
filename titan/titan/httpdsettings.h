#ifndef HTTDPSETTINGS_H
#define HTTPDSETTINGS_H

void screenhttpdsettings()
{
	int rcret = 0, i = 0;
	struct skin* httpdsettings = getscreen("httpdsettings");
	struct skin* listbox = getscreennode(httpdsettings, "listbox");
	struct skin* httpdstart = getscreennode(httpdsettings, "httpdstart");
	struct skin* user = getscreennode(httpdsettings, "user");
	struct skin* password = getscreennode(httpdsettings, "password");
	struct skin* httpdport = getscreennode(httpdsettings, "httpdport");
	struct skin* streamport = getscreennode(httpdsettings, "streamport");
	struct skin* rguidstart = getscreennode(httpdsettings, "rguidstart");
	struct skin* rguidport = getscreennode(httpdsettings, "rguidport");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *pos = NULL;

	addchoicebox(httpdstart, "0", _("no"));
	addchoicebox(httpdstart, "1", _("yes"));
	setchoiceboxselection(httpdstart, getconfig("httpdstart", NULL));

	tmpstr = ostrcat(getconfig("httpauth", NULL), NULL, 0, 0);
	if(tmpstr != NULL)
	{
		pos = ostrstr(tmpstr, ":");
		if(pos != NULL)
		{
			pos[0] = '\0';
			changeinput(user, tmpstr);
			pos++; 
			changeinput(password, pos);
		}
	}
	free(tmpstr); tmpstr = NULL;

	changemask(httpdport, "00000");
	changeinput(httpdport, getconfig("httpdport", NULL));
	if(httpdport->input != NULL)
	{
		for(i = 0; i < 6 - strlen(httpdport->input); i++)
			httpdport->input = ostrcat("0", httpdport->input, 0, 1);
	}

	changemask(streamport, "00000");
	changeinput(streamport, getconfig("streamport", NULL));
	if(streamport->input != NULL)
	{
		for(i = 0; i < 6 - strlen(streamport->input); i++)
			streamport->input = ostrcat("0", streamport->input, 0, 1);
	}
	
	addchoicebox(rguidstart, "0", _("no"));
	addchoicebox(rguidstart, "1", _("yes"));
	setchoiceboxselection(rguidstart, getconfig("rguidstart", NULL));

	changemask(rguidport, "00000");
	changeinput(rguidport, getconfig("rguidport", NULL));
	if(rguidport->input != NULL)
	{
		for(i = 0; i < 6 - strlen(rguidport->input); i++)
			rguidport->input = ostrcat("0", rguidport->input, 0, 1);
	}

	drawscreen(httpdsettings, 0, 0);
	addscreenrc(httpdsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(httpdsettings, tmp);
		rcret = waitrc(httpdsettings, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(httpdstart->ret != NULL)
			{
				if(ostrcmp(getconfig("httpdstart", NULL), httpdstart->ret) != 0)
				{
					addconfigscreen("httpdstart", httpdstart);
					if(ostrcmp(httpdstart->ret, "0") == 0)
						starthttpd(0);
					else
						starthttpd(1);
				}
			}
			if(user->ret != NULL && password->ret != NULL && strlen(user->ret) > 0 && strlen(password->ret) > 0)
			{
				tmpstr = ostrcat(tmpstr, user->ret, 1, 0);
				tmpstr = ostrcat(tmpstr, ":", 1, 0);
				tmpstr = ostrcat(tmpstr, password->ret, 1, 0);
				addconfig("httpauth", tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			else
				delconfig("httpauth");
			free(status.httpauth);
			status.httpauth = ostrcat(getconfig("httpauth", NULL), NULL, 0, 0);
			if(httpdport->ret != NULL)
			{
				int hport = atoi(httpdport->ret);
				if(hport != 80)
					addconfigint("httpdport", hport);
			}
			if(streamport->ret != NULL)
			{
				int sport = atoi(streamport->ret);
				if(sport != 22222)
					addconfigint("streamport", sport);
			}
			if(rguidstart->ret != NULL)
			{
				if(ostrcmp(getconfig("rguidstart", NULL), rguidstart->ret) != 0)
				{
					addconfigscreen("rguidstart", rguidstart);
					if(ostrcmp(rguidstart->ret, "0") == 0)
						startrguid(0);
					else
						startrguid(1);
				}
			}
			if(rguidport->ret != NULL)
			{
				int rport = atoi(rguidport->ret);
				if(rport != 22233)
					addconfigint("rguidport", rport);
			}
			break;
		}
	}

	delownerrc(httpdsettings);
	clearscreen(httpdsettings);
}

#endif
