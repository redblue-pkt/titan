#ifndef MUTE_H
#define MUTE_H

void screenmute(int flag)
{
	int tmpscreencalc = 0;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* mute = getscreen("mute");

	if(status.mute == 0 && atoi(getconfig("stepmute", NULL)) == 1)
	{
		err("************");
		status.mute = 2;
		setmute(2);
	}
	else if(status.mute == 0 || status.mute == 2)
	{
		setmute(1);
		if(status.mute == 2)
			setvol(getconfigint("vol", NULL));
		status.mute = 1;
		setnodeattr(mute, framebuffer);
		if(status.drawallwaysbg[0] != NULL)
			free(status.drawallwaysbg[0]);
		status.drawallwaysbg[0] = savescreen(mute);
		tmpscreencalc = status.screencalc;
		status.screencalc = 0;
		drawscreen(mute, 0);
		status.screencalc = tmpscreencalc;
		status.drawallways[0] = mute;
	}
	
	else
	{
		status.mute = 0;
		setmute(0);
		clearscreen(mute);
		if(status.drawallwaysbg[0] != NULL)
		{
			restorescreen(status.drawallwaysbg[0], mute);
			status.drawallwaysbg[0] = NULL;
		}
		status.drawallways[0] = NULL;
		if(flag == 0)
			blitfb();
	}
}

#endif
