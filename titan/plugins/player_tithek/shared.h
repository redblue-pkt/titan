#ifndef SHARED_H
#define SHARED_H

char* shared(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *ip = NULL, *streamlink = NULL, *timestamp = NULL;
	char* hash = NULL, *hashlen = NULL, *pos1 = NULL, *pos2 = NULL, *pos3 = NULL, *hashline = NULL, *expires = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/shared1_get");
	unlink("/tmp/shared2_post");
	
	char* tmplink = NULL, *pos = NULL, *path = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}
	

	if(tmplink == NULL || ostrncmp("http", tmplink, 4))
	{
		textbox(_("Message"), _("Hoster Url not http:// or https://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("http://", "", tmplink, 0);
	tmphost = string_replace("https://", "", tmphost, 1);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/", path, 0, 0);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nReferer: ", 1, 0);
	send = ostrcat(send, link, 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/shared1_get", NULL, NULL, NULL, tmpstr);
	free(send), send = NULL;

	waitmsgbar(7, 0, _("Connect with Hoster wait 7 seconds"), 1);

	if(ostrstr(tmpstr, "The file is being transfered to our other servers. This may take few minutes.") != NULL)
	{
		textbox(_("Message"), _("The file is being transfered to our other servers. This may take few minutes.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	pos1 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"hash\" value=");
	hashline = getxmlentry(pos1, "value=");

	pos2 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"expires\" value=");
	expires = getxmlentry(pos2, "value=");

	pos3 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"timestamp\" value=");
	timestamp = getxmlentry(pos3, "value=");

	free(tmpstr); tmpstr = NULL;
	debug(99, "hashline: %s", hashline);
	if(hashline == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	hash = ostrcat(hash, "timestamp=", 1, 0);
	hash = ostrcat(hash, timestamp, 1, 0);
	hash = ostrcat(hash, "&expires=", 1, 0);	
	hash = ostrcat(hash, expires, 1, 0);
	hash = ostrcat(hash, "&hash=", 1, 0);
	hash = ostrcat(hash, hashline, 1, 0);
	debug(99, "hash: %s", hash);
	hashlen = oitoa(strlen(hash));

	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nReferer: ", 1, 0);
	send = ostrcat(send, link, 1, 0);
	send = ostrcat(send, "\r\nX-Requested-With: XMLHttpRequest", 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/shared2_post", NULL, NULL, NULL, tmpstr);

	streamlink = string_resub("data-url=\"", "\"", tmpstr, 0);
	debug(99, "streamlink1: %s", streamlink);
	htmldecode(streamlink, streamlink);
	debug(99, "streamlink2: %s", streamlink);
	
end:
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(hashline); hashline = NULL;
	free(hash), hash = NULL;
	free(hashlen), hashlen = NULL;
	free(expires), expires = NULL;
	free(timestamp), timestamp = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
