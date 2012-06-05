#ifndef HTTPD_H
#define HTTPD_H

char* getmimetype(char* name, char* mime)
{
	char* dot;

	if(mime != NULL) return mime;
	if(name == NULL) return "text/html";

	dot = strrchr( name, '.' );
	if(dot == NULL || strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html";
	if(strcmp(dot, ".jpg") == 0 || strcmp( dot, ".jpeg" ) == 0)
		return "image/jpeg";
	if(strcmp(dot, ".gif") == 0)
		return "image/gif";
	if(strcmp(dot, ".png") == 0)
		return "image/png";
	if(strcmp(dot, ".css") == 0)
		return "text/css";
	if(strcmp(dot, ".au") == 0)
		return "audio/basic";
	if(strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if(strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if(strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt" ) == 0)
		return "video/quicktime";
	if(strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe" ) == 0)
		return "video/mpeg";
	if(strcmp(dot, ".ts") == 0)
		return "video/MP2T";	
	if(strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl" ) == 0)
		return "model/vrml";
	if(strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid" ) == 0)
		return "audio/midi";
	if(strcmp(dot, ".mp3") == 0)
		return "audio/mpeg";
	if(strcmp(dot, ".ogg") == 0)
		return "application/ogg";
	if(strcmp(dot, ".pac") == 0)
		return "application/x-ns-proxy-autoconfig";

	return "text/html";
}

char* createheader(off64_t len, char* filename, char* mime, char* ext, int code, int auth)
{
	char* header = NULL, *buf = NULL;
	time_t now = time(NULL);

	buf = malloc(100);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	if(auth == 1) code=401;
	switch(code)
	{
		case 200:
			header = ostrcat(header, "HTTP/1.1 200 OK\r\n", 1, 0);
			break;
		case 302:
			header = ostrcat(header, "HTTP/1.1 302 Found\r\n", 1, 0);
			break;
		case 401:
			header = ostrcat(header, "HTTP/1.1 401 Unauthorized\r\n", 1, 0);
			break;
		case 500:
			header = ostrcat(header, "HTTP/1.1 500 Internal Server Error\r\n", 1, 0);
			break;
	}

	snprintf(buf, 99, "Server: %s\r\n", PROGNAME);
	header = ostrcat(header, buf, 1, 0);

	if(auth == 1)
	{
		snprintf(buf, 99, "WWW-Authenticate: Basic realm=\"Contol Panel\"\r\n");
		header = ostrcat(header, buf, 1, 0);
	}

	strftime(buf, 99, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", gmtime(&now));
	header = ostrcat(header, buf, 1, 0);

	header = ostrcat(header, "Content-Type: ", 1, 0);
	header = ostrcat(header, getmimetype(filename, mime), 1, 0);
	header = ostrcat(header, "\r\n", 1, 0);

	if(auth == 1) len = 0;
	snprintf(buf, 99, "Content-Length: %lld\r\n", len);
	header = ostrcat(header, buf, 1, 0);

	strftime(buf, 99, "Expires: %a, %d %b %Y %H:%M:%S GMT\r\n", gmtime(&now));
	header = ostrcat(header, buf, 1, 0);

	if(filename != NULL)
	{
		now = getfiletime(filename, 1);
		strftime(buf, 99, "Last-Modified: %a, %d %b %Y %H:%M:%S GMT\r\n", gmtime(&now));
		header = ostrcat(header, buf, 1, 0);
	}

	if(ext != NULL)
	{
		header = ostrcat(header, ext, 1, 0);
		header = ostrcat(header, "\r\n", 1, 0);
	}

	header = ostrcat(header, "Connection: close\r\n", 1, 0);
	header = ostrcat(header, "\r\n", 1, 0);

	free(buf);
	return header;
}

void senderror(int* connfd, char* title, char* text, int auth)
{
	char* buf = NULL, *header = NULL;;

	buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css><title>Error ", 1, 0);
	buf = ostrcat(buf, title, 1, 0);
	buf = ostrcat(buf, "</title></head><body class=body><font class=error>Error - ", 1, 0);
	buf = ostrcat(buf, text, 1, 0);
	buf = ostrcat(buf, "</font></body></html>", 1, 0);

	header = createheader(strlen(buf), NULL, NULL, NULL, 500, auth);

	socksend(connfd, (unsigned char*)header, strlen(header), 5000 * 1000);
	socksend(connfd, (unsigned char*)buf, strlen(buf), 5000 * 1000);

	free(header);
	free(buf);
}

void checkquery(int* connfd, char* query, int auth, int fmt)
{
	char* buf = NULL, *header = NULL, *param = NULL;
	char* ext = NULL, *mime = NULL;
	int buflen = 0, onlyheader = 0, code = 200;

	//create param
	param = strchr(query, '&');
	if(param != NULL)
		*param++ = '\0';

	if(ostrcmp(query, "getrccodes") == 0)
		webgetrccodes(fmt);
	if(ostrcmp(query, "getmute") == 0)
		webgetmute(fmt);
	if(ostrcmp(query, "getvol") == 0)
		webgetvol(fmt);
	if(ostrcmp(query, "sendrc") == 0)
		websendrc(param, fmt);
	if(ostrcmp(query, "getrectimer") == 0)
		buf = webgetrectimer(param, 0, fmt);
	if(ostrcmp(query, "addrectimer") == 0)
		buf = webaddrectimer(param, fmt);
	if(ostrcmp(query, "delrectimer") == 0)
		buf = webdelrectimer(param, fmt);
	if(ostrcmp(query, "editrectimer") == 0)
		buf = webeditrectimer(param, fmt);
	if(ostrcmp(query, "rectimersend") == 0)
		buf = webrectimersend(param, fmt);
	if(ostrcmp(query, "getrectimerarchive") == 0)
		buf = webgetrectimer(param, 1, fmt);
	if(ostrcmp(query, "setvol") == 0)
		buf = websetvol(param, fmt);
	if(ostrcmp(query, "setmute") == 0)
		buf = websetmute(param, fmt);
	if(ostrcmp(query, "getbouquet") == 0)
		buf = webgetbouquet(fmt);
	if(ostrcmp(query, "getsat") == 0)
		buf = webgetsat(fmt);
	if(ostrcmp(query, "getprovider") == 0)
		buf = webgetprovider(fmt);
	if(ostrcmp(query, "getaz") == 0)
		buf = webgetaz(fmt);
	if(ostrcmp(query, "getchannelpage") == 0)
		buf = webgetchannelpage(param, fmt);
	if(ostrcmp(query, "getallchannel") == 0)
		buf = webgetchannel(0, 0, 1, fmt);
	if(ostrcmp(query, "getbouquetchannel") == 0)
		buf = webgetbouquetchannel(param, fmt);
	if(ostrcmp(query, "getsatchannel") == 0 && param != NULL)
		buf = webgetchannel(atoi(param), 1, 1, fmt);
	if(ostrcmp(query, "getproviderchannel") == 0 && param != NULL)
		buf = webgetchannel(atoi(param), 2, 1, fmt);
	if(ostrcmp(query, "getazchannel") == 0 && param != NULL)
		buf = webgetchannel(atoi(param), 3, 1, fmt);
	if(ostrcmp(query, "switch") == 0)
		buf = webswitch(param, fmt);
	if(ostrcmp(query, "getaktservice") == 0)
		buf = webgetaktservice(fmt);
	if(ostrcmp(query, "getepg") == 0)
		buf = webgetepg(param, fmt);
	if(ostrcmp(query, "getmovieepg") == 0)
		buf = webgetmovieepg(param, getconfig("rec_streampath", NULL), 1, fmt);
	if(ostrcmp(query, "getsingleepg") == 0)
		buf = webgetsingleepg(param, fmt);
	if(ostrcmp(query, "getgmultiepg") == 0)
		buf = webgetgmultiepg(param, fmt);
	if(query != NULL && strstr(query, "getepgsearch") == query)
		buf = webgetepgsearch(query, param, fmt);
	if(ostrcmp(query, "getsignal") == 0)
		buf = webgetsignal(fmt);
	if(ostrcmp(query, "getmoviefilelist") == 0)
		buf = webgetfilelist(param, "getmoviefilelist", "delmoviefile", getconfig("rec_streampath", NULL), "*.avi *.dat *.divx *.flv *.mkv *.m4v *.mp4 *.mov *.mpg *.mpeg *.mts *.m2ts *.trp *.ts *.vdr *.vob *.wmv *.rm", 31, fmt);
	if(ostrcmp(query, "delmoviefile") == 0)
		buf = webdelfile(param, "getmoviefilelist", "delmoviefile", getconfig("rec_streampath", NULL), "*.avi *.dat *.divx *.flv *.mkv *.m4v *.mp4 *.mov *.mpg *.mpeg *.mts *.m2ts *.trp *.ts *.vdr *.vob *.wmv *.rm", 31, fmt);
	if(ostrcmp(query, "getm3u") == 0)
	{
		buf = webgetm3u(param, *connfd, fmt);
		if(fmt == 0)
		{
			ext = "Content-Disposition: attachment; filename=stream.m3u";
			mime = "text/plain";
		}
	}
	if(ostrcmp(query, "getvideo") == 0)
		buf = webgetvideo(param, *connfd, fmt);
  if(ostrcmp(query, "videoplay") == 0)
		buf = webvideo(param, fmt);
	if(ostrcmp(query, "getshoot") == 0)
	{
		webgetshoot(param, fmt);
		if(fmt == 0)
		{
			ext = "Location: shoot.html";
			onlyheader = 1;
			code = 302;
		}
		else
			buf = ostrcat("shoot.html", NULL, 0, 0);
	}
	if(query != NULL && strstr(query, "poweroff") == query)
		oshutdown(1, 1);
	if(query != NULL && strstr(query, "restart") == query)
		oshutdown(2, 1);
	if(query != NULL && strstr(query, "guirestart") == query)
		oshutdown(3, 1);
	if(query != NULL && strstr(query, "standby") == query)
	{
		status.standby = 2;
		addtimer(&screenstandby, START, 1000, 1, NULL, NULL, NULL);
	}
	if(query != NULL && strstr(query, "boxstatus") == query)
	{
		if(status.standby > 0)
		{
			if(fmt == 0)
				sendoktext(connfd, "standby", auth);
			else
				buf = ostrcat("standby", NULL, 0, 0);
		}
		else
		{
			if(fmt == 0)
				sendoktext(connfd, "running", auth);
			else
				buf = ostrcat("running", NULL, 0, 0);
		}
	}
	if(query != NULL && strstr(query, "message") == query)
		buf = websendmessage(query, fmt);
		
	if(buf != NULL || onlyheader == 1)
	{
		if(buflen == 0 && onlyheader == 0) buflen = strlen(buf);

		header = createheader(buflen, NULL, mime, ext, code, auth);
		socksend(connfd, (unsigned char*)header, strlen(header), 5000 * 1000);
		if(onlyheader == 0 && auth == 0)
			socksend(connfd, (unsigned char*)buf, buflen, 5000 * 1000);
	}
	else
		senderror(connfd, "query", "Error in query string", auth);

	free(header);
	free(buf);
}

void gotdata(int* connfd)
{
	int ret = 0, filefd = -1, auth = 0;
	unsigned char* buf = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *filename = NULL, *fullfilename = NULL, *header = NULL, *query = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		sockclose(connfd);
		return;
	}
	memset(buf, 0, MINMALLOC);

	debug(250, "get client data");

	//read one line
	unsigned char* pbuf = buf;
	while(pbuf - buf < MINMALLOC)
	{
		unsigned char c;

		ret = sockreceive(connfd, &c, 1, 5000 * 1000);
		if(ret != 0)
		{
			err("no client data in buffer");
			break;
		}

		*pbuf = c;
		if(buf != NULL && (strstr((char*)buf, "\n\n") != NULL || strstr((char*)buf, "\r\n\r\n") != NULL))
			break;
		pbuf++;
	}

	if(buf != NULL)
	{
		tmpstr = strstr((char*)buf, "GET /");
		tmpstr1 = strstr((char*)buf, "Authorization: Basic ");
	}

	//Auth Password
	if(status.httpauth != NULL)
	{
		if(tmpstr1 != NULL)
		{
			tmpstr1 += 21;
			char* tmpstr3 = malloc(255);
			if(tmpstr3 != NULL)
			{
				int l = b64dec(tmpstr3, tmpstr1);
				if(l < 255) tmpstr3[l] = '\0';
			}
			if(ostrncmp(tmpstr3, status.httpauth, strlen(status.httpauth)) != 0)
			{
				//not ok:
				tmpstr1 = NULL;
			}
			free(tmpstr3); tmpstr3 = NULL;
		}
	
		if(tmpstr1 == NULL) auth = 1;
	}

	if(tmpstr != NULL)
	{
		tmpstr += 4;

		filename = malloc(MINMALLOC);
		if(filename == NULL)
		{
			err("no mem");
			sockclose(connfd);
			free(buf); buf = NULL;
			tmpstr = NULL;
			return;
		}
		memset(filename, 0, MINMALLOC);

		ret = sscanf(tmpstr, "%s", filename);
		if(ret == 1)
		{
			htmldecode(filename, filename);
			
			if(strstr(filename, "xmessage") == filename + 1  || strstr(filename, "/cgi-bin/xmessage") == filename )
			{	
				xmessage(filename);
				sendoktext(connfd, "ok", 0);
				//senderror(connfd, "ok", "ok", 0);
				free(buf); buf = NULL;
				free(filename); filename = NULL;
				tmpstr = NULL;
				return;
			}

			//create query
			query = strchr(filename, '?');
			if(query != NULL)
			{
				*query++ = '\0';
				debug(250, "httpd query=%s", query);
			}

			//queryraw
			if(ostrcmp(filename, "/queryraw") == 0 && query != NULL)
			{
				checkquery(connfd, query, auth, 1);
				free(buf); buf = NULL;
				free(filename); filename = NULL;
				tmpstr = NULL;
				return;
			}

			//query
			if(ostrcmp(filename, "/query") == 0 && query != NULL)
			{
				checkquery(connfd, query, auth, 0);
				free(buf); buf = NULL;
				free(filename); filename = NULL;
				tmpstr = NULL;
				return;
			}

			//create index.html
			if(filename[strlen(filename) - 1] == '/')
				filename = ostrcat(filename, "index.html", 1, 0);

			debug(250, "httpd filename=%s", filename);

			if(strstr(filename, "/movie/") != NULL) 
				fullfilename = ostrcat(filename, NULL, 0, 0);
			else
				fullfilename = ostrcat(getconfig("httpdpath", NULL), filename, 0, 0);
			filefd = open(fullfilename, O_RDONLY | O_LARGEFILE);
			if(filefd < 0)
			{
				perr("open filename=%s", fullfilename);
				senderror(connfd, "Open File", "Can't open File", auth);
				free(fullfilename); fullfilename = NULL;
				free(buf); buf = NULL;
				free(filename); filename = NULL;
				tmpstr = NULL;
				return;
			}
			debug(250, "sende OK response to client");
			char* rpath = realpath(fullfilename, NULL);
			header = createheader(getfilesize(rpath), fullfilename, NULL, NULL, 200, auth);
			free(rpath); rpath = NULL;
			free(fullfilename); fullfilename = NULL;
			ret = socksend(connfd, (unsigned char*)header, strlen(header), 5000 * 1000);
			free(header); header = NULL;

			if(ret != 0)
			{
				sockclose(connfd);
				free(buf); buf = NULL;
				free(filename); filename = NULL;
				tmpstr = NULL;
				return;
			}

			//TODO:
			int readret = 1;
			while(readret > 0 && auth == 0)
			{
				readret = dvbreadfd(filefd, buf, 0, MINMALLOC, 1000);
				if(readret > 0)
					socksend(connfd, buf, readret, 5000 * 1000);
			}
		}
	}

	close(filefd);
	free(buf); buf = NULL;
	free(filename); filename = NULL;
	tmpstr = NULL;
}

int newconn(int* streamfd, int* connfd)
{
	int i, fd = -1;

	fd = sockaccept(streamfd, 1);
	if(fd < 0) return -1;

	for(i = 0; i < MAXHTTPDCONN; i++)
	{
		if(connfd[i] < 0)
		{
			connfd[i] = fd;
			debug(250, "accept httpd connection fd=%d", fd);
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

void httpdthreadfunc(struct stimerthread* timernode)
{
	struct timeval timeout;
	fd_set rfds;
	int i, ret = 0, streamfd = -1, connfd[MAXHTTPDCONN], maxfd = -1;

	debug(250, "Starting httpd thread");

	for(i = 0; i < MAXHTTPDCONN; i++)
		connfd[i] = -1;

	while(timernode->aktion != STOP)
	{
		if(streamfd < 0)
		{
			sockportcreate(&streamfd, getconfigint("httpdport", NULL), MAXHTTPDCONN);
			if(streamfd < 0) break;
			maxfd = streamfd;

			//set nonblocking
			fcntl(streamfd, F_SETFL, fcntl(streamfd, F_GETFL) | O_NONBLOCK);
		}

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&rfds);
		FD_SET(streamfd, &rfds);

		for(i = 0; i < MAXHTTPDCONN; i++)
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
			perr("httpd listen socket fd=%d", maxfd);
			sleep(1);
			continue;
		}
		if(ret == 0) continue; //timeout

		if(FD_ISSET(streamfd, &rfds))
		{
			ret = newconn(&streamfd, connfd);
			if(ret < 0) continue;
		}

		for(i = 0; i < MAXHTTPDCONN; i++)
		{
			if(connfd[i] > -1 && FD_ISSET(connfd[i], &rfds))
				gotdata(&connfd[i]);
		}
	}

	debug(250, "Stopping httpd thread");
	sockclose(&streamfd);
	for(i = 0; i < MAXHTTPDCONN; i++)
		sockclose(&connfd[i]);

	return;
}

void sendoktext(int* connfd, char* text, int auth)
{
	char* buf = NULL;
	char* header = NULL;
	buf = webcreatehead(buf, NULL, 1);
	buf = ostrcat(buf, "<tr><td align=center valign=top><font class=biglabel><br><br>", 1, 0);
	buf = ostrcat(buf, text, 1, 0);
	buf = ostrcat(buf, "</font></td></tr>", 1, 0);
	buf = webcreatetail(buf, 1);
	int buflen = strlen(buf);
	header = createheader(buflen, NULL, NULL, NULL, 200, auth);
	socksend(connfd, (unsigned char*)header, strlen(header), 5000 * 1000);
	if(auth == 0)
		socksend(connfd, (unsigned char*)buf, buflen, 5000 * 1000);
	free(buf); buf=NULL;
	free(header); header=NULL;
}

#endif
