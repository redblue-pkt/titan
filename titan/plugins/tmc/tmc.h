#ifndef TMC_H
#define TMC_H

int menu0pos = 0;
char* menu0[] = {"skin/tmcsettings.png", "skin/tmcpicture.png", "skin/tmcvideo.png", "skin/tmcaudio.png", "skin/tmcbtn.png"};
char* menu0txt[] = {"Settings", "Picture", "Video", "Audio", ""};

int menu1pos = 0;
char* menu1[] = {"skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png"};
char* menu1txt[] = {"S1", "S2", "S3", "S4", "S5"};

int menu2pos = 0;
char* menu2[] = {"skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png"};
char* menu2txt[] = {"P1", "P2", "P3", "P4", "P5"};

int menu3pos = 0;
char* menu3[] = {"skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png"};
char* menu3txt[] = {"V1", "V2", "V3", "V4", "V5"};

int menu4pos = 0;
char* menu4[] = {"skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png"};
char* menu4txt[] = {"A1", "A2", "A3", "A4", "A5"};

//flag 0: draw only
//flag 1: left
//flag 2: right
void tmcpicscroll(struct skin* tmcpic1, struct skin* tmcpic2, struct skin* tmcpic3, struct skin* tmcpic4, struct skin* tmcpic5, int flag)
{
/*
	int id = 0, max = 0;
	char** menu = NULL, **menutxt = NULL;
	int *menupos;

	if(menuid == 0)
	{
		menu = menu0;
		menutxt = menu0txt;
		max = (sizeof(menu0) / sizeof(char*)) - 1;
		menupos = &menu0pos;
	}
	else if(menuid == 1)
	{
		menu = menu1;
		menutxt = menu1txt;
		max = (sizeof(menu1) / sizeof(char*)) - 1;
		menupos = &menu1pos;
	}
	else if(menuid == 2)
	{
		menu = menu2;
		menutxt = menu2txt;
		max = (sizeof(menu2) / sizeof(char*)) - 1;
		menupos = &menu2pos;
	}
	else if(menuid == 3)
	{
		menu = menu3;
		menutxt = menu3txt;
		max = (sizeof(menu3) / sizeof(char*)) - 1;
		menupos = &menu3pos;
	}
	else if(menuid == 4)
	{
		menu = menu4;
		menutxt = menu4txt;
		max = (sizeof(menu4) / sizeof(char*)) - 1;
		menupos = &menu4pos;
	}

	if(flag == 1) (*menupos)++;
	if(flag == 2) (*menupos)--;

	if(*menupos < 0) *menupos = max;
	if(*menupos > max) *menupos = 0;

	id = *menupos;

	if(id > max) id = 0;
	changepic(tmcpic1, menu[id]);
	id++;

	if(id > max) id = 0;
	changepic(tmcpic2, menu[id]);
	id++;

	if(id > max) id = 0;
	changepic(tmcpic3, menu[id]);
	id++;

	if(id > max) id = 0;
	changepic(tmcpic4, menu[id]);
	id++;

	if(id > max) id = 0;
	changepic(tmcpic5, menu[id]);

	drawscreen(tmcpic1, 1);
	drawscreen(tmcpic2, 1);
	drawscreen(tmcpic3, 1);
	drawscreen(tmcpic4, 1);
	drawscreen(tmcpic5, 0);
*/
}

//flag 0: draw only
//flag 1: left
//flag 2: right
void tmcmenuscroll(int menuid, int active, struct skin* tmcmenutxt, struct skin* tmcmenu1, struct skin* tmcmenu2, struct skin* tmcmenu3, struct skin* tmcmenu4, struct skin* tmcmenu5, int flag)
{
	int id = 0, max = 0;
	char** menu = NULL, **menutxt = NULL;
	int *menupos = 0;
  char* tmpstr = NULL;

	if(menuid == 0)
	{
		menu = menu0;
		menutxt = menu0txt;
		max = (sizeof(menu0) / sizeof(char*)) - 1;
		menupos = &menu0pos;
	}
	else if(menuid == 1)
	{
		menu = menu1;
		menutxt = menu1txt;
		max = (sizeof(menu1) / sizeof(char*)) - 1;
		menupos = &menu1pos;
	}
	else if(menuid == 2)
	{
		menu = menu2;
		menutxt = menu2txt;
		max = (sizeof(menu2) / sizeof(char*)) - 1;
		menupos = &menu2pos;
	}
	else if(menuid == 3)
	{
		menu = menu3;
		menutxt = menu3txt;
		max = (sizeof(menu3) / sizeof(char*)) - 1;
		menupos = &menu3pos;
	}
	else if(menuid == 4)
	{
		menu = menu4;
		menutxt = menu4txt;
		max = (sizeof(menu4) / sizeof(char*)) - 1;
		menupos = &menu4pos;
	}

	if(flag == 1) (*menupos)++;
	if(flag == 2) (*menupos)--;

	if(*menupos < 0) *menupos = max;
	if(*menupos > max) *menupos = 0;

	id = *menupos;

	if(id > max) id = 0;
  tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu1, menu[id]);
  free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
  tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu2, menu[id]);
  free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
  tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu3, menu[id]);
  free(tmpstr); tmpstr = NULL;
	changetext(tmcmenutxt, _(menutxt[id]));
	changename(tmcmenutxt, menutxt[id]);
	id++;

	if(id > max) id = 0;
  tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu4, menu[id]);
  free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
  tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu5, menu[id]);
  free(tmpstr); tmpstr = NULL;

	if(active == 1)
	{
		changetext(tmcmenu1, NULL);
		changetext(tmcmenu2, NULL);
		changetext(tmcmenu3, NULL);
		changetext(tmcmenu4, NULL);
		changetext(tmcmenu5, NULL);
	}
	else
	{
		changetext(tmcmenu1, "X");
		changetext(tmcmenu2, "X");
		changetext(tmcmenu3, "X");
		changetext(tmcmenu4, "X");
		changetext(tmcmenu5, "X");
	}

	drawscreen(tmcmenutxt, 1);
	drawscreen(tmcmenu1, 1);
	drawscreen(tmcmenu2, 1);
	drawscreen(tmcmenu3, 1);
	drawscreen(tmcmenu4, 1);
	drawscreen(tmcmenu5, 0);
}

void screentmcmenu()
{
	int rcret = 0, menuid = 0, active = 1;
	struct skin* tmcbg = getscreen("tmcbg");
	struct skin* tmcmenutxt = getscreen("tmcmenutxt");
	struct skin* tmcmenu1 = getscreen("tmcmenu1");
	struct skin* tmcmenu2 = getscreen("tmcmenu2");
	struct skin* tmcmenu3 = getscreen("tmcmenu3");
	struct skin* tmcmenu4 = getscreen("tmcmenu4");
	struct skin* tmcmenu5 = getscreen("tmcmenu5");
	struct skin* tmcpic1 = getscreen("tmcpic1");
	struct skin* tmcpic2 = getscreen("tmcpic2");
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* tmcpic4 = getscreen("tmcpic4");
	struct skin* tmcpic5 = getscreen("tmcpic5");

	drawscreen(tmcbg, 1);
	drawscreen(tmcpic1, 1);
	drawscreen(tmcpic2, 1);
	drawscreen(tmcpic3, 1);
	drawscreen(tmcpic4, 1);
	drawscreen(tmcpic5, 1);
	tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			if(active == 1)
				active = 0;
			else
				active = 1;

			tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(active == 1)
			{
				if(menuid == 0) break;
				else menuid = 0;

				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				continue;
			}
		}

		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 1);
			else
				tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 2);
			else
				tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(active == 1)
			{
				if(ostrcmp("Settings", tmcmenutxt->name) == 0) menuid = 1;
				else if(ostrcmp("Picture", tmcmenutxt->name) == 0) menuid = 2;
				else if(ostrcmp("Video", tmcmenutxt->name) == 0) menuid = 3;
				else if(ostrcmp("Audio", tmcmenutxt->name) == 0) menuid = 4;
	
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				continue;
			}
		}
	}

	clearscreen(tmcbg);
}

#endif
