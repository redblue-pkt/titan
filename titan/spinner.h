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
			rcret = textbox(_("Message"), _("Detect hanging System !!!\n\nKILL = Red\nBOOT = Power"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), _("KILL"), getrcconfigint("rcred", NULL), _("BOOT"), getrcconfigint("rcgreen", NULL), 600, 400, 0, 0);
			if(rcret == 3)
				system(KILLPROG);
			if(rcret == 4)
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
	int count = 0, treffer = 0;
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
	setnodeattr(spinner, framebuffer, 0);
	bg = savescreen(spinner);

	drawscreen(spinner, 0, 2);
	usleep(status.spinnerspeed * 10000);
	clearscreennolock(spinner);

	//restore bg
	restorescreen(bg, spinner);
	blitfb(0);
	m_unlock(&status.drawingmutex, 0);
	usleep(1000);
}

#endif
