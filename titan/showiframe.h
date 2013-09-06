#ifndef SHOWIFRAME_H
#define SHOWIFRAME_H

//flag 0: stop service
//flag 1: not stop service
int singlepicstart(const char *filename, int flag)
{
	int ret = 0;

	if(status.aktservice->type != STILLPIC)
	{
		if(flag == 0)
		{
			ret = servicestop(status.aktservice, 0, 1);
			if(ret == 1) return 1;
		}
	}

	int fd = open(filename, O_RDONLY);
	if(fd >= 0)
	{
		struct dvbdev* videonode = NULL;
		struct stat s;
		fstat(fd, &s);

		if(status.aktservice->type != STILLPIC)
		{
			if(status.aktservice->videodev != NULL)
			{
				videoclose(status.aktservice->videodev, -1);
				status.aktservice->videodev = NULL;
			}
			videonode = videoopen(0, 0);
			videoselectsource(videonode, VIDEO_SOURCE_MEMORY);

			videosetformat(videonode, VIDEO_FORMAT_16_9);
			videosetstreamtype(videonode, MPEGV);
			videoplay(videonode);
		}	
		else
			videonode = status.aktservice->videodev;

		if(videonode != NULL)
		{
			struct video_still_picture stillpic;
			char* iframe = NULL;

			iframe = malloc(s.st_size);
			if(iframe == NULL)
			{
				err("no mem");
				close(fd);
				return 1;
			}

			status.aktservice->type = STILLPIC;
			status.aktservice->videodev = videonode;

			TEMP_FAILURE_RETRY(read(fd, iframe, s.st_size));
			stillpic.iFrame = iframe;
			stillpic.size = s.st_size;

			videoclearbuffer(videonode);
			videostillpicture(videonode, &stillpic);

			free(iframe); iframe = NULL;
		}
		close(fd);
	}
	else
	{
		err("open %s\n", filename);
		return 1;
	}
	return 0;
}

#endif
