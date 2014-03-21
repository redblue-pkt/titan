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

	changeinput(leftoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(leftoffset, getconfig("fbleftoffset", NULL));

	changeinput(rightoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(rightoffset, getconfig("fbrightoffset", NULL));

	changeinput(topoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(topoffset, getconfig("fbtopoffset", NULL));

	changeinput(bottomoffset, "0\n10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
	setchoiceboxselection(bottomoffset, getconfig("fbbottomoffset", NULL));

	oleftoffset = getconfigint("fbleftoffset", NULL);
	orightoffset = getconfigint("fbrightoffset", NULL); 
	otopoffset = getconfigint("fbtopoffset", NULL);
	obottomoffset = getconfigint("fbbottomoffset", NULL);

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

			writeskinconfigtmp();
			if(reboot == 1)
			{
				textbox(_("Message"), _("Receiver now reboot !!!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
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
