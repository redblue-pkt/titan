#ifndef VIDEODEV_H
#define VIDEODEV_H

struct dvbdev* videoopen(int adapter)
{
	debug(1000, "in");
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(node->fd == -1 && node->type == VIDEODEV && node->adapter == adapter)
			break;
		node = node->next;
	}

	if(node != NULL)
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

	debug(1000, "out");
	return node;
}

int videoopendirect(char* videodev)
{
	debug(1000, "in");
	int fd = -1;

	if((fd = open(videodev, O_RDWR)) < 0)
	{
		debug(200, "open video failed %s", videodev);
	}

	closeonexec(fd);
	debug(1000, "out");
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
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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

int videosetformat(struct dvbdev* node, video_format_t format)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
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
		debug(1000, "out-> NULL detect");
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
	debug(1000, "in");
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *videodev = NULL;

	videodev = getconfig("videodev", NULL);
	if(videodev == NULL)
	{
		debug(1000, "out -> NULL detect");
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
				adddvbdev(buf, i, y, -1, VIDEODEV, NULL, NULL);
			}
		}
	}

	free(buf);
	debug(1000, "out");
	return count;
}

int videoreadqwidth(struct dvbdev* node)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

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

#endif
