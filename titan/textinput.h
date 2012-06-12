#ifndef TEXTINPUT_H
#define TEXTINPUT_H

char* textinput(char* title, char* text)
{
	debug(1000, "in");
	int rcret = -1, fromthread = 0, height = 0;
	struct skin* textinput = getscreen("textinput");
	struct skin* input = getscreennode(textinput, "input");
	struct skin* framebuffer = getscreen("framebuffer");
	char* ret = NULL, *bg = NULL;

	if(pthread_self() != status.mainthread)
	fromthread = 1;

	changetitle(textinput, title);
	height = textinput->height;
	if(title != NULL)
		textinput->height += textinput->fontsize + 6 + (textinput->bordersize * 2);
	changeinput(input, text);

	if(fromthread == 1)
	{
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(textinput, framebuffer, 2);
		status.rcowner = textinput;
		bg = savescreen(textinput);
		drawscreen(textinput, 0, 2);
	}
	else
		drawscreen(textinput, 0, 0);
	addscreenrc(textinput, input);

	while(1)
	{
		rcret = waitrc(textinput, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = ostrcat(input->input, NULL, 0, 0);
			break;
		}
	}

	delownerrc(textinput);

	if(fromthread == 1)
	{
		clearscreennolock(textinput);
		restorescreen(bg, textinput);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
	}
	else
	{
		clearscreen(textinput);
		drawscreen(skin, 0, 0);
	}

	textinput->height = height;
	debug(1000, "out");
	return ret;
}

#endif
