#ifndef SHOWIFRAME_H
#define SHOWIFRAME_H

#define VIDEO_STREAMTYPE_MPEG2 0
#define VIDEO_STREAMTYPE_MPEG4_H264 1

void singlepicthread(struct stimerthread* self, char* path, int flag)
{
	printf("singlepicthread start\n");

	if(status.singlepicthread != NULL || self == NULL)
	{
		free(path); path = NULL;
		return;
	}
	status.singlepicthreadstatus = 1;
	status.singlepicthread = self;


	int count = 0;
/*
	while(count < 2)
	{
		sleep(1);		
		count++;
	}
*/
	usleep(150000);

	showsinglepic();

	status.singlepicthread = NULL;
	status.singlepicthreadstatus = 0;
	printf("singlepicthread end\n");
}

void showsinglepic()
{
	printf("showsinglepic start\n");

	if (status.singlepicfd >= 0)
	{
		if (ioctl(status.singlepicfd, VIDEO_STOP, 0) < 0)
			printf("showsinglepic VIDEO_STOP failed: %m\n");
		if (ioctl(status.singlepicfd, VIDEO_SELECT_SOURCE, VIDEO_SOURCE_DEMUX) < 0)
				printf("showsinglepic VIDEO_SELECT_SOURCE DEMUX failed: %m\n");
		close(status.singlepicfd);
		status.singlepicfd = -1;
	}

	printf("showsinglepic end\n");
}

//flag 0: stop service
//flag 1: not stop service
int singlepicstart(const char *filename, int flag)
{
	int ret = 0;

	if(checkchipset("3798MV200") == 0)
	{
		if(status.aktservice->type != STILLPIC)
		{
			if(flag == 0)
			{
				ret = servicestop(status.aktservice, 0, 1);
				if(ret == 1) return 1;
			}
		}
	}

	int fd = open(filename, O_RDONLY);
	if(fd >= 0)
	{
		struct dvbdev* videonode = NULL;
		struct stat s;
		fstat(fd, &s);

		if(checkchipset("3798MV200") == 0)
		{
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
		}

		if(checkchipset("3798MV200") == 1 || videonode != NULL)
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

			if(checkchipset("3798MV200") == 0)
			{
				status.aktservice->type = STILLPIC;
				status.aktservice->videodev = videonode;
			}

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
			if(checkchipset("3798MV200") == 1)
			{
				int streamtype;
				if (iframe[0] == 0x00 && iframe[1] == 0x00 && iframe[2] == 0x00 && iframe[3] == 0x01 && (iframe[4] & 0x0f) == 0x07)
					streamtype = VIDEO_STREAMTYPE_MPEG4_H264;
				else
					streamtype = VIDEO_STREAMTYPE_MPEG2;

				if (ioctl(status.singlepicfd, VIDEO_SELECT_SOURCE, VIDEO_SOURCE_MEMORY) < 0)
					printf("[eTSMPEGDecoder] VIDEO_SELECT_SOURCE MEMORY failed: %m\n");
				if (ioctl(status.singlepicfd, VIDEO_SET_STREAMTYPE, streamtype) < 0)
					printf("[eTSMPEGDecoder] VIDEO_SET_STREAMTYPE failed: %m\n");
				if (ioctl(status.singlepicfd, VIDEO_PLAY) < 0)
					printf("[eTSMPEGDecoder] VIDEO_PLAY failed: %m\n");
				if (ioctl(status.singlepicfd, VIDEO_CONTINUE) < 0)
					printf("[eTSMPEGDecoder] VIDEO_CONTINUE: %m\n");
				if (ioctl(status.singlepicfd, VIDEO_CLEAR_BUFFER) < 0)
					printf("[eTSMPEGDecoder] VIDEO_CLEAR_BUFFER: %m\n");

				while(pos <= (s.st_size - 4) && !(seq_end_avail = (!iframe[pos] && !iframe[pos + 1] && iframe[pos + 2] == 1 && iframe[pos + 3] == 0xB7)))
					++pos;
			
				if((iframe[3] >> 4) != 0xE) // no pes header
					write(status.singlepicfd, pes_header, sizeof(pes_header));
				else
					iframe[4] = iframe[5] = 0x00;
			
				write(status.singlepicfd, iframe, s.st_size);
			
				if(!seq_end_avail)
					write(status.singlepicfd, seq_end, sizeof(seq_end));
			
				write(status.singlepicfd, stuffing, 8192);
				addtimer(&singlepicthread, START, 1000, 1, (void*)ostrcat(filename, NULL, 0, 0), (void*)flag, NULL);
			}
			else
			{
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
			}
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
