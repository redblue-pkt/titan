#ifndef FLASHX_H
#define FLASHX_H

unsigned char* flashxreceive(int* sock)
{
	int ret = 0;
	unsigned char* buf = NULL;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	//read one line
	unsigned char* pbuf = buf;

	while(pbuf - buf < MINMALLOC)
	{
		unsigned char c;

		ret = sockreceive(sock, &c, 1, 5000 * 1000);
		if(ret != 0)
		{
			err("no client data in buffer");
			break;
		}

		*pbuf = c;
		if(buf != NULL && (ostrstr((char*)buf, "\n\n") != NULL || ostrstr((char*)buf, "\r\n\r\n") != NULL))
			break;
		pbuf++;
	}

	return buf;
}

char* flashx(char* host, char* file)
{
	debug(99, "in host: %s file: %s", host, file);
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "file: %s", file);

	send = ostrcat(send, "GET /player/embed_player.php?vid=", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: www.", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 1);
//	writesys("/var/usr/local/share/titan/plugins/tithek/flashx1", tmpstr, 0);
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	sleep(1);

	send = ostrcat(send, "GET /player/embed_player.php?vid=", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 1);
//	writesys("/var/usr/local/share/titan/plugins/tithek/flashx2", tmpstr, 0);
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	sleep(1);

	send = ostrcat(send, "GET /nuevo/player/cst.php?hash=", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: play.flashx.tv\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/x9", NULL, NULL, 0, send, NULL, 0);

	free(send); send = NULL;
	tmpstr = command("cat /tmp/tithek/x9 | sed '1,1d' | zcat");
//	writesys("/var/usr/local/share/titan/plugins/tithek/flashx4", tmpstr, 0);
	streamlink = string_resub("<file>", "</file>", tmpstr, 0);

	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
