#ifndef AVSETTINGS_H
#define AVSETTINGS_H

//flag 0: whithout vfd output
//flag 1: whith vfd output
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
	struct skin* volautochangevalue = getscreennode(avsettings, "volautochangevalue");
	struct skin* mode3d = getscreennode(avsettings, "mode3d");
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
	addchoicebox(autoresolution, "blue", _("blueKey"));
	setchoiceboxselection(autoresolution, getconfig("av_videomode_autores", NULL));
	addchoicebox(autoressd, "576i", _("576i"));
	addchoicebox(autoressd, "576p", _("576p"));
	addchoicebox(autoressd, "720p", _("720p"));
	addchoicebox(autoressd, "1080i", _("1080i"));
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

	ret = getpolicychoices();
	changeinput(policy, ret);
	free(ret); ret = NULL;
	ret = getpolicy();
	setchoiceboxselection(policy, ret);
	free(ret); ret = NULL;

	ret = getaspectchoices();
	changeinput(aspect, ret);
	free(ret); ret = NULL;
	ret = getaspect();
	setchoiceboxselection(aspect, ret);
	free(ret); ret = NULL;

	ret = getcolorformatchoices();
	changeinput(colformat, ret);
	free(ret); ret = NULL;
	ret = getcolorformat(1);
	setchoiceboxselection(colformat, ret);
	free(ret); ret = NULL;

	ret = getcolorformat(2);
	if(ret == NULL || strlen(ret) == 0)
		colformatscart->hidden = YES;
	else
	{
		colformatscart->hidden = NO;
		changeinput(colformatscart, ret);
	}
	free(ret); ret = NULL;

	ret = getaudiosourcechoices();
	changeinput(audiosource, ret);
	free(ret); ret = NULL;
	ret = getaudiosource();
	setchoiceboxselection(audiosource, ret);
	free(ret); ret = NULL;

	addchoicebox(ac3default, "0", _("no"));
	addchoicebox(ac3default, "1", _("yes"));
	setchoiceboxselection(ac3default, getconfig("av_ac3default", NULL));

	ret = getac3choices();
	changeinput(ac3mode, ret);
	free(ret); ret = NULL;
	ret = getac3();
	setchoiceboxselection(ac3mode, ret);
	free(ret); ret = NULL;
	
	changeinput(volautochangevalue, "0\n5\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60\n65\n70\n75\n80\n85\n90\n95\n100");
	setchoiceboxselection(volautochangevalue, getconfig("volautochangevalue", NULL));

	ret = getmode3dchoices();
	changeinput(mode3d, ret);
	free(ret); ret = NULL;
	ret = getmode3d();
	setchoiceboxselection(mode3d, ret);
	free(ret); ret = NULL;

	drawscreen(avsettings, 0, 0);
	addscreenrc(avsettings, listbox);

	if(flag == 1 && listbox->select != NULL)
		writevfd(listbox->select->text);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(avsettings, tmp);
		rcret = waitrc(avsettings, 0, 0);
		tmp = listbox->select;

		if(flag == 1 && listbox->select != NULL)
		{
			if(rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL))
				writevfd(listbox->select->text);
			if(rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))
			{
				setlr = 1;
				writevfd(listbox->select->ret);
			}	
		} else {
			if(listbox->select != NULL)
			{	
				if(rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))
					setlr = 1;
			}
		}
		if(setlr ==1 && ostrcmp(listbox->select->name, "autoresolution") == 0)
		{	
			autoressd->hidden=YES;
			autorests->hidden=YES;
			if(ostrcmp(autoresolution->ret, "off") != 0)
			{
				if(ostrcmp(autoresolution->ret, "blue") == 0)
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
					setvideomode(resolution->ret, 0); 
					changefbresolution(resolution->ret);
					if(flag == 0)
					{
						int tret = textbox(_("Message"), _("Is this Videomode ok ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
						if(tret == 0 || tret == 2)
						{
							setvideomode(ret, 0); 
							changefbresolution(ret);
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
			if(aspect->ret != NULL)
			{
				ret = getaspect();
				if(ostrcmp(ret, aspect->ret) != 0)
					setaspect(aspect->ret); 
				free(ret); ret = NULL;
			}
			if(colformat->ret != NULL)
			{
				setcolorformat(colformat->ret); 
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
			if(mode3d->ret != NULL)
			{
				ret = getmode3d();
				if(ostrcmp(ret, mode3d->ret) != 0)
					setmode3d(mode3d->ret); 
				free(ret); ret = NULL;
			}
			addconfigscreencheck("volautochangevalue", volautochangevalue, 0);
			status.volautochangevalue = getconfigint("volautochangevalue", NULL);
			addconfigscreencheck("av_ac3default", ac3default, 0);
			if(rcret == getrcconfigint("rcok", NULL)) break;
		}
	}

	delownerrc(avsettings);
	clearscreen(avsettings);
}

#endif
