#ifndef AUTORES_H
#define AUTORES_H

void screenautores(char* text, int timeout, int flag)
{
	int rcret = -1, fromthread = 0;
	struct skin* autores = getscreen("autores");
	struct skin* framebuffer = getscreen("framebuffer");
	char* bg = NULL;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	changetext(autores, text);

	if(fromthread == 1)
	{
		delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
		delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute1", NULL), NULL, NULL);
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(autores, framebuffer, 2);
		status.rcowner = autores;
		bg = savescreen(autores);
		drawscreen(autores, 0, 2);
	}
	else
		drawscreen(autores, 0, 0);

	//deaktivate for test, so we can end the screen with each keypress
	//while(rcret != RCTIMEOUT && rcret != getrcconfigint("rcexit", NULL))
			rcret = waitrc(autores, timeout * 1000, 0);

	if(fromthread != 1)
		delownerrc(autores);

	if(fromthread == 1)
	{
		clearscreennolock(autores);
		restorescreen(bg, autores);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 10);
		m_unlock(&status.drawingmutex, 0);
		addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
		addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
		addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
		addrc(getrcconfigint("rcmute1", NULL), screenmute, NULL, NULL);
	}
	else
	{
		clearscreen(autores);
		drawscreen(skin, 0, 0);
	}

	//if(rcret != RCTIMEOUT) writerc(rcret);
}

void autoresthreadfunc(struct stimerthread* self, char* text, int timeout)
{
	screenautores(text, timeout, 0);
}

#endif
