#ifndef SHOWIFRAME_H
#define SHOWIFRAME_H

#define VIDEO_STREAMTYPE_MPEG2 0
#define VIDEO_STREAMTYPE_MPEG4_H264 1

void singlepicthread(struct stimerthread* self, struct dvbdev* node , char* filename)
{
	status.singlepicthreadstatus += 1;
	printf("singlepicthread (%d) start filename: %s\n", status.singlepicthreadstatus, filename);

	if(status.singlepicthread != NULL || self == NULL)
	{
		return;
	}
	status.singlepicthread = self;


	int count = 0;
/*
	while(count < 2)
	{
		sleep(1);		
		count++;
	}
*/
//	usleep(150000);

	showsinglepic(node, filename);

	status.singlepicthread = NULL;
	printf("singlepicthread (%d) end filename: %s\n", status.singlepicthreadstatus, filename);
}

void showsinglepic(struct dvbdev* node, char* filename)
{
	printf("showsinglepic (%d) start filename: %s\n", status.singlepicthreadstatus, filename);

	if (status.singlepicfd >= 0)
	{
		printf("showsinglepic (%d) videostop filename: %s\n", status.singlepicthreadstatus, filename);
		videostop(node, 1);
		videoselectsource(node, VIDEO_SOURCE_DEMUX);
		videoclose(node, -1);
		node = NULL;
	}

	printf("showsinglepic (%d) end filename: %s\n", status.singlepicthreadstatus, filename);
}

//flag 0: stop service
//flag 1: not stop service
int singlepicstart(const char *filename, int flag)
{
	printf("---start----------------------\n");
	printf("singlepicstart filename: %s\n", filename);

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

		if(checkchipset("3798MV200") == 1 || status.aktservice->type != STILLPIC)
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
			videocontinue(videonode);
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
			status.singlepicfd = status.aktservice->videodev->fd;

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

/*
			if(checkchipset("3798MV200") == 1)
			{
				int streamtype;
				if (iframe[0] == 0x00 && iframe[1] == 0x00 && iframe[2] == 0x00 && iframe[3] == 0x01 && (iframe[4] & 0x0f) == 0x07)
					streamtype = VIDEO_STREAMTYPE_MPEG4_H264;
				else
					streamtype = VIDEO_STREAMTYPE_MPEG2;
				videoselectsource(videonode, VIDEO_SOURCE_MEMORY);
				videosetstreamtype(videonode, streamtype);
				videosetformat(videonode, VIDEO_FORMAT_16_9);	
				videoplay(videonode);
				videocontinue(videonode);
				videoclearbuffer(videonode);
			}
*/
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
/*
			if(checkchipset("3798MV200") == 1)
				addtimer(&singlepicthread, START, 1000, 1, (void*)videonode, (void*)ostrcat(filename, NULL, 0, 0), NULL);
*/
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
	printf("---end-----------------------\n");

	return 0;
}

#endif
