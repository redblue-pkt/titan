#ifndef XVIDSTAGE_H
#define XVIDSTAGE_H

unsigned char* xvidstagereceive(int* sock)
{
	int ret = 0;
	unsigned char* buf = NULL;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	//read one line
	unsigned char* pbuf = buf;

	while(pbuf - buf < MINMALLOC)
	{
		unsigned char c;

		ret = sockreceive(sock, &c, 1, 5000 * 1000);
		if(ret != 0)
		{
			err("no client data in buffer");
			break;
		}

		*pbuf = c;
		if(buf != NULL && (ostrstr((char*)buf, "\n\n") != NULL || ostrstr((char*)buf, "\r\n\r\n") != NULL))
			break;
		pbuf++;
	}

	return buf;
}

char* xvidstage(char* host, char* file)
{
	debug(99, "in host: %s file: %s", host, file);
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;

	char* cookie = NULL;		
	char* id = NULL;
	char* fname = NULL;
	char* op = NULL;
	char* hash = NULL;
	char* hashline = NULL;
	char* hashlen = NULL;
	char* referer = NULL;
	char* usr_login = NULL;
	char* imhuman = NULL;

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "file: %s", file);


//'GET /bgszal6iipuy HTTP/1.1\r\nHost: www.xvidstage.com\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'

	send = ostrcat(send, "GET /", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: www.", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);

//	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 1);
//	writesys("/var/usr/local/share/titan/plugins/tithek/xvidstage1", tmpstr, 0);
	char* line = NULL;
	char* lines = NULL;
	char* cmd = NULL;
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/get", NULL, NULL, 0, send, NULL, 1);
	sleep(1);
	free(send); send = NULL;
//	system("cp /tmp/tithek/get /var/usr/local/share/titan/plugins/tithek/xvidstage4"); 
	if(!file_exist("/tmp/tithek/get"))
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmpstr = command("cat /tmp/tithek/get");
	cookie = string_resub("Set-Cookie: afc=", ";", tmpstr, 0);	
	debug(99, "cookie: %s", cookie);
	writesys("/var/usr/local/share/titan/plugins/tithek/xvidstage1", tmpstr, 0);

	free(tmpstr), tmpstr = NULL;

///////////////////
	line = string_newline(command("cat /tmp/tithek/get | grep 'Set-Cookie:' -n | tail -n1| cut -d':' -f1"));
	cmd = ostrcat(cmd, "cat /tmp/tithek/get | wc -l", 1, 0);
	lines = string_newline(command(cmd));
	free(cmd); cmd = NULL;
	debug(99, "line: %s", line);
	debug(99, "lines: %s", lines);

	if(lines == NULL || line == NULL) goto end;	
	
	int sline = atoi(lines) - atoi(line) - 4;
	debug(99, "sline: %d", sline);
	debug(99, "sline: %s", oitoa(sline));

	cmd = ostrcat(cmd, "cat /tmp/tithek/get | tail -n", 1, 0);
	cmd = ostrcat(cmd, oitoa(sline), 1, 0);
	cmd = ostrcat(cmd, " | sed '1,1d' | zcat", 1, 0);
	debug(99, "cmd: %s", cmd);	
	tmpstr = command(cmd);
	free(cmd); cmd = NULL;
	free(lines); lines = NULL;
	writesys("/var/usr/local/share/titan/plugins/tithek/xvidstage2", tmpstr, 0);


/////////////
	sleep(1);
///////////////////

	//get hash from tmpstr
	char* pos1 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"fname\" value=");
	fname = getxmlentry(pos1, "value=");
	debug(99, "fname: %s", fname);
//	if(fname == NULL) goto end;

	char* pos2 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"id\" value=");
	id = getxmlentry(pos2, "value=");
	debug(99, "id: %s", id);
	if(id == NULL) goto end;

	char* pos3 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"op\" value=");
	op = getxmlentry(pos3, "value=");

	char* pos4 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"rand\" value=");
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

//'POST /bgszal6iipuy HTTP/1.1\r\nContent-Length: 238\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.xvidstage.com\r\nCookie: aff=4784\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n'

	send = ostrcat(send, "POST /", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nCookie: afc=", 1, 0);
	send = ostrcat(send, cookie, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	free(tmpstr), tmpstr = NULL;
//	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 0);
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/get", NULL, NULL, 0, send, NULL, 0);
	cmd = ostrcat(cmd, "cat /tmp/tithek/get | sed '1,1d' | zcat", 1, 0);
	debug(99, "cmd: %s", cmd);
	tmpstr = command(cmd);
	free(cmd); cmd = NULL;
	system("cp /tmp/tithek/get /var/usr/local/share/titan/plugins/tithek/xvidstage4"); 	
	writesys("/var/usr/local/share/titan/plugins/tithek/xvidstage3", tmpstr, 0);

	sleep(1);
	streamlink = string_resub("file: \"", "\",", tmpstr, 0);

end:

	free(cookie); cookie = NULL;
	free(hash), hash = NULL;
	free(hashlen), hashlen = NULL;
	free(op), op = NULL;
	free(id), id = NULL;
	free(fname), fname = NULL;
		
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
