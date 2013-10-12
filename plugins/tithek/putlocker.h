#ifndef PUTLOCKER_H
#define PUTLOCKER_H

char* putlocker(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* hash = NULL;
	char* hashstr = NULL;
	char* hashlen = NULL;
	char* ip = NULL;
	char* phpsessid = NULL;
	char* serverid = NULL;
	char* usender = NULL;
	char* line = NULL;
	char* lines = NULL;	
	char* cmd = NULL;
	char* streamlink = NULL;
	char* streamlink1 = NULL;

	unlink("/tmp/tithek/get");
	unlink("/tmp/tithek/x9");

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);

	send = ostrcat(send, "GET /file/", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: www.", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);

	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/get", NULL, NULL, 0, send, NULL, 5000, 1);
	sleep(1);
	free(send); send = NULL;
	
	if(!file_exist("/tmp/tithek/get"))
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	if(debuglevel == 99) system("cp -a /tmp/tithek/get /tmp/putlocker1_tmpstr_get");

	char* header = NULL;
	header = command("cat /tmp/tithek/get");
	
	phpsessid = getxmlentry(header, "PHPSESSID=");
	debug(99, "phpsessid: %s", phpsessid);
	serverid = getxmlentry(header, "SERVERID=");
	debug(99, "serverid: %s", serverid);
	usender = getxmlentry(header, "usender=");
	debug(99, "usender: %s", usender);
	free(header), header = NULL;

	line = string_newline(command("cat /tmp/tithek/get | grep 'Set-Cookie:' -n | tail -n1| cut -d':' -f1"));
	cmd = ostrcat(cmd, "cat /tmp/tithek/get | wc -l", 1, 0);
	lines = string_newline(command(cmd));
	free(cmd); cmd = NULL;
	debug(99, "line: %s", line);
	debug(99, "lines: %s", lines);

	if(lines == NULL || line == NULL) goto end;	
	
	int sline = atoi(lines) - atoi(line) - 1;
	debug(99, "sline: %d", sline);
	debug(99, "sline: %s", oitoa(sline));

	cmd = ostrcat(cmd, "cat /tmp/tithek/get | tail -n", 1, 0);
	cmd = ostrcat(cmd, oitoa(sline), 1, 0);
	cmd = ostrcat(cmd, " | sed '1,1d' | zcat", 1, 0);
	debug(99, "cmd: %s", cmd);	
	tmpstr = command(cmd);
	free(cmd); cmd = NULL;
	free(lines); lines = NULL;
	titheklog(debuglevel, "/tmp/putlocker2_tmpstr_zcat", NULL, tmpstr);

	//get hash from tmpstr
	char* pos = ostrstr(tmpstr, "<input type=\"hidden\" value=");
	hashstr = getxmlentry(pos, "value=");
	free(tmpstr); tmpstr = NULL;
	debug(99, "hashstr: %s", hashstr);
	if(hashstr == NULL) goto end;

	hash = ostrcat(hash, "hash=", 1, 0);	
	hash = ostrcat(hash, hashstr, 1, 0);
	hash = ostrcat(hash, "&confirm=Continue+as+Free+User", 1, 0);
	debug(99, "hash: %s", hash);

	hashlen = oitoa(strlen(hash));
	debug(99, "hashlen: %s", hashlen);

	//create send string
	send = ostrcat(send, "POST /file/", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nCookie: usender=", 1, 0);
	send = ostrcat(send, usender, 1, 0);
	send = ostrcat(send, " SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, " PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	debug(99, "send: %s", send);

	sleep(7);
	//send and receive answer
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/x9", NULL, NULL, 0, send, NULL, 5000, 0);
	sleep(1);
	if(debuglevel == 99) system("cp -a /tmp/tithek/x9 /tmp/putlocker3_tmpstr_post");

	free(send); send = NULL;
	tmpstr = command("cat /tmp/tithek/x9 | sed '1,1d' | zcat");
	titheklog(debuglevel, "/tmp/putlocker4_tmpstr_post_zcat", NULL, tmpstr);
	
	if(ostrstr(tmpstr, "warning_message") != NULL)
	{
		tmpstr = string_resub("<div class='warning_message'>", "</div>", tmpstr, 0);
		tmpstr = strstrip(tmpstr);
		if(tmpstr == NULL || strlen(tmpstr) == 0)
			tmpstr = ostrcat(_("The page is temporarily unavailable"), tmpstr, 0, 1);
		textbox(_("Message"), _(tmpstr) , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	if(phpsessid == NULL || serverid == NULL) goto end;			

	//get streamlink
	if(ostrstr(tmpstr, "playlist:") != NULL)
		streamlink = getxmlentry(tmpstr, "playlist: '");
	else
	{
		streamlink = string_resub("/get_file.php", "\">Download File</a>", tmpstr, 0);	
		streamlink1 = ostrcat("http://www.", host, 0, 0);
		streamlink1 = ostrcat(streamlink1, "/get_file.php", 1, 0);
		streamlink1 = ostrcat(streamlink1, streamlink, 1, 0);
		debug(99, "stream downloadlink");
		goto end;
	}

	debug(99, "streamlink: %s", streamlink);

	if(streamlink != NULL)
	{
		char* pos = strchr(streamlink, '\'');
		if(pos != NULL) pos[0] = '\0';
	}
	free(tmpstr); tmpstr = NULL;
	if(streamlink == NULL) goto end;

	//create send string
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, streamlink, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: www.", 1, 0);
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nCookie: SERVERID=", 1, 0);
	send = ostrcat(send, serverid, 1, 0);
	send = ostrcat(send, "PHPSESSID=", 1, 0);
	send = ostrcat(send, phpsessid, 1, 0);
	send = ostrcat(send, "\r\nConnection: close\r\nUser-Agent: Python-urllib/2.6\r\n\r\n", 1, 0);

	sleep(5);
	
	//send and receive answer
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
	free(send); send = NULL;

	titheklog(debuglevel, "/tmp/putlocker5_tmpstr_get", NULL, tmpstr);

//Streaming version of this file is currently not available. You can download it below.

	//get streamlink1
	streamlink1 = getxmlentry(tmpstr, "url=");
	titheklog(debuglevel, "/tmp/putlocker5_streamlink", NULL, streamlink1);

	free(tmpstr); tmpstr = NULL;

end:
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(hash); hash = NULL;
	free(hashstr); hashstr = NULL;
	free(hashlen); hashlen = NULL;	
	free(ip); ip = NULL;
	free(phpsessid); phpsessid = NULL;
	free(serverid); serverid = NULL;
	free(usender); usender = NULL;
	free(streamlink); streamlink = NULL;

	return streamlink1;
}

#endif
