#ifndef STREAMCLOAD_H
#define STREAMCLOAD_H

char* streamcloud(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmpfile = NULL, *tmpstr = NULL, *send = NULL, *id = NULL, *fname = NULL, *op = NULL, *hash = NULL, *cmd = NULL;
	char* hashline = NULL, *hashlen = NULL, *referer = NULL, *usr_login = NULL, *imhuman = NULL, *ip = NULL, *cookie = NULL, *streamlink = NULL;

	unlink("/tmp/streamcould1_get");
	unlink("/tmp/streamcould2_post");
	unlink("/tmp/streamcould3_streamlink");

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat(host, NULL, 0, 0);
	tmpfile = ostrcat("/", file, 0, 0);

	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	sleep(20);
	titheklog(debuglevel, "/tmp/streamcould1_get", NULL, tmpstr);
	
	cookie = string_resub("Set-Cookie: afc=", ";", tmpstr, 0);	
	debug(99, "cookie: %s", cookie);

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
	hash = ostrcat(hash, "&imhuman=Weiter+zum+Video", 1, 0);
	hash = ostrcat(hash, "&usr_login=", 1, 0);	
	hash = ostrcat(hash, usr_login, 1, 0);
	hash = ostrcat(hash, "&referer=", 1, 0);
	hash = ostrcat(hash, referer, 1, 0);
	hash = ostrcat(hash, "&fname=", 1, 0);
	hash = ostrcat(hash, fname, 1, 0);
	hash = ostrcat(hash, "&id=", 1, 0);	
	hash = ostrcat(hash, id, 1, 0);
	hash = ostrcat(hash, "&op=", 1, 0);
	hash = ostrcat(hash, op, 1, 0);
	debug(99, "hash: %s", hash);
	hashlen = oitoa(strlen(hash));

	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: afc=", 1, 0);
	send = ostrcat(send, cookie, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/streamcould2_post", NULL, tmpstr);

//	streamlink = oregex(".*file: \".*(http:.*video.mp4).*\".*", tmpstr);
	streamlink = string_resub("file: \"", "\"", tmpstr, 0);
	titheklog(debuglevel, "/tmp/streamcould3_streamlink", NULL, streamlink);

end:

	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(hash), hash = NULL;
	free(hashlen), hashlen = NULL;
	free(op), op = NULL;
	free(id), id = NULL;
	free(fname), fname = NULL;
	free(ip); ip = NULL;
	free(cookie); cookie = NULL;
	free(cmd); cmd = NULL;	
	return streamlink;
}

#endif
