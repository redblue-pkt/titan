#ifndef BLINDSCANADJUST_H
#define BLINDSCANADJUST_H

void blindcalc()
{
	unsigned int blindmax = 0, cblindmax = 0, tblindmax = 0;
	char* tmpstr = NULL;

	blindmax = satblindscan(NULL, 1);
	cblindmax = cableblindscan(NULL, 1);
	tblindmax = terrblindscan(NULL, 1);

	tmpstr = ostrcat(tmpstr, _("Sat: "), 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(blindmax), 1, 1);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);

	tmpstr = ostrcat(tmpstr, _("Cable: "), 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(cblindmax), 1, 1);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);

	tmpstr = ostrcat(tmpstr, _("Terrestrial: "), 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(tblindmax), 1, 1);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);

	textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	free(tmpstr);
}

void blindchangetunertype(struct skin* tunertype, struct skin* minfrequency, struct skin* maxfrequency, struct skin* stepfrequency, struct skin* minsignalrate, struct skin* maxsignalrate, struct skin* stepsignalrate, struct skin* usedefaultsr, struct skin* onlydvbs, struct skin* usedefaultfec, struct skin* cminfrequency, struct skin* cmaxfrequency, struct skin* cstepfrequency, struct skin* cminsignalrate, struct skin* cmaxsignalrate, struct skin* cstepsignalrate, struct skin* cusedefaultsr, struct skin* cusedefaultfec, struct skin* tminfrequency, struct skin* tmaxfrequency, struct skin* tstepfrequency)
{
	minfrequency->hidden = YES;
	maxfrequency->hidden = YES;
	stepfrequency->hidden = YES;
	minsignalrate->hidden = YES;
	maxsignalrate->hidden = YES;
	stepsignalrate->hidden = YES;
	usedefaultsr->hidden = YES;
	onlydvbs->hidden = YES;
	usedefaultfec->hidden = YES;
	cminfrequency->hidden = YES;
	cmaxfrequency->hidden = YES;
	cstepfrequency->hidden = YES;
	cminsignalrate->hidden = YES;
	cmaxsignalrate->hidden = YES;
	cstepsignalrate->hidden = YES;
	cusedefaultsr->hidden = YES;
	cusedefaultfec->hidden = YES;
	tminfrequency->hidden = YES;
	tmaxfrequency->hidden = YES;
	tstepfrequency->hidden = YES;

	if(ostrcmp(tunertype->ret, "0") == 0) //sat
	{
		minfrequency->hidden = NO;
		maxfrequency->hidden = NO;
		stepfrequency->hidden = NO;
		minsignalrate->hidden = NO;
		maxsignalrate->hidden = NO;
		stepsignalrate->hidden = NO;
		usedefaultsr->hidden = NO;
		onlydvbs->hidden = NO;
		usedefaultfec->hidden = NO;
	}
	
	if(ostrcmp(tunertype->ret, "1") == 0) //cable
	{
		cminfrequency->hidden = NO;
		cmaxfrequency->hidden = NO;
		cstepfrequency->hidden = NO;
		cminsignalrate->hidden = NO;
		cmaxsignalrate->hidden = NO;
		cstepsignalrate->hidden = NO;
		cusedefaultsr->hidden = NO;
		cusedefaultfec->hidden = NO;
	}
	
	if(ostrcmp(tunertype->ret, "2") == 0) //terr
	{
		tminfrequency->hidden = NO;
		tmaxfrequency->hidden = NO;
		tstepfrequency->hidden = NO;
	}
}

void screenblindscanadjust()
{
	int rcret = 0, i = 0;
	struct skin* blindscan = getscreen("blindscanadjust");
	struct skin* listbox = getscreennode(blindscan, "listbox");
	struct skin* tunertype = getscreennode(blindscan, "tunertype");
	
	//sat
	struct skin* minfrequency = getscreennode(blindscan, "minfrequency");
	struct skin* maxfrequency = getscreennode(blindscan, "maxfrequency");
	struct skin* stepfrequency = getscreennode(blindscan, "stepfrequency");
	struct skin* minsignalrate = getscreennode(blindscan, "minsignalrate");
	struct skin* maxsignalrate = getscreennode(blindscan, "maxsignalrate");
	struct skin* stepsignalrate = getscreennode(blindscan, "stepsignalrate");
	struct skin* usedefaultsr = getscreennode(blindscan, "usedefaultsr");
	struct skin* onlydvbs = getscreennode(blindscan, "onlydvbs");
	struct skin* usedefaultfec = getscreennode(blindscan, "usedefaultfec");
	
	//cable
	struct skin* cminfrequency = getscreennode(blindscan, "cminfrequency");
	struct skin* cmaxfrequency = getscreennode(blindscan, "cmaxfrequency");
	struct skin* cstepfrequency = getscreennode(blindscan, "cstepfrequency");
	struct skin* cminsignalrate = getscreennode(blindscan, "cminsignalrate");
	struct skin* cmaxsignalrate = getscreennode(blindscan, "cmaxsignalrate");
	struct skin* cstepsignalrate = getscreennode(blindscan, "cstepsignalrate");
	struct skin* cusedefaultsr = getscreennode(blindscan, "cusedefaultsr");
	struct skin* cusedefaultfec = getscreennode(blindscan, "cusedefaultfec");
	
	//terr
	struct skin* tminfrequency = getscreennode(blindscan, "tminfrequency");
	struct skin* tmaxfrequency = getscreennode(blindscan, "tmaxfrequency");
	struct skin* tstepfrequency = getscreennode(blindscan, "tstepfrequency");
	
	struct skin* tmp = NULL;
	
	addchoicebox(tunertype, "0", _("Sat"));
	addchoicebox(tunertype, "1", _("Cable"));
	addchoicebox(tunertype, "2", _("Terrestrial"));
	setchoiceboxselection(tunertype, "0");

	//sat
	changemask(minfrequency, "00000");
	changeinput(minfrequency, getconfig("blindminfrequency", NULL));
	if(minfrequency->input != NULL)
	{
		int len = strlen(minfrequency->input);
		for(i = 0; i < 5 - len; i++)
			minfrequency->input = ostrcat("0", minfrequency->input, 0, 1);
	}
  
	changemask(maxfrequency, "00000");
	changeinput(maxfrequency, getconfig("blindmaxfrequency", NULL));
	if(maxfrequency->input != NULL)
	{
		int len = strlen(maxfrequency->input);
		for(i = 0; i < 5 - len; i++)
			maxfrequency->input = ostrcat("0", maxfrequency->input, 0, 1);
	}
  
	changeinput(stepfrequency, getconfig("blindstepfrequency", NULL));
	changemask(stepfrequency, "00");
	if(stepfrequency->input != NULL)
	{
		int len = strlen(stepfrequency->input);
		for(i = 0; i < 2 - len; i++)
			stepfrequency->input = ostrcat("0", stepfrequency->input, 0, 1);
	}
  
	changeinput(minsignalrate, getconfig("blindminsignalrate", NULL));
	changemask(minsignalrate, "00000");
	if(minsignalrate->input != NULL)
	{
		int len = strlen(minsignalrate->input);
		for(i = 0; i < 5 - len; i++)
			minsignalrate->input = ostrcat("0", minsignalrate->input, 0, 1);
	}
  
	changeinput(maxsignalrate, getconfig("blindmaxsignalrate", NULL));
	changemask(maxsignalrate, "00000");
	if(maxsignalrate->input != NULL)
	{
		int len = strlen(maxsignalrate->input);
		for(i = 0; i < 5 - len; i++)
			maxsignalrate->input = ostrcat("0", maxsignalrate->input, 0, 1);
	}
  
	changeinput(stepsignalrate, getconfig("blindstepsignalrate", NULL));
	changemask(stepsignalrate, "00");
	if(stepsignalrate->input != NULL)
	{
		int len = strlen(stepsignalrate->input);
		for(i = 0; i < 2 - len; i++)
			stepsignalrate->input = ostrcat("0", stepsignalrate->input, 0, 1);
	}

	addchoicebox(usedefaultsr, "0", _("no"));
	addchoicebox(usedefaultsr, "1", _("yes"));
	setchoiceboxselection(usedefaultsr, getconfig("blindusedefaultsr", NULL));

	addchoicebox(onlydvbs, "0", _("no"));
	addchoicebox(onlydvbs, "1", _("yes"));
	setchoiceboxselection(onlydvbs, getconfig("blindonlydvbs", NULL));

	addchoicebox(usedefaultfec, "0", _("no"));
	addchoicebox(usedefaultfec, "1", _("yes"));
	setchoiceboxselection(usedefaultfec, getconfig("blindusedefaultfec", NULL));
	
	//cable
	changemask(cminfrequency, "00000");
	changeinput(cminfrequency, getconfig("cblindminfrequency", NULL));
	if(cminfrequency->input != NULL)
	{
		int len = strlen(cminfrequency->input);
		for(i = 0; i < 5 - len; i++)
			cminfrequency->input = ostrcat("0", cminfrequency->input, 0, 1);
	}
  
	changemask(cmaxfrequency, "00000");
	changeinput(cmaxfrequency, getconfig("cblindmaxfrequency", NULL));
	if(cmaxfrequency->input != NULL)
	{
		int len = strlen(cmaxfrequency->input);
		for(i = 0; i < 5 - len; i++)
			cmaxfrequency->input = ostrcat("0", cmaxfrequency->input, 0, 1);
	}
  
	changeinput(cstepfrequency, getconfig("cblindstepfrequency", NULL));
	changemask(cstepfrequency, "00");
	if(cstepfrequency->input != NULL)
	{
		int len = strlen(cstepfrequency->input);
		for(i = 0; i < 2 - len; i++)
			cstepfrequency->input = ostrcat("0", cstepfrequency->input, 0, 1);
	}
  
	changeinput(cminsignalrate, getconfig("cblindminsignalrate", NULL));
	changemask(cminsignalrate, "00000");
	if(cminsignalrate->input != NULL)
	{
		int len = strlen(cminsignalrate->input);
		for(i = 0; i < 5 - len; i++)
			cminsignalrate->input = ostrcat("0", cminsignalrate->input, 0, 1);
	}
  
	changeinput(cmaxsignalrate, getconfig("cblindmaxsignalrate", NULL));
	changemask(cmaxsignalrate, "00000");
	if(cmaxsignalrate->input != NULL)
	{
		int len = strlen(cmaxsignalrate->input);
		for(i = 0; i < 5 - len; i++)
			cmaxsignalrate->input = ostrcat("0", cmaxsignalrate->input, 0, 1);
	}
  
	changeinput(cstepsignalrate, getconfig("cblindstepsignalrate", NULL));
	changemask(cstepsignalrate, "00");
	if(cstepsignalrate->input != NULL)
	{
		int len = strlen(cstepsignalrate->input);
		for(i = 0; i < 2 - len; i++)
			cstepsignalrate->input = ostrcat("0", cstepsignalrate->input, 0, 1);
	}

	addchoicebox(cusedefaultsr, "0", _("no"));
	addchoicebox(cusedefaultsr, "1", _("yes"));
	setchoiceboxselection(cusedefaultsr, getconfig("cblindusedefaultsr", NULL));

	addchoicebox(cusedefaultfec, "0", _("no"));
	addchoicebox(cusedefaultfec, "1", _("yes"));
	setchoiceboxselection(cusedefaultfec, getconfig("cblindusedefaultfec", NULL));
	
	//terr
	changemask(tminfrequency, "00000");
	changeinput(tminfrequency, getconfig("tblindminfrequency", NULL));
	if(tminfrequency->input != NULL)
	{
		int len = strlen(tminfrequency->input);
		for(i = 0; i < 5 - len; i++)
			tminfrequency->input = ostrcat("0", tminfrequency->input, 0, 1);
	}
  
	changemask(tmaxfrequency, "00000");
	changeinput(tmaxfrequency, getconfig("tblindmaxfrequency", NULL));
	if(tmaxfrequency->input != NULL)
	{
		int len = strlen(tmaxfrequency->input);
		for(i = 0; i < 5 - len; i++)
			tmaxfrequency->input = ostrcat("0", tmaxfrequency->input, 0, 1);
	}
  
	changeinput(tstepfrequency, getconfig("tblindstepfrequency", NULL));
	changemask(tstepfrequency, "00");
	if(tstepfrequency->input != NULL)
	{
		int len = strlen(tstepfrequency->input);
		for(i = 0; i < 2 - len; i++)
			tstepfrequency->input = ostrcat("0", tstepfrequency->input, 0, 1);
	}

	drawscreen(blindscan, 2, 0);
	blindchangetunertype(tunertype, minfrequency, maxfrequency, stepfrequency, minsignalrate, maxsignalrate, stepsignalrate, usedefaultsr, onlydvbs, usedefaultfec, cminfrequency, cmaxfrequency, cstepfrequency, cminsignalrate, cmaxsignalrate, cstepsignalrate, cusedefaultsr, cusedefaultfec, tminfrequency, tmaxfrequency, tstepfrequency);
	drawscreen(blindscan, 0, 0);
	addscreenrc(blindscan, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(blindscan, tmp);
		rcret = waitrc(blindscan, 0, 0);
		tmp = listbox->select;
		
		if(listbox->select != NULL && ostrcmp(listbox->select->name, "tunertype") == 0)
		{
			blindchangetunertype(tunertype, minfrequency, maxfrequency, stepfrequency, minsignalrate, maxsignalrate, stepsignalrate, usedefaultsr, onlydvbs, usedefaultfec, cminfrequency, cmaxfrequency, cstepfrequency, cminsignalrate, cmaxsignalrate, cstepsignalrate, cusedefaultsr, cusedefaultfec, tminfrequency, tmaxfrequency, tstepfrequency);
			drawscreen(blindscan, 0, 0);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			//sat
			if(minfrequency->ret != NULL)
			{
				int i = atoi(minfrequency->ret);
				addconfigint("blindminfrequency", i);
			}
			if(maxfrequency->ret != NULL)
			{
				int i = atoi(maxfrequency->ret);
				addconfigint("blindmaxfrequency", i);
			}
			if(stepfrequency->ret != NULL)
			{
				int i = atoi(stepfrequency->ret);
				addconfigint("blindstepfrequency", i);
			}
			if(minsignalrate->ret != NULL)
			{
				int i = atoi(minsignalrate->ret);
				addconfigint("blindminsignalrate", i);
			}
			if(maxsignalrate->ret != NULL)
			{
				int i = atoi(maxsignalrate->ret);
				addconfigint("blindmaxsignalrate", i);
			}
			if(stepsignalrate->ret != NULL)
			{
				int i = atoi(stepsignalrate->ret);
				addconfigint("blindstepsignalrate", i);
			}

			if(getconfigint("blindminfrequency", NULL) > getconfigint("blindmaxfrequency", NULL))
				addconfigint("blindminfrequency", getconfigint("blindmaxfrequency", NULL));

			if(getconfigint("blindminsignalrate", NULL) > getconfigint("blindmaxsignalrate", NULL))
				addconfigint("blindminfrequency", getconfigint("blindmaxsignalrate", NULL));

			if(getconfigint("blindstepfrequency", NULL) < 1)
				addconfigint("blindstepfrequency", 20);

			if(getconfigint("blindstepsignalrate", NULL) < 1)
				addconfigint("blindstepsignalrate", 20);

			addconfigscreencheck("blindusedefaultsr", usedefaultsr, "0");
			addconfigscreencheck("blindonlydvbs", onlydvbs, "0");
			addconfigscreencheck("blindusedefaultfec", usedefaultfec, "0");
			
			//cable
			if(cminfrequency->ret != NULL)
			{
				int i = atoi(cminfrequency->ret);
				addconfigint("cblindminfrequency", i);
			}
			if(cmaxfrequency->ret != NULL)
			{
				int i = atoi(cmaxfrequency->ret);
				addconfigint("cblindmaxfrequency", i);
			}
			if(cstepfrequency->ret != NULL)
			{
				int i = atoi(cstepfrequency->ret);
				addconfigint("cblindstepfrequency", i);
			}
			if(cminsignalrate->ret != NULL)
			{
				int i = atoi(cminsignalrate->ret);
				addconfigint("cblindminsignalrate", i);
			}
			if(cmaxsignalrate->ret != NULL)
			{
				int i = atoi(cmaxsignalrate->ret);
				addconfigint("cblindmaxsignalrate", i);
			}
			if(cstepsignalrate->ret != NULL)
			{
				int i = atoi(cstepsignalrate->ret);
				addconfigint("cblindstepsignalrate", i);
			}

			if(getconfigint("cblindminfrequency", NULL) > getconfigint("cblindmaxfrequency", NULL))
				addconfigint("cblindminfrequency", getconfigint("cblindmaxfrequency", NULL));

			if(getconfigint("cblindminsignalrate", NULL) > getconfigint("cblindmaxsignalrate", NULL))
				addconfigint("cblindminfrequency", getconfigint("cblindmaxsignalrate", NULL));

			if(getconfigint("cblindstepfrequency", NULL) < 1)
				addconfigint("cblindstepfrequency", 20);

			if(getconfigint("cblindstepsignalrate", NULL) < 1)
				addconfigint("cblindstepsignalrate", 20);

			addconfigscreencheck("cblindusedefaultsr", cusedefaultsr, "0");
			addconfigscreencheck("cblindusedefaultfec", cusedefaultfec, "0");
			
			//terr
			if(tminfrequency->ret != NULL)
			{
				int i = atoi(tminfrequency->ret);
				addconfigint("tblindminfrequency", i);
			}
			if(tmaxfrequency->ret != NULL)
			{
				int i = atoi(tmaxfrequency->ret);
				addconfigint("tblindmaxfrequency", i);
			}
			if(tstepfrequency->ret != NULL)
			{
				int i = atoi(tstepfrequency->ret);
				addconfigint("tblindstepfrequency", i);
			}

			if(getconfigint("tblindminfrequency", NULL) > getconfigint("tblindmaxfrequency", NULL))
				addconfigint("tblindminfrequency", getconfigint("tblindmaxfrequency", NULL));

			if(getconfigint("tblindstepfrequency", NULL) < 1)
				addconfigint("tblindstepfrequency", 20);


			blindcalc();
			break;
		}
	}

	delownerrc(blindscan);
	clearscreen(blindscan);
}

#endif
