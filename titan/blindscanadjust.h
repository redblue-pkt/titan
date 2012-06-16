#ifndef BLINDSCANADJUST_H
#define BLINDSCANADJUST_H

//todo set defaults

void screenblindscanadjust()
{
	int rcret = 0, i = 0;
	struct skin* blindscan = getscreen("blindscanadjust");
	struct skin* listbox = getscreennode(blindscan, "listbox");
	struct skin* minfrequency = getscreennode(blindscan, "minfrequency");
	struct skin* maxfrequency = getscreennode(blindscan, "maxfrequency");
	struct skin* stepfrequency = getscreennode(blindscan, "stepfrequency");
	struct skin* minsignalrate = getscreennode(blindscan, "minsignalrate");
	struct skin* maxsignalrate = getscreennode(blindscan, "maxsignalrate");
	struct skin* stepsignalrate = getscreennode(blindscan, "stepsignalrate");
	struct skin* tmp = NULL;

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

	drawscreen(blindscan, 0, 0);
	addscreenrc(blindscan, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(blindscan, tmp);
		rcret = waitrc(blindscan, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("blindminfrequency", minfrequency, "9500");
			addconfigscreencheck("blindmaxfrequency", maxfrequency, "14500");
			addconfigscreencheck("blindstepfrequency", stepfrequency, "20");
			addconfigscreencheck("blindminsignalrate", minsignalrate, "20000");
			addconfigscreencheck("blindmaxsignalrate", maxsignalrate, "30000");
			addconfigscreencheck("blindstepsignalrate", stepsignalrate, "20");
			
			break;
		}
	}

	delownerrc(blindscan);
	clearscreen(blindscan);
}

#endif
