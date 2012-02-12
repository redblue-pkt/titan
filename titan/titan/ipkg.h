#ifndef IPKG_H
#define IPKG_H

#define IPKG_LIB
#include "../libipkg/libipkg.h"
#include "../libipkg/args.h"
#include "../libipkg/ipkg_cmd.h"

struct ipkg
{
	char* name;
	char* desc;
	char* version;
	char* section;
	char* showname;
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

	if(count >= 4)
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

	err = ipkg_conf_init (&ipkg_conf, args);
	if(err)
	{
		return err;
	}

	ipkg_cb_list = cblist;
	/* we need to do this because of static declarations, 
	 * maybe a good idea to change */
	cmd = ipkg_cmd_find ("list_installed");
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
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL;
	struct splitstr* ret = NULL;

	if(src == NULL) return 1;

	debug(130, "src: %s", src);
	ip = string_replace("http://", "", (char*)src, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
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

	debug(130, "src: %s", src);
	debug(130, "ip: %s", ip);
	debug(130, "path: %s", path);
	
	if(withoutgui == 1)
	{
		if(ostrcmp("97.74.32.10", ip) == 0)
		{
			if(ostrcmp((char*)src, "//97.74.32.10/svn/ipk/sh4/titan") != 0) 	 	 
			{
				textbox(_("Message"), _("check your Secret Feed !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0); 	 	 
				free(ip); ip = NULL;
				//free(path); //path = NULL;
				return 1;
			}
		}

		char* checkfile = NULL;
		checkfile = ostrcat("/tmp/Packages.", ip, 0, 0);

		if(!file_exist(checkfile)) // +status.ipkg = date + 1day
		{
			char* tmppath = NULL;
			tmppath = ostrcat(tmppath, path, 1, 0);
			tmppath = string_replace("Packages.gz", "Packages.preview.tar.gz", tmppath, 1);
			gethttp(ip, tmppath, 80, "/tmp/Packages.preview.tar.gz", "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", NULL, 0);
			free(tmppath); tmppath = NULL;
			
			system("tar -zxvf /tmp/Packages.preview.tar.gz -C /tmp");
			unlink("/tmp/Packages.preview.tar.gz");
			writesys(checkfile, ".", 1);
		}
		free(checkfile); checkfile = NULL;		
		gethttp(ip, path, 80, (char*)filename, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", NULL, 0);
	}
	else
		err = screendownload("Download", ip, path, 80, (char*)filename, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 0);
	free(ip); ip = NULL;
	//free(path);// path = NULL; segfault
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
			if(!string_find(tmpstr,namelist))
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

char* ipk_listbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng)
{
	debug(1000, "in");
	char* skinname1 = NULL;
	char* skinpath1 = NULL;
	char* tmpskinpath = NULL;
	int rcret = 0;

	if(str == NULL)
		return 0;

	if(skinname == NULL)
	{
		debug(130, "skinname default=menulist");
		skinname1 = ostrcat(skinname, "menulist", 1, 0);
	}
	else
	{
		debug(130, "skinname changed=%s", skinname);
		skinname1 = ostrcat(skinname, "", 0, 0);
	}

	if(skinpath == NULL)
	{
		debug(130, "skinpath default=skin/");
		skinpath1 = ostrcat(skinpath, "skin/", 1, 0);
	}
	else
	{
		debug(130, "skinpath changed=%s", skinpath);
		skinpath1 = ostrcat(skinpath, "", 0, 0);
	}

	struct skin* screen = getscreen(skinname1);
	struct skin* listbox = getscreennode(screen, "listbox");
	
	listbox->aktpage = -1;
	listbox->aktline = 1;

	if(skintitle != NULL)
	{
		debug(130, "skintitle changed=%s", skintitle);
		changetitle(screen, _(skintitle));
	}

	struct skin* tmp = NULL;
	char* tmpstr = NULL;
	char* defaultdir = NULL;
	char* tmpinfo = NULL;
	char* tmptitle = NULL;
	char* showname = NULL;
//	char* tmpstr1 = NULL;
//	char* tmpck = NULL;
	char* tmpsplit = NULL;
	char* tmpck = NULL;
	char* tmpstr1 = NULL;
	char* tmpstr4 = NULL;
	char* tmpstr5 = NULL;
	if(showpng == 1)
	{
		struct splitstr* ret1 = NULL;
		struct splitstr* ret2 = NULL;
		struct splitstr* ret3 = NULL;
		int count = 0;
		int i = 0;
		ret1 = strsplit(str, "\n", &count);
	
		for( i = 0; i < count; i++)
		{
			int count2 = 0;
			tmpsplit = ostrcat(tmpsplit, (&ret1[i])->part, 1, 0);
			free(ret2); ret2 = NULL;
			ret2 = strsplit(tmpsplit, "-", &count2);
	
			tmp = addlistbox(screen, listbox, tmp, 1);
			
			if(tmp != NULL)
			{
				changetext(tmp, _((&ret1[i])->part));
				changename(tmp, (&ret1[i])->part);
	
				tmp->del = 1;
				tmp->textposx = 120;
				tmp->height = 50;
//				tmp->fontsize = 30;
				tmp->valign = convertxmlentry("middle", 0);
				tmp->hspace = 5;
				debug(130, "showpng changed=%d", showpng);
	
				if(string_find("%pluginpath%",skinpath1))
				{
					struct splitstr* ret6 = NULL;
					int count6 = 0;
					char* tmpstr6 = NULL;
					tmpstr6 = ostrcat("", skinpath1, 0, 0);
					ret6 = strsplit(tmpstr6, "%", &count6);
					defaultdir = ostrcat(getconfig("skinpath", NULL), "/skin/panel_", 0, 0);
					defaultdir = ostrcat(defaultdir, (&ret1[i])->part, 1, 0);
					defaultdir = ostrcat(defaultdir, ".png", 1, 0);

					if(!file_exist(defaultdir))
					{
						defaultdir = ostrcat(getconfig("pluginpath", NULL), (&ret6[1])->part, 0, 0);
						defaultdir = ostrcat(defaultdir, "panel_", 0, 0);
						defaultdir = ostrcat(defaultdir, (&ret1[i])->part, 1, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);
					}
					free(ret6), ret6 = NULL;
				}
				else
				{
					defaultdir = ostrcat(getconfig("skinpath", NULL), skinpath1, 0, 0);
					defaultdir = ostrcat(defaultdir, "/skin/panel_", 0, 0);
					defaultdir = ostrcat(defaultdir, (&ret1[i])->part, 1, 0);
					defaultdir = ostrcat(defaultdir, ".png", 1, 0);
				}

				debug(130, "defaultdir %s", defaultdir);
				if(file_exist(defaultdir))
				{
					tmpskinpath = ostrcat("", defaultdir, 0, 0);
					changepic(tmp, tmpskinpath);
					free(tmpskinpath); tmpskinpath = NULL;
				}
				else
				{
					tmpskinpath = ostrcat(skinpath1, "panel_default.png", 0, 0);
					changepic(tmp, tmpskinpath);
					free(tmpskinpath); tmpskinpath = NULL;
				}
				free(defaultdir); defaultdir = NULL;
	
				if(defaultstr != NULL)
					setlistboxselection(listbox, defaultstr);
			}
			free(ret2),ret2 = NULL;
		}
		free(ret1),ret1 = NULL;
	}
	else if(showpng == 2)
	{
		int i = 0;
		tmpck = get_ipk_listinstall();
		
		debug(130, "tmpck: %s", tmpck);
		debug(130, "tmpck list_installed ret: %d", ipkg_list_installed());

		struct ipkg *node = ipkg;
		while(node != NULL)
		{
			debug(130, "name: %s", node->name);
			debug(130, "section: %s", node->section);
			debug(130, "showname: %s", node->showname);
			debug(130, "str: %s", str);
									
			if(node->section != NULL && ostrcmp(str, node->section) == 0)
			{
				struct splitstr* ret1 = NULL;
				int count1 = 0;			
				tmpstr1 = ostrcat("", node->showname, 0, 0);
				ret1 = strsplit(tmpstr1, ".", &count1);
				int max = count1;
				i += 1;
				showname = ostrcat("", (&ret1[0])->part, 0, 0);
				free(ret1),ret1 = NULL;
//				showname = ostrcat(node->showname, NULL, 0, 0);

				struct splitstr* ret3 = NULL;
				int count3 = 0;
				int a = 0;
				tmpstr1 = ostrcat("", tmpck, 0, 0);
	
				debug(130, "tmpck: (%d) %s", i, tmpstr1);
	
				ret3 = strsplit(tmpstr1, "\n", &count3);
				int skip = 0;

				debug(130, "check list_installed");
				for(a = 0; a < count3; a++)
				{
					debug(130, "node->name=%s", node->name);
					debug(130, "(&ret3[a])->part=%s", (&ret3[a])->part);

					char* installed = NULL;
					installed = ostrcat(installed, node->section, 1, 0);
					installed = ostrcat(installed, "-", 1, 0);
					installed = ostrcat(installed, node->showname, 1, 0);

					if(ostrcmp((&ret3[a])->part, installed) == 0)
					{
						debug(130, "set skiped=1");
						skip = 1;
						break;
					}
					free(installed),installed = NULL;
				}
				free(ret3),ret3 = NULL;
				debug(130, "check list_installed done");				
				if(skip == 0)
				{
					tmp = addlistbox(screen, listbox, tmp, 1);
					tmp->del = 1;
					tmp->valign = convertxmlentry("middle", 0);
					tmp->textposx = 250;
					tmp->height = 170;
	 				tmp->textposx2 = 270;
					tmp->type=TEXTBOX;
					tmp->wrap=YES;
					tmp->hspace = 5;
					debug(130, "showpng changed=%d", showpng);
	
					defaultdir = ostrcat("", skinpath1, 0, 0);
					defaultdir = ostrcat(defaultdir, "titan-pluginpreview-", 1, 0);
					defaultdir = ostrcat(defaultdir, showname, 1, 0);
					defaultdir = ostrcat(defaultdir, ".png", 1, 0);
	
					debug(130, "defaultdir %s", defaultdir);
	
					if(file_exist(defaultdir))
					{
						debug(130, "defaultdir found");
						tmpskinpath = ostrcat(skinpath1, "titan-pluginpreview-", 0, 0);
						tmpskinpath = ostrcat(tmpskinpath, showname, 1, 0);
						tmpskinpath = ostrcat(tmpskinpath, ".png", 1, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}	
					else
					{
						debug(130, "defaultdir not found use default.png");
						tmpskinpath = ostrcat("", "skin/plugin.png", 0, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}
					free(defaultdir); defaultdir = NULL;
	
					if(defaultstr != NULL)
					{
						setlistboxselection(listbox, defaultstr);
					//	if(ostrcmp(defaultstr, (&ret1[i])->part) == 0)
					//		listbox->aktline = i + 1;
					}
					tmptitle = ostrcat(tmptitle, node->showname, 1, 0);
					tmptitle = ostrcat(tmptitle, " v.", 1, 0);
					tmptitle = ostrcat(tmptitle, node->version, 1, 0);
					changetext(tmp, _(tmptitle));
					changename(tmp, tmptitle);
	
					tmpinfo = ostrcat(tmpinfo, "\nSection: ", 1, 0);
					tmpinfo = ostrcat(tmpinfo, node->section, 1, 0);
					tmpinfo = ostrcat(tmpinfo, "\nDescription:\n", 1, 0);
					if(node->desc != NULL)
						tmpinfo = ostrcat(tmpinfo, node->desc, 1, 0);
					else
						tmpinfo = ostrcat(tmpinfo, _("no description found"), 1, 0);				
					changetext2(tmp, _(tmpinfo));
	
					free(tmpinfo); tmpinfo = NULL;
					free(tmptitle); tmptitle = NULL;
					free(tmpstr1);tmpstr1 = NULL;
					free(showname);showname = NULL;	
				}
				free(ret1),ret1 = NULL;
			}			
			node = node->next;
		}
	}
	listbox->aktpage = -1;
	drawscreen(screen,0);
	addscreenrc(screen, listbox);

	while (1)
	{
		rcret = waitrc(screen, 0, 0);

		if(rcret==getrcconfigint("rcexit",NULL)) break;
		if(listbox->select != NULL && rcret==getrcconfigint("rcok",NULL))
		{
			tmpstr = ostrcat(tmpstr, listbox->select->name, 1, 0);
			break;
		}
	}
	free(skinname1); skinname1 = NULL;
	free(skinpath1); skinpath1 = NULL;
	delownerrc(screen);
	delmarkedscreennodes(screen, 1);
	clearscreen(screen);
	debug(1000, "out");
	return tmpstr;

}

char* get_ipk_tmpinstall(char* ipk)
{
	debug(130, "in");

	unlink("/var/usr/lib/ipkg/cross");
	unlink("/var/usr/lib/ipkg/secret");
	unlink("/var/usr/lib/ipkg/titan");	

	char* cmd = NULL, *tmpstr = NULL;
	cmd = ostrcat(cmd, "ipkg install /tmp/", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(130, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_listinstall()
{
	debug(130, "in");
	char* cmd = NULL, *tmpstr = NULL;

	cmd = ostrcat(cmd, "ipkg list_installed | awk '{ print $1 }' | sed 's/Successfully//' | sed 's/titan-plugin-//'", 1, 0);

	tmpstr = command(cmd);

	debug(130, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_tmplistinstall()
{
	debug(130, "in");
	char* cmd = NULL, *tmpstr = NULL;

	cmd = ostrcat(cmd, "ls /tmp | grep '.ipk'", 1, 0);

	tmpstr = command(cmd);

	debug(130, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

#endif

