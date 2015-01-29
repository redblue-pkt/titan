#ifndef STREAMINFO_H
#define STREAMINFO_H

void screenstreaminfo()
{
	int rcret = 0;
	struct skin* streaminfo = getscreen("streaminfo");
	struct skin* aspect = getscreennode(streaminfo, "aspect");
	struct skin* framerate = getscreennode(streaminfo, "framerate");
	struct skin* xres = getscreennode(streaminfo, "xres");
	struct skin* yres = getscreennode(streaminfo, "yres");
	char* tmpstr = NULL;
	unsigned long tmpnr = 0;

	while(1)
	{
		tmpstr = getdevcontent("vmpegaspectdev");
		if(tmpstr != NULL)
		{
#ifdef MIPSEL
            if(ostrncmp(tmpstr, "0", 1) == 1)
#else
			if(ostrncmp(tmpstr, "0", 1) == 0)
#endif
				changetext(aspect, "4:3");
			else
				changetext(aspect, "16:9");
		}
		free(tmpstr); tmpstr = NULL;

		tmpstr = getdevcontent("vmpegframeratedev");
		if(tmpstr != NULL)
		{
			tmpnr = strtol(tmpstr, 0, 16);
			free(tmpstr); tmpstr = NULL;
			tmpstr = olutoa(tmpnr);
			changetext(framerate, tmpstr);
		}
		free(tmpstr); tmpstr = NULL;

		tmpstr = getdevcontent("vmpegxresdev");
		if(tmpstr != NULL)
		{
			tmpnr = strtol(tmpstr, 0, 16);
			free(tmpstr); tmpstr = NULL;
			tmpstr = olutoa(tmpnr);
			changetext(xres, tmpstr);
		}
		free(tmpstr); tmpstr = NULL;

		tmpstr = getdevcontent("vmpegyresdev");
		if(tmpstr != NULL)
		{
			tmpnr = strtol(tmpstr, 0, 16);
			free(tmpstr); tmpstr = NULL;
			tmpstr = olutoa(tmpnr);
			changetext(yres, tmpstr);
		}
		free(tmpstr); tmpstr = NULL;

		drawscreen(streaminfo, 0, 0);

		rcret = waitrc(streaminfo, 1000, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	clearscreen(streaminfo);
}

#endif
