#ifndef SKINADJUST_H
#define SKINADJUST_H

void screenskinadjust()
{
	int rcret = 0, oleftoffset = 0, orightoffset = 0, otopoffset = 0, obottomoffset = 0, reboot = 0;
	char* tmpstr = NULL;
	struct skin* skinadjust = getscreen("skinadjust");
	struct skin* listbox = getscreennode(skinadjust, "listbox");
	struct skin* fontsizeadjust = getscreennode(skinadjust, "fontsizeadjust");
	struct skin* listboxselecttype = getscreennode(skinadjust, "listboxselecttype");
	struct skin* osdtransparent = getscreennode(skinadjust, "osdtransparent");
	struct skin* leftoffset = getscreennode(skinadjust, "leftoffset");
	struct skin* rightoffset = getscreennode(skinadjust, "rightoffset");
	struct skin* topoffset = getscreennode(skinadjust, "topoffset");
	struct skin* bottomoffset = getscreennode(skinadjust, "bottomoffset");
	struct skin* showrecfreesize = getscreennode(skinadjust, "showrecfreesize");
	struct skin* listboxselect = getscreennode(skinadjust, "listboxselect");
	struct skin* tithek_selectcol = getscreennode(skinadjust, "tithek_selectcol");
	struct skin* filelistselect = getscreennode(skinadjust, "filelistselect");
	struct skin* buttonbar_bgcol = getscreennode(skinadjust, "buttonbar_bgcol");
	struct skin* buttonbar_bgcol2 = getscreennode(skinadjust, "buttonbar_bgcol2");
	struct skin* buttonbar_bordercol = getscreennode(skinadjust, "buttonbar_bordercol");
	struct skin* buttonbar_fontcol = getscreennode(skinadjust, "buttonbar_fontcol");
	struct skin* titlebar_bgcol = getscreennode(skinadjust, "titlebar_bgcol");
	struct skin* titlebar_bgcol2 = getscreennode(skinadjust, "titlebar_bgcol2");
	struct skin* titlebar_bordercol = getscreennode(skinadjust, "titlebar_bordercol");
	struct skin* titlebar_fontcol = getscreennode(skinadjust, "titlebar_fontcol");
	struct skin* bgcol = getscreennode(skinadjust, "bgcol");
	struct skin* bgcol2 = getscreennode(skinadjust, "bgcol2");
	struct skin* fontcol = getscreennode(skinadjust, "fontcol");
	struct skin* bordercol = getscreennode(skinadjust, "bordercol");
	struct skin* progresscol = getscreennode(skinadjust, "progresscol");
	struct skin* titlebgcol = getscreennode(skinadjust, "titlebgcol");
	struct skin* markcol = getscreennode(skinadjust, "markcol");
	struct skin* okcol = getscreennode(skinadjust, "okcol");
	struct skin* exitcol = getscreennode(skinadjust, "exitcol");
	struct skin* deaktivcol = getscreennode(skinadjust, "deaktivcol");
	struct skin* cpmode_bgcol = getscreennode(skinadjust, "cpmode_bgcol");
	struct skin* cpmode_fontcol = getscreennode(skinadjust, "cpmode_fontcol");
	struct skin* editmode_bgcol = getscreennode(skinadjust, "editmode_bgcol");
	struct skin* editmode_fontcol = getscreennode(skinadjust, "editmode_fontcol");
	struct skin* mvmode_bgcol = getscreennode(skinadjust, "mvmode_bgcol");
	struct skin* mvmode_fontcol = getscreennode(skinadjust, "mvmode_fontcol");
	struct skin* protectmode_bgcol = getscreennode(skinadjust, "protectmode_bgcol");
	struct skin* protectmode_fontcol = getscreennode(skinadjust, "protectmode_fontcol");
	struct skin* rec_progress = getscreennode(skinadjust, "rec_progress");
	struct skin* epgcol1 = getscreennode(skinadjust, "epgcol1");
	struct skin* epgcol2 = getscreennode(skinadjust, "epgcol2");
	struct skin* epgcol3 = getscreennode(skinadjust, "epgcol3");
	struct skin* emuaktivecol = getscreennode(skinadjust, "emuaktivecol");
	struct skin* emurunningcol = getscreennode(skinadjust, "emurunningcol");

	struct skin* oled_sel = getscreennode(skinadjust, "oled_sel");
	struct skin* infobar_sel = getscreennode(skinadjust, "infobar_sel");
	struct skin* infobar2_sel = getscreennode(skinadjust, "infobar2_sel");
//	struct skin* pic1 = getscreennode(skinadjust, "pic1");
//	struct skin* pic2 = getscreennode(skinadjust, "pic2");
	struct skin* b5 = getscreennode(skinadjust, "b5");

	struct skin* tmp = NULL;

	changeinput(fontsizeadjust, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n-15\n-14\n-13\n-12\n-11\n-10\n-9\n-8\n-7\n-6\n-5\n-4\n-3\n-2\n-1");
	setchoiceboxselection(fontsizeadjust, getskinconfig("fontsizeadjust", NULL));

	addchoicebox(listboxselecttype, "0", _("border"));
	addchoicebox(listboxselecttype, "1", _("bar"));
	addchoicebox(listboxselecttype, "2", _("text"));
	addchoicebox(listboxselecttype, "3", _("picture"));
	setchoiceboxselection(listboxselecttype, getskinconfig("listboxselecttype", NULL));

	changeinput(osdtransparent, "0\n5\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60\n65\n70");
	setchoiceboxselection(osdtransparent, getskinconfig("osdtransparent", NULL));

#ifdef MIPSEL
	changeinput(leftoffset, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20");
	setchoiceboxselection(leftoffset, getconfig("fbleftoffset", NULL));

	changeinput(rightoffset, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20");
	setchoiceboxselection(rightoffset, getconfig("fbrightoffset", NULL));

	changeinput(topoffset, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20");
	setchoiceboxselection(topoffset, getconfig("fbtopoffset", NULL));

	changeinput(bottomoffset, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20");
	setchoiceboxselection(bottomoffset, getconfig("fbbottomoffset", NULL));
#else
	changeinput(leftoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(leftoffset, getconfig("fbleftoffset", NULL));

	changeinput(rightoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(rightoffset, getconfig("fbrightoffset", NULL));

	changeinput(topoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(topoffset, getconfig("fbtopoffset", NULL));

	changeinput(bottomoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(bottomoffset, getconfig("fbbottomoffset", NULL));
#endif
	oleftoffset = getconfigint("fbleftoffset", NULL);
	orightoffset = getconfigint("fbrightoffset", NULL); 
	otopoffset = getconfigint("fbtopoffset", NULL);
	obottomoffset = getconfigint("fbbottomoffset", NULL);

	if(checkchipset("BCM7424") == 1) // inihdp
	{
		if(checkscreen("OLED_nemesis") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis", "v1");
		if(checkscreen("OLED_nemesis_v2") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis_v2","v2");
		if(checkscreen("OLED_nemesis_v3") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis_v3","v3");
		if(checkscreen("OLED_nemesis_v4") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis_v4","v4");
		if(checkscreen("OLED_nemesis_v5") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis_v5","v5");
		if(checkscreen("OLED_nemesis_v6") != status.skinerr)
			addchoicebox(oled_sel, "OLED_nemesis_v6","v6");

		setchoiceboxselection(oled_sel, getskinconfig("OLED_nemesis", NULL));
	}
	else
		oled_sel->hidden = YES;

	if(checkscreen("infobar") != status.skinerr)
		addchoicebox(infobar_sel, "infobar", "v1");
	if(checkscreen("infobar_v2") != status.skinerr)
		addchoicebox(infobar_sel, "infobar_v2","v2");
	if(checkscreen("infobar_v3") != status.skinerr)
		addchoicebox(infobar_sel, "infobar_v3","v3");
	setchoiceboxselection(infobar_sel, getskinconfig("infobar_selection", NULL));

	if(checkscreen("infobar2") != status.skinerr)
		addchoicebox(infobar2_sel, "infobar2", "v1");
	if(checkscreen("infobar2_v2") != status.skinerr)
		addchoicebox(infobar2_sel, "infobar2_v2","v2");
	if(checkscreen("infobar2_v3") != status.skinerr)
		addchoicebox(infobar2_sel, "infobar2_v3","v3");
	setchoiceboxselection(infobar2_sel, getskinconfig("infobar2_selection", NULL));

	addchoicebox(showrecfreesize, "0", _("no"));
	addchoicebox(showrecfreesize, "1", _("yes (Text in %)"));
	addchoicebox(showrecfreesize, "2", _("yes (Text in MB)"));
	setchoiceboxselection(showrecfreesize, getconfig("showrecfreesize", NULL));

	addchoicebox(listboxselect, "0", getskinconfig("markcol", NULL));
	addchoicebox(tithek_selectcol, "1", getskinconfig("tithek_selectcol", NULL));
	addchoicebox(filelistselect, "2", getskinconfig("filelistselect", NULL));
	addchoicebox(buttonbar_bgcol, "3", getskinconfig("buttonbar_bgcol", NULL));
	addchoicebox(buttonbar_bgcol2, "4", getskinconfig("buttonbar_bgcol2", NULL));
	addchoicebox(buttonbar_bordercol, "5", getskinconfig("buttonbar_bordercol", NULL));
	addchoicebox(buttonbar_fontcol, "6", getskinconfig("buttonbar_fontcol", NULL));
	addchoicebox(titlebar_bgcol, "7", getskinconfig("titlebar_bgcol", NULL));
	addchoicebox(titlebar_bgcol2, "8", getskinconfig("titlebar_bgcol2", NULL));
	addchoicebox(titlebar_bordercol, "9", getskinconfig("titlebar_bordercol", NULL));
	addchoicebox(titlebar_fontcol, "10", getskinconfig("titlebar_fontcol", NULL));
	addchoicebox(okcol, "11", getskinconfig("okcol", NULL));
	addchoicebox(exitcol, "12", getskinconfig("exitcol", NULL));
	addchoicebox(deaktivcol, "13", getskinconfig("deaktivcol", NULL));
	addchoicebox(bgcol, "14", getskinconfig("bgcol", NULL));
	addchoicebox(bgcol2, "15", getskinconfig("bgcol2", NULL));
	addchoicebox(fontcol, "16", getskinconfig("fontcol", NULL));
	addchoicebox(bordercol, "17", getskinconfig("bordercol", NULL));
	addchoicebox(progresscol, "18", getskinconfig("progresscol", NULL));
	addchoicebox(titlebgcol, "19", getskinconfig("titlebgcol", NULL));
	addchoicebox(markcol, "20", getskinconfig("markcol", NULL));
	addchoicebox(cpmode_bgcol, "21", getskinconfig("cpmode_bgcol", NULL));
	addchoicebox(cpmode_fontcol, "22", getskinconfig("cpmode_fontcol", NULL));
	addchoicebox(editmode_bgcol, "23", getskinconfig("editmode_bgcol", NULL));
	addchoicebox(editmode_fontcol, "24", getskinconfig("editmode_fontcol", NULL));
	addchoicebox(mvmode_fontcol, "25", getskinconfig("mvmode_fontcol", NULL));
	addchoicebox(protectmode_bgcol, "26", getskinconfig("protectmode_bgcol", NULL));
	addchoicebox(protectmode_fontcol, "27", getskinconfig("protectmode_fontcol", NULL));
	addchoicebox(rec_progress, "28", getskinconfig("rec_progress", NULL));
	addchoicebox(epgcol1, "29", getskinconfig("epgcol1", NULL));
	addchoicebox(epgcol2, "30", getskinconfig("epgcol2", NULL));
	addchoicebox(epgcol3, "31", getskinconfig("epgcol3", NULL));
	addchoicebox(emuaktivecol, "32", getskinconfig("emuaktivecol", NULL));
	addchoicebox(emurunningcol, "33", getskinconfig("emurunningcol", NULL));

	listboxselect->fontcol2 = convertcol(getskinconfig("listboxselect", NULL));
	tithek_selectcol->fontcol2 = convertcol(getskinconfig("tithek_selectcol", NULL));
	filelistselect->fontcol2 = convertcol(getskinconfig("filelistselect", NULL));
	buttonbar_bgcol->fontcol2 = convertcol(getskinconfig("buttonbar_bgcol", NULL));
	buttonbar_bgcol2->fontcol2 = convertcol(getskinconfig("buttonbar_bgcol2", NULL));
	buttonbar_bordercol->fontcol2 = convertcol(getskinconfig("buttonbar_bordercol", NULL));
	buttonbar_fontcol->fontcol2 = convertcol(getskinconfig("buttonbar_fontcol", NULL));
	titlebar_bgcol->fontcol2 = convertcol(getskinconfig("titlebar_bgcol", NULL));
	titlebar_bgcol2->fontcol2 = convertcol(getskinconfig("titlebar_bgcol2", NULL));
	titlebar_bordercol->fontcol2 = convertcol(getskinconfig("titlebar_bordercol", NULL));
	titlebar_fontcol->fontcol2 = convertcol(getskinconfig("titlebar_fontcol", NULL));
	okcol->fontcol2 = convertcol(getskinconfig("okcol", NULL));
	exitcol->fontcol2 = convertcol(getskinconfig("exitcol", NULL));
	deaktivcol->fontcol2 = convertcol(getskinconfig("deaktivcol", NULL));
	bgcol->fontcol2 = convertcol(getskinconfig("bgcol", NULL));
	bgcol2->fontcol2 = convertcol(getskinconfig("bgcol2", NULL));
	fontcol->fontcol2 = convertcol(getskinconfig("fontcol", NULL));
	bordercol->fontcol2 = convertcol(getskinconfig("bordercol", NULL));
	progresscol->fontcol2 = convertcol(getskinconfig("progresscol", NULL));
	titlebgcol->fontcol2 = convertcol(getskinconfig("titlebgcol", NULL));
	markcol->fontcol2 = convertcol(getskinconfig("markcol", NULL));
	rec_progress->fontcol2 = convertcol(getskinconfig("rec_progress", NULL));
	cpmode_bgcol->fontcol2 = convertcol(getskinconfig("cpmode_bgcol", NULL));
	cpmode_fontcol->fontcol2 = convertcol(getskinconfig("cpmode_fontcol", NULL));
	editmode_bgcol->fontcol2 = convertcol(getskinconfig("editmode_bgcol", NULL));
	editmode_fontcol->fontcol2 = convertcol(getskinconfig("editmode_fontcol", NULL));
	mvmode_bgcol->fontcol2 = convertcol(getskinconfig("mvmode_bgcol", NULL));
	mvmode_fontcol->fontcol2 = convertcol(getskinconfig("mvmode_fontcol", NULL));
	protectmode_bgcol->fontcol2 = convertcol(getskinconfig("protectmode_bgcol", NULL));
	protectmode_fontcol->fontcol2 = convertcol(getskinconfig("protectmode_fontcol", NULL));
	epgcol1->fontcol2 = convertcol(getskinconfig("epgcol1", NULL));
	epgcol2->fontcol2 = convertcol(getskinconfig("epgcol2", NULL));
	epgcol3->fontcol2 = convertcol(getskinconfig("epgcol3", NULL));
	emuaktivecol->fontcol2 = convertcol(getskinconfig("emuaktivecol", NULL));
	emurunningcol->fontcol2 = convertcol(getskinconfig("emurunningcol", NULL));

	if(status.security == 0 || checkemu() == 0)
	{
		emuaktivecol->hidden = YES;
		emurunningcol->hidden = YES;
	}
	else
	{
		emuaktivecol->hidden = NO;
		emurunningcol->hidden = NO;
	}
					
	b5->hidden = YES;

	drawscreen(skinadjust, 0, 0);
	addscreenrc(skinadjust, listbox);

	tmp = listbox->select;
	while(1)
	{
		int offsetchange = 0;
		addscreenrc(skinadjust, tmp);
		rcret = waitrc(skinadjust, 0, 0);
		tmp = listbox->select;

		addconfigscreencheck("fbleftoffset", leftoffset, "0");
		if(status.leftoffset != getconfigint("fbleftoffset", NULL)) offsetchange = 1; 
		status.leftoffset = getconfigint("fbleftoffset", NULL);

		addconfigscreencheck("fbrightoffset", rightoffset, "0");
		if(status.rightoffset != getconfigint("fbrightoffset", NULL)) offsetchange = 1;
		status.rightoffset = getconfigint("fbrightoffset", NULL);

		addconfigscreencheck("fbtopoffset", topoffset, "0");
		if(status.topoffset != getconfigint("fbtopoffset", NULL)) offsetchange = 1;
		status.topoffset = getconfigint("fbtopoffset", NULL);

		addconfigscreencheck("fbbottomoffset", bottomoffset, "0");
		if(status.bottomoffset != getconfigint("fbbottomoffset", NULL)) offsetchange = 1;
		status.bottomoffset = getconfigint("fbbottomoffset", NULL);

		if(offsetchange == 1 && (ostrcmp(getconfig("av_mode3d", NULL), "sbs") == 0 || ostrcmp(getconfig("av_mode3d", NULL), "tab") == 0)) clearfball();

		drawscreen(skinadjust, 0, 0);

#ifdef MIPSEL
		setfbosd();
#endif

		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && checkchipset("BCM7424") == 1 && listbox->select != NULL && ostrcmp(listbox->select->name, "oled_sel") == 0)
		{
			tmpstr = ostrcat(tmpstr, oled_sel->ret, 0, 0);
			struct skin* OLED_nemesis = getscreen(tmpstr);
			if(status.skinerr == OLED_nemesis)
				OLED_nemesis = getscreen("OLED_nemesis");
			struct skin* textbox = getscreennode(OLED_nemesis, "textbox");
			changetext(textbox, tmpstr);
			drawscreen(OLED_nemesis, 0, 0);
			free(tmpstr);tmpstr=NULL;
		}

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			addconfigint("fbleftoffset", oleftoffset);
			status.leftoffset = getconfigint("fbleftoffset", NULL);
			addconfigint("fbrightoffset", orightoffset);
			status.rightoffset = getconfigint("fbrightoffset", NULL);
			addconfigint("fbtopoffset", otopoffset);
			status.topoffset = getconfigint("fbtopoffset", NULL);
			addconfigint("fbbottomoffset", obottomoffset);
			status.bottomoffset = getconfigint("fbbottomoffset", NULL);
			clearfball();
			break;
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			char* cmd = NULL;
			if(ostrcmp(getconfig("skinconfig", NULL), "/var/usr/local/share/titan/skin/default/skinconfig") == 0)				
				cmd = ostrcat("cp -a /etc/titan.restore/mnt/config/skinconfig ", getconfig("skinconfig", NULL), 0, 0);
			else
			{
				cmd = ostrcat(cmd, "cp -a ", 1, 0);
				cmd = ostrcat(cmd, getconfig("skinpath", NULL), 1, 0);
				cmd = ostrcat(cmd, "/skinconfig.default ", 1, 0);
				cmd = ostrcat(cmd, getconfig("skinconfig", NULL), 1, 0);
			}
			printf("cmd: %s\n", cmd);	
			system(cmd);
			free(cmd); cmd = NULL;

			textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			oshutdown(3, 0);

			break;
		}

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "listboxselect") == 0)
			{
				long oldlistboxselectcol = convertcol("listboxselect");
				tmpstr = screencolorpicker(getskinconfig("listboxselect", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("listboxselect", tmpstr);
				if(oldlistboxselectcol != convertcol("listboxselect")) reboot = 1;
				listboxselect->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "tithek_selectcol") == 0)
			{
				long oldtithek_selectcol = convertcol("tithek_selectcol");
				tmpstr = screencolorpicker(getskinconfig("tithek_selectcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("tithek_selectcol", tmpstr);
				if(oldtithek_selectcol != convertcol("tithek_selectcol")) reboot = 1;
				tithek_selectcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "filelistselect") == 0)
			{
				long oldfilelistselect = convertcol("filelistselect");
				tmpstr = screencolorpicker(getskinconfig("filelistselect", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("filelistselect", tmpstr);
				if(oldfilelistselect != convertcol("filelistselect")) reboot = 1;
				filelistselect->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "buttonbar_bgcol") == 0)
			{
				long oldbuttonbar_bgcol = convertcol("buttonbar_bgcol");
				tmpstr = screencolorpicker(getskinconfig("buttonbar_bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("buttonbar_bgcol", tmpstr);
				if(oldbuttonbar_bgcol != convertcol("buttonbar_bgcol")) reboot = 1;
				buttonbar_bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "buttonbar_bgcol2") == 0)
			{
				long oldbuttonbar_bgcol2 = convertcol("buttonbar_bgcol2");
				tmpstr = screencolorpicker(getskinconfig("buttonbar_bgcol2", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("buttonbar_bgcol2", tmpstr);
				if(oldbuttonbar_bgcol2 != convertcol("buttonbar_bgcol2")) reboot = 1;
				buttonbar_bgcol2->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "buttonbar_bordercol") == 0)
			{
				long oldbuttonbar_bordercol = convertcol("buttonbar_bordercol");
				tmpstr = screencolorpicker(getskinconfig("buttonbar_bordercol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("buttonbar_bordercol", tmpstr);
				if(oldbuttonbar_bordercol != convertcol("buttonbar_bordercol")) reboot = 1;
				buttonbar_bordercol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "buttonbar_fontcol") == 0)
			{
				long oldbuttonbar_fontcol = convertcol("buttonbar_fontcol");
				tmpstr = screencolorpicker(getskinconfig("buttonbar_fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("buttonbar_fontcol", tmpstr);
				if(oldbuttonbar_fontcol != convertcol("buttonbar_fontcol")) reboot = 1;
				buttonbar_fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "titlebar_bgcol") == 0)
			{
				long oldtitlebar_bgcol = convertcol("titlebar_bgcol");
				tmpstr = screencolorpicker(getskinconfig("titlebar_bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("titlebar_bgcol", tmpstr);
				if(oldtitlebar_bgcol != convertcol("titlebar_bgcol")) reboot = 1;
				titlebar_bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "titlebar_bgcol2") == 0)
			{
				long oldtitlebar_bgcol2 = convertcol("titlebar_bgcol2");
				tmpstr = screencolorpicker(getskinconfig("titlebar_bgcol2", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("titlebar_bgcol2", tmpstr);
				if(oldtitlebar_bgcol2 != convertcol("titlebar_bgcol2")) reboot = 1;
				titlebar_bgcol2->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "titlebar_bordercol") == 0)
			{
				long oldtitlebar_bordercol = convertcol("titlebar_bordercol");
				tmpstr = screencolorpicker(getskinconfig("titlebar_bordercol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("titlebar_bordercol", tmpstr);
				if(oldtitlebar_bordercol != convertcol("titlebar_bordercol")) reboot = 1;
				titlebar_bordercol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "titlebar_fontcol") == 0)
			{
				long oldtitlebar_fontcol = convertcol("titlebar_fontcol");
				tmpstr = screencolorpicker(getskinconfig("titlebar_fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("titlebar_fontcol", tmpstr);
				if(oldtitlebar_fontcol != convertcol("titlebar_fontcol")) reboot = 1;
				titlebar_fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "okcol") == 0)
			{
				long oldokcol = convertcol("okcol");
				tmpstr = screencolorpicker(getskinconfig("okcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("okcol", tmpstr);
				if(oldokcol != convertcol("okcol")) reboot = 1;
				okcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "exitcol") == 0)
			{
				long oldexitcol = convertcol("exitcol");
				tmpstr = screencolorpicker(getskinconfig("exitcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("exitcol", tmpstr);
				if(oldexitcol != convertcol("exitcol")) reboot = 1;
				okcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "deaktivcol") == 0)
			{
				long olddeaktivcol = convertcol("deaktivcol");
				tmpstr = screencolorpicker(getskinconfig("deaktivcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("deaktivcol", tmpstr);
				if(olddeaktivcol != convertcol("deaktivcol")) reboot = 1;
				deaktivcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "bgcol") == 0)
			{
				long oldbgcol = convertcol("bgcol");
				tmpstr = screencolorpicker(getskinconfig("bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("bgcol", tmpstr);
				if(oldbgcol != convertcol("bgcol")) reboot = 1;
				bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "bgcol2") == 0)
			{
				long oldbgcol2 = convertcol("bgcol2");
				tmpstr = screencolorpicker(getskinconfig("bgcol2", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("bgcol2", tmpstr);
				if(oldbgcol2 != convertcol("bgcol2")) reboot = 1;
				bgcol2->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "fontcol") == 0)
			{
				long oldfontcol = convertcol("fontcol");
				tmpstr = screencolorpicker(getskinconfig("fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("fontcol", tmpstr);
				if(oldfontcol != convertcol("fontcol")) reboot = 1;
				fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "bordercol") == 0)
			{
				long oldbordercol = convertcol("bordercol");
				tmpstr = screencolorpicker(getskinconfig("bordercol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("bordercol", tmpstr);
				if(oldbordercol != convertcol("bordercol")) reboot = 1;
				bordercol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "progresscol") == 0)
			{
				long oldprogresscol = convertcol("progresscol");
				tmpstr = screencolorpicker(getskinconfig("progresscol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("progresscol", tmpstr);
				if(oldprogresscol != convertcol("progresscol")) reboot = 1;
				progresscol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "titlebgcol") == 0)
			{
				long oldtitlebgcol = convertcol("titlebgcol");
				tmpstr = screencolorpicker(getskinconfig("titlebgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("titlebgcol", tmpstr);
				if(oldtitlebgcol != convertcol("titlebgcol")) reboot = 1;
				titlebgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "markcol") == 0)
			{
				long oldmarkcol = convertcol("markcol");
				tmpstr = screencolorpicker(getskinconfig("markcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("markcol", tmpstr);
				if(oldmarkcol != convertcol("markcol")) reboot = 1;
				markcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "cpmode_bgcol") == 0)
			{
				long oldcpmode_bgcol = convertcol("cpmode_bgcol");
				tmpstr = screencolorpicker(getskinconfig("cpmode_bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("cpmode_bgcol", tmpstr);
				if(oldcpmode_bgcol != convertcol("cpmode_bgcol")) reboot = 1;
				cpmode_bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "cpmode_fontcol") == 0)
			{
				long oldcpmode_fontcol = convertcol("cpmode_fontcol");
				tmpstr = screencolorpicker(getskinconfig("cpmode_fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("cpmode_fontcol", tmpstr);
				if(oldcpmode_fontcol != convertcol("cpmode_fontcol")) reboot = 1;
				cpmode_fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "editmode_bgcol") == 0)
			{
				long oldeditmode_bgcol = convertcol("editmode_bgcol");
				tmpstr = screencolorpicker(getskinconfig("editmode_bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("editmode_bgcol", tmpstr);
				if(oldeditmode_bgcol != convertcol("editmode_bgcol")) reboot = 1;
				editmode_bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "editmode_fontcol") == 0)
			{
				long oldeditmode_fontcol = convertcol("editmode_fontcol");
				tmpstr = screencolorpicker(getskinconfig("editmode_fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("editmode_fontcol", tmpstr);
				if(oldeditmode_fontcol != convertcol("editmode_fontcol")) reboot = 1;
				editmode_fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "mvmode_bgcol") == 0)
			{
				long oldmvmode_bgcol = convertcol("mvmode_bgcol");
				tmpstr = screencolorpicker(getskinconfig("mvmode_bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("mvmode_bgcol", tmpstr);
				if(oldmvmode_bgcol != convertcol("mvmode_bgcol")) reboot = 1;
				mvmode_bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "mvmode_fontcol") == 0)
			{
				long oldmvmode_fontcol = convertcol("mvmode_fontcol");
				tmpstr = screencolorpicker(getskinconfig("mvmode_fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("mvmode_fontcol", tmpstr);
				if(oldmvmode_fontcol != convertcol("mvmode_fontcol")) reboot = 1;
				mvmode_fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "protectmode_bgcol") == 0)
			{
				long oldprotectmode_bgcol = convertcol("protectmode_bgcol");
				tmpstr = screencolorpicker(getskinconfig("protectmode_bgcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("protectmode_bgcol", tmpstr);
				if(oldprotectmode_bgcol != convertcol("protectmode_bgcol")) reboot = 1;
				protectmode_bgcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "protectmode_fontcol") == 0)
			{
				long oldprotectmode_fontcol = convertcol("protectmode_fontcol");
				tmpstr = screencolorpicker(getskinconfig("protectmode_fontcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("protectmode_fontcol", tmpstr);
				if(oldprotectmode_fontcol != convertcol("protectmode_fontcol")) reboot = 1;
				protectmode_fontcol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "rec_progress") == 0)
			{
				long oldrec_progress = convertcol("rec_progress");
				tmpstr = screencolorpicker(getskinconfig("rec_progress", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("rec_progress", tmpstr);
				if(oldrec_progress != convertcol("rec_progress")) reboot = 1;
				rec_progress->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "epgcol1") == 0)
			{
				long oldepgcol1 = convertcol("epgcol1");
				tmpstr = screencolorpicker(getskinconfig("epgcol1", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("epgcol1", tmpstr);
				if(oldepgcol1 != convertcol("epgcol1")) reboot = 1;
				epgcol1->fontcol2 = convertcol(tmpstr);
			}						

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "epgcol2") == 0)
			{
				long oldepgcol2 = convertcol("epgcol2");
				tmpstr = screencolorpicker(getskinconfig("epgcol2", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("epgcol2", tmpstr);
				if(oldepgcol2 != convertcol("epgcol2")) reboot = 1;
				epgcol2->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "epgcol3") == 0)
			{
				long oldepgcol3 = convertcol("epgcol3");
				tmpstr = screencolorpicker(getskinconfig("epgcol3", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("epgcol3", tmpstr);
				if(oldepgcol3 != convertcol("epgcol3")) reboot = 1;
				epgcol3->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "emuaktivecol") == 0)
			{
				long oldemuaktivecol = convertcol("emuaktivecol");
				tmpstr = screencolorpicker(getskinconfig("emuaktivecol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("emuaktivecol", tmpstr);
				if(oldemuaktivecol != convertcol("emuaktivecol")) reboot = 1;
				emuaktivecol->fontcol2 = convertcol(tmpstr);
			}

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "emurunningcol") == 0)
			{
				long oldemurunningcol = convertcol("emurunningcol");
				tmpstr = screencolorpicker(getskinconfig("emurunningcol", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("emurunningcol", tmpstr);
				if(oldemurunningcol != convertcol("emurunningcol")) reboot = 1;
				emurunningcol->fontcol2 = convertcol(tmpstr);
			}

			drawscreen(skinadjust, 0, 0);

			debug(10, "%s set %s", listbox->select->name, tmpstr);
			free(tmpstr), tmpstr = NULL;
			continue;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			int oldfontsizeadjust = getskinconfigint("fontsizeadjust", NULL);
			addskinconfigscreencheck("fontsizeadjust", fontsizeadjust, "0");
			if(oldfontsizeadjust != getskinconfigint("fontsizeadjust", NULL)) reboot = 1;
			status.fontsizeadjust = getskinconfigint("fontsizeadjust", NULL);

			addskinconfigscreencheck("listboxselecttype", listboxselecttype, "0");
			status.listboxselecttype = getskinconfigint("listboxselecttype", NULL);

			addskinconfigscreencheck("osdtransparent", osdtransparent, "0");
			setosdtransparent(getskinconfigint("osdtransparent", NULL));
			addconfigscreencheck("showrecfreesize", showrecfreesize, "0");
			status.showrecfreesize = getconfigint("showrecfreesize", NULL);

			if(checkchipset("BCM7424") == 1) //inihdp
				addskinconfigscreencheck("OLED_nemesis", oled_sel, "0");

			char* oldinfobar_sel = getskinconfig("infobar_selection", NULL);
			addskinconfigscreencheck("infobar_selection", infobar_sel, "0");
			if(ostrcmp(oldinfobar_sel,getskinconfig("infobar_selection", NULL)) != 0) reboot = 1;
			//free(oldinfobar_sel); oldinfobar_sel=NULL;

			char* oldinfobar2_sel = getskinconfig("infobar2_selection", NULL);
			addskinconfigscreencheck("infobar2_selection", infobar2_sel, "0");
			if(ostrcmp(oldinfobar2_sel,getskinconfig("infobar2_selection", NULL)) != 0) reboot = 1;
			//free(oldinfobar2_sel); oldinfobar2_sel=NULL;

			writeskinconfigtmp();
			if(reboot == 1)
			{
				textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				oshutdown(3, 0);
			}

			break;
		}

		if(ostrcmp(listbox->select->name, "listboxselect") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "tithek_selectcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "filelistselect") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "buttonbar_bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "buttonbar_bgcol2") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "buttonbar_bordercol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "buttonbar_fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "titlebar_bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "titlebar_bgcol2") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "titlebar_bordercol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "titlebar_fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "okcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "exitcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "deaktivcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "bgcol2") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "bordercol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "titlebgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "progresscol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "markcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "cpmode_bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "cpmode_fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "editmode_bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "editmode_fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "mvmode_bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "mvmode_fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "protectmode_bgcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "protectmode_fontcol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "rec_progress") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "epgcol1") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "epgcol2") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "epgcol3") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "emuaktivecol") == 0)
			b5->hidden = NO;
		else if(ostrcmp(listbox->select->name, "emurunningcol") == 0)
			b5->hidden = NO;
		else
			b5->hidden = YES;

		drawscreen(skinadjust, 0, 0);
	}

	delskinconfigtmpall();
	delownerrc(skinadjust);
	clearscreen(skinadjust);
}

#endif
