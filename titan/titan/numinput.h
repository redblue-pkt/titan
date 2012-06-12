#ifndef NUMINPUT_H
#define NUMINPUT_H

char* numinput(char* title, char* num, char* mask, int isip)
{
	debug(1000, "in");
	int rcret = -1, fromthread = 0, height = 0;
	struct skin* numinput = getscreen("numinput");
	struct skin* input = getscreennode(numinput, "input");
	struct skin* framebuffer = getscreen("framebuffer");
	char* ret = NULL, *bg = NULL;
	char* numtmp = NULL;

	if(pthread_self() != status.mainthread)
                fromthread = 1;

	changetitle(numinput, title);
	height = numinput->height;
	if(title != NULL)
		numinput->height += numinput->fontsize + 6 + (numinput->bordersize * 2);

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
		setnodeattr(numinput, framebuffer, 2);
		status.rcowner = numinput;
		bg = savescreen(numinput);
		drawscreen(numinput, 0, 2);
	}
	else
		drawscreen(numinput, 0, 0);
	addscreenrc(numinput, input);

	while(1)
	{
		rcret = waitrc(numinput, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = ostrcat(input->input, NULL, 0, 0);
			break;
		}
	}

	delownerrc(numinput);

	if(fromthread == 1)
	{
		clearscreennolock(numinput);
		restorescreen(bg, numinput);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
	}
	else
	{
		clearscreen(numinput);
		drawscreen(skin, 0, 0);
	}

	numinput->height = height;
	debug(1000, "out");
	return ret;
}

#endif
