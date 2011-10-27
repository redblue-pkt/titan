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

int allocbpamem(size_t size, int *memfd, unsigned char **mem)
{
	*memfd = open("/dev/bpamem0", O_RDWR);
	
	if(*memfd < 0)
	{
		err("cannot access /dev/bpamem0! err = %d", (int)*memfd);
		return -1;
	}

	BPAMemAllocMemData bpa_data;
	bpa_data.bpa_part = "LMI_VID";
	bpa_data.mem_size = size;
	int res;
	res = ioctl(*memfd, BPAMEMIO_ALLOCMEM, &bpa_data); // request memory from bpamem
	if(res)
	{
		err("cannot alloc required bpa mem");
		close(*memfd);
		return -1;
	}

	char* bpa_mem_device = NULL;
	bpa_mem_device = malloc(30);
	if(bpa_mem_device == NULL)
	{
		err("no mem");
		return -1;
	}

	sprintf(bpa_mem_device, "/dev/bpamem%d", bpa_data.device_num);
	close(*memfd);

	*memfd = open(bpa_mem_device, O_RDWR);

	if(*memfd < 0)
	{
		free(bpa_mem_device);
		err("cannot access %s! err = %d", bpa_mem_device, (int)*memfd);
		return -1;
	}

	*mem = (unsigned char *)mmap(0, bpa_data.mem_size, PROT_WRITE|PROT_READ, MAP_SHARED, *memfd, 0);

	if(*mem == MAP_FAILED) 
	{
		err("could not map bpa mem");
		free(bpa_mem_device);
		ioctl(*memfd, BPAMEMIO_FREEMEM);
		close(*memfd);
		return -1;
	}
	
	free(bpa_mem_device);
	return 0;
}

void freebpamem(int memfd, unsigned char* mem, size_t len)
{
	munmap(mem, len);
	ioctl(memfd, BPAMEMIO_FREEMEM);
	close(memfd);
}

struct pic* addpic(char *name, unsigned char* picbuf, int memfd, unsigned long width, unsigned long height, unsigned long rowbytes, int channels, int del, struct pic* last)
{
	debug(1000, "in");
	struct pic *newnode = NULL, *prev = NULL, *node = pic;

	newnode = (struct pic*)malloc(sizeof(struct pic));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct pic));

	newnode->name = ostrcat(name, "", 0, 0);
	newnode->picbuf = picbuf;
	newnode->memfd = memfd;
	newnode->width = width;
	newnode->height = height;
	newnode->rowbytes = rowbytes;
	newnode->channels = channels;
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

	debug(1000, "out");
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
	debug(1000, "in");
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
	debug(1000, "out");
}

void delmarkedpic(int del)
{
	debug(1000, "in");
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
	debug(1000, "out");
}

struct pic* getpic(char* name)
{
	debug(1000, "in");
	struct pic *node = pic;
	char* tmpstr = NULL;

	tmpstr = changepicpath(name);

	while(node != NULL)
	{
		if(ostrcmp(node->name, tmpstr) == 0)
		{
			debug(1000, "out");
			free(tmpstr);
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
	debug(1000, "in");
	struct pic *node = pic, *prev = pic;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delpic(prev->name);
	}
	debug(1000, "out");
}

#endif
