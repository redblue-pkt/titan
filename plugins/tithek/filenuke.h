#ifndef FILENUKE_H
#define FILENUKE_H

char* filenuke(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *error = NULL, *tmpfile = NULL, *tmpstr = NULL, *send = NULL, *id = NULL, *fname = NULL, *op = NULL, *hash = NULL, *hashlen = NULL, *ip = NULL;
	char* b36code = NULL, *base = NULL, *search = NULL, *post = NULL, *streamlink = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *charlist = NULL;
//	char* cmd = NULL;
	
	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "test host only: %s", get_ip(host));
	debug(99, "tmpfile: %s", tmpfile);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\n", 1, 0);
	send = ostrcat(send, "Accept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
	send = ostrcat(send, "Host: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n", 1, 0);
	send = ostrcat(send, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n", 1, 0);
	send = ostrcat(send, "Connection: close\r\nCookie: xxx2=ok;\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	free(send), send = NULL;
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/filenuke1_get", NULL, tmpstr);

//	sleep(2);

	if(ostrstr(tmpstr, "<title>The page is temporarily unavailable</title>") != NULL)
	{
		error = string_resub("<td align=\"center\" valign=\"middle\">", "</td>", tmpstr, 0);
		string_deltags(error);
		stringreplacechar(error, '|', '\0');
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), error, 0, 1);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "<title>Direct IP access not allowed") != NULL)
	{
		error = string_resub("<title>", "</title>", tmpstr, 0);
		string_deltags(error);
		stringreplacechar(error, '|', '\0');
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), error, 0, 1);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
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
	send = ostrcat(send, " HTTP/1.0\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

//	sleep(5);
	
	post = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
	free(send), send = NULL;
	debug(99, "post: %s", post);
	titheklog(debuglevel, "/tmp/filenuke2_post", NULL, tmpstr);

	free(tmpstr),tmpstr = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/filenuke3_tmpstr1", NULL, tmpstr);
	
	post = string_replace_all(tmpstr, "", post, 1);
	post = string_replace_all(";return p}(');'", "", post, 1);
	debug(99, "post: %s", post);
	titheklog(debuglevel, "/tmp/filenuke4_post2", NULL, post);

	free(tmpstr),tmpstr = NULL;
	free(b36code),b36code = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/filenuke5_tmpstr2", NULL, tmpstr);

	b36code = oregex(".*;',[0-9]{2,2},[0-9]{2,2},'(.*)'.split.*", post);
	
	b36code = string_replace_all("||", "| |", b36code, 1);
	debug(99, "b36code: %s", b36code);
	titheklog(debuglevel, "/tmp/filenuke6_b36code2", NULL, b36code);
	
	struct splitstr* ret1 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(b36code, "|", &count);

	charlist = ostrcat(charlist, "\"", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, "'", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, ".", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, ";", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, ":", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, "=", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, ",", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, " ", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, "\\", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, "/", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, "(", 1, 0);
	charlist = ostrcat(charlist, "|", 1, 0);
	charlist = ostrcat(charlist, ")", 1, 0);

	for(i = 0; i < count; i++)
	{
		if(ostrstr((&ret1[i])->part, " ") != NULL)
		{
			printf("continue\n");
			continue;
		}
		char* x = oltostr(i, 36);

		struct splitstr* ret2 = NULL;
		int count2 = 0;
		int i2 = 0;
		tmpstr2 = ostrcat(charlist, NULL, 0, 0);
		ret2 = strsplit(tmpstr2, "|", &count2);
		for(i2 = 0; i2 < count2; i2++)
		{
			struct splitstr* ret3 = NULL;
			int count3 = 0;
			int i3 = 0;
			tmpstr3 = ostrcat(charlist, NULL, 0, 0);
			ret3 = strsplit(tmpstr3, "|", &count3);
			for(i3 = 0; i3 < count3; i3++)
			{
				debug(99, "-----------------------------------------------");
				debug(99, "replace %s%s%s <> %s%s%s",(&ret2[i2])->part, x, (&ret3[i3])->part, (&ret2[i2])->part, (&ret1[i])->part, (&ret3[i3])->part);

				base = ostrcat(base, (&ret2[i2])->part, 1, 0);
				base = ostrcat(base, x, 1, 0);
				base = ostrcat(base, (&ret3[i3])->part, 1, 0);		
				search = ostrcat(search, (&ret2[i2])->part, 1, 0);
				search = ostrcat(search, (&ret1[i])->part, 1, 0);
				search = ostrcat(search, (&ret3[i3])->part, 1, 0);
				tmpstr = string_replace_all(base, search, tmpstr, 1);
				free(base), base = NULL;
				free(search), search = NULL;
			}
			free(ret3), ret3 = NULL;
			free(tmpstr3), tmpstr3 = NULL;
		}
		free(ret2), ret2 = NULL;
		free(tmpstr2), tmpstr2 = NULL;
		free(x);
	}
	free(ret1), ret1 = NULL;
	free(b36code), b36code = NULL;
	free(post), post = NULL;
	free(charlist), charlist = NULL;

	titheklog(debuglevel, "/tmp/filenuke7_tmpstr_last", NULL, tmpstr);

	streamlink = oregex(".*file.*(http:.*video.flv).*image.*", tmpstr);
	if(streamlink == NULL)
		streamlink = oregex(".*file.*(http:.*video.mp4).*image.*", tmpstr);				

	if(streamlink == NULL)
		streamlink = oregex(".*file.*(http:.*video.mkv).*image.*", tmpstr);

	if(streamlink == NULL)
		streamlink = oregex(".*file.*(http:.*video.avi).*image.*", tmpstr);

	if(streamlink == NULL)
		streamlink = oregex(".*src=.*(http:.*video.mp4).*\".*", tmpstr);

	if(streamlink == NULL)
		streamlink = oregex(".*src=.*(http:.*video.mkv).*\".*", tmpstr);

	if(streamlink == NULL)
		streamlink = oregex(".*src=.*(http:.*video.avi).*\".*", tmpstr);
		
	if(streamlink == NULL)
		streamlink = oregex(".*value=.*(http:.*video.mp4).*\".*", tmpstr);				

	if(streamlink == NULL)
		streamlink = oregex(".*value=.*(http:.*video.mkv).*\".*", tmpstr);

	if(streamlink == NULL)
		streamlink = oregex(".*value=.*(http:.*video.avi).*\".*", tmpstr);			

	titheklog(debuglevel, "/tmp/filenuke8_streamlink", NULL, streamlink);

	free(tmpstr); tmpstr = NULL;

end:

	free(error); error = NULL;
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
