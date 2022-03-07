#ifndef CLOUDZILLA_H
#define CLOUDZILLA_H

char* cloudzilla(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmpstr = NULL, *streamlink = NULL, *url = NULL;

	if(link == NULL) return NULL;

	unlink("/var/usr/local/share/titan/plugins/tithek//cloudzilla1_get");
	unlink("/var/usr/local/share/titan/plugins/tithek//cloudzilla2_streamlink");
/////////////

	url = string_replace("/share/file/", "/embed/", link, 0);

/*
http://cloudzilla.to/share/file/II5CP0EMM0DDTCA60FWDQLVX5/
http://cloudzilla.to/embed/II5CP0EMM0DDTCA60FWDQLVX5
'GET /embed/II5CP0EMM0DDTCA60FWDQLVX5 HTTP/1.1\r\nHost: neodrive.co\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
u'GET /stream/PL5S1TKUPLTP/4227909300e1f2ce48481e9f321c0ca0.mp4?st=IhfWZn-3wNuMYgq3ROwqNQ HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: d1.neodrive.co\r\nConnection: close\r\nUser-Agent: Python-urllib/2.7\r\n\r\n'
http://d1.neodrive.co/stream/PL5S1TKUPLTP/4227909300e1f2ce48481e9f321c0ca0.mp4?st=IhfWZn-3wNuMYgq3ROwqNQ


http://cloudzilla.me/g5oi83o7260u
'GET /embed-g5oi83o7260u.html HTTP/1.1\r\nHost: cloudzilla.me\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
'GET /ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4 HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: d2171.cloudzilla.me:8777\r\nCookie: __cfduid=de1d2f326ba9713a1f3a8497c518c5ee01441986583\r\nConnection: close\r\nUser-Agent: Python-urllib/2.7\r\n\r\n'
http://d2171.cloudzilla.me:8777/ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4
http://d2171.cloudzilla.me:8777/ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4|User-Agent=Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3 -> application/octet-stream
*/

	debug(99, "url: %s", url);
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek//cloudzilla1_get", NULL, NULL, NULL, tmpstr);	

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

	if(ostrstr(tmpstr, "video not found") != NULL)
	{
		textbox(_("Message"), _("The video no longer exists") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamlink = string_resub("var vurl = \"", "\"", tmpstr, 0);

	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek//cloudzilla1_streamlink", NULL, NULL, NULL, streamlink);

end:

	free(url); url = NULL;
	free(tmpstr); tmpstr = NULL;

	return streamlink;
}

#endif
