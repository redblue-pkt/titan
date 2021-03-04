#ifndef TPK_H
#define TPK_H

#ifndef TPKFILELIST
#define TPKFILELIST "/tmp/filelist.tpk"
#endif

#ifndef PREVIEWFILELIST
#define PREVIEWFILELIST "/tmp/filelist.preview"
#endif

#define TPKUSEBACKUP 1
#define TPKLOG "/tmp/tpk.log"
#define FEEDFILE "/etc/ipkg/official-feed.conf"
#define PREDIR "/etc/tpk.restore"
#define TMP "/tmp/tpk"
#define TMPALLPACKAGES TMP"/AllPackages"
#define TMPPACKAGES TMP"/Packages"
#define TMPPREVIEW TMP"/Packages.preview"
#define EXTRACTDIR "/mnt/tpk" //path must exist
#define PREFILE "/var/etc/.tpkpre"
#define TPKADDSIZE 100
#define TPKZIPALL 0

#define HTTPPACKAGES "Packages.gz"
#define HTTPPREVIEW "Packages.preview.gz"

int tpkremove(char* file, int restore, int flag);
int tpkgetpackage(char* package, char* url, char* installpath, int flag, int flag1);
int tpklistinstalled(int flag);
int tpkcreatefilereal(char* mainpath, char* from, char* to, off64_t start, off64_t len, char* name, int flag);

struct tpk
{
	char* name;
	char* filename;
	char* desc;
	char* section;
	char* showname;
	char* arch;
	char* titanname;
	int version;
	int group;
	int minversion;
	int preinstalled;
	int status;
	int done;
	char* url;
	int size;
	char* installpath;
	char* usepath;
	char* boxtype;
	struct tpk* prev;
	struct tpk* next;
};

struct tpk *tpk = NULL;

void debugtpk()
{
	struct tpk *node = tpk;

	while(node != NULL)
	{
		if(node->name != NULL)
			printf("pkg name: %s\n", node->name);
		if(node->desc != NULL)
			printf("pkg desc: %s\n", node->desc);
		if(node->section != NULL)
			printf("pkg section: %s\n", node->section);
		if(node->showname != NULL)
			printf("pkg showname: %s\n", node->showname);
		if(node->url != NULL)
			printf("pkg url: %s\n", node->url);

		printf("pkg version: %d\n", node->version);
		printf("pkg group: %d\n", node->group);
		printf("pkg minversion: %d\n", node->minversion);
		printf("pkg preinstalled: %d\n", node->preinstalled);
		node = node->next;
	}
}

struct tpk* addtpk(char *name, char* desc, char* section, char* showname, char* arch, char* filename, char* titanname, int version, int group, int minversion, int preinstalled, char* url, int size, char* installpath, char* usepath, char* boxtype, struct tpk* last)
{
	struct tpk *newnode = NULL, *prev = NULL, *node = tpk;

	newnode = (struct tpk*)calloc(1, sizeof(struct tpk));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->name = strstrip(ostrcat(name, NULL, 0, 0));
	newnode->desc = strstrip(ostrcat(desc, NULL, 0, 0));
	newnode->section = strstrip(ostrcat(section, NULL, 0, 0));
	newnode->arch = strstrip(ostrcat(arch, NULL, 0, 0));
	newnode->url = strstrip(ostrcat(url, NULL, 0, 0));
	newnode->filename = strstrip(ostrcat(filename, NULL, 0, 0));
	newnode->titanname = strstrip(ostrcat(titanname, NULL, 0, 0));
	if(showname == NULL || showname[0] == '*')
		newnode->showname = strstrip(ostrcat(name, NULL, 0, 0));
	else
		newnode->showname = strstrip(ostrcat(showname, NULL, 0, 0));
	newnode->version = version;
	newnode->group = group;
	newnode->minversion = minversion;
	newnode->preinstalled = preinstalled;
	newnode->size = size;
	newnode->installpath = strstrip(ostrcat(installpath, NULL, 0, 0));
	newnode->usepath = strstrip(ostrcat(usepath, NULL, 0, 0));
	newnode->boxtype = strstrip(ostrcat(boxtype, NULL, 0, 0));

	if(last == NULL)
	{
		while(node != NULL && strcasecmp(_(newnode->section), _(node->section)) > 0)
		{
			prev = node;
			node = node->next;
		}
		while(node != NULL && strcasecmp(_(newnode->section), _(node->section)) == 0 && strcasecmp(newnode->showname, node->showname) > 0)
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
		tpk = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	return newnode;
}

void deltpk(struct tpk* tpknode)
{
	struct tpk *node = tpk, *prev = tpk;

	while(node != NULL)
	{
		if(node == tpknode)
		{
			if(node == tpk)
			{
				tpk = node->next;
				if(tpk != NULL)
					tpk->prev = NULL;
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

			free(node->section);
			node->section = NULL;

			free(node->showname);
			node->showname = NULL;

			free(node->arch);
			node->arch = NULL;

			free(node->url);
			node->url = NULL;

			free(node->filename);
			node->filename = NULL;

			free(node->titanname);
			node->titanname = NULL;

			free(node->installpath);
			node->installpath = NULL;

			free(node->usepath);
			node->usepath = NULL;

			free(node->boxtype);
			node->boxtype = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freetpk()
{
	struct tpk *node = tpk, *prev = tpk;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltpk(prev);
	}
}

int tpkcreateflagfile(char* path, char* file)
{
	int ret = 0;
	FILE* fd = NULL;
	char* tmpstr = NULL;

	if(path == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(file == NULL)
		tmpstr = ostrcat(tmpstr, path, 1, 0);
	else
	{
		tmpstr = ostrcat(tmpstr, path, 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, file, 1, 0);
	}

	fd = fopen(tmpstr, "w");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		ret = 1;
	}

	free(tmpstr); tmpstr = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

//flag 0: normal filelist
//flag 1: preview filelist
int tpkcreatefilelist(char* mainpath, char* to, char* from, int type, off64_t endpos, off64_t len, int maj, int min, int flag, char* name)
{
	int ret = 0, writeret = 0;
	FILE *fd = NULL;
	char* file = TPKFILELIST, *tmpstr = NULL;

	if(flag == 1)
		file = PREVIEWFILELIST;

	if(to == NULL || from == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(strlen(to) > strlen(mainpath))
		to += strlen(mainpath);

	if(strlen(from) > strlen(mainpath))
		from += strlen(mainpath);

	if(to == NULL || from == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	fd = fopen(file, "a");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		ret = 1;
		goto end;
	}

	if(flag == 1)
	{
		tmpstr = ostrcat(tmpstr, TMP, 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, name, 1, 0);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		writeret = fprintf(fd, "%s#%s#%d#%lli#%lli#%d#%d\n", tmpstr, from, type, endpos, len, maj, min);
		free(tmpstr); tmpstr = NULL;
	}
	else
		writeret = fprintf(fd, "%s#%s#%d#%lli#%lli#%d#%d\n", to, from, type, endpos, len, maj, min);
	if(writeret < 0)
	{
		perr("writting file %s", file);
		ret = 1;
		goto end;
	}

end:
	if(fd != NULL) fclose(fd);
	return ret;
}

//flag 0: extract archive
//flag 1: create archive
//flag 2: create archive and write startpos + size to end
//flag 3: same as 1 but write preview filelist
//flag 4: same as 0 but append
int tpkcreatefile(char* mainpath, char* from, char* to, off64_t start, off64_t len, int flag)
{
	int ret = 0;

	ret = tpkcreatefilereal(mainpath, from, to, start, len, NULL, flag);
	return ret;
}

int tpkcreatefilereal(char* mainpath, char* from, char* to, off64_t start, off64_t len, char* name, int flag)
{
	int fdfrom = -1, fdto = -1, ret = 0, readret = 0, writeret = 0;
	off64_t count = 0, endpos = 0;
	unsigned char* buf = NULL;

	if(mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	fdfrom = open(from, O_RDONLY);
	if(fdfrom < 0)
	{
		perr("open from %s", from);
		ret = 1;
		goto end;
	}

	if(flag == 0)
	{
		ret = unlink(to);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", to);
			ret = 1;
			goto end;
		}
		else
			ret = 0;
	}

	if(flag == 1 || flag == 2 || flag == 3 || flag == 4)
		fdto = open(to, O_CREAT | O_WRONLY | O_APPEND | O_LARGEFILE, 0777);
	else
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

	if(len == -1)
	{
		len = lseek64(fdfrom, 0, SEEK_END);
		if(len < 0)
		{
			perr("can't get filelen %s", from);
			ret = 1;
			goto end;
		}
	}

	if(flag == 1 || flag == 2 || flag == 3)
	{
		endpos = lseek64(fdto, 0, SEEK_END);
		if(len < 0)
		{
			perr("can't get endpos %s", to);
			ret = 1;
			goto end;
		}
	}

	if(lseek64(fdfrom, start, SEEK_SET) < 0)
	{
		perr("can't seek to startpos %s", from);
		ret = 1;
		goto end;
	}

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

		writeret = dvbwrite(fdto, buf, readret, -1);
		if(writeret != readret)
		{
			err("write file %s", to);
			ret = 1;
			goto end;
		}

		count += readret;
		if(count == len) break;
		if(count > len)
		{
			err("write more then filelen %s", to);
			ret = 1;
			goto end;
		}
	}

	if(flag == 1 || flag == 3)
	{
		if(flag == 1)
			writeret = tpkcreatefilelist(mainpath, from, "*", DT_REG, endpos, len, 0, 0, 0, NULL);
		else if(flag == 3)
			writeret = tpkcreatefilelist(mainpath, from, "*", DT_REG, endpos, len, 0, 0, 1, name);
		if(writeret < 0)
		{
			err("create filelist %s", from);
			ret = 1;
			goto end;
		}
	}

	if(flag == 2)
	{
		writeret = dvbwrite(fdto, (unsigned char*)&endpos, sizeof(off64_t), -1);
		if(writeret < 0)
		{
			err("write file %s", to);
			ret = 1;
			goto end;
		}

		writeret = dvbwrite(fdto, (unsigned char*)&len, sizeof(off64_t), -1);
		if(writeret < 0)
		{
			err("write file %s", to);
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
		if(flag == 0 && ret == 1) unlink(to);
	}
	free(buf);

	return ret;
}

//flag 0: extract archive
//flag 1: create archive
int tpkcreatedir(char* mainpath, char* path, int flag)
{
	int writeret = 0, ret = 0;

	if(path == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(flag == 0)
	{
		ret = mkdir(path, 0777);
		if(ret != 0 && errno != EEXIST)
		{
			perr("create path %s", path);
			ret = 1;
			goto end;
		}
		else
			ret = 0;
	}

	if(flag == 1)
	{
		writeret = tpkcreatefilelist(mainpath, path, "*", DT_DIR, 0, 0, 0, 0, 0, NULL);
		if(writeret < 0)
		{
			err("create filelist %s", path);
			ret = 1;
			goto end;
		}
	}

end:
	return ret;
}

//flag 0: extract archive
//flag 1: create archive
int tpkcreatelink(char* mainpath, char* from, char* to, int flag)
{
	int writeret = 0, ret = 0;

	if(from == NULL || to == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(flag == 0)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove symlink %s -> %s", from, to);
			ret = 1;
			goto end;
		}
		else
			ret = 0;

		ret = symlink(to, from);
		if(ret != 0)
		{
			perr("create symlink %s -> %s", from, to);
			ret = 1;
			goto end;
		}
	}

	if(flag == 1)
	{
		writeret = tpkcreatefilelist(mainpath, from, to, DT_LNK, 0, 0, 0, 0, 0, NULL);
		if(writeret < 0)
		{
			err("create filelist %s -> %s", from, to);
			ret = 1;
			goto end;
		}
	}

end:
	return ret;
}

//flag 0: extract archive
//flag 1: create archive
int tpkcreatefifo(char* mainpath, char* file, int flag)
{
	int writeret = 0, ret = 0;

	if(file == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(flag == 0)
	{
		ret = unlink(file);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove fifo %s", file);
			ret = 1;
			goto end;
		}
		else
			ret = 0;

		ret = mkfifo(file, 0777);
		if(ret != 0)
		{
			perr("create fifo %s", file);
			ret = 1;
			goto end;
		}
	}

	if(flag == 1)
	{
		writeret = tpkcreatefilelist(mainpath, file, "*", DT_FIFO, 0, 0, 0, 0, 0, NULL);
		if(writeret < 0)
		{
			err("create filelist %s", file);
			ret = 1;
			goto end;
		}
	}

end:
	return ret;
}

//flag 0: extract archive
//flag 1: create archive
int tpkcreatechr(char* mainpath, char* file, int maj, int min, int flag)
{
	int writeret = 0, ret = 0;

	if(file == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(flag == 0)
	{
		ret = unlink(file);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove chr dev %s", file);
			ret = 1;
			goto end;
		}
		else
			ret = 0;

		ret = mknod(file, S_IFCHR|0777, makedev(maj, min));
		if(ret != 0)
		{
			perr("create chr dev %s", file);
			ret = 1;
			goto end;
		}
	}

	if(flag == 1)
	{
		writeret = tpkcreatefilelist(mainpath, file, "*", DT_CHR, 0, 0, maj, min, 0, NULL);
		if(writeret < 0)
		{
			err("create filelist %s", file);
			ret = 1;
			goto end;
		}
	}

end:
	return ret;
}

//flag 0: extract archive
//flag 1: create archive
int tpkcreateblk(char* mainpath, char* file, int maj, int min, int flag)
{
	int writeret = 0, ret = 0;

	if(file == NULL || mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(flag == 0)
	{
		ret = unlink(file);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove blk dev %s", file);
			ret = 1;
			goto end;
		}
		else
			ret = 0;

		ret = mknod(file, S_IFBLK|0777, makedev(maj, min));
		if(ret != 0)
		{
			perr("create blk dev %s", file);
			ret = 1;
			goto end;
		}
	}

	if(flag == 1)
	{
		writeret = tpkcreatefilelist(mainpath, file, "*", DT_BLK, 0, 0, maj, min, 0, NULL);
		if(writeret < 0)
		{
			err("create filelist %s", file);
			ret = 1;
			goto end;
		}
	}

end:
	return ret;
}

int tpkcleanworkdir(char* dir)
{
	DIR *d;
	int ret = 0;

	d = opendir(dir); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", dir);
		ret = 1;
		goto end;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dir);
		entry = readdir(d); //gets subsequent entries from d

		if(!entry) //no more entries, so break
			break;

		if(!(entry->d_type == DT_DIR))
		{
			if(entry->d_name != NULL)
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dir, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					goto end;
				}

				ret = unlink(path);
				if(ret != 0 && errno != ENOENT)
				{
					perr("removing %s", path);
					ret = 1;
					goto end;
				}
				else
					ret = 0;
			}
		}
	}

end:
	if(d && closedir(d))
	{
		perr("Could not close %s", dir);
		ret = 1;
	}

	return ret;
}

//flag 0: extract archive
//flag 1: create archive
struct tpk* tpkreadcontrol(char* path, int flag)
{
	int len = 0;
	FILE *fd = NULL;
	char* fileline = NULL, *tmpstr = NULL;
	char* showname = NULL, *section = NULL, *desc = NULL, *packagename = NULL, *arch = NULL, *filename = NULL, *titanname = NULL, *installpath = NULL, *usepath = NULL, *boxtype = NULL;
	int version = 0, group = 0, minversion = 0, preinstalled = 0, size = 0;
	struct tpk* tpknode = NULL;

	if(path == NULL) return NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		goto end;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	if(flag == 1)
		tmpstr = ostrcat(tmpstr, "CONTROL/control", 1, 0);
	else
		tmpstr = ostrcat(tmpstr, "control", 1, 0);

	fd = fopen(tmpstr, "r");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		if(strstr(fileline, "Package: ") == fileline)
			packagename = ostrcat(fileline + 9, NULL, 0, 0);
		if(strstr(fileline, "Architecture: ") == fileline)
			arch = ostrcat(fileline + 14, NULL, 0, 0);
		if(strstr(fileline, "Showname: ") == fileline)
			showname = ostrcat(fileline + 10, NULL, 0, 0);
		if(strstr(fileline, "Version: ") == fileline)
		{
			tmpstr = ostrcat(fileline + 9, NULL, 0, 0);
			if(tmpstr != NULL) version = atoi(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(strstr(fileline, "Section: ") == fileline)
			section = ostrcat(fileline + 9, NULL, 0, 0);
		if(strstr(fileline, "Description: ") == fileline)
			desc = ostrcat(fileline + 13, NULL, 0, 0);
		if(strstr(fileline, "Group: ") == fileline)
		{
			tmpstr = ostrcat(fileline + 7, NULL, 0, 0);
			if(tmpstr != NULL) group = atoi(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(strstr(fileline, "Minversion: ") == fileline)
		{
			tmpstr = ostrcat(fileline + 12, NULL, 0, 0);
			if(tmpstr != NULL) minversion = atoi(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(strstr(fileline, "Preinstalled: ") == fileline)
		{
			tmpstr = ostrcat(fileline + 14, NULL, 0, 0);
			if(tmpstr != NULL) preinstalled = atoi(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(strstr(fileline, "Size: ") == fileline)
		{
			tmpstr = ostrcat(fileline + 6, NULL, 0, 0);
			if(tmpstr != NULL) size = atoi(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(strstr(fileline, "Titanname: ") == fileline)
			titanname = ostrcat(fileline + 11, NULL, 0, 0);
		if(strstr(fileline, "Installpath: ") == fileline)
			installpath = ostrcat(fileline + 13, NULL, 0, 0);
		if(strstr(fileline, "Usepath: ") == fileline)
			usepath = ostrcat(fileline + 9, NULL, 0, 0);
		if(strstr(fileline, "Boxtype: ") == fileline)
			boxtype = ostrcat(fileline + 9, NULL, 0, 0);
	}

	if(section == NULL) section = ostrcat("extra", NULL, 0, 0);
	if(desc == NULL) desc = ostrcat("*", NULL, 0, 0);
	if(showname == NULL) showname = ostrcat("*", NULL, 0, 0);
	if(arch == NULL) arch = ostrcat("noarch", NULL, 0, 0);
	if(titanname == NULL) titanname = ostrcat("*", NULL, 0, 0);
	if(installpath == NULL) installpath = ostrcat("*", NULL, 0, 0);
	if(usepath == NULL) usepath = ostrcat("*", NULL, 0, 0);
	if(boxtype == NULL) boxtype = ostrcat("*", NULL, 0, 0);

	filename = ostrcat(packagename, "_", 0, 0);
	filename = ostrcat(filename, oitoa(version), 1, 1);
	filename = ostrcat(filename, "_", 1, 0);
	filename = ostrcat(filename, arch, 1, 0);

	tpknode = addtpk(packagename, desc, section, showname, arch, filename, titanname, version, group, minversion, preinstalled, NULL, size, installpath, usepath, boxtype, NULL);

end:
	free(showname); showname = NULL;
	free(section); section = NULL;
	free(desc); desc = NULL;
	free(packagename); packagename = NULL;
	free(arch); arch = NULL;
	free(filename); filename = NULL;
	free(titanname); titanname = NULL;
	free(installpath); installpath = NULL;
	free(titanname); titanname = NULL;
	free(usepath); usepath = NULL;
	free(boxtype); boxtype = NULL;
	if(fd != NULL) fclose(fd);
	return tpknode;
}

int tpkrevertfile(char* file)
{
	int ret = 0, count = 0, max = 0, i = 0;
	FILE *fd = NULL;
	char* fileline = NULL;

	fileline = calloc(1, MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return 1;
	}

	fd = fopen(file, "r");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		free(fileline); fileline = NULL;
		return 1;
	}

	//count lines
	while(fgets(fileline, MINMALLOC, fd) != NULL)
		count++;

	//create buffer
	max = count;
	char* buf[count];
	for(i = 0; i < max; i++)
		buf[i] = NULL;

	ret = fseek(fd, 0, SEEK_SET);
	if(ret < 0)
	{
		perr("seek to startpos");
		ret = 1;
		goto end;
	}
	else
		ret = 0;

	//read lines revert into buffer
	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(strstr(fileline, "/CONTROL") == fileline) continue;
		count--;
		buf[count] = ostrcat(fileline, NULL, 0, 0);
	}

	if(fd != NULL) fclose(fd);

	fd = fopen(file, "w");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		free(fileline); fileline = NULL;
		return 1;
	}

	for(i = 0; i < max; i++)
	{
		if(buf[i] == NULL) continue;
		ret = fprintf(fd, "%s", buf[i]);
		if(ret < 0)
		{
			perr("writting restorefile");
			ret = 1;
			goto end;
		}
		else
			ret = 0;
	}

end:
	for(i = 0; i < max; i++)
	{
		free(buf[i]);
		buf[i] = NULL;
	}

	free(fileline); fileline = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

#ifndef TPKCREATE

//flag 0: zlib
//flag 1: gzip
int tpkcreatefilegz(char* from, char* to, off64_t start, off64_t len, int flag)
{
	int fdfrom = -1, fdto = -1, ret = 0, readret = 0, writeret = 0;
	off64_t count = 0;
	unsigned char bufin[MINMALLOC * 4] = {'\0'};
	unsigned char bufout[MINMALLOC * 4] = {'\0'};
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	stream.avail_out = 0;
	stream.next_out = Z_NULL;

	if(flag == 1)
		ret = inflateInit2(&stream, 16 + MAX_WBITS);
	else
		ret = inflateInit(&stream);
	if(ret != Z_OK)
		return 1;

	fdfrom = open(from, O_RDONLY);
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

	if(len == -1)
	{
		len = lseek64(fdfrom, 0, SEEK_END);
		if(len < 0)
		{
			perr("can't get filelen %s", from);
			ret = 1;
			goto end;
		}
	}

	if(lseek64(fdfrom, start, SEEK_SET) < 0)
	{
		perr("can't seek to startpos %s", from);
		ret = 1;
		goto end;
	}

	while(len != 0)
	{
		if(len - count > (MINMALLOC * 4))
			readret = dvbreadfd(fdfrom, bufin, 0, MINMALLOC * 4, -1, 1);
		else
			readret = dvbreadfd(fdfrom, bufin, 0, len - count, -1, 1);
		if(readret <= 0)
		{
			err("read file %s", from);
			ret = 1;
			goto end;
		}

		stream.avail_in = readret;
		stream.next_in = (void*)bufin;

		do
		{
			stream.avail_out = MINMALLOC * 4;
			stream.next_out = bufout;

			ret = inflate(&stream, Z_NO_FLUSH);
			if(ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
			{
				err("unzip file %s", from);
				ret = 1;
				goto end;
			}
			else
				ret = 0;

			int have = (MINMALLOC * 4) - stream.avail_out;

			writeret = dvbwrite(fdto, bufout, have, -1);
			if(writeret != have)
			{
				err("write file %s", to);
				ret = 1;
				goto end;
			}
		}
		while (stream.avail_out == 0);

		count += readret;
		if(count == len) break;
		if(count > len)
		{
			err("write more then filelen %s", to);
			ret = 1;
			goto end;
		}
	}

end:
	(void)inflateEnd(&stream);

	if(fdfrom >= 0)
		close(fdfrom);
	if(fdto >= 0)
	{
		close(fdto);
		if(ret == 1) unlink(to);
	}

	return ret;
}

int tpkchecksize(struct tpk* tpknode, char* installpath, int size)
{
	int tpksize = 0;

	if(installpath == NULL || installpath[0] == '*') return 0;

	if(tpknode != NULL)
		tpksize = tpknode->size;
	else
		tpksize = size;

	if(tpksize != 0)
	{
		unsigned long long freesize = getfreespace(installpath) / 1024;
		if(tpksize + TPKADDSIZE >= freesize)
		{
			err("size to big for %s %d -> %lld", installpath, tpksize, freesize);
			return 1;
		}
	}

	return 0;
}

int tpkgettail(char* file, off64_t* startpos, off64_t* len)
{
	int fd = -1, ret = 0;

	fd = open(file, O_RDONLY);
	if(fd < 0)
	{
		perr("open %s", file);
		ret = 1;
		goto end;
	}

	int pos = (sizeof(off64_t) + sizeof(off64_t)) * -1;
	ret = lseek64(fd, pos, SEEK_END);
	if(ret < 0)
	{
		perr("can't get endpos %s", file);
		ret = 1;
		goto end;
	}
	else
		ret = 0;

	ret = dvbreadfd(fd, (unsigned char*)startpos, 0, sizeof(off64_t), -1, 1);
	if(ret <= 0)
	{
		err("read file %s", file);
		ret = 1;
		goto end;
	}
	else
		ret = 0;

	ret = dvbreadfd(fd, (unsigned char*)len, 0, sizeof(off64_t), -1, 1);
	if(ret <= 0)
	{
		err("read file %s", file);
		ret = 1;
		goto end;
	}
	else
		ret = 0;

end:
	close(fd);
	return ret;
}

//flag 0: normal
//flag 1: don't del dir
int tpkdel(char* path, int restore, int flag)
{
	int ret = 0, len = 0, newtype = 0, oldtype = 0, maj = 0, min = 0, exist = 0;
	FILE *fd = NULL;
	char* fileline = NULL, *from = NULL, *to = NULL, *tmpstr = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	from = calloc(1, MINMALLOC);
	if(from == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	to = calloc(1, MINMALLOC);
	if(to == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
	fd = fopen(tmpstr, "r");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		ret = sscanf(fileline, "%[^#]#%[^#]#%d#%d#%d#%d#%d", to, from, &newtype, &oldtype, &exist, &maj, &min);
		if(ret != 7)
		{
			err("read restore file");
			continue;
		}
		ret = 0;

		if(flag == 1)
		{
			oldtype = -1;
			exist = 1;
		}

		if(newtype == DT_REG || newtype == DT_LNK || newtype == DT_BLK || newtype == DT_CHR || newtype == DT_FIFO)
		{
			ret = unlink(to);
			if(ret != 0 && errno != ENOENT)
			{
				debug(130, "can't remove file %s", to);
			}
			ret = 0;
		}
		else if(newtype == DT_DIR)
		{
			if(exist == 0)
			{
				ret = rmdir(to);
				if(ret != 0 && errno != ENOENT)
				{
					debug(130, "can't remove dir %s", to);
				}
				ret = 0;
			}
		}
		else
		{
			ret = unlink(to);
			{
				if(ret != 0 && errno != ENOENT)
				{
					ret = rmdir(to);
					if(ret != 0 && errno != ENOENT)
					{
						debug(130, "can't remove file/dir %s", to);
					}
				}
			}
			ret = 0;
		}

		if(oldtype == DT_REG) //file
		{
			if(restore == 1 && from[0] != '*')
				tpkcreatefile("", from, to, 0, -1, 0);
		}
		else if(oldtype == DT_LNK) //link
		{
			if(restore == 1)
				tpkcreatelink("", to, from, 0);
		}
		else if(oldtype == DT_BLK) //block device
		{
			if(restore == 1)
				tpkcreateblk("", to, maj, min, 0);
		}
		else if(oldtype == DT_CHR) //charcter device
		{
			if(restore == 1)
				tpkcreatechr("", to, maj, min, 0);
		}
		else if(oldtype == DT_FIFO) //fifo
		{
			if(restore == 1)
				tpkcreatefifo("", to, 0);
		}
	}

end:
	free(fileline); fileline = NULL;
	free(from); from = NULL;
	free(to); to = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkdelbackup(char* path)
{
	int ret = 0, len = 0, newtype = 0, oldtype = 0, maj = 0, min = 0, exist = 0;
	FILE *fd = NULL;
	char* fileline = NULL, *from = NULL, *to = NULL, *tmpstr = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	from = calloc(1, MINMALLOC);
	if(from == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	to = calloc(1, MINMALLOC);
	if(to == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
	fd = fopen(tmpstr, "r");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		ret = sscanf(fileline, "%[^#]#%[^#]#%d#%d#%d#%d#%d", to, from, &newtype, &oldtype, &exist, &maj, &min);
		if(ret != 7)
		{
			err("read restore file");
			continue;
		}
		ret = 0;

		if(oldtype == DT_REG && from[0] != '*')
			unlink(from);
	}

end:
	free(fileline); fileline = NULL;
	free(from); from = NULL;
	free(to); to = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkwriterestore(char* path, char* to, int newtype, int oldtype, int exist)
{
	int ret = 0, maj = 0, min = 0;
	FILE *fd = NULL;
	char* tmpstr = NULL, *from = NULL;

	//check free space in /tmp for backup files
	//if(getfreespace("/tmp") < 1 * 1024 * 1024)
	//{
	//	err("no free space in /tmp");
	//	return 1;
	//}

	if(to == NULL)
	{
		err("NULL detect");
		return 1;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
	fd = fopen(tmpstr, "a");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	if(TPKUSEBACKUP == 1 && exist == 1)
	{
		if(oldtype == DT_REG)
		{
			from = ostrcat(from, tmpnam(NULL), 1, 0);
			ret = tpkcreatefile("", to, from, 0, -1, 0);
			if(ret != 0)
			{
				err("create file %s", from);
				ret = 1;
				goto end;
			}
		}
		else if(oldtype == DT_LNK) //link
		{
			char* buf = calloc(1, MINMALLOC);
			if(buf == NULL)
			{
				err("no mem");
				ret = 1;
				goto end;
			}

			ret = readlink(to, buf, MINMALLOC);
			if(ret < 0)
			{
				perr("read link %s", to);
				free(buf); buf = NULL;
				ret = 1;
				goto end;
			}

			from = ostrcat(buf, NULL, 0, 0);
			free(buf); buf = NULL;
		}
		else if(oldtype == DT_BLK) //block device
		{
			struct stat64 s;
			ret = stat64(to, &s);
			if(ret != 0)
			{
				perr("get file status %s", to);
				return 1;
			}
			maj = major(s.st_rdev);
			min = minor(s.st_rdev);
		}
		else if(oldtype == DT_CHR) //charcter device
		{
			struct stat64 s;
			ret = stat64(to, &s);
			if(ret != 0)
			{
				perr("get file status %s", to);
				return 1;
			}
			maj = major(s.st_rdev);
			min = minor(s.st_rdev);
		}
	}

	if(from == NULL) from = ostrcat("*", NULL, 0, 0);

	ret = fprintf(fd, "%s#%s#%d#%d#%d#%d#%d\n", to, from, newtype, oldtype, exist, maj, min);
	if(ret < 0)
	{
		perr("writting restorefile");
		ret = 1;
		goto end;
	}
	else
		ret = 0;

end:
	free(from); from = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

//flag 0: extract Control
//flag 1: extract other
int tpkextractfilelist(char* file, char* path, int flag)
{
	int ret = 0, ilen = 0, type = 0, maj = 0, min = 0, control = 0, exist = 0, count = 0;
	off64_t startpos = 0, len = 0;
	FILE *fd = NULL;
	char* fileline = NULL, *from = NULL, *to = NULL, *tmpstr = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	from = malloc(MINMALLOC);
	if(from == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	to = malloc(MINMALLOC);
	if(to == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/filelist.tpk", 1, 0);
	fd = fopen(tmpstr, "r");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		count++;

		//reset hangtime
		if(pthread_self() == status.mainthread)
		{
			time_t sec = time(NULL);
			if(status.sec != 0 && sec - status.sec > status.spinnertime && sec - status.sec < 86400)
				status.sec = sec - (status.spinnertime + 1);
		}

		ilen = strlen(fileline) - 1;
		if(ilen >= 0 && fileline[ilen] == '\n')
			fileline[ilen] = '\0';
		ilen--;
		if(ilen >= 0 && fileline[ilen] == '\r')
			fileline[ilen] = '\0';

		ret = sscanf(fileline, "%[^#]#%[^#]#%d#%lld#%lld#%d#%d", to, from, &type, &startpos, &len, &maj, &min);
		if(ret != 7)
		{
			err("read filelist");
			ret = 1;
			goto end;
		}
		else
			ret = 0;

		//check CONTROL
		control = 0;
		if(to != NULL && strstr(to, "/CONTROL/") == to)
		{
			control = 1;
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, to + 9, 1, 0);
			snprintf(to, MINMALLOC, "%s", tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		if(flag == 0 && control == 0) continue;
		if(flag == 1 && control == 1) continue;

		if(control == 0)
		{
			exist = 0;
			if(file_exist(to) == 1) exist = 1;
			ret = tpkwriterestore(path, to, type, getlfiletype(to), exist);
			if(ret != 0)
			{
				err("write restore file");
				ret = 1;
				goto end;
			}
		}

		if(type == DT_DIR) //dir
		{
			ret = tpkcreatedir("", to, 0);
			if(ret != 0)
			{
				err("create dir %s", to);
				ret = 1;
				goto end;
			}
		}
		else if(type == DT_LNK) //link
		{
			ret = tpkcreatelink("", to, from, 0);
			if(ret != 0)
			{
				err("create symlink %s -> %s", to, from);
				ret = 1;
				goto end;
			}
		}
		else if(type == DT_BLK) //block device
		{
			ret = tpkcreateblk("", to, maj, min, 0);
			if(ret != 0)
			{
				err("create blk dev %s", to);
				ret = 1;
				goto end;
			}
		}
		else if(type == DT_CHR) //charcter device
		{
			ret = tpkcreatechr("", to, maj, min, 0);
			if(ret != 0)
			{
				err("create chr dev %s", to);
				ret = 1;
				goto end;
			}
		}
		else if(type == DT_FIFO) //fifo
		{
			ret = tpkcreatefifo("", to, 0);
			if(ret != 0)
			{
				err("create fifo %s", to);
				ret = 1;
				goto end;
			}
		}
		else if(type == DT_REG) //file
		{
			if(TPKZIPALL == 1)
				ret = tpkcreatefile("", file, to, startpos, len, 0);
			else
				ret = tpkcreatefilegz(file, to, startpos, len, 1);
			if(ret != 0)
			{
				err("create file %s", to);
				ret = 1;
				goto end;
			}
		}
	}

end:
	if(count == 0)
	{
		err("no entry in filelist");
	}

	free(fileline); fileline = NULL;
	free(from); from = NULL;
	free(to); to = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkextractindex(char* url)
{
	int ret = 0;
	FILE *fdr = NULL, *fdw = NULL;
	off64_t startpos = 0, len = 0;
	char* tmpstr = NULL, *fileline = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	//uncompress
	if(file_exist(TMPPACKAGES) == 0)
	{
		tmpstr = ostrcat(tmpstr, "gzip -d ", 1, 0);
		tmpstr = ostrcat(tmpstr, TMPPACKAGES, 1, 0);
		tmpstr = ostrcat(tmpstr, ".gz", 1, 0);
		ret = system(tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(ret != 0)
		{
			err("unzip file %s", TMPPACKAGES);
			ret = 1;
			goto end;
		}
	}

	fdr = fopen(TMPPACKAGES, "r");
	if(fdr == NULL)
	{
		perr("can't open %s", TMPPACKAGES);
		ret = 1;
		goto end;
	}

	fdw = fopen(TMPALLPACKAGES, "a");
	if(fdw == NULL)
	{
		perr("can't open %s", TMPALLPACKAGES);
		ret = 1;
		goto end;
	}

	while(fgets(fileline, MINMALLOC, fdr) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		tmpstr = ostrcat(url, tmpstr, 0, 1);
		tmpstr = ostrcat(tmpstr, "#", 1, 0);
		tmpstr = ostrcat(tmpstr, fileline, 1, 0);

		ret = fprintf(fdw, "%s\n", tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(ret < 0)
		{
			perr("writting file %s", TMPALLPACKAGES);
			tpkcleanworkdir(TMP);
			ret = 1;
			goto end;
		}
		ret = 0;
	}

	//uncompress
	if(file_exist(TMPPREVIEW) == 0)
	{
		tmpstr = ostrcat(tmpstr, TMPPREVIEW, 1, 0);
		tmpstr = ostrcat(tmpstr, ".gz", 1, 0);

		if(file_exist(tmpstr) == 1)
		{
			tmpstr = ostrcat("gzip -d ", tmpstr, 0, 1);
			ret = system(tmpstr);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("unzip file %s", TMPPREVIEW);
				ret = 1;
				goto end;
			}
		}
		free(tmpstr); tmpstr = NULL;
	}

	if(file_exist(TMPPREVIEW) == 1)
	{
		ret = tpkgettail(TMPPREVIEW, &startpos, &len);
		if(ret != 0)
		{
			err("read startpos/len %s", TMPPREVIEW);
			ret = 1;
			goto end;
		}

		tmpstr = ostrcat(tmpstr, TMP, 1, 0);
		tmpstr = ostrcat(tmpstr, "/filelist.tpk", 1, 0);
		ret = tpkcreatefile("", TMPPREVIEW, tmpstr, startpos, len, 0); //extract filelist
		free(tmpstr); tmpstr = NULL;
		if(ret != 0)
		{
			err("extract filelist %s", TMPPREVIEW);
			ret = 1;
			goto end;
		}

		ret = tpkextractfilelist(TMPPREVIEW, TMP, 1); //extract other files
		if(ret != 0)
		{
			err("extract files %s", TMPPREVIEW);
			ret = 1;
			goto end;
		}
	}

end:
	if(fdr != NULL) fclose(fdr);
	if(fdw != NULL) fclose(fdw);

	if(ret != 0)
		tpkdel(TMP, 0, 0);

	int iret = unlink(TMPPACKAGES);
	if(iret != 0 && errno != ENOENT)
	{
		debug(130, "can't remove file %s", TMPPACKAGES);
	}

	iret = unlink(TMPPREVIEW);
	if(iret != 0 && errno != ENOENT)
	{
		debug(130, "can't remove file %s", TMPPREVIEW);
	}

	tmpstr = ostrcat(tmpstr, TMPPACKAGES, 1, 0);
	tmpstr = ostrcat(tmpstr, ".gz", 1, 0);
	iret = unlink(tmpstr);
	if(iret != 0 && errno != ENOENT)
	{
		debug(130, "can't remove file %s", tmpstr);
	}
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, TMPPREVIEW, 1, 0);
	tmpstr = ostrcat(tmpstr, ".gz", 1, 0);
	iret = unlink(tmpstr);
	if(iret != 0 && errno != ENOENT)
	{
		debug(130, "can't remove file %s", tmpstr);
	}
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, TMP, 1, 0);
	tmpstr = ostrcat(tmpstr, "/filelist.tpk", 1, 0);
	iret = unlink(tmpstr);
	if(iret != 0 && errno != ENOENT)
	{
		debug(130, "can't remove file %s", tmpstr);
	}
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, TMP, 1, 0);
	tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
	iret = unlink(tmpstr);
	if(iret != 0 && errno != ENOENT)
	{
		debug(130, "can't remove file %s", tmpstr);
	}
	free(tmpstr); tmpstr = NULL;

	free(fileline); fileline = NULL;
	return ret;
}

int tpkchangeinstallpath(char* file, char* installpath)
{
	int ret = 0, count = 0, i = 0, noinstpath = 0;
	FILE *fd = NULL;
	char* fileline = NULL;

	if(installpath == NULL || installpath[0] == '*') noinstpath = 1;

	fileline = calloc(1, MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return 1;
	}

	fd = fopen(file, "r");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		free(fileline); fileline = NULL;
		return 1;
	}

	//count lines
	while(fgets(fileline, MINMALLOC, fd) != NULL)
		count++;

	//create buffer
	char* buf[count];
	for(i = 0; i < count; i++)
		buf[i] = NULL;

	ret = fseek(fd, 0, SEEK_SET);
	if(ret < 0)
	{
		perr("seek to startpos");
		ret = 1;
		goto end;
	}
	else
		ret = 0;

	//read lines into buffer
	count = 0;
	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		buf[count] = ostrcat(fileline, NULL, 0, 0);
		count++;
	}

	if(fd != NULL) fclose(fd);

	fd = fopen(file, "w");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		free(fileline); fileline = NULL;
		return 1;
	}

	for(i = 0; i < count; i++)
	{
		if(buf[i] == NULL) continue;
		if(noinstpath == 0) buf[i] = string_replace("/_path_", installpath, buf[i], 1);
		if(ostrstr(buf[i], "/var/swap#") == buf[i]) continue; //don't touch /var/swap
		ret = fprintf(fd, "%s", buf[i]);
		if(ret < 0)
		{
			perr("writting filelist file (%s)", file);
			ret = 1;
			goto end;
		}
		else
			ret = 0;
	}

end:
	for(i = 0; i < count; i++)
	{
		free(buf[i]);
		buf[i] = NULL;
	}

	free(fileline); fileline = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkaddinstallpath(char* file, char* installpath)
{
	int ret = 0;
	FILE *fd = NULL;
	char* tmpstr = NULL;

	if(installpath == NULL || installpath[0] == '*') return 0;

	fd = fopen(file, "a");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		return 1;
	}

	tmpstr = ostrcat("\nInstallpath: ", installpath, 0, 0);
	ret = fprintf(fd, "%s", tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(ret < 0)
	{
		perr("add installpath to control file %s", file);
		ret = 1;
		goto end;
	}
	else
		ret = 0;

end:
	if(fd != NULL) fclose(fd);
	return ret;
}

//flag 0: check group
//flag 1: don't check group
int tpkinstall(char* file, char* installpath, int flag)
{
	int ret = 0, groupskip = 0;
	off64_t startpos = 0, len = 0;
	char* tmpstr = NULL, *name = NULL, *path = NULL, *tmpfile = NULL;
	struct tpk* tpknode = NULL, *tpkinstalled = NULL;

	if(file == NULL)
	{
		err("NULL detect");
		return 1;
	}

	//remove arch + version from filename
	tmpfile = ostrcat(file, NULL, 0, 0);
	if(tmpfile != NULL)
	{
		tmpstr = strrchr(tmpfile, '_');
		if(tmpstr != NULL)
		{
			tmpstr[0] = '\0';
			tmpstr = strrchr(tmpfile, '_');
			if(tmpstr != NULL) tmpstr[0] = '\0';
		}
	}
	tmpstr = NULL;

	if(tmpfile == NULL)
	{
		err("NULL detect");
		return 1;
	}

	name = basename(tmpfile);
	if(name == NULL || strcmp("/", name) == 0 || strcmp(".", name) == 0 || strcmp("..", name) == 0)
	{
		err("NULL detect %s", tmpfile);
		return 1;
	}

	path = ostrcat(path, EXTRACTDIR, 1, 0);
	path = ostrcat(path, "/", 1, 0);
	path = ostrcat(path, name, 1, 0);
	if(path == NULL)
	{
		err("NULL detect %s", file);
		return 1;
	}

	//remove del flag file
	tmpstr = ostrcat(path, ".del", 0, 0);
	unlink(tmpstr);
	free(tmpstr); tmpstr = NULL;

	ret = mkdir(path, 0777);
	if(ret != 0)
	{
		perr("create path %s", path);
		free(path); path = NULL;
		return 1;
	}

	//uncompress
	if(file_exist(file) == 0)
	{
		tmpstr = ostrcat(tmpstr, "gzip -d ", 1, 0);
		tmpstr = ostrcat(tmpstr, file, 1, 0);
		tmpstr = ostrcat(tmpstr, ".gz", 1, 0);
		ret = system(tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(ret != 0)
		{
			err("unzip file %s", file);
			ret = 1;
			goto end;
		}
	}

	if(file_exist(file) == 0)
	{
		err("file not found %s", file);
		ret = 1;
		goto end;
	}

	ret = tpkgettail(file, &startpos, &len);
	if(ret != 0)
	{
		err("read startpos/len %s", file);
		ret = 1;
		goto end;
	}

	//extract filelist
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/filelist.tpk", 1, 0);
	ret = tpkcreatefile("", file, tmpstr, startpos, len, 0);
	if(ret != 0)
	{
		err("extract filelist %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}

	//change filelist install path
	ret = tpkchangeinstallpath(tmpstr, installpath);
	if(ret != 0)
	{
		err("change installpath filelist %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	//extract CONTROL
	ret = tpkextractfilelist(file, path, 0);
	if(ret != 0)
	{
		err("extract control files %s", file);
		ret = 1;
		goto end;
	}

	//add installpath to control file
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/control", 1, 0);
	ret = tpkaddinstallpath(tmpstr, installpath);
	if(ret != 0)
	{
		err("add installpath to control files %s", tmpstr);
		free(tmpstr); tmpstr = NULL;
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	freetpk();
	tpklistinstalled(1);
	tpkinstalled = tpk;
	tpk = NULL;

	tpknode = tpkreadcontrol(path, 0);
	if(tpknode == NULL)
	{
		err("read control files %s/%s", path, name);
		ret = 1;
		goto end;
	}

	//check group
	if(flag == 0)
	{
		if(tpknode->group != 0)
		{
			struct tpk* node = tpkinstalled;
			while(node != NULL)
			{
				if(node->group == tpknode->group && ostrcmp(node->name, tpknode->name) != 0)
				{
					err("can't install tpk with same group %s (group %d)", path, tpknode->group);
					ret = 1;
					groupskip = 1;
					goto end;
				}
				node = node->next;
			}
		}
	}

	debug(130, "getboxtype: %s", getboxtype());
	debug(130, "tpknode->boxtype: %s", tpknode->boxtype);
	debug(130, "tpknode->name: %s", tpknode->name);
	//check boxtype
	if(tpknode->boxtype != NULL && ostrcmp(tpknode->boxtype, "*") != 0 && ostrstr(tpknode->boxtype, getboxtype()) == NULL)
	{
		printf("skiped\n");
		err("boxtype not allowed %s", tpknode->boxtype);
		ret = 1;
		goto end;
	}

	//check minversion not working...disable
/*
	if(tpknode->minversion != 0 && tpknode->minversion < PLUGINVERSION)
	{
		debug(10, "Tpk Install Skip: %s (%d < %d)\n", tpknode->name, tpknode->minversion, PLUGINVERSION);
		err("minversion to short %d", tpknode->minversion);
		ret = 1;
		goto end;
	}
*/
	//check minversion new
	if(tpknode->minversion != 0 && tpknode->minversion > PLUGINVERSION)
	{
		debug(10, "Install Skip %s: (Tpk Minversion %d > %d Pluginversion)", tpknode->name, tpknode->minversion, PLUGINVERSION);
		err("minversion greater pluginversion %d", tpknode->minversion);
		ret = 1;
		goto end;
	}

	//check size
	ret = tpkchecksize(tpknode, installpath, 0);
	if(ret != 0)
	{
		err("size to big %d", tpknode->size);
		ret = 1;
		goto end;
	}

	//execute pre install
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/preinst", 1, 0);
	if(file_exist(tmpstr) == 1)
	{
		tmpstr = ostrcat(tmpstr, " \"", 1, 0);
		tmpstr = ostrcat(tmpstr, installpath, 1, 0);
		tmpstr = ostrcat(tmpstr, "\"", 1, 0);
		tmpstr = ostrcat(tmpstr, " >> ", 1, 0);
		tmpstr = ostrcat(tmpstr, TPKLOG, 1, 0);
		tmpstr = ostrcat(tmpstr, " 2>&1", 1, 0);
		ret = system(tmpstr);
		if(ret != 0)
		{
			err("executing preinst %s", tmpstr);
			free(tmpstr); tmpstr = NULL;
			ret = 1;
			goto end;
		}
	}
	free(tmpstr); tmpstr = NULL;

	ret = tpkextractfilelist(file, path, 1); //extract other files
	if(ret != 0)
	{
		tmpstr = ostrcat(tmpstr, path, 1, 0);
		tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
		tpkrevertfile(tmpstr);
		free(tmpstr); tmpstr = NULL;

		err("extract files %s", file);
		ret = 1;
		goto end;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
	ret = tpkrevertfile(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(ret != 0)
	{
		err("revert restore file %s", path);
		ret = 1;
		goto end;
	}

	//execute post install
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/postinst", 1, 0);
	if(file_exist(tmpstr) == 1)
	{
		tmpstr = ostrcat(tmpstr, " \"", 1, 0);
		tmpstr = ostrcat(tmpstr, installpath, 1, 0);
		tmpstr = ostrcat(tmpstr, "\"", 1, 0);
		tmpstr = ostrcat(tmpstr, " >> ", 1, 0);
		tmpstr = ostrcat(tmpstr, TPKLOG, 1, 0);
		tmpstr = ostrcat(tmpstr, " 2>&1", 1, 0);
		ret = system(tmpstr);
		if(ret != 0)
		{
			err("executing postinst %s", tmpstr);
			free(tmpstr); tmpstr = NULL;
			ret = 1;
			goto end;
		}
	}
	free(tmpstr); tmpstr = NULL;

	//del preinst file
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/preinst", 1, 0);
	unlink(tmpstr);
	free(tmpstr); tmpstr = NULL;

	//del postinst file
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/postinst", 1, 0);
	unlink(tmpstr);
	free(tmpstr); tmpstr = NULL;

	//del filelist
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/filelist.tpk", 1, 0);
	unlink(tmpstr);
	free(tmpstr); tmpstr = NULL;

	//create ok flagfile
	ret = tpkcreateflagfile(path, ".tpkok");
	if(ret != 0)
	{
		err("create flagfile");
		ret = 1;
		goto end;
	}

end:
	freetpk();
	tpk = tpkinstalled;
	freetpk();

	if(ret != 0)
	{
		tpkremove(tmpfile, 1, 1);
		tpkdelbackup(path);
		tpkcleanworkdir(path);
		ret = rmdir(path);
		if(ret != 0 && errno != ENOENT)
		{
			debug(130, "can't remove dir %s", path);
		}
		ret = 1;
	}
	else
		tpkdelbackup(path);

	free(path); path = NULL;
	free(tmpfile); tmpfile = NULL;
	if(groupskip == 1) ret = 2;
	return ret;
}

char* tpkgetinstallpath(char* path)
{
	int len = 0;
	FILE *fd = NULL;
	char* fileline = NULL, *tmpstr = NULL;
	char* installpath = NULL;

	if(path == NULL) return NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		goto end;
	}

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, "control", 1, 0);

	fd = fopen(tmpstr, "r");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		goto end;
	}
	free(tmpstr); tmpstr = NULL;

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		if(strstr(fileline, "Installpath: ") == fileline)
			installpath = ostrcat(fileline + 13, NULL, 0, 0);
	}

end:
	if(fd != NULL) fclose(fd);
	return installpath;
}

//flag 0: clean workdir
//flag 1: don't clean workdir, don't create del flag file
//flag 2: same as 0 but don't create del flag file
int tpkremove(char* file, int restore, int flag)
{
	int ret = 0;
	char* tmpstr = NULL, *name = NULL, *path = NULL;
	char* installpath = NULL;

	if(file == NULL)
	{
		err("NULL detect");
		return 1;
	}

	name = basename(file);
	if(name == NULL || strcmp("/", name) == 0 || strcmp(".", name) == 0 || strcmp("..", name) == 0)
	{
		err("NULL detect %s", file);
		return 1;
	}

	path = ostrcat(path, EXTRACTDIR, 1, 0);
	path = ostrcat(path, "/", 1, 0);
	path = ostrcat(path, name, 1, 0);
	if(path == NULL)
	{
		err("NULL detect %s", file);
		return 1;
	}

	if(file_exist(path) == 0)
	{
		err("package not exist %s", path);
		ret = 1;
		goto end;
	}

	//create del flag file
	if(flag == 0)
	{
		tmpstr = ostrcat(name, ".del", 0, 0);
		tpkcreateflagfile(EXTRACTDIR, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//get install path from control file
	installpath = tpkgetinstallpath(path);

	//execute pre remove
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/prerm", 1, 0);
	if(file_exist(tmpstr) == 1)
	{
		tmpstr = ostrcat(tmpstr, " \"", 1, 0);
		tmpstr = ostrcat(tmpstr, installpath, 1, 0);
		tmpstr = ostrcat(tmpstr, "\"", 1, 0);
		tmpstr = ostrcat(tmpstr, " >> ", 1, 0);
		tmpstr = ostrcat(tmpstr, TPKLOG, 1, 0);
		tmpstr = ostrcat(tmpstr, " 2>&1", 1, 0);
		ret = system(tmpstr);
		if(ret != 0)
		{
			err("executing prerm %s", tmpstr);
			free(tmpstr); tmpstr = NULL;
			ret = 1;
			goto end;
		}
	}
	free(tmpstr); tmpstr = NULL;

	if(flag == 2)
		tpkdel(path, restore, 1);
	else
		tpkdel(path, restore, 0);

	//execute post remove
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/postrm", 1, 0);
	if(file_exist(tmpstr) == 1)
	{
		tmpstr = ostrcat(tmpstr, " \"", 1, 0);
		tmpstr = ostrcat(tmpstr, installpath, 1, 0);
		tmpstr = ostrcat(tmpstr, "\"", 1, 0);
		tmpstr = ostrcat(tmpstr, " >> ", 1, 0);
		tmpstr = ostrcat(tmpstr, TPKLOG, 1, 0);
		tmpstr = ostrcat(tmpstr, " 2>&1", 1, 0);
		ret = system(tmpstr);
		if(ret != 0)
		{
			err("executing postrm %s", tmpstr);
			free(tmpstr); tmpstr = NULL;
			ret = 1;
			goto end;
		}
	}
	free(tmpstr); tmpstr = NULL;

end:
	if(flag == 0 || flag == 2)
	{
		tpkcleanworkdir(path);
		int iret = rmdir(path);
		if(iret != 0 && errno != ENOENT)
		{
			debug(130, "can't remove dir %s", path);
			ret = 1;
		}
	}

	free(installpath); installpath = NULL;
	free(path); path = NULL;
	return ret;
}

int tpkupdatepre()
{
	DIR *d;
	int ret = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	if(file_exist(PREFILE) == 1)
		return 0;

	d = opendir(PREDIR); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", PREDIR);
		ret = 1;
		goto end;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", PREDIR);
		entry = readdir(d); //gets subsequent entries from d

		if(!entry) //no more entries, so break
			break;

		if(entry->d_type == DT_DIR)
		{
			if(entry->d_name != NULL && entry->d_name[0] != '.')
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", PREDIR, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					goto end;
				}

				tmpstr = ostrcat(tmpstr, EXTRACTDIR, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
				ret = mkdir(tmpstr, 0777);
				if(ret != 0 && errno != EEXIST)
				{
					perr("create path %s", tmpstr);
					ret = 0;
					free(tmpstr); tmpstr = NULL;
					continue;
				}
				free(tmpstr); tmpstr = NULL;

				//copy control file
				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/control", 1, 0);

				tmpstr1 = ostrcat(tmpstr1, EXTRACTDIR, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/control", 1, 0);

				ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
				if(ret != 0)
				{
					err("create preinstalled control file %s", tmpstr1);
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

				//copy prerm file
				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/prerm", 1, 0);

				tmpstr1 = ostrcat(tmpstr1, EXTRACTDIR, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/prerm", 1, 0);

				ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
				if(ret != 0)
				{
					err("create preinstalled prerm file %s", tmpstr1);
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

#ifdef OEBUILD
				//get install path from control file
				char* installpath = tpkgetinstallpath(path);

				//execute post install
				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/postinst", 1, 0);
				if(file_exist(tmpstr) == 1)
				{
					tmpstr = ostrcat(tmpstr, " \"", 1, 0);
					tmpstr = ostrcat(tmpstr, installpath, 1, 0);
					tmpstr = ostrcat(tmpstr, "\"", 1, 0);

					tmpstr = ostrcat(tmpstr, " >> ", 1, 0);
					tmpstr = ostrcat(tmpstr, TPKLOG, 1, 0);
					tmpstr = ostrcat(tmpstr, " 2>&1", 1, 0);

					ret = system(tmpstr);
					if(ret != 0)
					{
						err("executing postinst %s", tmpstr);
						free(tmpstr); tmpstr = NULL;
//						ret = 1;
//						goto end;
					}
				}
				free(tmpstr); tmpstr = NULL;
				//free installpath
				free(installpath), installpath = NULL;
#endif

				//copy postrm file
				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/postrm", 1, 0);

				tmpstr1 = ostrcat(tmpstr1, EXTRACTDIR, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/postrm", 1, 0);

				ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
				if(ret != 0)
				{
					err("create preinstalled postrm file %s", tmpstr1);
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

				//copy restore file
				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);

				tmpstr1 = ostrcat(tmpstr1, EXTRACTDIR, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/restore.tpk", 1, 0);

				ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
				if(ret != 0)
				{
					err("create preinstalled restore file %s", tmpstr1);
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

				//create .tpkok file
				tmpstr = ostrcat(tmpstr, EXTRACTDIR, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
				tmpstr = ostrcat(tmpstr, "/.tpkok", 1, 0);

				ret = tpkcreateflagfile(tmpstr, NULL);
				if(ret != 0)
				{
					err("create flag file %s", tmpstr);
				}
				free(tmpstr); tmpstr = NULL;

				//check if .del file exist
				tmpstr = ostrcat(tmpstr, EXTRACTDIR, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
				tmpstr = ostrcat(tmpstr, ".del", 1, 0);

				tmpstr1 = ostrcat(tmpstr1, EXTRACTDIR, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, entry->d_name, 1, 0);

				if(file_exist(tmpstr) == 1)
					tpkremove(tmpstr1, 0, 2);

				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

			}
		}
	}

end:
	if(d && closedir(d))
	{
		perr("Could not close %s", PREDIR);
		ret = 1;
	}

	//create preinstalled flag file
	ret = tpkcreateflagfile(PREFILE, NULL);
	if(ret != 0)
	{
		err("create flag file %s", PREFILE);
	}

	return ret;
}

int checkupdatedir()
{
	DIR *d;
	int ret = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	d = opendir(EXTRACTDIR); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", EXTRACTDIR);
		ret = 1;
		goto end;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;

		snprintf(path, PATH_MAX, "%s", EXTRACTDIR);
		entry = readdir(d); //gets subsequent entries from d

		if(!entry) //no more entries, so break
			break;

		if(entry->d_type == DT_DIR)
		{
			if(entry->d_name != NULL && entry->d_name[0] != '.' && strstr(entry->d_name, ".update") != NULL)
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", EXTRACTDIR, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					goto end;
				}

				tmpstr = ostrcat(tmpstr, path, 1, 0);
				if(tmpstr != NULL) tmpstr[strlen(tmpstr) - 7] = '\0';
				if(file_exist(tmpstr) == 0)
				{
					ret = rename(path, tmpstr);
					if(ret != 0)
					{
						perr("rename file %s -> %s", path, tmpstr);
						ret = 0;
						continue;
					}
				}
				else
				{
					tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
					tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
					tmpstr1 = ostrcat(tmpstr1, ".tpkok", 1, 0);
					if(file_exist(tmpstr1) == 1)
					{
						tpkcleanworkdir(path);
						ret = rmdir(path);
						if(ret != 0 && errno != ENOENT)
						{
							perr("remove dir %s", path);
							ret = 0;
						}
					}
					else
					{
						tpkcleanworkdir(tmpstr);
						ret = rmdir(tmpstr);
						if(ret != 0 && errno != ENOENT)
						{
							perr("remove dir %s", tmpstr);
							ret = 0;
						}
						ret = rename(path, tmpstr);
						if(ret != 0)
						{
							perr("rename file %s -> %s", path, tmpstr);
							ret = 0;
						}
					}
				}
			}
		}
	}

end:
	if(d && closedir(d))
	{
		perr("Could not close %s", EXTRACTDIR);
		ret = 1;
	}

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	return ret;
}

//flag 0: checkupdatedir
//flag 1: don't checkupdatedir
int tpklistinstalled(int flag)
{
	DIR *d;
	char* tmpstr = NULL;
	int ret = 0;
	struct tpk* tpknode = NULL;

	if(flag == 0) checkupdatedir();
	freetpk();

	d = opendir(EXTRACTDIR); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", EXTRACTDIR);
		ret = 1;
		goto end;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", EXTRACTDIR);
		entry = readdir(d); //gets subsequent entries from d

		if(!entry) //no more entries, so break
			break;

		if(entry->d_type == DT_DIR)
		{
			if(entry->d_name != NULL && entry->d_name[0] != '.')
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", EXTRACTDIR, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					goto end;
				}

				tpknode = tpkreadcontrol(path, 0);
				if(tpknode == NULL)
				{
					debug(130, "read control file %s", path);
					continue;
				}
				else
				{
					tmpstr = ostrcat(tmpstr, path, 1, 0);
					tmpstr = ostrcat(tmpstr, "/", 1, 0);
					tmpstr = ostrcat(tmpstr, ".tpkok", 1, 0);
					if(file_exist(tmpstr) == 1)
						tpknode->status = 1;
					else
						tpknode->status = 2;
				}
			}
		}
	}

end:
	if(d && closedir(d))
	{
		perr("Could not close %s", EXTRACTDIR);
		ret = 1;
	}

	return ret;
}

int tpklist()
{
	int ret = 0, len = 0;
	FILE *fd = NULL;
	char* fileline = NULL;
	char* name = NULL, *showname = NULL, *section = NULL, *desc = NULL, *url = NULL, *arch = NULL, *filename = NULL, *titanname = NULL, *usepath = NULL, *boxtype = NULL;
	int version = 0, group = 0, minversion = 0, preinstalled = 0, size = 0;
	struct tpk* tpknode = NULL, *tpkinstalled = NULL, *tpktmp = NULL;

	freetpk();
	tpklistinstalled(0);
	tpkinstalled = tpk;
	tpk = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	name = malloc(MINMALLOC);
	if(name == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	showname = malloc(MINMALLOC);
	if(showname == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	section = malloc(MINMALLOC);
	if(section == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	desc = malloc(MINMALLOC);
	if(desc == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	url = malloc(MINMALLOC);
	if(url == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	arch = malloc(MINMALLOC);
	if(arch == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	titanname = malloc(MINMALLOC);
	if(titanname == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	usepath = malloc(MINMALLOC);
	if(usepath == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	boxtype = malloc(MINMALLOC);
	if(boxtype == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}

	fd = fopen(TMPALLPACKAGES, "r");
	if(fd == NULL)
	{
		perr("can't open %s", TMPALLPACKAGES);
		ret = 1;
		goto end;
	}

	int skipgroup = 0;
	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		ret = sscanf(fileline, "%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%d#%d#%d#%d#%d", url, name, showname, section, desc, arch, titanname, usepath, boxtype, &version, &group, &minversion, &preinstalled, &size);
		if(ret != 14)
		{
			err("read file %s", TMPALLPACKAGES);
			continue;
		}
		ret = 0;

		//check group
		int skip = 0;
/*
		if(group != 0)
		{
			struct tpk* node = tpkinstalled;
			while(node != NULL)
			{
				if(node->group == group)
				{
					skipgroup++;
					break;
				}
				node = node->next;
			}
		}
*/
		//check boxtype
		if(boxtype != NULL && ostrcmp(boxtype, "*") != 0 && ostrstr(boxtype, getboxtype()) == NULL)
			skip = 1;

		//check minversion
//		if(minversion != 0 && minversion < PLUGINVERSION)
		if(minversion != 0 && minversion > PLUGINVERSION)
		{
			debug(10, "List Skip %s: (Tpk Minversion %d > %d Pluginversion)", name, minversion, PLUGINVERSION);
			skip = 1;
		}

		if(skip == 0)
		{
			filename = ostrcat(name, "_", 0, 0);
			filename = ostrcat(filename, oitoa(version), 1, 1);
			filename = ostrcat(filename, "_", 1, 0);
			filename = ostrcat(filename, arch, 1, 0);

			tpknode = addtpk(name, desc, section, showname, arch, filename, titanname, version, group, minversion, preinstalled, url, size, NULL, usepath, boxtype, NULL);
		}
		else if(skipgroup == 1)
		{
			filename = ostrcat(name, "_", 0, 0);
			filename = ostrcat(filename, oitoa(version), 1, 1);
			filename = ostrcat(filename, "_", 1, 0);
			filename = ostrcat(filename, arch, 1, 0);
			tpknode = addtpk(name, " ", section, _("There may only be one package installed from this section. To install another package from this section, remove the installed one. If the package is not visible after a software update, perform a TPK update to: "), arch, filename, titanname, version, group, minversion, preinstalled, url, size, NULL, usepath, boxtype, NULL);
		}
	}

end:
	tpktmp = tpk;
	tpk = tpkinstalled;
	freetpk();
	tpk = tpktmp;

	free(name); name = NULL;
	free(showname); showname = NULL;
	free(section); section = NULL;
	free(desc); desc = NULL;
	free(url); url = NULL;
	free(arch); arch = NULL;
	free(filename); filename = NULL;
	free(titanname); titanname = NULL;
	free(usepath); usepath = NULL;
	free(boxtype); boxtype = NULL;
	free(fileline); fileline = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

// flag = 0 gui
// flag = 1 web
int tpkupdate(int flag)
{
	int ret = 0, err = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	struct tpk* tpknode = NULL, *tpkinstalled = NULL, *tpkinstallednode = NULL, *tpkmainlist = NULL;

	system("touch /var/etc/.tpkupgrade");
	tpklistinstalled(0);
	tpkinstalled = tpk;
	tpk = NULL;
	tpklist();
	tpkmainlist = tpk;
	tpk = NULL;

	tpkinstallednode = tpkinstalled;
	while(tpkinstallednode != NULL)
	{
		tpknode = tpkmainlist;
		while(tpknode != NULL)
		{
			// disable settings upgrade group 100
			if(ostrcmp(tpkinstallednode->name, tpknode->name) == 0 && tpknode->version > tpkinstallednode->version && tpkinstallednode->group != 100 && tpknode->minversion <= PLUGINVERSION)
			{
				debug(10, "Tpk Upgrade: %s (Install Version %d > %d Installed Version) (Min Version %d <= %d Plugin Version) (Group %d)", tpknode->name, tpknode->version, tpkinstallednode->version, tpknode->minversion, PLUGINVERSION, tpkinstallednode->group);
				//TODO: remove file that never exist in new pakages
				tmpstr = ostrcat(tmpstr, EXTRACTDIR, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, tpkinstallednode->name, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, ".update", 1, 0);

				ret = rename(tmpstr, tmpstr1);

				if(ret != 0)
				{
					perr("rename file %s -> %s", tmpstr, tmpstr1);
					ret = 0;
					err++;
					free(tmpstr); tmpstr = NULL;
					free(tmpstr1); tmpstr1 = NULL;
					tpknode = tpknode->next;
					continue;
				}
				ret = tpkgetpackage(tpknode->filename, tpknode->url, tpkinstallednode->installpath, 1, flag);

				if(ret == 0) //install ok
				{
					tpkcleanworkdir(tmpstr1);
					ret = rmdir(tmpstr1);
					if(ret != 0 && errno != ENOENT)
					{
						perr("remove dir %s", tmpstr1);
					}
				}
				else //install nok
				{
					err++;
					ret = rename(tmpstr1, tmpstr);
					if(ret != 0)
					{
						perr("rename file %s -> %s", tmpstr1, tmpstr);
					}
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				ret = 0;
				break;
			}
			tpknode = tpknode->next;
		}
		tpkinstallednode = tpkinstallednode->next;
	}

	freetpk();
	tpk = tpkinstalled;
	freetpk();
	tpk = tpkmainlist;
	freetpk();
	return err;
}

void tpkgeturl(char* line, char** ip, char** path, int* port)
{
	if(line == NULL) return;

	*ip = strstr(line, "http://");
	if(*ip != NULL)
	{
		(*ip) += 7;
		if(*ip != NULL)
		{
			*path = strchr(*ip, '/');
			if(*path != NULL)
			{
				(*path)[0] = '\0';
				(*path) += 1;
			}
		}
	}
}

//flag 0: remove only content
//flag 1: remove dir
int tpkcleantmp(int flag)
{
	int ret = 0;

	tpkcleanworkdir(TMP);
	if(flag == 1)
	{
		ret = rmdir(TMP);
		if(ret != 0 && errno != ENOENT)
		{
			debug(130, "can't remove dir %s", TMP);
		}
		ret = 0;
	}

	return ret;
}

//flag 0: only package list
//flag 1: package + preview
int tpkgetindex(int flag)
{
	int ret = 0, len = 0, port = 80, err = 0;
	FILE *fd = NULL;
	char* fileline = NULL, *ip = NULL, *path = NULL, *httpret = NULL;
	char* tmpstr1 = NULL, *tmpstr2 = NULL;

	ret = mkdir(TMP, 0777);
	if(ret != 0 && errno != EEXIST)
	{
		perr("create path %s", TMP);
		err = 1;
		goto end;
	}

	tpkcleantmp(0);

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		err = 1;
		goto end;
	}

	fd = fopen(FEEDFILE, "r");
	if(fd == NULL)
	{
		perr("can't open %s", FEEDFILE);
		err = 1;
		goto end;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		tpkgeturl(fileline, &ip, &path, &port);

		if(ostrcmp("97.74.32.10", ip) == 0)
		{
//			if(ostrcmp(path, "/svn/tpk/sh4") == 0)

#ifdef SH4
			if(ostrcmp(path, "/svn/tpk/nightly-sh4-secret") == 0)
#elif ARM
			if(ostrcmp(path, "/svn/tpk/nightly-arm-secret") == 0)
#else
			if(ostrcmp(path, "/svn/tpk/nightly-mipsel-secret") == 0)
#endif
			{
				textbox(_("Message"), _("check your Secret Feed !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
				err = 1;
			}
		}

		if(ip != NULL && path != NULL)
		{
			if(ostrcmp("97.74.32.10", ip) == 0 && !file_exist("/etc/.beta"))
				ip = "openaaf.dyndns.tv";
			else if(ostrcmp("97.74.32.10", ip) == 0)
				ip = "beta.dyndns.tv";

			tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, HTTPPACKAGES, 1, 0);

			tmpstr2 = ostrcat(tmpstr2, TMP, 1, 0);
			tmpstr2 = ostrcat(tmpstr2, "/", 1, 0);
			tmpstr2 = ostrcat(tmpstr2, HTTPPACKAGES, 1, 0);

			debug(130, "get http://%s/%s -> %s", ip, tmpstr1, tmpstr2);
			httpret = gethttp(ip, tmpstr1, port, tmpstr2, HTTPAUTH, 5000, NULL, 0);
			if(httpret == NULL)
			{
				err("http download error %s/%s", ip, tmpstr1);
				unlink(tmpstr2);
			}
			free(tmpstr1); tmpstr1 = NULL;
			free(tmpstr2); tmpstr2 = NULL;

			if(httpret != NULL)
			{
				if(flag == 1)
				{
					tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
					tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
					tmpstr1 = ostrcat(tmpstr1, HTTPPREVIEW, 1, 0);

					tmpstr2 = ostrcat(tmpstr2, TMP, 1, 0);
					tmpstr2 = ostrcat(tmpstr2, "/", 1, 0);
					tmpstr2 = ostrcat(tmpstr2, HTTPPREVIEW, 1, 0);

          			debug(130, "get http://%s/%s -> %s", ip, tmpstr1, tmpstr2);
					gethttp(ip, tmpstr1, port, tmpstr2, HTTPAUTH, 5000, NULL, 0);
					free(tmpstr1); tmpstr1 = NULL;
					free(tmpstr2); tmpstr2 = NULL;
				}

				tmpstr1 = ostrcat(tmpstr1, "http://", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, ip, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
				ret = tpkextractindex(tmpstr1);
				if(ret != 0) err = 1;
				free(tmpstr1); tmpstr1 = NULL;
			}
			else
				err = 1;
		}
	}

end:
	if(fd != NULL) fclose(fd);
	free(fileline); fileline = NULL;
	return err;
}

//flag 0: check group
//flag 1: don't check group
//flag1 0: gui
//flag1 1: webif
int tpkgetpackage(char* package, char* url, char* installpath, int flag, int flag1)
{
	int ret = 0, port = 80;
	char* ip = NULL, *path = NULL;
	char* tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;
	char* tmpurl = NULL;

	struct skin* load = getscreen("loading");

	if(package == NULL || url == NULL)
	{
		err("NULL detect");
		ret = 1;
		goto end;
	}

	ret = mkdir(TMP, 0777);
	if(ret != 0 && errno != EEXIST)
	{
		perr("create path %s", TMP);
		ret = 1;
		goto end;
	}
	else
		ret = 0;

	tmpurl = ostrcat(url, NULL, 0, 0);
	tpkgeturl(tmpurl, &ip, &path, &port);

	if(ip != NULL && path != NULL)
	{
		tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
		tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, package, 1, 0);
		tmpstr1 = ostrcat(tmpstr1, ".tpk", 1, 0);
		if(TPKZIPALL == 1) tmpstr1 = ostrcat(tmpstr1, ".gz", 1, 0);

		tmpstr2 = ostrcat(tmpstr2, TMP, 1, 0);
		tmpstr2 = ostrcat(tmpstr2, "/", 1, 0);
		tmpstr2 = ostrcat(tmpstr2, package, 1, 0);
		tmpstr2 = ostrcat(tmpstr2, ".tpk", 1, 0);
		if(TPKZIPALL == 1) tmpstr2 = ostrcat(tmpstr2, ".gz", 1, 0);

		tmpstr3 = ostrcat(tmpstr3, TMP, 1, 0);
		tmpstr3 = ostrcat(tmpstr3, "/", 1, 0);
		tmpstr3 = ostrcat(tmpstr3, package, 1, 0);
		tmpstr3 = ostrcat(tmpstr3, ".tpk", 1, 0);

		if(ostrcmp("97.74.32.10", ip) == 0 && !file_exist("/etc/.beta"))
			ip = "openaaf.dyndns.tv";
		else if(ostrcmp("97.74.32.10", ip) == 0)
			ip = "beta.dyndns.tv";

    	debug(130, "get http://%s/%s -> %s", ip, tmpstr1, tmpstr2);
		if(flag1 == 0)
			screendownload("Download", ip, tmpstr1, port, tmpstr2, HTTPAUTH, 5000, 0);
		else
			gethttp(ip, tmpstr1, port, tmpstr2, HTTPAUTH, 5000, NULL, 0);

		if(flag1 == 0)
			drawscreen(load, 0, 0);
		ret = tpkinstall(tmpstr3, installpath, flag);
		if(flag1 == 0)
			clearscreen(load);
		unlink(tmpstr2);
		unlink(tmpstr3);

		free(tmpstr1); tmpstr1 = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr3); tmpstr3 = NULL;
	}

end:
	free(tmpurl); tmpurl = NULL;
	return ret;
}

int findsectiondone(char* section)
{
	struct tpk* node = tpk;

	if(node == NULL || section == NULL) return 1;

	while(node != NULL)
	{
		if(node->done == 1 && ostrcmp(section, node->section) == 0)
			return 1;
		node = node->next;
	}

	return 0;
}

//flag 0: show section
//flag 1: show entrys
//flag 2: show entrys for remove
struct menulist* tpkmenulist(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* section, int showpng, char* defentry, int flag)
{
	int skip = 0;
	struct tpk* node = tpk, *tpk_installed = NULL, *node_installed = NULL;
	struct menulist* tmpmlist = NULL;
	char* tmpstr = NULL, *tmpinfo = NULL, *tmppic = NULL;
	struct skin* load = getscreen("loading");

	if(node == NULL) return NULL;

	if(flag == 1)
	{
		drawscreen(load, 0, 0);
		tpk = NULL;
		tpklistinstalled(0);
		tpk_installed = tpk;
		tpk = node;
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
				tmpstr = ostrcat(tmpstr, node->showname, 1, 0);
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, node->section, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				tmpmlist = addmenulist(&mlist, tmpstr, NULL, tmppic, 0, 0);
				changemenulistparam(tmpmlist, node->name, node->titanname, NULL, NULL);
				free(tmpstr); tmpstr = NULL;
			}

			free(tmppic); tmppic = NULL;
		}

		if(flag == 1)
		{
			//check if tpk is installed
			node_installed = tpk_installed;
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

			//check if tpk is in section
			if(section != NULL && ostrcmp(node->section, section) != 0)
				skip = 2;

			if(skip == 2)
			{
				node = node->next;
				continue;
			}

			if(skip == 1)
			{
				tmpstr = ostrcat(tmpstr, "(", 1, 0);
				tmpstr = ostrcat(tmpstr, _("installed"), 1, 0);
				tmpstr = ostrcat(tmpstr, ") ", 1, 0);
			}

			tmpstr = ostrcat(tmpstr, _(node->showname), 1, 0);
			tmpstr = ostrcat(tmpstr, " v.", 1, 0);
			tmpstr = ostrcat(tmpstr, oitoa(node->version), 1, 1);

			tmpinfo = ostrcat(tmpinfo, "\n", 1, 0);
			tmpinfo = ostrcat(tmpinfo, _("Section: "), 1, 0);
			tmpinfo = ostrcat(tmpinfo, _(node->section), 1, 0);
			tmpinfo = ostrcat(tmpinfo, "\n", 1, 0);
			tmpinfo = ostrcat(tmpinfo, _("Description:"), 1, 0);
			tmpinfo = ostrcat(tmpinfo, "\n", 1, 0);
			if(node->desc != NULL)
				tmpinfo = ostrcat(tmpinfo, _(node->desc), 1, 0);
			else
				tmpinfo = ostrcat(tmpinfo, _("no description found"), 1, 0);

			tmppic = ostrcat(tmppic, node->name, 1, 0);
			if(tmppic != NULL)
			{
				tmppic = ostrcat(tmppic, ".png", 1, 0);

				//if pic not exist, get it from server
				int port = 80;
				char* ip = NULL, *path = NULL;
				char* tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;

				tmpstr1 = ostrcat(tmpstr1, TMP, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/", 1, 0);
				tmpstr1 = ostrcat(tmpstr1, tmppic, 1, 0);

				if(file_exist(tmpstr1) == 0)
				{
					tmpstr3 = ostrcat(tmpstr3, node->url, 1, 0);
					tpkgeturl(tmpstr3, &ip, &path, &port);

					if(ip != NULL && path != NULL)
					{
						tmpstr2 = ostrcat(tmpstr2, path, 1, 0);
						tmpstr2 = ostrcat(tmpstr2, "/", 1, 0);
						tmpstr2 = ostrcat(tmpstr2, node->filename, 1, 0);
						tmpstr2 = ostrcat(tmpstr2, ".png", 1, 0);

	          			debug(130, "get http://%s/%s -> %s", ip, tmpstr2, tmpstr1);
						gethttp(ip, tmpstr2, port, tmpstr1, HTTPAUTH, 5000, NULL, 0);
					}
				}

				free(tmpstr1); tmpstr1 = NULL;
				free(tmpstr2); tmpstr2 = NULL;
				free(tmpstr3); tmpstr3 = NULL;
			}

			if(skip == 1)
				tmpmlist = addmenulist(&mlist, tmpstr, tmpinfo, tmppic, 1, 0);
			else
				tmpmlist = addmenulist(&mlist, tmpstr, tmpinfo, tmppic, 0, 0);

			char* size = oitoa(node->size);
			changemenulistparam(tmpmlist, node->filename, node->url, node->usepath, size);
			free(size); size = NULL;

			free(tmpstr); tmpstr = NULL;
			free(tmpinfo); tmpinfo = NULL;
			free(tmppic); tmppic = NULL;
		}

		node = node->next;
	}

	if(flag == 1)
	{
		node = tpk;
		tpk = tpk_installed;
		freetpk();
		tpk = node;
		clearscreen(load);
	}

  setmenulistdefault(mlist, defentry);
	return menulistbox(mlist, paramskinname, skintitle, NULL, paramskinpath, "/skin/plugin.png", showpng, 0);
}

char* gettpktmplist(char* path)
{
	char* cmd = NULL, *tmpstr = NULL;

	cmd = ostrcat(cmd, "ls ", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	cmd = ostrcat(cmd, " | grep '\\.tpk' | sed s/.gz//", 1, 0);

	tmpstr = command(cmd);

	free(cmd); cmd = NULL;
	return tmpstr;
}

#endif
#endif

