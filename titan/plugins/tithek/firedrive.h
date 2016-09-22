
#ifndef FIREDRIVE_H
#define FIREDRIVE_H

char* firedrive(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *hash = NULL, *hashstr = NULL, *hashlen = NULL;
	char* ip = NULL, *streamlink = NULL, *streamlink1 = NULL, *tmpstr2 = NULL;
	char* srvid = NULL, *cauth = NULL, *cfduid = NULL, *confirm = NULL;

	unlink("/tmp/firedrive1_tmpstr_get");
	unlink("/tmp/firedrive2_tmpstr_post");
	unlink("/tmp/firedrive3_tmpstr_get");
	unlink("/tmp/firedrive4_streamlink");

	if(link == NULL) return NULL;

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
	tmphost = ostrcat("www.firedrive.com", NULL, 0, 0);
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
	titheklog(debuglevel, "/tmp/firedrive1_tmpstr_get", NULL, NULL, NULL, tmpstr);
	
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

	srvid = string_resub("SRVID=", ";", tmpstr, 0);
	cauth = string_resub("cauth=", ";", tmpstr, 0);
	cfduid = string_resub("__cfduid=", ";", tmpstr, 0);
	confirm = string_resub("name=\"confirm\" value=\"", "\"", tmpstr, 0);

	if(confirm == NULL) goto end;

	hashstr = htmlencode(confirm);
	hashstr = string_replace_all("/", "%2F", hashstr, 1);

	hash = ostrcat(hash, "confirm=", 1, 0);	
	hash = ostrcat(hash, hashstr, 1, 0);
	debug(99, "hash: %s", hash);

	if(hash == NULL) goto end;

	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);

	//create send string
	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: ", 1, 0);
	send = ostrcat(send, " SRVID=", 1, 0);
	send = ostrcat(send, srvid, 1, 0);
	send = ostrcat(send, "; cauth=", 1, 0);
	send = ostrcat(send, cauth, 1, 0);
	send = ostrcat(send, "; __cfduid=", 1, 0);
	send = ostrcat(send, cfduid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

//	waitmsgbar(16, 0, _("Connect with Hoster wait 16 seconds"), 1);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send); send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/firedrive2_tmpstr_post", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "This file is private and only viewable by the owner.") != NULL)
	{
		textbox(_("Message"), _("This file is private and only viewable by the owner.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
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

	streamlink = string_resub("file: '", "'", tmpstr, 0);
	titheklog(debuglevel, "/tmp/firedrive4_streamlink", NULL, NULL, NULL, streamlink1);

	if(streamlink == NULL)
	{
		streamlink = string_resub("file: loadURL('", "'", tmpstr, 0);
		titheklog(debuglevel, "/tmp/firedrive5_streamlink", NULL, NULL, NULL, streamlink1);
	}

	if(streamlink == NULL)
	{
		streamlink = string_resub("create_PRO_Modal('", "'", tmpstr, 0);
		titheklog(debuglevel, "/tmp/firedrive6_streamlink", NULL, NULL, NULL, streamlink1);
	}

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
	free(cauth); cauth = NULL;
	free(cfduid); cfduid = NULL;
	free(confirm); confirm = NULL;
	free(srvid); srvid = NULL;

	return streamlink;
}

#endif
