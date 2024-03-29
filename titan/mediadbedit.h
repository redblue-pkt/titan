#ifndef MEDIADBEDIT_H
#define MEDIADBEDIT_H

//flag 0: don't save/restore background
//flag 1: save/restore background
void screenmediadbedit(char* file, int id, int flag)
{
	int rcret = 0, type = 0;
	struct skin* mediadbedit = getscreen("mediadbedit");
	struct skin* listbox = getscreennode(mediadbedit, "listbox");
	struct skin* title = getscreennode(mediadbedit, "title");
	struct skin* year = getscreennode(mediadbedit, "year");
	struct skin* released = getscreennode(mediadbedit, "released");
	struct skin* runtime = getscreennode(mediadbedit, "runtime");
	struct skin* genre = getscreennode(mediadbedit, "genre");
	struct skin* director = getscreennode(mediadbedit, "director");
	struct skin* writer = getscreennode(mediadbedit, "writer");
	struct skin* actors = getscreennode(mediadbedit, "actors");
	struct skin* plot = getscreennode(mediadbedit, "plot");
	struct skin* rating = getscreennode(mediadbedit, "rating");
	struct skin* votes = getscreennode(mediadbedit, "votes");
	struct skin* locked = getscreennode(mediadbedit, "locked");
	struct skin* picture = getscreennode(mediadbedit, "picture");
	struct skin* shortname = getscreennode(mediadbedit, "shortname");
	struct skin* fileinfo = getscreennode(mediadbedit, "fileinfo");
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

	if(id == 3) type = 0; //video
	if(id == 4) type = 1; //audio
	if(id == 2) type = 2; //picture

	if(node != NULL)
	{
		changeinput(picture, NULL);
		changeinput(title, node->title);

		changemask(year, "0000");
		tmpstr = oitoa(node->year);
		changeinput(year, tmpstr);
		free(tmpstr); tmpstr = NULL;
		year->input = mask(year->input, 4, "0");

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
		votes->input = mask(votes->input, 7, "0");

		addchoicebox(locked, "0", _("unlock -> lock it"));
		addchoicebox(locked, "1", _("unlock -> leaf unlock"));
		addchoicebox(locked, "2", _("locked -> unlock it"));
		addchoicebox(locked, "3", _("locked -> leaf locked"));

		if(checkbit(node->flag, 31) == 1)
			setchoiceboxselection(locked, "3");
		else
			setchoiceboxselection(locked, "0");

		if(flag == 1)
		{
			drawscreen(mediadbedit, 2, 0);
			bg = savescreen(mediadbedit);
		}

		addscreenrc(mediadbedit, listbox);
		drawscreen(mediadbedit, 0, 0);

		tmp = listbox->select;
		while(1)
		{
			addscreenrc(mediadbedit, tmp);
			rcret = waitrcext(mediadbedit, 0, 0, 1000);
			delownerrc(mediadbedit);
			addscreenrc(mediadbedit, listbox);
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

					char* size = NULL, *logdir = NULL, *logfile = NULL, *cmd = NULL, *mvi = NULL, *tmpjpg = NULL, *tmpmpg = NULL, *tmpmeta = NULL, *timen = NULL;
					mvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					mvi = ostrcat(mvi, node->id, 1, 0);
					mvi = ostrcat(mvi, "_backdrop1.mvi", 1, 0);
					
					logdir = ostrcat(getconfig("mediadbpath", NULL), "/.mediadbdebug", 0, 0);
					if(!file_exist(logdir))
						mkdir(logdir, 0777);
					logfile = ostrcat(logdir, "/imdb-scan.log", 0, 0);

					timen = ostrcat(oitoa(time(NULL)), NULL, 1, 0);
					
					tmpjpg = ostrcat("/tmp/backdrop.resize.", timen, 0, 0);
					tmpjpg = ostrcat(tmpjpg, ".jpg", 1, 0);
					
					tmpmpg = ostrcat("/tmp/backdrop.resize.", timen, 0, 0);
					tmpmpg = ostrcat(tmpmpg, ".mpg", 1, 0);
					
					tmpmeta = ostrcat("/tmp/mediadb.", timen, 0, 0);
					tmpmeta = ostrcat(tmpmeta, ".meta", 1, 0);
				
					off64_t filesize = getfilesize(picret);
					debug(133, "filesize %lld", filesize);
								
					if(filesize < 1500000)
					{
						cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
						cmd = ostrcat(cmd, picret, 1, 0);
						cmd = ostrcat(cmd, " > ", 1, 0);
						cmd = ostrcat(cmd, tmpmeta, 1, 0);
						cmd = ostrcat(cmd, " 2>&1", 1, 0);
	
						debug(133, "cmd %s", cmd);
						system(cmd);
						free(cmd); cmd = NULL;
	
						cmd = ostrcat(cmd, "cat ", 1, 0);
						cmd = ostrcat(cmd, tmpmeta, 1, 0);
						cmd = ostrcat(cmd, " | grep Stream | awk '{print $6}' | cut -d'x' -f1", 1, 0);
						char* size = string_newline(command(cmd));
						free(cmd); cmd = NULL;
						debug(133, "size %s", size);
						if(size != NULL)
						{
							debug(133, "size %d", atoi(size));
							int picsize = atoi(size);
		
							if(picsize < 2000)
							{
								debug(133, "size ok %d", picsize);
												
								cmd = ostrcat(cmd, "jpegtran -outfile ", 1, 0);
								cmd = ostrcat(cmd, tmpjpg, 1, 0);
								cmd = ostrcat(cmd, " -copy none ", 1, 0);
								cmd = ostrcat(cmd, picret, 1, 0);
				
								debug(133, "cmd %s", cmd);
								system(cmd);
								free(cmd); cmd = NULL;
			
								if(file_exist(tmpjpg))
								{
									free(cmd), cmd = NULL;
									cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i ", 1, 0);
									cmd = ostrcat(cmd, tmpjpg, 1, 0);
									cmd = ostrcat(cmd, " ", 1, 0);
									cmd = ostrcat(cmd, tmpmpg, 1, 0);

									if(getconfigint("mediadbdebug", NULL) == 1)
									{
										filedebug(logfile, "#############\nEdit Localfile: %s size=(%s) filesize(%lld) (%s)\n#############", picret, size, filesize, node->id);
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
									filedebug(logfile, "#############\nERROR Localfile size to big skipped: %s size=(%s) filesize(%lld) (%s)\n#############", picret, size, filesize, node->id);
							}
						}
					}
					else
					{
						debug(133, "ERROR Edit Localfile filesize to BIG skipped %lld", filesize);
			
						if(getconfigint("mediadbdebug", NULL) == 1)
							filedebug(logfile, "#############\nERROR Edit Localfile filesize to BIG skipped: %s filesize(%lld) (%s)\n#############", picret, filesize, node->id);
					}
					free(timen), timen = NULL;
					free(tmpjpg), tmpjpg = NULL;
					free(tmpmpg), tmpmpg = NULL;
					free(tmpmeta), tmpmeta = NULL;
					free(size), size = NULL;
					free(logdir), logdir = NULL;
					free(logfile), logfile = NULL;
					free(cmd), cmd = NULL;
					free(mvi), mvi = NULL;

					unlink(tmpmeta);
					unlink(tmpjpg);
					unlink(tmpmpg);
/*
					cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
					cmd = ostrcat(cmd, "/", 1, 0);	
					cmd = ostrcat(cmd, timestamp, 1, 0);
					cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
					unlink(cmd);
					free(cmd); cmd = NULL;
					free(timestamp); timestamp = NULL;
*/
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
				picret = screendir(getconfig("mediadbpath", NULL), "*.jpg", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, mediadbedit->width, mediadbedit->prozwidth, mediadbedit->height, mediadbedit->prozheight, 0);
				if(picret != NULL)
					changeinput(picture, basename(picret));
				drawscreen(mediadbedit, 0, 0);
			}
		}

		free(picret); picret = NULL;
		delownerrc(mediadbedit);
		clearscreen(mediadbedit);

		if(flag == 1)
		{
			restorescreen(bg, mediadbedit);
			blitfb(0);
		}
	}
	else
		textbox(_("Message"), _("MediaDB entry for this file not found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
}

#endif
