#ifndef NOVAMOV_H
#define NOVAMOV_H

char* novamov(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL;
	char* tmppath = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;
	char* filekey = NULL;
	char* file = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/novamov1_get");
	unlink("/tmp/novamov2_get");
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
	tmphost = ostrcat(tmphost, ".com", 1, 0);

	tmppath = ostrcat("/video/", file, 0, 0);
	
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmppath: %s", tmppath);
*/

	if(ostrstr(link, "http://file.") != NULL)
		tmphost = string_replace("file.", "www.", tmphost, 1);

	if(ostrstr(link, "http://embed.") != NULL)
		tmphost = string_replace("embed.", "www.", tmphost, 1);

	if(ostrstr(link, "embed.php?v=") != NULL)
	{
		tmppath = string_replace("embed.php?v=", "video/", tmppath, 1);
		tmppath = stringreplacecharonce(tmppath, '&', '\0');
	}

	if(ostrstr(link, "auroravid.to/video/") != NULL)
		tmppath = string_replace("/video/", "/embed/?v=", tmppath, 1);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/novamov1_get", NULL, NULL, NULL, tmpstr);

	if(ostrstr(tmpstr, "The file is being transfered to our other servers. This may take few minutes.") != NULL)
	{
		textbox(_("Message"), _("The file is being transfered to our other servers. This may take few minutes.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "This file no longer exists on our servers!") != NULL)
	{
		textbox(_("Message"), _("This file no longer exists on our servers!") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	file = string_resub("flashvars.file=\"", "\";", tmpstr, 0);	
	if(file == NULL)
		file = string_replace("/video/", "", tmppath, 0);
	if(file == NULL)
		file = string_resub("login.php?return=/video/", "\"", tmpstr, 0);
	if(file == NULL)
		file = string_resub("<a href=\"/share.php?id=", "&title=", tmpstr, 0);
	
	char* r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL;
	pos = ostrstr(tmpstr, ");}('");
	if(pos != NULL)
	{
		r1 = string_resub(");}('", "'", pos, 0);
		pos = ostrstr(pos + 5, ",'");
		if(pos != NULL)
		{
			r2 = string_resub(",'", "'", pos, 0);
			pos = ostrstr(pos + 2, ",'");
			if(pos != NULL)
			{
				r3 = string_resub(",'", "'", pos, 0);
				pos = ostrstr(pos + 2, ",'");
				if(pos != NULL)
					r4 = string_resub(",'", "'", pos, 0);
			}
		}
	}
	
	filekey = getfilekey(r1, r2, r3, r4);
	
	free(r1); r1 = NULL;
	free(r2); r2 = NULL;
	free(r3); r3 = NULL;
	free(r4); r4 = NULL;
	
	if(filekey == NULL)
		filekey = string_resub("flashvars.filekey=\"", "\";", tmpstr, 0);

	if(filekey == NULL)
	{
		char* searchstr = string_resub("flashvars.filekey=", ";", tmpstr, 0);
		debug(99, "searchstr: %s", searchstr);
		searchstr = ostrcat(searchstr, "=\"", 1, 0);
		filekey = string_resub(searchstr, "\";", tmpstr, 0);
	}
	
	debug(99, "filekey: %s", filekey);

	free(tmppath), tmppath = NULL;
	tmppath = ostrcat("/api/player.api.php?file=", file, 0, 0);
	tmppath = ostrcat(tmppath, "&key=", 1, 0);
	tmppath = ostrcat(tmppath, filekey, 1, 0);

	free(send), send = NULL;
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/novamov2_get", NULL, NULL, NULL, tmpstr);

	streamlink = string_resub("url=", "&", tmpstr, 0);
	debug(99, "streamlink1: %s", streamlink);
	htmldecode(streamlink, streamlink);
	debug(99, "streamlink2: %s", streamlink);
	
end:
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(filekey); filekey = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
