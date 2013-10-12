#ifndef XVIDSTAGE_H
#define XVIDSTAGE_H

char* xvidstage(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
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
	char* post = NULL;

	char* b36code = NULL;
	char* base = NULL;
	char* search = NULL;

	char* tmpstr2 = NULL;
	char* tmpstr3 = NULL;
	char* charlist = NULL;

	unlink("/tmp/tithek/get");
	unlink("/tmp/tithek/post");

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "file: %s", file);

	send = ostrcat(send, "GET /", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: www.", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);

	char* line = NULL;
	char* lines = NULL;
	char* cmd = NULL;
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/get", NULL, NULL, 0, send, NULL, 5000, 1);
	sleep(1);
	free(send); send = NULL;
	if(!file_exist("/tmp/tithek/get"))
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(debuglevel == 99) system("cp -a /tmp/tithek/get /tmp/xvidstage1_tmpstr_get");

	tmpstr = command("cat /tmp/tithek/get");
	cookie = string_resub("Set-Cookie: afc=", ";", tmpstr, 0);	
	debug(99, "cookie: %s", cookie);
	
	titheklog(debuglevel, "/tmp/xvidstage2_tmpstr", NULL, tmpstr);

	free(tmpstr), tmpstr = NULL;

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

	titheklog(debuglevel, "/tmp/xvidstage3_tmpstr_zcat", NULL, tmpstr);

	sleep(1);

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

	//send and receive answer
//	post = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
//	writesys("/var/usr/local/share/titan/plugins/tithek/xvidstage_post1", post, 0);
///
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/post", NULL, NULL, 0, send, NULL, 5000, 0);

	if(!file_exist("/tmp/tithek/post"))
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(debuglevel == 99) system("cp -a /tmp/tithek/post /tmp/xvidstage4_tmpstr_post");	

	cmd = ostrcat(cmd, "cat /tmp/tithek/post | zcat", 1, 0);
	debug(99, "cmd: %s", cmd);
	post = command(cmd);
	titheklog(debuglevel, "/tmp/xvidstage5_tmpstr_post1_zcat", NULL, post);
	free(cmd); cmd = NULL;

	free(tmpstr),tmpstr = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	titheklog(debuglevel, "/tmp/xvidstage6_tmpstr_post1_zcat_resub", NULL, tmpstr);
	
	post = string_replace_all(tmpstr, "", post, 1);
	post = string_replace_all(";return p}(');'", "", post, 1);
	titheklog(debuglevel, "/tmp/xvidstage7_tmpstr_post2", NULL, post);

	free(tmpstr),tmpstr = NULL;
	free(b36code),b36code = NULL;
	tmpstr = string_resub(";return p}('", ");'", post, 0);
	titheklog(debuglevel, "/tmp/xvidstage8_tmpstr_post2_resub", NULL, tmpstr);

	b36code = oregex(".*;',[0-9]{2,2},[0-9]{2,2},'(.*)'.split.*", post);
	
	b36code = string_replace_all("||", "| |", b36code, 1);
	titheklog(debuglevel, "/tmp/xvidstage9_tmpstr_post2_resub_b36code2", NULL, b36code);
	
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

	titheklog(debuglevel, "/tmp/xvidstage10_tmpstr_last", NULL, tmpstr);

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

	titheklog(debuglevel, "/tmp/xvidstage11_streamlink", NULL, streamlink);

	free(tmpstr); tmpstr = NULL;

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
