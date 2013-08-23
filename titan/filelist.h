#ifndef FILELIST_H
#define FILELIST_H

int filelistflt(char* filter, char* name)
{
	int ret = 1, count = 0;

	char* tmpmatch = filter;
	char* tmpmatch1 = filter;
	char* tmpchar = NULL;

	if(filter != NULL && name != NULL)
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
					if(tmpchar[0] == '.') //filter without glob check
					{
						if(ostrrstrcase(name, tmpchar, -1, 1) != NULL)
							ret = 0;
					}
					else
						ret = fnmatch(tmpchar, name, FNM_CASEFOLD);
					free(tmpchar); tmpchar = NULL;
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

int filelistfilter(struct skin* node, char* name)
{
	if(node == NULL) return 1;

	return filelistflt(node->mask, name);
}

//int sizesort64(const void* v1, const void* v2)
int sizesort64(const struct dirent64** v1, const struct dirent64** v2)
{
	char* tmpstr = NULL, *rpath = NULL;
	off64_t s1, s2;

	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
	rpath = realpath(tmpstr, NULL);
	s1 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
	rpath = realpath(tmpstr, NULL);
	s2 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(s1 > s2) return 1;
	if(s1 < s2) return -1;
	return 0;
}


//int rsizesort64(const void* v1, const void* v2)
int rsizesort64(const struct dirent64** v1, const struct dirent64** v2)
{
	char* tmpstr = NULL, *rpath = NULL;
	off64_t s1, s2;

	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
	rpath = realpath(tmpstr, NULL);
	s1 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
	rpath = realpath(tmpstr, NULL);
	s2 = getfilesize(rpath);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(s1 > s2) return -1;
	if(s1 < s2) return 1;
	return 0;
}

//int datesort64(const void* v1, const void* v2)
int datesort64(const struct dirent64** v1, const struct dirent64** v2)
{
	char* tmpstr = NULL, *rpath = NULL;
	time_t t1, t2;

	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
	rpath = realpath(tmpstr, NULL);
	t1 = getfiletime(rpath, 2);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
	rpath = realpath(tmpstr, NULL);
	t2 = getfiletime(rpath, 2);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(t1 > t2) return -1;
	if(t1 < t2) return 1;
	return 0;
}

//int rdatesort64(const void* v1, const void* v2)
int rdatesort64(const struct dirent64** v1, const struct dirent64** v2)
{
	char* tmpstr = NULL, *rpath = NULL;
	time_t t1, t2;

	tmpstr = createpath(status.tmp, (char*) (*v1)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v1)->d_name);
	rpath = realpath(tmpstr, NULL);
	t1 = getfiletime(rpath, 2);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	tmpstr = createpath(status.tmp, (char*) (*v2)->d_name);
	//tmpstr = createpath(status.tmp, (char*) (*(const struct dirent64**)v2)->d_name);
	rpath = realpath(tmpstr, NULL);
	t2 = getfiletime(rpath, 2);
	free(tmpstr); tmpstr = NULL;
	free(rpath); rpath = NULL;

	if(t1 > t2) return 1;
	if(t1 < t2) return -1;
	return 0;
}

//int oalphasort64(const void* v1, const void* v2)
int oalphasort64(const struct dirent64** v1, const struct dirent64** v2)
{
	int ret = 0;

	ret = strcasecmp((*v1)->d_name, (*v2)->d_name);
	//ret = strcoll((*v1)->d_name, (*v2)->d_name);
	//ret = strcoll((*(const struct dirent64**)v1)->d_name, (*(const struct dirent64**)v2)->d_name);

	if(ret < 0) return -1;
	if(ret > 0) return 1;
	return 0;
}

//int ralphasort64(const void* v1, const void* v2)
int ralphasort64(const struct dirent64** v1, const struct dirent64** v2)
{
	int ret = 0;

	ret = strcasecmp((*v1)->d_name, (*v2)->d_name);
	//ret = strcoll((*v1)->d_name, (*v2)->d_name);
	//ret = strcoll((*(const struct dirent64**)v1)->d_name, (*(const struct dirent64**)v2)->d_name);

	if(ret > 0) return -1;
	if(ret < 0) return 1;
	return 0;
}

//view 0: deaktiv (normal filelist)
//view 1: big (list)
//view 2: cover (grid)
//view 3: fullcover (list)
//view 4: default (liste + size)
//view 5: details (liste + date)
//view 1000: not used here see inputhelp.h
int createfilelist(struct skin* screen, struct skin* node, int view)
{
	debug(1000, "in");
	struct dirent64 **filelist = NULL;

	int count = 0, tmpcount = 0, i = 0, gridbr = 0, posx = 0, pagecount = 0, sumcount = 0;
	struct skin *child = node, *oldchild = NULL, *parentdir = NULL;
	char *tmpstr = NULL;
	char *rpath = NULL;

	int (*cmpfunc)(const struct dirent64**, const struct dirent64**);
	//int (*cmpfunc)(const void*, const void*);

	if(node->input == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(view == 2)
		node->type |= GRID;
	else
		node->type &= ~(GRID);
	

	status.tmp = node->input;
	switch(getconfigint("dirsort", NULL))
	{
		case 1: cmpfunc = ralphasort64; break;
		case 2: cmpfunc = sizesort64; break;
		case 3: cmpfunc = rsizesort64; break;
		case 4: cmpfunc = datesort64; break;
		case 5: cmpfunc = rdatesort64; break;
		default: cmpfunc = oalphasort64; break;
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

	if(view == 2 && parentdir != NULL)
	{
		sumcount++;
		pagecount++;
		debug(913, "pdir: pagecount: %d", pagecount);
		debug(913, "pdir: sumcount: %d", sumcount);

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
		posx += parentdir->width;
		
		tmpstr = ostrcat(tmpstr, "skin/ext_grid_changedir.png", 1, 0);
		debug(913, "picpath: %s", tmpstr);
		changepic(parentdir, tmpstr);
		free(tmpstr); tmpstr = NULL;

		parentdir->type = GRIDBR; 
		gridbr++;
	} 

//	if(view == 2 || view == 3)
//		m_lock(&status.mediadbmutex, 17);
 	
	child = parentdir;

	//if the dir is empty add backpath
	if(count == 0)
	{
		free(filelist);
		filelist = (struct dirent64**)malloc(1 * sizeof(struct dirent*));
		struct dirent64* p = (struct dirent64*)malloc(sizeof(struct dirent64));
		p->d_type = DT_DIR;
		bcopy("..", p->d_name, 3);
		filelist[0] = p;
		count = 1;
	}

	tmpcount = count;
	while(tmpcount--)
	{
		//check if link is a dir or wenn unknown (rarfs ...)
		if(filelist[i]->d_type == DT_LNK || filelist[i]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(node->input, filelist[i]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[i]->d_type = DT_DIR;

			//for nfs mounts if file type is unknown use stat
			if(filelist[i]->d_type == DT_UNKNOWN)
				filelist[i]->d_type = getfiletype(tmpstr);
	
			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[i]->d_type == DT_DIR && filelist[i]->d_name != NULL && ostrcmp(filelist[i]->d_name, ".") != 0 && (status.showhiddenfiles == 1 || (status.showhiddenfiles == 0 && (filelist[i]->d_name[0] != '.' || ostrcmp(filelist[i]->d_name, "..") == 0))))
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
				if(view == 2)
				{
					if(gridbr == 0) child->type = GRIDBR;

					if(child != parentdir) 
					{
						sumcount++;
						pagecount++;
						debug(913, "dir: pagecount: %d", pagecount);
						debug(913, "dir: sumcount: %d", sumcount);
						
						debug(913, "filename: %s", filelist[i]->d_name);
	 					child->picheight = 180;
						child->picwidth = 180;
								
						child->height = 230;
						child->width = 370;
						child->prozwidth = 0;
						//child->bgcol = 0xffffff;
						child->bgspace = 1;
						child->vspace = 10;
						child->hspace = 10;
						//child->fontcol = 0x0000ff;
						child->halign = CENTER;
						child->valign = TEXTBOTTOM;
	
						child->posx = posx;
						posx += child->width;

						char* currentdir = getcurrentdir(node->input);

						if(ostrcmp(filelist[i]->d_name, "autofs") == 0)
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_autofs.png", 1, 0);
						else if(ostrcmp(filelist[i]->d_name, "hdd") == 0)
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_harddisk.png", 1, 0);
						else if((ostrcmp(filelist[i]->d_name, "usb") == 0) || (ostrcmp(currentdir, "usb") == 0))
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_usb.png", 1, 0);
						else if((ostrcmp(filelist[i]->d_name, "net") == 0) || (ostrcmp(currentdir, "net") == 0))
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_network.png", 1, 0);
						else
						{
							char* shortname = ostrcat(filelist[i]->d_name, NULL, 0, 0);
							string_tolower(shortname);
							string_removechar(shortname);
							strstrip(shortname);

							tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
							tmpstr = ostrcat(tmpstr, "/imdbfolder/", 1, 0);																			
							tmpstr = ostrcat(tmpstr, shortname, 1, 0);
							tmpstr = ostrcat(tmpstr, ".png", 1, 0);
							free(shortname); shortname = NULL;
													
							if(!file_exist(tmpstr))
							{
								free(tmpstr); tmpstr = NULL;
								tmpstr = ostrcat(tmpstr, "skin/ext_grid_directory.png", 1, 0);
							}
						}

						free(currentdir); currentdir = NULL;

						if(tmpstr != NULL)
						{
							debug(913, "picpath: %s", tmpstr);
							debug(913, "dir: change pic");
							changepic(child, tmpstr);
							free(tmpstr); tmpstr = NULL;
						}					
						gridbr++;
					}
					if(gridbr >= 3)
					{
						gridbr = 0;
						posx = 0;
					}
				}			
				else				
				{
					debug(913, "picpath: %s", node->pic);
					if(node->pic != NULL)
						changepic(child, node->pic);
				}
				changetext(child, filelist[i]->d_name);
				changename(child, filelist[i]->d_name);
				child->parentpointer = node;
				child->bordercol = node->bordercol;

				if(view != 2)
				{
					child->valign = MIDDLE;
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

				if(view > 3)
				{
					child->filelist = (struct filelist*)calloc(1, sizeof(struct filelist));
					if(child->filelist == NULL)
					{
						err("no mem");
						continue;
					}
					child->filelist->type = filelist[i]->d_type;
					child->filelist->view = view;

					tmpstr = createpath(node->input, filelist[i]->d_name);
					rpath = realpath(tmpstr, NULL);
					if(view == 4) child->filelist->size = getfilesize(rpath);
					else if(view == 5) child->filelist->date = getfiletime(rpath, 2);
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
		if(filelist[i]->d_type != DT_DIR && filelist[i]->d_name != NULL && (status.showhiddenfiles == 1 || (status.showhiddenfiles == 0 && filelist[i]->d_name[0] != '.')))
		{
			if(filelistfilter(node, filelist[i]->d_name) == 0)
			{
				child = addscreennode(screen, NULL, child);
				if(child != NULL)
				{
					debug(913, "filename: %s", filelist[i]->d_name);
					if(view == 2)
					{
						sumcount++;
						pagecount++;
						debug(913, "files: pagecount: %d", pagecount);
						debug(913, "files: sumcount: %d", sumcount);

						if(gridbr == 0) child->type = GRIDBR;
	
						child->picheight = 170;
						child->picwidth = 140;
						child->height = 230;
						child->width = 370;
						child->prozwidth = 0;
						//child->bgcol = 0xffffff;
						child->bgspace = 1;
						child->vspace = 10;
						child->hspace = 10;
						//child->fontcol = 0x0000ff;
						child->halign = CENTER;
						child->valign = TEXTBOTTOM;
	
						child->posx = posx;
						posx += child->width;

						if(cmpfilenameext(filelist[i]->d_name, ".png") == 0)
						{
							tmpstr = ostrcat(createpath(node->input, "/"), filelist[i]->d_name, 1, 0);
//							child->picheight = 210;
//							child->picwidth = 350;
							child->picwidth = 1;
							child->picheight = 1;
						}
						else if(cmpfilenameext(filelist[i]->d_name, ".jpg") == 0)
						{
							if(status.thumbthread != NULL)
							{
								//check if thumb exists
								tmpstr = checkthumb(node->input, filelist[i]->d_name);
								if(tmpstr == NULL)
								{
									addqueue(101, (void*)node->input, strlen(node->input) + 1, (void*)filelist[i]->d_name, strlen(filelist[i]->d_name) + 1, 0, NULL);
									child->picheight = 180;
									child->picwidth = 180;
								}
								else
								{
									child->picwidth = 1;
									child->picheight = 1;
								}
							}
							if(tmpstr == NULL) tmpstr = ostrcat(tmpstr, "skin/ext_grid_dummy.png", 1, 0);
						}
						else if(cmpfilenameext(filelist[i]->d_name, ".iso") == 0)
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_iso.png", 1, 0);
						else if(cmpfilenameext(filelist[i]->d_name, ".img") == 0)
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_img.png", 1, 0);
						else if(cmpfilenameext(filelist[i]->d_name, ".rar") == 0)
							tmpstr = ostrcat(tmpstr, "skin/ext_grid_rar.png", 1, 0);
						else
						{
							int fast = 0;
							if(fast == 1)
								tmpstr = ostrcat(tmpstr, changefilenameext(filelist[i]->d_name, ".jpg"), 1, 1);
							else
							{						
//								free(tmpstr), tmpstr = NULL;
//								tmpstr = ostrcat(node->input, "/", 0, 0);
//								tmpstr = ostrcat(tmpstr, filelist[i]->d_name, 1, 0);								
//								struct mediadb* mnode = getmediadb(tmpstr, 0);
								
								struct mediadb* mnode = getmediadb(node->input, filelist[i]->d_name, 0);
								free(tmpstr), tmpstr = NULL;
								if(mnode != NULL)
								{
									int musik = 0;
									if(cmpfilenameext(filelist[i]->d_name, ".mp3") == 0)
										musik = 1;
									else if(cmpfilenameext(filelist[i]->d_name, ".flac") == 0)
										musik = 1;
									else if(cmpfilenameext(filelist[i]->d_name, ".ogg") == 0)
										musik = 1;
									else if(cmpfilenameext(filelist[i]->d_name, ".ra") == 0)
										musik = 1;
									else if(cmpfilenameext(filelist[i]->d_name, ".wav") == 0)
										musik = 1;
								
									if(mnode->id != NULL && ostrstr(mnode->id, "tt") == NULL)
									{
										child->picheight = 180;
										child->picwidth = 300;
									}

									int len1 = strlen(mnode->title);
									int len2 = strlen(mnode->actors);
									if((mnode->title != NULL && len1 != 0 && musik == 0) || (mnode->actors != NULL && len2 != 0 && mnode->title != NULL && len1 != 0 && musik == 1))
									{
										if(musik == 1)
										{
											tmpstr = ostrcat(tmpstr, mnode->actors, 1, 0);
											tmpstr = ostrcat(tmpstr, " - ", 1, 0);
											tmpstr = ostrcat(tmpstr, mnode->title, 1, 0);
										}
										else
										{
											len1 = strlen(mnode->shortname);
											if(mnode->shortname != NULL && len1 != 0)
											{
												tmpstr = ostrcat(tmpstr, mnode->shortname, 1, 0);
											}
											else
												tmpstr = ostrcat(tmpstr, filelist[i]->d_name, 1, 0);
										}
	
										len1 = strlen(mnode->fileinfo);		
										if(mnode->fileinfo != NULL && len1 != 0)
										{
											tmpstr = ostrcat(tmpstr, " (", 1, 0);
											tmpstr = ostrcat(tmpstr, mnode->fileinfo, 1, 0);
											tmpstr = ostrcat(tmpstr, ")", 1, 0);
										}
											
										changetext(child, tmpstr);
										free(tmpstr), tmpstr = NULL;
									}
									else
										changetext(child, filelist[i]->d_name);

									if(mnode->id != NULL)
									{
										tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
										tmpstr = ostrcat(tmpstr, "/", 1, 0);																			
										tmpstr = ostrcat(tmpstr, mnode->id, 1, 0);
// cover + thumb unscharf
										if(musik == 1)
											tmpstr = ostrcat(tmpstr, "_cover.jpg", 1, 0);
										else
											tmpstr = ostrcat(tmpstr, "_cover.jpg", 1, 0);
									}

								}
								else
									changetext(child, filelist[i]->d_name);																
							}
							
							if(!file_exist(tmpstr))
							{
								free(tmpstr); tmpstr = NULL;
								child->picheight = 180;
								child->picwidth = 180;
								tmpstr = ostrcat(tmpstr, "skin/ext_grid_dummy.png", 1, 0);
							}
						}
						debug(913, "picpath: %s", tmpstr);
						if(tmpstr != NULL)
						{
							debug(913, "files: change pic");
							changepic(child, tmpstr);
							free(tmpstr); tmpstr = NULL;
						}
					
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
//						tmpstr = ostrcat(tmpstr, string_tolower(getfilenameext(filelist[i]->d_name)), 1, 1);
						tmpstr = ostrcat(tmpstr, string_tolower(getfilenamepng(filelist[i]->d_name)), 1, 1);
						tmpstr = ostrcat(tmpstr, ".png", 1, 0);
						debug(913, "picpath: %s", tmpstr);
						if(tmpstr != NULL)
							changepic(child, tmpstr);
						free(tmpstr); tmpstr = NULL;
					}

					child->bordercol = node->bordercol;
					if(view != 2)
					{
						child->valign = MIDDLE;
						child->width = 100;
						child->prozwidth = 1;
						child->height = node->fontsize + 2 + (node->bordersize * 2);
						child->textposx = node->textposx;			
					}
					else
						child->textposx = 1;

					if(view != 2)
						changetext(child, filelist[i]->d_name);
					changename(child, filelist[i]->d_name);
					
					child->parentpointer = node;

					child->del = FILELISTDELMARK;
					changeinput(child, NULL);

					if(view > 3)
					{
						child->filelist = (struct filelist*)calloc(1, sizeof(struct filelist));
						if(child->filelist == NULL)
						{
							err("no mem");
							continue;
						}
						child->filelist->type = filelist[i]->d_type;
						child->filelist->view = view;

						tmpstr = createpath(node->input, filelist[i]->d_name);
						rpath = realpath(tmpstr, NULL);
						if(view == 4) child->filelist->size = getfilesize(rpath);
						else if(view == 5) child->filelist->date = getfiletime(rpath, 2);
						free(rpath); rpath = NULL;
						free(tmpstr); tmpstr = NULL;
					}
					if(view == 3)
					{
						struct mediadb* mnode = getmediadb(node->input, filelist[i]->d_name, 0);
						int musik = 0;
						if(cmpfilenameext(filelist[i]->d_name, ".mp3") == 0)
							musik = 1;
						else if(cmpfilenameext(filelist[i]->d_name, ".flac") == 0)
							musik = 1;
						else if(cmpfilenameext(filelist[i]->d_name, ".ogg") == 0)
							musik = 1;
						else if(cmpfilenameext(filelist[i]->d_name, ".ra") == 0)
							musik = 1;
						else if(cmpfilenameext(filelist[i]->d_name, ".wav") == 0)
							musik = 1;

						if(mnode != NULL)
						{
							int len1 = strlen(mnode->title);
							int len2 = strlen(mnode->actors);
							if((mnode->title != NULL && len1 != 0 && musik == 0) || (mnode->actors != NULL && len2 != 0 && mnode->title != NULL && len1 != 0 && musik == 1))
							{
								if(musik == 1)
								{
									tmpstr = ostrcat(tmpstr, mnode->actors, 1, 0);
									tmpstr = ostrcat(tmpstr, " - ", 1, 0);
									tmpstr = ostrcat(tmpstr, mnode->title, 1, 0);
								}
								else
								{
									len1 = strlen(mnode->shortname);
									if(mnode->shortname != NULL && len1 != 0)
									{
										tmpstr = ostrcat(tmpstr, mnode->shortname, 1, 0);
									}
									else
										tmpstr = ostrcat(tmpstr, filelist[i]->d_name, 1, 0);									
								}

								len1 = strlen(mnode->fileinfo);								
								if(mnode->fileinfo != NULL && len1 != 1)
								{
									tmpstr = ostrcat(tmpstr, " (", 1, 0);
									tmpstr = ostrcat(tmpstr, mnode->fileinfo, 1, 0);
									tmpstr = ostrcat(tmpstr, ")", 1, 0);
								}
								changetext(child, tmpstr);
								free(tmpstr), tmpstr = NULL;
							}
							else
								changetext(child, filelist[i]->d_name);
						}
					}
				}
			}
		}
		free(filelist[i]);
		i++;
	}

//	if(view == 2 || view == 3)
//		m_unlock(&status.mediadbmutex, 17);	
/*
	for (i = 0; i <= pagecount; i++)
	{
		printf("delmarkedpic=%d\n", i + 1000);
		delmarkedpic(i + 1000);
	}
*/
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
		changemask(filelist, "*");
	else
		changemask(filelist, filemask);

	changeinput(filelist, tmpstr);
	changetext(filelistpath, filelist->input);
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(selection, NULL, 0, 0);

	delmarkedscreennodes(input, FILELISTDELMARK);

	if (tmpview == -1)
		createfilelist(input, filelist, 0);
	else
		createfilelist(input, filelist, tmpview);

	if(tmpstr != NULL)
		setlistboxselection(filelist, tmpstr);
	free(tmpstr); tmpstr = NULL;

	drawscreen(input, 0, 0);
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

	int r = getrandom(maxfiles);
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
