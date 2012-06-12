#ifndef SKINADJUST_H
#define SKINADJUST_H

void screenskinadjust()
{
	int rcret = 0, oleftoffset = 0, orightoffset = 0, otopoffset = 0, obottomoffset = 0;
	struct skin* skinadjust = getscreen("skinadjust");
	struct skin* listbox = getscreennode(skinadjust, "listbox");
	struct skin* fontsizeadjust = getscreennode(skinadjust, "fontsizeadjust");
	struct skin* listboxselecttype = getscreennode(skinadjust, "listboxselecttype");
	struct skin* osdtransparent = getscreennode(skinadjust, "osdtransparent");
	struct skin* leftoffset = getscreennode(skinadjust, "leftoffset");
	struct skin* rightoffset = getscreennode(skinadjust, "rightoffset");
	struct skin* topoffset = getscreennode(skinadjust, "topoffset");
	struct skin* bottomoffset = getscreennode(skinadjust, "bottomoffset");
	struct skin* piconpath = getscreennode(skinadjust, "piconpath");
	struct skin* tmp = NULL;
	char* ret = NULL;

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

	changeinput(piconpath, getconfig("piconpath", NULL));

	oleftoffset = getconfigint("fbleftoffset", NULL);
	orightoffset = getconfigint("fbrightoffset", NULL); 
	otopoffset = getconfigint("fbtopoffset", NULL);
	obottomoffset = getconfigint("fbbottomoffset", NULL);

	drawscreen(skinadjust, 0, 0);
	addscreenrc(skinadjust, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(skinadjust, tmp);
		rcret = waitrc(skinadjust, 0, 0);
		tmp = listbox->select;

		addconfigscreencheck("fbleftoffset", leftoffset, "0");
		status.leftoffset = getconfigint("fbleftoffset", NULL);
		addconfigscreencheck("fbrightoffset", rightoffset, "0");
		status.rightoffset = getconfigint("fbrightoffset", NULL);
		addconfigscreencheck("fbtopoffset", topoffset, "0");
		status.topoffset = getconfigint("fbtopoffset", NULL);
		addconfigscreencheck("fbbottomoffset", bottomoffset, "0");
		status.bottomoffset = getconfigint("fbbottomoffset", NULL);
		
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
			break;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			addskinconfigscreencheck("fontsizeadjust", fontsizeadjust, "0");
			addskinconfigscreencheck("listboxselecttype", listboxselecttype, "0");
			status.listboxselecttype = getskinconfigint("listboxselecttype", NULL);
			addskinconfigscreencheck("osdtransparent", osdtransparent, "0");
			setosdtransparent(getskinconfigint("osdtransparent", NULL));
			addconfigscreen("piconpath", piconpath);

			if(listbox->select != NULL && ostrcmp(listbox->select->name, "piconpath") == 0)
			{
				clearscreen(skinadjust);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 700, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(skinadjust, 0, 0);
				continue;
			}

			break;
		}
	}

	delownerrc(skinadjust);
	clearscreen(skinadjust);
}

#endif
