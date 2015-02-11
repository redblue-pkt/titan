#ifndef ADJUST_H
#define ADJUST_H

void screenadjust()
{
	int rcret = 0;
	char* tmpstr = NULL;
	
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
	struct skin* minitv = getscreennode(adjust, "choiceminitv");
	struct skin* usecec = getscreennode(adjust, "usecec");
	struct skin* playerbuffersize = getscreennode(adjust, "playerbuffersize");
	struct skin* playerbufferseektime = getscreennode(adjust, "playerbufferseektime");
	struct skin* sataswitch = getscreennode(adjust, "sataswitch");
	struct skin* dualboot = getscreennode(adjust, "dualboot");
	struct skin* community_user = getscreennode(adjust, "community_user");
	struct skin* community_pass = getscreennode(adjust, "community_pass");
	struct skin* debuglevel = getscreennode(adjust, "debuglevel");

	struct skin* tmp = NULL;

	changeinput(volbartimeout, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
	setchoiceboxselection(volbartimeout, getconfig("volbartimeout", NULL));

	changeinput(infobartimeout, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n20\n30\n60\n10000");
	setchoiceboxselection(infobartimeout, getconfig("infobartimeout", NULL));

	changeinput(infobarsleep, "0\n1\n2\n3\n4\n5");
	setchoiceboxselection(infobarsleep, getconfig("infobarsleep", NULL));

	addchoicebox(secondinfobar, "0", _("no"));
	addchoicebox(secondinfobar, "1", _("infobar"));
	addchoicebox(secondinfobar, "2", _("epg"));
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

	if(checkbox("ATEMIO7600") == 1)
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
	addchoicebox(poweraktion, "4", _("Gui Restart"));
	setchoiceboxselection(poweraktion, getconfig("poweraktion", NULL));

	addchoicebox(frontpoweraktion, "1", _("Power Off"));
	addchoicebox(frontpoweraktion, "2", _("Standby"));
	setchoiceboxselection(frontpoweraktion, getconfig("frontpoweraktion", NULL));

	addchoicebox(virtualzap, "0", _("deaktiv"));
	addchoicebox(virtualzap, "1", _("1 sec"));
	addchoicebox(virtualzap, "2", _("2 sec"));
	addchoicebox(virtualzap, "9999", _("endless"));
	setchoiceboxselection(virtualzap, getconfig("virtualzap", NULL));
	
	addchoicebox(fasttextrender, "0", _("no"));
	addchoicebox(fasttextrender, "1", _("yes"));
	setchoiceboxselection(fasttextrender, getconfig("fasttextrender", NULL));

	addchoicebox(recsplitsize, "0", _("deaktiv"));
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
	setchoiceboxselection(screenanim, getconfig("screenanim", NULL));
	
	addchoicebox(screenanimspeed, "1", _("very fast"));
	addchoicebox(screenanimspeed, "5", _("fast"));
	addchoicebox(screenanimspeed, "10", _("normal"));
	addchoicebox(screenanimspeed, "15", _("slow"));
	addchoicebox(screenanimspeed, "20", _("very slow"));
	setchoiceboxselection(screenanimspeed, getconfig("screenanimspeed", NULL));

	addchoicebox(channellistview, "0", _("hidden"));
	addchoicebox(channellistview, "1", _("deaktiv"));
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

	addchoicebox(minitv, "0", _("no"));
	addchoicebox(minitv, "1", _("yes"));
	setchoiceboxselection(minitv, getconfig("minitv", NULL));

	addchoicebox(usecec, "0", _("no"));
	addchoicebox(usecec, "1", _("yes"));
	setchoiceboxselection(usecec, getconfig("usecec", NULL));

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

	changemask(community_user, "abcdefghijklmnopqrstuvwxyz");
	changeinput(community_user, getconfig("community_user", NULL));

//	changemask(community_pass, "abcdefghijklmnopqrstuvwxyz");
//	changeinput(community_pass, getconfig("community_pass", NULL));
	changemask(community_pass, "****");

	if(getconfig("community_pass", NULL) != NULL && strlen(getconfig("community_pass", NULL)) != 32)
		changeinput(community_pass, getconfig("community_pass", NULL));
	else
		changeinput(community_pass, "****");
	

// setdebuglevel
#ifndef BETA
	debuglevel->hidden = YES;
#else
	addchoicebox(debuglevel, "10", _("Minimal"));
	addchoicebox(debuglevel, "40", _("TopfieldVFD"));
	addchoicebox(debuglevel, "50", _("MediaCenter"));
	addchoicebox(debuglevel, "60", _("Menulist"));
	addchoicebox(debuglevel, "70", _("Networkbrowser"));
	addchoicebox(debuglevel, "80", _("Harddisk"));
	addchoicebox(debuglevel, "90", _("Screensaver"));
	addchoicebox(debuglevel, "99", _("Tithek/HTTP Header"));
	addchoicebox(debuglevel, "100", _("Global"));
	addchoicebox(debuglevel, "130", _("TPK"));
	addchoicebox(debuglevel, "133", _("IMDbScan"));
	addchoicebox(debuglevel, "150", _("ePlayer"));
	addchoicebox(debuglevel, "199", _("Community login"));
	addchoicebox(debuglevel, "200", _("Tuning"));
	addchoicebox(debuglevel, "250", _("Stream/Record/Softcam"));
	addchoicebox(debuglevel, "270", _("EPG"));
	addchoicebox(debuglevel, "300", _("Radiotext"));
	addchoicebox(debuglevel, "400", _("Subtitle"));
	addchoicebox(debuglevel, "500", _("Scan"));
	addchoicebox(debuglevel, "777", _("MediaDB"));
	addchoicebox(debuglevel, "900", _("Mutex"));
	addchoicebox(debuglevel, "1000", _("Function"));
	setchoiceboxselection(debuglevel, getconfig("debuglevel", NULL));
#endif


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
			addconfigscreencheck("playertype", playertype, "1");
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

			if(ostrcmp(minitv->ret, "1") == 0) resettvpic();
			addconfigscreencheck("minitv", minitv, "0");

			if(checkbox("ATEMIO510") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("UFS912") == 1 || checkbox("UFS913") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
				addconfigscreencheck("usecec", usecec, "0");

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
			addconfigscreen("playerbuffersize", playerbuffersize);
			addconfigscreen("playerbufferseektime", playerbufferseektime);

#ifdef BETA
			addconfigscreencheck("debuglevel", debuglevel, "0");
			setdebuglevel();
#endif

			addconfigscreen("community_user", community_user);
			// hid pass text and convert to md5sum
			if(community_pass->ret != NULL && ostrcmp(community_pass->ret, "****") != 0)
			{
				debug(99, "community_pass: write");
				debug(99, "community_pass: %s", community_pass->ret);
				addconfigscreen("community_pass", community_pass);
			}
			else
				debug(99, "community_pass: skipped");

			writeallconfig(1);

			debug(99, "community_pass_read: %s", getconfig("community_pass", NULL));

			break;
		}
	}

	delownerrc(adjust);
	clearscreen(adjust);
}

#endif
