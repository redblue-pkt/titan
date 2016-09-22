#ifndef MIGHTYUPLOAD_H
#define MIGHTYUPLOAD_H

char* mightyupload(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmplink = NULL, *tmppath = NULL, *tmpstr = NULL, *streamlink = NULL, *pos = NULL, *path = NULL, *url = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/mightyupload1_get");
	unlink("/tmp/mightyupload2_get_jsunpack");
	unlink("/tmp/mightyupload3_streamlink");
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
	tmppath = ostrcat(tmppath, ".html", 1, 0);

	char* id = oregex("http://.*/(.*)/.*", link);
	debug(99, "id: %s", id);
	if(id == NULL)
	{
		id = oregex("http://.*/(.*).html.*", link);
		debug(99, "id2: %s", id);
	}
	
	debug(99, "tmppath: %s", tmppath);
	debug(99, "tmphost: %s", tmphost);
	url = ostrcat("www.mightyupload.com/embed-", id, 0, 0);
	url = ostrcat(url, ".html", 1, 0);
	free(id), id = NULL;

	debug(99, "url: %s", url);
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/mightyupload1_get", NULL, NULL, NULL, tmpstr);	
	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/tmp/mightyupload2_get_jsunpack", NULL, NULL, NULL, tmpstr);	

//////////////////		
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

	streamlink = string_resub("file: '", "'", tmpstr, 0);
	if(streamlink == NULL)
		streamlink = string_resub("name=\"src\"value=\"", "\"", tmpstr, 0);
	if(streamlink == NULL)
		streamlink = string_resub("'label' : '360p', 'file' : '", "'", tmpstr, 0);
	if(streamlink == NULL)
		streamlink = string_resub("'label' : '240p', 'file' : '", "'", tmpstr, 0);		
	if(streamlink == NULL)
		streamlink = string_resub("label: '240p', file: '", "'", tmpstr, 0);		
	if(streamlink == NULL)
		streamlink = string_resub("label: '360p', file: '", "'", tmpstr, 0);		
	if(streamlink == NULL)
		streamlink = oregex(".*(http://.*v.mp4).*", tmpstr);

	titheklog(debuglevel, "/tmp/mightyupload3_streamlink", NULL, NULL, NULL, streamlink);

end:

	free(url); url = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmplink); tmplink = NULL;
	free(tmpstr); tmpstr = NULL;

	return streamlink;
}


#endif
