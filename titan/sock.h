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
				debug(250, "connection closed fd=%d, count=%d", fd, count - tmpcount);
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
	int ret = 0, err = 0, iplen = 16, ext = 0;
	char *ip = NULL, *buf = NULL, *tmpstr = NULL;

	if(host == NULL) return NULL;

	ip = calloc(1, iplen);
	if(ip == NULL)
	{
		err("no mem");
		return NULL;
	}

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(ip);
		return NULL;
	}

	ret = gethostbyname_r(host, &hent, buf, MINMALLOC, &result, &err);
	//if((hent = gethostbyname(host)) == NULL)
	if(ret != 0 || result == NULL)
	{
		free(ip); ip = NULL;
		free(buf); buf = NULL;
		//workaround: if resolv.conf is changed, titan must stopped, so gethostbyname read resolv.conf new
		//the external tool read resolv.conf on each start
		err("can't get ip, test with extern tool (%s)", host);

		tmpstr = ostrcat("getip ", host, 0, 0);
		ip = command(tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(ip == NULL)
		{
			err("can't get ip (%s)", host);
			return NULL;
		}
		ext = 1;
	}

	//snprintf(ip, iplen, "%s", inet_ntoa(*((struct in_addr*)hent->h_addr)));
	//snprintf(ip, iplen, "%s", inet_ntoa(*((struct in_addr*)result->h_addr)));

	if(ext == 0)
	{
		//if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
		if(inet_ntop(AF_INET, (void *)result->h_addr_list[0], ip, iplen) == NULL)
		{
			free(ip);
			free(buf);
			perr("can't resolve host");
			return NULL;
		}
	}

	free(buf);
	return ip;
}

char* createhttpheader(char *host, char *page, char* auth)
{
	char *query = NULL;
	char *getpage = page;

	if(getpage != NULL)
	{
		if(getpage[0] == '/')
			getpage = getpage + 1;
	}

	query = ostrcat("GET /", getpage, 0, 0);
	query = ostrcat(query, " HTTP/1.0\r\nHost: ", 1, 0);
	query = ostrcat(query, host, 1, 0);
	query = ostrcat(query, "\r\nUser-Agent: ", 1, 0);
	//query = ostrcat(query, PROGNAME, 1, 0);
	query = ostrcat(query, "Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
	if(auth != NULL)
	{
		query = ostrcat(query, "\r\nAuthorization: Basic ", 1, 0);
		query = ostrcat(query, auth, 1, 0);
	}
	query = ostrcat(query, "\r\nConnection: close", 1, 0);
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

unsigned long getchunkedlen(int sock, int timeout)
{
	unsigned long len = 0;
	int ret = 0;
	char chunked[16] = {'\0'};
	int chunkedid = 0;

	while(chunkedid < 15)
	{
		unsigned char c = '\0';

		ret = sockreceive(&sock, &c, 1, timeout * 1000);
		if(ret != 0)
		{
			printf("no client data in buffer");
			break;
		}
		if(c == ';') break;

		chunked[chunkedid] = c;
		chunkedid++;
		if(chunkedid > 2 && c == '\n')
			break;
	}

	len = strtol(chunked, NULL, 16);
	debug(88, "chunkedlen=%u", len); 
	return len;
}

//flag 0: output without header
//flag 1: output with header
//flag 2: output only header
char* gethttpreal(char* host, char* page, int port, char* filename, char* auth, struct download* dnode, int redirect, char* header, long* clen, int timeout, int flag)
{
	int sock = -1, ret = 0, count = 0, hret = 0, freeheader = 0, gzip = 0;
	int headerlen = 0, chunkedlen = 0, chunked = 0, readsize = 0;
	unsigned int len = 0, maxret = 0;
	char *ip = NULL;
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

	ret = sockportopen(&sock, ip, port, timeout * 1000);
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
	if(header == NULL)
	{
		header = createhttpheader(host, page, auth);
		freeheader = 1;
	}

	//Send the query to the server
	ret = socksend(&sock, (unsigned char*)header, strlen(header), timeout * 1000);
	if(ret != 0)
	{
		perr("can't send query");
		if(fd != NULL)
		{
			fclose(fd);
			unlink(filename);
		}
		if(freeheader == 1) free(header);
		if(dnode != NULL) dnode->ret = 1;
		return NULL;
	}
	if(freeheader == 1) free(header);

	//now it is time to receive the page
	tmpbuf = calloc(1, MINMALLOC);
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

	//read one line
	char* pbuf = tmpbuf;
	while(pbuf - tmpbuf < MINMALLOC)
	{
		unsigned char c = '\0';

		ret = sockreceive(&sock, &c, 1, timeout * 1000);
		if(ret != 0)
		{
			err("no client data in buffer");
			break;
		}

		*pbuf = c;
		headerlen++;

		if(tmpbuf != NULL && (ostrstr(tmpbuf, "\n\n") != NULL || ostrstr(tmpbuf, "\r\n\r\n") != NULL))
		{
			hret = checkhttpheader(tmpbuf, &retstr);
			if(hret == 301 || hret == 302) goto end;
			break;
		}
		pbuf++;
	}
	
	debug(88, "-----------------------------------------------------------------");	
	debug(88, "host: %s page: %s", host, page);
	debug(88, "header: %s", tmpbuf);
	debug(88, "-----------------------------------------------------------------");

	if(flag == 2)
	{
		if(headerlen > 0)
		{
			count = headerlen;
			headerlen = 0;
			if(filename != NULL)
				fwrite(tmpbuf, count, 1, fd);
			else
			{
				buf = realloc(buf, count);
				if(buf != NULL)
					memcpy(buf, tmpbuf, count);
			}
		}
	
		goto end;
	}
	
	if(flag == 0) headerlen = 0;

	char* contentlen = ostrstrcase(tmpbuf, "Content-Length:");
	if(contentlen != NULL)
	{
		contentlen += 15;
		len = strtoul(contentlen, NULL, 10);
	}
	
	if(filename == NULL && (flag == 0 || flag == 1) && ostrstrcase(tmpbuf, "gzip") != NULL)
	{
		if(flag == 0) gzip = -1;
		else if(flag == 1) gzip = headerlen;
	}
	
	if(ostrstrcase(tmpbuf, "Transfer-Encoding: chunked") != NULL)
	{
		chunked = 1; 
		chunkedlen = getchunkedlen(sock, timeout);
		if(chunkedlen + headerlen > MINMALLOC)
			readsize = MINMALLOC - headerlen;
		else
			readsize = chunkedlen;
	}

	if(chunked == 0) readsize = MINMALLOC - headerlen;
	while((ret = sockread(sock, (unsigned char*)tmpbuf + headerlen, 0, readsize, timeout * 1000, 0)) > 0)
	{
		if(chunked == 1) chunkedlen -= ret;

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

		ret += headerlen;
		headerlen = 0;
		if(filename != NULL)
			fwrite(tmpbuf, ret, 1, fd);
		else
		{
			count = count + ret;
			buf = realloc(buf, count);
			if(buf != NULL)
				memcpy(buf + count - ret, tmpbuf, ret);
		}
		memset(tmpbuf, 0, ret);
		
		if(chunked == 1 && chunkedlen == 0)
		{
			chunkedlen = getchunkedlen(sock, timeout);
			if(chunkedlen + headerlen > MINMALLOC)
				readsize = MINMALLOC - headerlen;
			else
				readsize = chunkedlen;
		}
		if(chunked == 1 && chunkedlen > 0)
		{
			if(chunkedlen + headerlen > MINMALLOC)
				readsize = MINMALLOC - headerlen;
			else
				readsize = chunkedlen;
		}
		if(chunked == 0) readsize = MINMALLOC - headerlen;
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

	if(gzip != 0)
	{
		debug(88, "http unzip start");
		int unzipret = 0, outlen = 0;
		char* outbuf = NULL;
		
		if(gzip == -1)
			outbuf = malloc(MINMALLOC * 100);
		else
			outbuf = malloc((MINMALLOC * 100) + gzip);
			
		if(outbuf != NULL)
		{
			if(gzip == -1)
				unzipret = ounzip(buf, count, &outbuf, &outlen, MINMALLOC * 100, 3);
			else
			{
				memcpy(outbuf, buf, gzip);
				char* tmpoutbuf = outbuf + gzip;
				unzipret = ounzip(buf + gzip, count - gzip, &tmpoutbuf, &outlen, MINMALLOC * 100, 3);
				outlen += gzip;			
			}
			if(unzipret == 0)
			{
				free(buf); buf = outbuf;
				count = outlen;
			}
			else
			{
				free(outbuf); outbuf = NULL;
				err("unzip http data");
			}
		}
		else
		{
			err("no mem");
		}
		debug(88, "http unzip end");
	}

	if(filename == NULL)
	{
		buf = realloc(buf, count + 1);
		buf[count] = '\0';
	}

	if((hret == 301 || hret == 302) && retstr != NULL && redirect < 3) //redirect
	{
		char* pos = NULL, *rpage = NULL;
		char* rhost = NULL;

		if(strchr(retstr, '/') == retstr)
		{
			rhost = ostrcat(host, NULL, 0, 0);
			rpage = retstr;
		}
		else
		{
			rhost = string_replace("http://", "", retstr, 0);

			if(rhost != NULL)
				pos = strchr(rhost, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				rpage = pos + 1;
			}
		}

		redirect++;
		free(buf); buf = NULL;
		buf = gethttpreal(rhost, rpage, port, filename, auth, dnode, redirect, NULL, clen, timeout, flag);
		free(rhost); rhost = NULL;
	}

	if(clen != 0) *clen = maxret;
	if(dnode != NULL) dnode->ret = 0;
	free(retstr); retstr = NULL;

	if(filename == NULL)
		return buf;
	else
		return "0";
}

char* gethttp(char* host, char* page, int port, char* filename, char* auth, int timeout, struct download* dnode, int redirect)
{
	return gethttpreal(host, page, port, filename, auth, dnode, redirect, NULL, NULL, timeout, 0);
}

void gethttpstruct(struct stimerthread* timernode, struct download* node, int flag)
{
	if(node != NULL)
		gethttp(node->host, node->page, node->port, node->filename, node->auth, node->timeout, node, 0);
}

void gethttpstructmsg(struct stimerthread* timernode, struct download* node, int flag)
{
	char* tmpstr = NULL;

	if(node != NULL)
	{
		gethttp(node->host, node->page, node->port, node->filename, node->auth, node->timeout, node, 0);
		tmpstr = ostrcat(_("Download finished!"), "\n\n", 0, 0);
		tmpstr = ostrcat(tmpstr, node->filename, 1, 0);
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 7, 0);

		free(tmpstr); tmpstr = NULL;
	}
}

int sendmail(char* host, char* mailfrom, char* mailto, char* subject, int port, char* filename, char* string, int rtimeout, int wtimeout)
{
	int sock = -1, ret = 0;
	char *ip = NULL, *tmpstr = NULL, *sendstr = NULL, *data = NULL;

	if(filename == NULL && string == NULL) return 1;

	ip = get_ip(host);
	if(ip == NULL)
	{
		err("can't get ip");
		return 1;
	}

	data = malloc(MINMALLOC);
	if(data == NULL)
	{
		err("no memory");
		return 1;
	}

	if(filename != NULL)
		tmpstr = readfiletomem(filename, 0);
	else if(string != NULL)
		tmpstr = string;

	//open connection
	ret = sockportopen(&sock, ip, port, wtimeout);
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, wtimeout, 0);
	debug(975, "rcv: %s", data);
/*
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}
*/

	//send helo
	ret = 1;
	sendstr = ostrcat(sendstr, "HELO nit.at\r\n", 1, 0);
	debug(975, "send: %s", sendstr);
	if(sendstr != NULL)
		ret = socksend(&sock, (unsigned char*)sendstr, strlen(sendstr), wtimeout);
	free(sendstr); sendstr = NULL;
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, rtimeout, 0);
	debug(975, "rcv: %s", data);
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}

	//send mail from:
	ret = 1;
	sendstr = ostrcat(sendstr, "MAIL FROM:<", 1, 0);
	sendstr = ostrcat(sendstr, mailfrom, 1, 0);
	sendstr = ostrcat(sendstr, ">\r\n", 1, 0);
	debug(975, "send: %s", sendstr);
	if(sendstr != NULL)
		ret = socksend(&sock, (unsigned char*)sendstr, strlen(sendstr), wtimeout);
	free(sendstr); sendstr = NULL;
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, rtimeout, 0);
	debug(975, "rcv: %s", data);
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}

	//send rcpt to:
	ret = 1;
	sendstr = ostrcat(sendstr, "RCPT TO:<", 1, 0);
	sendstr = ostrcat(sendstr, mailto, 1, 0);
	sendstr = ostrcat(sendstr, ">\r\n", 1, 0);
	debug(975, "send: %s", sendstr);
	if(sendstr != NULL)
		ret = socksend(&sock, (unsigned char*)sendstr, strlen(sendstr), wtimeout);
	free(sendstr); sendstr = NULL;
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, rtimeout, 0);
	debug(975, "rcv: %s", data);
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}

	//send data
	ret = 1;
	sendstr = ostrcat(sendstr, "DATA\r\n", 1, 0);
	debug(975, "send: %s", sendstr);
	if(sendstr != NULL)
		ret = socksend(&sock, (unsigned char*)sendstr, strlen(sendstr), wtimeout);
	free(sendstr); sendstr = NULL;
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, rtimeout, 0);
	debug(975, "rcv: %s", data);
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}

	//send content
	ret = 1;

	sendstr = ostrcat(sendstr, "From: ", 1, 0);
	sendstr = ostrcat(sendstr, mailfrom, 1, 0);
	sendstr = ostrcat(sendstr, "\r\n", 1, 0);

	sendstr = ostrcat(sendstr, "To: ", 1, 0);
	sendstr = ostrcat(sendstr, mailto, 1, 0);
	sendstr = ostrcat(sendstr, "\r\n", 1, 0);

	sendstr = ostrcat(sendstr, "Subject: ", 1, 0);
	sendstr = ostrcat(sendstr, subject, 1, 0);
	sendstr = ostrcat(sendstr, "\r\n", 1, 0);

	sendstr = ostrcat(sendstr, "Content-Type: text/plain\r\n\r\n", 1, 0);

	sendstr = ostrcat(sendstr, tmpstr, 1, 0);
	sendstr = ostrcat(sendstr, "\r\n.\r\n", 1, 0);

	debug(975, "send: %s", sendstr);
	if(sendstr != NULL)
		ret = socksend(&sock, (unsigned char*)sendstr, strlen(sendstr), wtimeout);
	free(sendstr); sendstr = NULL;
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, rtimeout, 0);
	debug(975, "rcv: %s", data);
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}

	//send quit
	ret = 1;
	sendstr = ostrcat(sendstr, "QUIT\r\n", 1, 0);
	debug(975, "send: %s", sendstr);
	if(sendstr != NULL)
		ret = socksend(&sock, (unsigned char*)sendstr, strlen(sendstr), wtimeout);
	free(sendstr); sendstr = NULL;
	if(ret != 0) goto end;

	//wait for smtp answer
	memset(data, 0, MINMALLOC);
	sockread(sock, (unsigned char*)data, 0, MINMALLOC, rtimeout, 0);
	debug(975, "rcv: %s", data);
	if(ostrstr(data, "1") != data && ostrstr(data, "2") != data && ostrstr(data, "3") != data)
	{
		debug(975, "incorrect answer: %s", data);
		ret = 1;
		goto end;
	}

end:
	if(filename != NULL)
		free(tmpstr);
	tmpstr = NULL;
	return ret;
}

#include <stdio.h>
#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
	/* out of memory! */ 
	printf("not enough memory (realloc returned NULL)\n");
	return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

static size_t writeCallback(void *contents, size_t size, size_t nitems, FILE *file) {
  return fwrite(contents, size, nitems, file);
}

// flag = 0 (without header in output)
// flag = 1 (with header in output)
char* gethttps(char* url, char* localfile, char* data, char* user, char* pass, char* referer, int flag)
{
	debug(99, "url: %s", url);

	int debuglevel = getconfigint("debuglevel", NULL);

	char* tmpstr = NULL;
    	FILE *fp;

	CURL *curl_handle;
	CURLcode res;
	
	struct MemoryStruct chunk;
	
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	chunk.size = 0;    /* no data at this point */
	
	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */ 
	curl_handle = curl_easy_init();
	if(curl_handle)
	{
	    	if(localfile != NULL)
		    fp = fopen(localfile,"wb");
	       
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);

		if(user != NULL && pass != NULL)
		{
			curl_easy_setopt(curl_handle, CURLOPT_USERNAME, user);
			curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, pass);
			curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		}
		if(data == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
		else
		{
			curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

			/* example.com is redirected, so we tell libcurl to send POST on 301, 302 and
		     	303 HTTP response codes */
			curl_easy_setopt(curl_handle, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
		}
		if(flag == 1)
			curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 3);
//		curl_easy_setopt(curl_handle, CURLOPT_RETURNTRANSFER, 1);

	    	if(localfile == NULL)
		{
			/* send all data to this function  */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		}
		else
		{
			/* When data arrives, curl will call writeCallback. */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
		}



	    	if(localfile == NULL)
		{
			/* we pass our 'chunk' struct to the callback function */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		}
		else
		{
			/* The last argument to writeCallback will be our file: */
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)fp);
		}

		/* some servers don't like requests that are made without a user-agent field, so we provide one */
//		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36");

		// This is occassionally required to stop CURL from verifying the peers certificate.
		// CURLOPT_SSL_VERIFYHOST may also need to be TRUE or FALSE if
		// CURLOPT_SSL_VERIFYPEER is disabled (it defaults to 2 - check the existence of a
		// common name and also verify that it matches the hostname provided)
#ifdef MIPSEL
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
#else
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
		if(debuglevel == 99)
			curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "/mnt/network/cookies");
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "/mnt/network/cookies");
		/* enable redirect following */
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
		/* allow three redirects */
		curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 3L);


		/* enable all supported built-in compressions */
		curl_easy_setopt(curl_handle, CURLOPT_ACCEPT_ENCODING, "");
  
		if(referer == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1L);
		else
			curl_easy_setopt(curl_handle, CURLOPT_REFERER, referer);


		/* get it! */
		res = curl_easy_perform(curl_handle);

		/* check for errors */
		if(res != CURLE_OK)
		{
			err("failed: %s", curl_easy_strerror(res));
			printf("curl error\n");
		}
		else
		{
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */
			printf("%lu bytes retrieved\n", (long)chunk.size);
		}
		
		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);
		if(localfile != NULL)
			fclose(fp);
	}

	if(localfile == NULL)
		tmpstr = ostrcat(chunk.memory, NULL, 0, 0);

  	free(chunk.memory);
	/* we're done with libcurl, so clean it up */

	curl_global_cleanup();

	if(localfile != NULL)
		free(tmpstr), tmpstr = NULL;

	return tmpstr;
}

#endif
