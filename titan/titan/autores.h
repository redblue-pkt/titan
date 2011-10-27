#ifndef AUTORES_H
#define AUTORES_H

void screenautores(char* text, int timeout, int flag)
{
	debug(1000, "in");
	int rcret = -1, tmpscreencalc = 0, fromthread = 0;
	struct skin* autores = getscreen("autores");
	struct skin* framebuffer = getscreen("framebuffer");
	char* bg = NULL;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	changetext(autores, text);

	if(fromthread == 1)
	{
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		status.screencalc = 2;
		setnodeattr(autores, framebuffer);
		status.screencalc = 0;
		status.rcowner = autores;
		bg = saverect(autores->rposx, autores->rposy, autores->rwidth, autores->rheight);
		tmpscreencalc = status.screencalc;
		status.screencalc = 0;
		drawscreen(autores, 2);
	}
	else
		drawscreen(autores, 0);

	//deaktivate for test, so we can end the screen with each keypress
	//while(rcret != RCTIMEOUT && rcret != getrcconfigint("rcexit", NULL))
			rcret = waitrc(autores, timeout * 1000, 0);

	if(fromthread != 1)
		delownerrc(autores);

	if(fromthread == 1)
	{
		clearscreennolock(autores);
		restorerect(bg, autores->rposx, autores->rposy, autores->rwidth, autores->rheight);
		blitfb();
		status.screencalc = tmpscreencalc;
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 10);
		m_unlock(&status.drawingmutex, 0);
	}
	else
	{
		clearscreen(autores);
		drawscreen(skin, 0);
	}

	debug(1000, "out");
}

void autoresthreadfunc(struct stimerthread* self, char* text, int timeout)
{
	screenautores(text, timeout, 0);
}

#endif
