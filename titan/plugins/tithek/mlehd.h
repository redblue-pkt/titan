#ifndef MLEHD_H
#define MLEHD_H

void mlehd_userauth()
{
	int ret = 0;
	char* pass = NULL, *user = NULL, *url = NULL;

	user = getconfig("mlehd_user", NULL);
	pass = getconfig("mlehd_pass", NULL);

	url = ostrcat("http://", NULL, 0, 0);
	url = ostrcat(url, "mle", 1, 0);
	url = ostrcat(url, "-hd.se", 1, 0);
	url = ostrcat(url, "/forum/include.php?path=login/login.php", 1, 0);

	ret = phpkit_userauth(url, user, pass);
	if(ret == 1)
	{
		// user login
		debug(99, "Mlehd connecting UserAuth: OK");		
	}
	else
	{
		// guest login
		debug(99, "Mlehd connecting Guest: OK");
	}

	free(url),url = NULL;
}

char* mlehd(char* link)
{
	debug(99, "link %s", link);

	mlehd_userauth();

	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *tmpstr = NULL, *tmppath = NULL;
	
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	tmppath = string_resub("file: '", "'", tmpstr, 0);
	debug(99, "tmppath: %s", tmppath);
	free(tmpstr), tmpstr = NULL;

	if(tmppath == NULL) return streamurl;

	tmppath = string_replace("http://www.mle-hd.se/", "", tmppath, 1);
	debug(99, "tmppath: %s", tmppath);	
	tmpstr = gethttp(ip, tmppath, 80, NULL, NULL, 10000, NULL, 0);

	if(tmpstr != NULL)
	{
		streamurl = string_resub("<location>", "</location>", tmpstr, 0);
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;
	free(tmppath), tmppath = NULL;

  titheklog(debuglevel, "/tmp/mlehd2_streamurl", NULL, NULL, NULL, streamurl);

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

int mlehd_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/mlehd/streams/mlehd.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/mlehd.search.list", NULL, 0, 0);
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