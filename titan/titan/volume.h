#ifndef VOLUME_H
#define VOLUME_H

void changevolume(int direction, struct skin* volumebar)
{
	int vol = 0;

	vol = getvol();

	if(direction == 0)
		vol += 5;
	if(direction == 1)
		vol -= 5;
	if(vol < 0) vol = 0;
	if(vol > 100) vol = 100;

	if(volumebar != NULL)
	{
		if(setvol(vol) == 0);
			volumebar->progresssize = vol;
	}
}

void screenvolume(int direction)
{
	int rcret = 0, tmpscreencalc = 0;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* volume = getscreen("volume");
	struct skin* volumebar = getscreennode(volume, "volumebar");
	char* bg = NULL;

	delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
	delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
	delrc(getrcconfigint("rcmute", NULL), NULL, NULL);

	if(status.mute == 1)
		screenmute(1);

	changevolume(direction, volumebar);

	setnodeattr(volume, framebuffer);
	bg = saverect(volume->rposx, volume->rposy, volume->rwidth, volume->rheight);

	tmpscreencalc = status.screencalc;
	status.screencalc = 0;
	drawscreen(volume, 0);
	status.screencalc = tmpscreencalc;

	while(1)
	{
		rcret = waitrc(volume, getconfigint("volbartimeout", NULL) * 1000, 0);

		if(rcret == RCTIMEOUT) break;
		//if(rcret == getrcconfigint("rcexit", NULL)) break;
		//if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == getrcconfigint("rcvolup", NULL))
		{
			changevolume(0, volumebar);
			tmpscreencalc = status.screencalc;
			status.screencalc = 0;
			drawscreen(volume, 0);
			status.screencalc = tmpscreencalc;
			continue;
		}
		if(rcret == getrcconfigint("rcvoldown", NULL))
		{
			changevolume(1, volumebar);
			tmpscreencalc = status.screencalc;
			status.screencalc = 0;
			drawscreen(volume, 0);
			status.screencalc = tmpscreencalc;
			continue;
		}
		status.rckey = rcret;
		break;
	}

	addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
	addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
	addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
	restorerect(bg, volume->rposx, volume->rposy, volume->rwidth, volume->rheight);
	blitfb();
}

void screenvolumeup()
{
	screenvolume(0);
}

void screenvolumedown()
{
	screenvolume(1);
}

#endif
