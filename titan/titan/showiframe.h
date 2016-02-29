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

			if(checkbox("VUSOLO2") == 1) //fixt mvi background on tithek
			{
				videocontinue(videonode);
				videoclearbuffer(videonode);
			}
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

#ifdef MIPSEL
			int seq_end_avail = 0;
			size_t pos = 0;
			unsigned char pes_header[] = {0x0, 0x0, 0x1, 0xe0, 0x00, 0x00, 0x80, 0x80, 0x5, 0x21, 0x0, 0x1, 0x0, 0x1};
			unsigned char seq_end[] = {0x00, 0x00, 0x01, 0xB7};
			unsigned char stuffing[8192];

			memset(stuffing, 0, 8192);

			while(pos <= (s.st_size - 4) && !(seq_end_avail = (!iframe[pos] && !iframe[pos + 1] && iframe[pos + 2] == 1 && iframe[pos + 3] == 0xB7)))
				++pos;
		
			if((iframe[3] >> 4) != 0xE) // no pes header
				write(videonode->fd, pes_header, sizeof(pes_header));
			else
				iframe[4] = iframe[5] = 0x00;
		
			write(videonode->fd, iframe, s.st_size);
		
			if(!seq_end_avail)
				write(videonode->fd, seq_end, sizeof(seq_end));
		
			write(videonode->fd, stuffing, 8192);
#else
			videoclearbuffer(videonode);
			videostillpicture(videonode, &stillpic);
#endif

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
