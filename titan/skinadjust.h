#ifndef SKINADJUST_H
#define SKINADJUST_H

void screenskinadjust()
{
	int rcret = 0, oleftoffset = 0, orightoffset = 0, otopoffset = 0, obottomoffset = 0, reboot = 0;
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
	struct skin* oled_sel = getscreennode(skinadjust, "oled_sel");
	struct skin* pic1 = getscreennode(skinadjust, "pic1");
	struct skin* pic2 = getscreennode(skinadjust, "pic2");
	
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

// why this ? and atemio6200 ?
	if(checkbox("ATEMIO5200") == 3)
	{
		leftoffset->hidden = YES;
		rightoffset->hidden = YES;
		topoffset->hidden = YES;
		bottomoffset->hidden = YES;
		pic1->hidden = YES;
		pic2->hidden = YES;
	}
	if(checkbox("ATEMIO-NEMESIS"))
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

	addchoicebox(showrecfreesize, "0", _("no"));
	addchoicebox(showrecfreesize, "1", _("yes (Text in %)"));
	addchoicebox(showrecfreesize, "2", _("yes (Text in MB)"));
	setchoiceboxselection(showrecfreesize, getconfig("showrecfreesize", NULL));
	
	addchoicebox(listboxselect, "0", _("press red"));

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
		
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && checkbox("ATEMIO-NEMESIS") == 1 && listbox->select != NULL && ostrcmp(listbox->select->name, "oled_sel") == 0)
		{
			char* tmpstr = NULL;
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
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "listboxselect") == 0)
			{
				long oldlistboxselectcol = convertcol("listboxselect");
				char* tmpstr = screencolorpicker(getskinconfig("listboxselect", NULL), 0, 0, 0);
				if(tmpstr != NULL)
					addskinconfigtmp("listboxselect", tmpstr);
				if(oldlistboxselectcol != convertcol("listboxselect")) reboot = 1;
				drawscreen(skinadjust, 0, 0);
			}
			
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

			if(checkbox("ATEMIO-NEMESIS"))
				addskinconfigscreencheck("OLED_nemesis", oled_sel, "0");
			
			writeskinconfigtmp();
			if(reboot == 1)
			{
				textbox(_("Message"), _("Titan will be restartet !"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				oshutdown(3, 0);
			}
      
			break;
		}
	}

	delskinconfigtmpall();
	delownerrc(skinadjust);
	clearscreen(skinadjust);
}

#endif
