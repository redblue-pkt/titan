#ifndef AVSETTINGS_H
#define AVSETTINGS_H

//flag 0: without vfd output
//flag 1: with vfd output
void screenavsettings(int flag)
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* avsettings = getscreen("avsettings");
	struct skin* listbox = getscreennode(avsettings, "listbox");
	struct skin* resolution = getscreennode(avsettings, "resolution");
	struct skin* aktresolution = getscreennode(avsettings, "aktresolution");
	struct skin* autoresolution = getscreennode(avsettings, "autoresolution");
	struct skin* autoressd = getscreennode(avsettings, "autoressd");
	struct skin* autorests = getscreennode(avsettings, "autorests");
	struct skin* stepmute = getscreennode(avsettings, "stepmute");
	struct skin* policy = getscreennode(avsettings, "policy");
	struct skin* aspect = getscreennode(avsettings, "aspect");
	struct skin* colformat = getscreennode(avsettings, "colformat");
	struct skin* colformatscart = getscreennode(avsettings, "colformatscart");
	struct skin* audiosource = getscreennode(avsettings, "audiosource");
	struct skin* ac3default = getscreennode(avsettings, "ac3default");
	struct skin* ac3mode = getscreennode(avsettings, "ac3mode");
	struct skin* aacmode = getscreennode(avsettings, "aacmode");
	struct skin* ac3plusmode = getscreennode(avsettings, "ac3plusmode");
	struct skin* dtshdmode = getscreennode(avsettings, "dtshdmode");
	struct skin* wmapromode = getscreennode(avsettings, "wmapromode");
	struct skin* wssmode = getscreennode(avsettings, "wssmode");
	struct skin* volautochangevalue = getscreennode(avsettings, "volautochangevalue");
	struct skin* mode3d = getscreennode(avsettings, "mode3d");
	struct skin* autosubtitle = getscreennode(avsettings, "autosubtitle");
	struct skin* audiodelaybitstream = getscreennode(avsettings, "audiodelaybitstream");
	struct skin* audiodelaypcm = getscreennode(avsettings, "audiodelaypcm");
	struct skin* playerstart = getscreennode(avsettings, "playerstart");
	struct skin* playerstop = getscreennode(avsettings, "playerstop");

	struct skin* tmp = NULL;

	ret = getvideomodechoices();
	changeinput(resolution, ret);
	free(ret); ret = NULL;

	ret = getconfig("av_videomode", NULL);
	if(ret == NULL) 
	{
		ret = getvideomode();
		setchoiceboxselection(resolution, ret);
		free(ret); ret = NULL;
	}
	else
		setchoiceboxselection(resolution, ret);
		
	ret = getvideomode();
	changeinput(aktresolution, ret);
	free(ret); ret = NULL;
	
	autoressd->hidden=YES;
	autorests->hidden=YES;
	if(getconfig("av_videomode_autores", NULL) != NULL)
	{ 
		if(ostrcmp(getconfig("av_videomode_autores", NULL), "off") != 0)
		{
			if(ostrcmp(getconfig("av_videomode_autores", NULL), "auto") != 0)
				autoressd->hidden=NO;
			else {
				autoressd->hidden=NO;
				autorests->hidden=NO;
			}
		}
	}
	int setlr=0;
	addchoicebox(autoresolution, "off", _("off"));
	addchoicebox(autoresolution, "auto", _("auto"));
	addchoicebox(autoresolution, "key", _("redKey"));
	setchoiceboxselection(autoresolution, getconfig("av_videomode_autores", NULL));

	ret = getvideomodechoices();
	changeinput(autoressd, ret);
	free(ret); ret = NULL;
	setchoiceboxselection(autoressd, getconfig("av_videomode_autores_sd", NULL));

	addchoicebox(autorests, "2", _("2 sec"));
	addchoicebox(autorests, "4", _("4 sec"));
	addchoicebox(autorests, "6", _("6 sec"));
	addchoicebox(autorests, "8", _("8 sec"));
	addchoicebox(autorests, "10", _("10 sec"));
	setchoiceboxselection(autorests, getconfig("av_videomode_autores_ts", NULL));
	addchoicebox(stepmute, "0", _("off"));
	addchoicebox(stepmute, "1", _("on"));
	setchoiceboxselection(stepmute, getconfig("stepmute", NULL));

	changeinput(playerstart, "30\n40\n50\n60\n70\n80\n90\n100\n10\n20");
	setchoiceboxselection(playerstart, getconfig("vol_playerstart", NULL));
//#ifdef MIPSEL
		playerstart->hidden = YES;
//#endif
	changeinput(playerstop, "70\n80\n90\n100\n10\n20\n30\n40\n50\n60");
	setchoiceboxselection(playerstop, getconfig("vol_playerstop", NULL));
//#ifdef MIPSEL
		playerstop->hidden = YES;
//#endif
	ret = getpolicychoices();
	changeinput(policy, ret);
	free(ret); ret = NULL;
	ret = getpolicy();
	setchoiceboxselection(policy, ret);
	free(ret); ret = NULL;
	if(checkrealbox("HD51") == 0 && checkrealbox("HD60") == 0 && checkrealbox("HD61") == 0)
	{
		ret = getaspectchoices();
		changeinput(aspect, ret);
		free(ret); ret = NULL;
		ret = getaspect();
		setchoiceboxselection(aspect, ret);
		free(ret); ret = NULL;
	}
	else
		aspect->hidden = YES;

#ifdef MIPSEL
	aspect->hidden = YES;
#endif

	if(checkrealbox("HD51") == 0 && checkrealbox("HD60") == 0 && checkrealbox("HD61") == 0)
	{
		ret = getcolorformat(2);
		if(ret == NULL || strlen(ret) == 0)
		{
			free(ret); ret = NULL;
			colformatscart->hidden = YES;
		
			ret = getcolorformatchoices(0);
			changeinput(colformat, ret);
			free(ret); ret = NULL;
			ret = getcolorformat(1);
			setchoiceboxselection(colformat, ret);
			free(ret); ret = NULL;
		}
		else
		{
			free(ret); ret = NULL;
			colformatscart->hidden = NO;
		
			ret = getcolorformatchoices(1);
			changeinput(colformat, ret);
			free(ret); ret = NULL;
			ret = getcolorformat(1);
			setchoiceboxselection(colformat, ret);
			free(ret); ret = NULL;
		
			ret = getcolorformatchoices(2);
			changeinput(colformatscart, ret);
			free(ret); ret = NULL;
			ret = getcolorformat(2);
			setchoiceboxselection(colformatscart, ret);
			free(ret); ret = NULL;
		}
	}
	else
	{
		colformatscart->hidden = YES;
		colformat->hidden = YES;
	}
		

	ret = getaudiosourcechoices();
	changeinput(audiosource, ret);
	free(ret); ret = NULL;
	ret = getaudiosource();
	setchoiceboxselection(audiosource, ret);
	free(ret); ret = NULL;

#ifdef MIPSEL
	audiosource->hidden = YES;
#endif

	addchoicebox(ac3default, "0", _("no"));
	addchoicebox(ac3default, "1", _("yes"));
	setchoiceboxselection(ac3default, getconfig("av_ac3default", NULL));

	ret = getac3choices();
	changeinput(ac3mode, ret);
	free(ret); ret = NULL;
	ret = getac3();
	setchoiceboxselection(ac3mode, ret);
	free(ret); ret = NULL;

	if(checkrealbox("HD51") == 0 && checkrealbox("HD60") == 0 && checkrealbox("HD61") == 0)
	{
		ret = getaacchoices();
		changeinput(aacmode, ret);
		free(ret); ret = NULL;
		ret = getaac();
		setchoiceboxselection(aacmode, ret);
		free(ret); ret = NULL;
	}
	else
		aacmode->hidden = YES;

	if((checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1) && ac3plusmode != NULL)
	{
		if(getconfigint("av_can_ac3plusmode", NULL) == YES)
		{
			ac3plusmode->hidden = NO;
			ret = getac3pluschoices();
			changeinput(ac3plusmode, ret);
			free(ret); ret = NULL;
			ret = getac3plus();
			setchoiceboxselection(ac3plusmode, ret);
			free(ret); ret = NULL;
		}
		else
			ac3plusmode->hidden = YES;
			
		if(getconfigint("av_can_dtshdmode", NULL) == YES)
		{
			dtshdmode->hidden = NO;
			ret = getdtshdchoices();
			changeinput(dtshdmode, ret);
			free(ret); ret = NULL;
			ret = getdtshd();
			setchoiceboxselection(dtshdmode, ret);
			free(ret); ret = NULL;
		}
		else
			dtshdmode->hidden = YES;
			
		if(getconfigint("av_can_wmapromode", NULL) == YES)
		{
			wmapromode->hidden = NO;
			ret = getwmaprochoices();
			changeinput(wmapromode, ret);
			free(ret); ret = NULL;
			ret = getwmapro();
			setchoiceboxselection(wmapromode, ret);
			free(ret); ret = NULL;
		}
		else
			wmapromode->hidden = YES;
	}
	else
	{
		ac3plusmode->hidden = YES;
		dtshdmode->hidden = YES;
		wmapromode->hidden = YES;
	}
#ifdef SH4
		aacmode->hidden = YES;
#endif

	if(checkrealbox("HD51") == 0 && checkrealbox("HD60") == 0 && checkrealbox("HD61") == 0)	
	{	
		ret = getwsschoices();
		changeinput(wssmode, ret);
		free(ret); ret = NULL;
		ret = getwss();
		setchoiceboxselection(wssmode, ret);
		free(ret); ret = NULL;
	}
	else
		wssmode->hidden = YES;
		
#ifdef SH4
		wssmode->hidden = YES;
#endif
	
	changeinput(volautochangevalue, "0\n5\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60\n65\n70\n75");
	setchoiceboxselection(volautochangevalue, getconfig("volautochangevalue", NULL));

#ifdef MIPSEL
		volautochangevalue->hidden = YES;
#endif

	ret = getmode3dchoices();
	changeinput(mode3d, ret);
	free(ret); ret = NULL;
	ret = getmode3d();
// does not work
	/*addchoicebox(mode3d, "off", _("Off"));
	addchoicebox(mode3d, "sbs", _("SideBySide"));
	addchoicebox(mode3d, "tab", _("TopAndBottom"));*/	
	setchoiceboxselection(mode3d, ret);
	free(ret); ret = NULL;

	addchoicebox(autosubtitle, "0", _("no"));
	addchoicebox(autosubtitle, "1", _("yes"));
	setchoiceboxselection(autosubtitle, getconfig("autosubtitle", NULL));

#ifdef MIPSEL
	addchoicebox(audiodelaybitstream, "0", _("no"));
	addchoicebox(audiodelaybitstream, "08CA", "25ms");
	addchoicebox(audiodelaybitstream, "1194", "50ms");
	addchoicebox(audiodelaybitstream, "1A5E", "75ms");
	addchoicebox(audiodelaybitstream, "2328", "100ms");
	addchoicebox(audiodelaybitstream, "34BC", "150ms");
	addchoicebox(audiodelaybitstream, "4650", "200ms");
	addchoicebox(audiodelaybitstream, "6978", "300ms");
	addchoicebox(audiodelaybitstream, "8CA0", "400ms");
	addchoicebox(audiodelaybitstream, "AFC8", "500ms");
	setchoiceboxselection(audiodelaybitstream, getconfig("audiodelaybitstream", NULL));
	
	addchoicebox(audiodelaypcm, "0", _("no"));
	addchoicebox(audiodelaypcm, "08CA", "25ms");
	addchoicebox(audiodelaypcm, "1194", "50ms");
	addchoicebox(audiodelaypcm, "1A5E", "75ms");
	addchoicebox(audiodelaypcm, "2328", "100ms");
	addchoicebox(audiodelaypcm, "34BC", "150ms");
	addchoicebox(audiodelaypcm, "4650", "200ms");
	addchoicebox(audiodelaypcm, "6978", "300ms");
	addchoicebox(audiodelaypcm, "8CA0", "400ms");
	addchoicebox(audiodelaypcm, "AFC8", "500ms");
	setchoiceboxselection(audiodelaypcm, getconfig("audiodelaypcm", NULL));
#else	
	addchoicebox(audiodelaybitstream, "0", _("no"));
	addchoicebox(audiodelaybitstream, "350", "10ms");
	addchoicebox(audiodelaybitstream, "700", "20ms");
	addchoicebox(audiodelaybitstream, "1050", "30ms");
	addchoicebox(audiodelaybitstream, "1400", "40ms");
	addchoicebox(audiodelaybitstream, "1750", "50ms");
	addchoicebox(audiodelaybitstream, "2100", "60ms");
	addchoicebox(audiodelaybitstream, "2450", "70ms");
	addchoicebox(audiodelaybitstream, "2800", "80ms");
	addchoicebox(audiodelaybitstream, "3150", "90ms");
	addchoicebox(audiodelaybitstream, "3500", "100ms");
	setchoiceboxselection(audiodelaybitstream, getconfig("audiodelaybitstream", NULL));

	addchoicebox(audiodelaypcm, "0", _("no"));
	addchoicebox(audiodelaypcm, "350", "10ms");
	addchoicebox(audiodelaypcm, "700", "20ms");
	addchoicebox(audiodelaypcm, "1050", "30ms");
	addchoicebox(audiodelaypcm, "1400", "40ms");
	addchoicebox(audiodelaypcm, "1750", "50ms");
	addchoicebox(audiodelaypcm, "2100", "60ms");
	addchoicebox(audiodelaypcm, "2450", "70ms");
	addchoicebox(audiodelaypcm, "2800", "80ms");
	addchoicebox(audiodelaypcm, "3150", "90ms");
	addchoicebox(audiodelaypcm, "3500", "100ms");
	setchoiceboxselection(audiodelaypcm, getconfig("audiodelaypcm", NULL));
#endif
	
	drawscreen(avsettings, 0, 0);
	addscreenrc(avsettings, listbox);

//	if(flag == 1 && listbox->select != NULL)
	if(listbox->select != NULL)
		writevfdmenu(listbox->select->text);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(avsettings, tmp);
		rcret = waitrc(avsettings, 0, 0);
		tmp = listbox->select;

//		if(flag == 1 && listbox->select != NULL)
//		{
			if(rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL))
				writevfdmenu(listbox->select->text);
			if(rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))
			{
				setlr = 1;
				writevfdmenu(listbox->select->ret);
			}	
//		}
//	    else
//	    {
//			if(listbox->select != NULL)
//			{	
//				if(rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))
//					setlr = 1;
//			}
//		}
		if(setlr == 1 && ostrcmp(listbox->select->name, "autoresolution") == 0)
		{	
			autoressd->hidden=YES;
			autorests->hidden=YES;
			if(ostrcmp(autoresolution->ret, "off") != 0)
			{
				if(ostrcmp(autoresolution->ret, "key") == 0)
					autoressd->hidden=NO;
				else if(ostrcmp(autoresolution->ret, "auto") == 0)
				{	
					autoressd->hidden=NO;
					autorests->hidden=NO;
				}
			}
			drawscreen(avsettings, 0, 0);
		}
		setlr = 0;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(flag == 1 || rcret == getrcconfigint("rcok", NULL))
		{
			if(resolution->ret != NULL)
			{
				ret = getvideomode();
				if(ostrcmp(ret, resolution->ret) != 0)
				{
					setvideomode(resolution->ret, 1);
					changefbresolution(resolution->ret, 0);
					if(flag == 0)
					{
						int tret = textbox(_("Message"), _("Is this Videomode ok ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
						if(tret == 0 || tret == 2)
						{
							//setvideomode(ret, 0);
							setvideomode(getconfig("av_videomode", NULL), 1);
							changefbresolution(ret, 0);
						}
						else
						{
							addconfig("av_videomode", resolution->ret);
							writeallconfig(1);
							debug(10, "resolution->ret: %s", resolution->ret);
							if(ostrncmp("576", resolution->ret, 3) == 0 || ostrncmp("pal", resolution->ret, 3) == 0)
							{
								debug(10, "create /var/etc/.scart");
								writesys("/var/etc/.scart", "0", 0);
							}
							else
							{
								debug(10, "unlink /var/etc/.scart");
								unlink("/var/etc/.scart");
							}
						}
					}
					if(flag == 1) drawscreen(avsettings, 0, 0);
				}
				free(ret); ret = NULL;
			}
			if(autoresolution->ret != NULL)
			{
				addconfig("av_videomode_autores", autoresolution->ret);
				addconfig("av_videomode_autores_sd", autoressd->ret);
				addconfig("av_videomode_autores_ts", autorests->ret);
			}
			
			if(stepmute->ret != NULL)
				addconfig("stepmute", stepmute->ret);
				
			if(policy->ret != NULL)
			{
				ret = getpolicy();
				if(ostrcmp(ret, policy->ret) != 0)
					setpolicy(policy->ret); 
				free(ret); ret = NULL;
			}
#ifndef MIPSEL
			if(aspect->ret != NULL)
			{
				ret = getaspect();
				if(ostrcmp(ret, aspect->ret) != 0)
					setaspect(aspect->ret); 
				free(ret); ret = NULL;
			}
#endif
			if(colformat->ret != NULL)
			{
				setcolorformat(colformat->ret, 0); 
				ret = getcolorformat(1);
				changeinput(colformat, ret);
				free(ret); ret = NULL;
				drawscreen(avsettings, 0, 0);
			}
			if(colformatscart->ret != NULL)
			{
				setcolorformat(colformatscart->ret, 1); 
				ret = getcolorformat(2);
				changeinput(colformatscart, ret);
				free(ret); ret = NULL;
				drawscreen(avsettings, 0, 0);
			}
			if(audiosource->ret != NULL)
			{
				ret = getaudiosource();
				if(ostrcmp(ret, audiosource->ret) != 0)
					setaudiosource(audiosource->ret); 
				free(ret); ret = NULL;
			}
			if(ac3mode->ret != NULL)
			{
				ret = getac3();
				if(ostrcmp(ret, ac3mode->ret) != 0)
					setac3(ac3mode->ret); 
				free(ret); ret = NULL;
			}
			if(aacmode->ret != NULL)
			{
				ret = getaac();
				if(ostrcmp(ret, aacmode->ret) != 0)
					setaac(aacmode->ret); 
				free(ret); ret = NULL;
			}
			if(ac3plusmode->ret != NULL)
			{
				ret = getac3plus();
				if(ostrcmp(ret, ac3plusmode->ret) != 0)
					setac3plus(ac3plusmode->ret); 
				free(ret); ret = NULL;
			}
			if(dtshdmode->ret != NULL)
			{
				ret = getdtshd();
				if(ostrcmp(ret, dtshdmode->ret) != 0)
					setdtshd(dtshdmode->ret); 
				free(ret); ret = NULL;
			}
			if(wmapromode->ret != NULL)
			{
				ret = getwmapro();
				if(ostrcmp(ret, wmapromode->ret) != 0)
					setwmapro(wmapromode->ret); 
				free(ret); ret = NULL;
			}
			if(wssmode->ret != NULL)
			{
				ret = getwss();
				if(ostrcmp(ret, wssmode->ret) != 0)
					setwss(wssmode->ret); 
				free(ret); ret = NULL;
				drawscreen(avsettings, 0, 0);
			}
			if(mode3d->ret != NULL)
			{
				ret = getmode3d();
				if(ostrcmp(ret, mode3d->ret) != 0)
				{
					clearscreen(avsettings);
					blitfb(0);
					setmode3d(mode3d->ret);
					drawscreen(avsettings, 0, 0);
				}
				free(ret); ret = NULL;
			}
			if(audiodelaybitstream->ret != NULL)
			{
				ret = getaudiodelaybitstream();
				if(ostrcmp(ret, audiodelaybitstream->ret) != 0)
					setaudiodelaybitstream(audiodelaybitstream->ret);
				free(ret); ret = NULL;
			}

			if(audiodelaypcm->ret != NULL)
			{
				ret = getaudiodelaypcm();
				if(ostrcmp(ret, audiodelaypcm->ret) != 0)
					setaudiodelaypcm(audiodelaypcm->ret);
				free(ret); ret = NULL;
			}

			addconfigscreencheck("volautochangevalue", volautochangevalue, 0);
			status.volautochangevalue = getconfigint("volautochangevalue", NULL);
			addconfigscreencheck("av_ac3default", ac3default, 0);
			addconfigscreencheck("autosubtitle", autosubtitle, 0);
			status.autosubtitle = getconfigint("autosubtitle", NULL);
			addconfigscreen("vol_playerstart", playerstart);
			addconfigscreen("vol_playerstop", playerstop);

			if(rcret == getrcconfigint("rcok", NULL)) break;
		}
	}

	delownerrc(avsettings);
	clearscreen(avsettings);
}

#endif
