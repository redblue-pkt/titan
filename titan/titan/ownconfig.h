#ifndef OWNCONFIG_H
#define OWNCONFIG_H

struct clist* addownconfigtmp(char *key, char *value)
{
	return addlisttmp(ownconfig, key, value);
}

struct clist* addownconfigdef(char *key, char *value)
{
	return addlistdef(ownconfig, key, value);
}

struct clist* addownconfig(char *key, char *value)
{
	struct clist* node = NULL;

	node = addlist(ownconfig, key, value);
	if(node != NULL)
		status.writeownconfig = 1;

	return node;
}

struct clist* addownconfigscreentmp(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addownconfigtmp(key, node->ret);

	return ret;
}

struct clist* addownconfigscreenqtmp(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addownconfigtmp(key, string_quote(node->ret));
	
	return ret;
}

struct clist* addownconfigscreentmpcheck(char *key, struct skin *node, char* check)
{
	if(ostrcmp(node->ret, check) == 0)
		return addownconfigtmp(key, "");
	else
		return addownconfigscreentmp(key, node);
}

struct clist* addownconfigscreen(char *key, struct skin *node)
{
	struct clist* ret = NULL;

	if(key != NULL && node != NULL && node->ret != NULL)
		ret = addownconfig(key, node->ret);

	return ret;
}

struct clist* addownconfigint(char *key, int value)
{
	char* fileline = NULL;
	struct clist* ret = NULL;

	fileline = oitoa(value);
	ret = addownconfig(key, fileline);

	free(fileline);
	return ret;
}

int writeownconfigtmp()
{
	int ret = 0;

	if(writelisttmp(ownconfig) == 0)
		status.writeownconfig = 1;

	return ret;
}

int writeownconfig(const char *filename)
{
	return writelist(ownconfig, filename);
}

char* getownconfig(char *key)
{
	return getlist(ownconfig, key, NULL);
}

char* getownconfigq(char *key)
{
	char *ret = NULL;

	ret = ostrcat(ret, getlist(ownconfig, key, NULL), 1, 0);

	if(ret != NULL)
	{
		if(ret[0] == '\"') ret[0] = ' ';
		if(ret[strlen(ret) - 1] == '\"') ret[strlen(ret) - 1] = ' ';
		string_remove_whitechars(ret);
	}

	return ret;
}

int getownconfigint(char *key)
{
	char *ret = NULL;

	ret = getlist(ownconfig, key, NULL);
	if(ret != NULL)
		return atoi(ret);
	else
		return 0;
}

void delownconfigtmpall()
{
	dellisttmpall(ownconfig);
}

void delownconfigtmp(char *key)
{
	dellisttmp(ownconfig, key);
}

void delownconfig(char *key)
{
	status.writeownconfig = 1;
	dellist(ownconfig, key, 0);
}

void freeownconfig()
{
	freelist(ownconfig);
}

int reloadownconfig(char *filename)
{
	int ret = 0;

	freeownconfig();
	ret = readconfig(filename, ownconfig);
	if(ret != 0)
	{
		err("readownconfig fail");
		return 1;
	}

	return 0;
}

#endif
