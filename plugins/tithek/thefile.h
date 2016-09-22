#ifndef THEFILE_H
#define THEFILE_H

char* thefile(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *ip = NULL, *streamlink = NULL, *timestamp = NULL;
	char* hash = NULL, *hashlen = NULL, *pos1 = NULL, *pos2 = NULL, *pos3 = NULL, *hashline = NULL, *expires = NULL;
	char* usr_login = NULL, *referer = NULL, *fname = NULL, *op = NULL, *id = NULL, *cfduid = NULL;
	char* pos4 = NULL, *pos5 = NULL, *pos6 = NULL, *redirect = NULL, *method_free = NULL, *rand = NULL;
	char* pos7 = NULL, *pos8 = NULL, *pos9 = NULL, *pos10 = NULL, *pos11 = NULL, *pos12 = NULL;
	char* streamlink1 = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/thefile1_get");
	unlink("/tmp/thefile2_post");
	unlink("/tmp/thefile3_post");
		
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
	
	debug(99, "1send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/thefile1_get", NULL, NULL, NULL, tmpstr);
	free(send), send = NULL;

	if(ostrstr(tmpstr, "The file is being transfered to our other servers. This may take few minutes.") != NULL)
	{
		textbox(_("Message"), _("The file is being transfered to our other servers. This may take few minutes.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	pos1 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"redirect\" value=");
	redirect = getxmlentry(pos1, "value=");

	pos2 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"usr_login\" value=");
	usr_login = getxmlentry(pos2, "value=");

	pos3 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"referer\" value=");
	referer = getxmlentry(pos3, "value=");

	pos4 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"fname\" value=");
	fname = getxmlentry(pos4, "value=");

	pos5 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"id\" value=");
	id = getxmlentry(pos5, "value=");

	pos6 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"op\" value=");
	op = getxmlentry(pos6, "value=");

	cfduid = string_resub("__cfduid=", ";", tmpstr, 0);

	free(tmpstr); tmpstr = NULL;
	debug(99, "hashline: %s", hashline);

	free(hash), hash = NULL;
	hash = ostrcat(hash, "redirect=", 1, 0);
	hash = ostrcat(hash, redirect, 1, 0);
	hash = ostrcat(hash, "&usr_login=", 1, 0);	
	hash = ostrcat(hash, usr_login, 1, 0);
	hash = ostrcat(hash, "&referer=", 1, 0);
	hash = ostrcat(hash, referer, 1, 0);
	hash = ostrcat(hash, "&fname=", 1, 0);
	hash = ostrcat(hash, fname, 1, 0);
	hash = ostrcat(hash, "&method_free=Free+Download", 1, 0);
	hash = ostrcat(hash, "&id=", 1, 0);
	hash = ostrcat(hash, id, 1, 0);
	hash = ostrcat(hash, "&op=", 1, 0);
	hash = ostrcat(hash, op, 1, 0);
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
	send = ostrcat(send, "\r\nCookie: cfduid=", 1, 0);
	send = ostrcat(send, cfduid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "2send: %s", send);
	free(hash), hash = NULL;

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/thefile2_post", NULL, NULL, NULL, tmpstr);

	pos7 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"redirect\" value=");
	redirect = getxmlentry(pos7, "value=");

	pos8 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"rand\" value=");
	rand = getxmlentry(pos8, "value=");

	pos9 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"referer\" value=");
	referer = getxmlentry(pos9, "value=");

	pos10 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"method_free\" value=");
	method_free = getxmlentry(pos10, "value=");

	pos11 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"id\" value=");
	id = getxmlentry(pos11, "value=");

	pos12 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"op\" value=");
	op = getxmlentry(pos12, "value=");

	free(hash), hash = NULL;
	hash = ostrcat(hash, "redirect=", 1, 0);
	hash = ostrcat(hash, redirect, 1, 0);
	hash = ostrcat(hash, "&rand=", 1, 0);	
	hash = ostrcat(hash, rand, 1, 0);
	hash = ostrcat(hash, "&btn_download=Create+Download+Link&method_premium=&down_direct=1&referer", 1, 0);
	char* referer_encode = htmlencode(referer);
	hash = ostrcat(hash, referer_encode, 1, 0);
	hash = ostrcat(hash, "&method_free=Free+Download", 1, 0);
//	hash = ostrcat(hash, method_free, 1, 0);
	hash = ostrcat(hash, "&id=", 1, 0);
	hash = ostrcat(hash, id, 1, 0);
	hash = ostrcat(hash, "&op=", 1, 0);
	hash = ostrcat(hash, op, 1, 0);
	debug(99, "3hash: %s", hash);
	hashlen = oitoa(strlen(hash));

	free(send), send = NULL;	
/*
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
*/
	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nReferer: ", 1, 0);
	send = ostrcat(send, link, 1, 0);
	send = ostrcat(send, "\r\nCookie: cfduid=", 1, 0);
	send = ostrcat(send, cfduid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "3send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/thefile3_post", NULL, NULL, NULL, tmpstr);
///////

	if(ostrstr(tmpstr, "This direct link will be available for your IP next 8 hours") != NULL)
		textbox(_("Message"), _("This direct link will be available for your IP next 8 hours") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 5);

	streamlink1 = string_resub("<h3 class=\"x\"><span>", "</span></h3>", tmpstr, 0);
	streamlink = string_resub("<a href=\"", "</a>", streamlink1, 0);

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
	free(streamlink1), streamlink1 = NULL;
	free(redirect), redirect = NULL;
	free(rand), rand = NULL;
	free(referer), referer = NULL;
	free(method_free), method_free = NULL;
	free(op), op = NULL;
	free(id), id = NULL;
	free(redirect), redirect = NULL;
	free(cfduid), cfduid = NULL;
	free(usr_login), usr_login = NULL;
	free(fname), fname = NULL;

	return streamlink;
}

#endif
