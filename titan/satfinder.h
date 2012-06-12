#ifndef SATFINDER_H
#define SATFINDER_H

void screensatfinder()
{
	int rcret = 0;
	struct skin* satfinder = getscreen("satfinder");

	drawscreen(satfinder, 0, 0);

	while(1)
	{
		rcret = waitrc(satfinder, 1000, 0);
		if(rcret == RCTIMEOUT)
		{
			drawscreen(satfinder, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	clearscreen(satfinder);
}

#endif
