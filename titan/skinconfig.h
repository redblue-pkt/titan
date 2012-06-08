#ifndef SKINCONFIG_H
#define SKINCONFIG_H

struct clist* addskinconfigtmp(char *key, char *value)
{
	return addlisttmp(skinconfig, key, value);
}

struct clist* addskinconfigdef(char *key, char *value)
{
	return addlistdef(skinconfig, key, value);
}

struct clist* addskinconfig(char *key, char *value)
{
	struct clist* node = NULL;

	node =  addlist(skinconfig, key, value);
	if(node != NULL)
		status.writeskinconfig = 1;

	return node;
}

struct clist* addskinconfigscreentmp(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addskinconfigtmp(key, node->ret);
	
	return ret;
}

struct clist* addskinconfigscreentmpcheck(char *key, struct skin *node, char* check)
{
	if(ostrcmp(node->ret, check) == 0)
		return addskinconfigtmp(key, "");
	else
		return addskinconfigscreentmp(key, node);
}

struct clist* addskinconfigscreen(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addskinconfig(key, node->ret);
	
	return ret;
}

void delskinconfig(char *key)
{
	status.writeskinconfig = 1;
	dellist(skinconfig, key, 0);
}

struct clist* addskinconfigscreencheck(char *key, struct skin *node, char* check)
{
	if(ostrcmp(node->ret, check) == 0)
		delskinconfig(key);
	else
		return addskinconfigscreen(key, node);

	return NULL;
}

struct clist* addskinconfigint(char *key, int value)
{
	debug(1000, "in");
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addskinconfig(key, fileline);

	free(fileline);
	debug(1000, "out");
	return ret;
}

int readskinconfig(const char *filename, struct clist** tmpskinconfig)
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
		if(fileline[len] == '\n')
			fileline[len] = '\0';
		if(fileline[len] == '\r')
			fileline[len] = '\0';

		pos = strchr(fileline, '=');
		if(pos != NULL)
		{
			pos[0] = '\0';
			addlist(tmpskinconfig, fileline, pos + 1);
		}
	}

	fclose(fd);
	free(fileline);
	debug(1000, "out");
	return 0;
}

int writeskinconfigtmp()
{
	int ret = 0;

	if(writelisttmp(skinconfig) == 0)
		status.writeskinconfig = 1;

	return ret;
}

int writeskinconfig(const char *filename)
{
	return writelist(skinconfig, filename);
}

char* getskinconfig(char *key, char *ext)
{
	return getlist(skinconfig, key, ext);
}

int getskinconfigint(char *key, char *ext)
{
	char *ret = NULL;

	ret = getlist(skinconfig, key, ext);
	if(ret != NULL)
		return atoi(ret);
	else
		return 0;
}

void delskinconfigtmpall()
{
	dellisttmpall(skinconfig);
}

void delskinconfigtmp(char *key)
{
	dellisttmp(skinconfig, key);
}

void freeskinconfig()
{
	freelist(skinconfig);
}

int reloadskinconfig(char *filename)
{
	debug(1000, "in");
	int ret = 0;

	freeskinconfig();
	ret = readskinconfig(filename, skinconfig);
	if(ret != 0)
	{
		debug(1000, "out -> readskinconfig fail");
		return 1;
	}

	debug(1000, "out");
	return 0;
}

#endif
