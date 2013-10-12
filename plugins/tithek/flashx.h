#ifndef FLASHX_H
#define FLASHX_H

char* flashx(char* host, char* file)
{
	debug(99, "in host: %s file: %s", host, file);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;

	unlink("/tmp/tithek/x9");

	if(host == NULL || file == NULL) return NULL;

	tmphost = ostrcat("www.", host, 0, 0);
	tmpfile = ostrcat("/file/", file, 0, 0);
	debug(99, "tmphost: %s", tmphost);
	ip = get_ip(tmphost);
	debug(99, "ip: %s", ip);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "file: %s", file);

	send = ostrcat(send, "GET /player/embed_player.php?vid=", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: www.", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

  titheklog(debuglevel, "/tmp/flashx1_tmpstr_get1", NULL, tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	sleep(1);

	send = ostrcat(send, "GET /player/embed_player.php?vid=", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, host, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

  titheklog(debuglevel, "/tmp/flashx2_tmpstr_get2", NULL, tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	sleep(1);

	send = ostrcat(send, "GET /nuevo/player/cst.php?hash=", 1, 0);
	send = ostrcat(send, file, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: play.flashx.tv\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "tmphost: %s", tmphost);
	debug(99, "tmpfile: %s", tmpfile);
	debug(99, "send: %s", send);
	gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/x9", NULL, NULL, 0, send, NULL, 5000, 0);

	if(!file_exist("/tmp/tithek/x9"))
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

  if(debuglevel == 99) system("cp -a /tmp/tithek/x9 /tmp/flashx3_tmpstr_get3");

	free(send); send = NULL;
	tmpstr = command("cat /tmp/tithek/x9 | sed '1,1d' | zcat");

  titheklog(debuglevel, "/tmp/flashx4_tmpstr_get3_zcat", NULL, tmpstr);

	streamlink = string_resub("<file>", "</file>", tmpstr, 0);
	titheklog(debuglevel, "/tmp/flashx5_streamlink", NULL, streamlink);

end:
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
