#ifndef DEFAULTS_H
#define DEFAULTS_H

void setdefaults()
{
	addconfigdef("vol", "30");
	addconfigdef("maxsat", "4");
	addconfigdef("dmxvideobuffersize", "1048576");
	addconfigdef("dmxaudiobuffersize", "1048576");
	addconfigdef("dmxepgbuffersize", "524288");
	addconfigdef("dmxrecordbuffersize", "3145728");
	addconfigdef("dmxtimeshiftbuffersize", "3145728");
	addconfigdef("dmxstreambuffersize", "3145728");
	addconfigdef("rec_moviepath", "/media");
	addconfigdef("rec_path", "/media/hdd/movie");
	addconfigdef("rec_timeshiftpath", "/media/hdd/movie");
	addconfigdef("rec_timerpath", "/media/hdd/movie");
	addconfigdef("rec_streampath", "/media/hdd/movie");
	addconfigdef("epg_path", "/media/hdd");
	addconfigdef("epg_days", "14");
	addconfigdef("epg_del", "0");
	addconfigdef("spinnerspeed", "12");
	addconfigdef("spinnertime", "5");
	addconfigdef("hangtime", "15");
	addconfigdef("skinfbwidth", "1280");
	addconfigdef("skinfbheight", "720");
	addconfigdef("fbleftoffset", "0");
	addconfigdef("fbrightoffset", "0");
	addconfigdef("fbtopoffset", "0");
	addconfigdef("fbbottomoffset", "0");
	addconfigdef("infobartimeout", "3");
	addconfigdef("infobarsleep", "3");
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
	addconfigdef("recordfreespace", "1000");
	addconfigdef("skip13", "120");
	addconfigdef("skip46", "300");
	addconfigdef("skip79", "900");
	addconfigdef("gmultiepgzoom", "4");
	
	status.expertmodus = getconfigint("expertmodus", NULL);
	status.pmtmode = getconfigint("pmtmode", NULL);
	status.usedirectfb = getconfigint("usedirectfb", NULL);
	status.servicetype = getconfigint("servicetype", NULL);
	status.maxsat = getconfigint("maxsat", NULL);
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
}

#endif
