#ifndef GIGA_H
#define GIGA_H

char* giga(char* link)
{
//	int debuglevel = getconfigint("debuglevel", NULL);
	char* streamurl = NULL, *tmpstr = NULL, *title = NULL, *pic = NULL, *label = NULL;

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	tmpstr = string_replace_all("\n", " ", tmpstr, 1);
	tmpstr = string_replace_all("{file: \"", "\n{ile: \"", tmpstr, 1);
	
	int incount = 0, count = 0, i = 0;	
	struct splitstr* ret1 = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	ret1 = strsplit(tmpstr, "\n", &count);
	for(i = 0; i < count; i++)
	{
		if(ostrstr(ret1[i].part, "{ile: \"") != NULL)
		{
			incount++;
			free(streamurl), streamurl = NULL;			
			streamurl = string_resub("{ile: \"", "\"", ret1[i].part, 0);
	
			if(ostrstr(streamurl, "rtmp") != NULL)
				title = ostrcat("Rtmpe Stream (", NULL, 0, 0);
			else
				title = ostrcat("Http Stream (", NULL, 0, 0);
	
			label = string_resub("label: \"", "\"", ret1[i].part, 0);
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

	free(tmpstr), tmpstr = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

int giga_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/giga/streams/giga.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/giga.search.list", NULL, 0, 0);
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