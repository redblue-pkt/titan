#ifndef OIPKG_H
#define OIPKG_H

struct ipkg
{
	char* name;
	char* desc;
	char* version;
	char* section;
	char* showname;
	int done;
	struct ipkg* prev;
	struct ipkg* next;
};

struct ipkg *ipkg = NULL;

void debugipkg()
{
	struct ipkg *node = ipkg;
	
	while(node != NULL)
	{
		if(node->name != NULL)
			printf("pkg name: %s\n", node->name);
		if(node->desc != NULL)
			printf("pkg desc: %s\n", node->desc);
		if(node->version != NULL)
			printf("pkg version: %s\n", node->version);
		if(node->section != NULL)
			printf("pkg section: %s\n", node->section);
		if(node->showname != NULL)
			printf("pkg showname: %s\n", node->showname);
		node = node->next;
	}
}

struct ipkg* addipkg(char *name, char* desc, char* version, char* section, char* showname, struct ipkg* last)
{
	//debug(1000, "in");
	struct ipkg *newnode = NULL, *prev = NULL, *node = ipkg;

	newnode = (struct ipkg*)malloc(sizeof(struct ipkg));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}
	memset(newnode, 0, sizeof(struct ipkg));

	newnode->name = ostrcat(name, NULL, 0, 0);
	newnode->desc = ostrcat(desc, NULL, 0, 0);
	newnode->version = ostrcat(version, NULL, 0, 0);
	newnode->section = ostrcat(section, NULL, 0, 0);
	newnode->showname = ostrcat(showname, NULL, 0, 0);

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
		ipkg = newnode;
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

void delipkg(struct ipkg* ipkgnode)
{
	//debug(1000, "in");
	struct ipkg *node = ipkg, *prev = ipkg;

	while(node != NULL)
	{
		if(node == ipkgnode)
		{
			if(node == ipkg)
			{
				ipkg = node->next;
				if(ipkg != NULL)
					ipkg->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->name);
			node->name = NULL;

			free(node->desc);
			node->desc = NULL;

			free(node->version);
			node->version = NULL;

			free(node->section);
			node->section = NULL;

			free(node->showname);
			node->showname = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	//debug(1000, "out");
}

void freeipkg()
{
	debug(1000, "in");
	struct ipkg *node = ipkg, *prev = ipkg; 

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delipkg(prev);
	}
	debug(1000, "out");
}

int ipkg_list_cb(char *name, char *desc, char *version, pkg_state_status_t status, void *userdata)
{
	int count = 0;
#ifndef SIMULATE
	char* tmpstr = NULL;
	struct splitstr* ret = NULL;
	
	tmpstr = ostrcat(name, NULL, 0, 0);
	ret = strsplit(tmpstr, "-", &count);

	if(count >= 4 && ostrstr(name, "titan-plugin-") != NULL)
	{
		if(desc)
			addipkg(name, desc, version, (&ret[2])->part, (&ret[3])->part, NULL);
		else
			addipkg(name, NULL, version, (&ret[2])->part, (&ret[3])->part, NULL);
	}

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
#endif

	return 0;
}

int ipkg_status_cb(char *name, int istatus, char *desc, void *userdata)
{
	addipkg(name, desc, NULL, NULL, NULL, NULL);
	return 0;
}

int ipkg_update(void)
{
	unlink("/var/usr/lib/ipkg/cross");
	unlink("/var/usr/lib/ipkg/secret");
	unlink("/var/usr/lib/ipkg/titan");		
	int err = 0;
#ifndef SIMULATE
	args_t args; 

	args_init(&args);
	err = ipkg_lists_update(&args);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_packages_list_installed(args_t *args, const char *packages, ipkg_list_callback cblist, void *userdata)
{
	int err = 0;
#ifndef SIMULATE
	ipkg_cmd_t *cmd;
	ipkg_conf_t ipkg_conf;

	err = ipkg_conf_init(&ipkg_conf, args);
	if(err)
	{
		return err;
	}

	ipkg_cb_list = cblist;
	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find("list_installed");
	if(packages)
		err = ipkg_cmd_exec(cmd, &ipkg_conf, 1, &packages, userdata);
	else
		err = ipkg_cmd_exec(cmd, &ipkg_conf, 0, NULL, userdata);
	ipkg_cb_list = NULL;
	ipkg_conf_deinit(&ipkg_conf);
#endif
	return(err);
}

int ipkg_list_installed(void)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;
	
	args_init(&args);
	err = ipkg_packages_list_installed(&args, NULL, ipkg_list_cb, NULL);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_list(void)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;
	
	args_init(&args);
	err = ipkg_packages_list(&args, NULL, ipkg_list_cb, NULL);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_status(const char* package)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;

	args_init(&args);
	err = ipkg_packages_status(&args, package, ipkg_status_cb, NULL);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_info(const char* package)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;

	args_init(&args);
	err = ipkg_packages_info(&args, package, ipkg_status_cb, NULL);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_install(const char* package)
{
	debug(130, "package: %s", package);

	int err = 0;
#ifndef SIMULATE
	args_t args;
	
	args_init(&args);
	debug(130, "package1: %s", package);
	err = ipkg_packages_install(&args, package);
	debug(130, "package2: %s", package);
	args_deinit(&args);
	debug(130, "package3: %s", package);
#endif	
	return err;
}

int ipkg_remove(const char* package, int purge)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;

	args_init(&args);
	err = ipkg_packages_remove(&args, package, purge);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_upgrade(void)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;

	args_init(&args);
	err = ipkg_packages_upgrade(&args);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_download(ipkg_conf_t *conf, const char *src, const char *filename)
{
	int err = 0, count = 0, i = 0, withoutgui = 0;
	char* tmpip = NULL, *ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL;
	struct splitstr* ret = NULL;

	if(src == NULL) return 1;

	debug(130, "src: %s", src);
	tmpip = string_replace("http://", "", (char*)src, 0);

	if(tmpip != NULL)
		pos = strchr(tmpip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = ostrcat("", path, 0, 0);
	ret = strsplit(tmpstr, "/", &count);
	
	for(i = 0; i < count; i++)
	{
		if(ostrcmp("Packages.gz", (&ret[i])->part) == 0)
			withoutgui = 1;
	}
	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;

	ip = ostrcat(tmpip, NULL, 0, 0);

	debug(130, "ip: %s", ip);
	debug(130, "path: %s", path);
	
	if(withoutgui == 1)
	{
		if(ostrcmp("97.74.32.10", ip) == 0)
		{
			if(ostrcmp((char*)src, "//97.74.32.10/svn/ipk/sh4/titan") != 0)	 	 
			{
				textbox(_("Message"), _("check your Secret Feed !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0); 	 	 
				free(tmpip); tmpip = NULL;
				free(ip); ip = NULL;
				return 1;
			}
		}

		char* checkpath = ostrcat(path, NULL, 0, 0);
		checkpath = string_replace_all("/", ".", checkpath, 1);
		checkpath = string_replace_all("-", ".", checkpath, 1);

		char* checkfile = NULL;
		checkfile = ostrcat("/tmp/Packages.", ip, 0, 0);
		checkfile = ostrcat(checkfile, ".", 1, 0);
		checkfile = ostrcat(checkfile, checkpath, 1, 0);
		free(checkpath), checkpath = NULL;
		debug(130, "checkfile: %s", checkfile);
	
		if(ostrcmp("97.74.32.10", ip) == 0)
		{
			free(ip);
			ip = ostrcat("openaaf.dyndns.tv", NULL, 0, 0);
		}

		if(!file_exist(checkfile)) // +status.ipkg = date + 1day
		{
			char* tmppath = NULL;
			tmppath = ostrcat(tmppath, path, 1, 0);
			tmppath = string_replace("Packages.gz", "Packages.preview.tar.gz", tmppath, 1);
			gethttp(ip, tmppath, 80, "/tmp/Packages.preview.tar.gz", HTTPAUTH, 5000, NULL, 0);
			free(tmppath); tmppath = NULL;
			
			system("tar -zxvf /tmp/Packages.preview.tar.gz -C /tmp");
			unlink("/tmp/Packages.preview.tar.gz");
			writesys(checkfile, ".", 1);
		}

		free(checkfile); checkfile = NULL;		
		gethttp(ip, path, 80, (char*)filename, HTTPAUTH, 5000, NULL, 0);
	}
	else
	{
		if(ostrcmp("97.74.32.10", ip) == 0)
		{
			free(ip);
			ip = ostrcat("openaaf.dyndns.tv", NULL, 0, 0);
		}
		
		err = screendownload("Download", ip, path, 80, (char*)filename, HTTPAUTH, 5000, 0);
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
	}

	free(tmpip); tmpip = NULL;
	free(ip); ip = NULL;

	return err;
}

int ipkg_files(const char* package)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;

	args_init(&args);
	err = ipkg_package_files(&args, package, ipkg_list_cb, NULL);
	args_deinit(&args);
#endif
	return err;
}

int ipkg_search(const char* package)
{
	int err = 0;
#ifndef SIMULATE
	args_t args;

	args_init(&args);
	err = ipkg_file_search(&args, package, ipkg_list_cb, NULL);
	args_deinit(&args);
#endif
	return err;
}

char* get_ipk_section()
{
	struct ipkg *node = ipkg;
	char* sectionlist = NULL;
	char* namelist = NULL;
	char* tmpstr = NULL;
	while(node != NULL)
	{
		if(node->name != NULL && node->section != NULL)
		{
			tmpstr = ostrcat("-", node->section, 0, 0);
			tmpstr = ostrcat(tmpstr, "-", 1, 0);
			if(ostrstr(namelist, tmpstr) == NULL)
			{
				namelist = ostrcat(namelist, node->name, 1, 0);
				namelist = ostrcat(namelist, " ", 1, 0);
				sectionlist = ostrcat(sectionlist, node->section, 1, 0);
				sectionlist = ostrcat(sectionlist, "\n", 1, 0);
			}
			free(tmpstr), tmpstr = NULL;
		}
		node = node->next;
	}
	free(namelist), namelist = NULL;
	return sectionlist;
}
/*
use from tpk.h
int findsectiondone(char* section)
{
	struct ipkg* node = ipkg;
	
	if(node == NULL || section == NULL) return 1;
	
	while(node != NULL)
	{
		if(node->done == 1 && ostrcmp(section, node->section) == 0)
			return 1;
		node = node->next;
	}

	return 0;
}
*/
//flag 0: show section
//flag 1: show entrys
//flag 2: show entrys for remove
struct menulist* ipkmenulist(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* section, int showpng, char* defentry, int flag)
{
	int skip = 0;
	struct ipkg* node = ipkg, *ipkg_installed = NULL, *node_installed = NULL;
	struct menulist* tmpmlist = NULL;
	char* tmpstr = NULL, *tmpinfo = NULL, *tmppic = NULL;
	
	if(node == NULL) return NULL;
	
	if(flag == 1)
	{
		ipkg = NULL;
		ipkg_list_installed();
		ipkg_installed = ipkg;
		ipkg = node;
	}
	
	while(node != NULL)
	{
		if(flag == 0 || flag == 2)
		{
			if(flag == 0)
			{
				//check if section have seen
				if(findsectiondone(node->section) == 1)
				{
					node = node->next;
					continue;
				}
			}
		
			tmppic = ostrcat(node->section, ".png", 0, 0);
		
			if(flag == 0)
			{
				node->done = 1;
				addmenulist(&mlist, node->section, NULL, tmppic, 0, 0);
			}
			
			if(flag == 2)
			{
				tmpstr = ostrcat(tmpstr, node->section, 1, 0);
				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, node->showname, 1, 0);
				addmenulist(&mlist, tmpstr, NULL, tmppic, 0, 0);
				free(tmpstr); tmpstr = NULL;
			}
			
			free(tmppic); tmppic = NULL;
		}
		
		if(flag == 1)
		{
			//check if ipkg is installed
			node_installed = ipkg_installed;
			skip = 0;
			while(node_installed != NULL)
			{
				if(ostrcmp(node->section, node_installed->section) == 0 && ostrcmp(node->showname, node_installed->showname) == 0)
				{
					skip = 1;
					break;
				}
				node_installed = node_installed->next;
			}
			
			//check if ipkg is in section
			if(section != NULL && ostrcmp(node->section, section) != 0)
				skip = 1;

			if(skip == 1)
			{
				node = node->next;
				continue;
			}
			
			tmpstr = ostrcat(tmpstr, node->showname, 1, 0);
			tmpstr = ostrcat(tmpstr, " v.", 1, 0);
			tmpstr = ostrcat(tmpstr, node->version, 1, 0);

			tmpinfo = ostrcat(tmpinfo, "\nSection: ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, node->section, 1, 0);
			tmpinfo = ostrcat(tmpinfo, "\nDescription:\n", 1, 0);
			if(node->desc != NULL)
				tmpinfo = ostrcat(tmpinfo, node->desc, 1, 0);
			else
				tmpinfo = ostrcat(tmpinfo, _("no description found"), 1, 0);
			
			tmppic = ostrcat(tmppic, node->showname, 1, 0);
			if(tmppic != NULL)
			{
				char* pos = strchr(tmppic, '.');
				if(pos != NULL) pos[0] = '\0';
				tmppic = ostrcat("titan-pluginpreview-", tmppic, 0, 1);
				tmppic = ostrcat(tmppic, ".png", 1, 0);
			}

			tmpmlist = addmenulist(&mlist, tmpstr, tmpinfo, tmppic, 0, 0);
//			changemenulistparam(tmpmlist, node->name, node->titanname, NULL, NULL);
			changemenulistparam(tmpmlist, node->name, node->name, NULL, NULL);

			free(tmpstr); tmpstr = NULL;
			free(tmpinfo); tmpinfo = NULL;
			free(tmppic); tmppic = NULL;
		}
		
		node = node->next;
	}
	
	if(flag == 1)
	{
		node = ipkg;
		ipkg = ipkg_installed;
		freeipkg();
		ipkg = node;
	}

    setmenulistdefault(mlist, defentry);
	return menulistbox(mlist, paramskinname, skintitle, NULL, paramskinpath, "/skin/plugin.png", showpng, 0);
}

char* get_ipk_tmpinstall(char* path, char* ipk)
{
	debug(130, "in");

	unlink("/var/usr/lib/ipkg/cross");
	unlink("/var/usr/lib/ipkg/secret");
	unlink("/var/usr/lib/ipkg/titan");	

	char* cmd = NULL, *tmpstr = NULL;
	cmd = ostrcat(cmd, "ipkg install ", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(130, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_tmplistinstall(char* path)
{
	debug(130, "in");
	char* cmd = NULL, *tmpstr = NULL;

	cmd = ostrcat(cmd, "ls ", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	cmd = ostrcat(cmd, " | grep '\\.ipk'", 1, 0);

	tmpstr = command(cmd);

	debug(130, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

#endif

