#ifndef TEXTBOX_H
#define TEXTBOX_H

//flag 1: for barcode show
//flag 2: enable wrap
//flag 3: no clearscreen
int textbox(char* title, char* text, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int height, int timeout, int flag)
{
	debug(1000, "in");
	int rcret = -1, fromthread = 0;
	struct skin* messagebox = getscreen("messagebox");
	struct skin* textbox = getscreennode(messagebox, "textbox");
	struct skin *button = NULL;
	struct skin* framebuffer = getscreen("framebuffer");
	char* bg = NULL;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	textbox->aktline = 0;
	textbox->aktpage = 0;
	textbox->linecount = 0;
	textbox->pagecount = 0;
	textbox->poscount = 0;

	//show barcode for cpuid
	if(flag == 1)
	{
		struct skin* tmp = addscreennode(messagebox, NULL, NULL);
		if(tmp != NULL)
		{
			char* tmpstr = NULL;
			
			tmpstr = ostrcat("*", getcpuid(), 0, 1);
			tmpstr = ostrcat(tmpstr, "*", 1, 0);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->fontsize = 40;
			tmp->halign = MIDDLE;
			tmp->valign = CENTER;

			changefont(tmp, "free3of9x");
			tmp->width = 150;
			tmp->height = 50;
			tmp->bgcol = 0xffffff;
			tmp->fontcol = 0x000001;
			tmp->posx = 900;
			tmp->del = 1;
		}
	}

	if(flag == 2)
		textbox->wrap = YES;
	else
		textbox->wrap = NO;

	changetitle(messagebox, title);
	if(width != 0) messagebox->width = width;
	if(height != 0) messagebox->height = height;

	changetext(textbox, text);

	button = getscreennode(messagebox, "b1");
	if(b1 != NULL)
	{
		changetext(button, b1);
		button->hidden = NO;
	}
	else
		button->hidden = YES;

	button = getscreennode(messagebox, "b2");
	if(b2 != NULL)
	{
		changetext(button, b2);
		button->hidden = NO;
	}
	else
		button->hidden = YES;

	button = getscreennode(messagebox, "b3");
	if(b3 != NULL)
	{
		changetext(button, b3);
		button->hidden = NO;
	}
	else
		button->hidden = YES;

	button = getscreennode(messagebox, "b4");
	if(b4 != NULL)
	{
		changetext(button, b4);
		button->hidden = NO;
	}
	else
		button->hidden = YES;

	
	if(fromthread == 1)
	{
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(messagebox, framebuffer, 2);
		status.rcowner = messagebox;
		bg = savescreen(messagebox);
		drawscreen(messagebox, 0, 2);
	}
	else
		drawscreen(messagebox, 0, 0);

	if(fromthread != 1)
		addscreenrc(messagebox, textbox);

	while(rcret != RCTIMEOUT && rcret != rc1 && rcret != rc2 && rcret != rc3 && rcret != rc4)
			rcret = waitrc(messagebox, timeout * 1000, 0);

	if(fromthread != 1)
		delownerrc(messagebox);

	if(fromthread == 1)
	{
		clearscreennolock(messagebox);
		restorescreen(bg, messagebox);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
	}
	else if(flag != 3)
	{
		clearscreen(messagebox);
		drawscreen(skin, 0, 0);
	}

	if(flag == 1) delmarkedscreennodes(messagebox, 1);

	changetitle(messagebox, NULL);
	changetext(textbox, NULL);
	debug(1000, "out");

	if(rcret == rc1) return 1;
	else if(rcret == rc2) return 2;
	else if(rcret == rc3) return 3;
	else if(rcret == rc4) return 4;
	else return 0;
}

#endif
