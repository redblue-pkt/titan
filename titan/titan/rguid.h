#ifndef RGUID_H
#define RGUID_H

void rguidgotdata(struct stimerthread* timernode, int* connfd)
{
	int ret = 0, keycode = 0;
	unsigned char* buf = NULL;

	if(timernode == NULL)
	{
		sockclose(connfd);
		return;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		sockclose(connfd);
		return;
	}
	memset(buf, 0, 6);

	debug(250, "get client data");

	status.rguidfd = *connfd;
	blitfb(0);
	while(timernode->aktion != STOP)
	{
		ret = sockread(*connfd, buf, 0, 6, 1000 * 1000, 1);
		if(ret < 0)
		{
			err("lost connection");
			break;
		}

		if(buf != NULL && buf[0] != '\0')
		{
			keycode = atoi((char*)buf);
      memset(buf, 0, 6);
			debug(250, "got keycode %d\n", keycode);
			
			if(status.standby == 0 && keycode == 98989)
			{
				int i = 0;
				
				//leave all menüs and screens
				for(i = 0; i < 10; i++)
				{
					writerc(getrcconfigint("rcexit", NULL));
					usleep(5000);
				}
				
				//send box into standby
				int tmpaktion = getconfigint("poweraktion", NULL);
				addconfigint("poweraktion", 2);
				writerc(getrcconfigint("rcpower", NULL));
				usleep(500000);
				addconfigint("poweraktion", tmpaktion);
				continue;
			}
			
			if(status.standby == 0 && keycode == getrcconfigint("rcpower", NULL))
				continue;

			writerc(keycode);
		}
	}

	status.rguidfd = -1;
	sockclose(connfd);
	free(buf); buf = NULL;
}

int rguidnewconn(int* streamfd, int* connfd)
{
	int i, fd = -1;

	fd = sockaccept(streamfd, 1);
	if(fd < 0) return -1;

	for(i = 0; i < MAXRGUIDCONN; i++)
	{
		if(connfd[i] < 0)
		{
			connfd[i] = fd;
			debug(250, "accept rguid connection fd=%d", fd);
			fd = -1;
			break;
		}
	}

	if(fd != -1)
	{
		debug(250, "all connections in use");
		sockclose(&fd);
		return -1;
	}

	return 0;
}

void rguidthreadfunc(struct stimerthread* timernode)
{
	struct timeval timeout;
	fd_set rfds;
	int i, ret = 0, streamfd = -1, connfd[MAXRGUIDCONN], maxfd = -1;

	debug(250, "Starting rguid thread");

	for(i = 0; i < MAXRGUIDCONN; i++)
		connfd[i] = -1;

	while(timernode->aktion != STOP)
	{
		if(streamfd < 0)
		{
			sockportcreate(&streamfd, getconfigint("rguidport", NULL), MAXHTTPDCONN);
			if(streamfd < 0) break;
			maxfd = streamfd;

			//set nonblocking
			fcntl(streamfd, F_SETFL, fcntl(streamfd, F_GETFL) | O_NONBLOCK);
		}

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&rfds);
		FD_SET(streamfd, &rfds);

		for(i = 0; i < MAXRGUIDCONN; i++)
		{
			if(connfd[i] > -1)
			{
				FD_SET(connfd[i], &rfds);
				if(connfd[i] > maxfd) maxfd = connfd[i];
			}
		}

		ret = TEMP_FAILURE_RETRY(select(maxfd + 1, &rfds , NULL, NULL, &timeout));

		if(ret < 0)
		{
			perr("rguid listen socket fd=%d", maxfd);
			sleep(1);
			continue;
		}
		if(ret == 0) continue; //timeout

		if(FD_ISSET(streamfd, &rfds))
		{
			ret = rguidnewconn(&streamfd, connfd);
			if(ret < 0) continue;
		}

		for(i = 0; i < MAXRGUIDCONN; i++)
		{
			if(connfd[i] > -1 && FD_ISSET(connfd[i], &rfds))
				rguidgotdata(timernode, &connfd[i]);
		}
	}

	debug(250, "Stopping rguid thread");
	sockclose(&streamfd);
	for(i = 0; i < MAXRGUIDCONN; i++)
		sockclose(&connfd[i]);

	return;
}

#endif
