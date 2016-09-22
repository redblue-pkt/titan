#ifndef movreel_H
#define movreel_H

char* movreel(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *error = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *id = NULL, *fname = NULL, *op = NULL, *hash = NULL, *hashlen = NULL, *ip = NULL, *streamlink = NULL, *rand = NULL;
	
	if(link == NULL) return NULL;

	unlink("/tmp/movreel1_get");
	unlink("/tmp/movreel2_post");
	unlink("/tmp/movreel3_post");
	unlink("/tmp/movreel4_streamlink");
/////////////
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

	if(!ostrncmp("www.", tmphost, 7))
		tmphost = ostrcat("www.", tmphost, 0, 1);
	tmppath = string_replace(".html", "", tmppath, 1);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.0", 1, 0);
	send = ostrcat(send, "\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
	send = ostrcat(send, "\r\nConnection: close", 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);	
	send = ostrcat(send, "\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 15000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movreel1_get", NULL, NULL, NULL, tmpstr);
	char* cokkie = string_resub("Set-Cookie: ", ";", tmpstr, 0);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.0", 1, 0);
	send = ostrcat(send, "\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
	send = ostrcat(send, "\r\nCookie: ", 1, 0);
	send = ostrcat(send, cokkie, 1, 0);
	send = ostrcat(send, "\r\nConnection: close", 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);	
	send = ostrcat(send, "\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 15000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movreel2_get", NULL, NULL, NULL, tmpstr);

	char* pos4 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"id\" value=");
	id = getxmlentry(pos4, "value=");
	debug(99, "id: %s", id);
	if(id == NULL) goto end;

	char* pos5 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"rand\" value=");
	rand = getxmlentry(pos5, "value=");
	debug(99, "rand: %s", rand);
	if(rand == NULL) goto end;

	char* pos6 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"op\" value=");
	op = getxmlentry(pos6, "value=");
	debug(99, "op: %s", op);
	if(op == NULL) goto end;

	hash = ostrcat(hash, "rand=", 1, 0);
	hash = ostrcat(hash, rand, 1, 0);
	hash = ostrcat(hash, "&referer=%22&method_premium=&down_script=1&method_free=%22&id=", 1, 0);
	hash = ostrcat(hash, id, 1, 0);
	hash = ostrcat(hash, "&op=", 1, 0);
	hash = ostrcat(hash, op, 1, 0);

	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	
	//create send string
	send = ostrcat(send, "POST /", 1, 0);
	send = ostrcat(send, id, 1, 0);
	send = ostrcat(send, " HTTP/1.0\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);

	send = ostrcat(send, "\r\nCookie: ", 1, 0);
	send = ostrcat(send, cokkie, 1, 0);

	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);
	free(hash), hash = NULL;
	free(cokkie), cokkie = NULL;

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 15000, 0);
	free(send), send = NULL;
	debug(99, "post: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movreel3_post", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	char* streamlink1 = string_resub("<span style=", "</span>", tmpstr, 0);		
	streamlink = string_resub("<a href=\"", "\"", streamlink1, 0);		
	free(streamlink1); streamlink1 = NULL;

	titheklog(debuglevel, "/tmp/movreel4_streamlink", NULL, NULL, NULL, streamlink);

	free(tmpstr); tmpstr = NULL;

end:

	free(error); error = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(hash), hash = NULL;
	free(hashlen), hashlen = NULL;
	free(op), op = NULL;
	free(id), id = NULL;
	free(fname), fname = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
