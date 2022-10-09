#ifndef STREAM_H
#define STREAM_H

void screenstreaming()
{
	int count = 0;
	char* tmpstr = NULL;
	struct service* servicenode = service;
	struct menulist* mlist = NULL, *mbox = NULL;

	while(servicenode != NULL)
	{
		if(servicenode->type == RECORDSTREAM && servicenode->recname != NULL)
		{
			tmpstr = ostrcat(tmpstr, _("stop"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, servicenode->recname, 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			if(servicenode->channel != NULL && servicenode->channel->name != NULL)
				tmpstr = ostrcat(tmpstr, servicenode->channel->name, 1, 0);
			else
				tmpstr = ostrcat(tmpstr, _("unknown"), 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);

			count++;
			addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			free(tmpstr); tmpstr = NULL;
		}
		servicenode = servicenode->next;
	}

	if(count == 0)
		textbox(_("Message"), _("No Live Stream running"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	else
	{
		mbox = menulistbox(mlist, "menulist", _("Streaming"), _("Choose your Streaming link from the following list"), NULL, NULL, 0, 0);
		if(mbox != NULL && ostrstr(mbox->name, _("stop")) == mbox->name)
		{
			servicenode = getrecordbyname(mbox->name, RECORDSTREAM);
			if(servicenode != NULL)
				servicenode->recendtime = 1;
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
}

void streamthreadfunc(struct stimerthread* timernode)
{
	struct timeval timeout;
	fd_set rfds;
	int ret = 0, streamfd = -1, connfd = -1, filefd = -1, serviceid = 0;
	uint64_t transponderid = 0;
	unsigned char* buf = NULL;
	char* tmpstr = NULL, *filename = NULL;
	struct channel* chnode = NULL;

	debug(250, "Starting Stream thread");

	while(timernode->aktion != STOP)
	{
		if(streamfd < 0)
		{
			sockportcreate(&streamfd, getconfigint("stream_port", NULL), 5);
			if(streamfd < 0) break;

			//set nonblocking
			fcntl(streamfd, F_SETFL, fcntl(streamfd, F_GETFL) | O_NONBLOCK);
		}

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(streamfd, &rfds);

		ret = TEMP_FAILURE_RETRY(select(streamfd + 1, &rfds , NULL, NULL, &timeout));

		if(ret < 0)
		{
			perr("stream listen socket fd=%d", streamfd);
			sleep(1);
			continue;
		}
		if(ret == 0) continue; //timeout

		if(FD_ISSET(streamfd, &rfds))
		{
			connfd = sockaccept(&streamfd, 1);
			if(connfd < 0) continue;
			debug(250, "accept stream connection connfd=%d", connfd);

			buf = calloc(1, MINMALLOC);
			if(buf == NULL)
			{
				err("no mem");
				sockclose(&connfd);
				continue;
			}

			debug(250, "get client data");

			//read one line
			unsigned char* pbuf = buf;
			while(pbuf - buf < MINMALLOC)
			{
				unsigned char c;

				ret = sockreceive(&connfd, &c, 1, 5000 * 1000);
				if(ret != 0)
				{
					err("no client data in buffer");
					break;
				}

				*pbuf = c;
				if(c == '\n') break;
				pbuf++;
			}

			if(buf != NULL)
				tmpstr = ostrstr((char*)buf, "GET /");

			if(tmpstr != NULL)
			{
				tmpstr += 5;

				filename = malloc(512);
				if(filename == NULL)
				{
					err("no mem");
					sockclose(&connfd);
					free(buf); buf = NULL;
					tmpstr = NULL;
					continue;
				}
				memset(filename, 0, 512);

				ret = sscanf(tmpstr, "%d,%llu,%s", &serviceid, &transponderid, filename);
				if(ret < 2)
					ret = sscanf(tmpstr, "%d%%2c%llu%%2c%s", &serviceid, &transponderid, filename);
				if(ret == 2 || ret == 3)
				{
					if(ret == 2)
					{
						debug(250, "stream serviceid = %d, transponderid = %llu", serviceid, transponderid);
						chnode = getchannel(serviceid, transponderid);

						if(status.lastservice->channel != chnode && getconfigint("streamzapping", NULL) == 1)
							servicecheckret(servicestart(chnode, NULL, NULL, 5), 0);
#ifndef MIPSEL
						ret = recordstart(chnode, -1, connfd, RECSTREAM, 0, NULL);
#else
						if(status.webencode == 0)
							ret = recordstart(chnode, -1, connfd, RECSTREAM, 0, NULL);
						else
						{
								status.webencode = 0;
								ret = recordstartencode(chnode, -1, connfd, RECSTREAMENC, 0, NULL,	0, 0, 0, 0, 0, 0);
						}
#endif
					}
					if(ret == 3)
					{
						htmldecode(filename, filename);
						debug(250, "stream filename=%s", filename);
						if(filename != NULL && ostrstr(filename, getconfig("rec_streampath", NULL)) != filename)
						{
							filename = ostrcat("/", filename, 0, 1);
							filename = ostrcat(getconfig("rec_streampath", NULL), filename, 0, 1);
						}
						filefd = open(filename, O_RDONLY | O_LARGEFILE);
						if(filefd > -1)
							ret = recordstart(NULL, filefd, connfd, RECSTREAM, 0, NULL);
						else
						{
							perr("open stream filename=%s", filename);
							socksend(&connfd, (unsigned char*)"HTTP/1.1 500 \"open file\"\r\n\r\n", 28, 5000 * 1000);
							sockclose(&connfd);
							free(buf); buf = NULL;
							free(filename); filename = NULL;
							tmpstr = NULL;
							continue;
						}
					}
					if(ret != 0)
					{
						char* retstr = recordcheckret(NULL, ret, 12);
						retstr = ostrcat("-", retstr, 0, 1);
						retstr = ostrcat(oitoa(ret), retstr, 1, 1);
						retstr = ostrcat("HTTP/1.1 500 \"", retstr, 0, 1);
						retstr = ostrcat(retstr, "\"\r\n\r\n", 1, 0);
						socksend(&connfd, (unsigned char*)retstr, strlen(retstr), 5000 * 1000);
						free(retstr); retstr = NULL;
						sockclose(&connfd);
						free(buf); buf = NULL;
						free(filename); filename = NULL;
						tmpstr = NULL;
						continue;
					}
					debug(250, "sende OK response to client");
					ret = socksend(&connfd, (unsigned char*)"HTTP/1.1 200 OK\r\nServer: stream (ts)\r\n\r\n", 40, 5000 * 1000);
					if(ret != 0)
					{
						sockclose(&connfd);
						free(buf); buf = NULL;
						free(filename); filename = NULL;
						tmpstr = NULL;
						continue;
					}
				}
				else
				{
					err("bad response from client");
					sockclose(&connfd);
					free(buf); buf = NULL;
					free(filename); filename = NULL;
					tmpstr = NULL;
					continue;
				}
			}
			else
			{
				err("bad response from client");
				sockclose(&connfd);
				free(buf); buf = NULL;
				tmpstr = NULL;
				continue;
			}

			free(buf); buf = NULL;
			free(filename); filename = NULL;
			tmpstr = NULL;
		}
	}
	debug(250, "Stopping stream thread");
	sockclose(&streamfd);
	return;
}

#endif
