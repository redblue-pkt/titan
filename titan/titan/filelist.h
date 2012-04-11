#ifndef FILELIST_H
#define FILELIST_H

int filelistfilter(struct skin* node, char* name)
{
	int ret = 0, count = 0;

	if(node == NULL)
		return 1;

	char* tmpmatch = node->mask;
	char* tmpmatch1 = node->mask;

	char* tmpchar = NULL;

	if(node->mask != NULL)
	{
		tmpmatch--;
		do
		{
			tmpmatch++;
			if(*tmpmatch == ' ' || *tmpmatch == '\0')
			{
				tmpchar = strndup(tmpmatch1, count);
				if(tmpchar != NULL)
				{
					ret = fnmatch(tmpchar, name, FNM_CASEFOLD);
					free(tmpchar);
					if(ret == 0) return 0;
				}
				tmpmatch1 = tmpmatch + 1;
				count = -1;
			}
			count++;
		}
		while(*tmpmatch != '\0');
	}

	return 1;
}

#ifdef SIMULATE
int sizesort64(const struct dirent64** v1, const struct dirent64** v2)
#else
int sizesort64(const void* v1, const void* v2)
#endif
{
	char* tmpstr = NULL, *rpath = NULL;
	off64_t s1, s2;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	s1 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	s2 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(s1 > s2) return 1;
	if(s1 < s2) return -1;
	return 0;
}

#ifdef SIMULATE
int rsizesort64(const struct dirent64** v1, const struct dirent64** v2)
#else
int rsizesort64(const void* v1, const void* v2)
#endif
{
	char* tmpstr = NULL, *rpath = NULL;
	off64_t s1, s2;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	s1 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	s2 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(s1 > s2) return -1;
	if(s1 < s2) return 1;
	return 0;
}

#ifdef SIMULATE
int datesort64(const struct dirent64** v1, const struct dirent64** v2)
#else
int datesort64(const void* v1, const void* v2)
#endif
{
	char* tmpstr = NULL, *rpath = NULL;
	time_t t1, t2;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	t1 = getfiletime(rpath, 0);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	t2 = getfiletime(rpath, 0);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(t1 > t2) return -1;
	if(t1 < t2) return 1;
	return 0;
}

#ifdef SIMULATE
int rdatesort64(const struct dirent64** v1, const struct dirent64** v2)
#else
int rdatesort64(const void* v1, const void* v2)
#endif
{
	char* tmpstr = NULL, *rpath = NULL;
	time_t t1, t2;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	t1 = getfiletime(rpath, 0);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

#ifdef SIMULATE
	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
#else
	tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
#endif
	rpath = realpath(tmpstr, NULL);
	t2 = getfiletime(rpath, 0);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(t1 > t2) return 1;
	if(t1 < t2) return -1;
	return 0;
}

#ifdef SIMULATE
int ralphasort64(const struct dirent64** v1, const struct dirent64** v2)
#else
int ralphasort64(const void* v1, const void* v2)
#endif
{
	int ret = 0;

#ifdef SIMULATE
	ret = strcoll((*v1)->d_name, (*v2)->d_name);
#else
	ret = strcoll((*(const struct dirent64**)v1)->d_name, (*(const struct dirent64**)v2)->d_name);
#endif

	if(ret > 0) return -1;
	if(ret < 0) return 1;
	return 0;
}

//flag 0: listbox view
//flag 1: grid view
int createfilelist(struct skin* screen, struct skin* node, int flag)
{
	debug(1000, "in");
	struct dirent64 **filelist;
	int count, tmpcount, i = 0, gridbr = 0, posx = 0;
	struct skin *child = node, *oldchild = NULL, *parentdir = NULL;
	char *tmpstr = NULL;
	char *rpath = NULL;
#ifdef SIMULATE
	int (*cmpfunc)(const struct dirent64**, const struct dirent64**);
#else
	int (*cmpfunc)(const void*, const void*);
#endif

	if(node->input == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(status.filelistextend == 1) node->type |= GRID;

	status.tmp = node->input;
	switch(getconfigint("dirsort", NULL))
	{
		case 1: cmpfunc = ralphasort64; break;
		case 2: cmpfunc = sizesort64; break;
		case 3: cmpfunc = rsizesort64; break;
		case 4: cmpfunc = datesort64; break;
		case 5: cmpfunc = rdatesort64; break;
		default: cmpfunc = alphasort64; break;
	}

	count = scandir64(node->input , &filelist, 0, cmpfunc);
	if(count < 0)
	{
		if(getconfig("failbackpath", NULL) != NULL)
		{
			perr("scandir");
			count = scandir64(getconfig("failbackpath", NULL) , &filelist, 0, cmpfunc);
			changeinput(node, getconfig("failbackpath", NULL));
		}
		if(count < 0)
		{
			perr("scandir");
			count = scandir64("/" , &filelist, 0, cmpfunc);
			changeinput(node, "/");
		}
		if(count < 0)
			return 1;
	}

	status.tmp = NULL;
	parentdir = addscreennode(screen, NULL, child);

/*
0 fast
1 cover (grid)
2 big (list)
3 default (liste + gr�sse)
4 details (liste + date)
5 cover1 (list + imdb)
6 cover2 (list + imdb)
7 cover3 (list + imdb)
8 fullcover (list)
*/

	if(status.filelistextend == 1 && parentdir != NULL) 
	{
		parentdir->picheight = 180;
		parentdir->picwidth = 180;				
		parentdir->height = 230;
		parentdir->width = 370;
		parentdir->prozwidth = 0;
		//parentdir->bgcol = 0xffffff;
		parentdir->bgspace = 1;
		parentdir->vspace = 10;
		parentdir->hspace = 10;
		parentdir->posx = posx;
		//parentdir->fontcol = 0x0000ff;
		parentdir->halign = CENTER;
		parentdir->valign = TEXTBOTTOM;
		parentdir->posx = posx;
		posx += parentdir->width;

		tmpstr = ostrcat(tmpstr, "skin/changedir_cover.png", 1, 0);
		changepic(parentdir, tmpstr);
		free(tmpstr); tmpstr = NULL;

		parentdir->type = GRIDBR; 
		gridbr++;
	} 
 	
	child = parentdir;
	tmpcount = count;
	while(tmpcount--)
	{
		//check if link is a dir or wenn unknown (rarfs ...)
		if(filelist[i]->d_type == DT_LNK || filelist[i]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(node->input, filelist[i]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[i]->d_type = DT_DIR;
	
			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[i]->d_type == DT_DIR && ostrcmp(filelist[i]->d_name, ".") != 0)
		{
			if(ostrcmp(filelist[i]->d_name, "..") == 0)
			{
				oldchild = child;
				child = parentdir;
			}
			else
				child = addscreennode(screen, NULL, child);
			if(child != NULL)
			{
				if(status.filelistextend == 1)
				{
					if(gridbr == 0) child->type = GRIDBR;

					if(child != parentdir) 
					{
	 					child->picheight = 180;
						child->picwidth = 180;
								
						child->height = 230;
						child->width = 370;
						child->prozwidth = 0;
						//child->bgcol = 0xffffff;
						child->bgspace = 1;
						child->vspace = 10;
						child->hspace = 10;
						child->posx = posx;
						//child->fontcol = 0x0000ff;
						child->halign = CENTER;
						child->valign = TEXTBOTTOM;
	
						child->posx = posx;
	
						posx += child->width;
	
						if(ostrcmp(filelist[i]->d_name, "..") == 0)
							tmpstr = ostrcat(tmpstr, "skin/changedir_cover.png", 1, 0);
						else if(ostrcmp(filelist[i]->d_name, "autofs") == 0)
							tmpstr = ostrcat(tmpstr, "skin/autofs_cover.png", 1, 0);
						else if(ostrcmp(filelist[i]->d_name, "hdd") == 0)
							tmpstr = ostrcat(tmpstr, "skin/harddisk_cover.png", 1, 0);
						else if(ostrcmp(filelist[i]->d_name, "usb") == 0)
							tmpstr = ostrcat(tmpstr, "skin/usb_cover.png", 1, 0);
						else if(ostrcmp(filelist[i]->d_name, "net") == 0)
							tmpstr = ostrcat(tmpstr, "skin/network_cover.png", 1, 0);
						else
						{
	//						tmpstr = ostrcat(tmpstr, "/var/swap/imdbfolder/", 1, 0);
							tmpstr = ostrcat(tmpstr, "/media/autofs/DEBIAN/imdbfolder/", 1, 0);
							int fast = 0;
							if(fast == 1)
							{
								tmpstr = ostrcat(tmpstr, filelist[i]->d_name, 1, 0);
								tmpstr = ostrcat(tmpstr, ".png", 1, 0);
							}
							else
							{	
								char* filename = NULL;
								filename = ostrcat(filename, strdup(filelist[i]->d_name), 1, 0);
								debug(10, "[filelist] filename: %s", filename);
								string_tolower(filename);
//								printf("1filename: %s\n",filename);
	
								filename = string_shortname(filename,2);
//								printf("2filename: %s\n",filename);	
							
								string_removechar(filename);
//								printf("3filename: %s\n",filename);
	
								string_remove_whitechars(filename);
//								printf("4filename: %s\n",filename);
	
								tmpstr = ostrcat(tmpstr, filename, 1, 0);
								free(filename); filename = NULL;
								tmpstr = ostrcat(tmpstr, ".png", 1, 0);
							}	
														
							if(!file_exist(tmpstr))
							{
								free(tmpstr); tmpstr = NULL;
								tmpstr = ostrcat(tmpstr, "skin/directory_cover.png", 1, 0);
							}
						}
						if(tmpstr != NULL)
							changepic(child, tmpstr);
								
						printf("tmpstr: %s\n",tmpstr);
						free(tmpstr); tmpstr = NULL;
											
						gridbr++;
					}
					if(gridbr >= 3)
					{
						gridbr = 0;
						posx = 0;
					}
				}			

				child->bordercol = node->bordercol;
				
				if(status.filelistextend != 1)
				{
					if(node->pic != NULL)
						changepic(child, node->pic);

					child->valign = MIDDLE;
					child->width = 200;
					child->prozwidth = 1;
					child->height = node->fontsize + 2 + (node->bordersize * 2);
				}
				changetext(child, filelist[i]->d_name);
				changename(child, filelist[i]->d_name);
				child->parentpointer = node;

				if(status.filelistextend != 1)
				{
					child->width = 100;
					child->prozwidth = 1;
					child->height = node->fontsize + 2 + (node->bordersize * 2);
					child->textposx = node->textposx;
				}
				else
					child->textposx = 1;

				child->del = FILELISTDELMARK;
				tmpstr = createpath(node->input, child->text);
				changeinput(child, tmpstr);
				free(tmpstr); tmpstr = NULL;

				if(status.filelistextend > 1)
				{
					child->filelist = (struct filelist*)malloc(sizeof(struct filelist));
					if(child->filelist == NULL)
					{
						err("no mem");
						continue;
					}
					memset(child->filelist, 0, sizeof(struct filelist));
					child->filelist->type = DT_DIR;
					child->filelist->name = ostrcat(filelist[i]->d_name, "", 0, 0);
					child->filelist->path = createpath(node->input, "");

					if(status.filelistextend == 4)
					{
						char* filename = NULL;
						filename = ostrcat(filename, strdup(filelist[i]->d_name), 1, 0);
						debug(10, "[filelist] filename: %s", filename);
						string_tolower(filename);
						char* tmpfile = NULL;
						tmpfile = string_shortname(filename,1);
						string_removechar(tmpfile);
						string_strip_whitechars(tmpfile);
						char* shortname = NULL;
						shortname = ostrcat(shortname, tmpfile, 1, 0);
						string_toupper(shortname);
						changetext(child, shortname);
						char* tmpfile1;
						tmpfile1 = string_shortname(tmpfile,0);
						string_remove_whitechars(tmpfile1);
						child->filelist->imdbpath = tmpfile1;
					}

					tmpstr = createpath(node->input, filelist[i]->d_name);
					rpath = realpath(tmpstr, NULL);
					child->filelist->size = getfilesize(rpath);
					child->filelist->date = getfiletime(rpath, 0);
					free(tmpstr); tmpstr = NULL;
					free(rpath); rpath = NULL;
				}
			}
			if(parentdir == child)
				child = oldchild;

		}
		i++;
	}

	tmpcount = count;
	i=0;
	while(tmpcount--)
	{
		if(filelist[i]->d_type != DT_DIR)
		{
			if(filelistfilter(node, filelist[i]->d_name) == 0)
			{
				child = addscreennode(screen, NULL, child);
				if(child != NULL)
				{
					if(status.filelistextend == 1)
					{
						if(gridbr == 0) child->type = GRIDBR;
	
						child->picheight = 170;
						child->picwidth = 140;
	//					child->picwidth = 350;
								
						child->height = 230;
						child->width = 370;
						child->prozwidth = 0;
						//child->bgcol = 0xffffff;
						child->bgspace = 1;
						child->vspace = 10;
						child->hspace = 10;
						child->posx = posx;
						//child->fontcol = 0x0000ff;
						child->halign = CENTER;
						child->valign = TEXTBOTTOM;
	
						child->posx = posx;
	
						posx += child->width;
	
						if(cmpfilenameext(filelist[i]->d_name, ".iso") == 0)
							tmpstr = ostrcat(tmpstr, "skin/iso_cover.png", 1, 0);
						else if(cmpfilenameext(filelist[i]->d_name, ".img") == 0)
							tmpstr = ostrcat(tmpstr, "skin/img_cover.png", 1, 0);
						else if(cmpfilenameext(filelist[i]->d_name, ".rar") == 0)
							tmpstr = ostrcat(tmpstr, "skin/rar_cover.png", 1, 0);
						else
						{
							int fast = 0;
							if(fast == 1)
								tmpstr = ostrcat(tmpstr, changefilenameext(filelist[i]->d_name, ".jpg"), 1, 0);
							else
							{						
								char* filename = NULL;
								filename = ostrcat(filename, strdup(filelist[i]->d_name), 1, 0);
								debug(10, "[filelist] filename: %s", filename);
								string_tolower(filename);
//								printf("1filename: %s\n",filename);

								filename = string_shortname(filename,2);
//								printf("2filename: %s\n",filename);	
							
								string_removechar(filename);
//								printf("3filename: %s\n",filename);

								string_remove_whitechars(filename);
//								printf("4filename: %s\n",filename);

								tmpstr = ostrcat(tmpstr, "/media/autofs/DEBIAN/imdb/", 1, 0);
								tmpstr = ostrcat(tmpstr, filename, 1, 0);
								free(filename); filename = NULL;
								tmpstr = ostrcat(tmpstr, ".jpg", 1, 0);
							}
							
							if(!file_exist(tmpstr))
							{
								free(tmpstr); tmpstr = NULL;
								child->picheight = 180;
								child->picwidth = 180;
								tmpstr = ostrcat(tmpstr, "skin/dummy.png", 1, 0);
							}
						}
						if(tmpstr != NULL)
							changepic(child, tmpstr);
								
						printf("tmpstr: %s\n",tmpstr);
						free(tmpstr); tmpstr = NULL;
											
						gridbr++;
						if(gridbr >= 3)
						{
							gridbr = 0;
							posx = 0;
						}
					}
					else
					{
						tmpstr = ostrcat(tmpstr, "skin/ext_", 1, 0);
						tmpstr = ostrcat(tmpstr, getfilenameext(filelist[i]->d_name), 1, 0);
						tmpstr = ostrcat(tmpstr, ".png", 1, 0);
						if(tmpstr != NULL)
							changepic(child, tmpstr);
						free(tmpstr); tmpstr = NULL;
					}

					child->bordercol = node->bordercol;
					if(status.filelistextend != 1)
					{
						child->width = 100;
						child->prozwidth = 1;
						child->height = node->fontsize + 2 + (node->bordersize * 2);
						child->textposx = node->textposx;
					}
					else
						child->textposx = 1;

					changetext(child, filelist[i]->d_name);
					changename(child, filelist[i]->d_name);
					
					child->parentpointer = node;

					child->del = FILELISTDELMARK;
					changeinput(child, NULL);

					if(status.filelistextend > 1)
					{
						child->filelist = (struct filelist*)malloc(sizeof(struct filelist));
						if(child->filelist == NULL)
						{
							err("no mem");
							continue;
						}
						memset(child->filelist, 0, sizeof(struct filelist));
						child->filelist->type = DT_DIR;
						child->filelist->name = ostrcat(filelist[i]->d_name, "", 0, 0);
						child->filelist->path = createpath(node->input, "");

						if(status.filelistextend == 4)
						{
							char* filename = NULL;
							filename = ostrcat(filename, strdup(filelist[i]->d_name), 1, 0);
							debug(10, "[filelist] filename: %s", filename);
							string_tolower(filename);
							char* tmpfile = NULL;
							tmpfile = string_shortname(filename,1);
							string_removechar(tmpfile);
							string_strip_whitechars(tmpfile);
							char* shortname = NULL;
							shortname = ostrcat(shortname, tmpfile, 1, 0);
							string_toupper(shortname);
							changetext(child, shortname);
							char* tmpfile1;
							tmpfile1 = string_shortname(tmpfile,0);
							string_remove_whitechars(tmpfile1);
							child->filelist->imdbpath = tmpfile1;
						}

						tmpstr = createpath(node->input, filelist[i]->d_name);
						rpath = realpath(tmpstr, NULL);
						child->filelist->size = getfilesize(rpath);
						child->filelist->date = getfiletime(rpath, 0);
						free(tmpstr); tmpstr = NULL;
						free(rpath); rpath = NULL;
					}
				}
			}
		}
		free(filelist[i]);
		i++;
	}

	free(filelist);
	debug(1000, "out");
	return 0;
}

void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview, char* selection)
{
	debug(1000, "in");
	char* tmpstr = NULL;

	if(path == NULL || strlen(path) == 0 || !isdir(path))
		tmpstr = strdup("/");
	else
		tmpstr = strdup(path);

	filelist->aktline = 0;
	filelist->aktpage = 0;

	if(filemask == NULL)
		changemask(filelist,"*");	
	else
		changemask(filelist,filemask);

	changeinput(filelist, tmpstr);
	changetext(filelistpath, filelist->input);
	free(tmpstr);
	if(selection != NULL)
		setlistboxselection(filelist, selection);
	
	delmarkedscreennodes(input, FILELISTDELMARK);


	int view = 0;		
	if (tmpview == -1)
		view = getconfigint("view", NULL);
	else
		view = tmpview;

	status.filelistextend = view;
	printf("getfilelist: view=%d status=%d\n", view, status.filelistextend);

//	clearscreen(input);
	createfilelist(input, filelist, 0);
	drawscreen(input, 0);
	debug(1000, "out");
}

void getfilelistmax(struct skin* filelist, int* maxdirs, int* maxfiles)
{
	struct skin* node = filelist;

	while(node != NULL)
	{
		if(node->del == FILELISTDELMARK)
		{
			if(node->input != NULL)
				(*maxdirs)++;
			else
				(*maxfiles)++;
		}
		node = node->next;
	}
}

struct skin* getfilelistrandom(struct skin* filelist, int maxdirs, int maxfiles)
{
	int count = 0;
	struct skin* node = filelist;
	
	if(maxfiles < 1) return NULL;

	srand(time(NULL));
	int r = rand() % maxfiles;
	r++;

	while(node != NULL)
	{
		if(node->del == FILELISTDELMARK && node->input == NULL)
		{
			count++;
			if(count == r) break;
		}

		node = node->next;
	}

	return node;
}

#endif
