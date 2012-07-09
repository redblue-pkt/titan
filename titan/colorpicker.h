#ifndef COLORPICKER_H
#define COLORPICKER_H

char* screencolorpicer(char* color, int screencalc, int filelistview, int flag)
{
	int rcret = 0;
	long col = 0;
	struct skin* colorpicker = getscreen("colorpicker");
	struct skin* cp = getscreennode(colorpicker, "cp");
	struct skin* grid = getscreennode(colorpicker, "grid");
	char* tmpstr = NULL;

	if(color != NULL) col = convertcol(color);
	cp->bgcol = col;

	drawscreen(colorpicker, 0, flag);
	addscreenrc(colorpicker, grid);

	while(1)
	{
		rcret = waitrc(colorpicker, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			tmpstr = ostrcat(tmpstr, color, 1, 0);
			break;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(grid->select != NULL && grid->select->name != NULL)
			{
				tmpstr = ostrcat(tmpstr, grid->select->name, 1, 0);
				break;
			}
		}
	}

	delownerrc(colorpicker);
	clearscreen(colorpicker);
	drawscreen(skin, 0, flag);
	return tmpstr;
}

#endif
