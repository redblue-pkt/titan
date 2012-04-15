#ifndef THUMB_H
#define THUMB_H

char* checkthumb(char* file)
{
	return ostrcat(file, ".thumb.jpg", 0, 0);
}

void thumbthread(struct stimerthread* self)
{
	int width = 0, height = 0;
	unsigned char* buf = NULL;
	struct queue* qe = NULL;
	char* thumbfile = NULL, *tmpstr = NULL;

	if(self == NULL) return;

	debug(307, "start thumb thread");

	while(self->aktion != STOP)
	{
		sleep 5;

		qe = getqueue(101);
		while(qe != NULL)
		{
			buf = loadjpg((char*)qe->data, int *width, int *height, 16);
			if(buf != NULL)
			{
				buf = scale(buf, width, height, 3, 100, 100, 1);
				if(buf != NULL)
				{
					tmpstr = ostrcat((char*)qe->data, NULL, 0, 0);
					tmpstr = dirname(tmpstr);
					tmpstr = ostrcat(tmpstr, "/.Thumbnails", 1, 0);
					mkdir(tmpstr, 777);
					free(tmpstr); tmpstr = NULL;

					thumbfile = ostrcat((char*)qe->data, ".thumb.jpg", 1, 0);
					debug(307, "create thumb: %s from %s", thumbfile, (char*)qe->data);
					savejpg(thumbfile, 100, 100, buf);
				}
			}

			free(buf); buf = NULL;
			free(thumbfile); thumbfile = NULL;
			delqueue(qe, 0);
			qe = getqueue(dvbnode->devnr);
		}
	}

	debug(307, "end thumb thread");
}

#endif
