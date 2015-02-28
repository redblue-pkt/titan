#ifndef MC_IMDB_H
#define MC_IMDB_H

char* mc_imdb_sync(int mode, char* filepath, char* filename)
{
	debug(50, "[mc_imdb_sync] start");

	int mc_imdb_background = getconfigint("imdb_background", NULL);
	char* mc_imdb_replace = getconfig("imdb_replace", NULL);
	char* mc_imdb_directory = getconfig("imdb_directory", NULL);
	char* mc_imdb_lang = getconfig("imdb_lang", NULL);

	debug(50, "[mc_imdb_sync] mc_imdb_background: %d", mc_imdb_background);
	debug(50, "[mc_imdb_sync] mc_imdb_replace: %s", mc_imdb_replace);
	debug(50, "[mc_imdb_sync] mc_imdb_lang: %s", mc_imdb_lang);
	debug(50, "[mc_imdb_sync] mc_imdb_directory: %s", mc_imdb_directory);

	char* cmd = NULL;

	if (mode == 0){
		cmd = ostrcat(cmd, "echo 'touch /tmp/.imdbrunning ; sleep 60 ; /usr/bin/python /sbin/sync.pyc ", 0, 0);
	} else {
		cmd = ostrcat(cmd, "echo 'touch /tmp/.imdbrunning ; sleep 5 ; /usr/bin/python /sbin/sync.pyc ", 0, 0);
	}

	cmd = ostrcat(cmd, filepath, 0, 0);
	cmd = ostrcat(cmd, " ", 0, 0);
	cmd = ostrcat(cmd, mc_imdb_lang, 0, 1);
	cmd = ostrcat(cmd, " ", 0, 0);
	cmd = ostrcat(cmd, mc_imdb_directory, 0, 0);
	cmd = ostrcat(cmd, " ", 0, 0);
	cmd = ostrcat(cmd, mc_imdb_replace, 0, 1);
	cmd = ostrcat(cmd, " ", 0, 0);
	cmd = ostrcat(cmd, mc_imdb_directory, 1, 1);
	cmd = ostrcat(cmd, " &' > /tmp/imdb.run", 1, 0);

	if (mc_imdb_background == 1){
		if (isfile("/tmp/.imdbrunning")){
			debug(50, "[mc_imdb_sync] skip is running");
		} else {
			unlink("/tmp/imdb.run");
			debug(50, "[mc_imdb_sync] cmd: %s", cmd);
			system(cmd);
			debug(50, "[mc_imdb_sync] cmd: /tmp/imdb.run &");
			chmod("/tmp/imdb.run", 0755);
			system("/tmp/imdb.run &");
		}
	} else {
		debug(50, "[mc_imdb_sync] skip not enabled");
	}
	free(cmd);
	cmd = NULL;
	debug(50, "[mc_imdb_sync] start");
	return 0;
}

//char* screenmc_imdb(int style, struct skin* filelistpath)
char* screenmc_imdb(int style, char* filepath, char* filename)
{
	int rcret = 0;
	char* skinname = NULL;

	if (style == 2){
		skinname = "mc_imdb_mem";
	} else {
		skinname = "mc_imdb";
	}

	struct skin* mc_imdb = getscreen(skinname);
	struct skin* listbox = getscreennode(mc_imdb, "listbox");
	struct skin* file = getscreennode(mc_imdb, "file");
	struct skin* filealt = getscreennode(mc_imdb, "filealt");
	struct skin* filealtresult = getscreennode(mc_imdb, "filealtresult");
	struct skin* mvid = getscreennode(mc_imdb, "mvid");
	struct skin* all = getscreennode(mc_imdb, "all");

	struct skin* tmp = NULL;
	addchoicebox(file, "0", (""));
	addchoicebox(filealt, "1", (""));
	addchoicebox(filealtresult, "2", (""));
	addchoicebox(mvid, "3", (""));
	addchoicebox(all, "4", (""));

	drawscreen(mc_imdb, 0, 0);
	addscreenrc(mc_imdb, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_imdb, tmp);
		rcret = waitrc(mc_imdb, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(!strncmp("file", listbox->select->name, 4)){
//				tmpsort = 0;
			} else if(!strncmp("filealt", listbox->select->name, 7)){
//				tmpsort = 1;
			} else if(!strncmp("filealtresult", listbox->select->name, 13)){
//				tmpsort = 2;
			} else if(!strncmp("mvid", listbox->select->name, 4)){
//				tmpsort = 3;
			} else if(!strncmp("all", listbox->select->name, 3)){
				debug(50, "[mc_imdb] start mc_imdb_sync");
				mc_imdb_sync(1, filepath, filename);
//				mc_imdb_sync(1,filelistpath->text);
			}
			break;
		}
	}

	delownerrc(mc_imdb);
	clearscreen(mc_imdb);
	return 0;
}

#endif
