#ifndef PUTLOCKER_H
#define PUTLOCKER_H

unsigned char* putlockereceive(int* sock)
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

char* putlocker(char* host, char* file)
{
	int sock = -1, ret = 0;
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* hash = NULL;
	char* hashstr = NULL;
	char* hashlen = NULL;
	char* ip = NULL;
	char* phpsessid = NULL;
	char* serverid = NULL;
	char* streamlink = NULL;
	char* streamlink1 = NULL;

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	ip = get_ip(tmphost);
	tmpstr = gethttp(tmphost, tmpfile, 80, NULL, NULL, NULL, 0);

	//get hash from tmpstr
	char* pos = ostrstr(tmpstr, "<input type=\"hidden\" value=");
	hashstr = getxmlentry(pos, "value=");
	free(tmpstr); tmpstr = NULL;
	if(hashstr == NULL) goto end;

	hash = ostrcat(hash, "hash=", 1, 0);	
	hash = ostrcat(hash, hashstr, 1, 0);
	hash = ostrcat(hash, "&confirm=Continue+as+Free+User", 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);

	sleep(3);
	//create send string
	send = ostrcat(send, "POST /file/", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.", 1, 0);

	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);

	sleep(3);

	//send and receive answer
	ret = sockportopen(&sock, ip, 80, 5000 * 1000);
        if(ret != 0) goto end;
	ret = socksend(&sock, (unsigned char*)send, strlen(send), 5000 * 1000);
        if(ret != 0) goto end;
	free(send); send = NULL;
	tmpstr = putlockereceive(&sock);
	sockclose(&sock);
sleep(2);
	//get phpsessid and servierid
	phpsessid = getxmlentry(tmpstr, "PHPSESSID=");
	serverid = getxmlentry(tmpstr, "SERVERID=");
	free(tmpstr); tmpstr = NULL;
// move we need error msg later
//	if(phpsessid == NULL || serverid == NULL) goto end;
	if(strlen(phpsessid) > 0) phpsessid[strlen(phpsessid) - 1] = '\0';

	//create send string
	send = ostrcat(send, "GET /file/", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: www.", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nCookie: SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, "PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);

	//send and receive answer
	gethttpreal(tmphost, tmpfile, 80, "x9", NULL, NULL, 0, send, NULL);

	free(send); send = NULL;
	tmpstr = command("cat x9 | sed '1,1d' | zcat");
	
	if(ostrstr(tmpstr, "warning_message") != NULL)
	{
		tmpstr = string_resub("<div class='warning_message'>", "</div>", tmpstr, 0);	
		textbox(_("Message"), _(tmpstr) , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	if(phpsessid == NULL || serverid == NULL) goto end;			

	sleep(2);

	//get streamlink
	streamlink = getxmlentry(tmpstr, "playlist: '");
	if(streamlink != NULL)
	{
		char* pos = strchr(streamlink, '\'');
		if(pos != NULL) pos[0] = '\0';
	}
	free(tmpstr); tmpstr = NULL;
	if(streamlink == NULL) goto end;

	//create send string
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, streamlink, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: www.", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nCookie: SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, "PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nUser-Agent: Python-urllib/2.6\r\n\r\n", 1, 0);

	sleep(2);
	
	//send and receive answer
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL);
	free(send); send = NULL;

	//get streamlink1
	streamlink1 = getxmlentry(tmpstr, "url=");
	free(tmpstr); tmpstr = NULL;

end:
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(hash); hash = NULL;
	free(hashstr); hashstr = NULL;
	free(hashlen); hashlen = NULL;	
	free(ip); ip = NULL;
	free(phpsessid); phpsessid = NULL;
	free(serverid); serverid = NULL;
	free(streamlink); streamlink = NULL;
	sockclose(&sock);

	return streamlink1;
}

#endif
