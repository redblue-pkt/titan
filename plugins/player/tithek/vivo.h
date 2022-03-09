#ifndef VIVO_H
#define VIVO_H

char* vivo(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmplink = NULL, *tmpstr = NULL, *streamlink = NULL, *timestamp = NULL, *hash = NULL, *post = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/vivo1_get");
	unlink("/tmp/vivo2_streamlink");
/////////////

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}

/*
http://vivo.me/5942ab22ad
'GET /5942ab22ad HTTP/1.1\r\nHost: vivo.sx\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nReferer: http://vivo.sx/5942ab22ad\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
'POST /5942ab22ad HTTP/1.1\r\nContent-Length: 58\r\nAccept-Encoding: gzip\r\nHost: vivo.sx\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nReferer: http://vivo.sx/5942ab22ad\r\nX-Requested-With: XMLHttpRequest\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ntimestamp=1446560163&hash=22e41c3d978979656dfcd945fb285214'
'GET /get/0000106725?e=1446588963&s=300&m=video/mp4&h=wW_uIfK-PToRvLcAqacScg HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: bella.storage.vivo.sx:8080\r\nConnection: close\r\nUser-Agent: Python-urllib/2.7\r\n\r\n'
http://bella.storage.vivo.sx:8080/get/0000106725?e=1446588963&s=300&m=video/mp4&h=wW_uIfK-PToRvLcAqacScg
*/

	debug(99, "link: %s", tmplink);
	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/vivo1_get", NULL, NULL, NULL, tmpstr);	

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

	timestamp = string_resub("name=\"timestamp\" value=\"", "\"", tmpstr, 0);
	hash = string_resub("name=\"hash\" value=\"", "\"", tmpstr, 0);

	post = ostrcat("timestamp=", timestamp, 0, 0);
	post = ostrcat(post, "&hash=", 1, 0);
	post = ostrcat(post, hash, 1, 0);
	debug(99, "post: %s", post);

	tmpstr = gethttps(tmplink, NULL, post, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/vivo2_post", NULL, NULL, NULL, tmpstr);	

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

	streamlink = string_resub("data-url=\"", "\"", tmpstr, 0);

	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/vivo3_get", NULL, NULL, NULL, tmpstr);	

	titheklog(debuglevel, "/tmp/vivo1_streamlink", NULL, NULL, NULL, streamlink);

end:
	free(tmplink); tmplink = NULL;
	free(tmpstr); tmpstr = NULL;

	return streamlink;
}

#endif
