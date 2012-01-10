#ifndef IPKG_H
#define IPKG_H

#define IPKG_LIB
#include "../libipkg/libipkg.h"
#include "../libipkg/args.h"

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
	char* tmpstr = NULL;
	struct splitstr* ret = NULL;
	
	tmpstr = ostrcat(name, NULL, 0, 0);
	ret = strsplit(tmpstr, "-", &count);

	if(desc)
		addipkg(name, desc, version, (&ret[2])->part, (&ret[3])->part, NULL);
	else
		addipkg(name, NULL, version, (&ret[2])->part, (&ret[3])->part, NULL);

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;

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
	args_t args; 

	args_init(&args);
	err = ipkg_lists_update(&args);
	args_deinit(&args);

	return err;
}

int ipkg_list(void)
{
	int err = 0;
	args_t args;
	
	args_init(&args);
	err = ipkg_packages_list(&args, NULL, ipkg_list_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_status(const char* package)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_packages_status(&args, package, ipkg_status_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_info(const char* package)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_packages_info(&args, package, ipkg_status_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_install(const char* package)
{
	printf("package: %s\n",package);

	int err = 0;
	args_t args;
	args_init(&args);

printf("4444444\n");
	err = ipkg_packages_install(&args, package);
printf("5555555\n");
	args_deinit(&args);
printf("6666666\n");
	return err;
}

int ipkg_remove(const char* package, int purge)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_packages_remove(&args, package, purge);
	args_deinit(&args);

	return err;
}

int ipkg_upgrade(void)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_packages_upgrade(&args);
	args_deinit(&args);

	return err;
}

int ipkg_download(ipkg_conf_t *conf, const char *src, const char *filename)
{
	int err = 0, count = 0, i = 0, withoutgui = 0;
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL;
	struct splitstr* ret = NULL;

	if(src == NULL) return 1;

	printf("src = %s\n", src);
	ip = string_replace("http://", "", src, 0);

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

	printf("src = %s\n", src);
	printf("ip = %s\n", ip);
	printf("path = %s\n", path);	
	
	if(withoutgui == 1)
	{
		if(ostrcmp("97.74.32.10", ip) == 0)
		{
			if(ostrcmp(src, "//97.74.32.10/svn/ipk/sh4/titan") != 0) 	 	 
			{
				textbox(_("Message"), _("check your Secret Feed !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0); 	 	 
				free(ip); ip = NULL;
				free(path); path = NULL;
				return;
			}
		}

		char* checkfile = NULL;
		checkfile = ostrcat("/tmp/Packages.", ip, 0, 0);

		if(!file_exist(checkfile)) // +status.ipkg = date + 1day
		{
			char* tmppath = NULL;
			tmppath = ostrcat(tmppath, path, 1, 0);
			tmppath = string_replace("Packages.gz", "Packages.preview.tar.gz", tmppath, 1);
			gethttp(ip, tmppath, 80, "/tmp/Packages.preview.tar.gz", "YXRlbWlvOkZIWlZCR2huemZ2RWhGREZUR3p1aWY1Njc2emhqR1RVR0JOSGpt", NULL);
			free(tmppath); tmppath = NULL;
			
			system("tar -zxvf /tmp/Packages.preview.tar.gz -C /tmp");
			unlink("/tmp/Packages.preview.tar.gz");
			writesys(checkfile, ".", 1);
		}
		free(checkfile); checkfile = NULL;		
		err = gethttp(ip, path, 80, (char*)filename, "YXRlbWlvOkZIWlZCR2huemZ2RWhGREZUR3p1aWY1Njc2emhqR1RVR0JOSGpt", NULL);
	}
	else
		err = screendownload("Download", ip, path, 80, (char*)filename, "YXRlbWlvOkZIWlZCR2huemZ2RWhGREZUR3p1aWY1Njc2emhqR1RVR0JOSGpt", 0);
	printf("111\n");
	free(ip); ip = NULL;
	printf("222\n");
	free(path); path = NULL;
	printf("333\n");
	return err;
}

int ipkg_files(const char* package)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_package_files(&args, package, ipkg_list_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_search(const char* package)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_file_search(&args, package, ipkg_list_cb, NULL);
	args_deinit(&args);

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

/* not needed anymore
char* get_ipk_list(char* section)
{
	struct ipkg *node = ipkg;
	char* namelist = NULL;
	char* tmpstr = NULL;
	while(node != NULL)
	{
		if(node->name != NULL && node->version != NULL && section != NULL)
		{
			tmpstr = ostrcat("titan-plugin-", section, 0, 0);
			tmpstr = ostrcat(tmpstr, "-", 1, 0);
			if(string_find(tmpstr,node->name))
			{
				namelist = ostrcat(namelist, node->name, 1, 0);
				namelist = ostrcat(namelist, "\n", 1, 0);
			}
			free(tmpstr), tmpstr = NULL;
		}
		node = node->next;
	}
	return namelist;
}
*/

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
		debug(60, "skinname default=menulist");
		skinname1 = ostrcat(skinname, "menulist", 1, 0);
	}
	else
	{
		debug(60, "skinname changed=%s", skinname);
		skinname1 = ostrcat(skinname, "", 0, 0);
	}

	if(skinpath == NULL)
	{
		debug(60, "skinpath default=skin/");
		skinpath1 = ostrcat(skinpath, "skin/", 1, 0);
	}
	else
	{
		debug(60, "skinpath changed=%s", skinpath);
		skinpath1 = ostrcat(skinpath, "", 0, 0);
	}

	struct skin* screen = getscreen(skinname1);
	struct skin* listbox = getscreennode(screen, "listbox");
	
	listbox->aktpage = -1;
	listbox->aktline = 1;

	if(skintitle != NULL)
	{
		debug(60, "skintitle changed=%s", skintitle);
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
				debug(60, "showpng changed=%d", showpng);
	
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

				debug(60, "defaultdir %s", defaultdir);
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
		}
	}
	else if(showpng == 2)
	{
		int i = 0;
		tmpck = get_ipk_listinstall();
		struct ipkg *node = ipkg;
		while(node != NULL)
		{
			printf("name: %s\n", node->name);
			printf("section: %s\n", node->section);
			printf("showname: %s\n", node->showname);
			printf("str: %s\n", str);
									
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
	
				debug(60, "tmpck: (%d) %s", i, tmpstr1);
	
				ret3 = strsplit(tmpstr1, "\n", &count3);
				int skip = 0;
				for(a = 0; a < count3; a++)
				{
					if(ostrcmp((&ret3[a])->part, node->name) == 0)
					{
						debug(60, "set skiped=1");
						skip = 1;
						continue;
					}
				}
				if(skip == 0)
				{
					tmp = addlistbox(screen, listbox, tmp, 1);
					tmp->del = 1;
					tmp->valign = convertxmlentry("middle", 0);
					printf("321111\n");
					tmp->textposx = 250;
					tmp->height = 170;
	 				tmp->textposx2 = 270;
					tmp->type=TEXTBOX;
					tmp->wrap=YES;
					tmp->hspace = 5;
					debug(60, "showpng changed=%d", showpng);
	
					defaultdir = ostrcat("", skinpath1, 0, 0);
					defaultdir = ostrcat(defaultdir, "titan-pluginpreview-", 1, 0);
					defaultdir = ostrcat(defaultdir, showname, 1, 0);
					defaultdir = ostrcat(defaultdir, ".png", 1, 0);
	
					debug(60, "defaultdir %s", defaultdir);
	
					if(file_exist(defaultdir))
					{
						debug(60, "defaultdir found");
						tmpskinpath = ostrcat(skinpath1, "titan-pluginpreview-", 0, 0);
						tmpskinpath = ostrcat(tmpskinpath, showname, 1, 0);
						tmpskinpath = ostrcat(tmpskinpath, ".png", 1, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}	
					else
					{
						debug(60, "defaultdir not found use default.png");
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
	debug(60, "in");

	unlink("/var/usr/lib/ipkg/cross");
	unlink("/var/usr/lib/ipkg/secret");
	unlink("/var/usr/lib/ipkg/titan");	

	char* cmd = NULL, *tmpstr = NULL;
	cmd = ostrcat(cmd, "ipkg install /tmp/", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_listinstall()
{
	debug(60, "in");
	char* cmd = NULL, *tmpstr = NULL;

	cmd = ostrcat(cmd, "ipkg list_installed | awk '{ print $1 }' | sed 's/Successfully//' | sed 's/titan-plugin-//'", 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_tmplistinstall()
{
	debug(60, "in");
	char* cmd = NULL, *tmpstr = NULL;

	cmd = ostrcat(cmd, "ls /tmp | grep '.ipk'", 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_remove(char* ipk)
{
	debug(60, "in %s",ipk);
	char* cmd = NULL, *tmpstr = NULL;

	if(ipk == NULL) return NULL;

	cmd = ostrcat(cmd, "ipkg remove ", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(60, "out");
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_info(char* section)
{
	debug(60, "in %s",section);
	char* cmd = NULL, *tmpstr = NULL;

	if(section == NULL) return NULL;

	cmd = ostrcat(cmd, "ipkg list *-", 1, 0);
	cmd = ostrcat(cmd, section, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'-' -f6 | sed 's/Successfully terminated.//'", 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_install(char* ipk)
{
	debug(60, "in %s",ipk);

	char* cmd = NULL, *tmpstr = NULL;
	cmd = ostrcat(cmd, "ipkg install ", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

#endif

