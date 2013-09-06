#ifndef PIC_H
#define PIC_H

void debugpic()
{
	struct pic* node = pic;

	while(node != NULL)
	{
		printf("%s\n", node->name);
		node = node->next;
	}
}

void checkpictimeout()
{
	struct pic* node = pic, *prev = pic;
	time_t akttime = time(NULL);

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev->timeout != 0 && prev->lastaccess + prev->timeout < akttime)
			delpic(prev->name);
	}
}

struct pic* addpic(char *name, unsigned char* picbuf, int memfd, unsigned long width, unsigned long height, unsigned long rowbytes, int channels, int timeout, int del, struct pic* last)
{
	//chech if pic timed out and can remove
	checkpictimeout();

	struct pic *newnode = NULL, *prev = NULL, *node = pic;

	newnode = (struct pic*)calloc(1, sizeof(struct pic));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->name = ostrcat(name, NULL, 0, 0);
	newnode->picbuf = picbuf;
	newnode->memfd = memfd;
	newnode->width = width;
	newnode->height = height;
	newnode->rowbytes = rowbytes;
	newnode->channels = channels;
	if(timeout == 0)
		newnode->timeout = status.defpicmemtimeout;
	else
		newnode->timeout = timeout;
	newnode->lastaccess = time(NULL);
	newnode->del = del;

	if(last == NULL)
	{
		while(node != NULL && strcoll(newnode->name, node->name) > 0)
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
		pic = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	return newnode;
}

void freepiccontent(struct pic* node)
{
	free(node->name);
	node->name = NULL;

	if(node->memfd >= 0)
	{
		freebpamem(node->memfd, node->picbuf, node->width * node->height * node->channels);
		node->memfd = -1;
	}
	else
		free(node->picbuf);

	node->picbuf = NULL;
}

void delpic(char* name)
{
	struct pic *node = pic, *prev = pic;
	char* tmpstr = NULL;

	tmpstr = changepicpath(name);

	while(node != NULL)
	{
		if(ostrcmp(node->name, tmpstr) == 0)
		{
			if(node == pic)
				pic = node->next;
			else
				prev->next = node->next;

			freepiccontent(node);

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	free(tmpstr);
}

void delmarkedpic(int del)
{
	struct pic *node = pic, *prev = pic;

	while(node != NULL)
	{
		if(node->del == del)
		{
			if(node == pic)
				pic = node->next;
			else
				prev->next = node->next;

			freepiccontent(node);

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

struct pic* getpic(char* name)
{
	struct pic *node = pic;
	char* tmpstr = NULL;

	tmpstr = changepicpath(name);

	while(node != NULL)
	{
		if(ostrcmp(node->name, tmpstr) == 0)
		{
			free(tmpstr);
			node->lastaccess = time(NULL);
			return node;
		}

		node = node->next;
	}
	debug(100, "pic not found (name=%s)", name);
	free(tmpstr);
	return NULL;
}

void freepic()
{
	struct pic *node = pic, *prev = pic;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delpic(prev->name);
	}
}

#endif
