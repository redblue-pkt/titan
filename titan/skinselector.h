/*
 * =======================================================================
 *
 *       Filename:  skinselector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.06.2011 15:16:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dvboxer
 *        Company:  AAF
 *
 * =======================================================================
 */


#ifndef SKINSELECTOR_H
#define SKINSELECTOR_H


void screenskinselect(void)
{
	const int max_list_size=50;
	char *tmpstr = NULL, *token, *saveptr, *cur_skin=getconfig("skinfile",NULL), *skinpaths;

	if(getconfig("skins", NULL) == NULL)
		skinpaths=strdup("/var/swap/skin;/var/skin");
	else
		skinpaths=strdup(getconfig("skins", NULL));

	struct ListType
	{
		char *name;
		char *displayname;
		dev_t dev_id;
		ino_t inode;
		int is_cur;
	} list[max_list_size];

	int i = 0, j, skinchanged = 0;
	DIR *dp;
	struct dirent *ep;

	if(cur_skin == NULL)
	{
		err("skin not defined. This should not happen");
	}
	struct stat cur_skin_stat;
	stat(cur_skin, &cur_skin_stat);

	//make sure that current skin is listed
	skinpaths=(char *)realloc(skinpaths, strlen(skinpaths) + strlen(cur_skin) + 3);
	strcat(strcat(skinpaths, ";"), cur_skin);
	j = strlen(cur_skin);
	if(j > 8) j = 9;
	skinpaths[strlen(skinpaths) - j] = '\0';

	token = strtok_r(skinpaths, ";" , &saveptr);
	while(token != NULL) 
	{
		dp = opendir(token);
		if(dp != NULL)
		{
			while((ep = readdir(dp)))
			{
				if(!strcmp(ep->d_name, "..")) continue; //skip search in parent dir
				char *dn = strdup(ep->d_name);
				if(dn == NULL) continue;
				list[i].name=(char*)malloc(strlen(token) + strlen(dn) + 20);
				list[i].displayname=strdup(dn);
				if(list[i].name == NULL || list[i].displayname == NULL )
				{
					free(list[i].displayname);
					free(list[i].name);
					free(dn);
					break;
				}
				if(!strcmp(ep->d_name, ".")) 
					sprintf(list[i].name, "%s/skin.xml", token);
				else
					sprintf(list[i].name, "%s/%s/skin.xml", token, dn);
				free(dn);

				struct stat ss;
				if(stat(list[i].name, &ss)) //file doesn't exist
				{
					free(list[i].displayname);
					free(list[i].name);
					continue;
				}
				list[i].dev_id = ss.st_dev;
				list[i].inode = ss.st_ino;

				for(j = 0; j < i; ++j)
				{
					if(list[j].dev_id == list[i].dev_id && list[j].inode == list[i].inode) // same file already registered
					{
						j = -1; break;
					}
				}
				if(j == -1) //file already seen
				{
					free(list[i].displayname);
					free(list[i].name);
					continue;
				}
				list[i].is_cur = (list[i].dev_id == cur_skin_stat.st_dev && list[i].inode == cur_skin_stat.st_ino);

				if(++i == max_list_size) break;
			}
			if(i == max_list_size) break;
			closedir(dp);
		}
		else
		{
			err("reading skin directory %s", "x");
		}
        
		token=strtok_r(NULL, ";", &saveptr);
	}


	int rcret = 0;
	struct skin* screen = getscreen("skinselect");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* png = getscreennode(screen, "png");
	struct skin* tmp=NULL;

	for(j = 0; j < i; ++j)
	{
		tmp = addlistbox(screen, listbox, tmp, 1);
		if(tmp != NULL)
		{
			if(ostrcmp(list[j].displayname, ".") == 0)
				changetext(tmp, _("unknown")); //skin.xml is in main dir
			else
			{
				if(ostrstr(list[j].name, "/swap/") != NULL)
				{
					char* tmpname = NULL;
					tmpname = ostrcat("", list[j].displayname, 0, 0);
					tmpname = ostrcat(tmpname,  " (swap)", 1, 0);
					changetext(tmp, tmpname);
					free(tmpname),tmpname = NULL;
				}
				else
					changetext(tmp, list[j].displayname);
			}
			tmp->del = 1;
			tmp->handle = (char *)(list + j);
			if(list[j].is_cur)
			{
				listbox->aktline = j + 1;
				char *pf = strdup(list[j].name);
				strncpy(pf + strlen(pf)-8, "prev.png", 8);
				changepic(png, pf);
				free(pf);
			}
		}
		else
		{
			err("tmp is NULL");
		}
	}
   	
	listbox->aktpage = -1;
	
	drawscreen(screen, 0, 0);
	addscreenrc(screen, listbox);

	while (1)
	{
		addscreenrc(screen, tmp);
		rcret = waitrc(screen, 0, 0);
		tmp = listbox->select;
		if(rcret == getrcconfigint("rcexit", NULL))  //user pressed exit so no change
		{
			debug(100, "no change of skin");
			break;
		}
		else if(rcret == getrcconfigint("rcok",NULL)) //ok means "use that skin"
		{
			struct ListType *selection=((struct ListType *)listbox->select->handle);
			if(selection != NULL)
			{
				debug(10,"skin file selected: %s", selection->name);
				if(selection->is_cur) break; //same file chosen again
				
				//write old skinconfig on change and set write flag to 0
				//so new skinconfig is not overwritten with old
				if(status.writeskinconfig == 1)
				{
					writeskinconfig(getconfig("skinconfig", NULL));
					status.writeskinconfig = 0;
				}
				
				struct splitstr* ret1 = NULL;
				int count1 = 0;
				char* tmpstr1 = NULL;
				tmpstr1 = ostrcat("", selection->name, 0, 0);
				ret1 = strsplit(tmpstr1, " ", &count1);
				tmpstr = ostrcat((&ret1[0])->part, "", 0, 0);
				free(ret1), ret1 = NULL;
				free(tmpstr1), tmpstr1 = NULL;
				
				tmpstr = dirname(tmpstr);
				tmpstr = ostrcat(tmpstr, "/skinconfig", 1, 0);
				if(file_exist(tmpstr))
				{
					addconfig("skinconfig", tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
				else
				{
					textbox(_("Message"), _("No skinconfig found in skin.\nCan't chage to selected skin."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(screen, 0, 0);
					free(tmpstr); tmpstr = NULL;
					continue;
				}

				addconfig("skinfile", selection->name);

				tmpstr = ostrcat(selection->name, NULL, 0, 0);
				addconfig("skinpath", dirname(tmpstr));

				skinchanged = 1;
				free(tmpstr); tmpstr = NULL;

				break;
			}
		}
		else if(rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL))
		{
			struct ListType *selection = list + listbox->aktline - 1;
			char *pf=strdup(selection->name);
			strncpy(pf + strlen(pf) - 8, "prev.png", 8);
			changepic(png, pf);
			free(pf);
			drawscreen(screen, 0, 0);
		}
	}

	delownerrc(screen);
	delmarkedscreennodes(screen, 1);
	clearscreen(screen);
	--i; for(;i >= 0; --i) 
	{
		free(list[i].displayname);
		free(list[i].name);
	}
	free(skinpaths);
	if(skinchanged) oshutdown(3, 1);
}

#endif

