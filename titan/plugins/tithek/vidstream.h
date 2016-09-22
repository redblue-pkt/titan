#ifndef VIDSTREAM_H
#define VIDSTREAM_H

char* vidstream(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL;
	char* tmppath = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;

	char* id = NULL;
	char* fname = NULL;
	char* op = NULL;
	char* hash = NULL;
	char* hashline = NULL;
	char* hashlen = NULL;
	char* referer = NULL;
	char* usr_login = NULL;
	char* imhuman = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/vidstream1_get");
	unlink("/tmp/vidstream2_post");
	unlink("/tmp/vidstream3_streamlink");

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
	tmphost = ostrcat(host, NULL, 0, 0);
	tmppath = ostrcat("/", file, 0, 0);
	ip = get_ip(tmphost);
	debug(99, "tmppath: %s", tmppath);
	debug(99, "tmphost: %s", tmphost);
	debug(99, "ip: %s", ip);
*/
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/vidstream1_get", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "<b>File Not Found</b>") != NULL)
	{
		tmpstr = string_resub("<Center>", "</Center>", tmpstr, 1);
		string_deltags(tmpstr);
		tmpstr = string_replace("Terms", "\nTerms", tmpstr, 1);
		tmpstr = strstrip(tmpstr);
		if(tmpstr == NULL || strlen(tmpstr) == 0)
			tmpstr = ostrcat(_("The page is temporarily unavailable"), tmpstr, 0, 1);
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}
		
	//get hash from tmpstr
	char* pos1 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"fname\" value=");
	fname = getxmlentry(pos1, "value=");
	debug(99, "fname: %s", fname);
	if(fname == NULL) goto end;

	char* pos2 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"id\" value=");
	id = getxmlentry(pos2, "value=");
	debug(99, "id: %s", id);
	if(id == NULL) goto end;

	char* pos3 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"op\" value=");
	op = getxmlentry(pos3, "value=");

	char* pos4 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"hash\" value=");
	hashline = getxmlentry(pos4, "value=");

	char* pos5 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"referer\" value=");
	referer = getxmlentry(pos5, "value=");

	char* pos6 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"imhuman\" value=");
	imhuman = getxmlentry(pos6, "value=");

	char* pos7 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"usr_login\" value=");
	usr_login = getxmlentry(pos7, "value=");
	
	free(tmpstr); tmpstr = NULL;
	debug(99, "op: %s", op);
	if(op == NULL) goto end;

	hash = ostrcat(hash, "hash=", 1, 0);
	hash = ostrcat(hash, hashline, 1, 0);
	hash = ostrcat(hash, "&imhuman=Proceed+to+video%22+id%3D%22btn_download&k=Search...%22+size%3D16+onfocus%3D%22%24%28this%29.val%28%27%27%29.css%28%27color%27%2C%27%23333%27%29%3B%22+onblur%3D%22if%28%24%28this%29.val%28%29%3D%3D%27%27%29%24%28this%29.val%28%27Search...%27%29.css%28%27color%27%2C%27%27%29%22+id%3D%22search_top", 1, 0);
	hash = ostrcat(hash, "&usr_login=", 1, 0);	
	hash = ostrcat(hash, usr_login, 1, 0);
	hash = ostrcat(hash, "&referer=", 1, 0);
	hash = ostrcat(hash, referer, 1, 0);
	hash = ostrcat(hash, "&fname=", 1, 0);
	hash = ostrcat(hash, fname, 1, 0);
	hash = ostrcat(hash, "&id=", 1, 0);	
	hash = ostrcat(hash, id, 1, 0);
	hash = ostrcat(hash, "&op=download1", 1, 0);
//	hash = ostrcat(hash, op, 1, 0);
	debug(99, "hash: %s", hash);
	hashlen = oitoa(strlen(hash));

	//create send string
	free(send), send = NULL;

	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);	
	titheklog(debuglevel, "/tmp/vidstream2_post", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamlink = string_resub("file: \"", "\",", tmpstr, 0);

	titheklog(debuglevel, "/tmp/vidstream3_streamlink", NULL, NULL, NULL, streamlink);

end:

	free(hash), hash = NULL;
	free(hashlen), hashlen = NULL;
	free(op), op = NULL;
	free(id), id = NULL;
	free(fname), fname = NULL;
		
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
