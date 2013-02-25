#ifndef HTTDPSETTINGS_H
#define HTTPDSETTINGS_H

void screenhttpdsettings()
{
	int rcret = 0;
	struct skin* httpdsettings = getscreen("httpdsettings");
	struct skin* listbox = getscreennode(httpdsettings, "listbox");
	struct skin* httpdstart = getscreennode(httpdsettings, "httpdstart");
	struct skin* user = getscreennode(httpdsettings, "user");
	struct skin* password = getscreennode(httpdsettings, "password");
	struct skin* httpdport = getscreennode(httpdsettings, "httpdport");
	struct skin* streamport = getscreennode(httpdsettings, "streamport");
	struct skin* rguidstart = getscreennode(httpdsettings, "rguidstart");
	struct skin* rguidport = getscreennode(httpdsettings, "rguidport");
	struct skin* webifip = getscreennode(httpdsettings, "webifip");
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
	httpdport->input = mask(httpdport->input, 5, "0");

	addchoicebox(webifip, "0", _("intern"));
	addchoicebox(webifip, "1", _("extern"));
	setchoiceboxselection(webifip, getconfig("webifip", NULL));

	changemask(streamport, "00000");
	changeinput(streamport, getconfig("streamport", NULL));
	streamport->input = mask(streamport->input, 5, "0");
	
	addchoicebox(rguidstart, "0", _("no"));
	addchoicebox(rguidstart, "1", _("yes"));
	setchoiceboxselection(rguidstart, getconfig("rguidstart", NULL));

	changemask(rguidport, "00000");
	changeinput(rguidport, getconfig("rguidport", NULL));
	rguidport->input = mask(rguidport->input, 5, "0");

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
				addconfigint("httpdport", hport);
			}

			addconfigscreencheck("webifip", webifip, "0");

			if(streamport->ret != NULL)
			{
				int sport = atoi(streamport->ret);
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
				addconfigint("rguidport", rport);
			}
			break;
		}
	}

	delownerrc(httpdsettings);
	clearscreen(httpdsettings);
}

#endif
