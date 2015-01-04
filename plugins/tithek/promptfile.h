#ifndef PROMPTFILE_H
#define PROMPTFILE_H

char* promptfile(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *error = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *id = NULL, *fname = NULL, *op = NULL, *hash = NULL, *hashlen = NULL, *ip = NULL;
	char* post = NULL, *streamlink = NULL;
//	char* cmd = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/promptfile1_get");
	unlink("/tmp/promptfile2_post");
	unlink("/tmp/promptfile3_tmpstr1");
	unlink("/tmp/promptfile4_post2");
	unlink("/tmp/promptfile5_tmpstr2");
	unlink("/tmp/promptfile6_b36code2");
	unlink("/tmp/promptfile7_tmpstr_last");
	unlink("/tmp/promptfile8_streamlink");

/////////////
	char* tmplink = NULL, *pos = NULL, *path = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

	debug(99, "tmplink: %s", tmplink);

	char* referer = NULL;
//	htmldecode(referer, tmplink);
	referer = htmlencode(tmplink);
	referer = string_replace_all("/", "%2F", referer, 1);
	debug(99, "referer: %s", referer);
	
	
	if(ostrstr(link, "/Out/?s=") != NULL)
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
	tmphost = ostrcat("www.", host, 0, 0);
	tmppath = ostrcat("/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "test host only: %s", get_ip(host));
	debug(99, "tmppath: %s", tmppath);
*/

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

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/promptfile1_get", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	char* chash = string_resub("name=\"chash\" value=\"", "\"", tmpstr, 0);
	hash = ostrcat(hash, "chash=", 1, 0);
	hash = ostrcat(hash, chash, 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));

	char* cokkie = string_resub("Set-Cookie: ", ";", tmpstr, 0);

	//create send string
	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);

	send = ostrcat(send, " HTTP/1.0\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);	
	send = ostrcat(send, "\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:30.0) Gecko/20100101 Firefox/30.0", 1, 0);
	send = ostrcat(send, "\r\nConnection: close", 1, 0);
	send = ostrcat(send, "\r\nReferer: ", 1, 0);
	send = ostrcat(send, link, 1, 0);
	send = ostrcat(send, "\r\nCookie: ", 1, 0);
	send = ostrcat(send, cokkie, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded", 1, 0);
	send = ostrcat(send, "\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	free(cokkie), cokkie = NULL;
	
	post = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
	free(send), send = NULL;
	debug(99, "post: %s", post);
	titheklog(debuglevel, "/tmp/promptfile2_post", NULL, NULL, NULL, tmpstr);

	if(post == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	free(tmpstr),tmpstr = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/promptfile3_tmpstr1", NULL, NULL, NULL, tmpstr);
	
	post = string_replace_all(tmpstr, "", post, 1);
	post = string_replace_all(";return p}(');'", "", post, 1);
	debug(99, "post: %s", post);
	titheklog(debuglevel, "/tmp/promptfile4_post2", NULL, NULL, NULL, post);

	streamlink = string_resub("url: '", "',", post, 0);

	titheklog(debuglevel, "/tmp/promptfile8_streamlink", NULL, NULL, NULL, streamlink);

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
