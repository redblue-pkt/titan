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

	if(link == NULL) return NULL;

	unlink("/tmp/thefile1_get");
	unlink("/tmp/thefile2_post");
	
	char* tmplink = NULL, *pos = NULL, *path = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

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
/*
15:02:11 T:3492  NOTICE: '
GET /4x8bkdffgidh HTTP/1.1\r\n
Host: thefile.me\r\n
User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n
Connection: close\r\n
Accept-Encoding: gzip\r\n\r\n'
*/

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
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/thefile1_get", NULL, NULL, NULL, tmpstr);
	free(send), send = NULL;

//	waitmsgbar(7, 0, _("Connect with Hoster wait 7 seconds"), 1);

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
//	if(hashline == NULL)
//	{
//		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
//		goto end;
//	}

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

/*
15:02:11 T:3492  NOTICE: '
POST /4x8bkdffgidh 
HTTP/1.1\r\n
Content-Length: 164\r\n
Accept-Encoding: gzip\r\n
Host: thefile.me\r\n
User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n
Connection: close\r\n
Referer: http://thefile.me/4x8bkdffgidh\r\n
Cookie: __cfduid=ddc102e72b9b793355a5be3d3dcb03ff01412168530377\r\n
Content-Type: application/x-www-form-urlencoded\r\n\r\n
redirect=&usr_login=&referer=http%3A%2F%2Fthefile.me%2F4x8bkdffgidh&fname=X-Men.2000.720p.BrRip.x264.YIFY.mp4&method_free=Free+Download&id=4x8bkdffgidh&op=download1'
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
	debug(99, "send: %s", send);
	free(hash), hash = NULL;

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/thefile2_post", NULL, NULL, NULL, tmpstr);
/*
///////////
15:02:11 T:3492  NOTICE: '
POST /4x8bkdffgidh 
HTTP/1.1\r\n
Content-Length: 220\r\n
Accept-Encoding: gzip\r\n
Host: thefile.me\r\n
User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n
Connection: close\r\n
Referer: http://thefile.me/4x8bkdffgidh\r\n
Cookie: aff=6304; __cfduid=ddc102e72b9b793355a5be3d3dcb03ff01412168530377\r\n
Content-Type: application/x-www-form-urlencoded\r\n\r\n
redirect=&rand=aivaddwv4wpeil6z72doczmarkitsvau3x52gvi&btn_download=Create+Download+Link&method_premium=&down_direct=1&referer=http%3A%2F%2Fthefile.me%2F4x8bkdffgidh&method_free=Free+Download&id=4x8bkdffgidh&op=download2'

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

	hash = ostrcat(hash, "redirect=", 1, 0);
	hash = ostrcat(hash, redirect, 1, 0);
	hash = ostrcat(hash, "&rand=", 1, 0);	
	hash = ostrcat(hash, rand, 1, 0);
	hash = ostrcat(hash, "&btn_download=Create+Download+Link&method_premium=&down_direct=1&referer", 1, 0);
	char* referer_encode = htmldecode(referer);
	hash = ostrcat(hash, referer_encode, 1, 0);
	hash = ostrcat(hash, "&method_free=Free+Download", 1, 0);
//	hash = ostrcat(hash, method_free, 1, 0);
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
	send = ostrcat(send, "\r\nX-Requested-With: XMLHttpRequest", 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/thefile2_post", NULL, NULL, NULL, tmpstr);
///////

	streamlink = string_resub("data-url=\"", "\"", tmpstr, 0);
	debug(99, "streamlink1: %s", streamlink);
	htmldecode(streamlink, streamlink);
	debug(99, "streamlink2: %s", streamlink);
*/	
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
