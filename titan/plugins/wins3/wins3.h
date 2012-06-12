/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef WINS3_H
#define WINS3_H

void wins3new(struct skin* wins3)
{
	struct skin* node = wins3;

	while(node != NULL)
	{
		if(ostrcmp(node->text, "O") == 0 || ostrcmp(node->text, "X") == 0) 
			changetext(node, NULL);

		node = node->next;
	}
}

void wins3changetitle(struct skin* wins3, int player)
{
	char* title = "3 WINS - Player ";
	char* tmpstr = NULL, *tmpnr = NULL;

	tmpnr = oitoa(player);
	tmpstr = ostrcat(_(title), tmpnr, 0, 0);

	changetitle(wins3, tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(tmpnr); tmpnr = NULL;
}

void screenwins3()
{
	int rcret = 0, player = 1;
	struct skin* wins3 = getscreen("wins3");
	struct skin* grid = getscreennode(wins3, "grid");

	wins3new(wins3);
	wins3changetitle(wins3, player);
	
	drawscreen(wins3, 0, 0);
	addscreenrc(wins3, grid);

	while(1)
	{
		rcret = waitrc(wins3, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(grid->select != NULL && grid->select->text == NULL)
			{
				if(player == 1)
				{
					changetext(grid->select, "X");
					player = 2;
				}
				else
				{
					changetext(grid->select, "O");
					player = 1;
				}
				wins3changetitle(wins3, player);
				drawscreen(wins3, 0, 0);
			}
			continue;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			wins3new(grid);
			player = 1;
			wins3changetitle(wins3, player);
			drawscreen(wins3, 0, 0);
		}
	}

	delownerrc(wins3);
	clearscreen(wins3);
}

#endif
