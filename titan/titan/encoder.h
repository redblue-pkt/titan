#ifndef ENCODER_H
#define ENCODER_H


struct dvbdev* encoderopen()
{
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(node->fd == -1 && node->type == ENCODERDEV)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		if((fd = open(node->dev, O_RDONLY)) < 0)
		{
			debug(200, "open encoder failed %s", node->dev);
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


int encoderopendirect(char* encoderdev)
{
	int fd = -1;

	if((fd = open(encoderdev, O_RDONLY)) < 0)
	{
		debug(200, "open encoder failed %s", encoderdev);
	}

	closeonexec(fd);
	return fd;
}

void encoderclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}


int encodergetdev()
{
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *encoderdev = NULL;

	encoderdev = getconfig("encoderdev", NULL);
	if(encoderdev == NULL)
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

	i = 0;
	for(y = 0; y < MAXENCODERDEV; y++)
	{
		sprintf(buf, encoderdev, y);
		fd = file_exist(buf);
		if(fd > 0)
		{
			count++;
			adddvbdev(buf, i, y, -1, ENCODERDEV, NULL, NULL, NULL, 0);
		}
	}
	free(buf);
	return count;
}

int encodernextdev()
{
	int ret = 0;
	struct dvbdev* node = encoderopen();
	if(node == NULL)
		ret = -1;
	else
	{
		ret = node->devnr;
		encoderclose(node, 0);
	}
	return ret;
}
	
int encoderset(int dev, int flag, int bitrate, int width, int height, int framerate, int interlaced, int aspectratio)
{
	char *buf = NULL, *encoderdev = NULL;
	int nextdev = dev;
	
	if (nextdev < 0)
	{ 
		nextdev = encodernextdev();
		if(nextdev < 0)
			return -1;
	}
	encoderdev = getconfig("enc_all", NULL);
	if(encoderdev == NULL)
	{
		err("NULL detect");
		return -1;
	}
	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return -1;
	}
	sprintf(buf, encoderdev, nextdev, "bitrate");
	writesysint(buf, bitrate, 1);
	sprintf(buf, encoderdev, nextdev, "width");
	writesysint(buf, width, 1);
	sprintf(buf, encoderdev, nextdev, "height");
	writesysint(buf, height, 1);
	sprintf(buf, encoderdev, nextdev, "framerate");
	writesysint(buf, framerate, 1);
	sprintf(buf, encoderdev, nextdev, "interlaced");
	writesysint(buf, interlaced, 1);
	sprintf(buf, encoderdev, nextdev, "aspectratio");
	writesysint(buf, aspectratio, 1);
	if(flag == 1)
	{
		sprintf(buf, encoderdev, nextdev, "apply");
		writesysint(buf, 1, 1);
	}
	free(buf); buf=NULL;
	return 0;
}


#endif
