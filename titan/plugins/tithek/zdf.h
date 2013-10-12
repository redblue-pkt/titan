#ifndef ZDF_H
#define ZDF_H

char* zdf(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *tmpstr = NULL, *tmpstr2 = NULL, *pic = NULL, *title = NULL, *tmpstr3 = NULL, *quali = NULL;
	
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	titheklog(debuglevel, "/tmp/zdf1_tmpstr", NULL, tmpstr);

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "<div class=\"fsk\">") != NULL)
		{
			tmpstr2 = string_resub("<div class=\"fsk\">", "</div>", tmpstr, 0);
			tmpstr2 = string_replace(".", ".\n", tmpstr2, 1),
			textbox(_("Message"), tmpstr2, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
			free(tmpstr2), tmpstr2 = NULL;
		}

		tmpstr2 = string_resub("<formitaeten>", "</formitaeten>", tmpstr, 0);
		stringreplacechar(tmpstr2, '\n', ' ');
		stringreplacechar(tmpstr2, '\t', ' ');
		string_strip_whitechars(tmpstr2);
		tmpstr2 = string_replace_all("<formitaet basetype", "\n\t<formitaet basetype", tmpstr2, 1);

		titheklog(debuglevel, "/tmp/zdf1_tmpstr2", NULL, tmpstr");
		
		int count = 0, i = 0;	
 		struct splitstr* ret1 = NULL;
 		struct menulist* mlist = NULL, *mbox = NULL;
		ret1 = strsplit(string_decode(tmpstr2,0), "\n", &count);
		for(i = 0; i < count; i++)
		{
			if(ret1[i].part != NULL && strlen(ret1[i].part) > 1)
			{
				debug(99, "\nRound %d started, processing = %s",i,ret1[i].part);
				streamurl = string_resub("<url>", "</url>", ret1[i].part,0);
				
				if(ostrstr(ret1[i].part, "rtmp") != NULL)
				{
					char* ip1 = NULL, *pos1 = NULL, *path1 = NULL;
					ip1 = string_replace("http://", "", streamurl, 0);
				
					if(ip != NULL)
						pos1 = strchr(ip1, '/');
					if(pos1 != NULL)
					{
						pos1[0] = '\0';
						path1 = pos1 + 1;
					}

					free(streamurl), streamurl = NULL;
					tmpstr3 = gethttp(ip1, path1, 80, NULL, NULL, 10000, NULL, 0);
					streamurl = string_resub("<default-stream-url>", "</default-stream-url>", tmpstr3, 0);
					free(tmpstr3), tmpstr3 = NULL;
					free(ip1), ip1 = NULL;
				}

				title = string_resub("<formitaet basetype=\"", "\"",ret1[i].part,0);
				stringreplacechar(title, '_', ' ');
		
				if(ostrstr(ret1[i].part, "mp4") != NULL)
					pic = ostrcat("mp4.png", NULL, 0, 0);
				else if(ostrstr(ret1[i].part, "3gp") != NULL)
					pic = ostrcat("3gp.png", NULL, 0, 0);
				else if(ostrstr(ret1[i].part, "flv") != NULL)
					pic = ostrcat("flv.png", NULL, 0, 0);

				quali = string_resub("<quality>", "</quality>",ret1[i].part,0);

				if(title == NULL)
				{
					title = ostrcat(_("unknown"), NULL, 0, 0);
					title = ostrcat(title, " (", 1, 0);
					title = ostrcat(title, quali, 1, 0);
					title = ostrcat(title, ")", 1, 0);
					debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);
					debug(99, "(%d) ret1 %s\n",i, (ret1[i]).part);
				}
				else
				{
					title = ostrcat(title, " (", 1, 0);
					title = ostrcat(title, quali, 1, 0);
					title = ostrcat(title, ")", 1, 0);

					debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}
				free(quali), quali = NULL;
				free(title), title = NULL;
				free(pic), pic = NULL;
				free(streamurl), streamurl = NULL;
			}
		}
		free(ret1), ret1 = NULL;

		if(mlist != NULL)
		{
			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, 1, 0);
			if(mbox != NULL)
			{
				free(streamurl), streamurl = NULL;

				debug(99, "mbox->name %s", mbox->name);
				debug(99, "mbox->text %s", mbox->text);
				streamurl = ostrcat(mbox->text, NULL, 0, 0);

			}
		}
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;

	titheklog(debuglevel, "/tmp/zdf2_streamurl", NULL, streamurl");

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

#endif