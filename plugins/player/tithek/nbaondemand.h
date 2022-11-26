#ifndef NBAONDEMAND_H
#define NBAONDEMAND_H

char* nbaondemand(char* link)
{
	int debuglevel = getconfigint("debuglevel", NULL);
	system("rm -f /var/usr/local/share/titan/plugins/player/tithek/nbaondemand*");

	debug(99, "nbaondemand link=%s ", link);
	char* streamurl = NULL, *title = NULL, *pic = NULL, *label = NULL;
	char* tmpstr = NULL;
	char* path = NULL;
	char* host = NULL;
	char* url = NULL;
	char* id = NULL;
	char* type = NULL;

	host = string_resub("http://", "/", link, 0);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, link, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/nbaondemand1_tmpstr", NULL, host, path, tmpstr);

	url = string_resub("scrolling='no'src=\"", "\"", tmpstr, 0);
	if(url != NULL)
	{
		free(tmpstr), tmpstr = NULL;
		host = string_resub("http://", "/", link, 0);
		tmpstr = gethttps(link, NULL, NULL, NULL, NULL, link, 1);
		host = string_resub("http://", "/", link, 0);
		path = string_replace_all(host, "", link, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/nbaondemand1a_tmpstr", NULL, host, path, tmpstr);
		streamurl = string_resub("video height='480' width='700' src='", "'", tmpstr, 0);
	}
	else if(ostrstr(tmpstr, "config.playwire.com/19004/videos/v2/") != NULL)
	{
		id = string_resub("config.playwire.com/19004/videos/v2/", "/zeus.json", tmpstr, 0);
		if(id != NULL)
		{
	//<script src="//cdn.playwire.com/bolt/js/zeus/embed.js" data-config="//config.playwire.com/19004/videos/v2/4546546/zeus.json" data-width="600" data-height="343"  type="text/javascript" charset="utf-8"></script>
	//GET https://cdn.video.playwire.com/19004/videos/4546546/video-sd.mp4?hosting_id=19004 HTTP/1.1
	
		    streamurl = ostrcat("https://cdn.video.playwire.com/19004/videos/", id, 0, 0);
	    	streamurl = ostrcat(streamurl, "/video-sd.mp4?hosting_id=19004", 1, 0);
	    }
	}
    else
    {
//<iframe src='http://videoapi.my.mail.ru/videos/embed/mail/eccovskiy/_myvideo/3848.html' width='626' height='367' frameborder='0' webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>
		url = string_resub("<iframe src='", "'", tmpstr, 0);
	
		type = string_resub("/mail/", "/", url, 0);
		id = string_resub("/_myvideo/", ".html", url, 0);
	
		if(type == NULL || id == NULL)
		{
			textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
			goto end;
		}
	
		free(url), url = NULL;
	    url = ostrcat("http://videoapi.my.mail.ru/videos/mail/", type, 0, 0);
	    url = ostrcat(url, "/_myvideo/", 1, 0);
	    url = ostrcat(url, id, 1, 0);
	    url = ostrcat(url, ".json?ver=0.2.60", 1, 0);
		free(id), id = NULL;
		free(type), type = NULL;
	
		host = string_resub("http://", "/", url, 0);
		tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
		host = string_resub("http://", "/", url, 0);
		path = string_replace_all(host, "", url, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/nbaondemand2_tmpstr", NULL, host, path, tmpstr);
	
	///////////////
		tmpstr = string_replace_all("\n", " ", tmpstr, 1);
		tmpstr = string_replace_all("{\"key\":", "\nkey=", tmpstr, 1);
		
		int incount = 0, count = 0, i = 0;	
		struct splitstr* ret1 = NULL;
		struct menulist* mlist = NULL, *mbox = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);
		for(i = 0; i < count; i++)
		{
	//		if(ostrstr(ret1[i].part, "key=") != NULL)
			if(!ostrncmp("key=", ret1[i].part, 4))
			{
				incount++;
				free(streamurl), streamurl = NULL;			
				streamurl = string_resub("\"url\":\"", "\"", ret1[i].part, 0);
		
				if(ostrstr(streamurl, "rtmp") != NULL)
					title = ostrcat("Rtmpe Stream (", NULL, 0, 0);
				else
					title = ostrcat("Http Stream (", NULL, 0, 0);
		
				label = string_resub("key=\"", "\"", ret1[i].part, 0);
				title = ostrcat(title, label, 1, 0);
				title = ostrcat(title, ")", 1, 0);
				
				if(ostrstr(streamurl, "mp4") != NULL)
					pic = ostrcat("mp4.png", NULL, 0, 0);
				else
					pic = ostrcat("m3u8.png", NULL, 0, 0);
	
				if(streamurl != NULL)
				{
					debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}
	
				free(label), label = NULL;
				free(title), title = NULL;
				free(pic), pic = NULL;
			}
		}
		free(ret1), ret1 = NULL;
		if(mlist != NULL)
		{
			mbox = menulistbox(mlist, NULL, _("Stream Menu"), _("Choose your Streaming Format from the following list"), NULL, NULL, 1, 0);
			if(mbox != NULL)
			{
				free(streamurl), streamurl = NULL;
	
				debug(99, "mbox->name %s", mbox->name);
				debug(99, "mbox->text %s", mbox->text);
				streamurl = ostrcat(mbox->text, NULL, 0, 0);
	
			}
		}
	}
/////////////

end:

	free(host), host = NULL;
	free(path), path = NULL;
	free(tmpstr), tmpstr = NULL;
	free(url), url = NULL;
	free(type), type = NULL;
	free(id), id = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

#endif