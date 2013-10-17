#ifndef PRIMESHARE_H
#define PRIMESHARE_H

char* primeshare(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmpfile = NULL, *tmpstr = NULL, *tmpstr2 = NULL, *send = NULL, *ip = NULL, *streamlink = NULL, *hash = NULL, *hashlen = NULL, *hashid = NULL, *phpsessid = NULL;

	if(host == NULL || file == NULL) return NULL;

	unlink("/tmp/primeshare1_get");
	unlink("/tmp/primeshare2_get");

	tmphost = ostrcat(host, NULL, 0, 0);
//	tmphost = ostrcat(tmphost, ".tv", 1, 0);

	tmpfile = ostrcat("/download/", file, 0, 0);
	
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
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/primeshare1_get", NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "The file is being transfered to our other servers. This may take few minutes.") != NULL)
	{
		textbox(_("Message"), _("The file is being transfered to our other servers. This may take few minutes.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	waitmsgbar(20, 0, 1);

	phpsessid = getxmlentry(tmpstr, "PHPSESSID=");
	debug(99, "phpsessid: %s", phpsessid);

	char* pos1 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"hash\" value=");
	hashid = getxmlentry(pos1, "value=");
	debug(99, "hashid: %s", hashid);
	if(hashid == NULL) goto end;

	hash = ostrcat(hash, "hash=", 1, 0);	
	hash = ostrcat(hash, hashid, 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));

	free(send), send = NULL;
	//create send string
	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: ", 1, 0);
//	send = ostrcat(send, "\r\nCookie: usender=", 1, 0);
//	send = ostrcat(send, usender, 1, 0);
//	send = ostrcat(send, " SERVERID=", 1, 0);
//	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, " PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/primeshare2_get", NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmpstr2 = string_resub("clip: {", "}", tmpstr, 0);
	debug(99, "tmpstr2: %s", tmpstr2);
	streamlink = string_resub("url: '", "'", tmpstr2, 0);
	debug(99, "streamlink1: %s", streamlink);
	htmldecode(streamlink, streamlink);
	debug(99, "streamlink2: %s", streamlink);
	
end:
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr2); tmpstr2 = NULL;
	free(send); send = NULL;
	free(hash); hash = NULL;
	free(hashlen); hashlen = NULL;
	free(ip); ip = NULL;
	free(hashid), hashid = NULL;

	return streamlink;
}

#endif
