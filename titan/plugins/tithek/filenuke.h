#ifndef FILENUKE_H
#define FILENUKE_H

unsigned char* filenukereceive(int* sock)
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

char* filenuke(char* host, char* file)
{
	debug(99, "in host: %s file: %s", host, file);
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* id = NULL;
	char* fname = NULL;
	char* op = NULL;
	char* hash = NULL;
	char* hashlen = NULL;	
	char* ip = NULL;
	char* b36code = NULL;
	char* base = NULL;
	char* search = NULL;
	char* streamlink = NULL;

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/", file, 0, 0);
	ip = get_ip(tmphost);
	tmpstr = gethttp(tmphost, tmpfile, 80, NULL, NULL, NULL, 0);
	debug(99, "write file");
writesys("/tmp/filenuke", tmpstr, 0);

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
	free(tmpstr); tmpstr = NULL;
	debug(99, "op: %s", op);
	if(op == NULL) goto end;

	hash = ostrcat(hash, "id=", 1, 0);	
	hash = ostrcat(hash, id, 1, 0);
	hash = ostrcat(hash, "&referer=&fname=", 1, 0);
	hash = ostrcat(hash, fname, 1, 0);
	hash = ostrcat(hash, "&method_free=Free&usr_login=&op=", 1, 0);
	hash = ostrcat(hash, op, 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	
	//create send string
	send = ostrcat(send, "POST /", 1, 0);
	send = ostrcat(send, id, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	sleep(5);
	
	//send and receive answer
	gethttpreal(tmphost, tmpfile, 80, "post", NULL, NULL, 0, send, NULL);

	sleep(5);

	tmpstr = command("/var/usr/local/share/titan/plugins/tithek/getstring1.sh post");
	b36code = command("/var/usr/local/share/titan/plugins/tithek/getstring2.sh post");
	b36code = string_replace_all("||", "| |", b36code, 1);

	struct splitstr* ret1 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(b36code, "|", &count);

	for(i = 0; i < count; i++)
	{
		if(ostrstr((&ret1[i])->part, " ") != NULL)
		{
			printf("continue\n");
			continue;
		}
		char* x = oltostr(i, 36);

		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "\\", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "\\", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;
		
		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "'", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "'", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ":", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ":", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ":", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ":", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;
				
		base = ostrcat(base, "/", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "/", 1, 0);
		search = ostrcat(search, "/", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "/", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ".", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ".", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "/", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "/", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "'", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "'", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;
	
		base = ostrcat(base, "/", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ".", 1, 0);
		search = ostrcat(search, "/", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ".", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "(", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "(", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, " ", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "(", 1, 0);
		search = ostrcat(search, " ", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "(", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, "=", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, " ", 1, 0);
		search = ostrcat(search, "=", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, " ", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, " ", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "=", 1, 0);
		search = ostrcat(search, " ", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "=", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;

		base = ostrcat(base, ";", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ".", 1, 0);
		search = ostrcat(search, ";", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ".", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "\\", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "\\", 1, 0);
		tmpstr = string_replace(base, search, tmpstr, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		free(x);
	}
	free(ret1), ret1 = NULL;
	free(b36code), b36code = NULL;

	streamlink = oregex(".*file.*(http:.*video.flv).*image.*", tmpstr);				
	free(tmpstr); tmpstr = NULL;

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

	return streamlink;
}

#endif
