#ifndef TVTOAST_H
#define TVTOAST_H

char* tvtoast(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *fid = NULL, *streamurl = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *channelid = NULL;
	char* tmppath = NULL, *streamurl1 = NULL, *logfile = NULL, *tmpurl = NULL, *streamport = NULL, *streamip = NULL, *streamid = NULL, *tmphost = NULL, *title = NULL, *pic = NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}
	
	system("rm -rf /var/usr/local/share/titan/plugins/tithek/tvtoast_*");

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	titheklog(debuglevel, "/tmp/tithek/tvtoast1_tmpstr", NULL, tmpstr);
	logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr1", 1, 0);
	logfile = ostrcat(logfile, "_", 1, 0);
	tmpstr2 = string_replace_all("/", ".", path, 0);
	logfile = ostrcat(logfile, tmpstr2, 1, 0);
	writesys(logfile, tmpstr, 0);
	free(logfile), logfile = NULL;
	free(tmppath), tmppath = NULL;
	free(tmphost), tmphost = NULL;
	free(tmpurl), tmpurl = NULL;
	free(tmpstr2), tmpstr2 = NULL;


	tmpstr1 = string_resub("<ul class=\"player-top-links\">", "<div class=\"float-clear\"></div>", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;
	printf("tmpstr1: %s\n",tmpstr1);

	int count = 0, i = 0, incount = 0, www = 1;	
	struct splitstr* ret1 = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	ret1 = strsplit(tmpstr1, "\n", &count);
	for(i = 0; i < count; i++)
	{
		if(ret1[i].part != NULL && strlen(ret1[i].part) > 1)
		{
			if(ostrstr(ret1[i].part, "http://tvtoast.com/") != NULL)
			{
//				debug(99, "Round (%d) found tvtoast.com link", i);
				incount++;
				printf("####################### start (%d) ############################\n", incount);
				tmphost = string_resub("http://", "/", ret1[i].part, 0);
				tmppath = string_resub("http://tvtoast.com/", "\"", ret1[i].part, 0);
				tmppath = string_replace_all(" ", "%20", tmppath, 1);

				tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);
				logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr2_", 1, 0);
				logfile = ostrcat(logfile, oitoa(incount), 1, 1);
				logfile = ostrcat(logfile, "_", 1, 0);
				tmpstr2 = string_replace_all("/", ".", tmppath, 0);
				logfile = ostrcat(logfile, tmpstr2, 1, 0);
				writesys(logfile, tmpstr, 0);
				free(logfile), logfile = NULL;
				free(tmppath), tmppath = NULL;
				free(tmphost), tmphost = NULL;
				free(tmpurl), tmpurl = NULL;
				free(tmpstr2), tmpstr2 = NULL;


				if(ostrstr(tmpstr, "channel.php?a=") != NULL)
					www = 0;

				tmpurl = string_resub("src=\"", "\"", tmpstr, 0);				
				tmphost = string_resub("http://", "/", tmpurl, 0);
				tmppath = string_replace_all(tmphost, "", tmpurl, 0);
				tmppath = string_replace_all("http://", "", tmppath, 1);
				tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);

				logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr3b_", 1, 0);
				logfile = ostrcat(logfile, oitoa(incount), 1, 1);
				logfile = ostrcat(logfile, "_", 1, 0);
				tmpstr2 = string_replace_all("/", ".", tmppath, 0);
				logfile = ostrcat(logfile, tmpstr2, 1, 0);
				writesys(logfile, tmpstr, 0);
				free(logfile), logfile = NULL;
//				free(tmppath), tmppath = NULL;
//				free(tmphost), tmphost = NULL;
				free(tmpurl), tmpurl = NULL;
				free(tmpstr2), tmpstr2 = NULL;

				if(www == 1)
				{
					if(ostrstr(tmpstr, "Moved Permanently") != NULL && ostrstr(tmphost, "www.") == NULL)
					{
						printf("found Moved Permanently\n");
						free(tmpstr), tmpstr = NULL;
						tmpstr = ostrcat("www.", tmphost, 0, 0);
						free(tmphost), tmphost = NULL;
						tmphost = ostrcat(tmpstr, NULL, 0, 0);
					}
					else
					{
						fid = string_resub("fid='", "'", tmpstr, 0);
						free(tmppath), tmppath = NULL;
						free(tmphost), tmphost = NULL;
						tmpurl = string_resub("src='", "'", tmpstr, 0);
						if(tmpurl == NULL)
							tmpurl = string_resub("src=\"", "\"", tmpstr, 0);
							
						free(tmpstr), tmpstr = NULL;						
						tmphost = string_resub("http://", "/", tmpurl, 0);		
						tmppath = string_replace_all(tmphost, "", tmpurl, 0);
						tmppath = string_replace_all("http://", "", tmppath, 1);
						tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);
		
						logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr3c_", 1, 0);
						logfile = ostrcat(logfile, oitoa(incount), 1, 1);
						logfile = ostrcat(logfile, "_", 1, 0);
						tmpstr2 = string_replace_all("/", ".", tmppath, 0);
						logfile = ostrcat(logfile, tmpstr2, 1, 0);
						writesys(logfile, tmpstr, 0);
						free(logfile), logfile = NULL;
//						free(tmppath), tmppath = NULL;
//						free(tmphost), tmphost = NULL;
						free(tmpurl), tmpurl = NULL;
						free(tmpstr2), tmpstr2 = NULL;

						if(fid != NULL)
						{
							tmpurl = string_resub("src=\"", "'", tmpstr, 0);
							tmphost = string_resub("http://", "/", tmpurl, 0);			
							tmppath = string_replace_all(tmphost, "", tmpurl, 0);
							tmppath = string_replace_all("http://", "", tmppath, 1);
							tmppath = ostrcat(tmppath, fid, 1, 0);
							free(fid), fid = NULL;
							free(tmpstr), tmpstr = NULL;
						}
						else
						{
							printf("found fid: %s\n", fid);
							streamurl1 = string_resub("streamer: \"", "\"", tmpstr, 0);
							if(streamurl1 != NULL)
								streamurl1 = string_replace_all("\\", "", streamurl1, 1);

							if(streamurl1 == NULL)	
							{				
								free(tmppath), tmppath = NULL;
								free(tmphost), tmphost = NULL;
								free(tmpurl), tmpurl = NULL;
								tmpurl = string_resub("var url = '", "'", tmpstr, 0);
					
								free(tmpstr), tmpstr = NULL;		
								tmphost = string_resub("http://", "/", tmpurl, 0);
								tmppath = string_replace_all(tmphost, "", tmpurl, 0);
								tmppath = string_replace_all("http://", "", tmppath, 1);

								char* send = ostrcat(NULL, "GET ", 0, 0);
								send = ostrcat(send, tmppath, 1, 0);
								send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
								send = ostrcat(send, tmphost, 1, 0);
							//	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
								send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\n", 1, 0);	
								send = ostrcat(send, "\r\nReferer: ", 1, 0);
//								send = ostrcat(send, "http://embed.tgo-tv.com/usa/hbo.php?&width=650&height=480", 1, 0);
								send = ostrcat(send, "http://tgo-tv.com/usa/hbo.php?&width=650&height=480", 1, 0);								
								send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
							
								debug(99, "send: %s", send);
								tmpstr = gethttpreal(tmphost, "/embed.php", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
								logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstrsend_", 1, 0);
								logfile = ostrcat(logfile, oitoa(incount), 1, 1);
								writesys(logfile, tmpstr, 0);
/*
					free(tmppath), tmppath = NULL;
					free(tmphost), tmphost = NULL;
					free(tmpurl), tmpurl = NULL;
					tmpurl = string_resub("var url = '", "'", tmpstr, 0);
					free(tmpstr), tmpstr = NULL;		
printf("tmpurl: %s\n",tmpurl);
					tmphost = string_resub("http://", "/", tmpurl, 0);
					tmppath = string_replace_all(tmphost, "", tmpurl, 0);
					tmppath = string_replace_all("http://", "", tmppath, 1);
*/					
							}
							streamurl1 = string_resub("streamer: \"", "\"", tmpurl, 0);
						}
					}
					
				}
				else
				{

					printf("found www\n");

					free(tmppath), tmppath = NULL;
					free(tmphost), tmphost = NULL;
					free(tmpurl), tmpurl = NULL;
					tmpurl = string_resub("var url = '", "'", tmpstr, 0);
					free(tmpstr), tmpstr = NULL;		
					
					tmphost = string_resub("http://", "/", tmpurl, 0);
					tmppath = string_replace_all(tmphost, "", tmpurl, 0);
					tmppath = string_replace_all("http://", "", tmppath, 1);
				}

				debug(199, "connect (%d) %s  %s", i, tmphost, tmppath);
				tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);
				free(logfile), logfile = NULL;
				logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr4_", 1, 0);
				logfile = ostrcat(logfile, oitoa(incount), 1, 1);
				logfile = ostrcat(logfile, "_", 1, 0);
				tmpstr2 = string_replace_all("/", ".", tmppath, 0);
				logfile = ostrcat(logfile, tmpstr2, 1, 0);
				writesys(logfile, tmpstr, 0);
				free(logfile), logfile = NULL;
				free(tmppath), tmppath = NULL;
				free(tmphost), tmphost = NULL;
				free(tmpurl), tmpurl = NULL;
				free(tmpstr2), tmpstr2 = NULL;

				if(streamurl1 == NULL)
					streamurl1 = string_resub("file: \"", "\"", tmpstr, 0);			
			
				if(streamurl1 == NULL)
				{
					streamid = string_resub("stream=\"", "\"", tmpstr, 0);
					if(streamid != NULL)
					{
						free(tmpstr), tmpstr = NULL;
						streamip = ostrcat("173.208.201.162", NULL, 0, 0);
						streamport = ostrcat("1935", NULL, 0, 0);
						debug(99, "Round (%d) streamip=%s", i, streamip);
						debug(99, "Round (%d) streamport=%s", i, streamport);
						debug(199, "Round (%d) streamid=%s", i, streamid);
		
						streamurl = ostrcat("rtmpe://", streamip, 0, 0);
						streamurl = ostrcat(streamurl, ":", 1, 0);
						streamurl = ostrcat(streamurl, streamport, 1, 0);
						streamurl = ostrcat(streamurl, "/goLive playpath=", 1, 0);
						streamurl = ostrcat(streamurl, streamid, 1, 0);
						streamurl = ostrcat(streamurl, " swfUrl=http://b17014255105be61b178-d8c0dbd82389bad2adcb7a22d1883b64.r92.cf5.rackcdn.com/jwplayer.flash.swf live=1 pageUrl=http://envivo.pw/ token=OOG17t.x#K9Vh#|", 1, 0);
					}
				}
				else
					streamurl = ostrcat(streamurl1, NULL, 0, 0);

				free(streamurl1), streamurl1 = NULL;

				title = ostrcat("Rtmpe Stream (", NULL, 0, 0);
				title = ostrcat(title, oitoa(incount), 1, 1);
				title = ostrcat(title, ")", 1, 0);

				pic = ostrcat("flv.png", NULL, 0, 0);
				
				if(streamurl != NULL)
				{
					debug(199, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}
				free(channelid), channelid = NULL;

				free(streamip); streamip = NULL;
				free(streamport); streamport = NULL;
				free(streamid); streamid = NULL;
				free(title), title = NULL;
				free(pic), pic = NULL;
				free(streamurl), streamurl = NULL;
				www = 1;
				printf("####################### stop (%d) ############################\n", incount);


			}
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
	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;

	titheklog(debuglevel, "/tmp/tithek/tvtoast3_streamurl", NULL, streamurl);

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

int tvtoast_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *line = NULL, *menu = NULL, *search = NULL;
	int ret = 1, count = 0, i = 0;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	if(searchstr == NULL)
		search = textinputhist(_("Search"), " ", "searchhist");
	else
		search = textinputhist(_("Search"), searchstr, "searchhist");

	if(search != NULL)
	{
		drawscreen(load, 0, 0);

		strstrip(search);
		string_tolower(search);

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/tvtoast/streams/tvtoast.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);

		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
			
				tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
				tmpstr1 = stringreplacecharonce(tmpstr1, '#', '\0');
				string_tolower(tmpstr1);

				if(ostrstr(tmpstr1, search) != NULL)
				{
					printf("found: %s\n", ret1[i].part);
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;

					line = ostrcat(line, ret1[i].part, 1, 0);
					line = ostrcat(line, "\n", 0, 0);
				}
				free(tmpstr1), tmpstr1 = NULL;				
			}
			free(ret1), ret1 = NULL;

			if(line != NULL)
			{
				line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/tvtoast.search.list", NULL, 0, 0);
				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
				ret = 0;
			}
		}
		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;
	return ret;
}

#endif