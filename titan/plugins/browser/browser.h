#ifndef BROWSER_H
#define BROWSER_H

struct browserfav
{
	char* name;
	char* addr;
	struct browserfav* next;
};

extern struct skin* skin;
struct browserfav *browserfav = NULL;

struct browserfav* addbrowserfav(char *line, int count, struct browserfav* last)
{
	//debug(1000, "in");
	struct browserfav *newnode = NULL, *prev = NULL, *node = browserfav;
	int ret = 0;

	newnode = (struct browserfav*)malloc(sizeof(struct browserfav));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct browserfav));

	newnode->name = malloc(1024);
	if(newnode->name == NULL)
	{
		err("no mem");
		free(newnode);
		return NULL;
	}

	newnode->addr = malloc(1024);
	if(newnode->addr == NULL)
	{
		err("no mem");
		free(newnode->name);
		free(newnode);
		return NULL;
	}

	ret = sscanf(line, "%[^#]#%s", newnode->name, newnode->addr);
	if(ret != 2)
	{
		if(count > 0)
		{
			err("browserfav line %d not ok", count);
		}
		else
		{
			err("add browserfav");
		}
		free(newnode->name);
		free(newnode->addr);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(newnode->name, NULL, 1, 0);
	newnode->addr = ostrcat(newnode->addr, NULL, 1, 0);

	if(last == NULL)
	{
		while(node != NULL)
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
		browserfav = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	//debug(1000, "out");
	return newnode;
}

int readbrowserfav(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct browserfav* last = NULL, *tmplast = NULL;

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
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addbrowserfav(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int delbrowserfav(char* name)
{
	debug(1000, "in");
	int ret = 1;
	struct browserfav *node = browserfav, *prev = browserfav;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			if(node == browserfav)
				browserfav = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			ret = 0;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
	return ret;
}

void freebrowserfav()
{
	debug(1000, "in");
	struct browserfav *node = browserfav, *prev = browserfav;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delbrowserfav(prev->name);
	}
	debug(1000, "out");
}

void screenbrowser()
{
	char* tmpstr = NULL;
	int ret = 0;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmbox = NULL;
	struct browserfav *node = NULL;

	ret = servicestop(status.aktservice, 1, 0);
	if(ret == 1) return;
	setfbtransparent(255);

	readbrowserfav(getconfig("browserfavfile", NULL));
	node = browserfav;

	tmpmbox = addmenulist(&mlist, "Home", NULL, NULL, 0, 0);
	if(tmpmbox != NULL)
		tmpmbox->param = ostrcat(getconfig("browserhome", NULL), NULL, 0, 0);

	while(node != NULL)
	{
		tmpmbox = addmenulist(&mlist, node->name, NULL, NULL, 0, 0);
		if(tmpmbox != NULL)
			tmpmbox->param = ostrcat(node->addr, NULL, 0, 0);

		node = node->next;
	}

start:
	drawscreen(skin, 0);
	mbox = menulistbox(mlist, "menulist", "Browser Favoriten", NULL, NULL, 0, 0);
	if(mbox != NULL)
	{
		if(mbox->param != NULL)
		{
			status.sec = 0; //deaktivate spinner
			tmpstr = ostrcat("nsfb -f linux ", mbox->param, 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		goto start;
	}

	freemenulist(mlist, 1);
	freebrowserfav();
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	if(status.lastservice != NULL)
		servicestart(status.lastservice->channel, NULL, NULL, 0);
	flushrc(500);
}

#endif
