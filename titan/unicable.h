#ifndef UNICABLE_H
#define UNICABLE_H

struct unicable* addunicable(char *line, int count, struct unicable* last)
{
	struct unicable *newnode = NULL, *prev = NULL, *node = unicable;
	char *manufacturer = NULL, *product = NULL;;
	int ret = 0;

	newnode = (struct unicable*)calloc(1, sizeof(struct unicable));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	manufacturer = malloc(MINMALLOC);
	if(manufacturer == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	product = malloc(MINMALLOC);
	if(product == NULL)
	{
		err("no memory");
		free(newnode);
		free(product);
		return NULL;
	}

	ret = sscanf(line, "%[^#]#%[^#]#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d", manufacturer, product, &newnode->scr1, &newnode->scr2, &newnode->scr3, &newnode->scr4, &newnode->scr5, &newnode->scr6, &newnode->scr7, &newnode->scr8, &newnode->scr9, &newnode->scr10, &newnode->scr11, &newnode->scr12, &newnode->scr13, &newnode->scr14, &newnode->scr15, &newnode->scr16, &newnode->scr17, &newnode->scr18, &newnode->scr19, &newnode->scr20, &newnode->scr21, &newnode->scr22, &newnode->scr23, &newnode->scr24, &newnode->scr25, &newnode->scr26, &newnode->scr27, &newnode->scr28, &newnode->scr29, &newnode->scr30, &newnode->scr31, &newnode->scr32, &newnode->lofl, &newnode->lofh);
	if(ret != 12)
	{
		if(count > 0)
		{
			err("unicablelist line %d not ok", count);
		}
		else
		{
			err("add unicable");
		}
		free(manufacturer);
		free(product);
		free(newnode);
		return NULL;
	}

	newnode->manufacturer = ostrshrink(manufacturer);
	newnode->product = ostrshrink(product);

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
		unicable = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	return newnode;
}

int readunicable(const char* filename)
{
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct unicable* last = NULL;
	
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

		linecount++;

		last = addunicable(fileline, linecount, last);
	}
	
	free(fileline);
	fclose(fd);
	return 0;
}

void delunicable(struct unicable* unicablenode)
{
	struct unicable *node = unicable, *prev = unicable;

	while(node != NULL)
	{
		if(node == unicablenode)
		{
			if(node == unicable)
			{
				unicable = node->next;
				if(unicable != NULL)
					unicable->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->manufacturer);
			node->manufacturer = NULL;
			free(node->product);
			node->product = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freeunicable()
{
	struct unicable *node = unicable, *prev = unicable;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delunicable(prev);
	}
}

void screenunicable(struct skin* loftype, struct skin* lofl, struct skin* lofh, struct skin* satcr, struct skin* satcrfrequ2)
{
	int i = 0;
	struct menulist* mlist = NULL, *mbox = NULL;
	struct unicable* node = NULL;
	char* tmpnr = NULL;
	char* manufacturer = NULL;
	char* product = NULL;

	if(loftype == NULL || lofl == NULL || lofh == NULL || satcr == NULL || satcrfrequ2 == NULL)
		return;

	readunicable(getconfig("unicablefile", NULL));

manufactor:
	//show manufacturer
	freemenulist(mlist, 1); mlist = NULL; mbox = NULL;
	node = unicable;
	while(node != NULL)
	{
		if(getmenulist(mlist, node->manufacturer) == NULL)
			addmenulist(&mlist, node->manufacturer, NULL, NULL, 0, 0);
		node = node->next;
	}
	mbox = menulistbox(mlist, "menulist", _("Unicable manufacturer"), _("Choose your manufacturer from the following list"), NULL, NULL, 0, 1);

	if(mbox != NULL)
	{
		//show product
		free(manufacturer); manufacturer = NULL;
		manufacturer = ostrcat(mbox->name, NULL, 0, 0);
		freemenulist(mlist, 1); mlist = NULL; mbox = NULL;
		node = unicable;
		while(node != NULL)
		{
			if(ostrcmp(manufacturer, node->manufacturer) == 0)
				addmenulist(&mlist, node->product, NULL, NULL, 0, 0);
			node = node->next;
		}
		mbox = menulistbox(mlist, "menulist", _("Unicable product"), _("Choose your Unicable product from the following list"), NULL, NULL, 0, 1);

		if(mbox != NULL)
		{
			//show scr
			free(product); product = NULL;
			product = ostrcat(mbox->name, NULL, 0, 0);
			freemenulist(mlist, 1); mlist = NULL; mbox = NULL;
			node = unicable;
			while(node != NULL)
			{
				if(ostrcmp(manufacturer, node->manufacturer) == 0 && ostrcmp(product, node->product) == 0)
				{
					if(node->scr1 != 0)
					{
						tmpnr = oitoa(node->scr1);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr2 != 0)
					{
						tmpnr = oitoa(node->scr2);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr3 != 0)
					{
						tmpnr = oitoa(node->scr3);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr4 != 0)
					{
						tmpnr = oitoa(node->scr4);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr5 != 0)
					{
						tmpnr = oitoa(node->scr5);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr6 != 0)
					{
						tmpnr = oitoa(node->scr6);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr7 != 0)
					{
						tmpnr = oitoa(node->scr7);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr8 != 0)
					{
						tmpnr = oitoa(node->scr8);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr9 != 0)
					{
						tmpnr = oitoa(node->scr9);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr10 != 0)
					{
						tmpnr = oitoa(node->scr10);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr11 != 0)
					{
						tmpnr = oitoa(node->scr11);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr12 != 0)
					{
						tmpnr = oitoa(node->scr12);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr13 != 0)
					{
						tmpnr = oitoa(node->scr13);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr14 != 0)
					{
						tmpnr = oitoa(node->scr14);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr15 != 0)
					{
						tmpnr = oitoa(node->scr15);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr16 != 0)
					{
						tmpnr = oitoa(node->scr16);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr17 != 0)
					{
						tmpnr = oitoa(node->scr17);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr18 != 0)
					{
						tmpnr = oitoa(node->scr18);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr19 != 0)
					{
						tmpnr = oitoa(node->scr19);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr20 != 0)
					{
						tmpnr = oitoa(node->scr20);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr21 != 0)
					{
						tmpnr = oitoa(node->scr21);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr22 != 0)
					{
						tmpnr = oitoa(node->scr22);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr23 != 0)
					{
						tmpnr = oitoa(node->scr23);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr24 != 0)
					{
						tmpnr = oitoa(node->scr24);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr25 != 0)
					{
						tmpnr = oitoa(node->scr25);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr26 != 0)
					{
						tmpnr = oitoa(node->scr26);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr27 != 0)
					{
						tmpnr = oitoa(node->scr27);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr28 != 0)
					{
						tmpnr = oitoa(node->scr28);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr29 != 0)
					{
						tmpnr = oitoa(node->scr29);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr30 != 0)
					{
						tmpnr = oitoa(node->scr30);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr31 != 0)
					{
						tmpnr = oitoa(node->scr31);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					if(node->scr32 != 0)
					{
						tmpnr = oitoa(node->scr32);
						addmenulist(&mlist, tmpnr, NULL, NULL, 0, 0);
						free(tmpnr); tmpnr = NULL;
					}
					break;
				}
				node = node->next;
			}
			mbox = menulistbox(mlist, "menulist", _("Unicable SCR"), _("Choose your Unicable SCR from the following list"), NULL, NULL, 0, 1);
			if(mbox != NULL && mbox->name != NULL)
			{
				//change lnbconfig
				node = unicable;
				while(node != NULL)
				{
					if(ostrcmp(manufacturer, node->manufacturer) == 0 && ostrcmp(product, node->product) == 0)
					{
						int scr = 0;

						setchoiceboxselection(loftype, "4");
						tmpnr = oitoa(node->lofl);
						changeinput(lofl, tmpnr);
						lofl->input = mask(lofl->input, 6, "0");
						free(tmpnr); tmpnr = NULL;

						tmpnr = oitoa(node->lofh);
						changeinput(lofh, tmpnr);
						lofh->input = mask(lofh->input, 6, "0");
						free(tmpnr); tmpnr = NULL;

						scr = atoi(mbox->name);
						if(scr == node->scr1) setchoiceboxselection(satcr, "1");
						if(scr == node->scr2) setchoiceboxselection(satcr, "2");
						if(scr == node->scr3) setchoiceboxselection(satcr, "3");
						if(scr == node->scr4) setchoiceboxselection(satcr, "4");
						if(scr == node->scr5) setchoiceboxselection(satcr, "5");
						if(scr == node->scr6) setchoiceboxselection(satcr, "6");
						if(scr == node->scr7) setchoiceboxselection(satcr, "7");
						if(scr == node->scr8) setchoiceboxselection(satcr, "8");
						if(scr == node->scr9) setchoiceboxselection(satcr, "9");
						if(scr == node->scr10) setchoiceboxselection(satcr, "10");
						if(scr == node->scr11) setchoiceboxselection(satcr, "11");
						if(scr == node->scr12) setchoiceboxselection(satcr, "12");
						if(scr == node->scr13) setchoiceboxselection(satcr, "13");
						if(scr == node->scr14) setchoiceboxselection(satcr, "14");
						if(scr == node->scr15) setchoiceboxselection(satcr, "15");
						if(scr == node->scr16) setchoiceboxselection(satcr, "16");
						if(scr == node->scr17) setchoiceboxselection(satcr, "17");
						if(scr == node->scr18) setchoiceboxselection(satcr, "18");
						if(scr == node->scr19) setchoiceboxselection(satcr, "19");
						if(scr == node->scr20) setchoiceboxselection(satcr, "20");
						if(scr == node->scr21) setchoiceboxselection(satcr, "21");
						if(scr == node->scr22) setchoiceboxselection(satcr, "22");
						if(scr == node->scr23) setchoiceboxselection(satcr, "23");
						if(scr == node->scr24) setchoiceboxselection(satcr, "24");
						if(scr == node->scr25) setchoiceboxselection(satcr, "25");
						if(scr == node->scr26) setchoiceboxselection(satcr, "26");
						if(scr == node->scr27) setchoiceboxselection(satcr, "27");
						if(scr == node->scr28) setchoiceboxselection(satcr, "28");
						if(scr == node->scr29) setchoiceboxselection(satcr, "29");
						if(scr == node->scr30) setchoiceboxselection(satcr, "30");
						if(scr == node->scr31) setchoiceboxselection(satcr, "31");
						if(scr == node->scr32) setchoiceboxselection(satcr, "32");

						changeinput(satcrfrequ2, mbox->name);
						if(satcrfrequ2->input != NULL)
						{
							int len = strlen(satcrfrequ2->input);
							for(i = 0; i < 4 - len; i++)
								satcrfrequ2->input = ostrcat("0", satcrfrequ2->input, 0, 1);
						}
						break;
					}
					node = node->next;
				}
			}
			else
				goto manufactor;
		}
		else
			goto manufactor;
	}

	free(manufacturer); manufacturer = NULL;
	free(product); product = NULL;
	freemenulist(mlist, 1); mlist = NULL;
	freeunicable();
}

#endif

