#ifndef RCFUNC_H
#define RCFUNC_H

void progressbarright(struct skin* screen, struct skin* progressbar)
{
	debug(1000, "in");

	if(progressbar != NULL)
	{
		if(progressbar->progresssize <= 100)
		{
			progressbar->progresssize++;
			drawscreen(screen, 0);
		}
		else
			progressbar->progresssize = 100;
	}

	debug(1000, "out");
}

void progressbarleft(struct skin* screen, struct skin* progressbar)
{
	debug(1000, "in");

	if(progressbar != NULL)
	{
		if(progressbar->progresssize > 0)
		{
			progressbar->progresssize--;
			drawscreen(screen, 0);
		}
		else
			progressbar->progresssize = 0;
	}

	debug(1000, "out");
}

void filelistok(struct skin* screen, struct skin* filelist)
{
	debug(1000, "in");
	struct skin* path = NULL;
	char* tmp = NULL, *lastdir = NULL, *plastdir = NULL;
	int inplen = 0;

	if(filelist != NULL)
	{
		if(filelist->select != NULL && filelist->select->input != NULL)
		{
			inplen = strlen(filelist->select->input);
			if(inplen >= 3 && filelist->select->input[inplen - 1] == '.' && filelist->select->input[inplen - 2] == '.' && filelist->select->input[inplen - 3] == '/')
			{
				lastdir = ostrcat(lastdir, filelist->select->input, 1, 0);
				lastdir[inplen - 3] = '\0';
				plastdir = strrchr(lastdir, '/');
				if(plastdir == NULL)
					plastdir = lastdir;
				else
					plastdir++;
			}

			filelist->aktline = 0;
			filelist->aktpage = 0;
			tmp = createpath(filelist->select->input, "");
			changeinput(filelist, tmp);
			free(tmp); tmp = NULL;

			tmp = malloc(strlen(filelist->name) + 5);
			if(tmp != NULL)
			{
				sprintf(tmp, "%s%s", filelist->name, "path");
				path = getscreennode(screen, tmp);
				free(tmp);
				if(path != status.skinerr)
					changetext(path, filelist->input);
				delmarkedscreennodes(screen, FILELISTDELMARK);
				createfilelist(screen, filelist);
				setlistboxselection(filelist, plastdir);
				free(lastdir); lastdir = NULL;
				drawscreen(screen, 0);
			}
			else
				err("no memory");
		}
	}
	debug(1000, "out");
}

void inputboxhelp(struct skin* screen, struct skin* inputbox)
{
	char* tmpstr = NULL;

	if(inputbox != NULL && inputbox->type == INPUTBOX)
	{
		tmpstr = screeninputhelp(inputbox->input);
		changeinput(inputbox, tmpstr);
		free(tmpstr); tmpstr = NULL;
		drawscreen(screen, 0);
	}
}

void inputboxchar(struct skin* screen, struct skin* inputbox, char zeichen, int flag)
{
	debug(1000, "in");
	if(inputbox != NULL && inputbox->input != NULL)
	{
		if(strlen(inputbox->input) > 0)
		{
			inputbox->input[inputbox->aktpage - 1] = zeichen;
			if(flag == 0)
				drawscreen(screen, 0);
		}
	}
	debug(1000, "out");
}

void inputboxff(struct skin* screen, struct skin* inputbox)
{
	debug(1000, "in");
	if(inputbox != NULL)
	{
		if(inputbox->input == NULL)
			changeinput(inputbox, "");
		inputbox->aktpage = insertchar(&inputbox->input, '_', inputbox->aktpage);
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void inputboxfr(struct skin* screen, struct skin* inputbox)
{
	debug(1000, "in");
	if(inputbox != NULL && inputbox->input != NULL)
	{
		inputbox->aktpage = delchar(&inputbox->input, inputbox->aktpage);
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void checkinputboxnumright(struct skin* inputbox)
{
	debug(1000, "in");
	int count = 0;

	if(inputbox != NULL && inputbox->input != NULL && inputbox->type == INPUTBOXNUM)
	{
		while(isdigit(inputbox->input[inputbox->aktpage - 1]) == 0)
		{
			inputbox->aktpage++;
			if(inputbox->input[inputbox->aktpage - 1] == '\0')
			{
				count++;
				inputbox->aktpage = 0;
			}
			if(count > 1)
			{
				inputbox->aktpage = strlen(inputbox->input) + 1;
				break;
			}
		}
	}
	debug(1000, "out");
}

void checkinputboxnumleft(struct skin* inputbox)
{
	debug(1000, "in");
	int count = 0;

	if(inputbox != NULL && inputbox->input != NULL && inputbox->type == INPUTBOXNUM)
	{
		while(isdigit(inputbox->input[inputbox->aktpage - 1]) == 0)
		{
			inputbox->aktpage--;
			if(inputbox->aktpage < 0)
			{
				count++;
				inputbox->aktpage = strlen(inputbox->input);
			}
			if(count > 1)
			{
				inputbox->aktpage = strlen(inputbox->input) + 1;
				break;
			}
		}
	}
	debug(1000, "out");
}

void inputboxright(struct skin* screen, struct skin* inputbox)
{
	debug(1000, "in");

	if(inputbox != NULL && inputbox->input != NULL)
	{
		inputbox->aktpage++;
		if(inputbox->aktpage >= strlen(inputbox->input) + 1) inputbox->aktpage = 1;

		checkinputboxnumright(inputbox);
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void inputboxleft(struct skin* screen, struct skin* inputbox)
{
	debug(1000, "in");

	if(inputbox != NULL && inputbox->input != NULL)
	{
		inputbox->aktpage--;
		if(inputbox->aktpage < 1) inputbox->aktpage = strlen(inputbox->input);

		checkinputboxnumleft(inputbox);
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void inputbox0(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '0')
				inputboxchar(screen, inputbox, ' ', 0);
			else if(inputbox->input[aktpage] == ' ')
				inputboxchar(screen, inputbox, '#', 0);
			else if(inputbox->input[aktpage] == '#')
				inputboxchar(screen, inputbox, '*', 0);
			else if(inputbox->input[aktpage] == '*')
				inputboxchar(screen, inputbox, '_', 0);
			else
				inputboxchar(screen, inputbox, '0', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '0', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox1(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '1')
				inputboxchar(screen, inputbox, '.', 0);
			else if(inputbox->input[aktpage] == '.')
				inputboxchar(screen, inputbox, ',', 0);
			else if(inputbox->input[aktpage] == ',')
				inputboxchar(screen, inputbox, '?', 0);
			else if(inputbox->input[aktpage] == '?')
				inputboxchar(screen, inputbox, '!', 0);
			else if(inputbox->input[aktpage] == '!')
				inputboxchar(screen, inputbox, '\'', 0);
			else if(inputbox->input[aktpage] == '\'')
				inputboxchar(screen, inputbox, '-', 0);
			else if(inputbox->input[aktpage] == '-')
				inputboxchar(screen, inputbox, '(', 0);
			else if(inputbox->input[aktpage] == '(')
				inputboxchar(screen, inputbox, ')', 0);
			else if(inputbox->input[aktpage] == ')')
				inputboxchar(screen, inputbox, '@', 0);
			else if(inputbox->input[aktpage] == '@')
				inputboxchar(screen, inputbox, '/', 0);
			else if(inputbox->input[aktpage] == '/')
				inputboxchar(screen, inputbox, ':', 0);
			else
				inputboxchar(screen, inputbox, '1', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '1', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox2(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '2')
				inputboxchar(screen, inputbox, 'a', 0);
			else if(inputbox->input[aktpage] == 'a')
				inputboxchar(screen, inputbox, 'b', 0);
			else if(inputbox->input[aktpage] == 'b')
				inputboxchar(screen, inputbox, 'c', 0);
			else if(inputbox->input[aktpage] == 'c')
				inputboxchar(screen, inputbox, 'A', 0);
			else if(inputbox->input[aktpage] == 'A')
				inputboxchar(screen, inputbox, 'B', 0);
			else if(inputbox->input[aktpage] == 'B')
				inputboxchar(screen, inputbox, 'C', 0);
			else
				inputboxchar(screen, inputbox, '2', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '2', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox3(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '3')
				inputboxchar(screen, inputbox, 'd', 0);
			else if(inputbox->input[aktpage] == 'd')
				inputboxchar(screen, inputbox, 'e', 0);
			else if(inputbox->input[aktpage] == 'e')
				inputboxchar(screen, inputbox, 'f', 0);
			else if(inputbox->input[aktpage] == 'f')
				inputboxchar(screen, inputbox, 'D', 0);
			else if(inputbox->input[aktpage] == 'D')
				inputboxchar(screen, inputbox, 'E', 0);
			else if(inputbox->input[aktpage] == 'E')
				inputboxchar(screen, inputbox, 'F', 0);
			else
				inputboxchar(screen, inputbox, '3', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '3', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox4(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '4')
				inputboxchar(screen, inputbox, 'g', 0);
			else if(inputbox->input[aktpage] == 'g')
				inputboxchar(screen, inputbox, 'h', 0);
			else if(inputbox->input[aktpage] == 'h')
				inputboxchar(screen, inputbox, 'i', 0);
			else if(inputbox->input[aktpage] == 'i')
				inputboxchar(screen, inputbox, 'G', 0);
			else if(inputbox->input[aktpage] == 'G')
				inputboxchar(screen, inputbox, 'H', 0);
			else if(inputbox->input[aktpage] == 'H')
				inputboxchar(screen, inputbox, 'I', 0);
			else
				inputboxchar(screen, inputbox, '4', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '4', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox5(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '5')
				inputboxchar(screen, inputbox, 'j', 0);
			else if(inputbox->input[aktpage] == 'j')
				inputboxchar(screen, inputbox, 'k', 0);
			else if(inputbox->input[aktpage] == 'k')
				inputboxchar(screen, inputbox, 'l', 0);
			else if(inputbox->input[aktpage] == 'l')
				inputboxchar(screen, inputbox, 'J', 0);
			else if(inputbox->input[aktpage] == 'J')
				inputboxchar(screen, inputbox, 'K', 0);
			else if(inputbox->input[aktpage] == 'K')
				inputboxchar(screen, inputbox, 'L', 0);
			else
				inputboxchar(screen, inputbox, '5', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '5', 1);
			inputboxright(screen, inputbox);
		}

	}
}

void inputbox6(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '6')
				inputboxchar(screen, inputbox, 'm', 0);
			else if(inputbox->input[aktpage] == 'm')
				inputboxchar(screen, inputbox, 'n', 0);
			else if(inputbox->input[aktpage] == 'n')
				inputboxchar(screen, inputbox, 'o', 0);
			else if(inputbox->input[aktpage] == 'o')
				inputboxchar(screen, inputbox, 'M', 0);
			else if(inputbox->input[aktpage] == 'M')
				inputboxchar(screen, inputbox, 'N', 0);
			else if(inputbox->input[aktpage] == 'N')
				inputboxchar(screen, inputbox, 'O', 0);
			else
				inputboxchar(screen, inputbox, '6', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '6', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox7(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '7')
				inputboxchar(screen, inputbox, 'p', 0);
			else if(inputbox->input[aktpage] == 'p')
				inputboxchar(screen, inputbox, 'q', 0);
			else if(inputbox->input[aktpage] == 'q')
				inputboxchar(screen, inputbox, 'r', 0);
			else if(inputbox->input[aktpage] == 'r')
				inputboxchar(screen, inputbox, 's', 0);
			else if(inputbox->input[aktpage] == 's')
				inputboxchar(screen, inputbox, 'P', 0);
			else if(inputbox->input[aktpage] == 'P')
				inputboxchar(screen, inputbox, 'Q', 0);
			else if(inputbox->input[aktpage] == 'Q')
				inputboxchar(screen, inputbox, 'R', 0);
			else if(inputbox->input[aktpage] == 'R')
				inputboxchar(screen, inputbox, 'S', 0);
			else
				inputboxchar(screen, inputbox, '7', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '7', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox8(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '8')
				inputboxchar(screen, inputbox, 't', 0);
			else if(inputbox->input[aktpage] == 't')
				inputboxchar(screen, inputbox, 'u', 0);
			else if(inputbox->input[aktpage] == 'u')
				inputboxchar(screen, inputbox, 'v', 0);
			else if(inputbox->input[aktpage] == 'v')
				inputboxchar(screen, inputbox, 'T', 0);
			else if(inputbox->input[aktpage] == 'T')
				inputboxchar(screen, inputbox, 'U', 0);
			else if(inputbox->input[aktpage] == 'U')
				inputboxchar(screen, inputbox, 'V', 0);
			else
				inputboxchar(screen, inputbox, '8', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '8', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void inputbox9(struct skin* screen, struct skin* inputbox)
{
	if(inputbox != NULL && inputbox->input != NULL)
	{
		int aktpage = inputbox->aktpage - 1;

		if(inputbox->type == INPUTBOX)
		{
			if(inputbox->input[aktpage] == '9')
				inputboxchar(screen, inputbox, 'w', 0);
			else if(inputbox->input[aktpage] == 'w')
				inputboxchar(screen, inputbox, 'x', 0);
			else if(inputbox->input[aktpage] == 'x')
				inputboxchar(screen, inputbox, 'y', 0);
			else if(inputbox->input[aktpage] == 'y')
				inputboxchar(screen, inputbox, 'z', 0);
			else if(inputbox->input[aktpage] == 'z')
				inputboxchar(screen, inputbox, 'W', 0);
			else if(inputbox->input[aktpage] == 'W')
				inputboxchar(screen, inputbox, 'X', 0);
			else if(inputbox->input[aktpage] == 'X')
				inputboxchar(screen, inputbox, 'Y', 0);
			else if(inputbox->input[aktpage] == 'Y')
				inputboxchar(screen, inputbox, 'Z', 0);
			else
				inputboxchar(screen, inputbox, '9', 0);
		}
		else
		{
			inputboxchar(screen, inputbox, '9', 1);
			inputboxright(screen, inputbox);
		}
	}
}

void choiceboxleft(struct skin* screen, struct skin* choicebox)
{
	debug(1000, "in");
	if(choicebox != NULL)
	{
		if(choicebox->aktpage <= 1)
			choicebox->aktpage = choicebox->linecount;
		else
			choicebox->aktpage--;
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void choiceboxright(struct skin* screen, struct skin* choicebox)
{
	debug(1000, "in");
	if(choicebox != NULL)
	{
		if(choicebox->aktpage >= choicebox->linecount)
			choicebox->aktpage = 1;
		else
		{
			if(choicebox->aktpage < 1) choicebox->aktpage = 1;
			choicebox->aktpage++;
		}
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void griddown(struct skin* screen, struct skin* grid)
{
	debug(1000, "in");
	int end = 0, br = 0, mark = 0;
	struct skin* node = NULL;

	if(grid != NULL)
	{
		if(grid->select != NULL && grid->gridcol == 0)
			grid->gridcol = grid->select->posx + (grid->select->width / 2);

		node = grid->select;
		while(node != NULL)
		{
			if(node->next == NULL || node->deaktivcol != -1 || node->hidden == YES || (node->parentpointer != grid && ostrcmp(node->parent, grid->name) != 0))
			{
				node = node->next;
				if(node == NULL && mark == 0)
				{
					//go to the first cell
					node = grid->next;
					mark = 1;
					grid->aktline = 1;
				}
				continue;
			}

			if((node->type == GRIDBR || node->type == TEXTBOXGRIDBR) && node != grid->select) br++;
			if(br > 1)
			{
				grid->aktline--;
				break;
			}

			if(br > 0)
			{
				end = node->posx + node->width;
				if(end <= grid->gridcol)
				{
					grid->aktline++;
					node = node->next;
					continue;
				}
				break;
			}

			grid->aktline++;
			node = node->next;
		}

		grid->aktpage = -1;
		if(grid->aktline > grid->linecount || grid->aktline < 1)
			grid->aktline = grid->linecount;

		if(grid->select != NULL) delscreenrc(screen, grid->select);
		drawscreen(screen, 0);
	}

	debug(1000, "out");
}

void gridup(struct skin* screen, struct skin* grid)
{
	debug(1000, "in");
	int start = 0, br = 0, mark = 0;
	struct skin* node = NULL, *tmpnode = NULL;;

	if(grid != NULL)
	{
		if(grid->select != NULL && grid->gridcol == 0)
			grid->gridcol = grid->select->posx + (grid->select->width / 2);

		node = grid->select;
		while(node != NULL)
		{
			if(node->prev == grid || node->deaktivcol != -1 || node->hidden == YES || (node->parentpointer != grid && ostrcmp(node->parent, grid->name) != 0))
			{
				node = node->prev;
				if(node == grid && mark == 0)
				{
					while(node != NULL)
					{
						if(node->parentpointer == grid || ostrcmp(node->parent, grid->name) == 0)
							tmpnode = node;
						node = node->next;
					}
					if(tmpnode == NULL) break;
					node = tmpnode;
					grid->aktline = grid->linecount;
					if(br == 0) br++;
				}
				continue;
			}

			if(br > 1)
			{
				grid->aktline++;
				break;
			}

			if(br > 0)
			{
				start = node->posx;
				if(start > grid->gridcol)
				{
					grid->aktline--;
					node = node->prev;
					continue;
				}
				break;
			}

			if(node->type == GRIDBR || node->type == TEXTBOXGRIDBR) br++;

			grid->aktline--;
			node = node->prev;
		}

		grid->aktpage = -1;
		if(grid->aktline < 1 || grid->aktline > grid->linecount)
			grid->aktline = 1;

		if(grid->select != NULL) delscreenrc(screen, grid->select);
		drawscreen(screen, 0);
	}

	debug(1000, "out");
}

void gridleft(struct skin* screen, struct skin* grid)
{
	debug(1000, "in");
	struct skin* node = NULL;

	node = grid->select;
	if(grid != NULL)
	{
		if(node != NULL && (node->type == GRIDBR || node->type == TEXTBOXGRIDBR))
		{
			grid->aktpage = -1;
			while(node != NULL)
			{
				if(node->parentpointer == grid || ostrcmp(node->parent, grid->name) == 0)
				{
					if(node->next == NULL || node->next->type == GRIDBR || node->next->type == TEXTBOXGRIDBR)
						break;
					else
						grid->aktline++;
				}
				node = node->next;
			}
		}
		else
		{
			grid->aktpage = -1;
			grid->aktline--;
			if(grid->aktline < 1) grid->aktline = 1;
		}

		if(grid->select != NULL) delscreenrc(screen, grid->select);
		drawscreen(screen, 0);
		if(grid->select != NULL)
			grid->gridcol = grid->select->posx + (grid->select->width / 2);
	}
	debug(1000, "out");
}

void gridright(struct skin* screen, struct skin* grid)
{
	debug(1000, "in");
	struct skin* node = NULL;

	node = grid->select;
	if(grid != NULL)
	{
		if((node != NULL && node->next != NULL && (node->next->type == GRIDBR || node->next->type == TEXTBOXGRIDBR)) || grid->aktline == grid->linecount)
		{
			grid->aktpage = -1;
			while(node != NULL)
			{
				if(node->parentpointer == grid || ostrcmp(node->parent, grid->name) == 0)
				{
					if(node == NULL || node->type == GRIDBR || node->type == TEXTBOXGRIDBR)
						break;
					else
						grid->aktline--;
				}
				node = node->prev;
			}
		}
		else
		{
			grid->aktpage = -1;
			if(grid->aktline <= grid->linecount)
				grid->aktline++;
			if(grid->aktline > grid->linecount)
				grid->aktline = grid->linecount;
		}

		if(grid->select != NULL) delscreenrc(screen, grid->select);
		drawscreen(screen, 0);
		if(grid->select != NULL)
			grid->gridcol = grid->select->posx + (grid->select->width / 2);
	}
	debug(1000, "out");
}

void gridchup(struct skin* screen, struct skin* grid)
{
	debug(1000, "in");

	if(grid != NULL)
	{
		grid->aktline = -1;
		grid->aktpage--;
		if(grid->aktpage < 1)
			grid->aktpage = 1;

		drawscreen(screen, 0);
		if(grid->select != NULL)
			grid->gridcol = grid->select->posx + (grid->select->width / 2);
	}

	debug(1000, "out");
}

void gridchdown(struct skin* screen, struct skin* grid)
{
	debug(1000, "in");

	if(grid != NULL)
	{
		grid->aktline = -1;
		grid->aktpage++;
		if(grid->aktpage > grid->pagecount)
		{
			grid->aktpage = grid->pagecount;
			grid->aktline = -2;
		}

		drawscreen(screen, 0);
		if(grid->select != NULL)
			grid->gridcol = grid->select->posx + (grid->select->width / 2);
	}

	debug(1000, "out");
}


void listboxleft(struct skin* screen, struct skin* listbox)
{
	debug(1000, "in");

	if(listbox != NULL)
	{
		listbox->aktline = -1;
		listbox->aktpage--;
		if(listbox->aktpage < 1)
			listbox->aktpage = 1;

		if(listbox->select != NULL) delscreenrc(screen, listbox->select);
		drawscreen(screen, 0);
	}

	debug(1000, "out");
}

void listboxright(struct skin* screen, struct skin* listbox)
{
	debug(1000, "in");

	if(listbox != NULL)
	{
		listbox->aktline = -1;
		listbox->aktpage++;
		if(listbox->aktpage > listbox->pagecount)
		{
			listbox->aktpage = listbox->pagecount;
			listbox->aktline = -2;
		}

		if(listbox->select != NULL) delscreenrc(screen, listbox->select);
		drawscreen(screen, 0);
	}

	debug(1000, "out");
}

void listboxup(struct skin* screen, struct skin* listbox)
{
	debug(1000, "in");

	if(listbox != NULL)
	{
		listbox->aktpage = -1;
		listbox->aktline--;
		if(listbox->aktline < 1)
			listbox->aktline = listbox->linecount;

		if(listbox->select != NULL) delscreenrc(screen, listbox->select);
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void listboxdown(struct skin* screen, struct skin* listbox)
{
	debug(1000, "in");

	if(listbox != NULL)
	{
		listbox->aktpage = -1;
		if(listbox->aktline <= listbox->linecount)
			listbox->aktline++;
		if(listbox->aktline > listbox->linecount)
			listbox->aktline = 1;

		if(listbox->select != NULL) delscreenrc(screen, listbox->select);
		drawscreen(screen, 0);
	}
	debug(1000, "out");
}

void textboxup(struct skin* screen, struct skin* textbox)
{
	debug(1000, "in");
	if(textbox != NULL)
	{
		if(textbox->aktpage > 1)
		{
			textbox->aktpage--;
			drawscreen(screen, 0);
		}
	}
	debug(1000, "out");
}

void textboxdown(struct skin* screen, struct skin* textbox)
{
	debug(1000, "in");
	if(textbox != NULL)
	{
		if(textbox->aktpage < textbox->pagecount)
		{
			textbox->aktpage++;
			drawscreen(screen, 0);
		}
	}
	debug(1000, "out");
}

int addscreenrc(struct skin* screen, struct skin* node)
{
	debug(1000, "in");

	if(node == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(node->type == GRID)
	{
		addrc(getrcconfigint("rcright", NULL), gridright, screen, node);
		addrc(getrcconfigint("rcleft", NULL), gridleft, screen, node);
		addrc(getrcconfigint("rcup", NULL), gridup, screen, node);
		addrc(getrcconfigint("rcdown", NULL), griddown, screen, node);
		addrc(getrcconfigint("rcchup", NULL), gridchup, screen, node);
		addrc(getrcconfigint("rcchdown", NULL), gridchdown, screen, node);
	}
	else if(node->type == LISTBOX)
	{
		addrc(getrcconfigint("rcup", NULL), listboxup, screen, node);
		addrc(getrcconfigint("rcdown", NULL), listboxdown, screen, node);
		addrc(getrcconfigint("rcright", NULL), listboxright, screen, node);
		addrc(getrcconfigint("rcleft", NULL), listboxleft, screen, node);
	}
	else if(node->type == CHOICEBOX)
	{
		addrc(getrcconfigint("rcleft", NULL), choiceboxleft, screen, node);
		addrc(getrcconfigint("rcright", NULL), choiceboxright, screen, node);
	}
	else if(node->type == INPUTBOX || node->type == INPUTBOXNUM)
	{
		addrc(getrcconfigint("rc0", NULL), inputbox0, screen, node);
		addrc(getrcconfigint("rc1", NULL), inputbox1, screen, node);
		addrc(getrcconfigint("rc2", NULL), inputbox2, screen, node);
		addrc(getrcconfigint("rc3", NULL), inputbox3, screen, node);
		addrc(getrcconfigint("rc4", NULL), inputbox4, screen, node);
		addrc(getrcconfigint("rc5", NULL), inputbox5, screen, node);
		addrc(getrcconfigint("rc6", NULL), inputbox6, screen, node);
		addrc(getrcconfigint("rc7", NULL), inputbox7, screen, node);
		addrc(getrcconfigint("rc8", NULL), inputbox8, screen, node);
		addrc(getrcconfigint("rc9", NULL), inputbox9, screen, node);
		addrc(getrcconfigint("rcright", NULL), inputboxright, screen, node);
		addrc(getrcconfigint("rcleft", NULL), inputboxleft, screen, node);
		if(node->type == INPUTBOX) addrc(getrcconfigint("rcff", NULL), inputboxff, screen, node);
		if(node->type == INPUTBOX) addrc(getrcconfigint("rcfr", NULL), inputboxfr, screen, node);
		if(node->type == INPUTBOX) addrc(getrcconfigint("rchelp", NULL), inputboxhelp, screen, node);
	}
	else if(node->type == TEXTBOX)
	{
		addrc(getrcconfigint("rcup", NULL), textboxup, screen, node);
		addrc(getrcconfigint("rcdown", NULL), textboxdown, screen, node);
	}
	else if(node->type == FILELIST)
	{
		addrc(getrcconfigint("rcup", NULL), listboxup, screen, node);
		addrc(getrcconfigint("rcdown", NULL), listboxdown, screen, node);
		addrc(getrcconfigint("rcright", NULL), listboxright, screen, node);
		addrc(getrcconfigint("rcleft", NULL), listboxleft, screen, node);
		addrc(getrcconfigint("rcok", NULL), filelistok, screen, node);
	}
	else if(node->type == PROGRESSBAR)
	{
		addrc(getrcconfigint("rcleft", NULL), progressbarleft, screen, node);
		addrc(getrcconfigint("rcright", NULL), progressbarright, screen, node);
	}

	debug(1000, "out");
	return 0;
}

int delscreenrc(struct skin* screen, struct skin* node)
{
	debug(1000, "in");

	if(node == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(node->type == LISTBOX)
	{
		delrc(getrcconfigint("rcup", NULL), screen, node);
		delrc(getrcconfigint("rcdown", NULL), screen, node);
		delrc(getrcconfigint("rcright", NULL), screen, node);
		delrc(getrcconfigint("rcleft", NULL), screen, node);
	}
	else if(node->type == CHOICEBOX)
	{
		delrc(getrcconfigint("rcleft", NULL), screen, node);
		delrc(getrcconfigint("rcright", NULL), screen, node);
	}
	else if(node->type == INPUTBOX || node->type == INPUTBOXNUM)
	{
		delrc(getrcconfigint("rc0", NULL), screen, node);
		delrc(getrcconfigint("rc1", NULL), screen, node);
		delrc(getrcconfigint("rc2", NULL), screen, node);
		delrc(getrcconfigint("rc3", NULL), screen, node);
		delrc(getrcconfigint("rc4", NULL), screen, node);
		delrc(getrcconfigint("rc5", NULL), screen, node);
		delrc(getrcconfigint("rc6", NULL), screen, node);
		delrc(getrcconfigint("rc7", NULL), screen, node);
		delrc(getrcconfigint("rc8", NULL), screen, node);
		delrc(getrcconfigint("rc9", NULL), screen, node);
		delrc(getrcconfigint("rcright", NULL), screen, node);
		delrc(getrcconfigint("rcleft", NULL),  screen, node);
		if(node->type == INPUTBOX) delrc(getrcconfigint("rcff", NULL),  screen, node);
		if(node->type == INPUTBOX) delrc(getrcconfigint("rcfr", NULL),  screen, node);
	}
	else if(node->type == TEXTBOX)
	{
		delrc(getrcconfigint("rcup", NULL), screen, node);
		delrc(getrcconfigint("rcdown", NULL), screen, node);
	}
	else if(node->type == FILELIST)
	{
		delrc(getrcconfigint("rcup", NULL), screen, node);
		delrc(getrcconfigint("rcdown", NULL), screen, node);
		delrc(getrcconfigint("rcright", NULL), screen, node);
		delrc(getrcconfigint("rcleft", NULL), screen, node);
		delrc(getrcconfigint("rcok", NULL), screen, node);
	}
	else if(node->type == PROGRESSBAR)
	{
		delrc(getrcconfigint("rcleft", NULL), screen, node);
		delrc(getrcconfigint("rcright", NULL), screen, node);
	}

	debug(1000, "out");
	return 0;
}

#endif
