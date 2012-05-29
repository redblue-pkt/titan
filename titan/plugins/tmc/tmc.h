#ifndef TMC_H
#define TMC_H

struct mediadbfilter* mediadbfilterpos = NULL;

//Mainmenu
int menu0pos = 0;
char* menu0[] = {"skin/tmcsettings.png", "skin/tmcpicture.png", "skin/tmcvideo.png", "skin/tmcaudio.png", "skin/tmcexit.png"};
char* menu0txt[] = {"Settings", "Picture", "Video", "Audio", "Exit"};

//Settings
int menu1pos = 0;
char* menu1[] = {"skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png"};
char* menu1txt[] = {"", "", "", "", ""};

//Picture
int menu2pos = 0;
char* menu2[] = {"skin/tmcsearch.png", "skin/tmccategory.png", "skin/tmcall.png", "skin/tmcrandom.png", "skin/tmcone.png", "skin/tmcback.png"};
char* menu2txt[] = {"Search", "Category", "All", "Random", "Single", "Back"};

//Video
int menu3pos = 0;
char* menu3[] = {"skin/tmcsearch.png", "skin/tmccategory.png", "skin/tmcall.png", "skin/tmcback.png", "skin/tmcbtn.png"};
char* menu3txt[] = {"Search", "Category", "All", "Back", ""};

//Audio
int menu4pos = 0;
char* menu4[] = {"skin/tmcsearch.png", "skin/tmccategory.png", "skin/tmcall.png", "skin/tmcrandom.png", "skin/tmcone.png", "skin/tmcback.png"};
char* menu4txt[] = {"Search", "Category", "All", "Random", "Single", "Back"};

//Input
int menu5pos = 0;
char* menu5[] = {"skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png", "skin/tmcbtn.png"};
char* menu5txt[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"};

//flag 0: draw only
//flag 1: left
//flag 2: right
void tmcpicscroll(struct skin* tmcpic1, struct skin* tmcpic2, struct skin* tmcpic3, struct skin* tmcpic4, struct skin* tmcpic5, int flag)
{
	int count = 0;
	struct mediadbfilter* node = mediadbfilterpos;
	char* tmpstr = NULL;

	while(1)
	{
		if(flag == 0 && count == 0)
		{
			if(node == NULL) node = mediadbfilter;
			mediadbfilterpos = node;
		}
		if(flag == 1 && count == 0)
		{
			if(node == NULL) node = mediadbfilter;
			if(node != NULL) node = node->next;
			if(node == NULL) node = mediadbfilter;
			mediadbfilterpos = node;
		}
		if(flag == 2 && count == 0)
		{
			if(node == NULL) node = getlastmediadbfilter(mediadbfilter);
			if(node != NULL) node = node->prev;
			if(node == NULL) node = getlastmediadbfilter(mediadbfilter);
			mediadbfilterpos = node;
		}
		if(count != 0)
		{
			if(node == NULL) node = mediadbfilter;
			if(node != NULL) node = node->next;
			if(node == NULL) node = mediadbfilter;
		}

		if(node != NULL && node->node != NULL)
		{
			tmpstr = ostrcat(getconfig("imdb_directory", NULL), "/", 0, 0);
			tmpstr = ostrcat(tmpstr, node->node->poster, 1, 0);
			if(count == 0) changepic(tmcpic1, tmpstr);
			if(count == 1) changepic(tmcpic2, tmpstr);
			if(count == 2) changepic(tmcpic3, tmpstr);
			if(count == 3) changepic(tmcpic4, tmpstr);
			if(count == 4) changepic(tmcpic5, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		count++; if(count > 4) break;
	}

	drawscreen(tmcpic1, 1);
	drawscreen(tmcpic2, 1);
	drawscreen(tmcpic3, 1);
	drawscreen(tmcpic4, 1);
	drawscreen(tmcpic5, 0);
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
  else if(menuid == 5)
	{
		menu = menu5;
		menutxt = menu5txt;
		max = (sizeof(menu5) / sizeof(char*)) - 1;
		menupos = &menu5pos;
	}

	if(flag == 1) (*menupos)++;
	if(flag == 2) (*menupos)--;

	if(*menupos < 0) *menupos = max;
	if(*menupos > max) *menupos = 0;

	id = *menupos;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu1, tmpstr);
	free(tmpstr); tmpstr = NULL;
  if(menuid == 5)
    changetext(tmcmenu1, menutxt[id]);
  else
    changetext(tmcmenu1, NULL);
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu2, tmpstr);
	free(tmpstr); tmpstr = NULL;
  if(menuid == 5)
    changetext(tmcmenu2, menutxt[id]);
  else
    changetext(tmcmenu2, NULL);
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu3, tmpstr);
	free(tmpstr); tmpstr = NULL;
  if(menuid == 5)
    changetext(tmcmenu3, menutxt[id]);
  else
  {
    changetext(tmcmenu3, NULL);
    changetext(tmcmenutxt, _(menutxt[id]));
    changename(tmcmenutxt, menutxt[id]);
  }
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu4, tmpstr);
	free(tmpstr); tmpstr = NULL;
  if(menuid == 5)
    changetext(tmcmenu4, menutxt[id]);
  else
    changetext(tmcmenu4, NULL);
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu5, tmpstr);
	free(tmpstr); tmpstr = NULL;
  if(menuid == 5)
    changetext(tmcmenu5, menutxt[id]);
  else
    changetext(tmcmenu5, NULL);

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
	int rcret = 0, menuid = 0, oldmenuid = 0, active = 1;
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

  mediadbfilterpos = NULL;
	menu0pos = 0;
	menu1pos = 0;
	menu2pos = 0;
	menu3pos = 0;
	menu4pos = 0;

	drawscreen(tmcbg, 1);
	drawscreen(tmcpic1, 1);
	drawscreen(tmcpic2, 1);
	drawscreen(tmcpic3, 1);
	drawscreen(tmcpic4, 1);
	drawscreen(tmcpic5, 1);
	tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);

	readmediadb(getconfig("mediadbfile", NULL));

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(menuid != 0 && rcret == getrcconfigint("rcmenu", NULL))
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
				menuid = oldmenuid;
        oldmenuid = 0;

				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				continue;
			}
		}

		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 1);
			else
				tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 1);
			continue;
		}

		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 2);
			else
				tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 2);
			continue;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(active == 1)
			{
        if(ostrcmp("Exit", tmcmenutxt->name) == 0)
          break;
				if(ostrcmp("Settings", tmcmenutxt->name) == 0)
				{
					menuid = 1;
				}
        if(ostrcmp("Search", tmcmenutxt->name) == 0)
        {
          oldmenuid = menuid;
          menuid = 5;
        }
        if(ostrcmp("Back", tmcmenutxt->name) == 0)
        {
          menuid = oldmenuid;
          oldmenuid = 0;
        }
				else if(ostrcmp("Picture", tmcmenutxt->name) == 0)
				{
					menuid = 2;
					mediadbfilterpos = NULL;
					createmediadbfilter(2, 0);
					tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 0);
				}
				else if(ostrcmp("Video", tmcmenutxt->name) == 0)
				{
					menuid = 3;
					mediadbfilterpos = NULL;
					createmediadbfilter(0, 0);
					tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 0);
				}
				else if(ostrcmp("Audio", tmcmenutxt->name) == 0)
				{
					menuid = 4;
					mediadbfilterpos = NULL;
					createmediadbfilter(1, 0);
					tmcpicscroll(tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, 0);
				}
	
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				continue;
			}
		}
	}

	freemediadbfilter(0);
	freemediadb(0);
	clearscreen(tmcbg);
}

#endif
