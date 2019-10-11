#ifndef CONFIG_H
#define CONFIG_H

struct clist* addconfigtmp(char *key, char *value)
{
	return addlisttmp(config, key, value);
}

struct clist* addconfigdef(char *key, char *value)
{
	return addlistdef(config, key, value);
}

struct clist* addconfig(char *key, char *value)
{
	struct clist* node = NULL;

	node = addlist(config, key, value);
	if(node != NULL)
		status.writeconfig = 1;

	return node;
}

struct clist* addconfigscreentmp(char *key, struct skin *node)
{
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
	if(ostrcmp(node->ret, check) == 0)
		return addconfigtmp(key, "");
	else
		return addconfigscreentmp(key, node);
}

struct clist* addconfigscreen(char *key, struct skin *node)
{
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
	if(ostrcmp(node->ret, check) == 0)
		delconfig(key);
	else
		return addconfigscreen(key, node);

	return NULL;
}

struct clist* addconfiginttmp(char *key, int value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addconfigtmp(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigllutmp(char *key, uint64_t value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = ollutoa(value);
	ret = addconfigtmp(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfiglutmp(char *key, unsigned long value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = olutoa(value);
	ret = addconfigtmp(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigint(char *key, int value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addconfig(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigllu(char *key, uint64_t value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = ollutoa(value);
	ret = addconfig(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfiglu(char *key, unsigned long value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = olutoa(value);
	ret = addconfig(key, fileline);

	free(fileline);
	return ret;
}

struct clist* addconfigintcheck(char *key, int value, int check)
{
	if(value == check)
		delconfig(key);
	else
		return addconfigint(key, value);

	return NULL;
}

int readconfig(const char *filename, struct clist** tmpconfig)
{
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
	int ret = 0;

	if(writelisttmp(config) == 0)
		status.writeconfig = 1;

	return ret;
}

int writeconfig(const char *filename)
{
	return writelist(config, filename);
}

char* getconfigbyval(char *value, char *ext)
{
	return getlistbyval(config, value, ext);
}

char* getconfignotmp(char *key, char *ext)
{
	return getlistnotmp(config, key, ext);
}

char* getconfig(char *key, char *ext)
{
	return getlist(config, key, ext);
}

char* getconfigdef(char *key, char *ext, char *def)
{
	getlist(config, key, ext);
	if(getlist(config, key, ext) == NULL)
		return ostrcat(def, NULL,0,0);
	else
		return getlist(config, key, ext);
}

int getconfigint(char *key, char *ext)
{
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return atoi(ret);
	else
		return 0;
}

float getconfigfloat(char *key, char *ext)
{
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return atof(ret);
	else
		return 0;
}

uint64_t getconfigllu(char *key, char *ext)
{
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return strtoull(ret, NULL, 10);
	else
		return 0;
}

unsigned long getconfiglu(char *key, char *ext)
{
	char *ret = NULL;

	ret = getlist(config, key, ext);
	if(ret != NULL)
		return strtoul(ret, NULL, 10);
	else
		return 0;
}

void delconfigtmpall()
{
	dellisttmpall(config);
}

void delconfigtmp(char *key)
{
	dellisttmp(config, key);
}

void delconfig(char *key)
{
	status.writeconfig = 1;
	dellist(config, key, 0);
}

void freeconfig()
{
	freelist(config);
}

int reloadconfig(char *filename)
{
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
