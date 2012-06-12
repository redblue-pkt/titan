#ifndef INPUTHELP_H
#define INPUTHELP_H

void inputhelpnum(struct skin* inputhelp, struct skin* inputbox, char num, int flag)
{
	inputboxff(inputhelp, inputbox, flag);
	inputboxchar(inputhelp, inputbox, num, flag);
	drawscreen(inputhelp, 0, flag);
}

char* screeninputhelp(char* text, int flag)
{
	int rcret = 0;
	struct skin* inputhelp = getscreen("inputhelp");
	struct skin* grid = NULL;
	struct skin* grid1 = getscreennode(inputhelp, "grid1");
	struct skin* grid2 = getscreennode(inputhelp, "grid2");
	struct skin* inputbox = getscreennode(inputhelp, "inputbox");
	char* tmpstr = NULL;

	grid1->hidden = NO;
	grid2->hidden = YES;
	grid = grid1;
	if(text != NULL && strlen(text) > 0)
	{
		inputbox->aktpage = strlen(text);
	}
	changeinput(inputbox, text);
	drawscreen(inputhelp, 0, flag);
	addscreenrc(inputhelp, grid);

	while(1)
	{
		rcret = waitrc(inputhelp, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			tmpstr = ostrcat(tmpstr, text, 1, 0);
			break;
		}
		if(rcret == getrcconfigint("rcff", NULL))
			inputboxright(inputhelp, inputbox, flag);
		if(rcret == getrcconfigint("rcfr", NULL))
			inputboxleft(inputhelp, inputbox, flag);
		if(rcret == getrcconfigint("rc0", NULL))
			inputhelpnum(inputhelp, inputbox, '0', flag);
		if(rcret == getrcconfigint("rc1", NULL))
			inputhelpnum(inputhelp, inputbox, '1', flag);
		if(rcret == getrcconfigint("rc2", NULL))
			inputhelpnum(inputhelp, inputbox, '2', flag);
		if(rcret == getrcconfigint("rc3", NULL))
			inputhelpnum(inputhelp, inputbox, '3', flag);
		if(rcret == getrcconfigint("rc4", NULL))
			inputhelpnum(inputhelp, inputbox, '4', flag);
		if(rcret == getrcconfigint("rc5", NULL))
			inputhelpnum(inputhelp, inputbox, '5', flag);
		if(rcret == getrcconfigint("rc6", NULL))
			inputhelpnum(inputhelp, inputbox, '6', flag);
		if(rcret == getrcconfigint("rc7", NULL))
			inputhelpnum(inputhelp, inputbox, '7', flag);
		if(rcret == getrcconfigint("rc8", NULL))
			inputhelpnum(inputhelp, inputbox, '8', flag);
		if(rcret == getrcconfigint("rc9", NULL))
			inputhelpnum(inputhelp, inputbox, '9', flag);

		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcred", NULL) || rcret == getrcconfigint("rcgreen", NULL) || rcret == getrcconfigint("rcyellow", NULL) || rcret == getrcconfigint("rcblue", NULL))
		{
			if(grid->select != NULL && grid->select->name != NULL)
			{
				if(rcret == getrcconfigint("rcgreen", NULL) || (rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "ok") == 0))
				{
					tmpstr = ostrcat(tmpstr, inputbox->input, 1, 0);
					break;
				}
				if(rcret == getrcconfigint("rcred", NULL) || (rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "bs") == 0))
				{
					inputboxfr(inputhelp, inputbox, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "clear") == 0)
				{
					changeinput(inputbox, NULL);
					drawscreen(inputhelp, 0, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "left") == 0)
				{
					inputboxleft(inputhelp, inputbox, flag);
					drawscreen(inputhelp, 0, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "right") == 0)
				{
					inputboxright(inputhelp, inputbox, flag);
					drawscreen(inputhelp, 0, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcblue", NULL) || (rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "switch") == 0))
				{
					if(grid1->hidden == YES)
					{
						delownerrc(inputhelp);
						grid = grid1;
						grid1->hidden = NO;
						grid2->hidden = YES;
						addscreenrc(inputhelp, grid);
					}
					else
					{
						delownerrc(inputhelp);
						grid = grid2;
						grid1->hidden = YES;
						grid2->hidden = NO;
						addscreenrc(inputhelp, grid);
					}
					drawscreen(inputhelp, 0, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcyellow", NULL))
				{
					inputboxff(inputhelp, inputbox, flag);
					inputboxchar(inputhelp, inputbox, ' ', flag);
					drawscreen(inputhelp, 0, flag);
					continue;
				}

				inputboxff(inputhelp, inputbox, flag);
				inputboxchar(inputhelp, inputbox, grid->select->name[0], flag);
			}
		}
	}

	delownerrc(inputhelp);
	clearscreen(inputhelp);
	drawscreen(skin, 0, flag);
	return tmpstr;
}

#endif
