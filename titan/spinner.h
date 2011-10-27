#ifndef SPINNER_H
#define SPINNER_H

void checkspinner()
{
	debug(1000, "in");
	time_t sec = time(NULL);
	int rcret = 0;

	while(status.sec != 0 && sec - status.sec > status.spinnertime && sec - status.sec < 86400)
	{
		screenspinner();
		sec = time(NULL);

		if(status.sec != 0 && status.hangtime > 0 && sec - status.sec > status.spinnertime + status.hangtime && sec - status.sec < 86400)
		{
			status.sec = sec - status.spinnertime - 1;
			rcret = textbox(_("Message"), _("Detect hanging System !!!\n\nKILL = Red\nBOOT = Power"), _("EXIT"), getrcconfigint("rcexit", NULL), _("KILL"), getrcconfigint("rcred", NULL), _("BOOT"), getrcconfigint("rcpower", NULL), NULL, 0, 600, 400, 0, 0);
			if(rcret == 2)
				system(KILLPROG);
			if(rcret == 3)
				system(REBOOT);
		}
	}

	debug(1000, "out");
}

void screenspinner()
{
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* spinner = getscreen("spinner");
	struct skin* node = NULL;
	int count = 0, treffer = 0, tmpscreencalc = 0;
	char* bg = NULL;

	if(spinner == NULL)
	{
		usleep(status.spinnerspeed * 10000);
		return;
	}

	node = spinner->child;
	status.spinner++;

findspinner:
	while(node != NULL)
	{
		if(strncmp("spinner", node->name, 7) == 0)
			count++;
		if(count == status.spinner)
		{
			node->hidden = NO;
			treffer = 1;
		}
		else
			node->hidden = YES;

		node = node->next;
	}
	if(treffer == 0 && status.spinner != 1)
	{
		count = 0;
		status.spinner = 1;
		node = spinner->child;
		goto findspinner;
	}

	//save bg
	m_lock(&status.drawingmutex, 0);
	tmpscreencalc = status.screencalc;
	status.screencalc = 0;
	setnodeattr(spinner, framebuffer);
	bg = saverect(spinner->rposx, spinner->rposy, spinner->rwidth, spinner->rheight);

	drawscreen(spinner, 2);
	usleep(status.spinnerspeed * 10000);
	clearscreennolock(spinner);

	//restore bg
	restorerect(bg, spinner->rposx, spinner->rposy, spinner->rwidth, spinner->rheight);
	blitfb();
	status.screencalc = tmpscreencalc;
	m_unlock(&status.drawingmutex, 0);
	usleep(1000);
}

#endif
