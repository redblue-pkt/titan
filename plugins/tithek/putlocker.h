#ifndef PUTLOCKER_H
#define PUTLOCKER_H

char* putlocker(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *hash = NULL, *hashstr = NULL, *hashlen = NULL;
	char* ip = NULL, *cmd = NULL, *phpsessid = NULL, *serverid = NULL, *usender = NULL, *streamlink = NULL, *streamlink1 = NULL, *tmpstr2 = NULL;

	unlink("/tmp/putlocker1_tmpstr_get");
	unlink("/tmp/putlocker2_tmpstr_post");
	unlink("/tmp/putlocker3_tmpstr_get");
	unlink("/tmp/putlocker4_streamlink");

	if(link == NULL) return NULL;

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
//	free(path), path = NULL;

/////////////



/*
	tmphost = ostrcat("www.", host, 0, 0);
	tmppath = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmppath: %s", tmppath);
*/

	tmppath = string_replace_all("/embed/", "/file/", tmppath, 1);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send); send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/putlocker1_tmpstr_get", NULL, NULL, NULL, tmpstr);
	
	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "This file doesn't exist, or has been removed.") != NULL)
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "domain=.firedrive.com") != NULL)
	{
		textbox(_("Message"), _("Found Firedrive.com Link, supporting comming soon..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}
	
	phpsessid = getxmlentry(tmpstr, "PHPSESSID=");
	debug(99, "phpsessid: %s", phpsessid);
	serverid = getxmlentry(tmpstr, "SERVERID=");
	debug(99, "serverid: %s", serverid);
	usender = getxmlentry(tmpstr, "usender=");
	debug(99, "usender: %s", usender);

	//get hash from tmpstr
	char* pos1 = ostrstr(tmpstr, "<input type=\"hidden\" value=");
	hashstr = getxmlentry(pos1, "value=");
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
	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: usender=", 1, 0);
	send = ostrcat(send, usender, 1, 0);
	send = ostrcat(send, " SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, " PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send); send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/putlocker2_tmpstr_post", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	
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
//		streamlink1 = ostrcat("http://www.", host, 0, 0);
		streamlink1 = ostrcat("http://", tmphost, 0, 0);
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
	
	if(!ostrncmp("/get_file.php", streamlink, 13))
	{
		cmd = ostrcat(cmd, "wget 'http://", 1, 0);
		cmd = ostrcat(cmd, tmphost, 1, 0);
		cmd = ostrcat(cmd, "/", 1, 0);
		cmd = ostrcat(cmd, streamlink, 1, 0);
		cmd = ostrcat(cmd, "' -O /tmp/putlocker.stream", 1, 0);
		debug(99, "cmd: %s", cmd);
		unlink("/tmp/putlocker.stream");
		
		system(cmd);
		
		free(cmd), cmd = NULL;
		cmd = ostrcat("cat /tmp/putlocker.stream", NULL, 0, 0);
		
		tmpstr = command(cmd);
		free(cmd), cmd = NULL;
	}

/*
	free(tmpstr); tmpstr = NULL;

	if(streamlink == NULL) goto end;
	
	//create send string
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, streamlink, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, "PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nUser-Agent: Python-urllib/2.7\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);
	
	tmpstr = gethttpreal(tmphost, streamlink, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
*/
	free(send); send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/putlocker3_tmpstr_get", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	//get streamlink1
	streamlink1 = getxmlentry(tmpstr, "url=");
	titheklog(debuglevel, "/tmp/putlocker4_streamlink", NULL, NULL, NULL, streamlink1);

	free(tmpstr); tmpstr = NULL;

end:
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
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
