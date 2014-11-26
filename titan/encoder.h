#ifndef ENCODER_H
#define ENCODER_H


int encoderopendirect(char* encoderdev)
{
	int fd = -1;

	if((fd = open(encoderdev, O_RDWR)) < 0)
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

	for(y = 0; y < MAXENCODERDEV; y++)
	{
		sprintf(buf, encoderdev, y);
		fd = encoderopendirect(buf);
		if(fd >= 0)
		{
			encoderclose(NULL, fd);
			count++;
			adddvbdev(buf, i, y, -1, ENCODERDEV, NULL, NULL, NULL, 0);
		}
	}

	free(buf);
	return count;
}



#endif
