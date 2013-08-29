#ifndef AUDIODEV_H
#define AUDIODEV_H

struct dvbdev* audioopen(int adapter)
{
	STARTFUNC
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(node->fd == -1 && node->type == AUDIODEV && node->adapter == adapter)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		if((fd = open(node->dev, O_RDWR)) < 0)
		{
			debug(200, "open audio failed %s", node->dev);
			node = NULL;
		}
		else
		{
			node->fd = fd;
			closeonexec(fd);
		}
	}

	return node;
}

int audioopendirect(char *audiodev)
{
	STARTFUNC
	int fd = -1;

	if((fd = open(audiodev, O_RDWR)) < 0)
	{
		debug(200, "open audio failed %s", audiodev);
	}

	closeonexec(fd);
	return fd;
}

void audioclose(struct dvbdev* node, int fd)
{
	STARTFUNC
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int audiostop(struct dvbdev* node)
{
	STARTFUNC
	if(node == NULL)
	{
		debug(200, "NULL detect");
		return 1;
	}

	debug(200, "AUDIO_STOP");
	if(ioctl(node->fd, AUDIO_STOP) < 0)
	{
		perr("AUDIO_STOP");
		return 1;
	}
	
	return 0;
}

int audioplay(struct dvbdev* node)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_PLAY");
	if(ioctl(node->fd, AUDIO_PLAY) < 0)
	{
		perr("AUDIO_PLAY");
		return 1;
	}
	
	return 0;
}

int audiopause(struct dvbdev* node)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_PAUSE");
	if(ioctl(node->fd, AUDIO_PAUSE) < 0)
	{
		perr("AUDIO_PAUSE");
		return 1;
	}
	
	return 0;
}

int audiogetpts(struct dvbdev* node, uint64_t* pts)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_GET_PTS");
	if(ioctl(node->fd, AUDIO_GET_PTS, pts) < 0)
	{
		perr("AUDIO_GET_PTS");
		return 1;
	}
	
	return 0;
}

int audioselectsource(struct dvbdev* node, audio_stream_source_t source)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_SELECT_SOURCE=%d", source);
	if(ioctl(node->fd, AUDIO_SELECT_SOURCE, source) < 0)
	{
		perr("AUDIO_SELECT_SOURCE");
		return 1;
	}
	
	return 0;
}

int audiosetmute(struct dvbdev* node, int state)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_SET_MUTE=%d", state);
	if(ioctl(node->fd, AUDIO_SET_MUTE, state) < 0)
	{
		perr("AUDIO_SET_MUTE");
		return 1;
	}
	
	return 0;
}
int audiosetavsync(struct dvbdev* node, int state)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_SET_AV_SYNC=%d", state);
	if(ioctl(node->fd, AUDIO_SET_AV_SYNC, state) < 0)
	{
		perr("AUDIO_SET_AV_SYNC");
		return 1;
	}
	
	return 0;
}

int audiosetbypassmode(struct dvbdev* node, int mode)
{
	STARTFUNC	
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_SET_BYPASS_MODE=%d", mode);
	if(ioctl(node->fd, AUDIO_SET_BYPASS_MODE, mode) < 0)
	{
		perr("AUDIO_SET_BYPASS_MODE");
		return 1;
	}

	//auto increase volume for ac3
	if(status.volautochangevalue != 0)
	{
		int aktvol = getvol();
		if(mode == AC3)
		{
			if(status.volautochange == 0)
				status.volautochange = 1;
		}
		else if(status.volautochange > 0)
			status.volautochange = 0;

		setvol(aktvol);
	}
	
	return 0;
}

int audiochannelselect(struct dvbdev* node, audio_channel_select_t channel)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_CHANNEL_SELECT=%d", channel);
	if(ioctl(node->fd, AUDIO_CHANNEL_SELECT, channel) < 0)
	{
		perr("AUDIO_CHANNEL_SELECT");
		return 1;
	}
	
	return 0;
}

int audioclearbuffer(struct dvbdev* node)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(ioctl(node->fd, AUDIO_CLEAR_BUFFER) < 0)
	{
		perr("AUDIO_CLEAR_BUFFER");
		return 1;
	}
	
	return 0;
}

int audiosetid(struct dvbdev* node, int id)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_SET_ID=%d", id);
	if(ioctl(node->fd, AUDIO_SET_ID, id) < 0)
	{
		perr("AUDIO_SET_ID");
		return 1;
	}
	
	return 0;
}

int audiosetstreamtype(struct dvbdev* node, int type)
{
	STARTFUNC
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "AUDIO_SET_STREAMTYPE=%d", type);
	if(ioctl(node->fd, AUDIO_SET_STREAMTYPE, type) < 0)
	{
		perr("AUDIO_SET_STREAMTYPE");
		return 1;
	}
	
	return 0;
}

int audiogetdev()
{
	STARTFUNC
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *audiodev = NULL;

	audiodev = getconfig("audiodev", NULL);
	if(audiodev == NULL)
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
		for(y = 0; y < MAXAUDIODEV; y++)
		{
			sprintf(buf, audiodev, i, y);
			fd = audioopendirect(buf);
			if(fd >= 0)
			{
				audioclose(NULL, fd);
				count++;
				adddvbdev(buf, i, y, -1, AUDIODEV, NULL, NULL, 0);
			}
		}
	}

	free(buf);
	return count;
}

#endif
