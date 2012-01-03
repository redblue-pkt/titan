#ifndef NUMINPUT_H
#define NUMINPUT_H

char* numinput(char* title, char* num, char* mask, int isip)
{
	debug(1000, "in");
	int rcret = -1, tmpscreencalc = 0, fromthread = 0, height = 0;
	struct skin* ipinput = getscreen("ipinput");
	struct skin* input = getscreennode(ipinput, "input");
	struct skin* framebuffer = getscreen("framebuffer");
	char* ret = NULL, *bg = NULL;
	char* numtmp = NULL;

	if(pthread_self() != status.mainthread)
                fromthread = 1;

	changetitle(ipinput, title);
	height = ipinput->height;
	if(title != NULL)
		ipinput->height += ipinput->fontsize + 6 + (ipinput->bordersize * 2);

	if(isip == 1)
		numtmp = fixip(num, 0);
	else
		numtmp = ostrcat(num, NULL, 0, 0);

	changemask(input, mask);
        changeinput(input, numtmp);
	free(numtmp); numtmp = NULL;

	if(fromthread == 1)
	{
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		status.screencalc = 2;
		setnodeattr(ipinput, framebuffer);
		status.screencalc = 0;
		status.rcowner = ipinput;
		bg = savescreen(ipinput);
		tmpscreencalc = status.screencalc;
		status.screencalc = 0;
		drawscreen(ipinput, 2);
	}
	else
		drawscreen(ipinput, 0);
	addscreenrc(ipinput, input);

	while(1)
	{
		rcret = waitrc(ipinput, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = ostrcat(input->input, NULL, 0, 0);
			break;
		}
	}

	delownerrc(ipinput);

	if(fromthread == 1)
	{
		clearscreennolock(ipinput);
		restorescreen(bg, ipinput);
		blitfb();
		status.screencalc = tmpscreencalc;
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
	}
	else
	{
		clearscreen(ipinput);
		drawscreen(skin, 0);
	}

	ipinput->height = height;
	debug(1000, "out");
	return ret;
}

#endif
