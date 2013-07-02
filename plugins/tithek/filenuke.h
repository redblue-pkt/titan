#ifndef FILENUKE_H
#define FILENUKE_H

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
	char* post = NULL;
	char* streamlink = NULL;
	char* cmd = NULL;

	char* tmpstr2 = NULL;
	char* tmpstr3 = NULL;
	char* charlist = NULL;

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat(host, NULL, 0, 0);
	tmpfile = ostrcat("/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "test host only: %s", get_ip(host));
	debug(99, "tmpfile: %s", tmpfile);
	tmpstr = gethttp(tmphost, tmpfile, 80, NULL, NULL, 10000, NULL, 0);
	debug(99, "write file");
	sleep(2);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke1_tmpstr", tmpstr, 0);

	if(ostrstr(tmpstr, "<title>The page is temporarily unavailable</title>") != NULL)
	{
		tmpstr = string_resub("<td align=\"center\" valign=\"middle\">", "</td>", tmpstr, 1);
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
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	sleep(5);

	//send and receive answer
	post = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke2_tmpstr_post1", post, 0);
//
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/post", NULL, NULL, 0, send, NULL, 5000, 0);
	cmd = ostrcat("cat /tmp/tithek/post | zcat", NULL, 0, 0);
	debug(99, "cmd: %s", cmd);
	post = command(cmd);
	writesys("/tmp/filenuke2_post1", post, 0);
	free(cmd); cmd = NULL;
//
	free(tmpstr),tmpstr = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke3_tmpstr1", tmpstr, 0);
	
	post = string_replace_all(tmpstr, "", post, 1);
	post = string_replace_all(";return p}(');'", "", post, 1);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke4_post2", post, 0);

	free(tmpstr),tmpstr = NULL;
	free(b36code),b36code = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke5_tmpstr2", tmpstr, 0);

	b36code = oregex(".*;',[0-9]{2,2},[0-9]{2,2},'(.*)'.split.*", post);
	
	b36code = string_replace_all("||", "| |", b36code, 1);		
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke6_b36code2", b36code, 0);
	
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

	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke7_tmpstr_last", tmpstr, 0);

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

	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/filenuke8_streamlink", streamlink, 0);

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
