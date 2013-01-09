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
	struct skin* virtualzap = getscreennode(adjust, "virtualzap");
	struct skin* fasttextrender = getscreennode(adjust, "fasttextrender");
	struct skin* recsplitsize = getscreennode(adjust, "recsplitsize");
	struct skin* forerun = getscreennode(adjust, "recforerun");
	struct skin* overrun = getscreennode(adjust, "recoverrun");
	struct skin* skip13 = getscreennode(adjust, "skip13");
	struct skin* skip46 = getscreennode(adjust, "skip46");
	struct skin* skip79 = getscreennode(adjust, "skip79");
	struct skin* playertype = getscreennode(adjust, "playertype");
	struct skin* nocamsg = getscreennode(adjust, "nocamsg");
	struct skin* autochangechannelname = getscreennode(adjust, "autochangechannelname");
	struct skin* def_rectimer_after = getscreennode(adjust, "def_rectimer_after");
	struct skin* deltimeshift = getscreennode(adjust, "deltimeshift");
	struct skin* showchanneltimeline = getscreennode(adjust, "showchanneltimeline");
	struct skin* screenanim = getscreennode(adjust, "screenanim");
	struct skin* screenanimspeed = getscreennode(adjust, "screenanimspeed");
	struct skin* channellistview = getscreennode(adjust, "channellistview");
	struct skin* camwait = getscreennode(adjust, "camwait");
	struct skin* checkcamdecrypt = getscreennode(adjust, "checkcamdecrypt");
	struct skin* showlastpos = getscreennode(adjust, "showlastpos");
	struct skin* recsync = getscreennode(adjust, "recsync");
	struct skin* recordnamefmt = getscreennode(adjust, "recordnamefmt");
	struct skin* caskipprivat = getscreennode(adjust, "caskipprivat");
	struct skin* casendallcaids = getscreennode(adjust, "casendallcaids");
	struct skin* newsletter = getscreennode(adjust, "newsletter");
	struct skin* showhiddenfiles = getscreennode(adjust, "showhiddenfiles");
	struct skin* expertmodus = getscreennode(adjust, "expertmodus");
	struct skin* infobarprogram = getscreennode(adjust, "infobarprogram");
	struct skin* at7000frontrun = getscreennode(adjust, "at7000frontrun");
	struct skin* at7000frontsleep = getscreennode(adjust, "at7000frontsleep");
	
	struct skin* tmp = NULL;

	changeinput(volbartimeout, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
	setchoiceboxselection(volbartimeout, getconfig("volbartimeout", NULL));

	changeinput(infobartimeout, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
	setchoiceboxselection(infobartimeout, getconfig("infobartimeout", NULL));

	changeinput(infobarsleep, "0\n1\n2\n3\n4\n5");
	setchoiceboxselection(infobarsleep, getconfig("infobarsleep", NULL));
	
	addchoicebox(secondinfobar, "0", _("no"));
	addchoicebox(secondinfobar, "1", _("infobar"));
	addchoicebox(secondinfobar, "2", _("epg"));
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

	changeinput(forerun, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60");
	setchoiceboxselection(forerun, getconfig("recforerun", NULL));

	changeinput(overrun, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60");
	setchoiceboxselection(overrun, getconfig("recoverrun", NULL));

	changeinput(skip13, "15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200");
	setchoiceboxselection(skip13, getconfig("skip13", NULL));

	changeinput(skip46, "15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200");
	setchoiceboxselection(skip46, getconfig("skip46", NULL));

	changeinput(skip79, "15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200");
	setchoiceboxselection(skip79, getconfig("skip79", NULL));
	
	addchoicebox(playertype, "0", _("extern"));
	addchoicebox(playertype, "1", _("intern"));
	setchoiceboxselection(playertype, getconfig("playertype", NULL));
	
	addchoicebox(nocamsg, "0", _("yes"));
	addchoicebox(nocamsg, "1", _("no"));
	setchoiceboxselection(nocamsg, getconfig("nocamsg", NULL));
	
	addchoicebox(autochangechannelname, "0", _("no"));
	addchoicebox(autochangechannelname, "1", _("yes"));
	setchoiceboxselection(autochangechannelname, getconfig("autochangechannelname", NULL));

	addchoicebox(def_rectimer_after, "0", _("auto"));
	addchoicebox(def_rectimer_after, "1", _("nothing"));
	addchoicebox(def_rectimer_after, "2", _("standby"));
	addchoicebox(def_rectimer_after, "3", _("power off"));
	setchoiceboxselection(def_rectimer_after, getconfig("def_rectimer_after", NULL));
	
	addchoicebox(deltimeshift, "0", _("ask"));
	addchoicebox(deltimeshift, "1", _("allways del file"));
	addchoicebox(deltimeshift, "2", _("never del file"));
	setchoiceboxselection(deltimeshift, getconfig("deltimeshift", NULL));
	
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
	addchoicebox(channellistview, "1", _("deactive"));
	setchoiceboxselection(channellistview, getconfig("channellistview", NULL));

	addchoicebox(camwait, "25", _("slow"));
	addchoicebox(camwait, "15", _("middle"));
	addchoicebox(camwait, "5", _("fast"));
	setchoiceboxselection(camwait, getconfig("camwait", NULL));

	addchoicebox(checkcamdecrypt, "0", _("no"));
	addchoicebox(checkcamdecrypt, "1", _("yes"));
	setchoiceboxselection(checkcamdecrypt, getconfig("checkcamdecrypt", NULL));

	addchoicebox(showlastpos, "0", _("no"));	
	addchoicebox(showlastpos, "1", _("yes"));
	setchoiceboxselection(showlastpos, getconfig("showlastpos", NULL));

	addchoicebox(recsync, "0", _("no"));	
	addchoicebox(recsync, "1", _("yes"));
	setchoiceboxselection(recsync, getconfig("recsync", NULL));

	addchoicebox(recordnamefmt, "0", _("channel-movie"));
	addchoicebox(recordnamefmt, "1", _("movie-channel"));
	setchoiceboxselection(recordnamefmt, getconfig("recordnamefmt", NULL));

	addchoicebox(caskipprivat, "0", _("no"));
	addchoicebox(caskipprivat, "1", _("yes"));
	setchoiceboxselection(caskipprivat, getconfig("caskipprivat", NULL));

	addchoicebox(casendallcaids, "0", _("no"));
	addchoicebox(casendallcaids, "1", _("yes"));
	setchoiceboxselection(casendallcaids, getconfig("casendallcaids", NULL));

	addchoicebox(newsletter, "0", _("no"));
	addchoicebox(newsletter, "1", _("yes"));
	setchoiceboxselection(newsletter, getconfig("newsletter", NULL));

	addchoicebox(showhiddenfiles, "0", _("no"));
	addchoicebox(showhiddenfiles, "1", _("yes"));
	setchoiceboxselection(showhiddenfiles, getconfig("showhiddenfiles", NULL));

	addchoicebox(expertmodus, "0", _("no"));
	addchoicebox(expertmodus, "10", _("yes"));
	setchoiceboxselection(expertmodus, getconfig("expertmodus", NULL));

	addchoicebox(infobarprogram, "0", _("no"));
	addchoicebox(infobarprogram, "1", _("yes"));
	setchoiceboxselection(infobarprogram, getconfig("infobarprogram", NULL));
	
	changeinput(at7000frontrun, "15\n14\n13\n12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1");
	setchoiceboxselection(at7000frontrun, getconfig("at7000frontrun", NULL));
	
	changeinput(at7000frontsleep, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15");
	setchoiceboxselection(at7000frontsleep, getconfig("at7000frontsleep", NULL));
	
	if(checkbox("ATEVIO7000") == 1)
	{
		at7000frontrun->hidden = NO;
		at7000frontsleep->hidden = NO;
	}else{
		at7000frontrun->hidden = YES;
		at7000frontsleep->hidden = YES;
	}
	
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
			addconfigscreencheck("recforerun", forerun, "0");
			addconfigscreencheck("recoverrun", overrun, "0");
			addconfigscreencheck("virtualzap", virtualzap, "0");
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
			addconfigscreencheck("nocamsg", nocamsg, "0");
			addconfigscreencheck("autochangechannelname", autochangechannelname, "0");
			addconfigscreencheck("def_rectimer_after", def_rectimer_after, "0");
			addconfigscreencheck("deltimeshift", deltimeshift, "0");
			addconfigscreencheck("showchanneltimeline", showchanneltimeline, "0");
			status.showchanneltimeline = getconfigint("showchanneltimeline", NULL);
			addconfigscreencheck("screenanim", screenanim, "0");
			status.screenanim = getconfigint("screenanim", NULL);
			addconfigscreencheck("screenanimspeed", screenanimspeed, "1");
			status.screenanimspeed = getconfigint("screenanimspeed", NULL);
			addconfigscreencheck("channellistview", channellistview, "0");
			status.channellistview = getconfigint("channellistview", NULL);
			addconfigscreencheck("camwait", camwait, "25");
			addconfigscreencheck("checkcamdecrypt", checkcamdecrypt, "0");
			addconfigscreencheck("showlastpos", showlastpos, "0");
			addconfigscreencheck("recsync", recsync, "0");
			addconfigscreencheck("recordnamefmt", recordnamefmt, "0");
			addconfigscreencheck("caskipprivat", caskipprivat, "0");
			status.caskipprivat = getconfigint("caskipprivat", NULL);
			addconfigscreencheck("casendallcaids", casendallcaids, "0");
			status.casendallcaids = getconfigint("casendallcaids", NULL);

			addconfigscreen("newsletter", newsletter);
			if(newsletter->ret != NULL && ostrcmp(newsletter->ret, "0") == 0)
				startnewsletter(0);
			else
				startnewsletter(1);

			addconfigscreen("showhiddenfiles", showhiddenfiles);
			status.showhiddenfiles = getconfigint("showhiddenfiles", NULL);
			addconfigscreencheck("expertmodus", expertmodus, "0");
			addconfigscreencheck("infobarprogram", infobarprogram, "0");
			status.infobarprogram = getconfigint("infobarprogram", NULL);
			
			if(checkbox("ATEVIO7000") == 1)
			{
				addconfigscreencheck("at7000frontrun", at7000frontrun, "0");
				addconfigscreencheck("at7000frontsleep", at7000frontsleep, "0");
				tmpstr = ostrcat("fp_control -P ",getconfig("at7000frontrun", NULL), 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr=NULL;
			}
			break;
		}
	}

	delownerrc(adjust);
	clearscreen(adjust);
}

#endif
