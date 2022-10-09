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
	struct skin* stream_port = getscreennode(httpdsettings, "stream_port");
	struct skin* stream_zapping = getscreennode(httpdsettings, "stream_zapping");
	struct skin* stream_ecm = getscreennode(httpdsettings, "stream_ecm");
	struct skin* stream_scrambled = getscreennode(httpdsettings, "stream_scrambled");
	struct skin* rec_scrambled = getscreennode(httpdsettings, "rec_scrambled");
	struct skin* rguidstart = getscreennode(httpdsettings, "rguidstart");
	struct skin* rguidport = getscreennode(httpdsettings, "rguidport");
	struct skin* webifip = getscreennode(httpdsettings, "webifip");
	struct skin* transcode = getscreennode(httpdsettings, "transcode");
	struct skin* bitrate = getscreennode(httpdsettings, "bitrate");
	struct skin* resolution = getscreennode(httpdsettings, "resolution");
	struct skin* aspectratio = getscreennode(httpdsettings, "aspectratio");

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

	changemask(stream_port, "00000");
	changeinput(stream_port, getconfig("stream_port", NULL));
	stream_port->input = mask(stream_port->input, 5, "0");
	
	addchoicebox(rguidstart, "0", _("no"));
	addchoicebox(rguidstart, "1", _("yes"));
	setchoiceboxselection(rguidstart, getconfig("rguidstart", NULL));

	changemask(rguidport, "00000");
	changeinput(rguidport, getconfig("rguidport", NULL));
	rguidport->input = mask(rguidport->input, 5, "0");

	addchoicebox(stream_zapping, "0", _("no"));
	addchoicebox(stream_zapping, "1", _("yes"));
	setchoiceboxselection(stream_zapping, getconfig("stream_zapping", NULL));

	addchoicebox(stream_ecm, "0", _("no"));
	addchoicebox(stream_ecm, "1", _("yes"));
	setchoiceboxselection(stream_ecm, getconfig("stream_ecm", NULL));

	addchoicebox(stream_scrambled, "0", _("no"));
	addchoicebox(stream_scrambled, "1", _("yes"));
	setchoiceboxselection(stream_scrambled, getconfig("stream_scrambled", NULL));

	addchoicebox(rec_scrambled, "0", _("no"));
	addchoicebox(rec_scrambled, "1", _("yes"));
	setchoiceboxselection(rec_scrambled, getconfig("rec_scrambled", NULL));
	
#ifndef MIPSEL
	transcode->hidden=YES;
	bitrate->hidden=YES;
	resolution->hidden=YES;
	aspectratio->hidden=YES;

#else
	addchoicebox(transcode, "0", _("off"));
	addchoicebox(transcode, "1", _("on"));
	setchoiceboxselection(transcode, getconfig("web_trans_transcode", NULL));
	
	addchoicebox(bitrate, "-1", _("Auto"));
	addchoicebox(bitrate, "100000", "100 Kbits");
	addchoicebox(bitrate, "150000", "150 Kbits");
	addchoicebox(bitrate, "200000", "200 Kbits");
	addchoicebox(bitrate, "250000", "250 Kbits");
	addchoicebox(bitrate, "300000", "300 Kbits");
	addchoicebox(bitrate, "350000", "350 Kbits");
	addchoicebox(bitrate, "400000", "400 Kbits");
	addchoicebox(bitrate, "450000", "450 Kbits");
	addchoicebox(bitrate, "500000", "500 Kbits");
	addchoicebox(bitrate, "750000", "750 Kbits");
	addchoicebox(bitrate, "1000000", "1 Mbits");
	addchoicebox(bitrate, "1500000", "1.5 Mbits");
	addchoicebox(bitrate, "2000000", "2 Mbits");
	addchoicebox(bitrate, "2500000", "2.5 Mbits");
	addchoicebox(bitrate, "3000000", "3 Mbits");
	addchoicebox(bitrate, "3500000", "3.5 Mbits");
	addchoicebox(bitrate, "4000000", "4 Mbits");
	addchoicebox(bitrate, "4500000", "4.5 Mbits");
	addchoicebox(bitrate, "5000000", "5 Mbits");
	addchoicebox(bitrate, "10000000", "10 Mbits");
	setchoiceboxselection(bitrate, getconfig("web_trans_bitrate", NULL));
	
	addchoicebox(resolution, "0", "480p");
	addchoicebox(resolution, "1", "576p");
	addchoicebox(resolution, "2", "720p");
	addchoicebox(resolution, "3", "320x240");
	addchoicebox(resolution, "4", "160x120");
	setchoiceboxselection(resolution, getconfig("web_trans_resolution", NULL));

	addchoicebox(aspectratio, "0", _("auto"));
	addchoicebox(aspectratio, "1", "4x3");
	addchoicebox(aspectratio, "2", "16x9");
	setchoiceboxselection(aspectratio, getconfig("web_trans_aspectratio", NULL));

	if(checkchipset("BCM7424") != 1)
	{
		transcode->hidden=YES;
		bitrate->hidden=YES;
		resolution->hidden=YES;
		aspectratio->hidden=YES;
	}
	else
	{
		transcode->hidden=NO;
		if(getconfigint("web_trans_transcode", NULL) == 0)
		{
			bitrate->hidden=YES;
			resolution->hidden=YES;
			aspectratio->hidden=YES;
		}
		else
		{	
			bitrate->hidden=NO;
			resolution->hidden=NO;
			aspectratio->hidden=NO;
		}
	}
#endif
	

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
			addconfigscreencheck("stream_zapping", stream_zapping, "0");
			addconfigscreencheck("stream_ecm", stream_ecm, "0");
			addconfigscreencheck("stream_scrambled", stream_scrambled, "0");
			addconfigscreencheck("rec_scrambled", rec_scrambled, "0");

			if(stream_port->ret != NULL)
			{
				int sport = atoi(stream_port->ret);
				addconfigint("stream_port", sport);
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
#ifndef MIPSEL
			break;
		}
#else
			addconfigscreencheck("web_trans_transcode", transcode, "0");
			addconfigscreencheck("web_trans_bitrate", bitrate, "0");
			addconfigscreencheck("web_trans_resolution", resolution, "0");
			addconfigscreencheck("web_trans_aspectratio", aspectratio, "0");
			break;
		}
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "transcode") == 0)
		{
			if(ostrcmp(transcode->ret, "0") == 0)
			{
				bitrate->hidden=YES;
				resolution->hidden=YES;
				aspectratio->hidden=YES;
			}
			else
			{
				bitrate->hidden=NO;
				resolution->hidden=NO;
				aspectratio->hidden=NO;
			}
			drawscreen(httpdsettings, 0, 0);
		}
#endif
	}

	delownerrc(httpdsettings);
	clearscreen(httpdsettings);
}

#endif
