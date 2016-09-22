#ifndef VIDAG_H
#define VIDAG_H

char* vidag(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *error = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *id = NULL, *fname = NULL, *hashmsg = NULL, *hash = NULL, *hashlen = NULL, *ip = NULL, *streamlink = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/vidag1_get");
	unlink("/tmp/vidag2_get_jsunpack");
	unlink("/tmp/vidag3_streamlink");
/////////////
	char* tmplink = NULL, *pos = NULL, *path = NULL;

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

//	free(path), path = NULL;

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/vidag1_get", NULL, NULL, NULL, tmpstr);	

	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/tmp/vidag2_get_jsunpack", NULL, NULL, NULL, tmpstr);	

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

//	streamlink = string_resub("[{l:\"", "\"", tmpstr, 0);		
	streamlink = oregex(".*(http://.*v.mp4).*", tmpstr);

	titheklog(debuglevel, "/tmp/vidag3_streamlink", NULL, NULL, NULL, streamlink);

	free(tmpstr); tmpstr = NULL;

end:

	free(error); error = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmpstr); tmpstr = NULL;
	free(send); send = NULL;
	free(hash), hash = NULL;
	free(hashlen), hashlen = NULL;
	free(hashmsg), hashmsg = NULL;
	free(id), id = NULL;
	free(fname), fname = NULL;
	free(ip); ip = NULL;

	return streamlink;
}

#endif
