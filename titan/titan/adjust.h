#ifndef ADJUST_H
#define ADJUST_H

void screenadjust()
{
	int rcret = 0;
	char* tmpstr = NULL;
	char* ret = NULL;

	struct skin* adjust = getscreen("adjust");
	struct skin* listbox = getscreennode(adjust, "listbox");
	struct skin* volbartimeout = getscreennode(adjust, "volbartimeout");
	struct skin* infobartimeout = getscreennode(adjust, "infobartimeout");
	struct skin* infobarsleep = getscreennode(adjust, "infobarsleep");
	struct skin* secondinfobar = getscreennode(adjust, "secondinfobar");
	struct skin* spinnerspeed = getscreennode(adjust, "spinnerspeed");
	struct skin* spinnertime = getscreennode(adjust, "spinnertime");
	struct skin* hangtime = getscreennode(adjust, "hangtime");
	struct skin* nozapclear = getscreennode(adjust, "nozapclear");
	struct skin* fastzap = getscreennode(adjust, "fastzap");
	struct skin* faststop = getscreennode(adjust, "faststop");
	struct skin* dirsort = getscreennode(adjust, "dirsort");
	struct skin* poweraktion = getscreennode(adjust, "poweraktion");
	struct skin* frontpoweraktion = getscreennode(adjust, "frontpoweraktion");
	struct skin* virtualzap = getscreennode(adjust, "virtualzap");
	struct skin* fasttextrender = getscreennode(adjust, "fasttextrender");
	struct skin* recsplitsize = getscreennode(adjust, "recsplitsize");
	struct skin* recforerun = getscreennode(adjust, "recforerun");
	struct skin* recoverrun = getscreennode(adjust, "recoverrun");
	struct skin* skip13 = getscreennode(adjust, "skip13");
	struct skin* skip46 = getscreennode(adjust, "skip46");
	struct skin* skip79 = getscreennode(adjust, "skip79");
	struct skin* playertype = getscreennode(adjust, "playertype");
	struct skin* autochangechannelname = getscreennode(adjust, "autochangechannelname");
	struct skin* def_rectimer_after = getscreennode(adjust, "def_rectimer_after");
	struct skin* showchanneltimeline = getscreennode(adjust, "showchanneltimeline");
	struct skin* screenanim = getscreennode(adjust, "screenanim");
	struct skin* screenanimspeed = getscreennode(adjust, "screenanimspeed");
	struct skin* channellistview = getscreennode(adjust, "channellistview");
	struct skin* showlastpos = getscreennode(adjust, "showlastpos");
	struct skin* recsync = getscreennode(adjust, "recsync");
	struct skin* recordnamefmt = getscreennode(adjust, "recordnamefmt");
	struct skin* newsletter = getscreennode(adjust, "newsletter");
	struct skin* showhiddenfiles = getscreennode(adjust, "showhiddenfiles");
	struct skin* expertmodus = getscreennode(adjust, "expertmodus");
	struct skin* infobarprogram = getscreennode(adjust, "infobarprogram");
	struct skin* crosscontrol = getscreennode(adjust, "crosscontrol");
	struct skin* emucontrol = getscreennode(adjust, "emucontrol");
	struct skin* usecec = getscreennode(adjust, "usecec");
	struct skin* playerbuffersize = getscreennode(adjust, "playerbuffersize");
	struct skin* playerbufferseektime = getscreennode(adjust, "playerbufferseektime");
	struct skin* sataswitch = getscreennode(adjust, "sataswitch");
	struct skin* dualboot = getscreennode(adjust, "dualboot");
	struct skin* community_user = getscreennode(adjust, "community_user");
	struct skin* community_pass = getscreennode(adjust, "community_pass");
	struct skin* debuglevel = getscreennode(adjust, "debuglevel");
	struct skin* zapmode = getscreennode(adjust, "zapmode");
#if defined(OVBUILD) || defined (OEBUILD)
	struct skin* extensions_type = getscreennode(adjust, "extensions_type");
#endif

	struct skin* tmp = NULL;

	changeinput(volbartimeout, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
	setchoiceboxselection(volbartimeout, getconfig("volbartimeout", NULL));

	changeinput(infobartimeout, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n20\n30\n60\n10000");
	setchoiceboxselection(infobartimeout, getconfig("infobartimeout", NULL));

	changeinput(infobarsleep, "0\n1\n2\n3\n4\n5");
	setchoiceboxselection(infobarsleep, getconfig("infobarsleep", NULL));

	addchoicebox(secondinfobar, "0", _("no"));
	addchoicebox(secondinfobar, "1", _("infobar"));
	addchoicebox(secondinfobar, "2", _("EPG"));
	addchoicebox(secondinfobar, "3", _("Channel EPG"));
	setchoiceboxselection(secondinfobar, getconfig("secondinfobar", NULL));

	changeinput(spinnerspeed, "1\n3\n9\n12\n15\n18\n21\n24\n27\n30");
	setchoiceboxselection(spinnerspeed, getconfig("spinnerspeed", NULL));

	changeinput(spinnertime, "1\n2\n3\n4\n5\n6\n7\n8\n10000");
	setchoiceboxselection(spinnertime, getconfig("spinnertime", NULL));

	changeinput(hangtime, "5\n10\n15\n30\n60\n120\n10000");
	setchoiceboxselection(hangtime, getconfig("hangtime", NULL));

	addchoicebox(nozapclear, "0", _("no"));
	addchoicebox(nozapclear, "1", _("yes"));
	setchoiceboxselection(nozapclear, getconfig("nozapclear", NULL));

	if(checkbox("ATEMIO7600") == 1 || vubox1 == 1)
		fastzap->hidden = YES;
	else
		fastzap->hidden = NO;

	addchoicebox(fastzap, "0", _("no"));
	addchoicebox(fastzap, "2", _("medium"));
	addchoicebox(fastzap, "1", _("fast"));
	setchoiceboxselection(fastzap, getconfig("fastzap", NULL));

	addchoicebox(faststop, "0", _("no"));
	addchoicebox(faststop, "1", _("yes"));
	setchoiceboxselection(faststop, getconfig("faststop", NULL));

	addchoicebox(dirsort, "0", _("alpha"));
	addchoicebox(dirsort, "1", _("reverse alpha"));
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("dirsort", NULL));

	addchoicebox(poweraktion, "0", _("Power Menu"));
	addchoicebox(poweraktion, "1", _("Power Off"));
	addchoicebox(poweraktion, "2", _("Standby"));
	addchoicebox(poweraktion, "3", _("Restart"));
	addchoicebox(poweraktion, "4", _("GUI Restart"));
	setchoiceboxselection(poweraktion, getconfig("poweraktion", NULL));

	addchoicebox(frontpoweraktion, "1", _("Power Off"));
	addchoicebox(frontpoweraktion, "2", _("Standby"));
	setchoiceboxselection(frontpoweraktion, getconfig("frontpoweraktion", NULL));

	addchoicebox(virtualzap, "0", _("disable"));
	addchoicebox(virtualzap, "1", _("1 sec"));
	addchoicebox(virtualzap, "2", _("2 sec"));
	addchoicebox(virtualzap, "9999", _("endless"));
	setchoiceboxselection(virtualzap, getconfig("virtualzap", NULL));

	addchoicebox(fasttextrender, "0", _("no"));
	addchoicebox(fasttextrender, "1", _("yes"));
	setchoiceboxselection(fasttextrender, getconfig("fasttextrender", NULL));

	addchoicebox(recsplitsize, "0", _("disable"));
	addchoicebox(recsplitsize, "1", _("1 GB"));
	addchoicebox(recsplitsize, "2", _("2 GB"));
	addchoicebox(recsplitsize, "3", _("3 GB"));
	addchoicebox(recsplitsize, "4", _("4 GB"));
	setchoiceboxselection(recsplitsize, getconfig("recsplitsize", NULL));

	changeinput(recforerun, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60");
	setchoiceboxselection(recforerun, getconfig("recforerun", NULL));

	changeinput(recoverrun, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60");
	setchoiceboxselection(recoverrun, getconfig("recoverrun", NULL));

	changeinput(skip13, "15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200");
	setchoiceboxselection(skip13, getconfig("skip13", NULL));

	changeinput(skip46, "15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200");
	setchoiceboxselection(skip46, getconfig("skip46", NULL));

	changeinput(skip79, "15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200");
	setchoiceboxselection(skip79, getconfig("skip79", NULL));

	addchoicebox(playertype, "0", _("extern"));
	addchoicebox(playertype, "1", _("intern"));
	setchoiceboxselection(playertype, getconfig("playertype", NULL));

	addchoicebox(autochangechannelname, "0", _("no"));
	addchoicebox(autochangechannelname, "1", _("yes"));
	setchoiceboxselection(autochangechannelname, getconfig("autochangechannelname", NULL));

	addchoicebox(def_rectimer_after, "0", _("auto"));
	addchoicebox(def_rectimer_after, "1", _("nothing"));
	addchoicebox(def_rectimer_after, "2", _("standby"));
	addchoicebox(def_rectimer_after, "3", _("power off"));
	setchoiceboxselection(def_rectimer_after, getconfig("def_rectimer_after", NULL));

	addchoicebox(showchanneltimeline, "0", _("no"));
	addchoicebox(showchanneltimeline, "1", _("yes"));
	setchoiceboxselection(showchanneltimeline, getconfig("showchanneltimeline", NULL));

	addchoicebox(screenanim, "0", _("no"));
	addchoicebox(screenanim, "1", _("anim. width"));
	addchoicebox(screenanim, "2", _("anim. height"));
	addchoicebox(screenanim, "3", _("anim. both"));
#ifdef MIPSEL
	addchoicebox(screenanim, "4", _("left to right"));
	addchoicebox(screenanim, "5", _("top to bottom"));
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
	{
		addchoicebox(screenanim, "11", _("slide from left"));
		addchoicebox(screenanim, "12", _("slide from bottom"));
	}
#endif
	setchoiceboxselection(screenanim, getconfig("screenanim", NULL));

	addchoicebox(screenanimspeed, "1", _("very fast"));
	addchoicebox(screenanimspeed, "5", _("fast"));
	addchoicebox(screenanimspeed, "10", _("normal"));
	addchoicebox(screenanimspeed, "15", _("slow"));
	addchoicebox(screenanimspeed, "20", _("very slow"));
	setchoiceboxselection(screenanimspeed, getconfig("screenanimspeed", NULL));

	addchoicebox(channellistview, "0", _("hidden"));
	addchoicebox(channellistview, "1", _("show"));
	setchoiceboxselection(channellistview, getconfig("channellistview", NULL));

	addchoicebox(showlastpos, "0", _("no"));
	addchoicebox(showlastpos, "1", _("yes"));
	setchoiceboxselection(showlastpos, getconfig("showlastpos", NULL));

	addchoicebox(recsync, "0", _("no"));
	addchoicebox(recsync, "1", _("yes"));
	setchoiceboxselection(recsync, getconfig("recsync", NULL));

	addchoicebox(recordnamefmt, "0", _("channel-movie"));
	addchoicebox(recordnamefmt, "1", _("movie-channel"));
	setchoiceboxselection(recordnamefmt, getconfig("recordnamefmt", NULL));

	addchoicebox(newsletter, "0", _("no"));
	addchoicebox(newsletter, "1", _("yes"));
	setchoiceboxselection(newsletter, getconfig("newsletter", NULL));

	addchoicebox(showhiddenfiles, "0", _("no"));
	addchoicebox(showhiddenfiles, "1", _("yes"));
	setchoiceboxselection(showhiddenfiles, getconfig("showhiddenfiles", NULL));

	addchoicebox(expertmodus, "0", _("no"));
	addchoicebox(expertmodus, "10", _("yes"));
	if(getconfigint("expertmodus", NULL) == 11)
		addchoicebox(expertmodus, "11", _("expert (11)"));
	setchoiceboxselection(expertmodus, getconfig("expertmodus", NULL));

	addchoicebox(infobarprogram, "0", _("no"));
	addchoicebox(infobarprogram, "1", _("yes"));
	setchoiceboxselection(infobarprogram, getconfig("infobarprogram", NULL));

	addchoicebox(crosscontrol, "0", _("no"));
	addchoicebox(crosscontrol, "1", _("yes"));
	addchoicebox(crosscontrol, "2", _("yes / vzap 1 sec"));
	addchoicebox(crosscontrol, "3", _("yes / vzap 2 sec"));
	addchoicebox(crosscontrol, "9999", _("yes / vzap endless"));
	setchoiceboxselection(crosscontrol, getconfig("crosscontrol", NULL));

	addchoicebox(emucontrol, "0", _("no"));
	addchoicebox(emucontrol, "1", _("yes"));
	setchoiceboxselection(emucontrol, getconfig("emucontrol", NULL));

	addchoicebox(usecec, "0", _("no"));
	addchoicebox(usecec, "1", _("yes"));
	setchoiceboxselection(usecec, getconfig("usecec", NULL));
#if defined(OVBUILD) || defined (OEBUILD)
	addchoicebox(extensions_type, "0", _("Tpk"));
	addchoicebox(extensions_type, "1", _("Ipkg"));
	addchoicebox(extensions_type, "2", _("Tpk / Ipkg"));
	setchoiceboxselection(extensions_type, getconfig("extensions_type", NULL));
#endif
	if(!file_exist("/mnt/config/dualboot"))
	{
		addchoicebox(dualboot, "0", _("no"));
		addchoicebox(dualboot, "1", _("yes"));
	}
	else
	{
		addchoicebox(dualboot, "1", _("yes"));
		addchoicebox(dualboot, "0", _("no"));
	}
	setchoiceboxselection(dualboot, getconfig("dualboot", NULL));

	changemask(community_user, "****");
	if(getconfig("community_user", NULL) == NULL)
		changeinput(community_user, getconfig("community_user", NULL));
	else
		changeinput(community_pass, "****");

	changemask(community_pass, "****");
	if(getconfig("community_pass", NULL) == NULL)
		changeinput(community_pass, getconfig("community_pass", NULL));
	else
		changeinput(community_pass, "****");

//	if(file_exist("/proc/stb/video/zapmode") || file_exist("/proc/stb/video/zapping_mode"))
	if(file_exist(getconfig("zapmodedev", NULL)))
	{
		addchoicebox(zapmode, "mute", _("Black screen"));
		addchoicebox(zapmode, "hold", _("Hold screen"));
		addchoicebox(zapmode, "mutetilllock", _("Black screen till locked"));
		addchoicebox(zapmode, "holdtilllock", _("Hold till locked"));
		ret = getzapmode();
		setchoiceboxselection(zapmode, ret);
		free(ret); ret = NULL;
	}
	else
		zapmode->hidden = YES;

// setdebuglevel
//#ifndef BETA
//	debuglevel->hidden = YES;
//#else
	tmpstr = ostrcat("10 - ", _("Minimal"), 0, 0);
	addchoicebox(debuglevel, "10", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("40 - ", _("MTD Operation/System Update"), 0, 0);
	addchoicebox(debuglevel, "40", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("50 - ", _("MediaCenter"), 0, 0);
	addchoicebox(debuglevel, "50", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("55 - ", _("Network Interface"), 0, 0);
	addchoicebox(debuglevel, "55", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("60 - ", _("Key Actions"), 0, 0);
	addchoicebox(debuglevel, "60", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("70 - ", _("Networkbrowser"), 0, 0);
	addchoicebox(debuglevel, "70", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("77 - ", _("Web Adjust"), 0, 0);
	addchoicebox(debuglevel, "77", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("80 - ", _("Harddisk"), 0, 0);
	addchoicebox(debuglevel, "80", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("81 - ", _("MultiImage"), 0, 0);
	addchoicebox(debuglevel, "81", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("88 - ", _("Tithek"), 0, 0);
	addchoicebox(debuglevel, "88", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("90 - ", _("Screensaver"), 0, 0);
	addchoicebox(debuglevel, "90", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("99 - ", _("HTTP Header/Community"), 0, 0);
	addchoicebox(debuglevel, "99", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("100 - ", _("Global"), 0, 0);
	addchoicebox(debuglevel, "100", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("110 - ", _("Stringconvert"), 0, 0);
	addchoicebox(debuglevel, "110", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("130 - ", _("TPK"), 0, 0);
	addchoicebox(debuglevel, "130", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("133 - ", _("MediaDB/IMDb/TMDb"), 0, 0);
	addchoicebox(debuglevel, "133", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("150 - ", _("ePlayer"), 0, 0);
	addchoicebox(debuglevel, "150", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("200 - ", _("DVB Devices"), 0, 0);
	addchoicebox(debuglevel, "200", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("201 - ", _("CA Device"), 0, 0);
	addchoicebox(debuglevel, "201", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("202 - ", _("Iptv"), 0, 0);
	addchoicebox(debuglevel, "202", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("250 - ", _("HTTP Server/Stream/Record"), 0, 0);
	addchoicebox(debuglevel, "250", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("270 - ", _("Radiotext"), 0, 0);
	addchoicebox(debuglevel, "270", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("278 - ", _("ID3"), 0, 0);
	addchoicebox(debuglevel, "278", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("300 - ", _("Subtitle"), 0, 0);
	addchoicebox(debuglevel, "300", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("307 - ", _("Thumbnails"), 0, 0);
	addchoicebox(debuglevel, "307", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("333 - ", _("DVD Player"), 0, 0);
	addchoicebox(debuglevel, "333", tmpstr);
	free(tmpstr), tmpstr = NULL;
/*
	tmpstr = ostrcat("369 - ", _("Facebook"), 0, 0);
	addchoicebox(debuglevel, "369", tmpstr);
	free(tmpstr), tmpstr = NULL;
*/
	tmpstr = ostrcat("400 - ", _("EPG Task"), 0, 0);
	addchoicebox(debuglevel, "400", tmpstr);
	free(tmpstr), tmpstr = NULL;


	tmpstr = ostrcat("401 - ", _("Old Entry"), 0, 0);
	addchoicebox(debuglevel, "401", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("427 - ", _("Newsletter"), 0, 0);
	addchoicebox(debuglevel, "427", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("444 - ", _("Framebuffer"), 0, 0);
	addchoicebox(debuglevel, "444", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("500 - ", _("Channel Scan"), 0, 0);
	addchoicebox(debuglevel, "500", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("555 - ", _("Draw Screen"), 0, 0);
	addchoicebox(debuglevel, "555", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("620 - ", _("CA"), 0, 0);
	addchoicebox(debuglevel, "620", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("777 - ", _("MediaDB"), 0, 0);
	addchoicebox(debuglevel, "777", tmpstr);
	free(tmpstr), tmpstr = NULL;
/*
	tmpstr = ostrcat("788 - ", _("HbbTV"), 0, 0);
	addchoicebox(debuglevel, "788", tmpstr);
	free(tmpstr), tmpstr = NULL;
*/
	tmpstr = ostrcat("900 - ", _("Mutex"), 0, 0);
	addchoicebox(debuglevel, "900", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("913 - ", _("File Operation"), 0, 0);
	addchoicebox(debuglevel, "913", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("975 - ", _("SMTP"), 0, 0);
	addchoicebox(debuglevel, "975", tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat("4440 - ", _("PiP"), 0, 0);
	addchoicebox(debuglevel, "4440", tmpstr);
	free(tmpstr), tmpstr = NULL;

	setchoiceboxselection(debuglevel, getconfig("debuglevel", NULL));
//#endif


#ifdef MIPSEL
	dualboot->hidden = NO;
	frontpoweraktion->hidden = NO;
#else
	dualboot->hidden = YES;
	frontpoweraktion->hidden = YES;
#endif

	if(!file_exist("/usr/bin/enigma2"))
		dualboot->hidden = YES;

	if(status.security == 0 || checkemu() == 0)
		emucontrol->hidden = YES;
	else
		emucontrol->hidden = NO;
	if(checkbox("ATEMIO510") == 1 || checkbox("UFS912") == 1 || checkbox("UFS913") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
		usecec->hidden = NO;
	else
		usecec->hidden = YES;

	if(checkbox("ATEMIO7600") == 1)
	{
		sataswitch->hidden = NO;
		addchoicebox(sataswitch, "0", _("extern"));
		addchoicebox(sataswitch, "1", _("intern"));
		tmpstr = getsataswitch();
		setchoiceboxselection(sataswitch, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}
	else
		sataswitch->hidden = YES;

	addchoicebox(playerbuffersize, "0", _("no"));
	addchoicebox(playerbuffersize, "524288", "512KB");
	addchoicebox(playerbuffersize, "1048576", "1MB");
	addchoicebox(playerbuffersize, "1572864", "1,5MB");
	addchoicebox(playerbuffersize, "2097152", "2MB");
	addchoicebox(playerbuffersize, "2621440", "2,5MB");
	addchoicebox(playerbuffersize, "3145728", "3MB");
	addchoicebox(playerbuffersize, "3670016", "3,5MB");
	addchoicebox(playerbuffersize, "4194304", "4MB");
	addchoicebox(playerbuffersize, "4718592", "4,5MB");
	addchoicebox(playerbuffersize, "5242880", "5MB");
	setchoiceboxselection(playerbuffersize, getconfig("playerbuffersize", NULL));

	addchoicebox(playerbufferseektime, "0", "0");
	addchoicebox(playerbufferseektime, "1", "1");
	addchoicebox(playerbufferseektime, "2", "2");
	addchoicebox(playerbufferseektime, "3", "3");
	addchoicebox(playerbufferseektime, "4", "4");
	addchoicebox(playerbufferseektime, "5", "5");
	addchoicebox(playerbufferseektime, "6", "6");
	addchoicebox(playerbufferseektime, "7", "7");
	addchoicebox(playerbufferseektime, "8", "8");
	addchoicebox(playerbufferseektime, "9", "9");
	addchoicebox(playerbufferseektime, "10", "10");
	setchoiceboxselection(playerbufferseektime, getconfig("playerbufferseektime", NULL));

	drawscreen(adjust, 0, 0);
	addscreenrc(adjust, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(adjust, tmp);
		rcret = waitrc(adjust, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreen("volbartimeout", volbartimeout);
			addconfigscreen("infobartimeout", infobartimeout);
			addconfigscreen("infobarsleep", infobarsleep);
			addconfigscreencheck("secondinfobar", secondinfobar, "0");
			addconfigscreen("spinnerspeed", spinnerspeed);
			status.spinnerspeed = getconfigint("spinnerspeed", NULL);
			addconfigscreen("spinnertime", spinnertime);
			status.spinnertime = getconfigint("spinnertime", NULL);
			addconfigscreen("hangtime", hangtime);
			status.hangtime = getconfigint("hangtime", NULL);
			addconfigscreencheck("nozapclear", nozapclear, "0");
			addconfigscreencheck("fastzap", fastzap, "0");
			addconfigscreencheck("faststop", faststop, "0");
			addconfigscreencheck("dirsort", dirsort, "0");
			addconfigscreencheck("poweraktion", poweraktion, "0");
			addconfigscreencheck("frontpoweraktion", frontpoweraktion, "0");
			addconfigscreencheck("recforerun", recforerun, "0");
			addconfigscreencheck("recoverrun", recoverrun, "0");
			addconfigscreencheck("virtualzap", virtualzap, "0");
			status.virtualzap = getconfigint("virtualzap", NULL);
			if(fasttextrender->ret != NULL)
			{
				addconfigscreencheck("fasttextrender", fasttextrender, "0");
				status.fasttextrender = atoi(fasttextrender->ret);
			}
			if(recsplitsize->ret != NULL)
			{
				addconfigscreencheck("recsplitsize", recsplitsize, "0");
				status.recsplitsize = atoi(recsplitsize->ret);
				status.recsplitsize *= 1000 * 1000 * 1000;
			}

			addconfigscreen("skip13", skip13);
			addconfigscreen("skip46", skip46);
			addconfigscreen("skip79", skip79);
			addconfigscreencheck("playertype", playertype, "0");
			addconfigscreencheck("autochangechannelname", autochangechannelname, "0");
			status.autochangechannelname = getconfigint("autochangechannelname", NULL);
			addconfigscreencheck("def_rectimer_after", def_rectimer_after, "0");
			addconfigscreencheck("showchanneltimeline", showchanneltimeline, "0");
			status.showchanneltimeline = getconfigint("showchanneltimeline", NULL);
			addconfigscreencheck("screenanim", screenanim, "0");
			status.screenanim = getconfigint("screenanim", NULL);
			addconfigscreencheck("screenanimspeed", screenanimspeed, "1");
			status.screenanimspeed = getconfigint("screenanimspeed", NULL);
			addconfigscreencheck("channellistview", channellistview, "0");
			status.channellistview = getconfigint("channellistview", NULL);
			addconfigscreencheck("showlastpos", showlastpos, "0");
			addconfigscreencheck("recsync", recsync, "0");
			addconfigscreencheck("recordnamefmt", recordnamefmt, "0");

			addconfigscreen("newsletter", newsletter);
			if(newsletter->ret != NULL && ostrcmp(newsletter->ret, "0") == 0)
				startnewsletter(0);
			else
				startnewsletter(1);

			addconfigscreen("showhiddenfiles", showhiddenfiles);
			status.showhiddenfiles = getconfigint("showhiddenfiles", NULL);
			addconfigscreencheck("expertmodus", expertmodus, "0");
			status.expertmodus = getconfigint("expertmodus", NULL);
			addconfigscreencheck("infobarprogram", infobarprogram, "0");
			status.infobarprogram = getconfigint("infobarprogram", NULL);
			addconfigscreen("crosscontrol", crosscontrol);
			//addconfigscreencheck("crosscontrol", crosscontrol, "0");
			status.crosscontrol = getconfigint("crosscontrol", NULL);
			addconfigscreencheck("emucontrol", emucontrol, "0");

			if(checkbox("ATEMIO510") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("UFS912") == 1 || checkbox("UFS913") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
				addconfigscreencheck("usecec", usecec, "0");
#if defined(OVBUILD) || defined (OEBUILD)
			addconfigscreencheck("extensions_type", extensions_type, "0");
			if(extensions_type->ret != NULL)
			{
	            //set skinentrys locked
	            if(status.security == 0)
		            setskinnodeslocked(1);
	            else
		            setskinnodeslocked(0);
			}
#endif
			if(checkbox("ATEMIO7600") == 1)
			{
				addconfigscreencheck("usecec", usecec, "0");
				if(sataswitch->ret != NULL)
				{
					addownconfig("sataswitch", sataswitch->ret);
					setsataswitch(sataswitch->ret);
				}
			}

#ifdef MIPSEL
			addconfigscreen("dualboot", dualboot);
			if(dualboot->ret != NULL && ostrcmp(dualboot->ret, "0") == 0)
				unlink("/mnt/config/dualboot");
			else
				system("touch /mnt/config/dualboot");
#endif

			addconfigscreen("zapmode", zapmode);
			if(zapmode->ret != NULL)
				setzapmode(zapmode->ret);

			addconfigscreen("playerbuffersize", playerbuffersize);
			addconfigscreen("playerbufferseektime", playerbufferseektime);

//#ifdef BETA
			addconfigscreencheck("debuglevel", debuglevel, "0");
			setdebuglevel();
//#endif
			if(community_user->ret != NULL && ostrcmp(community_user->ret, "****") != 0)
			{
				debug(99, "community_user: write");
				debug(99, "community_user: %s", community_user->ret);
				addconfigscreen("community_user", community_user);
			}
			else
			{
				debug(99, "community_user: skipped");
			}
			if(community_pass->ret != NULL && ostrcmp(community_pass->ret, "****") != 0)
			{
				debug(99, "community_pass: write");
				debug(99, "community_pass: %s", community_pass->ret);
				addconfigscreen("community_pass", community_pass);
			}
			else
			{
				debug(99, "community_pass: skipped");
			}
			writeallconfig(1);

			debug(99, "community_user read: %s", getconfig("community_user", NULL));
			debug(99, "community_pass read: %s", getconfig("community_pass", NULL));

			break;
		}
	}
	delownerrc(adjust);
	clearscreen(adjust);
}

#endif
