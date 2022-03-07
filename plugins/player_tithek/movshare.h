#ifndef MOVESHARE_H
#define MOVESHARE_H

char* movshare(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL;
	char* tmpurl = NULL;
	char* tmppath = NULL;
	char* tmpstr = NULL;
	char* send = NULL;
	char* ip = NULL;
	char* streamlink = NULL;
	char* filekey = NULL;
	char* file = NULL;
	
	if(link == NULL) return NULL;

	unlink("/tmp/movshare1_get");
	unlink("/tmp/movshare2_get");
/////////////
	char* tmplink = NULL, *pos = NULL, *path = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);
	
printf("11111111111111111111\n");
	if(ostrstr(link, "/Out/?s=") != NULL)
	{
printf("222222222222222222222\n");

		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}
	
	if(tmplink == NULL || ostrncmp("http", tmplink, 4))
	{
printf("33333333333333333333333333\n");

		textbox(_("Message"), _("Hoster Url not http:// or https://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("https://", "", tmplink, 0);
	tmphost = string_replace("https://", "", tmphost, 1);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/", path, 0, 0);
//	free(path), path = NULL;

/////////////

/////////////
	char* host = NULL;
	host = ostrcat(tmphost, NULL, 0, 0);
	tmppath = string_replace("/video/", "/embed.php?v=", tmppath, 1);
//	tmppath = string_replace("/video/", "/embed/?v=", tmppath, 1);

// https://www.wholecloud.net/embed.php?v=e723e5185b818
// 7481628c80fea HTTP/1.1\r\nHost: www.wholecloud.net\r\nUser-Agent: 
 
	free(tmphost), tmphost = NULL;
//	tmphost = ostrcat("embed.movshare.net", NULL, 0, 0);
	tmphost = ostrcat("www.wholecloud.net", NULL, 0, 0);

/////////////
/*
	tmppath = ostrcat("/embed.php?v=", file, 0, 0);
	tmphost = ostrcat("embed.movshare.net", NULL, 0, 0);
*/
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpurl = ostrcat("www.wholecloud.net", tmppath, 0, 0);
	tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);
	free(tmpurl), tmpurl = NULL;

//	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movshare1_get", NULL, NULL, NULL, tmpstr);

	streamlink = string_resub("<source src=\"", "\"", tmpstr, 0);
	if(streamlink != NULL)
		goto end;

	if(tmpstr == NULL || ostrstr(tmpstr, "error=1&error_msg=The video no longer exists") != NULL)
	{
		textbox(_("Message"), _("The video no longer exists") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	file = string_resub("flashvars.file=\"", "\";", tmpstr, 0);
	if(file == NULL)
		file = string_replace("/embed.php?v=", "", tmppath, 0);	
	
	char* r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL;
	pos = ostrstr(tmpstr, ");}('");
	if(pos != NULL)
	{
		r1 = string_resub(");}('", "'", pos, 0);
		pos = ostrstr(pos + 5, ",'");
		if(pos != NULL)
		{
			r2 = string_resub(",'", "'", pos, 0);
			pos = ostrstr(pos + 2, ",'");
			if(pos != NULL)
			{
				r3 = string_resub(",'", "'", pos, 0);
				pos = ostrstr(pos + 2, ",'");
				if(pos != NULL)
					r4 = string_resub(",'", "'", pos, 0);
			}
		}
	}
	
	filekey = getfilekey(r1, r2, r3, r4);
	
	free(r1); r1 = NULL;
	free(r2); r2 = NULL;
	free(r3); r3 = NULL;
	free(r4); r4 = NULL;
	
	if(filekey == NULL)
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
	free(tmppath), tmppath = NULL;
//	free(tmphost), tmphost = NULL;
	free(send), send = NULL;

//	tmppath = ostrcat("/api/player.api.php?file=", file, 0, 0);
//	tmppath = ostrcat(tmppath, "&key=", 1, 0);
//	tmppath = ostrcat(tmppath, filekey, 1, 0);
	
	tmppath = ostrcat("/api/player.api.php?key=", filekey, 0, 0);
	tmppath = ostrcat(tmppath, "&file=", 1, 0);
	tmppath = ostrcat(tmppath, file, 1, 0);
//	tmphost = ostrcat(tmphost, NULL, 0, 0);
	
	free(tmphost), tmphost = NULL;
	tmphost = ostrcat(host, NULL, 0, 0);

	if(ostrstr(link, "http://embed.") != NULL)
		tmphost = string_replace("embed.", "www.", tmphost, 1);

	free(send), send = NULL;
	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
//	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "www.wholecloud.net", 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);
	free(tmpstr), tmpstr = NULL;


	tmpurl = ostrcat("www.wholecloud.net", tmppath, 0, 0);
	tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);
	free(tmpurl), tmpurl = NULL;
	
//	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/movshare2_get", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(tmpstr == NULL || ostrstr(tmpstr, "error=1&error_msg=The video no longer exists") != NULL)
	{
		textbox(_("Message"), _("The video no longer exists") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamlink = string_resub("url=", "&", tmpstr, 0);
end:
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(filekey); filekey = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
