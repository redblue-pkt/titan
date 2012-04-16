#ifndef THUMB_H
#define THUMB_H

char* checkthumb(char* path, char* file)
{
	char* tmpstr = NULL;
	
	tmpstr = ostrcat(createpath(path, "/.Thumbnails/"), file, 1, 0);
	if(file_exist(tmpstr))
		return tmpstr;
	
	return NULL;
}

void thumbthread(struct stimerthread* self)
{
	int width = 0, height = 0;
	unsigned char* buf = NULL;
	struct queue* qe = NULL;
	char* thumbfile = NULL;

	if(self == NULL) return;

	debug(307, "start thumb thread");

	while(self->aktion != STOP)
	{
		sleep 5;

		qe = getqueue(101);
		while(qe != NULL)
		{
			buf = loadjpg((char*)qe->data, &width, &height, 16);
			if(buf != NULL)
			{
				buf = scale(buf, width, height, 3, 100, 100, 1);
				if(buf != NULL)
				{
					thumbfile = ostrcat(thumbfile, (char*)qe->data, 1, 0);
					thumbfile = ostrcat(thumbfile, "/.Thumbnails", 1, 0);
					mkdir(thumbfile, 777);
					
					thumbfile = ostrcat(thumbfile, "/", 1, 0);
					thumbfile = ostrcat(thumbfile, (char*)qe->data1, 1, 0);

					debug(307, "create thumb: %s from %s", thumbfile, (char*)qe->data);
					savejpg(thumbfile, 100, 100, buf);
				}
			}

			free(buf); buf = NULL;
			free(thumbfile); thumbfile = NULL;
			delqueue(qe, 0);
			qe = getqueue(101);
		}
	}

	debug(307, "end thumb thread");
}

#endif
