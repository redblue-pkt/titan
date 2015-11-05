#ifndef THEVIDEO_H
#define THEVIDEO_H

char* thevideo(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmplink = NULL, *tmppath = NULL, *tmpstr = NULL, *streamlink = NULL, *pos = NULL, *path = NULL, *url = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/thevideo1_get");
	unlink("/tmp/thevideo2_streamlink");
/////////////

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}

	if(tmplink == NULL || ostrncmp("http://", tmplink, 7))
	{
		textbox(_("Message"), _("Hoster Url not http://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("http://", "", tmplink, 0);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/embed-", path, 0, 0);
	tmppath = ostrcat(tmppath, ".html", 1, 0);

/*
http://thevideo.me/g5oi83o7260u
'GET /embed-g5oi83o7260u.html HTTP/1.1\r\nHost: thevideo.me\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
'GET /ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4 HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: d2171.thevideo.me:8777\r\nCookie: __cfduid=de1d2f326ba9713a1f3a8497c518c5ee01441986583\r\nConnection: close\r\nUser-Agent: Python-urllib/2.7\r\n\r\n'
http://d2171.thevideo.me:8777/ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4
http://d2171.thevideo.me:8777/ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4|User-Agent=Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3 -> application/octet-stream
*/
	debug(99, "tmppath: %s", tmppath);
	debug(99, "tmphost: %s", tmphost);
	url = ostrcat(tmphost, tmppath, 0, 0);
	debug(99, "url: %s", url);
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/thevideo1_get", NULL, NULL, NULL, tmpstr);	

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "404 Not Found") != NULL)
	{
		textbox(_("Message"), _("The video no longer exists") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamlink = string_resub("'label' : '360p', 'file' : '", "'", tmpstr, 0);
	if(streamlink == NULL)
		streamlink = string_resub("'label' : '240p', 'file' : '", "'", tmpstr, 0);		
	if(streamlink == NULL)
		streamlink = string_resub("label: '240p', file: '", "'", tmpstr, 0);		
	if(streamlink == NULL)
		streamlink = string_resub("label: '360p', file: '", "'", tmpstr, 0);		
	if(streamlink == NULL)
		streamlink = oregex(".*(http://.*v.mp4).*", tmpstr);

	titheklog(debuglevel, "/tmp/thevideo1_streamlink", NULL, NULL, NULL, streamlink);

end:

	free(url); url = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmplink); tmplink = NULL;
	free(tmpstr); tmpstr = NULL;

	return streamlink;
}

#endif
