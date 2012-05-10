#ifndef TMC_H
#define TMC_H

//flag 0: left
//flag 1: right
void tmcmenuscroll(int flag, struct skin* tmcmenu1, struct skin* tmcmenu2, struct skin* tmcmenu3, struct skin* tmcmenu4, struct skin* tmcmenu5)
{
	char* tmpstr = NULL;

	if(flag == 0)
	{
		tmpstr = ostrcat(tmcmenu1->pic, NULL, 0, 0);
		changepic(tmcmenu1, tmcmenu2->pic);
		changepic(tmcmenu2, tmcmenu3->pic);
		changepic(tmcmenu3, tmcmenu4->pic);
		changepic(tmcmenu4, tmcmenu5->pic);
		changepic(tmcmenu5, tmpstr);
		free(tmpstr); tmpstr = NULL;
		drawscreen(tmcmenu1, 1);
		drawscreen(tmcmenu2, 1);
		drawscreen(tmcmenu3, 1);
		drawscreen(tmcmenu4, 1);
		drawscreen(tmcmenu5, 0);
	}
	else
	{
		tmpstr = ostrcat(tmcmenu5->pic, NULL, 0, 0);
		changepic(tmcmenu5, tmcmenu4->pic);
		changepic(tmcmenu4, tmcmenu3->pic);
		changepic(tmcmenu3, tmcmenu2->pic);
		changepic(tmcmenu2, tmcmenu1->pic);
		changepic(tmcmenu1, tmpstr);
		free(tmpstr); tmpstr = NULL;
		drawscreen(tmcmenu1, 1);
		drawscreen(tmcmenu2, 1);
		drawscreen(tmcmenu3, 1);
		drawscreen(tmcmenu4, 1);
		drawscreen(tmcmenu5, 0);
	}
}

void screentmcmenu()
{
	int rcret = 0;
	struct skin* tmcbg = getscreen("tmcbg");
	struct skin* tmcmenu1 = getscreen("tmcmenu1");
	struct skin* tmcmenu2 = getscreen("tmcmenu2");
	struct skin* tmcmenu3 = getscreen("tmcmenu3");
	struct skin* tmcmenu4 = getscreen("tmcmenu4");
	struct skin* tmcmenu5 = getscreen("tmcmenu5");

	drawscreen(tmcbg, 1);
	drawscreen(tmcmenu1, 1);
	drawscreen(tmcmenu2, 1);
	drawscreen(tmcmenu3, 1);
	drawscreen(tmcmenu4, 1);
	drawscreen(tmcmenu5, 0);

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcleft", NULL))
			tmcmenuscroll(0, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5);
		if(rcret == getrcconfigint("rcright", NULL))
			tmcmenuscroll(1, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5);
	}

	clearscreen(tmcbg);
}

#endif
