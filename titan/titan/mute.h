#ifndef MUTE_H
#define MUTE_H

//screen and node are needed if funktion is called from rc handle
//flag 1 = no blit
//flag 2 = no framebuffer lock
//flag 3 = no framebuffer lock and no blit
void screenmute(struct skin* screen, struct skin* node, int flag)
{
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* mute = getscreen("mute");

	if(getconfig("stepmute", NULL) == NULL)
		addconfig("stepmute", "0");
		
	if(status.mute == 0 && atoi(getconfig("stepmute", NULL)) == 1)
	{
		status.mute = 2;
		setmute(2);
	}
	else if(status.mute == 0 || status.mute == 2)
	{
		if(status.mute == 2)
			setvol(getconfigint("vol", NULL));
		status.mute = 1;
		setmute(1);
		setnodeattr(mute, framebuffer);
		if(status.drawallwaysbg[0] != NULL)
			free(status.drawallwaysbg[0]);
		status.drawallwaysbg[0] = savescreen(mute);
		if(flag == 2 || flag == 3)
			drawscreen(mute, 0, 2);
		else
			drawscreen(mute, 0, 0);
		status.drawallways[0] = mute;
	}
	
	else
	{
		status.mute = 0;
		setmute(0);
		if(flag == 2 || flag == 3)
			clearscreennolock(mute);
		else
			clearscreen(mute);
		if(status.drawallwaysbg[0] != NULL)
		{
			restorescreen(status.drawallwaysbg[0], mute);
			status.drawallwaysbg[0] = NULL;
		}
		status.drawallways[0] = NULL;
		if(flag == 0 || flag == 2)
			blitfb(0);
	}
}

#endif
