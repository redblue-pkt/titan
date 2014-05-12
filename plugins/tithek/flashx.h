#ifndef FLASHX_H
#define FLASHX_H

char* flashx(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *streamlink = NULL, *ip = NULL;
	char* playurl = NULL, *tmpstr2 = NULL, *hash1tmp = NULL, *hash2tmp = NULL, *hash = NULL, *hash1 = NULL, *hash2 = NULL, *error = NULL, *cookie1 = NULL, *cookie2 = NULL, *cookie3 = NULL, *tmphost1 = NULL;


/////////////
	char* tmplink = NULL, *pos = NULL, *path = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(tmplink, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}
	

	if(tmplink == NULL || ostrncmp("http://", tmplink, 7))
	{
		textbox(_("Message"), _("Hoster Url not http://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("http://", "", tmplink, 0);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/", path, 0, 0);
//	free(path), path = NULL;

/////////////


/*
	if(host == NULL || file == NULL) return NULL;

	ip = string_replace("http://", "", (char*)hosterurl, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}
	
	tmphost = ostrcat(ip, NULL, 0, 0);
	tmppath = ostrcat("/", path, 0, 0);
*/
	
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "#############################################################################################################");
	debug(99, "send1: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get1", NULL, NULL, NULL, tmpstr);
 
	if(tmpstr == NULL || ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}
	
	cookie1 = string_resub("Set-Cookie: pageredir=", ";", tmpstr, 0);
	debug(99, "cookie1 (pageredir): %s", cookie1);

	cookie2 = string_resub("Set-Cookie: PHPSESSID=", ";", tmpstr, 0);
	debug(99, "cookie2 (PHPSESSID): %s", cookie2);

	cookie3 = string_resub("Set-Cookie: video_", "=", tmpstr, 0);
	debug(99, "cookie3 (vid): %s", cookie3);
	
	if(cookie3 == NULL)
	{
		debug(99, "use url (vid): %s", cookie3);	
		cookie3 = string_resub("http://flashx.tv/video/", "/", tmpstr, 0);
	}

	free(tmppath), tmppath = NULL;
	tmppath = ostrcat("/player/embed.php?vid=", cookie3, 0, 0);
	free(tmphost), tmphost = NULL;
	tmphost1 = oregex(".*<iframe width=.*src=\"(http://.*)&width.*", tmpstr);
	tmphost = string_resub("http://", "/", tmphost1, 0);
	free(tmphost1), tmphost1 = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);	
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n", 1, 0);

	send = ostrcat(send, "Cookie: pageredir=", 1, 0);	
	send = ostrcat(send, cookie1, 1, 0);
	send = ostrcat(send, "; PHPSESSID=", 1, 0);	
	send = ostrcat(send, cookie2, 1, 0);	
	send = ostrcat(send, "\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "#############################################################################################################");
	debug(99, "send2: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get2", NULL, NULL, NULL, tmpstr);
	debug(99, "tmpstr: %s", tmpstr);

	if(tmpstr == NULL || ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	hash1tmp = string_resub("<input type=\"hidden\" name=\"sec\" value=\"", "\"", tmpstr, 0);
	hash2tmp = string_resub("<input type=\"hidden\" name=\"id\" value=\"", "\"", tmpstr, 0);
	
	if(hash1tmp == NULL || hash2tmp == NULL) goto end;
	hash1 = htmlencode(hash1tmp);
	hash2 = htmlencode(hash2tmp);
	free(hash1tmp); hash1tmp = NULL;
	free(hash2tmp); hash2tmp = NULL;
	
	// htmldecod cant / to %2f
	hash1 = string_replace_all("/", "%2F", hash1, 1);
	hash2 = string_replace_all("/", "%2F", hash2, 1);

	hash = ostrcat("sec=", hash1, 0, 0);
	hash = ostrcat(hash, "&id=", 0, 0);
	hash = ostrcat(hash, hash2, 0, 0);
	char* hashlen = NULL;

	if(hash == NULL) goto end;
	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;

//	send = ostrcat(send, "POST /player/show.php HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, "POST /player/playit.php HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: pageredir=", 1, 0);
	send = ostrcat(send, cookie1, 1, 0);
	send = ostrcat(send, "; PHPSESSID=", 1, 0);	
	send = ostrcat(send, cookie2, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);	
	send = ostrcat(send, hash, 1, 0);
	free(hash); hash = NULL;
	free(hashlen); hashlen = NULL;

	debug(99, "#############################################################################################################");
	debug(99, "send3: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, "/player/show.php", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get3", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL || ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	playurl = string_resub("?config=", "\"", tmpstr, 0);
	free(tmppath), tmppath = NULL;
	tmppath = ostrcat("/nuevo/player/player.swf?config=", playurl, 0, 0);
// needed in post
//	free(playurl), playurl = NULL;

	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n", 1, 0);
	send = ostrcat(send, "Cookie: pageredir=", 1, 0);	
	send = ostrcat(send, cookie1, 1, 0);
	send = ostrcat(send, "; PHPSESSID=", 1, 0);	
	send = ostrcat(send, cookie2, 1, 0);	
	send = ostrcat(send, "\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "#############################################################################################################");
	debug(99, "send4: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get4", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL || ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	free(tmppath), tmppath = NULL;
	tmpstr2 = ostrcat("http://", tmphost, 0, 0);
	tmppath = string_replace(tmpstr2, "", playurl, 0);
	free(tmpstr2); tmpstr2 = NULL;		
	free(playurl), playurl = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n", 1, 0);
	send = ostrcat(send, "Cookie: pageredir=", 1, 0);	
	send = ostrcat(send, cookie1, 1, 0);
	send = ostrcat(send, "; PHPSESSID=", 1, 0);	
	send = ostrcat(send, cookie2, 1, 0);	
	send = ostrcat(send, "\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "#############################################################################################################");
	debug(99, "send6: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get5", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL || ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	streamlink = string_resub("<file>", "</file>", tmpstr, 0);
	titheklog(debuglevel, "flashx_streamlink", NULL, NULL, NULL, tmpstr);

end:
	free(error); error = NULL;
	free(tmphost); tmphost = NULL;
	free(tmphost); tmphost = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
