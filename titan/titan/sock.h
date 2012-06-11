#ifndef SOCK_H
#define SOCK_H

int sockwrite(int fd, unsigned char* buf, int count, int tout)
{
	struct timeval timeout;
	int ret = 0, usec = 0, sec = 0, tmpcount = count;
	unsigned char* buffer = NULL;

	if(fd < 0) return -1;

	if(tout == -1) tout = 3000 * 1000;
	usec = tout % 1000000;
	sec = (tout - usec) / 1000000;

	fd_set wfds;

	while (tmpcount > 0)
	{
		buffer = buf + (count - tmpcount);
		ret = send(fd, buffer, tmpcount, MSG_DONTWAIT | MSG_NOSIGNAL);
		if(ret < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				FD_ZERO(&wfds);
				FD_SET(fd, &wfds);
			
				timeout.tv_sec = sec;
				timeout.tv_usec = usec;
			
				ret = TEMP_FAILURE_RETRY(select(fd + 1, NULL, &wfds, NULL, &timeout));
			}
			
			if(ret == 0)
			{
				perr("sock send timed out fd=%d", fd);
				return -1;
			}
			if(ret < 0)
			{
				perr("sock send data fd=%d", fd);
				return -1;
			}
		}
		else
			tmpcount -= ret;
	}

	return count;
}

//flag 0: normal
//flag 1: from check
int sockread(int fd, unsigned char *buf, int pos, int count, int tout, int flag)
{
	struct timeval timeout;
	int ret = 0, usec = 0, sec = 0, tmpcount = count;
	unsigned char* buffer = NULL;

	if(fd < 0) return -1;

	if(tout == -1) tout = 3000 * 1000;
	usec = tout % 1000000;
	sec = (tout - usec) / 1000000;

	fd_set rfds;

	while(tmpcount > 0)
	{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		
		timeout.tv_sec = sec;
		timeout.tv_usec = usec;
		
		ret = TEMP_FAILURE_RETRY(select(fd + 1, &rfds , NULL, NULL, &timeout));
			
		if(ret == 1)
		{
			buffer = buf + (count - tmpcount);
retry:
			ret = recv(fd, buffer, tmpcount, MSG_DONTWAIT | MSG_NOSIGNAL);
			if(ret == 0) //connection closed
			{
				if(count - tmpcount != 0) return count - tmpcount;
				err("connection closed fd=%d, count=%d", fd, count - tmpcount);
				if(flag == 1) return -1;
				return 0;
			}
			if(ret < 0) //error
			{
				if(errno == EAGAIN && tout > 0) 
				{
					tout = tout - 1000;
					usleep(1000);
					goto retry;
				}
				perr("sock read data fd=%d", fd);
				return ret;
			}
			else
				tmpcount -= ret;
		}
		else if(ret == 0)
		{
			debug(250, "sock timeout fd=%d", fd);
			return ret;
		}
		else
		{
			perr("sock select fd=%d", fd);
			return ret;
		}
	}
	return count;
}

void sockclose(int *fd)
{
	if(*fd != -1)
		close(*fd);
	*fd = -1;
}

int sockcheck(int *fd)
{
	int ret = 0;
	unsigned char buf[1];

	if(*fd < 0) return 1;

	ret = sockread(*fd, buf, 0, 1, 0, 1);
	if(ret < 0)
	{
		sockclose(fd);
		return 1;
	}

	return 0;
}

int sockportopen(int *fd, char* ip, int port, int tout)
{
	int ret = 0, rest = 0, optval;
	socklen_t optlen = sizeof(optval);
	struct timeval timeout;
	struct sockaddr_in cliaddr;
	
	if(ip == NULL) return 1;
	sockclose(fd);

	memset(&cliaddr, 0, sizeof(struct sockaddr_in));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(port);

	if((*fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perr("open port socket");
		return 1;
	}

	ret = inet_pton(AF_INET, ip, (void *)(&(cliaddr.sin_addr.s_addr)));
	if(ret < 0)
	{
		sockclose(fd);
		perr("Can't set remote->sin_addr.s_addr");
		return 1;
	}
	else if(ret == 0)
	{
		sockclose(fd);
		err("%s is not a valid IP address", ip);
		return 1;
	}

	fcntl(*fd, F_SETFL, fcntl(*fd, F_GETFL) | O_NONBLOCK);

	if(tout == -1) tout = 3000 * 1000;
	rest = tout % 1000000;
	tout = (tout - rest) / 1000000;

	fd_set wfds;

	ret = connect(*fd, (struct sockaddr*) &cliaddr, sizeof(struct sockaddr));
	if(ret < 0)
	{
		if(errno == EINTR || errno == EINPROGRESS)
		{
			FD_ZERO(&wfds);
			FD_SET(*fd, &wfds);

			timeout.tv_sec = tout;
			timeout.tv_usec = rest;
			
			ret = TEMP_FAILURE_RETRY(select(*fd + 1, NULL, &wfds, NULL, &timeout));

			if(ret == 1 && FD_ISSET(*fd, &wfds))
			{
				ret = getsockopt(*fd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
				if(ret == -1)	
				{
					sockclose(fd);
					return 1;
				}
				if(optval == 0) return 0;
				sockclose(fd);
				return 1;
			}
		}
			
		if(ret <= 0)
		{
			perr("connect");
			sockclose(fd);
			return 1;
		}
	}
	return 0;
}

int sockopen(int *fd, char* sockname, int check, int tout)
{
	sockclose(fd);

	int ret = 0, rest = 0, optval;
	socklen_t optlen = sizeof(optval);
	struct timeval timeout;
	struct sockaddr_un servaddr;
	socklen_t len;

	memset(&servaddr, 0, sizeof(struct sockaddr_un));
	servaddr.sun_family = AF_UNIX;
	strncpy(servaddr.sun_path, sockname, sizeof(servaddr.sun_path) - 1);
	servaddr.sun_path[sizeof(servaddr.sun_path) - 1] = '\0';
	len = sizeof(servaddr.sun_family) + strlen(servaddr.sun_path);

	if((*fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		if(check == 0)
		{
			perr("open socket");
		}
		return 1;
	}

	fcntl(*fd, F_SETFL, fcntl(*fd, F_GETFL) | O_NONBLOCK);

	if(tout == -1) tout = 3000 * 1000;
	rest = tout % 1000000;
	tout = (tout - rest) / 1000000;

	fd_set wfds;

	ret = connect(*fd, (struct sockaddr*) &servaddr, len);
	if(ret < 0)
	{
		if(errno == EINTR || errno == EINPROGRESS)
		{
			FD_ZERO(&wfds);
			FD_SET(*fd, &wfds);

			timeout.tv_sec = tout;
			timeout.tv_usec = rest;
			
			ret = TEMP_FAILURE_RETRY(select(*fd + 1, NULL, &wfds, NULL, &timeout));

			if(ret == 1 && FD_ISSET(*fd, &wfds))
			{
				ret = getsockopt(*fd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
				if(ret == -1)	
				{
					sockclose(fd);
					return 1;
				}
				if(optval == 0) return 0;
				sockclose(fd);
				return 1;
			}
		}
			
		if(ret <= 0)
		{
			if(check == 0)
			{
				perr("connect");
			}
			sockclose(fd);
			return 1;
		}
	}
	return 0;
}

int sockportcreate(int *fd, int port, int maxconn)
{
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int sockoptactive = 1, ret = 0;

	if(!port) return 1;

	if((*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perr("open socket");
		return 1;
	}

	if(setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&sockoptactive, sizeof (int)) < 0)
	{
		perr("network port %u open: error setsockopt", port);

		sockclose(fd);
		return 1;
	}

	while(1)
	{
		ret = bind(*fd, &servaddr, sizeof(servaddr));
		if(ret < 0)
		{
			if(errno == EINTR) continue;
			perr("bind");
			sockclose(fd);
			return 1;
		}
		break;
	}

	while(1)
	{
		ret = listen(*fd, maxconn);
		if(ret < 0)
		{
			if(errno == EINTR) continue;
			perr("listen");
			sockclose(fd);
			return 1;
		}
		break;
	}

	return ret;
}

int sockcreate(int *fd, char* sockname, int maxconn)
{
	sockclose(fd);

	struct sockaddr_un servaddr;
	socklen_t len;
	int ret = 0;

	memset(&servaddr, 0, sizeof(struct sockaddr_un));
	servaddr.sun_family = AF_UNIX;
	strncpy(servaddr.sun_path, sockname, sizeof(servaddr.sun_path) - 1);
	servaddr.sun_path[sizeof(servaddr.sun_path) - 1] = '\0';
	len = sizeof(servaddr.sun_family) + strlen(servaddr.sun_path);
	unlink(servaddr.sun_path);

	if((*fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		perr("open socket");
		return 1;
	}

	while(1)
	{
		ret = bind(*fd, &servaddr, len);
		if(ret < 0)
		{
			if(errno == EINTR) continue;
			perr("bind");
			sockclose(fd);
			return 1;
		}
		break;
	}

	while(1)
	{
		ret = listen(*fd, maxconn);
		if(ret < 0)
		{
			if(errno == EINTR) continue;
			perr("listen");
			sockclose(fd);
			return 1;
		}
		break;
	}

	return ret;
}

//flag 0 = in blocking mode
//flag 1 = in non blocking mode
int sockaccept(int *fd, int flag)
{
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);
	int ret = 0;

	if(*fd < 0) return -1;

	while(1)
	{
		ret = accept(*fd, &cliaddr, &clilen);
		if(ret < 0)
		{
			if(errno == EINTR) continue;
			perr("accept");
			if(flag == 0 || (flag == 1 && errno != EAGAIN))
				sockclose(fd);
			return -1;
		}
		break;
	}

	return ret;
}

int socksend(int *fd, unsigned char* data, int count, int timeout)
{
	int ret = 0;

	if(*fd == -1) return 1;
	
	ret = sockwrite(*fd, data, count, timeout);
	if(ret < 0)
	{
		sockclose(fd);
		return 1;
	}
	return 0;
}

int sockreceive(int *fd, unsigned char* data, int count, int timeout)
{
	int ret = 0;

	if(*fd == -1) return 1;

	ret = sockread(*fd, data, 0, count, timeout, 0);
	if(ret <= 0)
	{
		sockclose(fd);
		return 1;
	}
	return 0;
}

char *get_ip(char *host)
{
	struct hostent hent;
	struct hostent *result = NULL;
	int ret = 0, err = 0, iplen = 16;
	char *ip = NULL, *buf = NULL;

	if(host == NULL) return NULL;

	ip = malloc(iplen);
	if(ip == NULL)
	{
		err("no mem");
		return NULL;
	}
	memset(ip, 0, iplen);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(ip);
		return NULL;
	}
	memset(buf, 0, MINMALLOC);

	ret = gethostbyname_r(host, &hent, buf, MINMALLOC, &result, &err);
	//if((hent = gethostbyname(host)) == NULL)
	if(ret != 0 || result == NULL)
	{
		free(ip);
		free(buf);
		perr("can't get ip");
		return NULL;
	}

	//snprintf(ip, iplen, "%s", inet_ntoa(*((struct in_addr*)hent->h_addr)));
	//snprintf(ip, iplen, "%s", inet_ntoa(*((struct in_addr*)result->h_addr)));
	
	//if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
	if(inet_ntop(AF_INET, (void *)result->h_addr_list[0], ip, iplen) == NULL)
	{
		free(ip);
		free(buf);
		perr("can't resolve host");
		return NULL;
	}

	free(buf);
	return ip;
}

char* createhttpheader(char *host, char *page, char* auth)
{
	char *query = NULL;
	char *getpage = page;

	if(getpage[0] == '/') getpage = getpage + 1;

	query = ostrcat("GET /", getpage, 0, 0);
	query = ostrcat(query, " HTTP/1.0\r\nHost: ", 1, 0);
	query = ostrcat(query, host, 1, 0);
	query = ostrcat(query, "\r\nUser-Agent: ", 1, 0);
	query = ostrcat(query, PROGNAME, 1, 0);
	if(auth != NULL)
	{
		query = ostrcat(query, "\r\nAuthorization: Basic ", 1, 0);
		query = ostrcat(query, auth, 1, 0);
	}
	query = ostrcat(query, "\r\n\r\n", 1, 0);

	return query;
}

int checkhttpheader(char* tmpbuf, char** retstr)
{
	int stat = 0;
	char* tmppos = NULL, *tmpstr = NULL;

	if(tmpbuf == NULL) return 0;

	tmpstr = malloc(MINMALLOC);
	if(tmpstr == NULL)
	{
		err("no mem");
		return 0;
	}

	tmppos = ostrstr(tmpbuf, "HTTP/1.1 ");
	if(tmppos != NULL)
	{
		sscanf(tmppos, "HTTP/1.1 %s ", tmpstr);
		if(tmpstr != NULL)
		{
			stat = atoi(tmpstr);
			tmpstr[0] = '\0';
		}
	}

	if(stat == 301 || stat == 302) // Redirect
	{
		tmppos = ostrstr(tmpbuf, "Location: ");
		if(tmppos != NULL)
		{
			sscanf(tmppos, "Location: %s", tmpstr);
			if(tmpstr != NULL)
				*retstr = ostrcat(tmpstr, NULL, 0, 0);
		}
	}

	free(tmpstr); tmpstr = NULL;
	return stat;
}

char* gethttp(char* host, char* page, int port, char* filename, char* auth, struct download* dnode, int redirect)
{
	int sock = -1, ret = 0, count = 0, hret = 0;
	unsigned int len = 0, maxret = 0;
	char *ip = NULL, *header = NULL;
	char *tmpbuf = NULL, *buf = NULL;
	char *tmppage = "/", *retstr = NULL;
	FILE *fd = NULL;
	
	if(filename != NULL)
	{
		fd = fopen(filename, "wb");
		if(fd == NULL)
		{
			err("can't open file %s", filename);
			if(dnode != NULL) dnode->ret = 1;
			return NULL;
		}
	}

	if(page == NULL) page = tmppage;

	ip = get_ip(host);
	if(ip == NULL)
	{
		err("can't get ip");
		if(fd != NULL)
		{
			fclose(fd);
			unlink(filename);
		}
		if(dnode != NULL) dnode->ret = 1;
		return NULL;
	}

	ret = sockportopen(&sock, ip, port, 5000 * 1000);
	if(ret != 0)
	{
		if(fd != NULL)
		{
			fclose(fd);
			unlink(filename);
		}
		free(ip);
		if(dnode != NULL) dnode->ret = 1;
		return NULL;
	}
	free(ip);

	if(dnode != NULL) dnode->connfd = sock;
	header = createhttpheader(host, page, auth);

	//Send the query to the server
	ret = socksend(&sock, (unsigned char*)header, strlen(header), 5000 * 1000);
	if(ret != 0)
	{
		perr("can't send query");
		if(fd != NULL)
		{
			fclose(fd);
			unlink(filename);
		}
		free(header);
		if(dnode != NULL) dnode->ret = 1;
		return NULL;
	}
	free(header);

	//now it is time to receive the page
	tmpbuf = malloc(MINMALLOC);
	if(tmpbuf == NULL)
	{
		if(fd != NULL)
		{
			fclose(fd);
			unlink(filename);
		}
		sockclose(&sock);
		err("no mem");
		if(dnode != NULL) dnode->ret = 1;
		return NULL;
	}
	memset(tmpbuf, 0, MINMALLOC);

	//read one line
	char* pbuf = tmpbuf;
	while(pbuf - tmpbuf < MINMALLOC)
	{
		unsigned char c;

		ret = sockreceive(&sock, &c, 1, 5000 * 1000);
		if(ret != 0)
		{
			err("no client data in buffer");
			break;
		}

		*pbuf = c;

		if(tmpbuf != NULL && (ostrstr(tmpbuf, "\n\n") != NULL || ostrstr(tmpbuf, "\r\n\r\n") != NULL))
		{
			hret = checkhttpheader(tmpbuf, &retstr);
			if(hret == 301 || hret == 302) goto end;
			break;
		}
		pbuf++;
	}

	//TODO: case-sens check
	char* contentlen = ostrstr(tmpbuf, "Content-Length:");
	if(contentlen != NULL)
	{
		contentlen += 15;
		len = strtoul(contentlen, NULL, 10);
	}

	while((ret = sockread(sock, (unsigned char*)tmpbuf, 0, MINMALLOC, 5000 * 1000, 0)) > 0)
	{
		maxret += ret;
		if(len > 0)
		{
			if(dnode != NULL)
			{
				dnode->proz = (int)ceil((((float)maxret / len) * 100));
				dnode->maxkb = len;
				dnode->aktkb = maxret;
			}
		}

		if(filename != NULL)
			fwrite(tmpbuf, ret, 1, fd);
		else
		{
			count = count + ret;
			buf = realloc(buf, count);
			memcpy(buf + count - ret, tmpbuf, ret);
		}
		memset(tmpbuf, 0, ret);
	}
	if(ret < 0)
	{
		perr("receiving data");
		if(fd != NULL)
		{
			fclose(fd);
			unlink(filename);
		}
		sockclose(&sock);
		free(buf);
		free(tmpbuf);
		if(dnode != NULL) dnode->ret = 1;
		return NULL;
	}

end:
	free(tmpbuf);
	if(fd != NULL) fclose(fd);
	sockclose(&sock);

	if(filename == NULL)
	{
		buf = realloc(buf, count + 1);
		buf[count] = '\0';
	}

	if((hret == 301 || hret == 302) && retstr != NULL && redirect < 3) //redirect
	{
		char* pos = NULL, *rpage = NULL;
        	char* rhost = string_replace("http://", "", retstr, 0);

	    	if(rhost != NULL)
			pos = strchr(rhost, '/');
		if(pos != NULL)
		{
			pos[0] = '\0';
			rpage = pos + 1;
		}

		if(dnode != NULL)
		{
			dnode->host = rhost;
			dnode->page = rpage;
		}

		redirect++;
		free(buf); buf = NULL;
		buf = gethttp(rhost, rpage, port, filename, auth, dnode, redirect);
	}

	if(dnode != NULL) dnode->ret = 0;
	free(retstr); retstr = NULL;
	return buf;
}

void gethttpstruct(struct stimerthread* timernode, struct download* node, int flag)
{
	if(node != NULL)
		gethttp(node->host, node->page, node->port, node->filename, node->auth, node, 0);
}

#endif
