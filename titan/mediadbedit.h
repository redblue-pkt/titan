#ifndef MEDIADBEDIT_H
#define MEDIADBEDIT_H

void screenmediadbedit(char* file, int menuid)
{
	int rcret = 0, type = 0, i = 0;
	struct skin* tmcedit = getscreen("mediadbedit");
	struct skin* listbox = getscreennode(tmcedit, "listbox");
	struct skin* title = getscreennode(tmcedit, "title");
	struct skin* year = getscreennode(tmcedit, "year");
	struct skin* released = getscreennode(tmcedit, "released");
	struct skin* runtime = getscreennode(tmcedit, "runtime");
	struct skin* genre = getscreennode(tmcedit, "genre");
	struct skin* director = getscreennode(tmcedit, "director");
	struct skin* writer = getscreennode(tmcedit, "writer");
	struct skin* actors = getscreennode(tmcedit, "actors");
	struct skin* plot = getscreennode(tmcedit, "plot");
	struct skin* rating = getscreennode(tmcedit, "rating");
	struct skin* votes = getscreennode(tmcedit, "votes");
	struct skin* locked = getscreennode(tmcedit, "locked");
	struct skin* picture = getscreennode(tmcedit, "picture");
	struct skin* shortname = getscreennode(tmcedit, "shortname");
	struct skin* fileinfo = getscreennode(tmcedit, "fileinfo");
	struct skin* tmp = NULL;
	struct skin* load = getscreen("loading");
	char* tmpstr = NULL, *bg = NULL, *picret = NULL;
	struct mediadb* node = NULL;
	
	if(file != NULL)
	{
		char* dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		node = getmediadb(dname, basename(file), 0);
		free(dname); dname = NULL;
	}

	if(menuid == 3) type = 0; //video
	if(menuid == 4) type = 1; //audio
	if(menuid == 2) type = 2; //picture

	if(node != NULL)
	{
		changeinput(picture, NULL);
		changeinput(title, node->title);

		changemask(year, "0000");
		tmpstr = oitoa(node->year);
		changeinput(year, tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(year->input != NULL)
		{
			int len = strlen(year->input);
			for(i = 0; i < 4 - len; i++)
				year->input = ostrcat("0", year->input, 0, 1);
		}

		changeinput(released, node->released);
		changeinput(runtime, node->runtime);
		changeinput(genre, node->genre);
		changeinput(director, node->director);
		changeinput(writer, node->writer);
		changeinput(actors, node->actors);
		changeinput(plot, node->plot);
		changeinput(shortname, node->shortname);
		changeinput(fileinfo, node->fileinfo);

		tmpstr = oitoa(node->rating);
		changeinput(rating, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
		setchoiceboxselection(rating, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changemask(votes, "0000000");
		tmpstr = oitoa(node->votes);
		changeinput(votes, tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(votes->input != NULL)
		{
			int len = strlen(votes->input);
			for(i = 0; i < 7 - len; i++)
				votes->input = ostrcat("0", votes->input, 0, 1);
		}

		addchoicebox(locked, "0", _("unlock -> lock it"));
		addchoicebox(locked, "1", _("unlock -> leaf unlock"));
		addchoicebox(locked, "2", _("locked -> unlock it"));
		addchoicebox(locked, "3", _("locked -> leaf locked"));

		if(checkbit(node->flag, 31) == 1)
			setchoiceboxselection(locked, "3");
		else
			setchoiceboxselection(locked, "0");

		drawscreen(tmcedit, 2, 0);
		bg = savescreen(tmcedit);

		addscreenrc(tmcedit, listbox);
		drawscreen(tmcedit, 0, 0);

		tmp = listbox->select;
		while(1)
		{
			addscreenrc(tmcedit, tmp);
			rcret = waitrcext(tmcedit, 0, 0, 1000);
			delownerrc(tmcedit);
			addscreenrc(tmcedit, listbox);
			tmp = listbox->select;

			if(rcret == getrcconfigint("rcexit", NULL)) break;
			if(rcret == getrcconfigint("rcok", NULL))
			{
				drawscreen(load, 0, 0);
				unsigned long hash = 0;

				if(node->id == NULL || strlen(node->id) == 0)
				{
					hash = gethash(node->file);
					tmpstr = olutoa(hash);
				}
				else
					tmpstr = ostrcat(node->id, NULL, 0, 0);

				if(ostrcmp(locked->ret, "1") == 0 || ostrcmp(locked->ret, "2") == 0)
					node->flag = clearbit(node->flag, 31);
				if(ostrcmp(locked->ret, "0") == 0 || ostrcmp(locked->ret, "3") == 0)
					node->flag = setbit(node->flag, 31);

				if(picret != NULL)
				{
					int channels = 0;
					unsigned long width = 0, height = 0, rowbytes = 0;
					char* thumb = NULL;
					unsigned char* buf = NULL;
					
					buf = loadjpg(picret, &width, &height, &rowbytes, &channels, 16);
					thumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					thumb = ostrcat(thumb, tmpstr, 1, 0);
					thumb = ostrcat(thumb, "_thumb.jpg", 1, 0);
					buf = savejpg(thumb, width, height, channels, 91, 140, 70, buf);
					free(thumb); thumb = NULL;
					free(buf); buf = NULL;
          
					buf = loadjpg(picret, &width, &height, &rowbytes, &channels, 16);
					thumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					thumb = ostrcat(thumb, tmpstr, 1, 0);
					thumb = ostrcat(thumb, "_cover.jpg", 1, 0);
					buf = savejpg(thumb, width, height, channels, 185, 264, 70, buf);
					free(thumb); thumb = NULL;
					free(buf); buf = NULL;

					buf = loadjpg(picret, &width, &height, &rowbytes, &channels, 16);
					thumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					thumb = ostrcat(thumb, tmpstr, 1, 0);
					thumb = ostrcat(thumb, "_poster.jpg", 1, 0);
					buf = savejpg(thumb, width, height, channels, 400, 450, 70, buf);
					free(thumb); thumb = NULL;
					free(buf); buf = NULL;

					buf = loadjpg(picret, &width, &height, &rowbytes, &channels, 16);
					thumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					thumb = ostrcat(thumb, tmpstr, 1, 0);
					thumb = ostrcat(thumb, "_postermid.jpg", 1, 0);
					buf = savejpg(thumb, width, height, channels, 400, 450, 70, buf);
					free(thumb); thumb = NULL;
					free(buf); buf = NULL;

					char* size = NULL, *logdir = NULL, *logfile = NULL, *cmd = NULL, *mvi = NULL;
					mvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					mvi = ostrcat(mvi, node->id, 1, 0);
					mvi = ostrcat(mvi, "_backdrop1.mvi", 1, 0);
					
					logdir = ostrcat(getconfig("mediadbpath", NULL), "/.mediadbdebug", 0, 0);
					if(!file_exist(logdir))
						mkdir(logdir, 0777);
					logfile = ostrcat(logdir, "/imdb-scan.log", 0, 0);
			
					off64_t filesize = getfilesize(picret);
					debug(133, "filesize %lld", filesize);
								
					if(filesize < 1500000)
					{
						cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
						cmd = ostrcat(cmd, picret, 1, 0);
						cmd = ostrcat(cmd, " > /tmp/mediadb.meta 2>&1", 1, 0);
	
						debug(133, "cmd %s", cmd);
						system(cmd);
						free(cmd); cmd = NULL;
	
						size = string_newline(command("cat /tmp/mediadb.meta | grep Stream | awk '{print $6}' | cut -d'x' -f1"));
						debug(133, "size %s", size);
						if(size != NULL)
						{
							debug(133, "size %d", atoi(size));
							int picsize = atoi(size);
		
							if(picsize < 2000)
							{
								debug(133, "size ok %d", picsize);
												
								cmd = ostrcat(cmd, "jpegtran -outfile /tmp/backdrop.resize.jpg -copy none ", 1, 0);
								cmd = ostrcat(cmd, picret, 1, 0);
				
								debug(133, "cmd %s", cmd);
								system(cmd);
								free(cmd); cmd = NULL;
			
								if(file_exist("/tmp/backdrop.resize.jpg"))
								{
									if(getconfigint("mediadbdebug", NULL) == 1)
									{
										writesys(logfile, "#############", 3); 
										writesys(logfile, "Edit Localfile: ", 2); 
										writesys(logfile, picret, 2);										
										writesys(logfile, " size=(", 2);
										writesys(logfile, size, 2);
										writesys(logfile, ") filesize(", 2);
										writesys(logfile, oitoa(filesize), 2);
										writesys(logfile, ") (", 2);
										writesys(logfile, node->id, 2);
										writesys(logfile, ")", 3);
										writesys(logfile, "#############", 3);
		
										cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg >> ", 1, 0);
										cmd = ostrcat(cmd, logfile, 1, 0);
										cmd = ostrcat(cmd, " 2>&1", 1, 0);
									}
									else
									{
										cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg > /dev/null 2>&1", 1, 0);
									}
									debug(133, "cmd %s", cmd);
									system(cmd);
									free(cmd); cmd = NULL;
	
									if(file_exist("/tmp/backdrop.resize.mpg"))
									{					
										cmd = ostrcat(cmd, "mv -f /tmp/backdrop.resize.mpg ", 1, 0);
										cmd = ostrcat(cmd, mvi, 1, 0);
										debug(133, "cmd %s", cmd);
										system(cmd);
										free(cmd); cmd = NULL;
										
										writesysint("/proc/sys/vm/drop_caches", 3, 0);
										free(mvi), mvi = NULL;
										node->backdropcount = 1;
									}
									else
									{
										free(mvi), mvi = NULL;
										node->backdropcount = 0;
									}
								}
							}
							else
							{
								debug(133, "ERROR Edit Localfile size to big skipped %d", picsize);
		
								if(getconfigint("mediadbdebug", NULL) == 1)
								{
									writesys(logfile, "#############", 3); 
									writesys(logfile, "ERROR Edit Localfile size to big skipped: ", 3); 
									writesys(logfile, picret, 2);
									writesys(logfile, " size=(", 2);
									writesys(logfile, size, 2);
									writesys(logfile, ") filesize(", 2);
									writesys(logfile, oitoa(filesize), 2);
									writesys(logfile, ") (", 2);
									writesys(logfile, node->id, 2);
									writesys(logfile, ")", 3);
									writesys(logfile, "#############", 3);
								}
							}
						}
					}
					else
					{
						debug(133, "ERROR Edit Localfile filesize to BIG skipped %lld", filesize);
			
						if(getconfigint("mediadbdebug", NULL) == 1)
						{
							writesys(logfile, "#############", 3); 
							writesys(logfile, "ERROR Edit Localfile filesize to BIG skipped: ", 3); 
							writesys(logfile, picret, 2);
							writesys(logfile, " filesize(", 2);
							writesys(logfile, oitoa(filesize), 2);
							writesys(logfile, ") (", 2);
							writesys(logfile, node->id, 2);
							writesys(logfile, ")", 3);
							writesys(logfile, "#############", 3);
						}
					}
					free(size), size = NULL;
					free(logdir), logdir = NULL;
					free(logfile), logfile = NULL;
					free(cmd), cmd = NULL;
					free(mvi), mvi = NULL;

					unlink("/tmp/mediadb.meta");
					unlink("/tmp/backdrop.resize.jpg");
					unlink("/tmp/backdrop.resize.mpg");

					node = createmediadb(node, tmpstr, type, title->ret, year->ret, released->ret, runtime->ret, genre->ret, director->ret, writer->ret, actors->ret, plot->ret, node->poster, rating->ret, votes->ret, node->path, node->file, node->shortname, node->fileinfo, node->flag, node->backdropcount);
				}
				else
					node = createmediadb(node, tmpstr, type, title->ret, year->ret, released->ret, runtime->ret, genre->ret, director->ret, writer->ret, actors->ret, plot->ret, node->poster, rating->ret, votes->ret, node->path, node->file, node->shortname, node->fileinfo, node->flag, node->backdropcount);

				free(tmpstr); tmpstr = NULL;
				clearscreen(load);
				break;
			}
			if(rcret == getrcconfigint("rcred", NULL))
			{
				free(picret); picret = NULL;
				picret = screendir(getconfig("mediadbpath", NULL), "*.jpg", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, tmcedit->width, tmcedit->prozwidth, tmcedit->height, tmcedit->prozheight, 0);
				if(picret != NULL)
					changeinput(picture, basename(picret));
				drawscreen(tmcedit, 0, 0);
			}
		}

		free(picret); picret = NULL;
		delownerrc(tmcedit);
		clearscreen(tmcedit);
		restorescreen(bg, tmcedit);
		blitfb(0);
	}
}

#endif
