#ifndef RCCONFIG_H
#define RCCONFIG_H

struct clist* addrcconfigtmp(char *key, char *value)
{
	return addlisttmp(rcconfig, key, value);
}

struct clist* addrcconfigdef(char *key, char *value)
{
	return addlistdef(rcconfig, key, value);
}

struct clist* addrcconfig(char *key, char *value)
{
	struct clist* node = NULL;

	node =  addlist(rcconfig, key, value);
	if(node != NULL)
		status.writercconfig = 1;

	return node;
}

struct clist* addrcconfigscreentmp(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addrcconfigtmp(key, node->ret);
	
	return ret;
}

struct clist* addrcconfigscreentmpcheck(char *key, struct skin *node, char* check)
{
	if(ostrcmp(node->ret, check) == 0)
		return addrcconfigtmp(key, "");
	else
		return addrcconfigscreentmp(key, node);
}

struct clist* addrcconfigscreen(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addrcconfig(key, node->ret);
	
	return ret;
}

struct clist* addrcconfigint(char *key, int value)
{
	debug(1000, "in");
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addrcconfig(key, fileline);

	free(fileline);
	debug(1000, "out");
	return ret;
}

int readrcconfig(const char *filename, struct clist** tmprcconfig)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *pos;
	int len = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");	
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		pos = strchr(fileline, '=');
		if(pos != NULL)
		{
			pos[0] = '\0';
			addlist(tmprcconfig, fileline, pos + 1);
		}
	}

	fclose(fd);
	free(fileline);
	debug(1000, "out");
	return 0;
}

int writercconfigtmp()
{
	int ret = 0;

	if(writelisttmp(rcconfig) == 0)
		status.writercconfig = 1;

	return ret;
}

int writercconfig(const char *filename)
{
	return writelist(rcconfig, filename);
}

char* getrcconfig(char *key, char *ext)
{
	return getlist(rcconfig, key, ext);
}

int getrcconfigint(char *key, char *ext)
{
	char *ret = NULL;

	ret = getlist(rcconfig, key, ext);
	if(ret != NULL)
		return atoi(ret);
	else
		return 0;
}

void delrcconfigtmpall()
{
	dellisttmpall(rcconfig);
}

void delrcconfigtmp(char *key)
{
	dellisttmp(rcconfig, key);
}

void delrcconfig(char *key)
{
	status.writercconfig = 1;
	dellist(rcconfig, key, 0);
}

void freercconfig()
{
	freelist(rcconfig);
}

int reloadrcconfig(char *filename)
{
	debug(1000, "in");
	int ret = 0;

	freercconfig();
	ret = readrcconfig(filename, rcconfig);
	if(ret != 0)
	{
		debug(1000, "out -> readrcconfig fail");
		return 1;
	}

	debug(1000, "out");
	return 0;
}

#endif
