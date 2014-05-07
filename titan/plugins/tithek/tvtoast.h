#ifndef TVTOAST_H
#define TVTOAST_H

char* tvtoast(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *fid = NULL, *streamurl = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *channelid = NULL;
	char* tmppath = NULL, *streamurl1 = NULL, *logfile = NULL, *tmpurl = NULL, *streamport = NULL, *streamip = NULL, *streamid = NULL, *tmphost = NULL, *title = NULL, *pic = NULL;
	char* streamer = NULL;
	char* playpath = NULL;
	char* swfUrl = NULL;
	//char* pageUrl = NULL;
	char* app = NULL;
	char* url = NULL;
	char* Cookie1 = NULL;
	char* tmpstr4 = NULL;
	char* tmpstr5 = NULL;
	char* token = NULL;
	char* typemsg = NULL;
	char* referer = NULL;
	char* send = NULL;
	char* tmpstr3 = NULL;
char* pageurlid = NULL;
char* pageurl = NULL;
char* cmd = NULL;
char* urlid = NULL;
char* tokenid = NULL;
char* check = NULL;
			
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
				incount++;
				printf("####################### start (%d) ############################\n", incount);

				referer = string_resub("<a href=\"", "\"", ret1[i].part, 0);
				tmphost = string_resub("http://", "/", ret1[i].part, 0);
				tmppath = string_resub("http://tvtoast.com/", "\"", ret1[i].part, 0);
				tmppath = string_replace_all(" ", "%20", tmppath, 1);
printf("tmphost: %s\n",tmphost);
printf("tmppath: %s\n",tmppath);

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
				typemsg = ostrcat(tmphost, NULL, 0, 0);
							
				tmppath = string_replace_all(tmphost, "", tmpurl, 0);
				tmppath = string_replace_all("http://", "", tmppath, 1);





							send = ostrcat(NULL, "GET ", 0, 0);
							send = ostrcat(send, tmppath, 1, 0);
							send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: gzip\r\nHost: ", 1, 0);	
							send = ostrcat(send, tmphost, 1, 0);
							send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\n", 1, 0);	
							send = ostrcat(send, "Referer: ", 1, 0);
							send = ostrcat(send, referer, 1, 0);

							//send = ostrcat(send, "\r\nCookie: ", 1, 0);	
							//send = ostrcat(send, Cookie1, 1, 0);
							send = ostrcat(send, "\r\n\r\n", 1, 0);
						
						debug(99, "send: %s", send);
						tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
						free(send), send = NULL;
						
//				tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);

//				referer = ostrcat(tmpurl, NULL, 0, 0);

				logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr3b_", 1, 0);
				logfile = ostrcat(logfile, oitoa(incount), 1, 1);
				logfile = ostrcat(logfile, "_", 1, 0);
				tmpstr2 = string_replace_all("/", ".", tmppath, 0);
				logfile = ostrcat(logfile, tmpstr2, 1, 0);
				writesys(logfile, tmpstr, 0);
				free(logfile), logfile = NULL;
				free(tmpurl), tmpurl = NULL;
				free(tmpstr2), tmpstr2 = NULL;

				check = string_resub("file: \"", "\"", tmpstr, 0);
				if(ostrstr(check, "rtmpe://5.135.128.157:1935") != NULL)
				{
					printf("####################################################\n");
					printf("streamid: %s\n",streamid);
					printf("check: %s\n",check);
					printf("fid: %s\n",fid);
					printf("####################################################\n");

//rtmpdump -r rtmpe://strm.ukcast.tv/redirect -y bhousaa0 --debug
//rtmpdump -r rtmpe://5.135.128.157:1935/ula/abc8815 --debug
					cmd = ostrcat("rtmpdump -r ", NULL, 0, 0);
					cmd = ostrcat(cmd, check, 1, 0);
					cmd = ostrcat(cmd, " --debug > /tmp/tithek/rtmp.log 2>&1", 1, 0);												
					system("rm -rf /tmp/tithek/rtmp.log");

						free(tmpstr), tmpstr = NULL;
						printf("cmd: %s\n",cmd);
						system(cmd);
						free(cmd),cmd = NULL;

						tmpstr = command("cat /tmp/tithek/rtmp.log");

						streamip = string_resub("redirect, STRING:", ">", tmpstr, 0);
						strstrip(streamip);

						free(tmpstr), tmpstr = NULL;
	

//rtmpe://5.135.128.157:1935/ula/abc8815

fid = basename(check);
						streamurl = ostrcat(streamip, NULL, 0, 0);
						streamurl = ostrcat(streamurl, " playpath=", 1, 0);
						streamurl = ostrcat(streamurl, fid, 1, 0);
						streamurl = ostrcat(streamurl, " swfUrl=http://www.zingo.tv/jwplayer/player.swf", 1, 0);
						streamurl = ostrcat(streamurl, " pageUrl=http://zingo.tv/", 1, 0);
						streamurl = ostrcat(streamurl, " live=true", 1, 0);
						streamurl = ostrcat(streamurl, " swfVfy=true", 1, 0);
						streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
						
						free(urlid), urlid = NULL;
						free(tokenid), tokenid = NULL;
						free(pageurlid), pageurlid = NULL;
						free(url), url = NULL;
						free(token), token = NULL;
						free(pageurl), pageurl = NULL;

				}
				else if(ostrstr(check, "rtmp://") != NULL)
					streamurl = ostrcat(check, NULL, 0, 0);

				free(check), check = NULL;
				
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

						send = ostrcat(NULL, "GET ", 0, 0);
						send = ostrcat(send, tmppath, 1, 0);
						send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
						send = ostrcat(send, tmphost, 1, 0);
						send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\n", 1, 0);	
						send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
					
						debug(99, "send: %s", send);
						tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
						free(send), send = NULL;

						logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr3c_", 1, 0);
						logfile = ostrcat(logfile, oitoa(incount), 1, 1);
						logfile = ostrcat(logfile, "_", 1, 0);
						tmpstr2 = string_replace_all("/", ".", tmppath, 0);
						logfile = ostrcat(logfile, tmpstr2, 1, 0);
						writesys(logfile, tmpstr, 0);
						free(logfile), logfile = NULL;
						free(tmpurl), tmpurl = NULL;
						free(tmpstr2), tmpstr2 = NULL;

						streamer = string_resub("streamer: \"", "\"", tmpstr, 0);
						if(streamer != NULL)
						{
							streamer = string_replace_all("\\", "", streamer, 1);
						
							playpath = string_resub("file: \"", "\"", tmpstr, 0);
							playpath = string_replace_all(".flv", "", playpath, 1);
						
							swfUrl = string_resub("type: 'flash', src: '", "'", tmpstr, 0);
					//		pageUrl = ostrcat("http://www.ilive.to", NULL, 0, 0);
							printf("streamer: %s\n", streamer);
							app = string_resub("/edge/", "\0", streamer, 0);
							
							app = oregex(".*(/edge/) .*", streamer);
						
							app = ostrcat(streamer, NULL, 0, 0);
							url = string_resub("rtmp://", "/", app, 0);		
							app = string_replace(url, "", app, 0);
							app = string_replace_all("rtmp:///", "", app, 1);
						}





						while(ostrstr(tmpstr, "Set-Cookie:") != NULL)
						{
							tmpstr4 = string_resub("Set-Cookie:", ";", tmpstr, 0);
							
							if(tmpstr4 != NULL)
							{
								tmpstr5 = ostrcat("Set-Cookie: ", tmpstr4, 0, 0);
								tmpstr5 = ostrcat(tmpstr5, ";", 1, 0);
								tmpstr = string_replace(tmpstr5, "", tmpstr, 1);
								tmpstr4 = ostrcat(tmpstr4, "; ", 1, 0);
								Cookie1 = ostrcat(Cookie1, tmpstr4, 1, 0);
							}
							free(tmpstr4), tmpstr4 = NULL; 
							free(tmpstr5), tmpstr5 = NULL; 
						}

						if(ostrstr(tmpstr, "getJSON(\"") != NULL)
						{
							printf("found getJSON\n");
							tmpurl = string_resub("getJSON(\"", "\"", tmpstr, 0);
							
							printf("tmpurl: %s\n", tmpurl);
							tmphost = string_resub("http://", "/", tmpurl, 0);			
							tmppath = string_replace_all(tmphost, "", tmpurl, 0);
							tmppath = string_replace_all("http://", "", tmppath, 1);
							free(fid), fid = NULL;
							free(tmpstr), tmpstr = NULL;

							send = ostrcat(NULL, "GET ", 0, 0);
							send = ostrcat(send, tmppath, 1, 0);
							send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: gzip\r\nHost: ", 1, 0);	
							send = ostrcat(send, tmphost, 1, 0);
							send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\n", 1, 0);	
							send = ostrcat(send, "Referer: ", 1, 0);
							send = ostrcat(send, "http://", 1, 0);
							send = ostrcat(send, tmphost, 1, 0);

							send = ostrcat(send, "\r\nCookie: ", 1, 0);	
							send = ostrcat(send, Cookie1, 1, 0);
							send = ostrcat(send, "\r\n\r\n", 1, 0);
			
							debug(99, "send: %s", send);
							tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
							free(send), send = NULL;
							logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr3d_", 1, 0);
							logfile = ostrcat(logfile, oitoa(incount), 1, 1);
							logfile = ostrcat(logfile, "_", 1, 0);
							tmpstr2 = string_replace_all("/", ".", tmppath, 0);
							logfile = ostrcat(logfile, tmpstr2, 1, 0);
							writesys(logfile, tmpstr, 0);
							free(logfile), logfile = NULL;
							free(tmpstr2), tmpstr2 = NULL;

							token = string_resub("{\"token\":\"", "\"", tmpstr, 0);

							streamurl1 = ostrcat(streamer, " app=", 0, 0);
							streamurl1 = ostrcat(streamurl1, app, 1, 0);
							streamurl1 = ostrcat(streamurl1, " playpath=", 1, 0);
							streamurl1 = ostrcat(streamurl1, playpath, 1, 0);
							streamurl1 = ostrcat(streamurl1, " swfUrl=", 1, 0);
							streamurl1 = ostrcat(streamurl1, swfUrl, 1, 0);
							streamurl1 = ostrcat(streamurl1, " live=1", 1, 0);
							streamurl1 = ostrcat(streamurl1, " timeout=15", 1, 0);
							streamurl1 = ostrcat(streamurl1, " token=", 1, 0);
							streamurl1 = ostrcat(streamurl1, token, 1, 0);
							streamurl1 = ostrcat(streamurl1, " pageUrl=http://", 1, 0);
							streamurl1 = ostrcat(streamurl1, tmphost, 1, 0);
							typemsg = ostrcat(tmphost, NULL, 0, 0);
						}
						else if(fid != NULL)
						{
							tmpurl = string_resub("src=\"", "'", tmpstr, 0);
							tmphost = string_resub("http://", "/", tmpurl, 0);			
							tmppath = string_replace_all(tmphost, "", tmpurl, 0);
							tmppath = string_replace_all("http://", "", tmppath, 1);
							tmppath = ostrcat(tmppath, fid, 1, 0);
							printf("1111fid: %s\n",fid);
							//free(fid), fid = NULL;
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

								if(ostrstr(tmpstr, "<h1>This channel can only work in") != NULL)
									referer = string_resub("<h1>This channel can only work in <a href='", "'", tmpstr, 0);
					
								free(tmpstr), tmpstr = NULL;		
								tmphost = string_resub("http://", "/", tmpurl, 0);
								tmppath = string_replace_all(tmphost, "", tmpurl, 0);
								tmppath = string_replace_all("http://", "", tmppath, 1);

								send = ostrcat(NULL, "GET ", 0, 0);
								send = ostrcat(send, tmppath, 1, 0);
								send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: gzip\r\nHost: ", 1, 0);	
								send = ostrcat(send, tmphost, 1, 0);
								send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\n", 1, 0);	
								send = ostrcat(send, "Referer: ", 1, 0);
								send = ostrcat(send, referer, 1, 0);

								send = ostrcat(send, "\r\nCookie: ", 1, 0);	
								send = ostrcat(send, Cookie1, 1, 0);
								send = ostrcat(send, "\r\n\r\n", 1, 0);
								
								free(referer), referer = NULL;
								
								debug(99, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");													
								debug(99, "send1: %s", send);
								debug(99, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");													

								tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
								logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr3e_", 1, 0);
								logfile = ostrcat(logfile, oitoa(incount), 1, 1);
								logfile = ostrcat(logfile, "_", 1, 0);
								tmpstr2 = string_replace_all("/", ".", tmppath, 0);
								logfile = ostrcat(logfile, tmpstr2, 1, 0);
						
								writesys(logfile, tmpstr, 0);
								free(logfile), logfile = NULL;
								free(tmpstr2), tmpstr2 = NULL;

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

printf("vor refer\n");
				if(ostrstr(tmpstr, "<h1>This channel can only work in") != NULL)
				{
printf("in refer\n");

					referer = string_resub("<h1>This channel can only work in <a href='", "'", tmpstr, 0);
					
				}								

				free(tmpstr), tmpstr = NULL;
				free(send), send = NULL;

				send = ostrcat(NULL, "GET ", 0, 0);
				send = ostrcat(send, tmppath, 1, 0);
				send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: gzip\r\nHost: ", 1, 0);	
				send = ostrcat(send, tmphost, 1, 0);
				send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\n", 1, 0);	
				send = ostrcat(send, "Referer: ", 1, 0);
				if(referer != NULL)
					send = ostrcat(send, referer, 1, 0);
				else
					send = ostrcat(send, tmphost, 1, 0);
				send = ostrcat(send, "\r\nCookie: ", 1, 0);	
				send = ostrcat(send, Cookie1, 1, 0);
				send = ostrcat(send, "\r\n\r\n", 1, 0);
				
				free(referer), referer = NULL;

				debug(99, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");													
				debug(99, "send2: %s", send);
				debug(99, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;");													

								
				debug(199, "connect (%d) %s  %s", i, tmphost, tmppath);
				tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
//				tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);
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

				swfUrl = string_resub("<script type=\"text/javascript\" src=\"", "\"", tmpstr, 0);

				check = string_resub("var str = \"", "\"", tmpstr, 0);
				if(ostrstr(check, "rtmpe://cdn.ukcast.tv/redirect") != NULL)
				{
					printf("####################################################\n");
					printf("streamid: %s\n",streamid);
					printf("check: %s\n",check);
					printf("fid: %s\n",fid);
					printf("####################################################\n");

//rtmpdump -r rtmpe://strm.ukcast.tv/redirect -y bhousaa0 --debug
					cmd = ostrcat("rtmpdump -r ", NULL, 0, 0);
					cmd = ostrcat(cmd, "rtmpe://strm.ukcast.tv/redirect -y ", 1, 0);
					cmd = ostrcat(cmd, fid, 1, 0);
					cmd = ostrcat(cmd, " --debug > /tmp/tithek/rtmp.log 2>&1", 1, 0);												
					system("rm -rf /tmp/tithek/rtmp.log");

						free(tmpstr), tmpstr = NULL;
						printf("cmd: %s\n",cmd);
						system(cmd);
						free(cmd),cmd = NULL;

						tmpstr = command("cat /tmp/tithek/rtmp.log");

						streamip = string_resub("redirect, STRING:", ">", tmpstr, 0);
						strstrip(streamip);

						free(tmpstr), tmpstr = NULL;
		
						streamurl = ostrcat(streamip, NULL, 0, 0);
						streamurl = ostrcat(streamurl, " playpath=", 1, 0);
						streamurl = ostrcat(streamurl, fid, 1, 0);
						streamurl = ostrcat(streamurl, " swfUrl=http://ukcast.tv/player/player.swf", 1, 0);
						streamurl = ostrcat(streamurl, " pageUrl=http://ukcast.tv/", 1, 0);
						streamurl = ostrcat(streamurl, " live=true", 1, 0);
						streamurl = ostrcat(streamurl, " swfVfy=true", 1, 0);
						streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
						
						free(urlid), urlid = NULL;
						free(tokenid), tokenid = NULL;
						free(pageurlid), pageurlid = NULL;
						free(url), url = NULL;
						free(token), token = NULL;
						free(pageurl), pageurl = NULL;

				}

				
				if(streamurl1 == NULL)
					streamurl1 = string_resub("file: \"", "\"", tmpstr, 0);			
			
				if(streamurl1 == NULL)
				{
					streamid = string_resub("stream=\"", "\"", tmpstr, 0);
					if(streamid != NULL)
					{
						tmpstr1 = string_resub("<div style=\"visibility:hidden;\">", "</body>", tmpstr, 0);				
						tmpurl = string_resub("type=\"text/javascript\" src=\"", "\"", tmpstr1, 0);				

						tmphost = string_resub("http://", "/", tmpurl, 0);
						tmppath = string_replace_all(tmphost, "", tmpurl, 0);
						tmppath = string_replace_all("http://", "", tmppath, 1);

						tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);

						while(ostrstr(tmpstr, "\\") != NULL)
						{
							tmpstr1 = string_resub("\"", "\"", tmpstr, 0);
							tmpstr2 = string_replace_all("\\x", "%", tmpstr1, 0);
							
							if(tmpstr2 != NULL)
							{
								htmldecode(tmpstr2, tmpstr2);

								tmpstr3 = ostrcat("\"", tmpstr1, 0, 0);
								tmpstr3 = ostrcat(tmpstr3, "\"", 1, 0);

								tmpstr4 = ostrcat("'", tmpstr2, 0, 0);
								tmpstr4 = ostrcat(tmpstr4, "'", 1, 0);

								tmpstr = string_replace(tmpstr3, tmpstr4, tmpstr, 1);
								free(tmpstr1), tmpstr1 = NULL;
								free(tmpstr2), tmpstr2 = NULL;
							}
							else
								break;

							free(tmpstr1), tmpstr1 = NULL; 
							free(tmpstr2), tmpstr2 = NULL; 
							free(tmpstr3), tmpstr3 = NULL;
							free(tmpstr4), tmpstr4 = NULL;
						}


						free(logfile), logfile = NULL;
						logfile = ostrcat(logfile, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr4a_", 1, 0);
						logfile = ostrcat(logfile, oitoa(incount), 1, 1);
						logfile = ostrcat(logfile, "_", 1, 0);
						tmpstr2 = string_replace_all("/", ".", tmppath, 0);
						logfile = ostrcat(logfile, tmpstr2, 1, 0);
						writesys(logfile, tmpstr, 0);
free(logfile), logfile = NULL;
free(tmpstr2), tmpstr2 = NULL;

						urlid = oregex(".*\\[(.*)\\]\\+stream.*", tmpstr);
						tokenid = oregex(".*securetoken:_0x.*\\[(.*)\\].*", tmpstr);
						pageurlid = oregex(".*abouttext:_0x.*\\[(.*)\\],width.*", tmpstr);

						tmpstr1 = string_resub("[", "]", tmpstr, 0);
						int count2 = 0;
						struct splitstr* ret2 = NULL;
						ret2 = strsplit(tmpstr1, ",", &count2);

						if(count2 > atoi(urlid)) url = string_resub("'", "'", ret2[atoi(urlid)].part, 0);
						if(count2 > atoi(tokenid)) token = string_resub("'", "'", ret2[atoi(tokenid)].part, 0);
						if(count2 > atoi(pageurlid)) pageurl = string_resub("'", "'", ret2[atoi(pageurlid)].part, 0);

printf("urlid: %s\n",urlid);
printf("tokenid: %s\n",tokenid);
printf("pageurlid: %s\n",pageurlid);
printf("url: %s\n",url);
printf("token: %s\n",token);
printf("1pageurl: %s\n",pageurl);

						free(ret2), ret2 = NULL;

//rtmpdump -r rtmpe://strm.ukcast.tv/redirect -y bhousaa0 --debug

						cmd = ostrcat("rtmpdump -r ", NULL, 0, 0);
						cmd = ostrcat(cmd, url, 1, 0);
						cmd = ostrcat(cmd, streamid, 1, 0);
						cmd = ostrcat(cmd, " --debug > /tmp/tithek/rtmp.log 2>&1", 1, 0);												
						system("rm -rf /tmp/tithek/rtmp.log");
						
						free(tmpstr), tmpstr = NULL;
						printf("cmd: %s\n",cmd);
						system(cmd);
						free(cmd),cmd = NULL;

						tmpstr = command("cat /tmp/tithek/rtmp.log");

						streamip = string_resub("redirect, STRING:", ">", tmpstr, 0);
						strstrip(streamip);

						free(tmpstr), tmpstr = NULL;
		
						streamurl = ostrcat(streamip, NULL, 0, 0);
						streamurl = ostrcat(streamurl, " playpath=", 1, 0);
						streamurl = ostrcat(streamurl, streamid, 1, 0);
//						streamurl = ostrcat(streamurl, " swfUrl=http://b17014255105be61b178-d8c0dbd82389bad2adcb7a22d1883b64.r92.cf5.rackcdn.com/jwplayer.flash.swf live=1 pageUrl=http://envivo.pw/ token=OOG17t.x#K9Vh#|", 1, 0);

//						streamurl = ostrcat(streamurl, " swfUrl=http://b17014255105be61b178-d8c0dbd82389bad2adcb7a22d1883b64.r92.cf5.rackcdn.com/jwplayer.flash.swf", 1, 0);
						streamurl = ostrcat(streamurl, " swfUrl=", 1, 0);
						streamurl = ostrcat(streamurl, swfUrl, 1, 0);
						streamurl = ostrcat(streamurl, " live=1", 1, 0);
//						streamurl = ostrcat(streamurl, " pageUrl=http://envivo.pw/", 1, 0);
//						streamurl = ostrcat(streamurl, pageUrl, 1, 0);
						streamurl = ostrcat(streamurl, " pageUrl=http://", 1, 0);
						streamurl = ostrcat(streamurl, pageurl, 1, 0);
						streamurl = ostrcat(streamurl, "/", 1, 0);
//						typemsg = ostrcat(pageurl, NULL, 0, 0);
							
						streamurl = ostrcat(streamurl, " token=", 1, 0);
						streamurl = ostrcat(streamurl, token, 1, 0);
						
						free(urlid), urlid = NULL;
						free(tokenid), tokenid = NULL;
						free(pageurlid), pageurlid = NULL;
						free(url), url = NULL;
						free(token), token = NULL;
						free(pageurl), pageurl = NULL;
						
					}
				}
				else
					streamurl = ostrcat(streamurl1, NULL, 0, 0);

				free(streamurl1), streamurl1 = NULL;

				title = ostrcat("Rtmpe Stream (", NULL, 0, 0);
				title = ostrcat(title, oitoa(incount), 1, 1);
				title = ostrcat(title, ")", 1, 0);
				if(typemsg != NULL)
				{
					title = ostrcat(title, " (", 1, 0);
					title = ostrcat(title, typemsg, 1, 0);
					title = ostrcat(title, ")", 1, 0);
				}
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
				free(typemsg), typemsg = NULL;

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