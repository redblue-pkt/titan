#ifndef PUTLOCKER_H
#define PUTLOCKER_H

char* putlocker(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
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
	char* usender = NULL;
	char* streamlink = NULL;
	char* streamlink1 = NULL;
	char* tmpstr2 = NULL;

	unlink("/tmp/tithek/get");
	unlink("/tmp/tithek/x9");

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send); send = NULL;
	phpsessid = getxmlentry(tmpstr, "PHPSESSID=");
	debug(99, "phpsessid: %s", phpsessid);
	serverid = getxmlentry(tmpstr, "SERVERID=");
	debug(99, "serverid: %s", serverid);
	usender = getxmlentry(tmpstr, "usender=");
	debug(99, "usender: %s", usender);

	//get hash from tmpstr
	char* pos = ostrstr(tmpstr, "<input type=\"hidden\" value=");
	debug(99, "tmpstr: %s", tmpstr);
	hashstr = getxmlentry(pos, "value=");
// free(tmpstr) after getxml or segfault...
	free(tmpstr); tmpstr = NULL;
	debug(99, "hashstr: %s", hashstr);
	if(hashstr == NULL) goto end;

	hash = ostrcat(hash, "hash=", 1, 0);	
	hash = ostrcat(hash, hashstr, 1, 0);
	hash = ostrcat(hash, "&confirm=Continue+as+Free+User", 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);

	//create send string
	send = ostrcat(send, "POST /file/", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nCookie: usender=", 1, 0);
	send = ostrcat(send, usender, 1, 0);
	send = ostrcat(send, " SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, " PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

//	sleep(7);
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send); send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/putlocker4_tmpstr_post_zcat", NULL, tmpstr);
	
	if(ostrstr(tmpstr, "warning_message") != NULL)
	{
		tmpstr2 = string_resub("<div class='warning_message'>", "</div>", tmpstr, 0);
		tmpstr2 = strstrip(tmpstr2);
		if(tmpstr2 == NULL || strlen(tmpstr2) == 0)
			tmpstr2 = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), _(tmpstr2) , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(phpsessid == NULL || serverid == NULL) goto end;			

	//get streamlink
	if(ostrstr(tmpstr, "playlist:") != NULL)
		streamlink = getxmlentry(tmpstr, "playlist: '");
	else
	{
		streamlink = string_resub("/get_file.php", "\">Download File</a>", tmpstr, 0);
		streamlink1 = ostrcat("http://www.", host, 0, 0);
		streamlink1 = ostrcat(streamlink1, "/get_file.php", 1, 0);
		streamlink1 = ostrcat(streamlink1, streamlink, 1, 0);
		debug(99, "stream downloadlink: %s", streamlink1);
		goto end;
	}
	debug(99, "streamlink: %s", streamlink);

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

//	sleep(5);
	
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
	free(send); send = NULL;
	titheklog(debuglevel, "/tmp/putlocker5_tmpstr_get", NULL, tmpstr);

	//get streamlink1
	streamlink1 = getxmlentry(tmpstr, "url=");
	titheklog(debuglevel, "/tmp/putlocker5_streamlink", NULL, streamlink1);

	free(tmpstr); tmpstr = NULL;

end:
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr2); tmpstr2 = NULL;
	free(send); send = NULL;
	free(hash); hash = NULL;
	free(hashstr); hashstr = NULL;
	free(hashlen); hashlen = NULL;	
	free(ip); ip = NULL;
	free(phpsessid); phpsessid = NULL;
	free(serverid); serverid = NULL;
	free(usender); usender = NULL;
	free(streamlink); streamlink = NULL;

	return streamlink1;
}

#endif
