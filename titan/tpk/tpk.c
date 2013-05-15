#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

char* ostrcat(char* value1, char* value2, int free1, int free2);
int file_exist(char* filename);
int dvbwrite(int fd, unsigned char* buf, int count, int tout);
int dvbread(int fd, unsigned char *buf, int pos, int count, int tout, int flag);

short debug_level = 10;
#define MINMALLOC 4096
#define PROGNAME "tpk"

//#define WORKDIR "/tpk" //path must exist
#define TPKFILELIST WORKDIR"/filelist.tpk"
#define ARCHIVE WORKDIR"/archive.tpk"
#define PACKAGES WORKDIR"/Packages"
#define PREVIEW WORKDIR"/Packages.preview"
#define PREVIEWFILELIST WORKDIR"/filelist.preview"

#define debug(level, fmt, args...) if(debug_level == level) { do { printf("[%s] " fmt, PROGNAME, ##args); } while (0); printf(", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define err(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define perr(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", err=%m, file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define filedebug(file, fmt, args...) { FILE* fd = fopen(file, "a"); if(fd != NULL) { do { fprintf(fd, "" fmt, ##args); } while (0); fprintf(fd, "\n"); fclose(fd); }}

#include <tpk.h>

char* ostrcat(char* value1, char* value2, int free1, int free2)
{
	int len = 0, len1 = 0, len2 = 0;
	char* buf = NULL;

	if(value1 == NULL && value2 == NULL) return NULL;

	if(value1 != NULL) len1 = strlen(value1);
	if(value2 != NULL) len2 = strlen(value2);

	len = len1 + len2 + 1;

	if(free1 == 1)
		buf = realloc(value1, len);
	else
		buf = malloc(len);
	if(buf == NULL)
	{
		if(free1 == 1) free(value1);
		if(free2 == 1) free(value2);
		return NULL;
	}

	if(free1 == 0 && len1 > 0) memcpy(buf, value1, len1);
	if(len2 > 0) memcpy(buf + len1, value2, len2);
	buf[len - 1] = '\0';

	if(free2 == 1) free(value2);

	return buf;
}

int file_exist(char* filename)
{
	debug(1000, "in");
	if(access(filename, F_OK) == 0)
		return 1;
	else
		return 0;
}

int dvbwrite(int fd, unsigned char* buf, int count, int tout)
{
	struct timeval timeout;
	int ret = 0, usec = 0, sec = 0, tmpcount = count;
	unsigned char* buffer = NULL;

	if(fd < 0) return -1;

	if(tout == -1) tout = 3000 * 1000;
	usec = tout % 1000000;
	sec = (tout - usec) / 1000000;

	fd_set wfds;

	while (tmpcount > 0)
	{
		buffer = buf + (count - tmpcount);
		ret = write(fd, buffer, tmpcount);
		if(ret < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				FD_ZERO(&wfds);
				FD_SET(fd, &wfds);

				timeout.tv_sec = sec;
				timeout.tv_usec = usec;

				ret = TEMP_FAILURE_RETRY(select(fd + 1, NULL, &wfds, NULL, &timeout));
			}

			if(ret == 0)
			{
				perr("dvbwrite timed out fd=%d", fd);
				return -1;
			}
			if(ret < 0)
			{
				perr("can't write fd=%d", fd);
				return errno * -1;
			}
		}
		else
			tmpcount -= ret;
	}

	return count;
}

//flag 0: eof with timeout
//flag 1: eof without timeout
int dvbread(int fd, unsigned char *buf, int pos, int count, int tout, int flag)
{
	struct timeval timeout;
	int ret = 0, usec = 0, sec = 0;
	fd_set rfds;

	if(fd < 0) return -1;

	if(tout == -1) tout = 3000 * 1000;
	usec = tout % 1000000;
	sec = (tout - usec) / 1000000;

	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	ret = TEMP_FAILURE_RETRY(select(fd + 1, &rfds , NULL, NULL, &timeout));

	if(ret == 1)
	{
retry:
		ret = TEMP_FAILURE_RETRY(read(fd, buf + pos, count));
		if(ret > 0)
			return ret;
		else if(ret == 0)
		{
			tout = tout - 1000;
			usleep(1000);

			if(flag == 0 && tout > 0) goto retry;
			debug(10, "tpk read timeout fd=%d", fd);
		}
		else if(ret < 0)
		{
			if((errno == EAGAIN || errno == EOVERFLOW) && tout > 0)
			{
				if(errno != EAGAIN) perr("tpk read data fd=%d -> retry", fd);

				tout = tout - 1000;
				usleep(1000);

				goto retry;
			}
			perr("tpk read data fd=%d", fd);
		}
	}
	else if(ret == 0)
	{
		debug(10, "tpk select timeout fd=%d, tout=%d", fd, tout);
	}
	else
	{
		perr("tpk select fd=%d", fd);
	}

	return -1;
}

int tpkcheckcontrol(char* path)
{
	int ret = 0;
	FILE *fd = NULL;
	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, "CONTROL/control", 1, 0);

	fd = fopen(tmpstr, "r");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		ret = 1;
		goto end;
	}

end:
	free(tmpstr); tmpstr = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkcreatepreinstalled(char* name)
{
	int ret = 0;
	char* tmpstr = NULL;

	if(name == NULL)
	{
		err("NULL detect");
		ret = 1;
		goto end;
	}

	tmpstr = ostrcat(tmpstr, WORKDIR, 1, 0);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, name, 1, 0);
	tmpstr = ostrcat(tmpstr, ".tpk.pre", 1, 0);

	ret = tpkcreatefile("", TPKFILELIST, tmpstr, 0, -1, 0);
	if(ret != 0)
	{
		err("create preinstalled file %s\n", tmpstr);
		ret = 1;
		goto end;
	}

	ret = tpkrevertfile(tmpstr);
	if(ret != 0)
	{
		err("revert preinstalled file %s\n", tmpstr);
		ret = 1;
		goto end;
	}

	free(tmpstr); tmpstr = NULL;

end:
	return ret;
}

int tpkcreateindex(char* path, char* name)
{
	int ret = 0, writeret = 0;
	FILE *fd = NULL;
	struct tpk* tpknode = NULL;
	char* tmpstr = NULL;

	if(name == NULL)
	{
		err("NULL detect");
		return 1;
	}

	freetpk();
	tmpstr = ostrcat(name, ".tpk", 0, 0);
	tpknode = tpkreadcontrol(path, tmpstr, 1);
	free(tmpstr); tmpstr = NULL;
	if(tpknode == NULL)
	{
		err("read control file %s", path);
		ret = 1;
		goto end;
	}

	if(tpknode->section == NULL) tpknode->section = ostrcat("*", NULL, 0, 0);
	if(tpknode->desc == NULL) tpknode->desc = ostrcat("*", NULL, 0, 0);
	if(tpknode->showname == NULL) tpknode->showname = ostrcat("*", NULL, 0, 0);

	fd = fopen(PACKAGES, "a");
	if(fd == NULL)
	{
		perr("can't open %s", PACKAGES);
		ret = 1;
		goto end;
	}

	if(tpknode->preinstalled == 1)
	{
		ret = tpkcreatepreinstalled(name);
		if(ret != 0)
		{
			err("create preinstalled file");
			ret = 1;
			goto end;
		}
	}

	writeret = fprintf(fd, "%s#%s#%s#%s#%d#%d#%d#%d\n", tpknode->name, tpknode->showname, tpknode->section, tpknode->desc, tpknode->version, tpknode->group, tpknode->minversion, tpknode->preinstalled);
	if(writeret < 0)
	{
		perr("writting file %s", PACKAGES);
		ret = 1;
		goto end;
	}
end:
	freetpk();
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkcreatearchive(char* mainpath, char* dirname, int first)
{
	DIR *d;
	struct stat64 s;
	char* tmpstr = NULL;
	int ret = 0;

	if(mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	d = opendir(dirname); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", dirname);
		return 1;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length = 0;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dirname);
		entry = readdir(d); //Readdir gets subsequent entries from d

		if(!entry) //no more entries , so break
			break;

		free(tmpstr); tmpstr = NULL;
		if(entry->d_type == DT_DIR) //dir
		{
			//Check that the directory is not d or d's parent
			if(entry->d_name != NULL && entry->d_name[0] != '.' && !(strcmp("preview", entry->d_name) == 0 && first == 1))
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					return 1;
				}

				if(!(strcmp("CONTROL", entry->d_name) == 0 && first == 1))
				{

					ret = tpkcreatedir(mainpath, path, 1);
					if(ret != 0)
					{
						err("create dir %s", path);
						return 1;
					}
				}

				ret = tpkcreatearchive(mainpath, path, 0); //Recursively call with the new path
				if(ret != 0)
				{
					err("create archive %s", path);
					return 1;
				}
			}
		}
		else if(entry->d_type == DT_LNK) //link
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);

			char* buf = calloc(1, MINMALLOC);
			if(buf == NULL)
			{
				err("no mem");
				return 1;
			}

			ret = readlink(tmpstr, buf, MINMALLOC);
			if(ret < 0)
			{
				free(buf); buf = NULL;
				err("read link %s", tmpstr);
				return 1;
			}

			ret = tpkcreatelink(mainpath, tmpstr, buf, 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				free(buf); buf = NULL;
				err("create link");
				return 1;
			}
			free(buf); buf = NULL;
		}
		else if(entry->d_type == DT_BLK) //block device
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			ret = stat64(tmpstr, &s);
			if(ret != 0)
			{
				err("get file status %s", tmpstr);
				return 1;
			}
			ret = tpkcreateblk(mainpath, tmpstr, major(s.st_rdev), minor(s.st_rdev), 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create blk dev");
				return 1;
			}
		}
		else if(entry->d_type == DT_CHR) //charcter device
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			ret = stat64(tmpstr, &s);
			if(ret != 0)
			{
				err("get file status %s", tmpstr);
				return 1;
			}
			ret = tpkcreatechr(mainpath, tmpstr, major(s.st_rdev), minor(s.st_rdev), 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create chr dev");
				return 1;
			}
		}
		else if(entry->d_type == DT_FIFO) //fifo
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			ret = tpkcreatefifo(mainpath, tmpstr, 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0) return 1;
		}
		else if(entry->d_type == DT_REG) //file
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			ret = tpkcreatefile(mainpath, tmpstr, ARCHIVE, 0, -1, 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create file");
				return 1;
			}
		}
		else if(entry->d_type == DT_SOCK) //socket
		{
			err("socket filetype found");
			return 1;
		}
		else if(entry->d_type == DT_UNKNOWN) //unknown
		{
			err("unknown filetype found");
			return 1;
		}
		else //unknown
		{
			err("unknown filetype found");
			return 1;
		}
	}

	if(closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	return 0;
}

int tpkcreatepreviewarchive(char* mainpath, char* dirname, char* name)
{
	DIR *d;
	char* tmpstr = NULL;
	int ret = 0;

	if(mainpath == NULL)
	{
		err("NULL detect");
		return 1;
	}

	d = opendir(dirname); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", dirname);
		return 0;
	}

	while(1)
	{
		struct dirent* entry;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dirname);
		entry = readdir(d); //Readdir gets subsequent entries from d

		if(!entry) //no more entries , so break
			break;

		if(entry->d_type == DT_REG && strcmp(entry->d_name, "prev.png") == 0) //file
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			ret = tpkcreatefilereal(mainpath, tmpstr, PREVIEW, 0, -1, 3, name);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create file");
				return 1;
			}
		}
	}

	if(closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	return 0;
}

int tpkcreatallearchive(char* dirname, char* name)
{
	DIR *d;
	char* tmpstr = NULL;
	int ret = 0;

	//clean workdir
	ret = tpkcleanworkdir(WORKDIR);
	if(ret != 0)
	{
		err("clean working dir %s", WORKDIR);
		ret = 1;
		goto end;
	}

	d = opendir(dirname); //Open the directory
	if(! d) //Check it was opened
	{
		perr("Cannot open directory %s", dirname);
		ret = 1;
		goto end;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dirname);
		entry = readdir(d); //Readdir gets subsequent entries from d

		if(!entry) //no more entries , so break
			break;

		if(entry->d_type & DT_DIR) //dir
		{
			//Check that the directory is not d or d's parent
			if(entry->d_name != NULL && entry->d_name[0] != '.' && (name == NULL || strstr(entry->d_name, name) != NULL))
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					goto end;
				}

				ret = tpkcheckcontrol(path);
				if(ret == 1)
				{
					ret = 0;
					continue;
				}
				if(ret != 0)
				{
					err("read contol file %s", path);
					ret = 1;
					goto end;
				}

				debug(10, "create %s", path);

				ret = unlink(TPKFILELIST);
				if(ret != 0 && errno != ENOENT)
				{
					perr("removing %s", TPKFILELIST);
					ret = 1;
					goto end;
				}
				else
					ret = 0;

				ret = unlink(ARCHIVE);
				if(ret != 0 && errno != ENOENT)
				{
					perr("removing %s", ARCHIVE);
					ret = 1;
					goto end;
				}
				else
					ret = 0;

				ret = tpkcreatearchive(path, path, 1);
				if(ret != 0)
				{
					err("create archive %s", path);
					ret = 1;
					goto end;
				}

				ret = tpkcreateindex(path, entry->d_name);
				if(ret != 0)
				{
					err("create index file %s/%s", path, entry->d_name);
					ret = 1;
					goto end;
				}

				ret = tpkcreatefile("", TPKFILELIST, ARCHIVE, 0, -1, 2);
				if(ret != 0)
				{
					err("merge filelist to archive %s -> %s", TPKFILELIST, ARCHIVE);
					ret = 1;
					goto end;
				}

				tmpstr = ostrcat(tmpstr, WORKDIR, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
				tmpstr = ostrcat(tmpstr, ".tpk", 1, 0);
				ret = rename(ARCHIVE, tmpstr);
				free(tmpstr); tmpstr = NULL;
				if(ret != 0)
				{
					err("rename archive %s", ARCHIVE);
					ret = 1;
					goto end;
				}

				tmpstr = ostrcat(tmpstr, "gzip ", 1, 0);
				tmpstr = ostrcat(tmpstr, WORKDIR, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
				tmpstr = ostrcat(tmpstr, ".tpk", 1, 0);
				ret = system(tmpstr);
				free(tmpstr); tmpstr = NULL;
				if(ret != 0)
				{
					err("gzip file");
					ret = 1;
					goto end;
				}

				ret = unlink(TPKFILELIST);
				if(ret != 0 && errno != ENOENT)
				{
					perr("removing %s", TPKFILELIST);
					ret = 1;
					goto end;
				}
				else
					ret = 0;

				ret = unlink(ARCHIVE);
				if(ret != 0 && errno != ENOENT)
				{
					perr("removing %s", ARCHIVE);
					ret = 1;
					goto end;
				}
				else
				  	ret = 0;

				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/preview", 1, 0);
				ret = tpkcreatepreviewarchive(tmpstr, tmpstr, entry->d_name);
				free(tmpstr); tmpstr = NULL;
				if(ret != 0)
				{
					err("create preview archive %s", path);
					ret = 1;
					goto end;
				}
			}
		}
	}

	ret = tpkcreatefile("", PREVIEWFILELIST, PREVIEW, 0, -1, 2);
	if(ret != 0)
	{
		err("merge preview filelist to archive %s -> %s", PREVIEWFILELIST, PREVIEW);
		ret = 1;
		goto end;
	}

	ret = unlink(PREVIEWFILELIST);
	if(ret != 0 && errno != ENOENT)
	{
		perr("removing %s", PREVIEWFILELIST);
		ret = 1;
		goto end;
	}
	else
		ret = 0;

	tmpstr = ostrcat(tmpstr, "gzip ", 1, 0);
	tmpstr = ostrcat(tmpstr, PACKAGES, 1, 0);
	ret = system(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(ret != 0)
	{
		err("gzip file %s", PACKAGES);
		ret = 1;
		goto end;
	}

	tmpstr = ostrcat(tmpstr, "gzip ", 1, 0);
	tmpstr = ostrcat(tmpstr, PREVIEW, 1, 0);
	ret = system(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(ret != 0)
	{
		err("gzip file %s", PREVIEW);
		ret = 1;
		goto end;
	}
end:
	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		ret = 1;
	}

	if(ret != 0)
		tpkcleanworkdir(WORKDIR);

	return ret;
}

int main(int argc, char *argv[])
{
	int ret = 0;

	if(argc > 2)
	{
		if(argv[2] != NULL && argv[2][0] != '*')
			ret = tpkcreatallearchive(argv[1], argv[2]);
		else
			ret = tpkcreatallearchive(argv[1], NULL);
	}
	else
	{
		err("parameter not ok");
		ret = 1;
	}

	return ret;
}
