#ifndef DEFAULTS_H
#define DEFAULTS_H

void setdefaults()
{
	int i = 0;
	char* tmpstr = NULL;

	addconfigdef("vol", "30");
	addconfigdef("dmxvideobuffersize", "1048576");
	addconfigdef("dmxaudiobuffersize", "1048576");
	addconfigdef("dmxepgbuffersize", "524288");
	addconfigdef("dmxrecordbuffersize", "3145728");
	addconfigdef("dmxtimeshiftbuffersize", "3145728");
	addconfigdef("dmxstreambuffersize", "3145728");
	addconfigdef("mediadbpath", "/media/hdd/mediadb");
	addconfigdef("mediadbfile", "/media/hdd/mediadb/mediadb");
	addconfigdef("rec_moviepath", "/media");
	addconfigdef("rec_path", "/media/hdd/movie");
	addconfigdef("rec_timeshiftpath", "/media/hdd/movie");
	addconfigdef("rec_timerpath", "/media/hdd/movie");
	addconfigdef("rec_streampath", "/media/hdd/movie");
	addconfigdef("epg_path", "/media/hdd");
	addconfigdef("epg_days", "14");
	addconfigdef("epg_del", "0");
	addconfigdef("tracelog", "/media/hdd");
	addconfigdef("spinnerspeed", "12");
	addconfigdef("spinnertime", "5");
	addconfigdef("hangtime", "60");
	addconfigdef("skinfbwidth", "1280");
	addconfigdef("skinfbheight", "720");
	addconfigdef("fbleftoffset", "0");
	addconfigdef("fbrightoffset", "0");
	addconfigdef("fbtopoffset", "0");
	addconfigdef("fbbottomoffset", "0");
	addconfigdef("infobartimeout", "4");
	addconfigdef("infobarsleep", "0");
	addconfigdef("volbartimeout", "1");
	addconfigdef("mc_videoplayerpath", "/media");
	addconfigdef("vfdbrightness", "3");
	addconfigdef("vfdstandbybrightness", "3");
	addconfigdef("vs_brightness", "128");
	addconfigdef("vs_contrast", "128");
	addconfigdef("vs_tint", "128");
	addconfigdef("vs_saturation", "128");
	addconfigdef("epgfreespace", "100");
	addconfigdef("pincode", "0000");
	addconfigdef("pincodemenu", "0000");
	addconfigdef("httpdstart", "0");
	addconfigdef("httpdport", "80");
	addconfigdef("streamport", "22222");
	addconfigdef("rguidport", "22233");
	addconfigdef("recordfreespace", "1000");
	addconfigdef("skip13", "120");
	addconfigdef("skip46", "300");
	addconfigdef("skip79", "900");
	addconfigdef("gmultiepgzoom", "4");
	addconfigdef("wakeuptimerdevdiff", "300");
	addconfigdef("rectimer_timediff", "120");
	addconfigdef("screenanimspeed", "1");
	addconfigdef("blindminfrequency", "9500");
	addconfigdef("blindmaxfrequency", "14500");
	addconfigdef("blindstepfrequency", "20");
	addconfigdef("blindminsignalrate", "20000");
	addconfigdef("blindmaxsignalrate", "30000");
	addconfigdef("blindstepsignalrate", "20");
	
	status.expertmodus = getconfigint("expertmodus", NULL);
	status.pmtmode = getconfigint("pmtmode", NULL);
	status.usedirectfb = getconfigint("usedirectfb", NULL);
	status.servicetype = getconfigint("servicetype", NULL);
	status.spinnertime = getconfigint("spinnertime", NULL);
	status.spinnerspeed = getconfigint("spinnerspeed", NULL);
	status.hangtime = getconfigint("hangtime", NULL);
	status.epgdays = getconfigint("epg_days", NULL);
	status.fasttextrender = getconfigint("fasttextrender", NULL);
	status.recsplitsize = (off64_t)getconfigint("recsplitsize", NULL) * 1000 * 1000 * 1000;
	status.epglistmode = getconfigint("epg_listmode", NULL);
	status.listboxselecttype = getskinconfigint("listboxselecttype", NULL);
	status.borderradius = getskinconfigint("borderradius", NULL);
	status.httpauth = ostrcat(getconfig("httpauth", NULL), NULL, 0, 0);
	status.showchanneltimeline = getconfigint("showchanneltimeline", NULL);
	status.screenanim = getconfigint("screenanim", NULL);
	status.screenanimspeed = getconfigint("screenanimspeed", NULL);
	status.leftoffset = getconfigint("fbleftoffset", NULL);
	status.rightoffset = getconfigint("fbrightoffset", NULL);
	status.topoffset = getconfigint("fbtopoffset", NULL);
	status.bottomoffset = getconfigint("fbbottomoffset", NULL);
	status.createthumb = getconfigint("createthumb", NULL);
	
	status.bgpic = getskinconfig("bgpic", NULL);
	if(status.bgpic != NULL)
	{
		unsigned long width = 0, height = 0, rowbytes = 0;
		int channels = 0, memfd = -1, length = 0;
		unsigned char* buf = NULL;
		
		status.bgpic = changepicpath(status.bgpic);
		
		length = strlen(status.bgpic);
		if(status.bgpic[length - 1] == 'g' && status.bgpic[length - 2] == 'n' && status.bgpic[length - 3] == 'p')
		{
			buf = readpng(status.bgpic, &width, &height, &rowbytes, &channels, 0, 0, 0, 0, 0, 0);
			addpic(status.bgpic, buf, memfd, width, height, rowbytes, channels, 0, 0, NULL);
		}
	}

	for(i = 0; i < MAXLONGKEY; i++)
	{
		tmpstr = ostrcat("longkey", oitoa(i), 0, 1);
		status.longkeycode[i] = getrcconfigint(tmpstr, NULL);
		free(tmpstr); tmpstr = NULL;
	}
}

#endif
