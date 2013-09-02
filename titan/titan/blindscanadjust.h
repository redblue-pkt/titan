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
	int rcret = 0;
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
	minfrequency->input = mask(minfrequency->input, 5, "0");
  
	changemask(maxfrequency, "00000");
	changeinput(maxfrequency, getconfig("blindmaxfrequency", NULL));
	maxfrequency->input = mask(maxfrequency->input, 5, "0");
  
	changemask(stepfrequency, "00");
	changeinput(stepfrequency, getconfig("blindstepfrequency", NULL));
	stepfrequency->input = mask(stepfrequency->input, 2, "0");
  
	changemask(minsignalrate, "00000");
	changeinput(minsignalrate, getconfig("blindminsignalrate", NULL));
	minsignalrate->input = mask(minsignalrate->input, 5, "0");
  
	changemask(maxsignalrate, "00000");
	changeinput(maxsignalrate, getconfig("blindmaxsignalrate", NULL));
	maxsignalrate->input = mask(maxsignalrate->input, 5, "0");
  
	changemask(stepsignalrate, "00");
	changeinput(stepsignalrate, getconfig("blindstepsignalrate", NULL));
	stepsignalrate->input = mask(stepsignalrate->input, 2, "0");

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
	changemask(cminfrequency, "000000");
	changeinput(cminfrequency, getconfig("cblindminfrequency", NULL));
	cminfrequency->input = mask(cminfrequency->input, 6, "0");
  
	changemask(cmaxfrequency, "000000");
	changeinput(cmaxfrequency, getconfig("cblindmaxfrequency", NULL));
	cmaxfrequency->input = mask(cmaxfrequency->input, 6, "0");
  
	changeinput(cstepfrequency, getconfig("cblindstepfrequency", NULL));
	changemask(cstepfrequency, "00000");
	cstepfrequency->input = mask(cstepfrequency->input, 5, "0");
  
	changeinput(cminsignalrate, getconfig("cblindminsignalrate", NULL));
	changemask(cminsignalrate, "00000");
	cminsignalrate->input = mask(cminsignalrate->input, 5, "0");
  
	changeinput(cmaxsignalrate, getconfig("cblindmaxsignalrate", NULL));
	changemask(cmaxsignalrate, "00000");
	cmaxsignalrate->input = mask(cmaxsignalrate->input, 5, "0");
  
	changeinput(cstepsignalrate, getconfig("cblindstepsignalrate", NULL));
	changemask(cstepsignalrate, "00");
	cstepsignalrate->input = mask(cstepsignalrate->input, 2, "0");

	addchoicebox(cusedefaultsr, "0", _("no"));
	addchoicebox(cusedefaultsr, "1", _("yes"));
	setchoiceboxselection(cusedefaultsr, getconfig("cblindusedefaultsr", NULL));

	addchoicebox(cusedefaultfec, "0", _("no"));
	addchoicebox(cusedefaultfec, "1", _("yes"));
	setchoiceboxselection(cusedefaultfec, getconfig("cblindusedefaultfec", NULL));
	
	//terr
	changemask(tminfrequency, "000000");
	changeinput(tminfrequency, getconfig("tblindminfrequency", NULL));
	tminfrequency->input = mask(tminfrequency->input, 6, "0");
  
	changemask(tmaxfrequency, "000000");
	changeinput(tmaxfrequency, getconfig("tblindmaxfrequency", NULL));
	tmaxfrequency->input = mask(tmaxfrequency->input, 6, "0");
  
	changeinput(tstepfrequency, getconfig("tblindstepfrequency", NULL));
	changemask(tstepfrequency, "00000");
	tstepfrequency->input = mask(tstepfrequency->input, 5, "0");

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
				addconfigint("cblindstepfrequency", 20000);

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
				addconfigint("tblindstepfrequency", 20000);


			blindcalc();
			break;
		}
	}

	delownerrc(blindscan);
	clearscreen(blindscan);
}

#endif
