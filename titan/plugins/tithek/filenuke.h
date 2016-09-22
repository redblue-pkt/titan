#ifndef FILENUKE_H
#define FILENUKE_H

char* filenuke(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmplink = NULL, *tmppath = NULL, *tmpstr = NULL, *streamlink = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/filenuke1_get");
	unlink("/tmp/filenuke2_get_jsunpack");
	unlink("/tmp/filenuke3_streamlink");

/*
'GET /f/64Lmmb6 HTTP/1.1\r\nHost: filenuke.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
'GET /f/64Lmmb6/?code=79b0b13602bba3b0ef7611ea17c149c3 HTTP/1.1\r\nHost: filenuke.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko\r\nCookie: __cfduid=d51466bc1a055427260984443f3433e541446559673\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
'GET /d3/788e480510b692c80b6cc4bcbc2f541f/v.flv?md5=24XhN1toJK_mou1FNFU8WQ&expires=1446577673&mode=1&frm=flv&dsk=6&id=1709647 HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: 37.252.3.244\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko\r\n\r\n'
http://37.252.3.244/d3/788e480510b692c80b6cc4bcbc2f541f/v.flv?md5=24XhN1toJK_mou1FNFU8WQ&expires=1446577673&mode=1&frm=flv&dsk=6&id=1709647|User-Agent=Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko -> video/x-flv
http://37.252.3.244/d3/788e480510b692c80b6cc4bcbc2f541f/v.flv?md5=24XhN1toJK_mou1FNFU8WQ&expires=1446577673&mode=1&frm=flv&dsk=6&id=1709647|User-Agent=Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko
http://37.252.3.244/d3/788e480510b692c80b6cc4bcbc2f541f/v.flv|User-Agent=Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko
*/

/////////////

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}

	if(tmplink == NULL || ostrncmp("http", tmplink, 4))
	{
		textbox(_("Message"), _("Hoster Url not http:// or https://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	debug(99, "url: %s", tmplink);
	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/filenuke1_get", NULL, NULL, NULL, tmpstr);	

	tmppath = string_resub("id=\"go-next\" href=\"", "\"", tmpstr, 0);		
	tmplink = ostrcat("http://filenuke.com", tmppath, 0, 0);

	if(ostrstr(tmplink, "http://filenuke.com?code=") != NULL)
	{
		tmplink = ostrcat(link, "/", 0, 0);
		tmplink = ostrcat(tmplink, tmppath, 0, 0);

	}

	debug(99, "tmplink: %s", tmplink);

	debug(99, "url: %s", tmplink);
	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/filenuke2_get", NULL, NULL, NULL, tmpstr);	

// not needed
	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/tmp/filenuke2_get_jsunpack", NULL, NULL, NULL, tmpstr);	

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

	if(ostrstr(tmpstr, "Video is processing now.") != NULL)
	{
		textbox(_("Message"), _("Video is processing now. Conversion stage...") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamlink = string_resub("var lnk234 = '", "';", tmpstr, 0);		
//	streamlink = oregex(".*(http://.*v.flv).*", tmpstr);

	if(streamlink != NULL)
		streamlink = ostrcat(streamlink, "|User-Agent=libcurl-agent/1.0", 1, 0);
//		streamlink = ostrcat(streamlink, "|User-Agent=Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko", 1, 0);

	titheklog(debuglevel, "/tmp/filenuke3_streamlink", NULL, NULL, NULL, streamlink);

	free(tmpstr); tmpstr = NULL;

end:

	free(tmpstr); tmpstr = NULL;
	free(tmppath); tmppath = NULL;
	free(tmplink); tmplink = NULL;

	return streamlink;
}

#endif
