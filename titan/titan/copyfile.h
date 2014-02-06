#ifndef COPYFILE_H
#define COPYFILE_H

int countfiles(char* dirname, int* count, int first)
{
	int ret = 0;
	DIR *d;
	char* tmpstr = NULL;

	//Open the directory specified by dirname
	d = opendir(dirname);

	//Check it was opened
	if(! d)
	{
		perr("Cannot open directory %s", dirname);
		return 1;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dirname);
		//Readdir gets subsequent entries from d
		entry = readdir(d);

		if(!entry) //There are no more entries in this directory, so break out of the while loop
			break;
			
		//for nfs mounts if file type is unknown use stat
		if(entry->d_type == DT_UNKNOWN)
		{
			tmpstr = ostrcat(dirname, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			entry->d_type = getlfiletype(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		//See if entry is a subdirectory of d
		if(entry->d_type == DT_DIR)
		{
			//Check that the directory is not d or d's parent
			if(entry->d_name != NULL && ostrcmp(entry->d_name, ".") != 0 && ostrcmp(entry->d_name, "..") != 0)
			{
				(*count)++;
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					break;
				}
				//Recursively call findfiles with the new path
				countfiles(path, count, 0);
			}
		}
		else //File
		{
			if(entry->d_type == DT_LNK)
				(*count)++;
			else if(entry->d_type == DT_BLK)
				(*count)++;
			else if(entry->d_type == DT_CHR)
				(*count)++;
			else if(entry->d_type == DT_FIFO)
				(*count)++;
			else if(entry->d_type == DT_REG)
				(*count)++;
		}
	}

	//After going through all the entries, close the directory
	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		ret = 1;
	}

	return ret;
}

//flag 0: copy file
//flag 1: move file
int copyfilereal(char* from, char* to, struct copyfile* node, int flag)
{
	int fdfrom = -1, fdto = -1, ret = 0, readret = 0, writeret = 0;
	off64_t count = 0, len = 0;
	unsigned char* buf = NULL;

	fdfrom = open(from, O_RDONLY | O_LARGEFILE);
	if(fdfrom < 0)
	{
		perr("open from %s", from);
		ret = 1;
		goto end;
	}
	
	ret = unlink(to);
	if(ret != 0 && errno != ENOENT)
	{
		perr("remove file %s", to);
		ret = 1;
		goto end;
	}
	else
		ret = 0;
		
	fdto = open(to, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0777);
	if(fdto < 0)
	{
		perr("open to %s", to);
		ret = 1;
		goto end;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}
	
	len = lseek64(fdfrom, 0, SEEK_END);
	if(len < 0)
	{
		perr("can't get filelen %s", from);
		ret = 1;
		goto end;
	}
	
	if(lseek64(fdfrom, 0, SEEK_SET) < 0)
	{
		perr("can't seek to startpos %s", from);
		ret = 1;
		goto end;
	}

	if(node != NULL) node->maxkb += len;  	

	while(len != 0)
	{		
		if(len - count > MINMALLOC)
			readret = dvbreadfd(fdfrom, buf, 0, MINMALLOC, -1, 1);
		else
			readret = dvbreadfd(fdfrom, buf, 0, len - count, -1, 1);
		if(readret <= 0)
		{
			err("read file %s", from);
			ret = 1;
			goto end;
		}
		
		if(node != NULL && node->stop == 1)
		{
		  ret = 1;
			goto end;
		}

		writeret = dvbwrite(fdto, buf, readret, -1);
		if(writeret != readret)
		{
			err("write file %s", to);
			ret = 1;
			goto end;
		}
		
		if(node != NULL && node->stop == 1)
		{
		  ret = 1;
			goto end;
		}

		count += readret;
		if(node != NULL)
		{ 
			node->aktkb += readret;
			if(node->maxfilecount == 1)
				node->proz = (int)ceil((((float)node->aktkb / node->maxkb) * 100));
		}
		
		if(count == len) break;
		if(count > len)
		{
			err("write more then filelen %s", to);
			ret = 1;
			goto end;
		}
	}
	
end:
	if(fdfrom >= 0)
		close(fdfrom);
	if(fdto >= 0)
	{
		close(fdto);
		if(ret == 1) unlink(to);
	}	
	if(ret == 0 && flag == 1)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", from);
			unlink(to);
			ret = 1;
		}
		else
			ret = 0;
	}
	free(buf);

	if(node != NULL)
	{ 
		node->filecount++;
		if(node->maxfilecount > 1)
			node->proz = (int)ceil((((float)node->filecount / node->maxfilecount) * 100));
	}
	return ret;
}

//flag 0: copy file
//flag 1: move file
int copylink(char* from, char* to, struct copyfile* node, int flag)
{
	int ret = 0;
	
	if(from == NULL || to == NULL) return 1;

	char* buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	ret = readlink(from, buf, MINMALLOC);
	if(ret < 0)
	{
		perr("read link %s", from);
		free(buf); buf = NULL;
		return 1;
	}

	ret = tpkcreatelink("", to, buf, 0);
	if(ret != 0)
	{
		free(buf); buf = NULL;
		err("create link");
		return 1;
	}
			
	if(flag == 1)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", from);
			free(buf); buf = NULL;
			return 1;
		}
	}

	if(node != NULL)
	{ 
		node->filecount++;
		if(node->maxfilecount > 1)
			node->proz = (int)ceil((((float)node->filecount / node->maxfilecount) * 100));
	}
	free(buf); buf = NULL;
	return 0;
}

//flag 0: copy file
//flag 1: move file
int copyblk(char* from, char* to, struct copyfile* node, int flag)
{
	int ret = 0;
	struct stat64 s;
	
	if(from == NULL || to == NULL) return 1;

	ret = stat64(from, &s);
	if(ret != 0)
	{
		perr("get file status %s", from);
		return 1;
	}
			
	ret = tpkcreateblk("", to, major(s.st_rdev), minor(s.st_rdev), 0);
	if(ret != 0)
	{
		err("create blk dev");
		return 1;
	}
			
	if(flag == 1)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", from);
			return 1;
		}
	}

	if(node != NULL)
	{ 
		node->filecount++;
		if(node->maxfilecount > 1)
			node->proz = (int)ceil((((float)node->filecount / node->maxfilecount) * 100));
	}
	return 0;
}

//flag 0: copy file
//flag 1: move file
int copychr(char* from, char* to, struct copyfile* node, int flag)
{
	int ret = 0;
	struct stat64 s;
	
	if(from == NULL || to == NULL) return 1;

	ret = stat64(from, &s);
	if(ret != 0)
	{
		perr("get file status %s", from);
		return 1;
	}
			
	ret = tpkcreatechr("", to, major(s.st_rdev), minor(s.st_rdev), 0);
	if(ret != 0)
	{
		err("create chr dev");
		return 1;
	}
			
	if(flag == 1)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", from);
			return 1;
		}
	}

	if(node != NULL)
	{ 
		node->filecount++;
		if(node->maxfilecount > 1)
			node->proz = (int)ceil((((float)node->filecount / node->maxfilecount) * 100));
	}
	return 0;
}

//flag 0: copy file
//flag 1: move file
int copyfifo(char* from, char* to, struct copyfile* node, int flag)
{
	int ret = 0;
	
	if(from == NULL || to == NULL) return 1;
			
	ret = tpkcreatefifo("", to, 0);
	if(ret != 0)
	{
		err("create fifo");
		return 1;
	}
			
	if(flag == 1)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", from);
			return 1;
		}
	}

	if(node != NULL)
	{
		node->filecount++;
		if(node->maxfilecount > 1)
			node->proz = (int)ceil((((float)node->filecount / node->maxfilecount) * 100));
	}
	return 0;
}

//flag 0: copy file
//flag 1: move file
int copydir(char* dirfrom, char* dirto, struct copyfile* node, int first, int flag)
{
	DIR *d;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	int ret = 0;

	d = opendir(dirfrom); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", dirfrom);
		return 1;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length = 0;
		char pathfrom[PATH_MAX];
		char pathto[PATH_MAX];

		snprintf(pathfrom, PATH_MAX, "%s", dirfrom);
		snprintf(pathto, PATH_MAX, "%s", dirto);
		entry = readdir(d); //Readdir gets subsequent entries from d

		if(!entry) //no more entries , so break
			break;
			
		//for nfs mounts if file type is unknown use stat
		if(entry->d_type == DT_UNKNOWN)
		{
			tmpstr = ostrcat(dirfrom, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			entry->d_type = getlfiletype(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		free(tmpstr); tmpstr = NULL;
		if(entry->d_type == DT_DIR) //dir
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			//Check that the directory is not d or d's parent
			if(entry->d_name != NULL && ostrcmp(entry->d_name, ".") != 0 && ostrcmp(entry->d_name, "..") != 0)
			{
				path_length = snprintf(pathfrom, PATH_MAX, "%s/%s", dirfrom, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					break;
				}
				
				path_length = snprintf(pathto, PATH_MAX, "%s/%s", dirto, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					break;
				}

				ret = tpkcreatedir("", pathto, 0);
				if(ret != 0)
				{
					err("create dir %s", pathto);
					ret = 1;
					break;
				}
				
				if(node != NULL)
				{
					node->filecount++;
					if(node->maxfilecount > 1)
						node->proz = (int)ceil((((float)node->filecount / node->maxfilecount) * 100));
				}

				ret = copydir(pathfrom, pathto, node, 0, flag); //Recursively call with the new path
				if(ret != 0)
				{
					err("copydir %s", pathfrom);
					ret = 1;
					break;
				}
				
				if(flag == 1)
				{
					ret = rmdir(pathfrom);
					if(ret != 0 && errno != ENOENT)
					{
						perr("remove dir %s", pathfrom);
						ret = 1;
						break;
					}
				}
			}
		}
		else if(entry->d_type == DT_LNK) //link
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			tmpstr = ostrcat(tmpstr, pathfrom, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);

			tmpstr1 = ostrcat(tmpstr1, pathto, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);

			if(copylink(tmpstr, tmpstr, node, flag) != 0)
			{
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				ret = 1;
				break;
			}
			
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}
		else if(entry->d_type == DT_BLK) //block device
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			tmpstr = ostrcat(tmpstr, pathfrom, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			
			tmpstr1 = ostrcat(tmpstr1, pathto, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
			
			if(copyblk(tmpstr, tmpstr1, node, flag) != 0)
			{
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				ret = 1;
				break;
			}
			
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}
		else if(entry->d_type == DT_CHR) //charcter device
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			tmpstr = ostrcat(tmpstr, pathfrom, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			
			tmpstr1 = ostrcat(tmpstr1, pathto, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
			
			if(copychr(tmpstr, tmpstr1, node, flag) != 0)
			{
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				ret = 1;
				break;
			}
			
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}
		else if(entry->d_type == DT_FIFO) //fifo
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			tmpstr = ostrcat(tmpstr, pathfrom, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			
			tmpstr1 = ostrcat(tmpstr1, pathto, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
			
			if(copyfifo(tmpstr, tmpstr1, node, flag) != 0)
			{
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				ret = 1;
				break;
			}
			
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}
		else if(entry->d_type == DT_REG) //file
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			tmpstr = ostrcat(tmpstr, pathfrom, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			
			tmpstr1 = ostrcat(tmpstr1, pathto, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
			
			if(copyfilereal(tmpstr, tmpstr1, node, flag) != 0)
			{
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				ret = 1;
				break;
			}
			
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}
		else //unknown
		{
			if(node != NULL && node->stop == 1)
			{
				ret = 1;
				break;
			}

			err("unknown filetype found");
		}
	}

	if(d && closedir(d))
	{
		perr("Could not close %s", dirfrom);
		ret = 1;
	}

	return ret;
}

//flag 0: copy file
//flag 1: move file
int copyfile(char* from, char* to, struct copyfile* node, int flag)
{
	char* tmpto = NULL, *tmpstr = NULL, *bname = NULL;
	int ret = 1, fromtype = 0, totype = 0;
	
	if(from == NULL || to == NULL || ostrcmp(from, to) == 0)
	{
		if(node != NULL) node->ret = 1;
		return 1;
	}
	
	fromtype = getlfiletype(from);
	totype = getlfiletype(to);
	
	tmpto = ostrcat(to, NULL, 0, 0);
	
	//check if link is a dir
	if(totype == DT_LNK)
	{
		char* rpath = realpath(tmpto, NULL);
		if(rpath != NULL)
		{
			if(getlfiletype(rpath) == DT_DIR)
			{
				free(tmpto); tmpto = NULL;
				tmpto = ostrcat(rpath, NULL, 0, 0);
				totype = DT_DIR; 
			}
		}
		free(rpath); rpath = NULL;	
	}
	
	if((fromtype == DT_LNK || fromtype == DT_BLK || fromtype == DT_CHR || fromtype == DT_FIFO || fromtype == DT_REG) && totype == DT_DIR)
	{
		tmpstr = ostrcat(from, NULL, 0, 0);
		if(tmpstr != NULL)
			bname = basename(tmpstr);
		tmpto = ostrcat(tmpto, "/", 1, 0);
		tmpto = ostrcat(tmpto, bname, 1, 0);
		free(tmpstr); tmpstr = NULL;
		
		totype = fromtype;
	}
	
	if(fromtype == DT_DIR && totype == DT_DIR)
		ret = copydir(from, tmpto, node, 1, flag);
	else if(fromtype == DT_LNK && totype != DT_DIR)
		ret = copylink(from, tmpto, node, flag);	
	else if(fromtype == DT_BLK && totype != DT_DIR)
		ret = copyblk(from, tmpto, node, flag);	
	else if(fromtype == DT_CHR && totype != DT_DIR)
		ret = copychr(from, tmpto, node, flag);
	else if(fromtype == DT_FIFO && totype != DT_DIR)
		ret = copyfifo(from, tmpto, node, flag);	
	else if(fromtype == DT_REG && totype != DT_DIR)
		ret = copyfilereal(from, tmpto, node, flag);
	else
		err("copy not allowed");
	
	free(tmpto); tmpto = NULL;
	if(node != NULL) node->ret = ret;
	return ret;
}

void copyfilestruct(struct stimerthread* timernode, struct copyfile* node, int flag)
{
	if(node != NULL)
		copyfile(node->from, node->to, node, 0);
}

void movefilestruct(struct stimerthread* timernode, struct copyfile* node, int flag)
{
	if(node != NULL)
		copyfile(node->from, node->to, node, 1);
}

//flag 0: copy
//flag 1: move
int screencopy(char* title, char* from, char* to, int flag)
{
	int rcret = -1, count = 0, ret = 0, fromthread = 0, sleeptime = 2;
	struct skin* copyfile = getscreen("copyfile");
	struct skin* progress = getscreennode(copyfile, "progress");
	struct skin* filefrom = getscreennode(copyfile, "filefrom");
	struct skin* fileto = getscreennode(copyfile, "fileto");
	struct skin* maxkb = getscreennode(copyfile, "maxkb");
	struct skin* aktkb = getscreennode(copyfile, "aktkb");
	struct skin* filecount = getscreennode(copyfile, "filecount");
	struct skin* framebuffer = getscreen("framebuffer");
	char* bg = NULL, *tmpstr = NULL;
	struct copyfile* cnode = NULL;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	if(from == NULL || to == NULL)
	{
		err("NULL detect");
		return 1;
	}

	changetitle(copyfile, title);
	changetext(filefrom, from);
	changetext(fileto, to);
	progress->progresssize = 0;
	changetext(maxkb, NULL);
	changetext(aktkb, NULL);
	changetext(filecount, NULL);

	if(fromthread == 1)
	{
		delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
		delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute", NULL), NULL, NULL);
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(copyfile, framebuffer, 2);
		status.rcowner = copyfile;
		bg = savescreen(copyfile);
		drawscreen(copyfile, 0, 2);
	}
	else
		drawscreen(copyfile, 0, 0);

	cnode = calloc(1, sizeof(struct copyfile));
	if(cnode == NULL)
	{
		err("no mem");
		return 1;
	}

	cnode->from = from;
	cnode->to = to;
	cnode->ret = -1;
	cnode->stop = 0;
	cnode->filecount = 0;
	cnode->maxfilecount = 1;
	
	ret = countfiles(from, &count, 1);
	if(ret == 0)
		cnode->maxfilecount = count;
	
	if(flag == 1)
		addtimer(&movefilestruct, START, 1000, 1, (void*)cnode, NULL, NULL);
	else
		addtimer(&copyfilestruct, START, 1000, 1, (void*)cnode, NULL, NULL);

	while(1)
	{
		rcret = waitrc(copyfile, 1000, 0);

		if(rcret == RCTIMEOUT)
		{
			progress->progresssize = cnode->proz;
			
			if(cnode->maxkb < 1024)
				tmpstr = ostrcat(_("Filesize (B): "), oitoa(cnode->maxkb), 0, 1);
			else if(cnode->maxkb < 1048576)
				tmpstr = ostrcat(_("Filesize (KB): "), oitoa(cnode->maxkb / 1024), 0, 1);
			else
				tmpstr = ostrcat(_("Filesize (MB): "), oitoa(cnode->maxkb / 1024 / 1024), 0, 1);
			changetext(maxkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			if(flag == 1)
			{
				if(cnode->aktkb < 1024)
					tmpstr = ostrcat(_("Move (B): "), oitoa(cnode->aktkb), 0, 1);
				else if(cnode->aktkb < 1048576)
					tmpstr = ostrcat(_("Move (KB): "), oitoa(cnode->aktkb / 1024), 0, 1);
				else
					tmpstr = ostrcat(_("Move (MB): "), oitoa(cnode->aktkb / 1024 / 1024), 0, 1);
			}
			else
			{
				if(cnode->aktkb < 1024)
					tmpstr = ostrcat(_("Copy (B): "), oitoa(cnode->aktkb), 0, 1);
				else if(cnode->aktkb < 1048576)
					tmpstr = ostrcat(_("Copy (KB): "), oitoa(cnode->aktkb / 1024), 0, 1);
				else
					tmpstr = ostrcat(_("Copy (MB): "), oitoa(cnode->aktkb / 1024 / 1024), 0, 1);
			}
			changetext(aktkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			tmpstr = ostrcat(_("Amount of files: "), oitoa(cnode->filecount), 0, 1);
			tmpstr = ostrcat(tmpstr, " / ", 1, 0);
			tmpstr = ostrcat(tmpstr, oitoa(cnode->maxfilecount), 1, 1);
			changetext(filecount, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		drawscreen(copyfile, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(cnode->ret == 0)
		{
			progress->progresssize = cnode->proz;
			
			if(flag == 1)
			{
				if(cnode->aktkb < 1024)
					tmpstr = ostrcat(_("Move (B): "), oitoa(cnode->aktkb), 0, 1);
				else if(cnode->aktkb < 1048576)
					tmpstr = ostrcat(_("Move (KB): "), oitoa(cnode->aktkb / 1024), 0, 1);
				else
					tmpstr = ostrcat(_("Move (MB): "), oitoa(cnode->aktkb / 1024 / 1024), 0, 1);
			}
			else
			{
				if(cnode->aktkb < 1024)
					tmpstr = ostrcat(_("Copy (B): "), oitoa(cnode->aktkb), 0, 1);
				else if(cnode->aktkb < 1048576)
					tmpstr = ostrcat(_("Copy (KB): "), oitoa(cnode->aktkb / 1024), 0, 1);
				else
					tmpstr = ostrcat(_("Copy (MB): "), oitoa(cnode->aktkb / 1024 / 1024), 0, 1);
			}
			changetext(aktkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			break;
		}
		if(cnode->ret > 0) break;
	}

	if(cnode->ret > 0 && rcret != getrcconfigint("rcexit", NULL))
	{	
		if(flag == 1)
		{
			changetext(filefrom, _("file move error"));
			changetext(fileto, _("file move error"));
		}
		else
		{
			changetext(filefrom, _("file copy error"));
			changetext(fileto, _("file copy error"));
		}
		sleeptime = 5;
	}
	else
	{
		changetext(filefrom, _("wait for stopping"));
		changetext(fileto, _("wait for stopping"));
	}
	drawscreen(copyfile, 0, 0);
	cnode->stop = 1;
	sleep(sleeptime);
	count = 0;
	while(cnode->ret < 0 && count < 10)
	{
		count++;
		sleep(1);
	}
	ret = cnode->ret;
	
	if(count < 10)
	{
		free(cnode);
		cnode = NULL;
	}
	else
		addoldentry((void*)cnode, 2, time(NULL) + 7200, NULL);

	if(fromthread == 1)
	{
		clearscreennolock(copyfile);
		restorescreen(bg, copyfile);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 10);
		m_unlock(&status.drawingmutex, 0);
		addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
		addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
		addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
	}
	else
	{
		clearscreen(copyfile);
		drawscreen(skin, 0, 0);
	}
	return ret;
}

#endif
