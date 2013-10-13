#ifndef FLASHX_H
#define FLASHX_H

char* flashx(char* host, char* file, char* hosterurl)
{
	debug(99, "host: %s", host);
	debug(99, "file: %s", file);
	debug(99, "hosterurl: %s", hosterurl);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *streamlink = NULL, *ip = NULL, *pos = NULL, *path = NULL, *error = NULL;

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
	
//printf("\n#######################################################\n");
//printf("GET /video/UX98AKXU2ARB/RiddickUeberlebenistseineRacheTSLDGermanX264-AOE\n");
//printf("tmphost: %s\n",tmphost);
//printf("tmppath: %s\n",tmppath);
//printf("\n#######################################################\n");
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get1", NULL, tmpstr);
	writesys("/var/usr/local/share/titan/plugins/tithek//flashx_tmpstr_get1", tmpstr, 0);
//printf("\n#######################################################\n");
 
	if(ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}
	
	char* cookie1 = string_resub("Set-Cookie: pageredir=", ";", tmpstr, 0);
	debug(99, "cookie1: %s", cookie1);

	char* cookie2 = string_resub("Set-Cookie: PHPSESSID=", ";", tmpstr, 0);
	debug(99, "cookie2: %s", cookie2);

	char* cookie3 = string_resub("Set-Cookie: video_", "=", tmpstr, 0);
	debug(99, "cookie3: %s", cookie3);
	
	free(tmppath), tmppath = NULL;
	tmppath = ostrcat("/player/embed.php?vid=", cookie3, 0, 0);
	free(tmphost), tmphost = NULL;
	char* tmphost1 = oregex(".*<iframe width=.*src=\"(http://.*)&width.*", tmpstr);
	tmphost = string_resub("http://", "/", tmphost1, 0);
	free(tmphost1), tmphost1 = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
//	sleep(1);
//printf("\n#######################################################\n");
//printf("GET /player/embed.php?vid=1008580\n");
//printf("tmphost: %s\n",tmphost);
//printf("tmppath: %s\n",tmppath);
//printf("\n#######################################################\n");
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
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

	titheklog(debuglevel, "/tmp/flashx_tmpstr_get2", NULL, tmpstr);
	writesys("/var/usr/local/share/titan/plugins/tithek//flashx_tmpstr_get2", tmpstr, 0);
//printf("\n#######################################################\n");

	char* hash1tmp = string_resub("<input type=\"hidden\" name=\"sec\" value=\"", "\"", tmpstr, 0);
	char* hash2tmp = string_resub("<input type=\"hidden\" name=\"id\" value=\"", "\"", tmpstr, 0);
	
	if(hash1tmp == NULL || hash2tmp == NULL) goto end;
	char* hash1 = htmlencode(hash1tmp);
	char* hash2 = htmlencode(hash2tmp);
	free(hash1tmp); hash1tmp = NULL;
	free(hash2tmp); hash2tmp = NULL;
	
	// htmldecod cant / to %2f
//	hash1 = string_replace_all("/", "%2F", hash1, 1);
//	hash2 = string_replace_all("/", "%2F", hash2, 1);

	char* hash = ostrcat("sec=", hash1, 0, 0);
	hash = ostrcat(hash, "&id=", 0, 0);
	hash = ostrcat(hash, hash2, 0, 0);
	char* hashlen = NULL;

	if(hash == NULL) goto end;
	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);

	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
//	sleep(1);

//printf("\n#######################################################\n");
//printf("POST /player/show.php\n");
//printf("\n#######################################################\n");	

	send = ostrcat(send, "POST /player/show.php HTTP/1.1\r\nContent-Length: ", 1, 0);
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
	
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, "/player/show.php", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

	titheklog(debuglevel, "/tmp/flashx_tmpstr_get3", NULL, tmpstr);
	writesys("/var/usr/local/share/titan/plugins/tithek//flashx_tmpstr_get3", tmpstr, 0);
//printf("\n#######################################################\n");
//var code ='<object id="nuevoplayer" width="'+ww+'" height="'+hh+'" data="http://play.flashx.tv/nuevo/player/player.swf?config=http://play.flashx.tv/nuevo/player/play.php?str=4MfrzrW4qrPKnM2dw5/Fvcs=" type="application/x-shockwave-flash">';
	char* playurl = string_resub("?config=", "\"", tmpstr, 0);
	free(tmppath), tmppath = NULL;
	tmppath = ostrcat("/nuevo/player/player.swf?config=", playurl, 0, 0);
//	free(playurl), playurl = NULL;

	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
//	sleep(1);

//printf("\n#######################################################\n");
//printf("GET /nuevo/player/player.swf?config=http://play.flashx.tv/nuevo/player/play.php?str=4MfrzrW70J.wp8O.zZi5uLo=\n");
//printf("tmphost: %s\n",tmphost);
//printf("tmppath: %s\n",tmppath);
//printf("\n#######################################################\n");	

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


	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

	titheklog(debuglevel, "/tmp/flashx_tmpstr_get4", NULL, tmpstr);
	writesys("/var/usr/local/share/titan/plugins/tithek//flashx_tmpstr_get4", tmpstr, 0);
//printf("\n#######################################################\n");



//printf("tmppath: %s\n",tmppath);
//printf("playurl: %s\n",playurl);
	free(tmppath), tmppath = NULL;
//	tmppath = ostrcat("/nuevo/player/play.php?str=4MfrzrW4qrPKnM2dw5/Fvcs=", NULL, 0, 0);
//	tmppath = string_resub("/play.php?", "\0", tmpstr, 0);
	char* tmpstr2 = ostrcat("http://", tmphost, 0, 0);
	tmppath = string_replace(tmpstr2, "", playurl, 0);

	free(tmpstr2); tmpstr2 = NULL;		
	free(playurl), playurl = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
//	sleep(1);
	
//printf("\n#######################################################\n");
//printf("GET /nuevo/player/play.php?str=4MfrzrW70J.wp8O.zZi5uLo=\n");
//printf("tmphost: %s\n",tmphost);
//printf("tmppath: %s\n",tmppath);

//printf("\n#######################################################\n");	

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


	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/flashx_tmpstr_get5", tmpstr, 0);
	writesys("/var/usr/local/share/titan/plugins/tithek//flashx_tmpstr_get5", tmpstr, 0);
//printf("\n#######################################################\n");

	streamlink = string_resub("<file>", "</file>", tmpstr, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/flashx5_streamlink", streamlink, 0);

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
