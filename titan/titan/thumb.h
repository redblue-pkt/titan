#ifndef THUMB_H
#define THUMB_H

char* checkthumb(char* path, char* file)
{
	char* tmpstr = NULL;
	
	tmpstr = ostrcat(path, "/.Thumbnails/", 0, 0);
	tmpstr = ostrcat(tmpstr, file, 1, 0);
	if(file_exist(tmpstr))
		return tmpstr;
	
	free(tmpstr);
	return NULL;
}

void thumbthread(struct stimerthread* self)
{
	int channels = 0;
	unsigned long width = 0, height = 0, rowbytes = 0;
	unsigned char* buf = NULL;
	struct queue* qe = NULL;
	char* thumbfile = NULL;

	if(self == NULL) return;

	debug(307, "start thumb thread");
	status.createthumb = 1;

	while(self->aktion != STOP)
	{
		int count = 0;

		qe = getqueue(101);
		while(qe != NULL)
		{
			if(qe->data != NULL && strstr(qe->data, ".Thumbnails") == NULL)
			{
				char* picname = createpath(qe->data, qe->data1);		
				buf = loadjpg(picname, &width, &height, &rowbytes, &channels, 16);		
				free(picname); picname = NULL;

				if(buf != NULL)
				{
					thumbfile = ostrcat(thumbfile, (char*)qe->data, 1, 0);
					thumbfile = ostrcat(thumbfile, "/.Thumbnails", 1, 0);
					mkdir(thumbfile, 777);

					thumbfile = ostrcat(thumbfile, "/", 1, 0);
					thumbfile = ostrcat(thumbfile, (char*)qe->data1, 1, 0);

					debug(307, "create thumb: %s from %s/%s", thumbfile, (char*)qe->data, (char*)qe->data1);
					savejpg(thumbfile, 100, 100, buf);
				}
			}

			free(buf); buf = NULL;
			free(thumbfile); thumbfile = NULL;
			delqueue(qe, 0);
			qe = getqueue(101);
		}

		while(count < 6)
		{
			if(self->aktion == STOP) break;
			sleep(1);
			count++;
		}
	}

	status.createthumb = 0;
	debug(307, "end thumb thread");
}

#endif
