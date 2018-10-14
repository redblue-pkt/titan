#ifndef INPUTHELP_H
#define INPUTHELP_H

void inputhelpnum(struct skin* inputhelp, struct skin* inputbox, char num, int screencalc, int filelistview, int flag)
{
	inputboxff(inputhelp, inputbox, screencalc, filelistview, flag);
	inputboxchar(inputhelp, inputbox, num, screencalc, filelistview, flag);
	drawscreen(inputhelp, screencalc, flag);
}

void translateinputhelp(struct skin* inputhelp, int flag)
{
	struct skin* sup2grid1 = getscreennode(inputhelp, "sup2grid1");
	struct skin* sup2grid2 = getscreennode(inputhelp, "sup2grid2");

	struct skin* sup3grid1 = getscreennode(inputhelp, "sup3grid1");
	struct skin* sup3grid2 = getscreennode(inputhelp, "sup3grid2");

	struct skin* eurogrid1 = getscreennode(inputhelp, "eurogrid1");
	struct skin* eurogrid2 = getscreennode(inputhelp, "eurogrid2");

	struct skin* microgrid1 = getscreennode(inputhelp, "microgrid1");
	struct skin* microgrid2 = getscreennode(inputhelp, "microgrid2");

	struct skin* deggrid1 = getscreennode(inputhelp, "deggrid1");
	struct skin* deggrid2 = getscreennode(inputhelp, "deggrid2");

	struct skin* acutegrid1 = getscreennode(inputhelp, "acutegrid1");
	struct skin* acutegrid2 = getscreennode(inputhelp, "acutegrid2");

	struct skin* quotegrid1 = getscreennode(inputhelp, "quotegrid1");
	struct skin* quotegrid2 = getscreennode(inputhelp, "quotegrid2");

	struct skin* sectiongrid1 = getscreennode(inputhelp, "sectiongrid1");
	struct skin* sectiongrid2 = getscreennode(inputhelp, "sectiongrid2");

	struct skin* backslashgrid1 = getscreennode(inputhelp, "backslashgrid1");
	struct skin* backslashgrid2 = getscreennode(inputhelp, "backslashgrid2");

	struct skin* szliggrid1 = getscreennode(inputhelp, "szliggrid1");
	struct skin* szliggrid2 = getscreennode(inputhelp, "szliggrid2");

	changetext(sup2grid1, "²");
	changetext(sup2grid2, "²");

	changetext(sup3grid1, "³");
	changetext(sup3grid2, "³");

	changetext(eurogrid1, "€");
	changetext(eurogrid2, "€");

	changetext(microgrid1, "µ");
	changetext(microgrid2, "µ");

	changetext(deggrid1, "°");
	changetext(deggrid2, "°");

	changetext(acutegrid1, "´");
	changetext(acutegrid2, "´");

	changetext(quotegrid1, "\"");
	changetext(quotegrid2, "\"");

	changetext(sectiongrid1, "§");
	changetext(sectiongrid2, "§");

	changetext(backslashgrid1, "\\");
	changetext(backslashgrid2, "\\");

	changetext(szliggrid1, "ß");
	changetext(szliggrid2, "ß");

	drawscreen(inputhelp, 0, flag);
}

char* screeninputhelp(char* text, int screencalc, int filelistview, int flag)
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

	translateinputhelp(inputhelp, flag);

	while(1)
	{
		rcret = waitrc(inputhelp, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			tmpstr = ostrcat(tmpstr, text, 1, 0);
			break;
		}
		if(rcret == getrcconfigint("rcff", NULL))
			inputboxright(inputhelp, inputbox, screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rcfr", NULL))
			inputboxleft(inputhelp, inputbox, screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc0", NULL))
			inputhelpnum(inputhelp, inputbox, '0', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc1", NULL))
			inputhelpnum(inputhelp, inputbox, '1', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc2", NULL))
			inputhelpnum(inputhelp, inputbox, '2', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc3", NULL))
			inputhelpnum(inputhelp, inputbox, '3', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc4", NULL))
			inputhelpnum(inputhelp, inputbox, '4', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc5", NULL))
			inputhelpnum(inputhelp, inputbox, '5', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc6", NULL))
			inputhelpnum(inputhelp, inputbox, '6', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc7", NULL))
			inputhelpnum(inputhelp, inputbox, '7', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc8", NULL))
			inputhelpnum(inputhelp, inputbox, '8', screencalc, filelistview, flag);
		if(rcret == getrcconfigint("rc9", NULL))
			inputhelpnum(inputhelp, inputbox, '9', screencalc, filelistview, flag);

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
					inputboxfr(inputhelp, inputbox, screencalc, filelistview, flag);
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
					inputboxleft(inputhelp, inputbox, screencalc, filelistview, flag);
					drawscreen(inputhelp, 0, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcok", NULL) && ostrcmp(grid->select->name, "right") == 0)
				{
					inputboxright(inputhelp, inputbox, screencalc, filelistview, flag);
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
					translateinputhelp(inputhelp, flag);
					continue;
				}
				if(rcret == getrcconfigint("rcyellow", NULL))
				{
					inputboxff(inputhelp, inputbox, screencalc, filelistview, flag);
					inputboxchar(inputhelp, inputbox, ' ', screencalc, filelistview, flag);
					drawscreen(inputhelp, 0, flag);
					continue;
				}

//      not working with spezial chars 
//                              inputboxff(inputhelp, inputbox, screencalc, filelistview, flag); 
//                              inputboxchar(inputhelp, inputbox, grid->select->name[0], screencalc, filelistview, flag); 
				inputboxchar2(inputhelp, inputbox, grid->select->text, screencalc, filelistview, flag); 
			}
		}
	}

	delownerrc(inputhelp);
	clearscreen(inputhelp);
	if(filelistview != 1000)
		drawscreen(skin, 0, flag);
	return tmpstr;
}

#endif
