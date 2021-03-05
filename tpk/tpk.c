#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/sysmacros.h>

char* ostrcat(char* value1, char* value2, int free1, int free2);
int file_exist(char* filename);
int dvbwrite(int fd, unsigned char* buf, int count, int tout);
int dvbreadfd(int fd, unsigned char *buf, int pos, int count, int tout, int flag);
char* strstrip(char *text);
char* oitoa(int value);

short debug_level = 10;
#define _(x) gettext(x)
#define MINMALLOC 4096
#define PROGNAME "tpk"
#define TPKZIPALL 0

//#define WORKDIR "/tpk" //path must exist
#define TPKFILELIST WORKDIR"/filelist.tpk"
#define ARCHIVE WORKDIR"/archive.tpk"
#define PACKAGES WORKDIR"/Packages"
#define PREVIEW WORKDIR"/Packages.preview"
#define PREVIEWFILELIST WORKDIR"/filelist.preview"
#define PREINSTDIR WORKDIR"/pre" //path must exist

#define debug(level, fmt, args...) if(debug_level == level) { do { printf("[%s] " fmt, PROGNAME, ##args); } while (0); printf(", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define err(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define perr(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", err=%m, file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define filedebug(file, fmt, args...) { FILE* fd = fopen(file, "a"); if(fd != NULL) { do { fprintf(fd, "" fmt, ##args); } while (0); fprintf(fd, "\n"); fclose(fd); }}

#include <tpk.h>

char* strstrip(char *text)
{
	debug(1000, "in");
	char* tmpstr = text;

	if(text == NULL) return NULL;
	int len = strlen(text);

	while(isspace(tmpstr[len - 1])) tmpstr[--len] = '\0';
	while(*tmpstr && isspace(*tmpstr)) ++tmpstr, --len;

	memmove(text, tmpstr, len + 1);

	debug(1000, "out");
	return text;
}

char* oitoa(int value)
{
	debug(1000, "in");
	char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%d", value);
	//buf = ostrshrink(buf);

	debug(1000, "out");
	return buf;
}

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
int dvbreadfd(int fd, unsigned char *buf, int pos, int count, int tout, int flag)
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
	tmpstr = ostrcat(tmpstr, "/CONTROL/control", 1, 0);

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

int tpkcreatepreinstalled(char* mainpath, char* name)
{
	int ret = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *path = NULL;

	if(name == NULL)
	{
		err("NULL detect");
		ret = 1;
		goto end;
	}
	
	path = ostrcat(path, PREINSTDIR, 1, 0);
	path = ostrcat(path, "/", 1, 0);
	path = ostrcat(path, name, 1, 0);
	ret = mkdir(path, 0777);
	if(ret != 0)
	{
		perr("create path %s", path);
		ret = 1;
		goto end;
	}

	//create restore file
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/restore.tpk", 1, 0);
	ret = tpkcreatefile("", TPKFILELIST, tmpstr, 0, -1, 0);
	if(ret != 0)
	{
		err("create preinstalled restore file %s", tmpstr);
		ret = 1;
		goto end;
	}

	ret = tpkrevertfile(tmpstr);
	if(ret != 0)
	{
		err("revert preinstalled file %s", tmpstr);
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;
	
	//create control file
	tmpstr = ostrcat(tmpstr, mainpath, 1, 0);
	tmpstr = ostrcat(tmpstr, "/CONTROL/control", 1, 0);
	
	tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "/control", 1, 0);
	
	ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
	if(ret != 0)
	{
		err("create preinstalled control file %s", tmpstr1);
		ret = 1;
		goto end;
	}
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	
	//create prerm file
	tmpstr = ostrcat(tmpstr, mainpath, 1, 0);
	tmpstr = ostrcat(tmpstr, "/CONTROL/prerm", 1, 0);
	
	tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "/prerm", 1, 0);
	
	if(file_exist(tmpstr) == 1)
	{
		ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
		if(ret != 0)
		{
			err("create preinstalled prerm file %s", tmpstr1);
			ret = 1;
			goto end;
		}
	}
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	
	//create postrm file
	tmpstr = ostrcat(tmpstr, mainpath, 1, 0);
	tmpstr = ostrcat(tmpstr, "/CONTROL/postrm", 1, 0);
	
	tmpstr1 = ostrcat(tmpstr1, path, 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "/postrm", 1, 0);
	
	if(file_exist(tmpstr) == 1)
	{
		ret = tpkcreatefile("", tmpstr, tmpstr1, 0, -1, 0);
		if(ret != 0)
		{
			err("create preinstalled postrm file %s", tmpstr1);
			ret = 1;
			goto end;
		}
	}
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;

end:
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(path); path = NULL;
	return ret;
}

//flag 0: write index file
//flag 1: don't write index file
struct tpk* tpkcreateindex(char* path, char* name, int flag)
{
	int ret = 0, writeret = 0;
	FILE *fd = NULL;
	struct tpk* tpknode = NULL;

	if(name == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	freetpk();
	tpknode = tpkreadcontrol(path, 1);
	if(tpknode == NULL || tpknode->name == NULL)
	{
		err("read control file %s", path);
		ret = 1;
		goto end;
	}
  
  tpknode->name = strstrip(tpknode->name);
  tpknode->section = strstrip(tpknode->section);
  tpknode->desc = strstrip(tpknode->desc);
  tpknode->arch = strstrip(tpknode->arch);
  tpknode->titanname = strstrip(tpknode->titanname);

	if(tpknode->section == NULL) tpknode->section = ostrcat("extra", NULL, 0, 0);
	if(tpknode->desc == NULL) tpknode->desc = ostrcat("*", NULL, 0, 0);
	if(tpknode->showname == NULL) tpknode->showname = ostrcat("*", NULL, 0, 0);
	if(tpknode->arch == NULL) tpknode->arch = ostrcat("noarch", NULL, 0, 0);
	if(tpknode->titanname == NULL) tpknode->titanname = ostrcat("*", NULL, 0, 0);
	if(tpknode->usepath == NULL) tpknode->usepath = ostrcat("*", NULL, 0, 0);
	if(tpknode->boxtype == NULL) tpknode->boxtype = ostrcat("*", NULL, 0, 0);

  if(flag == 0)
  {
  	fd = fopen(PACKAGES, "a");
  	if(fd == NULL)
  	{
  		perr("can't open %s", PACKAGES);
  		ret = 1;
  		goto end;
  	}
  
  	writeret = fprintf(fd, "%s#%s#%s#%s#%s#%s#%s#%s#%d#%d#%d#%d#%d\n", tpknode->name, tpknode->showname, tpknode->section, tpknode->desc, tpknode->arch, tpknode->titanname, tpknode->usepath, tpknode->boxtype, tpknode->version, tpknode->group, tpknode->minversion, tpknode->preinstalled, tpknode->size);
  	if(writeret < 0)
  	{
  		perr("writting file %s", PACKAGES);
  		ret = 1;
  		goto end;
  	}
  }

end:
	if(fd != NULL) fclose(fd);
	
	if(ret == 1) 
		return NULL;
	else
		return tpknode;
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
					ret = 1;
					break;
				}

				if(!(strcmp("CONTROL", entry->d_name) == 0 && first == 1))
				{

					ret = tpkcreatedir(mainpath, path, 1);
					if(ret != 0)
					{
						err("create dir %s", path);
						ret = 1;
						break;
					}
				}

				ret = tpkcreatearchive(mainpath, path, 0); //Recursively call with the new path
				if(ret != 0)
				{
					err("create archive %s", path);
					ret = 1;
					break;
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
				free(tmpstr); tmpstr = NULL;
				ret = 1;
				break;
			}

			ret = readlink(tmpstr, buf, MINMALLOC);
			if(ret < 0)
			{
				perr("read link %s", tmpstr);
				free(tmpstr); tmpstr = NULL;
				free(buf); buf = NULL;
				ret = 1;
				break;
			}

			ret = tpkcreatelink(mainpath, tmpstr, buf, 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				free(buf); buf = NULL;
				err("create link");
				ret = 1;
				break;
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
				perr("get file status %s", tmpstr);
				free(tmpstr); tmpstr = NULL;
				ret = 1;
				break;
			}
			ret = tpkcreateblk(mainpath, tmpstr, major(s.st_rdev), minor(s.st_rdev), 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create blk dev");
				ret = 1;
				break;
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
				perr("get file status %s", tmpstr);
				free(tmpstr); tmpstr = NULL;
				ret = 1;
				break;
			}
			ret = tpkcreatechr(mainpath, tmpstr, major(s.st_rdev), minor(s.st_rdev), 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create chr dev");
				ret = 1;
				break;
			}
		}
		else if(entry->d_type == DT_FIFO) //fifo
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			ret = tpkcreatefifo(mainpath, tmpstr, 1);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create fifo");
				ret = 1;
				break;
			}
		}
		else if(entry->d_type == DT_REG) //file
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			
			if(TPKZIPALL == 0)
			{
				char* tmpzip = NULL;
				
				tmpzip = ostrcat("gzip \"", tmpstr, 0, 0);
				tmpzip = ostrcat(tmpzip, "\"", 1, 0);
				ret = system(tmpzip);
				free(tmpzip); tmpzip = NULL;
				if(ret != 0)
				{
					err("zip file %s", tmpstr);
					free(tmpstr); tmpstr = NULL;
					ret = 1;
					break;
				}
				
				tmpzip = ostrcat(tmpstr, ".gz", 0, 0);
				ret = rename(tmpzip, tmpstr);
				free(tmpzip); tmpzip = NULL;
				if(ret != 0)
				{
					err("rename file %s", tmpstr);
					free(tmpstr); tmpstr = NULL;
					ret = 1;
					break;
				}		
			}

			ret = tpkcreatefile(mainpath, tmpstr, ARCHIVE, 0, -1, 1);
			if(ret != 0)
			{
				err("create file %s", tmpstr);
				free(tmpstr); tmpstr = NULL;
				ret = 1;
				break;
			}
			
			if(TPKZIPALL == 0)
			{
				char* tmpzip = NULL;
				
				tmpzip = ostrcat(tmpstr, ".gz", 0, 0);
				ret = rename(tmpstr, tmpzip);
				free(tmpzip); tmpzip = NULL;
				if(ret != 0)
				{
					err("rename file %s", tmpstr);
					free(tmpstr); tmpstr = NULL;
					ret = 1;
					break;
				}
				
				tmpzip = ostrcat("gzip -d \"", tmpstr, 0, 0);
				tmpzip = ostrcat(tmpzip, ".gz\"", 1, 0);
				ret = system(tmpzip);
				free(tmpzip); tmpzip = NULL;
				if(ret != 0)
				{
					err("unzip file %s", tmpstr);
					free(tmpstr); tmpstr = NULL;
					ret = 1;
					break;
				}			
			}
			
			free(tmpstr); tmpstr = NULL;
		}
		else if(entry->d_type == DT_SOCK) //socket
		{
			err("socket filetype found");
			ret = 1;
			break;
		}
		else if(entry->d_type == DT_UNKNOWN) //unknown
		{
			err("unknown filetype found");
			ret = 1;
			break;
		}
		else //unknown
		{
			err("unknown filetype found");
			ret = 1;
			break;
		}
	}

	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	return ret;
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
			ret = tpkcreatefilereal(mainpath, tmpstr, PREVIEW, 0, -1, name, 3);
			free(tmpstr); tmpstr = NULL;
			if(ret != 0)
			{
				err("create file");
				ret = 1;
				break;
			}
		}
	}

	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	return ret;
}

int tpkcreatallearchive(char* dirname, char* name)
{
	DIR *d;
	char* tmpstr = NULL;
	int ret = 0;
	struct tpk* tpknode = NULL;

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
		tpknode = NULL;

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
					
				tpknode = tpkcreateindex(path, entry->d_name, 0);
				if(tpknode == NULL)
				{
					err("create index file %s", path);
					ret = 1;
					goto end;
				}

				ret = tpkcreatearchive(path, path, 1);
				if(ret != 0)
				{
					err("create archive %s", path);
					ret = 1;
					goto end;
				}

				if(tpknode->preinstalled == 1)
				{
					ret = tpkcreatepreinstalled(path, tpknode->name);
					if(ret != 0)
					{
						err("create preinstalled file");
						ret = 1;
						goto end;
					}
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
				tmpstr = ostrcat(tmpstr, tpknode->name, 1, 0);
				tmpstr = ostrcat(tmpstr, "_", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(tpknode->version), 1, 1);
				tmpstr = ostrcat(tmpstr, "_", 1, 0);
				tmpstr = ostrcat(tmpstr, tpknode->arch, 1, 0);
				tmpstr = ostrcat(tmpstr, ".tpk", 1, 0);
				ret = rename(ARCHIVE, tmpstr);
				free(tmpstr); tmpstr = NULL;
				if(ret != 0)
				{
					perr("rename archive %s", ARCHIVE);
					ret = 1;
					goto end;
				}

				if(TPKZIPALL == 1)
				{
					tmpstr = ostrcat(tmpstr, "gzip ", 1, 0);
					tmpstr = ostrcat(tmpstr, WORKDIR, 1, 0);
					tmpstr = ostrcat(tmpstr, "/", 1, 0);
					tmpstr = ostrcat(tmpstr, tpknode->name, 1, 0);
					tmpstr = ostrcat(tmpstr, "_", 1, 0);
					tmpstr = ostrcat(tmpstr, oitoa(tpknode->version), 1, 1);
					tmpstr = ostrcat(tmpstr, "_", 1, 0);
					tmpstr = ostrcat(tmpstr, tpknode->arch, 1, 0);
					tmpstr = ostrcat(tmpstr, ".tpk", 1, 0);
					ret = system(tmpstr);
					free(tmpstr); tmpstr = NULL;
					if(ret != 0)
					{
						err("gzip file");
						ret = 1;
						goto end;
					}
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
				ret = tpkcreatepreviewarchive(tmpstr, tmpstr, tpknode->name);
				free(tmpstr); tmpstr = NULL;
				if(ret != 0)
				{
					err("create preview archive %s", path);
					ret = 1;
					goto end;
				}

				freetpk();
			}
		}
	}

	if(file_exist(PREVIEWFILELIST) || file_exist(PREVIEW))
	{
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
		tmpstr = ostrcat(tmpstr, PREVIEW, 1, 0);
		ret = system(tmpstr);
		free(tmpstr); tmpstr = NULL;
		if(ret != 0)
		{
			err("gzip file %s", PREVIEW);
			ret = 1;
			goto end;
		}
	}

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

end:
	freetpk();

	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		ret = 1;
	}

	if(ret != 0)
		tpkcleanworkdir(WORKDIR);

	return ret;
}

int tpkgetfilesize(char* file)
{
  int fd = -1;
  off64_t len = 0;
  
  fd = open(file, O_RDONLY);
	if(fd < 0)
	{
		perr("open file %s", file);
		return 0;
	}
  
  len = lseek64(fd, 0, SEEK_END);
  if(len < 0)
	{
    perr("can't get filelen %s", file);
    close(fd);
    return 0;
  }

  close(fd);
  return (len / 1024) + 1;
}

int tpkwritecontrol(char* path, struct tpk* tpknode, int size)
{
  int ret = 0;
	FILE *fd = NULL;
	char* tmpstr = NULL;
  
  if(tpknode == NULL)
  {
    err("NULL detect");
    return 1;
  }

	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "/CONTROL/control", 1, 0);

	fd = fopen(tmpstr, "w");
	if(fd == NULL)
	{
		perr("can't open %s", tmpstr);
		ret = 1;
		goto end;
	}
  
  ret = fprintf(fd, "Package: %s\nArchitecture: %s\nShowname: %s\nVersion: %d\nSection: %s\nDescription: %s\nGroup: %d\nMinversion: %d\nPreinstalled: %d\nSize: %d\nTitanname: %s\nUsepath: %s\nBoxtype: %s", tpknode->name, tpknode->arch, tpknode->showname, tpknode->version, tpknode->section, tpknode->desc, tpknode->group, tpknode->minversion, tpknode->preinstalled, size, tpknode->titanname, tpknode->usepath, tpknode->boxtype);
  if(ret < 0)
  {
    perr("writting file %s", tmpstr);
    ret = 1;
    goto end;
  }
  else
    ret = 0;

end:
	free(tmpstr); tmpstr = NULL;
	if(fd != NULL) fclose(fd);
	return ret;
}

int tpkcalcsize(char* mainpath, char* dirname, int* size, int first)
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
					ret = 1;
					break;
				}
        
				ret = tpkcalcsize(mainpath, path, size, 0); //Recursively call with the new path
				if(ret != 0)
				{
					err("calc size %s", path);
					ret = 1;
					break;
				}
			}
		}
		else if(entry->d_type == DT_REG) //file
		{
			tmpstr = ostrcat(tmpstr, path, 1, 0);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			*size += tpkgetfilesize(tmpstr);      
			free(tmpstr); tmpstr = NULL;
		}
	}

	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	return ret;
}

int tpkcalcallsize(char* dirname, char* name)
{
	DIR *d;
	int ret = 0, size = 0;
	struct tpk* tpknode = NULL;

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
		tpknode = NULL;
    size = 0;

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
				if(ret != 0)
				{
					ret = 0;
					continue;
				}
        
        debug(10, "calc %s", path);

				tpknode = tpkcreateindex(path, entry->d_name, 1);
				if(tpknode == NULL)
				{
					err("read index file %s", path);
					freetpk();
					continue;
				}

				ret = tpkcalcsize(path, path, &size, 1);
				if(ret != 0)
				{
					err("calc size %s", path);
					freetpk();
          ret = 0;
					continue;
				}
        
        ret = tpkwritecontrol(path, tpknode, size);
        if(ret != 0)
				{
					err("write control %s", path);
					freetpk();
          ret = 0;
					continue;
				} 			
			}
		}
    
    freetpk();
	}

end:
	freetpk();

	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		ret = 1;
	}

	return ret;
}

int main(int argc, char *argv[])
{
	int ret = 0;

	if(argc == 3)
  {
    if(argv[1] != NULL && strcmp(argv[1], "calc") == 0)
      ret = tpkcalcallsize(argv[2], NULL);
    else
		  ret = tpkcreatallearchive(argv[2], NULL);
  }
	else if(argc == 4)
  {
    if(argv[1] != NULL && strcmp(argv[1], "calc") == 0)
      ret = tpkcalcallsize(argv[2], argv[3]);
    else
		  ret = tpkcreatallearchive(argv[2], argv[3]);
  }
	else
	{
		err("parameter not ok");
		ret = 1;
	}

	return ret;
}
