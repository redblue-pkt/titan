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

#ifdef MIPSEL
	if(getconfigint("ForwardVolume", NULL) == 1)
	{
		forwardKey(getrcconfigint("rcmute", NULL));
		return;
	}
#endif
	
	if(getconfig("stepmute", NULL) == NULL)
		addconfig("stepmute", "0");
		
	if(status.mute == 0 && atoi(getconfig("stepmute", NULL)) == 1)
		setmute(2);
	else if(status.mute == 0 || status.mute == 2)
	{
		m_lock(&status.vfdmutex, 3);
		if(status.mute == 2)
			setvol(getconfigint("vol", NULL));
		setmute(1);
#ifndef MIPSEL
//#ifdef SH4
		system("amixer -c 1 set HDMI mute &");
		system("amixer -c 1 set Analog mute &");
		system("amixer -c 1 set SPDIF mute &");
		audiostop(status.aktservice->audiodev);
#endif
		setnodeattr(mute, framebuffer, 0);
		if(status.drawallwaysbg[0] != NULL)
		{
			free(status.drawallwaysbg[0]);
			status.drawallwaysbg[0] = NULL;
		}
		status.drawallwaysbg[0] = savescreen(mute);
		if(flag == 2 || flag == 3)
			drawscreen(mute, 0, 2);
		else
			drawscreen(mute, 0, 0);
		status.drawallways[0] = mute;
		m_unlock(&status.vfdmutex, 3);
	}
	else
	{
#ifndef MIPSEL
//#ifdef SH4
		audioplay(status.aktservice->audiodev);
		system("amixer -c 1 set HDMI unmute &");
		system("amixer -c 1 set Analog unmute &");
		system("amixer -c 1 set SPDIF unmute &");
#endif
		m_lock(&status.vfdmutex, 3);
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
		m_unlock(&status.vfdmutex, 3);
	}
}

#endif
