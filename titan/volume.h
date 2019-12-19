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
		if(setvol(vol) == 0)
			volumebar->progresssize = vol;
	}
}

//flag 2 = no framebuffer lock
void screenvolume(int direction, int flag)
{
	int rcret = 0;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* volume = getscreen("volume");
	struct skin* volumebar = getscreennode(volume, "volumebar");
	char* bg = NULL;

	delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
	delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);	
	delrc(getrcconfigint("rcmute", NULL), NULL, NULL);

	if(status.mute == 1)
	{
		if(flag == 2)
			screenmute(NULL, NULL, 3);
		else
			screenmute(NULL, NULL, 1);
	}

	changevolume(direction, volumebar);

	setnodeattr(volume, framebuffer, 0);
	bg = savescreen(volume);

	drawscreen(volume, 0, flag);

	while(1)
	{
		if(flag == 2)
		{
			rcret = RCTIMEOUT;
			usleep(300000);
		}
		else
			rcret = waitrc(volume, getconfigint("volbartimeout", NULL) * 1000, 0);

		if(rcret == RCTIMEOUT) break;
		//if(rcret == getrcconfigint("rcexit", NULL)) break;
		//if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == getrcconfigint("rcvolup", NULL))
		{
			changevolume(0, volumebar);
			drawscreen(volume, 0, flag);
			continue;
		}
		if(rcret == getrcconfigint("rcvoldown", NULL))
		{
			changevolume(1, volumebar);
			drawscreen(volume, 0, flag);
			continue;
		}
		status.rckey = rcret;
		break;
	}

	addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
	addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);

	addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
	restorescreen(bg, volume);
	blitfb(0);
}

//screen, node, screencalc and filelistview are needed if funktion is called from rc handle
//flag 2 = no framebuffer lock
void screenvolumeup(struct skin* screen, struct skin* node, int screencalc, int filelistview, int flag)
{
#ifdef MIPSEL
	if(getconfigint("ForwardVolume", NULL) == 1)
	{
		forwardKey(getrcconfigint("rcvolup", NULL));
		return;
	}
#endif
	screenvolume(0, flag);
}

//screen, node, screencalc and filelistview are needed if funktion is called from rc handle
//flag 2 = no framebuffer lock
void screenvolumedown(struct skin* screen, struct skin* node, int screencalc, int filelistview, int flag)
{
#ifdef MIPSEL
	if(getconfigint("ForwardVolume", NULL) == 1)
	{
		forwardKey(getrcconfigint("rcvoldown", NULL));
		return;
	}
#endif
	screenvolume(1, flag);
}

#endif
