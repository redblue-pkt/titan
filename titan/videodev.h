#ifndef VIDEODEV_H
#define VIDEODEV_H

#ifndef VIDEO_SET_ENCODING
#define VIDEO_SET_ENCODING              _IO('o',  81)
#endif

// flag 1 = dont close
// flag 0 = close
struct dvbdev* videoopen(int adapter, int devnr)
{
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(devnr > -1 && devnr != node->devnr)
		{
			node = node->next;
			continue;
		}
		if(node->fd == -1 && node->type == VIDEODEV && node->adapter == adapter)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		printf("[videodev] open devnr: %i\n");

		if(checkchipset("3798MV200") == 1)
		{
			if((fd = open(node->dev, O_WRONLY)) < 0)
			{
				debug(200, "open video failed %s", node->dev);
				node = NULL;
			}
			else
			{
				node->fd = fd;
	
				closeonexec(fd);
			}
		}
		else
		{
			if((fd = open(node->dev, O_RDWR)) < 0)
			{
				debug(200, "open video failed %s", node->dev);
				node = NULL;
			}
			else
			{
				node->fd = fd;
	
				closeonexec(fd);
			}
		}
	}

	return node;
}

int videoopendirect(char* videodev)
{
	int fd = -1;

	if((fd = open(videodev, O_RDWR)) < 0)
	{
		debug(200, "open video failed %s", videodev);
	}

	closeonexec(fd);
	return fd;
}

void videoclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int videostop(struct dvbdev* node, int clearscreen)
{
	if(node == NULL)
		return 1;

	debug(200, "VIDEO_STOP");
	if(ioctl(node->fd, VIDEO_STOP, clearscreen) < 0)
	{
		perr("VIDEO_STOP");
		return 1;
	}
	
	return 0;
}

int videoplay(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_PLAY");
	if(ioctl(node->fd, VIDEO_PLAY) < 0)
	{
		perr("VIDEO_PLAY");
		return 1;
	}
	
	return 0;
}

int videofreeze(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(ioctl(node->fd, VIDEO_FREEZE) < 0)
	{
		perr("VIDEO_FREEZE");
		return 1;
	}
	
	return 0;
}

int videocontinue(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(ioctl(node->fd, VIDEO_CONTINUE) < 0)
	{
		perr("VIDEO_CONTINUE");
		return 1;
	}
	
	return 0;
}

int videogetpts(struct dvbdev* node, uint64_t* pts)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_GET_PTS");
	if(ioctl(node->fd, VIDEO_GET_PTS, pts) < 0)
	{
		perr("VIDEO_GET_PTS");
		return 1;
	}

	return 0;
}

int videoselectsource(struct dvbdev* node, video_stream_source_t source)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SELECT_SOURCE=%d", source);
	if(ioctl(node->fd, VIDEO_SELECT_SOURCE, source) < 0)
	{
		perr("VIDEO_SELECT_SOURCE");
		return 1;
	}
	
	return 0;
}

int videosetblank(struct dvbdev* node, int clearscreen)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_BLANK=%d", clearscreen);
	if(ioctl(node->fd, VIDEO_SET_BLANK, clearscreen) < 0)
	{
		perr("VIDEO_SETBLANK");
		return 1;
	}
	
	return 0;
}

/*
int videosetdisplayformat(struct dvbdev* node, video_display_format_t format)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_DISPLAY_FORMAT=%d", format);
	if(ioctl(node->fd, VIDEO_SET_DISPLAY_FORMAT, format) < 0)
	{
		perr("VIDEO_SET_DISPLAY_FORMAT");
		return 1;
	}
	
	return 0;
}
*/

int videofastforward(struct dvbdev* node, int frames)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_FAST_FORWARD=%d", frames);
	if(ioctl(node->fd, VIDEO_FAST_FORWARD, frames) < 0)
	{
		perr("video VIDEO_FAST_FORWARD");
		return 1;
	}
	
	return 0;
}

int videoslowmotion(struct dvbdev* node, int frames)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SLOWMOTION=%d", frames);
	if(ioctl(node->fd, VIDEO_SLOWMOTION, frames) < 0)
	{
		perr("video VIDEO_SLOWMOTION");
		return 1;
	}
	
	return 0;
}

int videosetid(struct dvbdev* node, int id)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_ID=%d", id);
	if(ioctl(node->fd, VIDEO_SET_ID, id) < 0)
	{
		perr("video VIDEO_SET_ID");
		return 1;
	}
	
	return 0;
}

int videoclearbuffer(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(ioctl(node->fd, VIDEO_CLEAR_BUFFER) < 0)
	{
		perr("video VIDEO_CLEAR_BUFFER");
		return 1;
	}
	
	return 0;
}

int videostillpicture(struct dvbdev* node, struct video_still_picture* stillpic)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(ioctl(node->fd, VIDEO_STILLPICTURE, stillpic) < 0)
	{
		perr("video VIDEO_STILLPICTURE");
		return 1;
	}
	
	return 0;
}

int videosetstreamtype(struct dvbdev* node, int type)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_STREAMTYPE=%d", type);
	if(ioctl(node->fd, VIDEO_SET_STREAMTYPE, type) < 0)
	{
		perr("video VIDEO_SET_STREAMTYPE");
		return 1;
	}
	
	return 0;
}

int videosetencoding(struct dvbdev* node, int type)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_ENCODING=%d", type);
	if(ioctl(node->fd, VIDEO_SET_ENCODING, type) < 0)
	{
		perr("video VIDEO_SET_ENCODING");
		return 1;
	}
	
	return 0;
}

int videosetformat(struct dvbdev* node, video_format_t format)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_FORMAT=%d", format);
	if(ioctl(node->fd, VIDEO_SET_FORMAT, format) < 0)
	{
		perr("video VIDEO_SET_FORMAT");
		return 1;
	}
	
	return 0;
}

int videosetsystem(struct dvbdev* node, video_system_t system)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "VIDEO_SET_SYSTEM=%d", system);
	if(ioctl(node->fd, VIDEO_SET_SYSTEM, system) < 0)
	{
		perr("video VIDEO_SET_SYSTEM");
		return 1;
	}
	
	return 0;
}

int videogetdev()
{
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *videodev = NULL;

	videodev = getconfig("videodev", NULL);
	if(videodev == NULL)
	{
		err("NULL detect");
		return count;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return count;
	}

	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		for(y = 0; y < MAXVIDEODEV; y++)
		{
			sprintf(buf, videodev, i, y);
			fd = videoopendirect(buf);
			if(fd >= 0)
			{
				videoclose(NULL, fd);
				count++;
				adddvbdev(buf, i, y, -1, VIDEODEV, NULL, NULL, NULL, 0);
			}
		}
	}

	free(buf);
	return count;
}

int videoreadqwidth(struct dvbdev* node)
{
	if(node == NULL) return 1;

	if(ioctl(node->fd, VIDEO_GET_SIZE, &status.videosize) < 0)
	{
		perr("video VIDEO_GET_SIZE");
		return 1;
	}

	//status.videosize.w;
	//status.videosize.h;
	//status.videosize.aspect_ratio == 0 ? 2 : 3;  // convert dvb api to etsi

	return 0;
}

//flag = 0 --> start at PES
//flag = 1 --> start immediately
int videoMakePES(char* buf, int len, int pid, int tssize, int flag)
{
	int adaptation = 0;
	int payload = 0;
	int pes = 0;
  int pesfound = 0;
  int posbufaus = 0;
  int i;
  int tspid = 0;
  
  if(flag == 1)
		pesfound = 1;

	for(i = 0; i <= len-tssize; i = i + tssize)
	{
		payload = 0;
		
		tspid = (buf[i+1] & 0x1F) << 8;
		tspid = tspid + (buf[i+2] & 0xFF);
		pes = buf[i+1] & 0x40;

		//video pid?
		if(tspid == pid)
		{	
			adaptation = buf[i+3] & 0x30;
			//payload only? 
			if(adaptation == 16)
			{
				payload = 4;
			}
			//adaptation field only?
			if(adaptation == 32)
			{
				//printf("adaptation field only\n");
			}
	  	//adaptation field and payload?
	  	if(adaptation == 48)
	  	{
				payload = buf[i+4] & 0xFF;
				payload = payload + 5;
			}
			if(payload != 0)
			{
				//start of PES data?
				if(pes == 64)
				{
					pesfound = 1;
				}
				if(pesfound == 1)
				{
					memcpy(buf+posbufaus,buf+i+payload, tssize - payload);
					posbufaus = posbufaus + tssize - payload;

				}
		  } 
		}
	}
	return posbufaus;
}

#endif
