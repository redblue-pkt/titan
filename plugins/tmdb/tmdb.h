#ifndef TMDB_H
#define TMDB_H

#define TMPTMDBPIC1 "/tmp/tmptmdb1.jpg"
#define TMPTMDBPIC2 "/tmp/tmptmdb2.jpg"
#define TMPTMDBPIC3 "/tmp/tmptmdb3.jpg"
#define TMPTMDBPIC4 "/tmp/tmptmdb4.jpg"
#define TMPTMDBPIC5 "/tmp/tmptmdb5.mvi"

struct tmdb* addtmdb(struct tmdb** first, int count, struct tmdb* last)
{
	//debug(1000, "in");
	struct tmdb *newnode = NULL, *prev = NULL, *node = *first;

	newnode = (struct tmdb*)calloc(1, sizeof(struct tmdb));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		*first = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

int deltmdb(struct tmdb** first, struct tmdb* tnode, int flag)
{
	debug(1000, "in");
	int ret = 1;

	struct tmdb *node = *first, *prev = *first;

	while(node != NULL)
	{
		if(node == tnode)
		{
			ret = 0;
			if(node == *first)
			{
				*first = node->next;
				if(*first != NULL)
					(*first)->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->title); node->title = NULL;
			free(node->year); node->year = NULL;
			free(node->rated); node->rated = NULL;
			free(node->released); node->released = NULL;
			free(node->genre); node->genre = NULL;
			free(node->runtime); node->runtime = NULL;
			free(node->plot); node->plot = NULL;
			free(node->postermid); node->postermid = NULL;
			free(node->rating); node->rating = NULL;
			free(node->votes); node->votes = NULL;
			free(node->id); node->id = NULL;
			free(node->orgname); node->orgname = NULL;
			free(node->imdbid); node->imdbid = NULL;
			free(node->backdrop); node->backdrop = NULL;
			free(node->language); node->language = NULL;
			free(node->type); node->type = NULL;
			free(node->score); node->score = NULL;
			free(node->cover); node->cover = NULL;
			free(node->thumb); node->thumb = NULL;
			free(node->mvi); node->mvi = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return ret;
}

void freetmdb(struct tmdb** first, int flag)
{
	debug(1000, "in");
	struct tmdb *node = *first, *prev = *first;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltmdb(first, prev, flag);
	}

	unlink(TMPTMDBPIC1);
	unlink(TMPTMDBPIC2);
	unlink(TMPTMDBPIC3);
	unlink(TMPTMDBPIC4);
	unlink(TMPTMDBPIC5);

	debug(1000, "out");
}

char* savetmdbpic(char* imdbid, char* url, char* tmppic, char* pic, int flag1)
{
	char* ip = NULL, *pos = NULL, *path = NULL, *ret = NULL;
	char* savefile = NULL;

	debug(133, "url: %s", url);
	
	if(imdbid == NULL || url == NULL) return NULL;
	ip = string_replace("http://", "", url, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	if(flag1 == 1)
	{
		savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
		savefile = ostrcat(savefile, imdbid, 1, 0);
		savefile = ostrcat(savefile, pic, 1, 0);
		if(!file_exist(savefile))
			gethttp(ip, path, 80, savefile, NULL, 5000, NULL, 0);
		ret = savefile;
	}
	else
	{
		gethttp(ip, path, 80, tmppic, NULL, 5000, NULL, 0);
		ret = ostrcat(tmppic, NULL, 0, 0);
	}

	free(ip); ip = NULL;
	return ret;
}

//flag: 0 = title search
//flag: 1 = imdbid search
//flag1: 0 = save pic in tmp
//flag1: 1 = save pic in mediadb path if pic not exist
//flag1: 2 = save pic in mediadb path 
//flag2: 2 = save no pic
struct tmdb* gettmdb(struct tmdb** first, char* input, int flag, int flag1)
{
	debug(133, "title: %s",input);
	debug(133, "flag: %d",flag);
	debug(133, "flag1: %d",flag1);

	if(checkinternet() == 1)
	{
		debug(133, "Error, Internet Connection not found");
		return NULL;
	}
		
	struct tmdb* tnode = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *logdir = NULL, *logfile = NULL, *tmpsearch = NULL, *savefile = NULL, *timen = NULL, *log = NULL, *posterurl = NULL, *title = NULL, *tmpjpg = NULL, *tmpmpg = NULL, *tmpmeta = NULL;

	title = ostrcat(title, input, 1, 0);

	int count = 0;
	int mvicount = 0;
/*
	tmpsearch = ostrcat("2.1/", NULL, 0, 0);
	if(flag == 0)
		tmpsearch = ostrcat(tmpsearch, "Movie.search", 1, 0);
	else
		tmpsearch = ostrcat(tmpsearch, "Movie.imdbLookup", 1, 0);
		
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "de", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/xml/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "7bcd34bb47bc65d20a49b6b446a32866", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');
*/

	if(flag == 1)
	{
		tmpsearch = ostrcat("3/movie/", NULL, 0, 0);
		tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	}
	else
		tmpsearch = ostrcat("3/movie/tt0083944", NULL, 0, 0);

	tmpsearch = ostrcat(tmpsearch, "?api_key=351217450d7c7865ca39c74a7e3a0a4b&language=de", 1, 0);

	debug(133, "search: http://api.themoviedb.org/%s", tmpsearch);
	tmpstr = gethttp("api.themoviedb.org", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
//	writesys("/var/usr/local/share/titan/plugins/tmdb/tmpstr", tmpstr, 1);
	
	debug(133, "tmpsearch: %s", tmpsearch);
	debug(133, "tmpstr: %s", tmpstr);
				
	free(tmpsearch); tmpsearch = NULL;

	logdir = ostrcat(getconfig("mediadbpath", NULL), "/.mediadbdebug", 0, 0);
	if(!file_exist(logdir))
		mkdir(logdir, 0777);
	logfile = ostrcat(logdir, "/imdb-scan.log", 0, 0);
	timen = ostrcat(oitoa(time(NULL)), NULL, 1, 0);

	tmpjpg = ostrcat("/tmp/backdrop.resize.", timen, 0, 0);
	tmpjpg = ostrcat(tmpjpg, ".jpg", 1, 0);

	tmpmpg = ostrcat("/tmp/backdrop.resize.", timen, 0, 0);
#ifdef MIPSEL
	tmpmpg = ostrcat(tmpmpg, ".mvi", 1, 0);
#else
	tmpmpg = ostrcat(tmpmpg, ".mpg", 1, 0);
#endif

	tmpmeta = ostrcat("/tmp/mediadb.", timen, 0, 0);
	tmpmeta = ostrcat(tmpmeta, ".meta", 1, 0);

	if(getconfigint("mediadbdebug", NULL) == 1)
		filedebug(logfile, "#############\nTitle: %s\n#############", title);

	free(tmpsearch); tmpsearch = NULL;
	char* ret = NULL;

	if(tmpstr != NULL && ostrstr(tmpstr, "Invalid id - The pre-requisite id is invalid or not found") == NULL)
	{
		ret = getxmlentry(tmpstr, "\"Response\":");
		if(ostrcmp(ret, "False") == 0)
		{
			free(ret); ret = NULL;
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		free(ret); ret = NULL;

		tnode = addtmdb(first, 1, tnode);
		if(tnode == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return *first;
		}
		count++;

		tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);
		int count = 0;
		int incount = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr1, ",", &count);
		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(ostrstr(ret1[i].part, "\"backdrop_path\"") != NULL)
				{
					tmpstr2 = string_resub("\"backdrop_path\":\"", "\"", ret1[i].part, 0);
					tmpstr3 = ostrcat("http://image.tmdb.org/t/p/w1280", tmpstr2, 0, 0);
					tnode->backdrop = ostrcat(tnode->backdrop, tmpstr3, 1, 0);
					tnode->backdrop = ostrcat(tnode->backdrop, "\n", 1, 0);
	
					mvicount++;
				}
			}
		}
		free(tmpstr2), tmpstr2 = NULL;				
		free(tmpstr3), tmpstr3 = NULL;	
		free(ret1), ret1 = NULL;
	
		tmpstr1 = getxmlentry(tmpstr, "\"genres\":");
		tmpstr2 = string_resub("[", "]", tmpstr1, 0);
		count = 0;
		incount = 0;
		i = 0;
		struct splitstr* ret2 = NULL;
		ret2 = strsplit(tmpstr2, ",", &count);
		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{	
				if(ostrstr(ret1[i].part, "\"name\"") != NULL)
				{
					tmpstr3 = string_resub("\"name\":\"", "\"", ret2[i].part, 0);
					if(tnode->genre != NULL)
						tnode->genre = ostrcat(tnode->genre, ",", 1, 0);						
					tnode->genre = ostrcat(tnode->genre, tmpstr3, 1, 0);
				}
			}
		}
		free(tmpstr2), tmpstr2 = NULL;				
		free(tmpstr3), tmpstr3 = NULL;
		free(ret2), ret2 = NULL;				
		
		tnode->title = getxmlentry(tmpstr, "\"title\":");

		tmpstr2 = getxmlentry(tmpstr, "\"spoken_languages\":");
		tnode->language = string_resub("\"name\":\"", "\"", tmpstr2, 0);
		free(tmpstr2), tmpstr2 = NULL;

		tnode->type = string_resub("\"adult\":", ",", tmpstr, 0);
		tnode->orgname = getxmlentry(tmpstr, "\"original_title\":");
		tnode->score = string_resub("\"popularity\":", ",", tmpstr, 0);
		tnode->rating = string_resub("\"vote_average\":", ",", tmpstr, 0);
		tnode->votes = string_resub("\"vote_count\":", "}", tmpstr, 0);
		tnode->released = getxmlentry(tmpstr, "\"release_date\":");
		tnode->runtime = string_resub("\"runtime\":", ",", tmpstr, 0);
		tnode->plot = getxmlentry(tmpstr, "\"overview\":");
		tnode->year = getxmlentry(tmpstr, "\"Year\":");

		tmpstr2 = getxmlentry(tmpstr, "\"poster_path\":");
		tnode->thumb = ostrcat("http://image.tmdb.org/t/p/w300", tmpstr2, 0, 0);
		free(tmpstr2), tmpstr2 = NULL;

		tmpstr2 = getxmlentry(tmpstr, "\"poster_path\":");
		tnode->cover = ostrcat("http://image.tmdb.org/t/p/w300", tmpstr2, 0, 0);
		free(tmpstr2), tmpstr2 = NULL;

		tmpstr2 = getxmlentry(tmpstr, "\"poster_path\":");
		tnode->postermid = ostrcat("http://image.tmdb.org/t/p/w500", tmpstr2, 0, 0);
		free(tmpstr2), tmpstr2 = NULL;
			
		tnode->year = getxmlentry(tmpstr, "\"release_date\":");
		tnode->imdbid = getxmlentry(tmpstr, "\"imdb_id\":");
		tnode->id = string_resub(",\"id\":", ",", tmpstr, 0);

		if(getconfigint("mediadbdebug", NULL) == 1 && tnode->backdrop == NULL)
		{
			log = ostrcat(logdir, "/tmdb_", 0, 0);
			if(tnode->imdbid != NULL)
				log = ostrcat(log, tnode->imdbid, 1, 0);
			else
				log = ostrcat(log, timen, 1, 0);
			
			log = ostrcat(log, "_html", 1, 0);
			writesys(log, tmpstr1, 1);
			free(log), log = NULL;
			
			log = ostrcat(logdir, "/tmdb_", 0, 0);
			if(tnode->imdbid != NULL)
				log = ostrcat(log, tnode->imdbid, 1, 0);
			else
				log = ostrcat(log, timen, 1, 0);

			log = ostrcat(log, "_link", 1, 0);
			writesys(log, tnode->backdrop, 1);
			free(log), log = NULL;
		}
			
		if((flag1 == 0 && count == 1) || flag1 == 1 || flag1 == 2)
		{
			savefile = savetmdbpic(tnode->imdbid, tnode->thumb, TMPTMDBPIC1, "_thumb.jpg", flag1);
			free(tnode->thumb);
			tnode->thumb = savefile;

			savefile = savetmdbpic(tnode->imdbid, tnode->cover, TMPTMDBPIC2, "_cover.jpg", flag1);
			free(tnode->cover);
			tnode->cover = savefile;

			savefile = savetmdbpic(tnode->imdbid, tnode->postermid, TMPTMDBPIC3, "_postermid.jpg", flag1);
			posterurl = ostrcat(posterurl, tnode->postermid, 1, 0);
			free(tnode->postermid);
			tnode->postermid = savefile;

			if(mvicount == 0)
			{
				savefile = savetmdbpic(tnode->imdbid, tnode->backdrop, TMPTMDBPIC4, "_backdrop1.jpg", flag1);
				free(tnode->backdrop);
				tnode->backdrop = savefile;					
			}
		}
					
		if((flag1 == 1 || flag1 == 2) && tnode->backdrop != NULL && tnode->imdbid != NULL) 
		{
			debug(133, "load backdrop flag1: %d", flag1);
			char* tmppath = NULL;
			mvicount = 0;
			char* tmpstr2 = NULL;
			tmpstr2 = ostrcat(tnode->backdrop, NULL, 0, 0);
			struct splitstr* ret1 = NULL;
			int rcount = 0;
			int i = 0;
			ret1 = strsplit(tmpstr2, "\n", &rcount);

			for(i = 0; i < rcount; i++)
			{
				mvicount++;
				debug(133, "load %s\n",(&ret1[i])->part);

				if(!ostrncmp("http://", (&ret1[i])->part, 7))
				{
					tmppath = ostrcat("_backdrop", oitoa(mvicount), 0, 1);
					tmppath = ostrcat(tmppath, ".jpg", 1, 0);

					savefile = savetmdbpic(tnode->imdbid, (&ret1[i])->part, TMPTMDBPIC4, tmppath, flag1);
					if(file_exist(savefile))
					{
						free(tnode->backdrop);
						tnode->backdrop = savefile;

						free(tnode->mvi);
						tnode->mvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
						tnode->mvi = ostrcat(tnode->mvi, tnode->imdbid, 1, 0);
						tnode->mvi = ostrcat(tnode->mvi, "_backdrop", 1, 0);
						tnode->mvi = ostrcat(tnode->mvi, oitoa(mvicount), 1, 1);
						tnode->mvi = ostrcat(tnode->mvi, ".mvi", 1, 0);
						
						if((!file_exist(tnode->mvi)) || (flag1 == 2))
						{
							off64_t filesize = getfilesize(tnode->backdrop);
							debug(133, "filesize %lld", filesize);
							
							if(filesize < 1500000)
							{
								char* cmd = NULL, *size = NULL;
#ifndef MIPSEL
								cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
								cmd = ostrcat(cmd, tnode->backdrop, 1, 0);
								cmd = ostrcat(cmd, " > ", 1, 0);
								cmd = ostrcat(cmd, tmpmeta, 1, 0);
								cmd = ostrcat(cmd, " 2>&1", 1, 0);
				
								debug(133, "cmd %s", cmd);
								system(cmd);
								free(cmd); cmd = NULL;

								cmd = ostrcat(cmd, "cat ", 1, 0);
								cmd = ostrcat(cmd, tmpmeta, 1, 0);
								cmd = ostrcat(cmd, " | grep Stream | awk '{print $6}' | cut -d'x' -f1", 1, 0);
								size = string_newline(command(cmd));
								free(cmd), cmd = NULL;	
								debug(133, "size %s", size);
								if(size != NULL)
								{
									debug(133, "size %d", atoi(size));
									int picsize = atoi(size);
									unlink(tmpmeta);

									if(picsize < 2000)
									{
										debug(133, "size ok %d", picsize);
#endif
#ifdef MIPSEL
										cmd = ostrcat(cmd, "cp -a ", 1, 0);
										cmd = ostrcat(cmd, tnode->backdrop, 1, 0);
										cmd = ostrcat(cmd, " ", 1, 0);
										cmd = ostrcat(cmd, tmpjpg, 1, 0);
#else
										cmd = ostrcat(cmd, "jpegtran -outfile ", 1, 0);
										cmd = ostrcat(cmd, tmpjpg, 1, 0);
										cmd = ostrcat(cmd, " -copy none ", 1, 0);
										cmd = ostrcat(cmd, tnode->backdrop, 1, 0);
#endif
										debug(133, "cmd %s", cmd);
										system(cmd);
										free(cmd); cmd = NULL;
										if(file_exist(tmpjpg))
										{
											free(cmd), cmd = NULL;
#ifdef MIPSEL
											cmd = ostrcat(cmd, "jpeg2yuv -v 1 -f 25 -n1 -I p -j ", 1, 0);
											cmd = ostrcat(cmd, tmpjpg, 1, 0);
											cmd = ostrcat(cmd, " | mpeg2enc -v 1 -x 1280 -y 720 -a 3 -f12 -4 1 -2 1 -q 1 -H -o ", 1, 0);
											cmd = ostrcat(cmd, tmpmpg, 1, 0);
#else
											cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i ", 1, 0);
											cmd = ostrcat(cmd, tmpjpg, 1, 0);
											cmd = ostrcat(cmd, " ", 1, 0);
											cmd = ostrcat(cmd, tmpmpg, 1, 0);
#endif

											if(getconfigint("mediadbdebug", NULL) == 1)
											{
												filedebug(logfile, "#############\nbackdrop: %s size=(%s) filesize(%lld) (%s)\n#############", tnode->backdrop, size, filesize, (&ret1[i])->part);
												cmd = ostrcat(cmd, " >> ", 1, 0);
												cmd = ostrcat(cmd, logfile, 1, 0);
												cmd = ostrcat(cmd, " 2>&1", 1, 0);
											}
											else
											{
												cmd = ostrcat(cmd, " > /dev/null 2>&1", 1, 0);
											}
											debug(133, "cmd %s", cmd);
											system(cmd);
											free(cmd); cmd = NULL;
											
											if(file_exist(tmpmpg))
											{
												cmd = ostrcat(cmd, "mv -f ", 1, 0);
												cmd = ostrcat(cmd, tmpmpg, 1, 0);
												cmd = ostrcat(cmd, " ", 1, 0);
												cmd = ostrcat(cmd, tnode->mvi, 1, 0);
												debug(133, "cmd %s", cmd);
												system(cmd);
												free(cmd); cmd = NULL;
												
												if(getconfigint("mediadbdebug", NULL) == 1)
													writesysint("/proc/sys/vm/drop_caches", 3, 0);
												
												if(mvicount == getconfigint("mediadbbackdrop", NULL))
													break;
											}
											else
												mvicount--;
										}
#ifndef MIPSEL
									}
									else
									{
										debug(133, "ERROR Backdrop size to big skipped %d", picsize);

										if(getconfigint("mediadbdebug", NULL) == 1)
											filedebug(logfile, "#############\nERROR Backdrop size to big skipped: %s size=(%s) filesize(%lld) (%s)\n#############", tnode->backdrop, size, filesize, (&ret1[i])->part);
										mvicount--;
									}
								}
								else
								{
									debug(133, "ERROR Backdrop size is NULL skipped %s", size);

									if(getconfigint("mediadbdebug", NULL) == 1)
										filedebug(logfile, "#############\nERROR Backdrop size is NULL skipped: %s size=(%s) filesize(%lld) (%s)\n#############", tnode->backdrop, size, filesize, (&ret1[i])->part);

									mvicount--;
								}
								free(size), size = NULL;
#endif
								unlink(tmpmeta);
								unlink(tmpjpg);
								unlink(tmpmpg);
							}
							else
							{
								debug(133, "ERROR Backdrop filesize to BIG skipped %lld", filesize);
		
								if(getconfigint("mediadbdebug", NULL) == 1)
									filedebug(logfile, "#############\nERROR Backdrop filesize to BIG skipped: %s filesize(%lld) (%s)\n#############", tnode->backdrop, filesize, (&ret1[i])->part);
							}
						}								

						free(tmppath), tmppath = NULL;
					}
					else
						mvicount--;
				}
				else
				{
					log = ostrcat(logdir, "/tmdb_", 0, 0);
					if(tnode->imdbid != NULL)
						log = ostrcat(log, tnode->imdbid, 1, 0);
					else
						log = ostrcat(log, timen, 1, 0);
					
					log = ostrcat(log, "_html", 1, 0);
					
					char* cmd = NULL;
					cmd = command("free");
					log = ostrcat(log, "_error_link", 1, 0);
					cmd = ostrcat(cmd, "\nlink: ", 1, 0);
					cmd = ostrcat(cmd, (&ret1[i])->part, 1, 0);
					writesys(log, cmd, 1);
					free(cmd), cmd = NULL;
					free(log), log = NULL;
					mvicount--;
				}

				if(file_exist(tnode->mvi))
					unlink(tnode->backdrop);
			}
			free(ret1), ret1 = NULL;				
			free(tmpstr2), tmpstr2 = NULL;
		}
		else if((flag1 == 1 || flag1 == 2) && tnode->postermid != NULL && tnode->imdbid != NULL)
		{
			debug(133, "load postermid flag1: %d", flag1);

			free(tnode->mvi);
			tnode->mvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
			tnode->mvi = ostrcat(tnode->mvi, tnode->imdbid, 1, 0);
			tnode->mvi = ostrcat(tnode->mvi, "_backdrop1.mvi", 1, 0);
			
			if((!file_exist(tnode->mvi)) || (flag1 == 2))
			{

				off64_t filesize = getfilesize(tnode->backdrop);
				debug(133, "filesize %lld", filesize);
				
				if(filesize < 1500000)
				{
					char* cmd = NULL, *size = NULL;
#ifndef MIPSEL
					cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
					cmd = ostrcat(cmd, tnode->postermid, 1, 0);
					cmd = ostrcat(cmd, " > ", 1, 0);
					cmd = ostrcat(cmd, tmpmeta, 1, 0);
					cmd = ostrcat(cmd, " 2>&1", 1, 0);
	
					debug(133, "cmd %s", cmd);
					system(cmd);
					free(cmd); cmd = NULL;

					cmd = ostrcat(cmd, "cat ", 1, 0);
					cmd = ostrcat(cmd, tmpmeta, 1, 0);
					cmd = ostrcat(cmd, " | grep Stream | awk '{print $6}' | cut -d'x' -f1", 1, 0);
					size = string_newline(command(cmd));
					free(cmd), cmd = NULL;
					debug(133, "size %s", size);
					if(size != NULL)
					{
						debug(133, "size %d", atoi(size));
						int picsize = atoi(size);
	
						if(picsize < 2000)
						{
							debug(133, "size ok %d", picsize);
#endif
#ifdef MIPSEL
							cmd = ostrcat(cmd, "cp -a ", 1, 0);
							cmd = ostrcat(cmd, tnode->postermid, 1, 0);
							cmd = ostrcat(cmd, " ", 1, 0);
							cmd = ostrcat(cmd, tmpjpg, 1, 0);
#else
							cmd = ostrcat(cmd, "jpegtran -outfile ", 1, 0);
							cmd = ostrcat(cmd, tmpjpg, 1, 0);
							cmd = ostrcat(cmd, " -copy none ", 1, 0);
							cmd = ostrcat(cmd, tnode->postermid, 1, 0);
#endif
							debug(133, "cmd %s", cmd);
							system(cmd);
							free(cmd); cmd = NULL;
											
							if(file_exist(tmpjpg))
							{
								free(cmd), cmd = NULL;
#ifdef MIPSEL
								cmd = ostrcat(cmd, "jpeg2yuv -v 1 -f 25 -n1 -I p -j ", 1, 0);
								cmd = ostrcat(cmd, tmpjpg, 1, 0);
								cmd = ostrcat(cmd, " | mpeg2enc -v 1 -x 1280 -y 720 -a 3 -f12 -4 1 -2 1 -q 1 -H -o ", 1, 0);
								cmd = ostrcat(cmd, tmpmpg, 1, 0);
#else
								cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i ", 1, 0);
								cmd = ostrcat(cmd, tmpjpg, 1, 0);
								cmd = ostrcat(cmd, " ", 1, 0);
								cmd = ostrcat(cmd, tmpmpg, 1, 0);
#endif

								if(getconfigint("mediadbdebug", NULL) == 1)
								{
									filedebug(logfile, "#############\npostermid: %s size=(%s) filesize(%lld) (%s)\n#############", tnode->postermid, size, filesize, posterurl);	
									cmd = ostrcat(cmd, " >> ", 1, 0);
									cmd = ostrcat(cmd, logfile, 1, 0);
									cmd = ostrcat(cmd, " 2>&1", 1, 0);
								}
								else
								{
									cmd = ostrcat(cmd, " > /dev/null 2>&1", 1, 0);
								}

								debug(133, "cmd %s", cmd);
								system(cmd);
								free(cmd); cmd = NULL;
								if(file_exist(tmpmpg))
								{					
									cmd = ostrcat(cmd, "mv -f ", 1, 0);
									cmd = ostrcat(cmd, tmpmpg, 1, 0);
									cmd = ostrcat(cmd, " ", 1, 0);
									cmd = ostrcat(cmd, tnode->mvi, 1, 0);
									debug(133, "cmd %s", cmd);
									system(cmd);
									free(cmd); cmd = NULL;
									
									writesysint("/proc/sys/vm/drop_caches", 3, 0);
								}
							}
							free(cmd), cmd = NULL;
#ifndef MIPSEL
						}
						else
						{
							debug(133, "ERROR Postermid size to big skipped %d", picsize);

							if(getconfigint("mediadbdebug", NULL) == 1)
								filedebug(logfile, "#############\nERROR Postermid size to big skipped: %s size=(%s) filesize(%lld) (%s)\n#############", tnode->postermid, size, filesize, posterurl);	
						}
					}
					else
					{
						debug(133, "ERROR Postermid size is NULL skipped %s", size);

						if(getconfigint("mediadbdebug", NULL) == 1)
							filedebug(logfile, "#############\nERROR Postermid size is NULL skipped: %s size=(%s) filesize(%lld) (%s)\n#############", tnode->postermid, size, filesize, posterurl);
					}
					free(size), size = NULL;
#endif
					unlink(tmpmeta);
					unlink(tmpjpg);
					unlink(tmpmpg);
				}
				else
				{
					debug(133, "ERROR Postermid filesize to BIG skipped %lld", filesize);

					if(getconfigint("mediadbdebug", NULL) == 1)
						filedebug(logfile, "#############\nERROR Postermid filesize to BIG skipped: %s filesize(%lld) (%s)\n#############", tnode->postermid, filesize, posterurl);	
				}
			}
		}


		debug(133, "mvi=%s (mvicount=%d)", tnode->mvi,flag1);
		debug(133, "backdrop=%s (mvicount=%d)", tnode->backdrop,flag1);

		if(file_exist(tnode->mvi))
			unlink(tnode->backdrop);

		if(mvicount > 0)
		{
			free(tnode->mvi);
			tnode->mvi = ostrcat(oitoa(mvicount), NULL, 1, 0);
		}
		else
		{
			free(tnode->mvi);
			tnode->mvi = ostrcat("1", NULL, 0, 0);
		}
		debug(133, "change mvi=%s (mvicount=%d)", tnode->mvi,flag1);
			
		free(tmpstr); tmpstr = NULL;

		debug(133, "----------------------tmdb start----------------------");
		debug(133, "title: %s", tnode->title);
		debug(133, "language: %s", tnode->language);
		debug(133, "type: %s", tnode->type);
		debug(133, "orgname: %s", tnode->orgname);
		debug(133, "score: %s", tnode->score);
		debug(133, "rating: %s", tnode->rating);
		debug(133, "released: %s", tnode->released);
		debug(133, "genre: %s", tnode->genre);
		debug(133, "runtime: %s", tnode->runtime);
		debug(133, "plot: %s", tnode->plot);
		debug(133, "thumb: %s", tnode->thumb);
		debug(133, "cover: %s", tnode->cover);
		debug(133, "postermid: %s", tnode->postermid);
		debug(133, "backdrop: %s", tnode->backdrop);
		debug(133, "mvi: %s", tnode->mvi);
		debug(133, "rating: %s", tnode->rating);
		debug(133, "votes: %s", tnode->votes);
		debug(133, "id: %s", tnode->id);	
		debug(133, "imdbid: %s", tnode->imdbid);
		debug(133, "----------------------tmdb end----------------------");
	}

	return *first;
}

//flag 0: only view
//flag 1: can return tmdb node
struct tmdb* screentmdb(char* title, char* skinname, int flag, char* path, char* file)
{
	debug(133, "title: %s",title);
	debug(133, "path: %s",path);
	debug(133, "file: %s",file);	
	char* searchstr = NULL, *tmpstr = NULL;	

	if(checkinternet() == 1)
	{
		textbox(_("Message"), _("Error, Internet Connection not found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 500, 10, 0);
		return NULL;
	}

	int rcret = 0;
	struct skin* tmdbskin = NULL;

	if(skinname != NULL)
		tmdbskin = getscreen(skinname);
	else
		tmdbskin = getscreen("tmdb");

	struct skin* skin_plot = getscreennode(tmdbskin, "plot");
	struct skin* skin_title = getscreennode(tmdbskin, "title");
	struct skin* skin_orgname = getscreennode(tmdbskin, "orgname");
	struct skin* skin_rating = getscreennode(tmdbskin, "rating");
	struct skin* skin_genre = getscreennode(tmdbskin, "genre");
	struct skin* skin_released = getscreennode(tmdbskin, "released");
	struct skin* skin_cover = getscreennode(tmdbskin, "cover");
	struct skin* skin_votes = getscreennode(tmdbskin, "votes");
	struct skin* skin_b3 = getscreennode(tmdbskin, "b3");
	struct skin* load = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	
	struct tmdb* node = NULL, *retnode = NULL;
	char* search = NULL;

//	setfbtransparent(255);
	status.hangtime = 99999;

//	if(flag == 0)
//		skin_b3->hidden = YES;
//	else
//		skin_b3->hidden = NO;

	if(path == NULL || file == NULL || !file_exist(getconfig("mediadbpath", NULL)))
		skin_b3->hidden = YES;
	else
		skin_b3->hidden = NO;
				
	if(title == NULL)
		searchstr = getepgakttitle(NULL);
	else
	{
		int isrec = 0, iscam = 0;
		tmpstr = createshortname(file, &isrec, &iscam, 1);
		if(tmpstr != NULL)
			searchstr = ostrcat(searchstr, tmpstr, 1, 0);
		else
			searchstr = ostrcat(searchstr, title, 1, 0);
	}

	debug(133, "searchstr: %s",searchstr);

	drawscreen(blackscreen, 0, 0);
	drawscreen(load, 0, 0);

	node = gettmdb(&node, searchstr, 0, 0);
	clearscreen(load);
	clearscreen(blackscreen);

start:
	if(node != NULL)
	{
		changetext(skin_plot, node->plot);
		changetext(skin_title, node->title);
		changetext(skin_orgname, node->orgname);
		changetext(skin_rating, node->rating);
		changetext(skin_genre, node->genre);
		changetext(skin_released, node->released);
		char* tmpstr = NULL;
		tmpstr = ostrcat("( ", node->votes, 0, 0);
		tmpstr = ostrcat(tmpstr, " votes)", 1, 0);		
		changetext(skin_votes, tmpstr);
		free(tmpstr), tmpstr = NULL;
		changepic(skin_cover, TMPTMDBPIC3);
		skin_cover->hidden = NO;
	}
	else
	{
		changetext(skin_plot, _("--plot--"));
		changetext(skin_title, _("--title--"));
		changetext(skin_orgname, _("--orgname--"));
		changetext(skin_rating, _("--rating--"));
		changetext(skin_genre, _("--genre--"));
		changetext(skin_released, _("--releasetime--"));
		changetext(skin_votes, _("( --votes-- )"));		
		skin_cover->hidden = YES;
	}

	drawscreen(tmdbskin, 0, 0);

	while(1)
	{
		rcret = waitrc(tmdbskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
		{
			search = textinput(_("Search"), searchstr);
			if(search != NULL)
			{
				freetmdb(&node, 0), node = NULL;
				drawscreen(blackscreen, 0, 0);
				drawscreen(load, 0, 0);
				node = gettmdb(&node, search, 0, 0);
				clearscreen(load);
				clearscreen(blackscreen);
				free(search), search = NULL;
				goto start;
			}
			drawscreen(tmdbskin, 0, 0);
			continue;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			struct tmdb* tnode = node;
			struct menulist* mlist = NULL, *mbox = NULL;

			while(tnode != NULL)
			{
				addmenulist(&mlist, tnode->title, tnode->imdbid, NULL, 0, 0);
				tnode = tnode->next;
			}

			mbox = menulistbox(mlist, NULL, _("Result List"), _("Choose your Title from the following list"), NULL, NULL, 0, 0);
			if(mbox != NULL)
			{
				search = ostrcat(mbox->text, NULL, 0, 0);
				if(search != NULL)
				{
					freetmdb(&node, 0); node = NULL;
					drawscreen(blackscreen, 0, 0);
					drawscreen(load, 0, 0);
					node = gettmdb(&node, search, 1, 0);
					clearscreen(load);
					clearscreen(blackscreen);
					free(search), search = NULL;
					freemenulist(mlist, 1); mlist = NULL, mbox = NULL;
					goto start;
				}
			}
			freemenulist(mlist, 1); mlist = NULL, mbox = NULL;
			drawscreen(tmdbskin, 0, 0);
			continue;
		}
		if(rcret == getrcconfigint("rcyellow", NULL) && flag == 1)
		{
			retnode = node;
			break;
		}

		if(rcret == getrcconfigint("rcyellow", NULL) && path != NULL && file != NULL && node != NULL && node->imdbid != NULL && flag == 2)
		{
			drawscreen(blackscreen, 0, 0);
			drawscreen(load, 0, 0);
			debug(133, "path: %s",path);
			debug(133, "file: %s",file);
			debug(133, "type: 2");
			debug(133, "node->imdbid: %s",node->imdbid);
			if(ostrncmp("tt", node->imdbid, 2))
				textbox(_("Message"), _("Error, found result without imdbid we can't this save in MediaDB"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 10, 0);
			else
			{
				addconfigtmp("mediadbscantimeout", "0");
				mediadbfindfilecb(path, file, 0, node->imdbid, 2);
				delconfigtmp("mediadbscantimeout");			
			}
			clearscreen(load);
			clearscreen(blackscreen);
			break;
		}
	}

	if(retnode == NULL)
	{
		freetmdb(&node, 0);
		node = NULL;
	}

	free(searchstr), searchstr = NULL;
//	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(tmdbskin);

	return retnode;
}

#endif
