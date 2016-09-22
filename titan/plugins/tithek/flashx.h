#ifndef FLASHX_H
#define FLASHX_H

char* flashx(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *streamlink = NULL, *ip = NULL;
	char* playurl = NULL, *tmpstr2 = NULL, *error = NULL;
	char* app = NULL, *playpath = NULL, *swfUrl = NULL, *pageUrl = NULL, *swfVfy = NULL;
	char* tmplink = NULL, *pos = NULL, *path = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(tmplink, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}

	if(tmplink == NULL || ostrncmp("http", tmplink, 4))
	{
		textbox(_("Message"), _("Hoster Url not http:// or https://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("http://", "", tmplink, 0);
	tmphost = string_replace("https://", "", tmphost, 1);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/embed-", path, 0, 0);
//	tmphost = string_replace("www.", "", tmphost, 0);
	char* tmpurl = ostrcat(tmphost, "/", 0, 0);
	tmpurl = ostrcat(tmpurl, tmppath, 1, 0);

	tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get1", NULL, NULL, NULL, tmpstr);	
	free(tmpurl), tmpurl = NULL;
	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/tmp/flashx_tmpstr_get2_jsunpack", NULL, NULL, NULL, tmpstr);	

	if(tmpstr == NULL || ostrstr(tmpstr, "<center>Video not found, deleted or abused, sorry!<br") != NULL)
	{
		error = string_resub("<li> <center>", "<br", tmpstr, 0);
		string_deltags(error);
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), NULL, 0, 0);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}

	streamlink = oregex(".*(http://.*normal.mp4).*", tmpstr);

	if(streamlink == NULL)
	{
	//	tmpstr2 = oregex(".*(luq4qk.*)\|.*", tmpstr);
		tmpstr2 = string_resub("|luq4", "|", tmpstr, 0);
		tmpstr2 = ostrcat("/luq4", tmpstr2, 0, 1);
		tmpstr2 = ostrcat(tmpstr2, ".smil", 1, 0);
		free(tmpstr), tmpstr = NULL;
		free(send), send = NULL;
	
		send = ostrcat(send, "GET ", 1, 0);
		send = ostrcat(send, tmpstr2, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
		send = ostrcat(send, tmphost, 1, 0);
		send = ostrcat(send, "\r\n", 1, 0);	
		send = ostrcat(send, "User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\n", 1, 0);	
		send = ostrcat(send, "Referer: http://flashx.tv", 1, 0);
		send = ostrcat(send, tmppath, 1, 0);
		send = ostrcat(send, "\r\n", 1, 0);	
		send = ostrcat(send, "Connection: close\r\n", 1, 0);	
		send = ostrcat(send, "Accept-Encoding: gzip\r\n\r\n", 1, 0);	
	
		debug(99, "#############################################################################################################");
		debug(99, "send2: %s", send);
		debug(99, "#############################################################################################################");
	
		tmpstr = gethttpreal(tmphost, tmpstr2, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		debug(99, "tmpstr: %s", tmpstr);
		titheklog(debuglevel, "/tmp/flashx_tmpstr_get2", NULL, NULL, NULL, tmpstr);
	
		playurl = string_resub("<meta base=\"", "vod/\"", tmpstr, 0);
		app = ostrcat("vod/", NULL, 0, 0);
		playpath = string_resub("<video src=\"", "\"", tmpstr, 0);
		swfUrl= ostrcat("http://static.flashx.tv/player6/jwplayer.flash.swf", NULL, 0, 0); 
		pageUrl = ostrcat(link, NULL, 0, 0);
		swfVfy = ostrcat("true", NULL, 0, 0);
		
		streamlink = ostrcat(streamlink, playurl, 1, 0);
		streamlink = ostrcat(streamlink, " app=", 1, 0);
		streamlink = ostrcat(streamlink, app, 1, 0);
		streamlink = ostrcat(streamlink, " playpath=", 1, 0);
		streamlink = ostrcat(streamlink, playpath, 1, 0);
		streamlink = ostrcat(streamlink, " swfUrl=", 1, 0);
		streamlink = ostrcat(streamlink, swfUrl, 1, 0);
		streamlink = ostrcat(streamlink, " pageUrl=", 1, 0);
		streamlink = ostrcat(streamlink, pageUrl, 1, 0);
		streamlink = ostrcat(streamlink, " swfVfy=", 1, 0);
		streamlink = ostrcat(streamlink, swfVfy, 1, 0);
	}
	titheklog(debuglevel, "flashx_streamlink", NULL, NULL, NULL, tmpstr);

end:
	free(error); error = NULL;
	free(tmphost); tmphost = NULL;
	free(tmphost); tmphost = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
