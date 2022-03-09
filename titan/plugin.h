#ifndef PLUGIN_H
#define PLUGIN_H

int delplugin(char *pluginname)
{
	struct skin* plugin = getscreen("plugin");
	struct skin* listbox = getscreennode(plugin, "listbox");
	struct skin* child = getscreennode(plugin, pluginname);
	void (*deinitplugin)(void);

	if(plugin == NULL || child == NULL || listbox == NULL || child->pluginhandle == NULL)
		return 1;
		
	if(plugin == status.skinerr || child == status.skinerr || listbox == status.skinerr)
		return 1;

	deinitplugin = dlsym(child->pluginhandle, "deinit");
	if(deinitplugin != NULL)
		deinitplugin();

	dlclose(child->pluginhandle);
	delscreennode(plugin, pluginname);

	listbox->aktpage = 0;
	listbox->aktline = 0;
	listbox->select = NULL;

	return 0;
}

struct skin* getplugin(char* pluginname)
{
	struct skin* plugin = getscreen("plugin");
	struct skin* pluginnode = getscreennode(plugin, pluginname);

	if(pluginnode != status.skinerr)
		return pluginnode;
	else
		return NULL;
}

int addplugin(char *pluginname, char* plugindesc, char* pluginpic, char* pluginhandle, int hidden)
{
	struct skin* plugin = getscreen("plugin");
	struct skin* listbox = getscreennode(plugin, "listbox");
	struct skin* child = NULL;
	char* tmpstr = NULL;
	char* tmppng = NULL;

	child = checkscreennode(plugin, pluginname);
	if(child != status.skinerr)
	{
		err("plugin %s exists, don't load it", pluginname);
		return 1;
	}
	child = NULL;

	child = addlistbox(plugin, listbox, child, 0);
	if(child != NULL)
	{
		char* defaultdir = NULL;
		defaultdir = ostrcat(getconfig("skinpath", NULL), "/skin/", 0, 0);

		tmppng = ostrcat("", pluginname, 0, 0);
		stringreplacechar(tmppng, ' ', '_');
		stringreplacechar(tmppng, ':', '_');
		string_tolower(tmppng);

		defaultdir = ostrcat(defaultdir, tmppng, 1, 0);
		defaultdir = ostrcat(defaultdir, ".png", 1, 0);
		if(file_exist(defaultdir))
			changepic(child, defaultdir);
		else if(pluginpic != NULL)
			changepic(child, pluginpic);
		else
		{
			tmpstr = ostrcat(getconfig("skinpath", NULL), "/skin/plugin.png", 0, 0);
			changepic(child, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		free(tmppng); tmppng = NULL;
		free(defaultdir), defaultdir = NULL;
		child->valign = MIDDLE;
//		child->height = listbox->fontsize + 30;
		child->hspace = 5;
		child->height = 50;
		if(hidden) child->hidden = YES;
		if(pluginname != NULL)
		{
			changename(child, pluginname);
			changetext(child, _(pluginname));
		}
//		child->textposx = listbox->textposx;
		child->textposx = 120;
		child->pluginhandle = pluginhandle;
		child->del = PLUGINDELMARK;
	}

	return 0;
}

int readplugin(char *dir)
{
	struct dirent **filelist;
	void *pluginhandle = NULL;
	int count = 0, ret = 0;
	char *pluginpath = NULL;
	char *pluginname = NULL;
	char *plugindesc = NULL;
	char *pluginpic = NULL;
	void **pluginmenu = NULL, **pluginmenu1 = NULL;
	int *pluginflag = NULL;
	int *pluginaktiv = NULL;
	int *pluginversion = NULL;
	void (*initplugin)(void);
	int nocheck = 0;

	count = scandir(dir, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir");
		return 1;
	}

	nocheck = getconfigint("nopluginversion", NULL);

	while(count--)
	{
		if(filelist[count]->d_type != DT_DIR && fnmatch("*.so", filelist[count]->d_name, FNM_CASEFOLD) == 0)
		{
			pluginpath = createpath(dir, filelist[count]->d_name);
			pluginhandle = dlopen(pluginpath, RTLD_LOCAL | RTLD_LAZY);
			if(pluginhandle == NULL)
			{
				perr("load plugin %s", pluginpath);
				free(pluginpath); pluginpath = NULL;
				free(filelist[count]);
				continue;
			}
			dlerror();

			//check plugin version
			if(nocheck == 0)
			{
				pluginversion = dlsym(pluginhandle, "pluginversion");
//				if(pluginversion == NULL || (*pluginversion != PLUGINVERSION && *pluginversion != 999999)) //999999 for plugins without titan dependent
// allowed newer plugins as pluginversion
				if(pluginversion == NULL || (*pluginversion < PLUGINVERSION && *pluginversion != 999999)) //999999 for plugins without titan dependent
				{
					if(pluginversion == NULL)
					{
						err("pluginversion not ok titan=%d plugin=NULL (%s)", PLUGINVERSION, pluginpath);
					}
					else
					{
						err("pluginversion not ok titan=%d plugin=%d (%s)", PLUGINVERSION, *pluginversion, pluginpath);
					}
					dlclose(pluginhandle);
					free(pluginpath); pluginpath = NULL;
					free(filelist[count]);
					continue;
				}
			}

			pluginname = dlsym(pluginhandle, "pluginname");
			if(pluginname == NULL)
			{
				err("not a plugin -> pluginname not found: %s", pluginpath);
				dlclose(pluginhandle);
				free(pluginpath); pluginpath = NULL;
				free(filelist[count]);
				continue;
			}
			free(pluginpath); pluginpath = NULL;

			pluginflag = dlsym(pluginhandle, "pluginflag");
			pluginaktiv = dlsym(pluginhandle, "pluginaktiv");
			plugindesc = dlsym(pluginhandle, "plugindesc");
			pluginpic = dlsym(pluginhandle, "pluginpic");
			pluginflag = dlsym(pluginhandle, "pluginflag");

			if(pluginaktiv != NULL && *pluginaktiv == 0)
			{
				if(pluginflag == NULL || *pluginflag == 0)
					ret = addplugin(pluginname, plugindesc, pluginpic, pluginhandle, 0);
				else
					ret = addplugin(pluginname, plugindesc, pluginpic, pluginhandle, 1);
				if(ret == 0)
				{
					initplugin = dlsym(pluginhandle, "init");
					if(initplugin != NULL)
						initplugin();

					pluginmenu = dlsym(pluginhandle, "pluginmenu");
					if(pluginmenu != NULL && *pluginmenu != NULL)
						((struct skin*)*pluginmenu)->pluginhandle = pluginhandle;
					pluginmenu1 = dlsym(pluginhandle, "pluginmenu1");
					if(pluginmenu1 != NULL && *pluginmenu1 != NULL)
						((struct skin*)*pluginmenu1)->pluginhandle = pluginhandle;
				}
				else
					dlclose(pluginhandle);
			}
			else
				dlclose(pluginhandle);
		}
		free(filelist[count]);
	}

	free(filelist);
	return 0;
}

int loadplugin()
{
#if defined(OVBUILD) || defined (OEBUILD)
	struct dirent **filelist;
	struct dirent **filelist2;

	int count = 0;
	int count2 = 0;
	char *pluginpath = NULL;
	char *tmpstr = NULL;
	char *tmppath = NULL;
	char *subdir = NULL;

	//pluginpath
	subdir = getconfig("pluginpath", NULL);

	if(subdir == NULL)
		return 1;

	count = scandir(subdir, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir1");
		return 1;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(subdir, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			tmppath = createpath(subdir, filelist[count]->d_name);

	        count2 = scandir(tmppath, &filelist2, 0, 0);
	        if(count2 < 0)
	        {
		        perr("scandir1a");
		        return 0;
	        }

	        while(count2--)
	        {
		        //check if link is a dir
		        if(filelist2[count2]->d_type == DT_LNK || filelist2[count2]->d_type == DT_UNKNOWN)
		        {

			        tmpstr = createpath(tmppath, filelist2[count2]->d_name);
			        if(isdir(tmpstr) == 1)
				        filelist2[count2]->d_type = DT_DIR;

			        free(tmpstr); tmpstr = NULL;
		        }

		        if(filelist2[count2]->d_type == DT_DIR && ostrcmp(filelist2[count2]->d_name, ".") != 0 && ostrcmp(filelist2[count2]->d_name, "..") != 0)
		        {
			        pluginpath = createpath(tmppath, filelist2[count2]->d_name);
printf("pluginpath: %s\n",pluginpath);
			        readplugin(pluginpath);
			        free(pluginpath); pluginpath = NULL;
		        }
		        free(filelist2[count2]);
	        }
	        free(filelist2);
		}
		free(filelist[count]);
	}
	free(filelist);

	//pluginpath1
	subdir = getconfig("pluginpath1", NULL);

	if(subdir == NULL)
		return 1;

	count = scandir(subdir, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir2");
		return 1;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(subdir, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			tmppath = createpath(subdir, filelist[count]->d_name);

	        count2 = scandir(tmppath, &filelist2, 0, 0);
	        if(count2 < 0)
	        {
		        perr("scandir2a");
		        return 0;
	        }

	        while(count2--)
	        {
		        //check if link is a dir
		        if(filelist2[count2]->d_type == DT_LNK || filelist2[count2]->d_type == DT_UNKNOWN)
		        {

			        tmpstr = createpath(tmppath, filelist2[count2]->d_name);
			        if(isdir(tmpstr) == 1)
				        filelist2[count2]->d_type = DT_DIR;

			        free(tmpstr); tmpstr = NULL;
		        }

		        if(filelist2[count2]->d_type == DT_DIR && ostrcmp(filelist2[count2]->d_name, ".") != 0 && ostrcmp(filelist2[count2]->d_name, "..") != 0)
		        {
			        pluginpath = createpath(tmppath, filelist2[count2]->d_name);
printf("pluginpath1: %s\n",pluginpath);

			        readplugin(pluginpath);
			        free(pluginpath); pluginpath = NULL;
		        }
		        free(filelist2[count2]);
	        }
	        free(filelist2);
		}
		free(filelist[count]);
	}
	free(filelist);

	//pluginpath2
	subdir = getconfig("pluginpath2", NULL);

	if(subdir == NULL)
		return 1;

	count = scandir(subdir, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir3");
		return 1;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(subdir, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			tmppath = createpath(subdir, filelist[count]->d_name);

	        count2 = scandir(tmppath, &filelist2, 0, 0);
	        if(count2 < 0)
	        {
		        perr("scandir3a");
		        return 0;
	        }

	        while(count2--)
	        {
		        //check if link is a dir
		        if(filelist2[count2]->d_type == DT_LNK || filelist2[count2]->d_type == DT_UNKNOWN)
		        {

			        tmpstr = createpath(tmppath, filelist2[count2]->d_name);
			        if(isdir(tmpstr) == 1)
				        filelist2[count2]->d_type = DT_DIR;

			        free(tmpstr); tmpstr = NULL;
		        }

		        if(filelist2[count2]->d_type == DT_DIR && ostrcmp(filelist2[count2]->d_name, ".") != 0 && ostrcmp(filelist2[count2]->d_name, "..") != 0)
		        {
			        pluginpath = createpath(tmppath, filelist2[count2]->d_name);
printf("pluginpath2: %s\n",pluginpath);

			        readplugin(pluginpath);
			        free(pluginpath); pluginpath = NULL;
		        }
		        free(filelist2[count2]);
	        }
	        free(filelist2);
		}
		free(filelist[count]);
	}
	free(filelist);
#else
	struct dirent **filelist;
	int count = 0;
	char *pluginpath = NULL;
	char *tmpstr = NULL;
	char *tmppath = NULL;

	//pluginpath
	tmppath = getconfig("pluginpath", NULL);

	if(tmppath == NULL)
		return 1;

	count = scandir(tmppath, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir");
		return 1;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(tmppath, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			pluginpath = createpath(tmppath, filelist[count]->d_name);
			readplugin(pluginpath);
			free(pluginpath); pluginpath = NULL;
		}
		free(filelist[count]);
	}
	free(filelist);
	
	//pluginpath1
	tmppath = getconfig("pluginpath1", NULL);

	if(tmppath == NULL)
		return 0;

	count = scandir(tmppath, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir1");
		return 0;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(tmppath, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			pluginpath = createpath(tmppath, filelist[count]->d_name);
			readplugin(pluginpath);
			free(pluginpath); pluginpath = NULL;
		}
		free(filelist[count]);
	}
	free(filelist);
	
	//pluginpath2
	tmppath = getconfig("pluginpath2", NULL);

	if(tmppath == NULL)
		return 0;

	count = scandir(tmppath, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir2");
		return 0;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(tmppath, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			pluginpath = createpath(tmppath, filelist[count]->d_name);
			readplugin(pluginpath);
			free(pluginpath); pluginpath = NULL;
		}
		free(filelist[count]);
	}
	free(filelist);	
#endif
	return 0;
}

void screenplugin()
{
	int rcret = 0;
	void (*startplugin)(void);
	struct skin* plugin = getscreen("plugin");
	struct skin* listbox = getscreennode(plugin, "listbox");

	drawscreen(plugin, 0, 0);
	addscreenrc(plugin, listbox);

	while(1)
	{
		rcret = waitrc(plugin, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				startplugin = dlsym(listbox->select->pluginhandle, "start");
				if(startplugin != NULL)
				{
					clearscreen(plugin);
					resettvpic();
					startplugin();
					drawscreen(plugin, 0, 0);
				}
			}
			continue;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL)
			{
				delplugin(listbox->select->name);
				drawscreen(plugin, 0, 0);
			}
			continue;
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			loadplugin();
			drawscreen(plugin, 0, 0);
			continue;
		}
	}

	delownerrc(plugin);
	clearscreen(plugin);
}

#endif
