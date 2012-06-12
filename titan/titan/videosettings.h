#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

void screenvideosettings()
{
	int rcret = 0, ibrightness = 0, icontrast = 0, itint = 0;
	//int isaturation = 0;
	struct skin* videosettings = getscreen("videosettings");
	struct skin* listbox = getscreennode(videosettings, "listbox");
	struct skin* brightness = getscreennode(videosettings, "brightness");
	struct skin* contrast = getscreennode(videosettings, "contrast");
	struct skin* tint = getscreennode(videosettings, "tint");
	//struct skin* saturation = getscreennode(videosettings, "saturation");
	struct skin* tmp = NULL;

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
/*
	if(saturation != NULL)
	{
		isaturation = getconfigint("vs_saturation", NULL);
		saturation->progresssize = (int)ceil(((float)isaturation / 2.56));
	}
*/

	drawscreen(videosettings, 0, 0);
	addscreenrc(videosettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(videosettings, tmp);
		rcret = waitrc(videosettings, 0, 0);
		tmp = listbox->select;

		ibrightness = (int)ceil(((float)brightness->progresssize * 2.56));
		icontrast = (int)ceil(((float)contrast->progresssize * 2.56));
		itint = (int)ceil(((float)tint->progresssize * 2.56));
		//isaturation = (int)ceil(((float)saturation->progresssize * 2.56));

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigint("vs_brightness", ibrightness);
			addconfigint("vs_contrast", icontrast);
			addconfigint("vs_tint", itint);
			//addconfigint("vs_saturation", isaturation);
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			ibrightness = 128;
			icontrast = 128;
			itint = 128;
			//isaturation = 128;

			brightness->progresssize = (int)ceil(((float)ibrightness / 2.56));
			contrast->progresssize = (int)ceil(((float)icontrast / 2.56));
			tint->progresssize = (int)ceil(((float)itint / 2.56));
			//saturation->progresssize = (int)ceil(((float)isaturation / 2.56));

			drawscreen(videosettings, 0, 0);
		}

		setbrightness(ibrightness);
		setcontrast(icontrast);
		settint(itint);
		//setsaturation(isaturation);
	}

	setbrightness(getconfigint("vs_brightness", NULL));
	setcontrast(getconfigint("vs_contrast", NULL));
	settint(getconfigint("vs_tint", NULL));
	//setsaturation(getconfigint("vs_saturation", NULL));
	delownerrc(videosettings);
	clearscreen(videosettings);
}

#endif
