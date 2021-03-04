#ifndef NEWSLETTER_H
#define NEWSLETTER_H

struct newsletter* addnewsletter(char *line, int count, struct newsletter* last)
{
	struct newsletter *newnode = NULL, *prev = NULL, *node = newsletter;
	char *date = NULL, *title = NULL, *text = NULL;
	int ret = 0;

	newnode = (struct newsletter*)calloc(1, sizeof(struct newsletter));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	date = malloc(MINMALLOC);
	if(date == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	title = malloc(MINMALLOC);
	if(title == NULL)
	{
		err("no memory");
		free(date);
		free(newnode);
		return NULL;
	}

	text = malloc(MINMALLOC * 10);
	if(text == NULL)
	{
		err("no memory");
		free(date);
		free(title);
		free(newnode);
		return NULL;
	}

	ret = sscanf(line, "%lu#%[^#]#%[^#]#%[^#]", &newnode->nr, date, title, text);
	if(ret != 4)
	{
		if(count > 0)
		{
			err("newsletter line %d not ok or double", count);
		}
		else
		{
			err("add newsletter");
		}
		free(date);
		free(title);
		free(text);
		free(newnode);
		return NULL;
	}

	newnode->date = ostrshrink(date);
	newnode->title = ostrshrink(title);
	newnode->text = ostrshrink(text);
	newnode->text = string_replace_all("\\n", "\n", newnode->text, 1);

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
		newsletter = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	return newnode;
}

int readnewsletter()
{
	FILE *fd = NULL;
	char* fileline = NULL;
	int linecount = 0, len = 0;
	struct newsletter* last = NULL, *tmplast = NULL;
	char* newsletterfile = "/tmp/newsletter.txt";

	unlink(newsletterfile);

//	if(checkbox("WHITEBOX") == 1)
//		gethttp("openaaf.dyndns.tv", "/mediathek/newsletter.crenova.txt", 80, newsletterfile, HTTPAUTH, 5000, NULL, 0);
//	else
		gethttp("openaaf.dyndns.tv", "/mediathek/newsletter.txt", 80, newsletterfile, HTTPAUTH, 5000, NULL, 0);
	
	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(newsletterfile, "r");
	if(fd == NULL)
	{
		perr("can't open %s", newsletterfile);
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

		linecount++;

		if(last == NULL) last = tmplast;
		last = addnewsletter(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	unlink(newsletterfile);
	return 0;
}

int delnewsletter(struct newsletter* nnode)
{
	int ret = 1;
	struct newsletter *node = newsletter, *prev = newsletter;

	while(node != NULL)
	{
		if(nnode == node)
		{
			ret = 0;
			if(node == newsletter)
				newsletter = node->next;
			else
				prev->next = node->next;

			free(node->date);
			node->date = NULL;

			free(node->title);
			node->date = NULL;

			free(node->text);
			node->text = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	return ret;
}

void freenewsletter()
{
	struct newsletter *node = newsletter, *prev = newsletter;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delnewsletter(prev);
	}
}

void screennewsletter()
{
	int rcret = -1;
	struct skin* loading = getscreen("loading");
	struct skin* skinnewsletter = getscreen("newsletter");
	struct skin* listbox = getscreennode(skinnewsletter, "listbox");
	struct skin* tmp = NULL;
	struct newsletter* node = NULL;
	char* tmpstr = NULL;

	drawscreen(loading, 0, 0);

	m_lock(&status.newslettermutex, 19);
	long unsigned lastnewsletter = getconfiglu("lastnewsletter", NULL);

	readnewsletter();
	listbox->aktline = 1;
	listbox->aktpage = -1;

	node = newsletter;

	while(node != NULL)
	{
		tmp = addlistbox(skinnewsletter, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(node->title, NULL, 0, 0);
			tmpstr = ostrcat(tmpstr, " - ", 1, 0);
			tmpstr = ostrcat(tmpstr, node->date, 1, 0);

			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->del = 1;
			tmp->handle = (char*)node;
		}

		if(node->nr > lastnewsletter)
			addconfiglu("lastnewsletter", node->nr);

		node = node->next;
	}

	drawscreen(skinnewsletter, 0, 0);
	addscreenrc(skinnewsletter, listbox);

	while(1)
	{
		rcret = waitrc(skinnewsletter, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				tmpstr = ostrcat(((struct newsletter*)listbox->select->handle)->title, NULL, 0, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				tmpstr = ostrcat(tmpstr, ((struct newsletter*)listbox->select->handle)->date, 1, 0);
				textbox(tmpstr, ((struct newsletter*)listbox->select->handle)->text, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 600, 0, 2);
				free(tmpstr); tmpstr = NULL;
				drawscreen(skinnewsletter, 0, 0);
			}
		}
	}

	delmarkedscreennodes(skinnewsletter, 1);
	freenewsletter();
	delownerrc(skinnewsletter);
	clearscreen(skinnewsletter);
	m_unlock(&status.newslettermutex, 19);
}

void newsletterthreadfunc(struct stimerthread* self)
{
	char* tmpstr = NULL;
	struct newsletter* node = NULL;

	if(self == NULL) return;

	debug(427, "start newsletter thread");

	while(self->aktion != STOP)
	{
		int count = 0, ret = 0;

		if(status.standby == 0)
		{
			m_lock(&status.newslettermutex, 19);
			long unsigned lastnewsletter = getconfiglu("lastnewsletter", NULL);
			readnewsletter();
			node = newsletter;

			if(node != NULL && node->nr > lastnewsletter)
			{
				tmpstr = ostrcat(node->title, NULL, 0, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				tmpstr = ostrcat(tmpstr, node->date, 1, 0);

				ret = textbox(tmpstr, node->text, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 700, 15, 2);
				free(tmpstr); tmpstr = NULL;

				//mark only if no timeout
				if(ret != 0)
				{
					addconfiglu("lastnewsletter", node->nr);
					writeallconfig(3);
				}
			}

			freenewsletter();
			m_unlock(&status.newslettermutex, 19);
		}

		//wait 1h
		while(count < 3600)
		{
			if(self->aktion == STOP) break;
			sleep(1);
			count++;
		}
	}

	debug(427, "end newsletter thread");
}

#endif
