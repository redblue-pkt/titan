#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

void videoApplySettings()
{
	if(file_exist("/proc/stb/vmpeg/0/pep_apply"))
		writesys("/proc/stb/vmpeg/0/pep_apply", "0", 1);
}

void screenvideosettings()
{
	int rcret = 0, ibrightness = 0, icontrast = 0, itint = 0;
	int isaturation = 0;
	struct skin* videosettings = getscreen("videosettings");
	struct skin* listbox = getscreennode(videosettings, "listbox");
	struct skin* brightness = getscreennode(videosettings, "brightness");
	struct skin* contrast = getscreennode(videosettings, "contrast");
	struct skin* tint = getscreennode(videosettings, "tint");
	struct skin* saturation = getscreennode(videosettings, "saturation");
	struct skin* tmp = NULL;

#ifdef SH4
	if(brightness != NULL)
	{
		ibrightness = getconfigint("vs_brightness", NULL);
		brightness->progresssize = (int)ceil(((float)ibrightness / 2.56));
	}
	if(contrast != NULL)
	{
		icontrast = getconfigint("vs_contrast", NULL);
		contrast->progresssize = (int)ceil(((float)icontrast / 2.56));
	}
	if(tint != NULL)
	{
		itint = getconfigint("vs_tint", NULL);
		tint->progresssize = (int)ceil(((float)itint / 2.56));
	}

	if(saturation != NULL)
	{
		isaturation = getconfigint("vs_saturation", NULL);
		saturation->progresssize = (int)ceil(((float)isaturation / 2.56));
	}
#else
	if(brightness != NULL)
	{
		ibrightness = getconfigint("vs_brightness", NULL);
		brightness->progresssize = (int)ceil(((float)ibrightness / 160.00));
	}
	if(contrast != NULL)
	{
		icontrast = getconfigint("vs_contrast", NULL);
		contrast->progresssize = (int)ceil(((float)icontrast / 160.00));
	}
	if(tint != NULL)
	{
		itint = getconfigint("vs_tint", NULL);
		tint->progresssize = (int)ceil(((float)itint / 160.00));
	}

	if(saturation != NULL)
	{
		isaturation = getconfigint("vs_saturation", NULL);
		saturation->progresssize = (int)ceil(((float)isaturation / 160.00));
	}
#endif

	drawscreen(videosettings, 0, 0);
	addscreenrc(videosettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(videosettings, tmp);
		rcret = waitrc(videosettings, 0, 0);
		tmp = listbox->select;

#ifdef SH4
		ibrightness = (int)ceil(((float)brightness->progresssize * 2.56));
		icontrast = (int)ceil(((float)contrast->progresssize * 2.56));
		itint = (int)ceil(((float)tint->progresssize * 2.56));
		isaturation = (int)ceil(((float)saturation->progresssize * 2.56));
#else
		ibrightness = (int)ceil(((float)brightness->progresssize * 160.00));
		icontrast = (int)ceil(((float)contrast->progresssize * 160.00));
		itint = (int)ceil(((float)tint->progresssize * 160.00));
		isaturation = (int)ceil(((float)saturation->progresssize * 160.00));
#endif
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigint("vs_brightness", ibrightness);
			addconfigint("vs_contrast", icontrast);
			addconfigint("vs_tint", itint);
			addconfigint("vs_saturation", isaturation);
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
#ifdef SH4
			ibrightness = 128;
			icontrast = 128;
			itint = 128;
			isaturation = 128;

			brightness->progresssize = (int)ceil(((float)ibrightness / 2.56));
			contrast->progresssize = (int)ceil(((float)icontrast / 2.56));
			tint->progresssize = (int)ceil(((float)itint / 2.56));
			saturation->progresssize = (int)ceil(((float)isaturation / 2.56));
#else
			ibrightness = 8000;
			icontrast = 8000;
			itint = 8000;
			isaturation = 8000;

			brightness->progresssize = (int)ceil(((float)ibrightness / 160.00));
			contrast->progresssize = (int)ceil(((float)icontrast / 160.00));
			tint->progresssize = (int)ceil(((float)itint / 160.00));
			saturation->progresssize = (int)ceil(((float)isaturation / 160.00));
#endif

			drawscreen(videosettings, 0, 0);
		}

		setbrightness(ibrightness);
		setcontrast(icontrast);
		settint(itint);
		setsaturation(isaturation);
		videoApplySettings();
	}

	setbrightness(getconfigint("vs_brightness", NULL));
	setcontrast(getconfigint("vs_contrast", NULL));
	settint(getconfigint("vs_tint", NULL));
	setsaturation(getconfigint("vs_saturation", NULL));
	videoApplySettings();
	delownerrc(videosettings);
	clearscreen(videosettings);
}

#endif
