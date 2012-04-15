#ifndef TRANSPONDER_H
#define TRANSPONDER_H

char* checkthump(char* file)
{
	return ostrcat(file, ".thump", 0, 0);
}

void thumpthread(struct stimerthread* self)
{
	int width = 0, height = 0;
	unsigned char* buf = NULL;
	struct queue* qe = NULL;
	char* thumpfile = NULL;

	if(self == NULL) return;

	debug(307, "start thump thread");

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
					debug(307, "create thump: %s from %s", thumpfile, (char*)qe->data);
					savejpg(thumpfile, 100, 100, buf);
				}
			}

			free(buf); buf = NULL;
			free(thumpfile); thumpfile = NULL;
			delqueue(qe, 0);
			qe = getqueue(dvbnode->devnr);
		}
	}

	debug(307, "end thump thread");
}

#endif
