#ifndef TMC_H
#define TMC_H

struct mediadbfilter* mediadbfilterpos = NULL;

//Mainmenu
int menu0pos = 0;
char* menu0[] = {"skin/tmcsettings.png", "skin/tmcpicture.png", "skin/tmcvideo.png", "skin/tmcaudio.png", "skin/tmcexit.png"};
char* menu0txt[] = {"Settings", "Picture", "Video", "Audio", "Exit"};

//Settings
int menu1pos = 0;
char* menu1[] = {"skin/tmcscan.png", "skin/tmcdb.png", "skin/tmcsettings.png", "skin/tmcback.png", "skin/tmcbtn.png"};
char* menu1txt[] = {"Scan", "Database", "Main", "Back", ""};

//Picture
int menu2pos = 0;
char* menu2[] = {"skin/tmccategory.png", "skin/tmcall.png", "skin/tmcrandom.png", "skin/tmcone.png", "skin/tmcback.png"};
char* menu2txt[] = {"Category", "All", "Random", "Single", "Back"};

//Video
int menu3pos = 0;
char* menu3[] = {"skin/tmcgenre.png", "skin/tmccategory.png", "skin/tmcall.png", "skin/tmcyear.png", "skin/tmcdirector.png", "skin/tmcactors.png", "skin/tmcrating.png", "skin/tmcback.png"};
char* menu3txt[] = {"Genre", "Category", "All", "Year", "Director", "Actors", "Rating", "Back"};

//Audio
int menu4pos = 0;
char* menu4[] = {"skin/tmccategory.png", "skin/tmcall.png", "skin/tmcrandom.png", "skin/tmcone.png", "skin/tmcback.png"};
char* menu4txt[] = {"Category", "All", "Random", "Single", "Back"};

//flag 0: don't draw
//flag 1: draw
void tmcpicdel(struct skin* tmcpictitle, struct skin* tmcpicstar, struct skin* tmcpic1, struct skin* tmcpic2, struct skin* tmcpic3, struct skin* tmcpic4, struct skin* tmcpic5, char* tmcpictitlebg, char* tmcpicstarbg, int flag)
{
	changepic(tmcpic1, NULL);
	changepic(tmcpic2, NULL);
	changepic(tmcpic3, NULL);
	changepic(tmcpic4, NULL);
	changepic(tmcpic5, NULL);
	changeret(tmcpic3, NULL);
	changetext(tmcpictitle, NULL);
	changepic(tmcpicstar, NULL);
	restorescreennofree(tmcpictitlebg, tmcpictitle);
	restorescreennofree(tmcpicstarbg, tmcpicstar);

	if(flag == 1)
	{
		drawscreen(tmcpic1, 1);
		drawscreen(tmcpic2, 1);
		drawscreen(tmcpic3, 1);
		drawscreen(tmcpic4, 1);
		drawscreen(tmcpic5, 0);
	}
}

//flag 0: draw only
//flag 1: left
//flag 2: right
void tmcpicscroll(struct skin* tmcpictitle, struct skin* tmcpicstar, struct skin* tmcpic1, struct skin* tmcpic2, struct skin* tmcpic3, struct skin* tmcpic4, struct skin* tmcpic5, char* tmcpictitlebg, char* tmcpicstarbg, int flag)
{
	int count = 0;
	struct mediadbfilter* node = mediadbfilterpos;
	char* tmpstr = NULL;

	tmcpicdel(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);

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
			if(node == NULL) node = getlastmediadbfilter(mediadbfilter, 0);
			if(node != NULL) node = node->prev;
			if(node == NULL) node = getlastmediadbfilter(mediadbfilter, 0);
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
			tmpstr = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
			tmpstr = ostrcat(tmpstr, node->node->poster, 1, 0);
			tmpstr = ostrcat(tmpstr, "_poster.jpg", 1, 0);
			
			if(!file_exist(tmpstr))
			{
				free(tmpstr);
				tmpstr = ostrcat("%pluginpath%/tmc/skin/tmcnopic.jpg", NULL, 0, 0);
			}
			
			if(count == 0) changepic(tmcpic1, tmpstr);
			if(count == 1) changepic(tmcpic2, tmpstr);
			if(count == 2)
			{
				changepic(tmcpic3, tmpstr);
				changeret(tmcpic3, node->node->file);
				changetext(tmcpictitle, node->node->title);
				
				//TODO RATING
				double rating = 7.5;
				rating = rating / 2;
				
				if(rating > 4.5)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar5.png");
				else if(rating > 4)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar4x.png");
				else if(rating > 3.5)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar4.png");
				else if(rating > 3)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar3x.png");
				else if(rating > 2.5)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar3.png");
				else if(rating > 2)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar2x.png");
				else if(rating > 1.5)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar2.png");
				else if(rating > 1)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar1x.png");
				else if(rating > 0.5)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar1.png");
				else if(rating > 0)
					changepic(tmcpicstar, "%pluginpath%/tmc/skin/tmcstar0x.png");
				else
					changepic(tmcpicstar, NULL);
			}
			if(count == 3) changepic(tmcpic4, tmpstr);
			if(count == 4) changepic(tmcpic5, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		count++; if(count > 4) break;
	}

	drawscreen(tmcpictitle, 1);
	drawscreen(tmcpicstar, 1);
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

	if(flag == 1) (*menupos)++;
	if(flag == 2) (*menupos)--;

	if(*menupos < 0) *menupos = max;
	if(*menupos > max) *menupos = 0;

	id = *menupos;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu1, tmpstr);
	free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu2, tmpstr);
	free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu3, tmpstr);
	free(tmpstr); tmpstr = NULL;
	changetext(tmcmenutxt, _(menutxt[id]));
	changeret(tmcmenutxt, menutxt[id]);
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu4, tmpstr);
	free(tmpstr); tmpstr = NULL;
	id++;

	if(id > max) id = 0;
	tmpstr = ostrcat("%pluginpath%/tmc/", menu[id], 0, 0);
	changepic(tmcmenu5, tmpstr);
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

char* screentmccategory(int type, char* category)
{
	int rcret = 0;
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* listbox = getscreennode(tmcpic3, "listbox");
	struct skin* tmp = NULL;
	struct mediadbcategory* node = NULL;
	char* tmpstr = NULL, *tmppic = NULL, *ret = NULL;

	tmppic = ostrcat(tmcpic3->pic, NULL, 0, 0);
	changepic(tmcpic3, NULL);

	freemediadbcategory(0);
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), category, 0, 0);
	readmediadb(tmpstr, type, 1);
	free(tmpstr); tmpstr = NULL;

	node = mediadbcategory;
	while(node != NULL)
	{
		tmp = addlistbox(tmcpic3, listbox, tmp, 1);
		if(tmp != NULL)
		{
			changetext(tmp, node->name);
		}
		node = node->next;
	}

	addscreenrc(tmcpic3, listbox);
	drawscreen(tmcpic3, 0);

	while(1)
	{
		rcret = waitrc(tmcpic3, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			ret = ostrcat(listbox->select->text, NULL, 0, 0);
			break;
		}
	}

	delownerrc(tmcpic3);
	delmarkedscreennodes(tmcpic3, 1);
	freemediadbcategory(0);

	changepic(tmcpic3, tmppic);
	free(tmppic); tmppic = NULL;
	drawscreen(tmcpic3, 0);

	return ret;
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
	struct skin* tmcpictitle = getscreen("tmcpictitle");
	struct skin* tmcpicstar = getscreen("tmcpicstar");
	struct skin* tmcpic1 = getscreen("tmcpic1");
	struct skin* tmcpic2 = getscreen("tmcpic2");
	struct skin* tmcpic3 = getscreen("tmcpic3");
	struct skin* tmcpic4 = getscreen("tmcpic4");
	struct skin* tmcpic5 = getscreen("tmcpic5");
	char* tmpstr = NULL;
	char* tmcpictitlebg = NULL, *tmcpicstarbg = NULL; 

	mediadbfilterpos = NULL;

	menu0pos = 0;
	menu1pos = 0;
	menu2pos = 0;
	menu3pos = 0;
	menu4pos = 0;

	tmcpicdel(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);

	drawscreen(tmcbg, 1);
	drawscreen(tmcpic1, 1);
	drawscreen(tmcpic2, 1);
	drawscreen(tmcpic3, 1);
	drawscreen(tmcpic4, 1);
	drawscreen(tmcpic5, 1);
	tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
	
	//save backgrounds
	status.screencalc = 2;
	drawscreen(tmcpictitle, 0);
	drawscreen(tmcpicstar, 0);
	tmcpictitlebg = savescreen(tmcpictitle);
	tmcpicstarbg = savescreen(tmcpicstar);
	status.screencalc = 0;

	readmediadb(getconfig("mediadbfile", NULL), 0, 0);

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(menuid != 0 && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
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
				menuid = 0;
				tmcpicdel(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 1);

				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				continue;
			}
		}

		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 1);
			else
				tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 1);
			continue;
		}

		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(active == 1)
				tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 2);
			else
				tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 2);
			continue;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(active == 1)
			{
				if(ostrcmp("Exit", tmcmenutxt->ret) == 0)
					break;
				else if(ostrcmp("Back", tmcmenutxt->ret) == 0)
				{
					menuid = 0;
					tmcpicdel(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 1);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
				else if(ostrcmp("Settings", tmcmenutxt->ret) == 0) //mainmenu settings
				{
					menuid = 1;
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 1 && ostrcmp("Scan", tmcmenutxt->ret) == 0)
				{
					mediadbscan();
				}
				else if(menuid == 1 && ostrcmp("Database", tmcmenutxt->ret) == 0)
				{
					//TODO
				}
				else if(menuid == 1 && ostrcmp("Main", tmcmenutxt->ret) == 0)
				{
					//TODO
				}
				else if(ostrcmp("Picture", tmcmenutxt->ret) == 0) //mainmenu picture
				{
					menuid = 2;
					mediadbfilterpos = NULL;
					createmediadbfilter(2, NULL, 0);
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 2 && ostrcmp("Category", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(2, ".category");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(2, tmpstr, 4);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 2 && ostrcmp("All", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					createmediadbfilter(2, NULL, 0);
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
				}
				else if(menuid == 2 && ostrcmp("Random", tmcmenutxt->ret) == 0)
				{
					//TODO
				}
				else if(menuid == 2 && ostrcmp("Single", tmcmenutxt->ret) == 0)
				{
					//TODO
				}
				else if(ostrcmp("Video", tmcmenutxt->ret) == 0) //mainmenu video
				{
					menuid = 3;
					mediadbfilterpos = NULL;
					createmediadbfilter(0, NULL, 0);
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 3 && ostrcmp("Genre", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".genre");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(0, tmpstr, 6);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Category", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".category");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(0, tmpstr, 4);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("All", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					createmediadbfilter(0, NULL, 0);
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
				}
				else if(menuid == 3 && ostrcmp("Year", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".year");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(0, tmpstr, 1);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Director", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".director");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(0, tmpstr, 2);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Actors", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".actors");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(0, tmpstr, 3);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 3 && ostrcmp("Rating", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(0, ".rating");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(0, tmpstr, 5);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(ostrcmp("Audio", tmcmenutxt->ret) == 0) //mainmenu audio
				{
					menuid = 4;
					mediadbfilterpos = NULL;
					createmediadbfilter(1, NULL, 0);
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 4 && ostrcmp("Category", tmcmenutxt->ret) == 0)
				{
					tmpstr = screentmccategory(1, ".category");
					if(tmpstr != NULL)
					{
						mediadbfilterpos = NULL;
						createmediadbfilter(1, tmpstr, 4);
						tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else if(menuid == 4 && ostrcmp("All", tmcmenutxt->ret) == 0)
				{
					mediadbfilterpos = NULL;
					createmediadbfilter(1, NULL, 0);
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
				}
				else if(menuid == 4 && ostrcmp("Random", tmcmenutxt->ret) == 0)
				{
					//TODO
				}
				else if(menuid == 4 && ostrcmp("Single", tmcmenutxt->ret) == 0)
				{
					//TODO
				}

				continue;
			}
			
			if(active == 0 && tmcpic3->ret != NULL)
			{
				if(menuid == 3 || menuid == 4)
				{
					drawscreen(skin, 0);
					//TODO: play
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
				else if(menuid == 2)
				{
					drawscreen(skin, 0);
					//TODO: show pic
					tmcpicscroll(tmcpictitle, tmcpicstar, tmcpic1, tmcpic2, tmcpic3, tmcpic4, tmcpic5, tmcpictitlebg, tmcpicstarbg, 0);
					tmcmenuscroll(menuid, active, tmcmenutxt, tmcmenu1, tmcmenu2, tmcmenu3, tmcmenu4, tmcmenu5, 0);
				}
			}
		}
	}

	free(tmcpictitlebg); tmcpictitlebg = NULL;
	free(tmcpicstarbg); tmcpicstarbg = NULL;
	freemediadbfilter(0);
	freemediadb(0);
	clearscreen(tmcbg);
}

#endif
