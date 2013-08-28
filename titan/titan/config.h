#ifndef CONFIG_H
#define CONFIG_H

struct clist* addconfigtmp(char *key, char *value)
{
	STARTFUNC
	return addlisttmp(config, key, value);
}

struct clist* addconfigdef(char *key, char *value)
{
	STARTFUNC
	return addlistdef(config, key, value);
}

struct clist* addconfig(char *key, char *value)
{
	STARTFUNC
	struct clist* node = NULL;

	node = addlist(config, key, value);
	if(node != NULL)
		status.writeconfig = 1;

	return node;
}

struct clist* addconfigscreentmp(char *key, struct skin *node)
{
	STARTFUNC
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
	{
		if(node->hidden == NO)
			ret = addconfigtmp(key, node->ret);
		else
			ret = addconfigtmp(key, "");
	}
	
	return ret;
}

struct clist* addconfigscreentmpcheck(char *key, struct skin *node, char* check)
{
	STARTFUNC
	if(ostrcmp(node->ret, check) == 0)
		return addconfigtmp(key, "");
	else
		return addconfigscreentmp(key, node);
}

struct clist* addconfigscreen(char *key, struct skin *node)
{
	STARTFUNC
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
	{
		if(node->hidden == NO)
			ret = addconfig(key, node->ret);
		else
			delconfig(key);
	}

	return ret;
}

struct clist* addconfigscreencheck(char *key, struct skin *node, char* check)
{
	STARTFUNC
	if(ostrcmp(node->ret, check) == 0)
		delconfig(key);
	else
		return addconfigscreen(key, node);

	return NULL;
}

struct clist* addconfiginttmp(char *key, int value)
{
	STARTFUNC
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addconfigtmp(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigllutmp(char *key, uint64_t value)
{
	STARTFUNC
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = ollutoa(value);
	ret = addconfigtmp(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfiglutmp(char *key, unsigned long value)
{
	STARTFUNC
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = olutoa(value);
	ret = addconfigtmp(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigint(char *key, int value)
{
	STARTFUNC
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addconfig(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigllu(char *key, uint64_t value)
{
	STARTFUNC
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = ollutoa(value);
	ret = addconfig(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfiglu(char *key, unsigned long value)
{
	STARTFUNC
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = olutoa(value);
	ret = addconfig(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigintcheck(char *key, int value, int check)
{
	STARTFUNC
	if(value == check)
		delconfig(key);
	else
		return addconfigint(key, value);

	return NULL;
}

int readconfig(const char *filename, struct clist** tmpconfig)
{
	STARTFUNC
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
			addlist(tmpconfig, fileline, pos + 1);
		}
	}

	fclose(fd);
	free(fileline);
	return 0;
}

int writeconfigtmp()
{
	STARTFUNC
	int ret = 0;

	if(writelisttmp(config) == 0)
		status.writeconfig = 1;

	return ret;
}

int writeconfig(const char *filename)
{
	STARTFUNC
	return writelist(config, filename);
}

char* getconfigbyval(char *value, char *ext)
{
	STARTFUNC
	return getlistbyval(config, value, ext);
}

char* getconfignotmp(char *key, char *ext)
{
	STARTFUNC
	return getlistnotmp(config, key, ext);
}

char* getconfig(char *key, char *ext)
{
	STARTFUNC
	return getlist(config, key, ext);
}

int getconfigint(char *key, char *ext)
{
	STARTFUNC
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return atoi(ret);
	else
		return 0;
}

float getconfigfloat(char *key, char *ext)
{
	STARTFUNC
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return atof(ret);
	else
		return 0;
}

uint64_t getconfigllu(char *key, char *ext)
{
	STARTFUNC
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return strtoull(ret, NULL, 10);
	else
		return 0;
}

unsigned long getconfiglu(char *key, char *ext)
{
	STARTFUNC
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return strtoul(ret, NULL, 10);
	else
		return 0;
}

void delconfigtmpall()
{
	STARTFUNC
	dellisttmpall(config);
}

void delconfigtmp(char *key)
{
	STARTFUNC
	dellisttmp(config, key);
}

void delconfig(char *key)
{
	STARTFUNC
	status.writeconfig = 1;
	dellist(config, key, 0);
}

void freeconfig()
{
	STARTFUNC
	freelist(config);
}

int reloadconfig(char *filename)
{
	STARTFUNC
	int ret = 0;

	freeconfig();
	ret = readconfig(filename, config);
	if(ret != 0)
	{
		err("readconfig fail");
		return 1;
	}

	return 0;
}

#endif
