#ifndef MOVESHARE_H
#define MOVESHARE_H

char* movshare(char* host, char* file, char* hosterurl)
{
	debug(99, "in host: %s file: %s", host, file);
	debug(99, "in hosterurl: %s file: %s", hosterurl);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL;
	char* tmpfile = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;
	char* filekey = NULL;
	
	if(host == NULL || file == NULL) return NULL;

	unlink("/tmp/movshare1_get");
	unlink("/tmp/movshare2_get");

	tmpfile = ostrcat("/embed.php?v=", file, 0, 0);
	tmphost = ostrcat("embed.movshare.net", NULL, 0, 0);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movshare00_get", NULL, tmpstr);

	file = string_resub("flashvars.file=\"", "\";", tmpstr, 0);
	filekey = string_resub("flashvars.filekey=\"", "\";", tmpstr, 0);
	if(filekey == NULL)
	{
		char* searchstr = string_resub("flashvars.filekey=", ";", tmpstr, 0);
		debug(99, "searchstr: %s", searchstr);
		searchstr = ostrcat(searchstr, "=\"", 1, 0);
		filekey = string_resub(searchstr, "\";", tmpstr, 0);
	}
	debug(99, "filekey: %s", filekey);
	debug(99, "file: %s", file);
	
	free(tmpstr), tmpstr = NULL;
	free(tmpfile), tmpfile = NULL;
	free(tmphost), tmphost = NULL;
	free(send), send = NULL;
	free(tmpfile), tmpfile = NULL;
	tmpfile = ostrcat("/api/player.api.php?file=", file, 0, 0);
	tmpfile = ostrcat(tmpfile, "&key=", 1, 0);
	tmpfile = ostrcat(tmpfile, filekey, 1, 0);
	tmphost = ostrcat(host, NULL, 0, 0);
		
	free(send), send = NULL;
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmpfile, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttpreal(tmphost, tmpfile, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movshare2_get", NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	sleep(1);
	streamlink = string_resub("url=", "&", tmpstr, 0);
end:
	free(tmphost); tmphost = NULL;
	free(tmpfile); tmpfile = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(filekey); filekey = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
