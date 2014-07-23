#ifndef FIREDRIVE_H
#define FIREDRIVE_H

char* firedrive(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *hash = NULL, *hashstr = NULL, *hashlen = NULL;
	char* ip = NULL, *cmd = NULL, *phpsessid = NULL, *serverid = NULL, *usender = NULL, *streamlink = NULL, *streamlink1 = NULL, *tmpstr2 = NULL;

	unlink("/tmp/firedrive1_tmpstr_get");
	unlink("/tmp/firedrive2_tmpstr_post");
	unlink("/tmp/firedrive3_tmpstr_get");
	unlink("/tmp/firedrive4_streamlink");

	if(link == NULL) return NULL;

/////////////
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

tmphost = ostrcat("www.firedrive.com", NULL, 0, 0);

	tmppath = string_replace_all("/embed/", "/file/", tmppath, 1);
//17:14:39 T:2396  NOTICE: 'GET /file/E340EF13D664B113 HTTP/1.1\r\nHost: www.firedrive.com\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
//17:14:39 T:2396  NOTICE: 'POST /file/E340EF13D664B113 HTTP/1.1\r\nContent-Length: 106\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.firedrive.com\r\nCookie: SRVID=02; cauth=a7211240297e38d448cba9bcf77908dd; __cfduid=d9cf5b4767cf7b2a0ff02d5449547407d1406128478606\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nconfirm=1bDVI%2BEPCKM1nfpZNyZ3e8VkVMMQfbM7dYSYIGPG592z36Ze3TbLsvSLouQbWUPM2EsvDxvvn2riaxOxm%2B%2B2lQ%3D%3D'

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

	if(ostrstr(tmpstr, "11111111111domain=.firedrive.com") != NULL)
	{
		textbox(_("Message"), _("Found Firedrive.com Link, supporting comming soon..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

char* srvid = string_resub("SRVID=", ";", tmpstr, 0);
char* cauth = string_resub("cauth=", ";", tmpstr, 0);
char* cfduid = string_resub("__cfduid=", ";", tmpstr, 0);
char* confirm = string_resub("name=\"confirm\" value=\"", "\"", tmpstr, 0);

printf("11aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
printf("confirm: %s\n", confirm);

//htmldecode(confirm, confirm);

char* confirm1 = NULL;
confirm1 = ostrcat(confirm1, "2OyC1rnlqPAp9ZjR3L/Q2Ab6Dp5NiWW8cb+ZzpVQDuStJ4/DzQUrpA1kd41qZxt9rQe40DcX/dQYIXaMPXfGKA==", 1, 0);
printf("confirm1: %s\n",confirm1);

char* hashstr1 = htmlencode(confirm1);
hashstr1 = string_replace_all("/", "%5C", hashstr1, 1);

	char* hash1 = ostrcat(hash1, "confirm=", 1, 0);	
	hash1 = ostrcat(hash1, hashstr1, 1, 0);
	debug(99, "hash1: %s", hash1);

	char* hashlen1 = oitoa(strlen(hash1));
printf("hashlen1: %s\n",hashlen1);



hashstr = htmlencode(confirm);

hashstr = string_replace_all("/", "%5C", hashstr, 1);
printf("hashstr: %s\n", hashstr);

	hash = ostrcat(hash, "confirm=", 1, 0);	
	hash = ostrcat(hash, hashstr, 1, 0);
//	hash = ostrcat(hash, "&confirm=Continue+as+Free+User", 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);
	
/*
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
//	if(hashstr == NULL) goto end;

	hash = ostrcat(hash, "hash=", 1, 0);	
	hash = ostrcat(hash, hashstr, 1, 0);
	hash = ostrcat(hash, "&confirm=Continue+as+Free+User", 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);
*/
/*
Set-Cookie: __cfduid=d9a671dab77ba110fa43804657911e9af1406130120143; expires=Mon, 23-Dec-2019 23:50:00 GMT; path=/; domain=.firedrive.com; HttpOnly
Set-Cookie: cauth=1c9382bf72c1f4af61236a27ff442339; expires=Thu, 23-Jul-2015 15:42:00 GMT; path=/; domain=.firedrive.com
Set-Cookie: lang=en; path=/; domain=.
Set-Cookie: lang=en; path=/; domain=.
Set-Cookie: lang=en; path=/; domain=.
Set-Cookie: lang=en; path=/; domain=.
Vary: Accept-Encoding
Set-Cookie: SRVID=21; path=/
CF-RAY: 14e8fc42e69502dc-AMS
Content-Encoding: gzip
*/

/*
//17:14:39 T:2396  NOTICE: '
POST /file/E340EF13D664B113 
HTTP/1.1\r\nContent-Length: 106\r\n
Accept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: 

www.firedrive.com\r\nCookie: 
SRVID=02; cauth=a7211240297e38d448cba9bcf77908dd; __cfduid=d9cf5b4767cf7b2a0ff02d5449547407d1406128478606\r\n
Content-Type: application/x-www-form-urlencoded\r\n\r\n
confirm=1bDVI%2BEPCKM1nfpZNyZ3e8VkVMMQfbM7dYSYIGPG592z36Ze3TbLsvSLouQbWUPM2EsvDxvvn2riaxOxm%2B%2B2lQ%3D%3D'
*/

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

//	send = ostrcat(send, "; cauth=", 1, 0);
//	send = ostrcat(send, cauth, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	waitmsgbar(12, 0, _("Connect with Hoster wait 12 seconds"), 1);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send); send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/firedrive2_tmpstr_post", NULL, NULL, NULL, tmpstr);

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

	streamlink = string_resub("file: '", "'", tmpstr, 0);
	titheklog(debuglevel, "/tmp/firedrive4_streamlink", NULL, NULL, NULL, streamlink1);

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

	return streamlink;
}

#endif
