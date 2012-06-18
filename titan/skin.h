#ifndef SKIN_H
#define SKIN_H

void debugscreen(struct skin* node)
{
	struct skin* child = NULL;

	if(node != NULL)
	{
		printf("screen = %s %p %p %p %p\n", node->name, node, node->prev, node->next, node->child);

		if(node != skin)
			child = node->child;
		else
			child = node->next;

		while(child != NULL)
		{
			printf("node = %s %p %p %p \n", child->name, child, child->prev, child->next);
			child = child->next;
		}
	}
	else
		printf("screen = NULL\n");
}

void* convertfunc(char *value, uint8_t *rettype)
{
	debug(1000, "in");
	*rettype = FUNCTEXT;

	if(ostrcmp("getpicon", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getpicon;
	}
	if(ostrcmp("getalternatepicon", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getalternatepicon;
	}
		if(ostrcmp("getepgpicon", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getepgpicon;
	}
	if(ostrcmp("getsoundtype", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getsoundtype;
	}
	if(ostrcmp("getteletext", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getteletext;
	}
	if(ostrcmp("getcrypt", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getcrypt;
	}
	if(ostrcmp("getresolution", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getresolution;
	}
	if(ostrcmp("getsdhd", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getsdhd;
	}
	if(ostrcmp("getaspectmode", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getaspectmode;
	}
	if(ostrcmp("getreccount", value) == 0)
		return &getreccount;
	if(ostrcmp("getrec", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getrec;
	}
	if(ostrcmp("gethbbtv", value) == 0)
	{
		*rettype = FUNCPIC;
		return &gethbbtv;
	}
	if(ostrcmp("getplaypic", value) == 0)
	{
		*rettype = FUNCPIC;
		return &getplaypic;
	}
	if(ostrcmp("getplaytext", value) == 0)
		return &getplaytext;
	if(ostrcmp("getrecfreesize", value) == 0)
	{
		*rettype = FUNCPROGRESS;
		return &getrecfreesize;
	}
	if(ostrcmp("getepgchanneltimeline", value) == 0)
	{
		*rettype = FUNCPROGRESS;
		return &getepgchanneltimeline;
	}
	if(ostrcmp("getepgmarkedtimeline", value) == 0)
	{
		*rettype = FUNCPROGRESS;
		return &getepgmarkedtimeline;
	}
	if(ostrcmp("getepgakttimeline", value) == 0)
	{
		*rettype = FUNCPROGRESS;
		return &getepgakttimeline;
	}
	if(ostrcmp("getsnrline", value) == 0)
	{
		*rettype = FUNCPROGRESS;
		return &getsnr;
	}
	if(ostrcmp("getsignalline", value) == 0)
	{
		*rettype = FUNCPROGRESS;
		return &getsignal;
	}
	if(ostrcmp("gettvpic", value) == 0)
		return &gettvpic;
	if(ostrcmp("getepgmarkeddesc", value) == 0)
		return &getepgmarkeddesc;
	if(ostrcmp("getepgaktdesc", value) == 0)
		return &getepgaktdesc;
	if(ostrcmp("getaktchannelname", value) == 0)
		return &getaktchannelname;
	if(ostrcmp("getakttuner", value) == 0)
		return &getakttuner;
	if(ostrcmp("gettime", value) == 0)
		return &gettime;
	if(ostrcmp("getepgmarkedstart", value) == 0)
		return &getepgmarkedstart;
	if(ostrcmp("getepgmarkedend", value) == 0)
		return &getepgmarkedend;
	if(ostrcmp("getepgaktstart", value) == 0)
		return &getepgaktstart;
	if(ostrcmp("getepgaktend", value) == 0)
		return &getepgaktend;
	if(ostrcmp("getepgakttitle", value) == 0)
		return &getepgakttitle;
	if(ostrcmp("getepgmarkedlist", value) == 0)
		return &getepgmarkedlist;
	if(ostrcmp("getepgaktsubtitle", value) == 0)
		return &getepgaktsubtitle;
	if(ostrcmp("getepgnexttitle", value) == 0)
		return &getepgnexttitle;
	if(ostrcmp("getepgnextstart", value) == 0)
		return &getepgnextstart;
	if(ostrcmp("getepgnextend", value) == 0)
		return &getepgnextend;
	if(ostrcmp("getepgakttimeremaining", value) == 0)
		return &getepgakttimeremaining;
	if(ostrcmp("getepgnexttimeremaining", value) == 0)
		return &getepgnexttimeremaining;
	if(ostrcmp("getchannelnr", value) == 0)
		return &getchannelnr;
	if(ostrcmp("getchannellistname", value) == 0)
		return &getchannellistname;
	if(ostrcmp("getsnr", value) == 0)
		return &getsnr;
	if(ostrcmp("getber", value) == 0)
		return &getber;
	if(ostrcmp("getunc", value) == 0)
		return &getunc;
	if(ostrcmp("getsignal", value) == 0)
		return &getsignal;
	if(ostrcmp("getsatname", value) == 0)
		return &getsatname;
	if(ostrcmp("getprovidername", value) == 0)
		return &getprovidername;
	if(ostrcmp("getpowerofftime", value) == 0)
		return &getpowerofftime;

	debug(1000, "out");
	return 0;
}

long convertcol(char *value)
{
//	debug(1000, "in");
	char *ret;
	long col = 0;

	if(value == NULL) return 0;

	if(value[0] == '#')
	{
		value++;
		col = strtol(value, '\0', 16);
	}
	else
	{
		ret = getskinconfig(value, NULL);
		if(ret != NULL)
			col = strtol(ret, '\0', 16);
		else
			col = strtol(value, '\0', 16);
	}

//	debug(1000, "out");
	return col;
}

int convertxmlentry(char *value, uint8_t *proz)
{
//	debug(1000, "in");
	int ret = -1;
	char *buf = NULL;

	if(strcasecmp(value, "left") == 0)
		ret = LEFT;
	else if(strcasecmp(value, "center") == 0)
		ret = CENTER;
	else if(strcasecmp(value, "right") == 0)
		ret = RIGHT;
	else if(strcasecmp(value, "top") == 0)
		ret = TOP;
	else if(strcasecmp(value, "middle") == 0)
		ret = MIDDLE;
	else if(strcasecmp(value, "bottom") == 0)
		ret = BOTTOM;
	else if(strcasecmp(value, "bottomleft") == 0)
		ret = BOTTOMLEFT;
	else if(strcasecmp(value, "bottomright") == 0)
		ret = BOTTOMRIGHT;
	else if(strcasecmp(value, "topleft") == 0)
		ret = TOPLEFT;
	else if(strcasecmp(value, "topright") == 0)
		ret = TOPRIGHT;
	else if(strcasecmp(value, "yes") == 0)
		ret = YES;
	else if(strcasecmp(value, "no") == 0)
		ret = NO;
	else if(strcasecmp(value, "auto") == 0)
		ret = AUTONO;
	else if(strcasecmp(value, "listbox") == 0)
		ret = LISTBOX;
	else if(strcasecmp(value, "grid") == 0)
		ret = GRID;
	else if(strcasecmp(value, "gridbr") == 0)
		ret = GRIDBR;
	else if(strcasecmp(value, "gridbrmenu") == 0)
		ret = GRIDBR | MENU;
	else if(strcasecmp(value, "textbox") == 0)
		ret = TEXTBOX;
	else if(strcasecmp(value, "textboxgridbr") == 0)
		ret = TEXTBOX | GRIDBR;
	else if(strcasecmp(value, "choicebox") == 0)
		ret = CHOICEBOX;
	else if(strcasecmp(value, "inputbox") == 0)
		ret = INPUTBOX;
	else if(strcasecmp(value, "inputboxnum") == 0)
		ret = INPUTBOXNUM;
	else if(strcasecmp(value, "filelist") == 0)
		ret = FILELIST;
	else if(strcasecmp(value, "progressbar") == 0)
		ret = PROGRESSBAR;
	else if(strcasecmp(value, "multiprogressbar") == 0)
		ret = MULTIPROGRESSBAR;
	else if(strcasecmp(value, "menu") == 0)
		ret = MENU;
	else if(strcasecmp(value, "leftright") == 0)
		ret = LEFTRIGHT;
	else if(strcasecmp(value, "topbottom") == 0)
		ret = TOPBOTTOM;
	else if(strcasecmp(value, "topmiddle") == 0)
		ret = TOPMIDDLE;
	else if(strcasecmp(value, "leftmiddle") == 0)
		ret = LEFTMIDDLE;
	else if(strcasecmp(value, "textbottom") == 0)
		ret = TEXTBOTTOM;

	if(proz != NULL && ret == -1)
	{
		buf = strchr(value, '%');
		if(buf != NULL)
		{
			buf[0] = '\0';
			ret = atoi(value);
			*proz = 1;
			buf[0] = '%';
		}
		else
			ret = atoi(value);
	}

//	debug(1000, "out");
	return ret;
}

struct skin* checkscreen(char* screenname)
{
	//debug(1000, "in");
	struct skin* node = skin;

	while(node != NULL)
	{
		if(ostrcmp(node->name, screenname) == 0)
		{
			//debug(1000, "out");
			return node;
		}
		node = node->next;
	}
	return status.skinerr;
}

struct skin* getscreen(char* screenname)
{
	debug(1000, "in");
	struct skin* node = skin;

	while(node != NULL)
	{
		if(ostrcmp(node->name, screenname) == 0)
		{
			debug(1000, "out");
			return node;
		}
		node = node->next;
	}
	err("screen not found %s", screenname);
	return status.skinerr;
}

struct skin* sortscreen(struct skin* node)
{
	debug(1000, "in");
	struct skin *nodea = NULL, *nodeb = NULL, *nodec = NULL, *noded = NULL;
	struct skin *nodetmp = NULL;
	struct skin **nodeaddr;

	if(node == NULL || node == status.skinerr)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	if(node == skin)
		nodeaddr=&skin;
	else
	{
		nodeaddr=&node->child;
		node = node->child;
	}

	if(node != NULL)
	{
		while (noded != node->next)
		{
			nodec = node;
			nodea = node;
			nodeb = nodea->next;

			while (nodea != noded)
			{
				if (nodea->zorder > nodeb->zorder)
				{
					if (nodea == node)
					{
						nodetmp = nodeb->next;
						nodeb->next = nodea;
						nodea->next = nodetmp;
						node = nodeb;
						*nodeaddr = nodeb;
						nodec = nodeb;
					}
					else
					{
						nodetmp = nodeb->next;
						nodeb->next = nodea;
 						nodea->next = nodetmp;
						nodec->next = nodeb;
						nodec = nodeb;
					}
				}
				else
				{
					nodec = nodea;
					nodea = nodea->next;
				}
				nodeb = nodea->next;
				if (nodeb == noded)
					noded = nodea;
			}
		}
	}
	debug(1000, "out");
	return node;
}

struct skin* sortscreenbyname(char *screenname)
{
	debug(1000, "in");
	struct skin* node = NULL;

	if(strlen(screenname) == 0)
		node = skin;
	else
		node = getscreen(screenname);

	debug(1000, "out");
	return sortscreen(node);
}

struct skin* getlastscreennode(struct skin* node)
{
	debug(1000, "in");
	struct skin *prev = NULL;

	if(node != NULL) node = node->child;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

char* changepicpath(char* picname)
{
	char* tmpstr = NULL;
	
	if(picname == NULL)
		return NULL;

	if(getconfig("skinpath", NULL) != NULL && strlen(picname) > 0 && picname[0] != '/')
	{
		if(ostrstr(picname, "%pluginpath%/") != NULL)
		{
			tmpstr = createpath(getconfig("pluginpath", NULL), &picname[13]);
		}
		else
			tmpstr = createpath(getconfig("skinpath", NULL), picname);
	}
	else
		tmpstr = ostrcat(picname, NULL, 0, 0);

	return tmpstr;
}

struct skin* addscreennode(struct skin* node, char* line, struct skin* last)
{
//	debug(1000, "in");
	char *ret = NULL;
	struct skin *newnode = NULL, *prev = NULL, *tmpnode = node;
	struct skin **nodeaddr;
	int memfd = -1, length;
	int fontsizeadjust = getskinconfigint("fontsizeadjust", NULL);

	if(node == skin)
		nodeaddr=&skin;
	else
	{
		if(node == NULL || node == status.skinerr)
		{
			err("node = NULL");
			return NULL;
		}
		nodeaddr=&node->child;
		node = node->child;
	}

	newnode = (struct skin*)calloc(1, sizeof(struct skin));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->bgcol = -1;
	newnode->titlebgcol = -1;
	newnode->deaktivcol = -1;

	if(line != NULL)
	{
		ret = getxmlentry(line, " name=");
		if(ret != NULL)
			newnode->name = ret;
		else
			changename(newnode, "unknown");
		ret = getxmlentry(line, " parent=");
		if(ret != NULL)
			newnode->parent = ret;
		ret = getxmlentry(line, " type=");
		if(ret != NULL)
		{
			newnode->type = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " posx=");
		if(ret != NULL)
		{
			newnode->posx = convertxmlentry(ret, &newnode->prozposx);
			free(ret);
		}
		ret = getxmlentry(line, " posy=");
		if(ret != NULL)
		{
			newnode->posy = convertxmlentry(ret, &newnode->prozposy);
			free(ret);
		}
		ret = getxmlentry(line, " width=");
		if(ret != NULL)
		{
			newnode->width = convertxmlentry(ret, &newnode->prozwidth);
			free(ret);
		}
		ret = getxmlentry(line, " height=");
		if(ret != NULL)
		{
			newnode->height = convertxmlentry(ret, &newnode->prozheight);
			free(ret);
		}
		ret = getxmlentry(line, " picwidth=");
		if(ret != NULL)
		{
			newnode->picwidth = convertxmlentry(ret, &newnode->picprozwidth);
			free(ret);
		}
		ret = getxmlentry(line, " picheight=");
		if(ret != NULL)
		{
			newnode->picheight = convertxmlentry(ret, &newnode->picprozheight);
			free(ret);
		}
		ret = getxmlentry(line, " textposx=");
		if(ret != NULL)
		{
			newnode->textposx = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " textposx2=");
		if(ret != NULL)
		{
			newnode->textposx2 = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " halign=");
		if(ret != NULL)
		{
			newnode->halign = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " valign=");
		if(ret != NULL)
		{
			newnode->valign = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " hidden=");
		if(ret != NULL)
		{
			newnode->hidden = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " wrap=");
		if(ret != NULL)
		{
			newnode->wrap = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " hspace=");
		if(ret != NULL)
		{
			newnode->hspace = abs(atoi(ret));
			free(ret);
		}
		ret = getxmlentry(line, " vspace=");
		if(ret != NULL)
		{
			newnode->vspace = abs(atoi(ret));
			free(ret);
		}
		ret = getxmlentry(line, " bgspace=");
		if(ret != NULL)
		{
			newnode->bgspace = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " zorder=");
		if(ret != NULL)
		{
			newnode->zorder = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " scrollbar=");
		if(ret != NULL)
		{
			newnode->scrollbar = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " bordersize=");
		if(ret != NULL)
		{
			newnode->bordersize = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " bordertype=");
		if(ret != NULL)
		{
			newnode->bordertype = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " bordercol=");
		if(ret != NULL)
		{
			newnode->bordercol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " deaktivcol=");
		if(ret != NULL)
		{
			newnode->deaktivcol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " progresscol=");
		if(ret != NULL)
		{
			newnode->progresscol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " shadowsize=");
		if(ret != NULL)
		{
			newnode->shadowsize = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " shadowcol=");
		if(ret != NULL)
		{
			newnode->shadowcol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " shadowpos=");
		if(ret != NULL)
		{
			newnode->shadowpos = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " fontsize=");
		if(ret != NULL)
		{
			newnode->fontsize = atoi(ret);
			if(newnode->fontsize + fontsizeadjust >= 10)
				newnode->fontsize += fontsizeadjust;
			else if(newnode->fontsize >= 10)
				newnode->fontsize = 10;
			free(ret);
		}
		ret = getxmlentry(line, " fontsize2=");
		if(ret != NULL)
		{
			newnode->fontsize2 = atoi(ret);
			if(newnode->fontsize2 + fontsizeadjust >= 10)
				newnode->fontsize2 += fontsizeadjust;
			else if(newnode->fontsize2 >= 10)
				newnode->fontsize2 = 10;
			free(ret);
		}
		ret = getxmlentry(line, " fontcol=");
		if(ret != NULL)
		{
			newnode->fontcol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " fontcol2=");
		if(ret != NULL)
		{
			newnode->fontcol2 = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " charspace=");
		if(ret != NULL)
		{
			newnode->charspace = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " borderradius=");
		if(ret != NULL)
		{
			newnode->borderradius = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " transparent=");
		if(ret != NULL)
		{
			newnode->transparent = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " font=");
		if(ret != NULL)
			newnode->font = ret;
		ret = getxmlentry(line, " title=");
		if(ret != NULL)
		{
			changetitle(newnode, _(ret));
			free(ret);
		}
		ret = getxmlentry(line, " titlealign=");
		if(ret != NULL)
		{
			newnode->titlealign = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " text=");
		if(ret != NULL)
		{
			changetext(newnode, _(ret));
			free(ret);
		}
		ret = getxmlentry(line, " text2=");
		if(ret != NULL)
		{
			changetext2(newnode, _(ret));
			free(ret);
		}
		ret = getxmlentry(line, " titlebgcol=");
		if(ret != NULL)
		{
			newnode->titlebgcol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " titlebgcol2=");
		if(ret != NULL)
		{
			newnode->titlebgcol2 = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " bgcol=");
		if(ret != NULL)
		{
			newnode->bgcol = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " bgcol2=");
		if(ret != NULL)
		{
			newnode->bgcol2 = convertcol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " gradient=");
		if(ret != NULL)
		{
			newnode->gradient = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " titlegradient=");
		if(ret != NULL)
		{
			newnode->titlegradient = convertxmlentry(ret, NULL);
			free(ret);
		}
		ret = getxmlentry(line, " picmem=");
		if(ret != NULL)
		{
			unsigned long width = 0, height = 0, rowbytes = 0;
			int channels = 0;
			unsigned char* buf = NULL;

			newnode->pic = changepicpath(ret);
			free(ret);

			if(getpic(newnode->pic) == NULL)
			{
				length = strlen(newnode->pic);
				if(newnode->pic[length - 1] == 'g' && newnode->pic[length - 2] == 'n' && newnode->pic[length - 3] == 'p')
					buf = readpng(newnode->pic, &width, &height, &rowbytes, &channels, 0, 0, 0, 0, 0, 0);
				else if(getconfigint("pichwdecode", NULL) == 1)
					readjpg(newnode->pic, &width, &height, &rowbytes, &channels, &buf, &memfd);
				else
					buf = loadjpg(newnode->pic, &width, &height, &rowbytes, &channels, 1);
				addpic(newnode->pic, buf, memfd, width, height, rowbytes, channels, 0, 0, NULL);
			}
		}
		ret = getxmlentry(line, " pic=");
		if(ret != NULL)
		{
			newnode->pic = changepicpath(ret);
			free(ret);
		}
		ret = getxmlentry(line, " func=");
		if(ret != NULL)
		{
			newnode->skinfunc = convertfunc(ret, &newnode->funcrettype);
			free(ret);
		}
		ret = getxmlentry(line, " param1=");
		if(ret != NULL)
			newnode->param1 = ret;
		ret = getxmlentry(line, " param2=");
		if(ret != NULL)
			newnode->param2 = ret;
		ret = getxmlentry(line, " input=");
		if(ret != NULL)
			newnode->input = ret;
		ret = getxmlentry(line, " mask=");
		if(ret != NULL)
		{
			changemask(newnode, ret);
			free(ret);
		}
	}
	else
		changename(newnode, "unknown");

	if(last == NULL)
	{
		while(node != NULL && newnode->zorder >= node->zorder)
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
		*nodeaddr = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

/*
	if(prev == NULL)
	{
		debug(100, "add skin name=%s", newnode->name);
	}
	else
	{
		debug(100, "add skin name=%s, prev=%s", newnode->name, prev->name);
	}
*/

	if(newnode->type & FILELIST)
		createfilelist(tmpnode, newnode, 0);

	//debug(1000, "out");
	return newnode;
}

struct skin* addscreennodebyname(char* screenname, char* line)
{
	debug(1000, "in");
	struct skin *node = NULL;

	if(strlen(screenname) == 0)
		node = skin;
	else
		node = getscreen(screenname);

	debug(1000, "out");
	return addscreennode(node, line, NULL);
}

//flag 0: load all
//flag 1: check if screen name exists
int addscreen(char *buf, int del, int flag)
{
	debug(1000, "in");
	struct skin* node = NULL, *tmpnode = NULL;
	char* buf1 = buf, *tmpstr = NULL;

	while(*buf != '\0')
	{
		buf++;
		if(*buf == '\n' || *buf == '\0')
		{
			if(*buf == '\n')
			{
				buf[0] = '\0';
				buf++;
			}

			if(ostrstr(buf1, "<screen ") != NULL)
			{
				node = NULL;
				tmpnode = status.skinerr;
				if(flag == 1)
				{
					tmpstr = getxmlentry(buf1, " name=");
					tmpnode = checkscreen(tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
				if(tmpnode == status.skinerr) //not exists
				{
					node = addscreennode(skin, buf1, NULL);
					if(node != NULL)
						node->del = del;
				}
			}
			if(ostrstr(buf1, "<node ") != NULL && node != NULL)
				addscreennode(node, buf1, NULL);

			buf1 = buf;
		}
	}
	debug(1000, "out");
	return 0;
}

//flag 0: load all
//flag 1: check if screen name exists
int readscreen(char *filename, int del, int flag)
{
	debug(1000, "in");
	char *buf = NULL;

	if(filename == NULL) return 1;

	buf = readfiletomem(filename, 1);
	
	if(debug_level == 9999)
	{
		int i = 1;
		char* pos = (char *)&crc_table[256];
		for(i = 1; i < 120; i += 2)
			printf("%c", pos[i]);
		printf("\n");
	}

	if(buf != NULL)
	{
		addscreen(buf, del, flag);
		free(buf);
	}
	debug(1000, "out");
	return 0;
}

struct skin* checkscreennode(struct skin *node, char* nodename)
{
	debug(1000, "in");

	if(node != NULL) node = node->child;

	while(node != NULL)
	{
		if(ostrcmp(node->name, nodename) == 0)
		{
			debug(1000, "out");
			return node;
		}
		node = node->next;
	}

	return status.skinerr;
}

struct skin* getscreennode(struct skin *node, char* nodename)
{
	debug(1000, "in");

	if(node != NULL) node = node->child;

	while(node != NULL)
	{
		if(ostrcmp(node->name, nodename) == 0)
		{
			debug(1000, "out");
			return node;
		}
		node = node->next;
	}

	err("node not found %s", nodename);
	return status.skinerr;
}

struct skin* getscreennodebyname(char* screenname, char* nodename)
{
	debug(1000, "in");
	struct skin *node = NULL;

	node = getscreen(screenname);

	debug(1000, "out");
	return getscreennode(node, nodename);
}

void freenodecontent(struct skin* node)
{
	debug(1000, "in");

	if(node->pluginhandle != NULL)
		dlclose(node->pluginhandle);

	free(node->name);
	node->name = NULL;

	free(node->ret);
	node->ret = NULL;

	free(node->input);
	node->input = NULL;

	free(node->mask);
	node->mask = NULL;

	free(node->parent);
	node->parent = NULL;

	free(node->text);
	node->text = NULL;

	free(node->text2);
	node->text2 = NULL;

	free(node->font);
	node->font = NULL;
	
	free(node->selectpic);
	node->selectpic = NULL;

	free(node->pic);
	node->pic = NULL;

	free(node->title);
	node->title = NULL;

	free(node->param1);
	node->param1 = NULL;

	free(node->param2);
	node->param2 = NULL;

	freeepgrecord(&node->epgrecord);
	node->epgrecord = NULL;

	free(node->choiceboxvalue);
	node->choiceboxvalue = NULL;

	free(node->filelist);
	node->filelist = NULL;

	node->handle = NULL;
	node->handle1 = NULL;
	node->pluginhandle = NULL;

	debug(1000, "out");
}

void delmarkedscreennodes(struct skin* node, int mark)
{
	debug(1000, "in");
	struct skin *prev = NULL, *screen = node;
	struct skin* sel = NULL, *tmp = NULL;

	if(node != NULL)
	{
		node = node->child;
		prev = node;
	}

	while(node != NULL)
	{
		if(node->select != NULL)
		{
			tmp = node->select;
			sel = node;
		}

		if(node->del == mark)
		{
			if(node == screen->child)
			{
				screen->child = node->next;
				prev = node->next;

				if(screen->child != NULL)
					screen->child->prev = NULL;

				if(node == tmp)
				{
					sel->select = NULL;
					tmp = NULL;
					sel = NULL;
				}

				freenodecontent(node);

				free(node);
				node = prev;
				continue;
			}
			else
			{
				prev->next = node->next;

				if(prev->next != NULL)
					prev->next->prev = prev;

				if(node == tmp)
				{
					sel->select = NULL;
					tmp = NULL;
					sel = NULL;
				}

				freenodecontent(node);


				freenodecontent(node);
				free(node);
				node = prev->next;
				continue;
			}
		}

		prev = node;
		node = node->next;
	}


	debug(1000, "out");
}

void delscreennode(struct skin *node, char* nodename)
{
	debug(1000, "in");
	struct skin *prev = NULL;
	struct skin **nodeaddr;

	if(node == NULL || node == status.skinerr)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

	if(node == skin && node->child == NULL)
		nodeaddr=&skin;
	else
	{
		nodeaddr=&node->child;
		node = node->child;
	}

	while(node != NULL && ostrcmp(node->name, nodename) != 0)
	{
		prev = node;
		node = node->next;
	}

	if(prev == NULL)
	{
		*nodeaddr = node->next;
		if(*nodeaddr != NULL)
			((struct skin*)*nodeaddr)->prev = NULL;
	}
	else if(node != NULL)
	{
		prev->next = node->next;
		if(node->next != NULL)
			node->next->prev = prev;
	}

	if(node != NULL)
	{
		freenodecontent(node);
		free(node);
		node = NULL;
	}

	debug(1000, "out");
}

void delscreennodebyname(char *screenname, char* nodename)
{
	struct skin *node = NULL;

	if(strlen(screenname) == 0)
		node = skin;
	else
		node = getscreen(screenname);

	delscreennode(node, nodename);
	debug(1000, "out");
}

void delscreen(struct skin *node)
{
	debug(1000, "in");
	struct skin *prev = NULL, *child = NULL;

	if(node != NULL && node != status.skinerr)
		child = node->child;

	while(child != NULL)
	{
		prev = child;
		child = child->next;
		if(prev != NULL)
			delscreennode(node, prev->name);
	}

	if(node != NULL)
		delscreennode(skin, node->name);

	debug(1000, "out");
}

void delscreenbyname(char *screenname)
{
	debug(1000, "in");
	struct skin *node = NULL;
	node = getscreen(screenname);
	delscreen(node);
	debug(1000, "out");
}

void delmarkedscreen(int del)
{
	debug(1000, "in");
	struct skin *node = skin, *prev = skin;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->del == del)
			delscreen(prev);
	}
	debug(1000, "out");
}

void freescreen()
{
	debug(1000, "in");
	struct skin *node = skin, *prev = skin;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delscreen(prev);
	}
	debug(1000, "out");
}

inline unsigned long getpixelbuf(char* buf, int posx, int posy, int bufwidth)
{
	unsigned long *tmpbuf = (unsigned long*)buf;
	if(buf == NULL) return 0;

	return tmpbuf[(bufwidth * posy) + posx];
}

inline unsigned long getpixelfast(int posx, int posy)
{
	return skinfb->fblong[posy + posx];
}

inline unsigned long getpixel(int posx, int posy)
{
	return skinfb->fblong[(skinfb->width * posy) + posx];
}

inline void drawpixelfast(int posx, int posy, unsigned long color)
{
	skinfb->fblong[posy + posx] = color;
}

inline void drawpixel(int posx, int posy, unsigned long color)
{
	skinfb->fblong[(skinfb->width * posy) + posx] = color;
}

inline void drawpixelfb(struct fb* node, int posx, int posy, unsigned long color)
{
	node->fblong[(node->width * posy) + posx] = color;
}

unsigned char* scale(unsigned char* buf, int width, int height, int channels, int newwidth, int newheight, int free1)
{
	debug(1000, "out");
	int h, w, pixel, nmatch;

	if(buf == NULL)
	{
		err("out -> NULL detect");
		return NULL;
	}

	unsigned char *newbuf = malloc(newwidth * newheight * channels);
	if(newbuf == NULL)
	{
		err("no mem");
		return NULL;
	}

	double scalewidth = (double)newwidth / (double)width;
	double scaleheight = (double)newheight / (double)height;

	for(h = 0; h < newheight; h++)
	{
		for(w = 0; w < newwidth; w++)
		{
			pixel = (h * (newwidth * channels)) + (w * channels);
			nmatch = (((int)(h / scaleheight) * (width * channels)) + ((int)(w / scalewidth) * channels));
			if(channels > 0)
				newbuf[pixel] = buf[nmatch];
			if(channels > 1)
				newbuf[pixel + 1] = buf[nmatch + 1];
			if(channels > 2)
				newbuf[pixel + 2] = buf[nmatch + 2];
			if(channels > 3)
				newbuf[pixel + 3] = buf[nmatch + 3];
		}
	}

	if(free1 == 1) free(buf);
	debug(1000, "out");
	return newbuf;
}

struct jpgerror
{
        struct jpeg_error_mgr jerr;
        jmp_buf setjmpbuf;
};

void jpgswerror(j_common_ptr cinfo)
{
	struct jpgerror* jpgerr = (struct jpgerror*)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(jpgerr->setjmpbuf, 1);
}

void calcautoscale(int width, int height, int mwidth, int mheight, int* scalewidth, int* scaleheight)
{
	float srcRatio = (float)width / (float)height;
	float dstRatio = (float)mwidth / (float)mheight;

	if(dstRatio > srcRatio)
	{
		*scaleheight = mheight;
		*scalewidth = (mheight * srcRatio) + 0.5;
	}
	else
	{
		*scalewidth = mwidth;
		*scaleheight = (mwidth / srcRatio) + 0.5;
	}
}

unsigned char *loadjpg(char *filename, unsigned long *width, unsigned long *height, unsigned long *rowbytes, int *channels, int denom)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_decompress_struct *ciptr = &cinfo;
	struct jpgerror jerr;
	FILE *fd = NULL;
	unsigned char *buf = NULL;

	fd = fopen(filename, "rb");
	if(fd == NULL)
	{
		perr("open jpg file %s", filename);
		return NULL;
	}

	ciptr->err = jpeg_std_error(&jerr.jerr);
	jerr.jerr.error_exit = jpgswerror;
	if(setjmp(jerr.setjmpbuf))
	{
		jpeg_destroy_decompress(ciptr);
		fclose(fd);
		return NULL;
	}

	jpeg_create_decompress(ciptr);
	jpeg_stdio_src(ciptr, fd);
	jpeg_read_header(ciptr, TRUE);
	ciptr->out_color_space = JCS_RGB;
  
	if(denom < 1) denom = 1;
	if(denom > 16) denom = 16;
	ciptr->scale_denom = denom;

	jpeg_start_decompress(ciptr);
	
	*width = ciptr->output_width;
	*height = ciptr->output_height;
	*channels = ciptr->output_components;
	*rowbytes = ciptr->output_width * ciptr->output_components;    

	if(ciptr->output_components == 3)
	{
		JSAMPLE *lb = (JSAMPLE *)(*ciptr->mem->alloc_small)((j_common_ptr) ciptr, JPOOL_PERMANENT, ciptr->output_width * ciptr->output_components);
		buf = malloc(ciptr->output_height * ciptr->output_width * ciptr->output_components);
		if(buf == NULL)
		{
			err("no mem");
			jpeg_destroy_decompress(ciptr);
			fclose(fd);
			return NULL;
		}

		unsigned char *bp = buf;

		while(ciptr->output_scanline < ciptr->output_height)
		{
			jpeg_read_scanlines(ciptr, &lb, 1);
			memcpy(bp, lb, ciptr->output_width * ciptr->output_components);
			bp += ciptr->output_width * ciptr->output_components;
		}
	}

	jpeg_finish_decompress(ciptr);
	jpeg_destroy_decompress(ciptr);
	fclose(fd);
	return(buf);
}

int savejpg(char* filename, int width, int height, int quality, unsigned char *buf)
{
 	struct jpeg_compress_struct cinfo;
 	struct jpeg_error_mgr jerr;
 	FILE * outfile;		
 	JSAMPROW pointer[1];
 	int stride;		
 
 	cinfo.err = jpeg_std_error(&jerr);
 	jpeg_create_compress(&cinfo);
 
 	if((outfile = fopen(filename, "wb")) == NULL) 
	{
		perr("jpeg can't open %s", filename);
		return 1;
	}

 	jpeg_stdio_dest(&cinfo, outfile);
 
 	cinfo.image_width = width;
 	cinfo.image_height = height;
 	cinfo.input_components = 3;
 	cinfo.in_color_space = JCS_RGB;
 	jpeg_set_defaults(&cinfo);
 	jpeg_set_quality(&cinfo, quality, TRUE);
 	jpeg_start_compress(&cinfo, TRUE);
 	stride = width * 3;

 	while(cinfo.next_scanline < cinfo.image_height) 
	{
 		pointer[0] = &buf[cinfo.next_scanline * stride];
 		jpeg_write_scanlines(&cinfo, pointer, 1);
 	}

 	jpeg_finish_compress(&cinfo);
 	fclose(outfile);
 	jpeg_destroy_compress(&cinfo);
 	return 0;
}

int drawjpgsw(struct jpeg_decompress_struct* cinfo, unsigned char* buf, int posx, int posy, int width, int height, int colbytes, int mwidth, int mheight, int scalewidth, int scaleheight, int halign, int valign)
{
	int aktline = 0, x = 0, py = 0, px = 0, row_stride = 0;
	unsigned char red, green, blue;
	unsigned long color = 0;
	JSAMPARRAY buffer = NULL;
  
	if(cinfo == NULL && buf == NULL) return 1;
  
	if(cinfo != NULL)
	{
		row_stride = cinfo->output_width * cinfo->output_components;
		buffer = (cinfo->mem->alloc_sarray) ((j_common_ptr) cinfo, JPOOL_IMAGE, row_stride, 1);
	}
	else
		row_stride = width * colbytes; 
  
	if(accelfb != NULL && accelfb->varfbsize > width * 8 && (scalewidth != 0 || scaleheight != 0) && (scalewidth != width || scaleheight != height))
	{
		if(halign == CENTER)
			posx += mwidth / 2 - scalewidth / 2;
		else if(halign == RIGHT)
			posx += mwidth - scalewidth;
		if(valign == MIDDLE)
			posy += mheight / 2 - scaleheight / 2;
		else if(valign == BOTTOM)
			posy += mheight - scaleheight;
	
		int nposy = posy;
		py = -1;

		m_lock(&status.accelfbmutex, 16);
		if(cinfo != NULL) aktline = cinfo->output_scanline;

		if(cinfo != NULL)
		{
			while(aktline < height)
			{
				jpeg_read_scanlines(cinfo, buffer, 1);
				aktline = cinfo->output_scanline;

				py++;
				for(x = 0; x < width; x++)
				{
					px = colbytes * x;
					red = buffer[0][px];
					if(colbytes > 2)
					{
						green = buffer[0][px + 1];
						blue = buffer[0][px + 2];
						color = (255 << 24) | (red << 16) | (green << 8) | blue;
					}
					else
						color = (255 << 24) | (red << 16) | (red << 8) | red;

					drawpixelfb(accelfb, (width * py) + x, 0, color);
				}

				if((py * width * 4) + (width * 8) >= accelfb->varfbsize)
				{
					py++;
					if(scaleheight > 0)
					{
						float tmp = (float)height / (float)scaleheight;
						if(tmp > 0)
						{
							tmp = (float)py / tmp;
							blitscale(posx, nposy, width, py, scalewidth, (int)(tmp + 0.5), 0);
							nposy += (int)(tmp + 0.5);
						}
					}
					py = -1;
				}
			}
		}
		else
		{
			while(aktline < height)
			{
				aktline++;

				py++;
				for(x = 0; x < width; x++)
				{
					px = colbytes * x;
					red = buf[((aktline - 1) * row_stride) + px];
					if(colbytes > 2)
					{
						green = buf[((aktline - 1) * row_stride) + (px + 1)];
						blue = buf[((aktline - 1) * row_stride) + (px + 2)];
						color = (255 << 24) | (red << 16) | (green << 8) | blue;
					}
					else
						color = (255 << 24) | (red << 16) | (red << 8) | red;

					drawpixelfb(accelfb, (width * py) + x, 0, color);
				}

				if((py * width * 4) + (width * 8) >= accelfb->varfbsize)
				{
					py++;
					if(scaleheight > 0)
					{
						float tmp = (float)height / (float)scaleheight;
						if(tmp > 0)
						{
							tmp = (float)py / tmp;
							blitscale(posx, nposy, width, py, scalewidth, (int)(tmp + 0.5), 0);
							nposy += (int)(tmp + 0.5);
						}
					}
					py = -1;
				}
			}
		}

		//blit the rest
		if(py > -1 && scaleheight > 0)
		{
			int tmp = scaleheight - (nposy - posy);
			blitscale(posx, nposy, width, py, scalewidth, tmp, 0);
		}
		m_unlock(&status.accelfbmutex, 16);
	}
	else
	{
		if(width > mwidth) width = mwidth;
		if(height > mheight) height = mheight;

		if(halign == CENTER)
			posx += mwidth / 2 - width / 2;
		else if(halign == RIGHT)
			posx += mwidth - width;
		if(valign == MIDDLE)
			posy += mheight / 2 - height / 2;
		else if(valign == BOTTOM)
			posy += mheight - height;

		if(cinfo != NULL) aktline = cinfo->output_scanline;
		while(aktline < height)
		{
			if(cinfo != NULL)
			{
				jpeg_read_scanlines(cinfo, buffer, 1);
				aktline = cinfo->output_scanline;
			}
			else
				aktline++;

			py = (posy + aktline - 1) * skinfb->width;
			for(x = 0; x < width; x++)
			{
				px = colbytes * x;

				if(cinfo != NULL)
					red = buffer[0][px];
				else
					red = buf[((aktline - 1) * row_stride) + px];
 
				if(colbytes > 2)
				{
					if(cinfo != NULL)
					{
						green = buffer[0][px + 1];
						blue = buffer[0][px + 2];
						color = (255 << 24) | (red << 16) | (green << 8) | blue;
					}
					else
					{
						green = buf[((aktline - 1) * row_stride) + (px + 1)];
						blue = buf[((aktline - 1) * row_stride) + (px + 2)];
						color = (255 << 24) | (red << 16) | (green << 8) | blue;
					}
				}
				else
					color = (255 << 24) | (red << 16) | (red << 8) | red;

				drawpixelfast(posx + x, py, color);
			}
		} 
	}

	return 0;
}

int readjpgsw(const char* filename, int posx, int posy, int mwidth, int mheight, int scalewidth, int scaleheight, int halign, int valign)
{
	struct jpeg_decompress_struct cinfo;
	struct jpgerror jerr;
	FILE* fd = NULL;
	int width = 0, height = 0;

	fd = fopen(filename, "rb");
	if(fd == NULL)
	{
		perr("open jpg file %s", filename);
		return 1;
	}

	cinfo.err = jpeg_std_error(&jerr.jerr);
	jerr.jerr.error_exit = jpgswerror;
	if(setjmp(jerr.setjmpbuf))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(fd);
		return 1;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fd);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.out_color_space = JCS_RGB;

	jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;
	height = cinfo.output_height;
	cinfo.scale_denom = 1;
	
	if(scalewidth != 0 || scaleheight != 0)
	{
		//auto scale to mwidth / mheight
		if(scalewidth == 1 && scaleheight == 1)
			calcautoscale(cinfo.output_width, cinfo.output_height, mwidth, mheight, &scalewidth, &scaleheight);

		if(scalewidth == 0) scalewidth = cinfo.output_width;
		if(scaleheight == 0) scaleheight = cinfo.output_height;
		if(scalewidth > mwidth) scalewidth = mwidth;
		if(scaleheight > mheight) scaleheight = mheight;

		int tmpwidth = cinfo.output_width;
		int tmpheight = cinfo.output_height;
		while(scalewidth < tmpwidth || scaleheight < tmpheight)
		{
			tmpwidth /= 2; tmpheight /= 2; cinfo.scale_denom *= 2;
			if(cinfo.scale_denom > 8) break;
		}
	}

	drawjpgsw(&cinfo, NULL, posx, posy, width, height, cinfo.output_components, mwidth, mheight, scalewidth, scaleheight, halign, valign);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fd);
	return 0;
}

unsigned char* readpng(const char* filename, unsigned long* width, unsigned long* height, unsigned long* rowbytes, int* channels, int posx, int posy, int mwidth, int mheight, int halign, int valign)
{
	debug(1000, "in");
	FILE *fd = NULL;
	int ret, bit_depth, color_type;
	unsigned char *buf = NULL;
	static png_structp png_ptr = NULL;
	static png_infop info_ptr = NULL;
	double gamma;
	png_uint_32 y;
	png_bytepp row_pointers = NULL;
	png_bytep sig = NULL;

	fd = fopen(filename, "rb");
	if(fd == NULL)
	{
		perr("open png file %s", filename);
		return NULL;
	}

	sig = malloc(8);
	if(sig == NULL)
	{
		err("no memory");
		fclose(fd);
		return NULL;
	}

	fread(sig, 1, 8, fd);
	ret = png_check_sig(sig, 8);
	if(ret == 0)
	{
		err("%s is not a PNG file", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL)
	{
		err("%s no memory", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		err("%s no memory", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	ret = setjmp(png_jmpbuf(png_ptr));
	if(ret != 0)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		err("%s has bad IHDR (libpng longjmp)", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	png_init_io(png_ptr, fd);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)width, (png_uint_32*)height, &bit_depth, &color_type, NULL, NULL, NULL);

	ret = setjmp(png_jmpbuf(png_ptr));
	if(ret != 0)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		err("%s unknown error", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand(png_ptr);
	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);
	if(bit_depth == 16)
		png_set_strip_16(png_ptr);
	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);
	if(png_get_gAMA(png_ptr, info_ptr, &gamma))
		png_set_gamma(png_ptr, 2.2, gamma);

	png_read_update_info(png_ptr, info_ptr);
	*rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	*channels = (int)png_get_channels(png_ptr, info_ptr);

	buf = malloc((*rowbytes) * (*height));
	if(buf == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		err("%s no memory", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	row_pointers = (png_bytepp)malloc((*height) * sizeof(png_bytep));
	if(row_pointers == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		err("%s unknown error", filename);
		free(sig);
		fclose(fd);
		return NULL;
	}

	for (y = 0; y < (*height); ++y)
		row_pointers[y] = (png_bytep)buf + y * (*rowbytes);

	png_read_image(png_ptr, row_pointers);
	free(row_pointers);
	row_pointers = NULL;

	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	free(sig);
	fclose(fd);

	debug(100, "png width=%ld height=%ld channels=%d rowbytes=%ld", *width, *height, *channels, *rowbytes);

	debug(1000, "out");
	return buf;
}

void drawpic(const char* filename, int posx, int posy, int scalewidth, int scaleheight, int mwidth, int mheight, int halign, int valign)
{
	debug(1000, "in");
	unsigned char *buf = NULL, *scalebuf = NULL;
	int memfd = -1, py = 0, pyh = 0, pxw = 0, diff = 0;
	unsigned long width, height, rowbytes;
	int channels, length;
	unsigned char *src;
	png_uint_32 y, x;
	struct pic* picnode = NULL;
	int decoding = getconfigint("pichwdecode", NULL);
	int pictype = 0; //0 = png, 1 = jpg HW, 2 = jpg SW

	length = strlen(filename);
	if(filename[length - 1] == 'g' && filename[length - 2] == 'n' && filename[length - 3] == 'p')
		pictype = 0;
	else if(decoding == 1)
		pictype = 1;
	else
		pictype = 2;

	picnode = getpic((char*)filename);
	if(picnode == NULL)
	{
		if(pictype == 0)
			buf = readpng(filename, &width, &height, &rowbytes, &channels, 0, 0, 0, 0, 0, 0);
		else if(pictype == 1)
			readjpg(filename, &width, &height, &rowbytes, &channels, &buf, &memfd);
		else if(pictype == 2)
			readjpgsw(filename, posx, posy, mwidth, mheight, scalewidth, scaleheight, halign, valign);
	}
	else
	{
		buf = picnode->picbuf;
		width = picnode->width;
		height = picnode->height;
		rowbytes = picnode->rowbytes;
		channels = picnode->channels;
		memfd = picnode->memfd;
	}

	if(buf == NULL) return;

	if(pictype == 0 && (scalewidth != 0 || scaleheight != 0) && (scalewidth != width || scaleheight != height))
	{
		//auto scale to mwidth / mheight
		if(scalewidth == 1 && scaleheight == 1)
			calcautoscale(width, height, mwidth, mheight, &scalewidth, &scaleheight);
	
		if(scalewidth == 0) scalewidth = width;
		if(scaleheight == 0) scaleheight = height;
		if(picnode == NULL)
			scalebuf = scale(buf, rowbytes / channels, height, channels, scalewidth, scaleheight, 1);
		else
		{
			scalebuf = scale(buf, rowbytes / channels, height, channels, scalewidth, scaleheight, 0);
			buf = NULL;
			picnode = NULL;
		}
		if(scalebuf != NULL)
		{
			buf = scalebuf;
			width = scalewidth;
			height = scaleheight;
			rowbytes = scalewidth * (channels);
		}
	}

	if(pictype == 0)
	{
		if(width > mwidth) width = mwidth;
		if(height > mheight) height = mheight;

		if(halign == CENTER)
			posx += mwidth / 2 - width / 2;
		else if(halign == RIGHT)
			posx += mwidth - width;
		if(valign == MIDDLE)
			posy += mheight / 2 - height / 2;
		else if(valign == BOTTOM)
			posy += mheight - height;

		py = (posy * skinfb->width) + posx;
		pyh = py + (height * skinfb->width);
		src = buf;
		diff = rowbytes - (width * channels);

		if(channels == 3)
		{
			for(y = py; y < pyh; y += skinfb->width)
			{
				pxw = y + width;
				for(x = y; x < pxw; x++)
				{
					skinfb->fblong[x] = (255 << 24) | (src[0] << 16) | (src[1] << 8) | src[2];
					src += 3;
				}
				src += diff;
			}
		}
		else
		{
			for(y = py; y < pyh; y += skinfb->width)
			{
				pxw = y + width;
				for(x = y; x < pxw; x++)
				{
					if(src[3] < 10)
					{
						src += 4;
						continue;
					}
					skinfb->fblong[x] = (src[3] << 24) | (src[0] << 16) | (src[1] << 8) | src[2];
					src += 4;
				}
				src += diff;
			}
		}
	}
	else if(pictype == 1 && memfd > -1)
		blitjpg(buf, posx, posy, width, height, scalewidth, scaleheight, mwidth, mheight, halign, valign);
	else if(pictype == 2 && picnode != NULL)
		drawjpgsw(NULL, buf, posx, posy, width, height, rowbytes / width, mwidth, mheight, scalewidth, scaleheight, halign, valign);

	if(picnode == NULL)
	{
		if(pictype == 0)
			free(buf);
		else if(pictype == 1)
			freebpamem(memfd, buf, width * height * 3);
	}
	
	debug(1000, "out");
}

//flag 0: don't del outside
//flag 1: del outside
void drawcircle(int x0, int y0, int radius, int startangle, int endangle, long color, int transparent, char* bg, int flag)
{
	float d = (5/4.0) - radius, theta;
	int x = 0, y = radius, px = 0, py = 0;

	transparent = (transparent - 255) * -1;
        unsigned long tmpcol = color | ((transparent & 0xff) << 24);

	while(y > x)
	{
		if(d < 0)
			d += 2 * x + 3;
		else
		{
			d += (2 * x) - (2 * y) + 5;
			y--;
		}
		x++;

		theta = (atan((float)y / x)) * (180 / 3.14);

		//rt2
		if(theta >= startangle && theta <= endangle)
		{
			px = x0 + x;
			py = (y0 - y) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - x;
				int r = radius - y;
				for(; i > 0; i--)
					drawpixelfast(px + i, py, getpixelbuf(bg, x + i - 1, r, radius));
			}
		}
		//rb2
		if(360 - theta >= startangle && 360 - theta <= endangle)
		{
			px = x0 + x;
			py = (y0 + y) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - x;
				int r = y - 1;
				for(; i > 0; i--)
					drawpixelfast(px + i, py, getpixelbuf(bg, x + i - 1, r, radius));
			}
		}
		//rt1
		if(90 - theta >= startangle && 90 - theta <= endangle)
		{
			px = x0 + y;
			py = (y0 - x) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - y;
				int r = radius - x;
				for(; i > 0; i--)
					drawpixelfast(px + i, py, getpixelbuf(bg, y + i - 1, r, radius));
			}
		}
		//rb1
		if(270 + theta >= startangle && 270 + theta <= endangle)
		{
			px = x0 + y;
			py = (y0 + x) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - y;
				int r = x - 1;
				for(; i > 0; i--)
					drawpixelfast(px + i, py, getpixelbuf(bg, y + i - 1, r, radius));
			}
		}
		//lt2
		if(180 - theta >= startangle && 180 - theta <= endangle)
		{
			px = x0 - x;
			py = (y0 - y) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - x;
				int r = radius - y;
				for(; i > 0; i--)
					drawpixelfast(px - i, py, getpixelbuf(bg, radius - x - i, r, radius));
			}
		}
		//lb2
		if(180 + theta >= startangle && 180 + theta <= endangle)
		{
			px = x0 - x;
			py = (y0 + y) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - x;
				int r = y - 1;
				for(; i > 0; i--)
					drawpixelfast(px - i, py, getpixelbuf(bg, radius - x - i, r, radius));
			}
		}
		//lt1
		if(90 + theta >= startangle && 90 + theta <= endangle)
		{
			px = x0 - y;
			py = (y0 - x) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - y;
				int r = radius - x;
				for(; i > 0; i--)
					drawpixelfast(px - i, py, getpixelbuf(bg, radius - y - i, r, radius));
			}
		}
		//lb1
		if(270 - theta >= startangle && 270 - theta <= endangle)
		{
			px = x0 - y;
			py = (y0 + x) * skinfb->width;
			drawpixelfast(px, py, tmpcol);
			if(flag == 1)
			{
				int i = radius - y;
				int r = x - 1;
				for(; i > 0; i--)
					drawpixelfast(px - i, py, getpixelbuf(bg, radius - y - i , r, radius));
			}
		}
	}
}

int drawchar(struct font* font, FT_ULong currentchar, int posx, int posy, int mwidth, int height, long color, int transparent, int charspace, int test)
{
//	debug(1000, "in");
	int space = 0, y = 0, x = 0, py = 0, pxw = 0, pyh = 0, max = 220, min = 35;
	FT_UInt glyphindex;
	FT_Vector kerning;
	FT_Error ret;
	FTC_SBit sbit;
	unsigned long tmpcol = 0, tmpcol1 = 0;
	long buffercol = 0;
	unsigned char red, green, blue, r, g, b;
	unsigned char* src = NULL;

	if(currentchar == 32) space = 1;

	glyphindex = FT_Get_Char_Index(font->face, currentchar);
	if(glyphindex == 0)
	{
		debug(100, "FT_Get_Char_Index for char %x %c failed", (int)currentchar, (int)currentchar);
		return 0;
	}

	FTC_Node anode;
	ret = FTC_SBitCache_Lookup(font->cache, &font->desc, glyphindex, &sbit, &anode);

	if(ret != 0)
	{
		err("FTC_SBitCache_Lookup for char %x %c failed. Error: 0x%.2X", (int)currentchar, (int)currentchar, ret);
		return 0;
	}

	if(font->use_kerning)
	{
		FT_Get_Kerning(font->face, font->prev_glyphindex, glyphindex, ft_kerning_default, &kerning);
		font->prev_glyphindex = glyphindex;
		kerning.x >>= 6;
	}
	else
		kerning.x = 0;

	if (test == 1 || space == 1)
	{
		//debug(1000, "out -> test or space");
		return sbit->xadvance + kerning.x + charspace;
	}
	if(posx + sbit->xadvance > mwidth)
	{
		//debug(1000, "out -> text to long");
		return -1;
	}

	if(status.fasttextrender == 1)
	{
		max = 150;
		min = 100;
	}
	
	red = (color & 0xff0000) >> 16;
	green = (color & 0x00ff00) >> 8;
	blue = color & 0xff;
	transparent = transparent & 0xff;
	tmpcol = color | transparent << 24;
	posy = posy + height - sbit->top;
	posx = posx + sbit->left + kerning.x;
		
	py = (posy * skinfb->width) + posx;
	pyh = py + (sbit->height * skinfb->width);
	src = sbit->buffer;
		
	for(y = py; y < pyh; y += skinfb->width)
	{
		pxw = y + sbit->pitch;
		for(x = y; x < pxw; x++)
		{
			buffercol = src[0];
			src++;
			if(buffercol)
			{
				//renderquality 255-0 = best
				if(buffercol > max)
					skinfb->fblong[x] = tmpcol;
				else if(buffercol < min)
					continue;
				else
				{
					tmpcol1 = skinfb->fblong[x];
					alpha_composite(r, red, buffercol, (tmpcol1 & 0xff0000) >> 16);
					alpha_composite(g, green, buffercol, (tmpcol1 & 0x00ff00) >> 8);
					alpha_composite(b, blue, buffercol, tmpcol1 & 0xff);
					skinfb->fblong[x] = transparent << 24 | r << 16 | g << 8 | b;
				}
			}
		}
	}

//	debug(1000, "out");
	return sbit->xadvance + kerning.x + charspace;
}

void getstringwh(struct font* font, char *string, int *stringwidth, int *stringheight, int charspace)
{
	debug(1000, "in");
	FT_ULong cret = 0;
	if(string == NULL) return;

	while(*string != '\0' && *string != '\n')
	{
		string += strfreetype(string, &cret);
		*stringwidth += drawchar(font, cret, 0, 0, 0, 0, 0, 0, charspace, 1);
		string++;
	}
	debug(1000, "out");
}

int calcstrhalign(struct font* aktfont, char *string, int posx, int mwidth, int halign, int charspace)
{
	int stringwidth = 0, stringheight = 0;
	if(string == NULL) return 0;

	switch(halign)
	{
		case TEXTCENTER:
		case CENTER:
			getstringwh(aktfont, string, &stringwidth, &stringheight, charspace);
			if(stringwidth < mwidth) posx += (mwidth - stringwidth) / 2;
			break;
		case TEXTRIGHT:
		case RIGHT:
			getstringwh(aktfont, string, &stringwidth, &stringheight, charspace);
			if(stringwidth < mwidth) posx += mwidth - stringwidth;
			break;
	}
	return posx;
}

int calcstrvalign(char *string, int posy, int oldposy, int mheight, int fontsize, int linecount, int valign)
{
	if(string == NULL) return 0;

	switch(valign)
	{
		case TEXTMIDDLE:
		case MIDDLE:
			posy += mheight / 2 - (fontsize * linecount) / 2;
			if(posy < oldposy) posy = oldposy;
			break;
		case TEXTBOTTOM:
		case BOTTOM:
			posy += mheight - (fontsize * linecount);
			if(posy < oldposy) posy = oldposy;
			break;
	}

	return posy;
}

struct font* setaktfont(char* fontname, int fontsize)
{
	struct font* aktfont = NULL;

	if(fontname != NULL) aktfont = getfont(fontname);
	if(aktfont == NULL) aktfont = font;

	aktfont->desc.width = fontsize;
	aktfont->desc.height = fontsize;
	aktfont->desc.flags = FT_LOAD_DEFAULT;
	aktfont->prev_glyphindex = 0;

	return aktfont;
}

void wrapstr(char* string, char* fontname, int fontsize, int mwidth, int charspace)
{
	int posx = 0;
	int stringheight = 0, stringwidth = 0;
	struct font* aktfont = NULL;
	char* tmpstr = NULL, *origstr = string;
	unsigned char c = 0;

	if(string == NULL) return;
	aktfont = setaktfont(fontname, fontsize);

	while(*string != '\0')
	{
		stringheight = 0; stringwidth = 0;

		tmpstr = string;

		while(*string != '\0' && *string != '\n' && *string != ' ' && *string != '-')
			string++;
		if(*string == '\n')
		{
			if(*string != '\0') string++;
			posx = 0;
		}
		else
		{
			if(*string != '\0') string++;
			c = *string; *string = '\0';

			getstringwh(aktfont, tmpstr, &stringwidth, &stringheight, charspace);
			*string = c;

			posx += stringwidth;
		}

		if(posx > mwidth && tmpstr > origstr)
		{	
			tmpstr--;
			*tmpstr = '\n';
			posx = stringwidth;
		}
	}
}


int drawstring(char* string, unsigned long linecount, unsigned int poscount, unsigned int markpos, int posx, int posy, int mwidth, int mheight, int halign, int valign, char* fontname, int fontsize, long color, int transparent, int wrap, int* lastposx, int* lastposy, int* len, int charspace)
{
	debug(1000, "in");
	int charwidth = 0, lineend = 0;
	int charcount = 0;
	if(lastposy == NULL || *lastposy == 0) posy=posy + FONTPOSYOFFSET;
	int oldposx = posx, aktheight = 0, ret = 0;
	int oldposy = posy, oldmwidth = mwidth;
	struct font* aktfont = NULL;
	long tmpcol = color;
	FT_ULong cret = 0;

	if(string == NULL || color == -1) return 1;

	transparent = (transparent - 255) * -1;

	if(linecount < 2 && (fontsize == 0 || fontsize > mheight))
		fontsize = mheight;

	aktfont = setaktfont(fontname, fontsize);

	string = string + poscount;

	if(fontsize < 5)
	{
		debug(1000, "out -> fontsize to small");
		return 1;
	}

	posx = calcstrhalign(aktfont, string, posx, oldmwidth, halign, charspace);
	posy = calcstrvalign(string, posy, oldposy, mheight, fontsize, linecount, valign);

	mwidth = posx + mwidth;
	aktheight += fontsize;
	if(aktheight > mheight)
	{
		debug(1000, "out -> to many textlines");
		return 1;
	}

	while(*string != '\0')
	{
		charcount++;
		if(markpos == charcount)
			color = convertcol("markcol");
		else
			color = tmpcol;

		if(*string == '\n')
		{
			if(linecount < 2) break;
			lineend = 0;
			aktheight += fontsize;
			if(aktheight > mheight)
			{
				ret = 1;
				break;
			}
			posy = posy + fontsize;
			posx = oldposx;
			string++;
			posx = calcstrhalign(aktfont, string, posx, oldmwidth, halign, charspace);
			continue;
		}

		if(lineend == 1)
		{
			string++;
			continue;
		}

		string += strfreetype(string, &cret);

		if((charwidth = drawchar(aktfont, cret, posx, posy, mwidth, fontsize, color, transparent, charspace, 0)) == -1)
			lineend = 1;

		posx += charwidth;
		if(len != NULL) *len += charwidth;
		string++;
	}
	if(lastposx != NULL) *lastposx = posx;
	if(lastposy != NULL) *lastposy = posy + fontsize;
	debug(1000, "out");
	return ret;
}

char* saverect(int posx, int posy, int width, int height)
{
	debug(1000, "in");
	char* buf;
	int y;

	buf = malloc(width * height * skinfb->colbytes);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	for(y = 0; y < height; y++)
	{
		memcpy(buf + width * skinfb->colbytes * y, skinfb->fb + ((y + posy) * skinfb->pitch) + (posx * skinfb->colbytes), width * skinfb->colbytes);
	}

	debug(1000, "out");
	return buf;
}

char* savescreen(struct skin* node)
{
	return saverect(node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->rwidth + (node->shadowsize * 2), node->rheight + (node->shadowsize * 2));
}

//flag 0: no free
//flag 1: free
void restorerectcheck(char* buf, int posx, int posy, int width, int height, int flag)
{
	debug(1000, "in");
	int y;

	if(buf != NULL)
	{
		for(y = 0; y < height; y++)
		{
			memcpy(skinfb->fb + ((y + posy) * skinfb->pitch) + (posx * skinfb->colbytes), buf + width * skinfb->colbytes * y,  width * skinfb->colbytes);
		}
		if(flag == 1)
		{
			free(buf);
			buf = NULL;
		}
	}
	debug(1000, "out");
}

void restorerect(char* buf, int posx, int posy, int width, int height)
{
	restorerectcheck(buf, posx, posy, width, height, 1);
}

void restorescreen(char* buf, struct skin* node)
{
	restorerectcheck(buf, node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->rwidth + (node->shadowsize * 2), node->rheight + (node->shadowsize * 2), 1);
}

void restorescreennofree(char* buf, struct skin* node)
{
	restorerectcheck(buf, node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->rwidth + (node->shadowsize * 2), node->rheight + (node->shadowsize * 2), 0);
}

//*************** GOST LCD
void lcd_fillrect(int posx, int posy, int width, int height, long color, int transparent)
{
	//debug(1000, "in");
	int y, x;
	unsigned long tmpcol;

	if(posx < 0) posx = 0;
	if(posx > skinfb->width) posx = skinfb->width;
	if(posy < 0) posy = 0;
	if(posy > skinfb->height) posy = skinfb->height;
	if(posx + width > skinfb->width) width = skinfb->width - posx;
	if(posy + height > skinfb->height) height = skinfb->height - posy;

	if(width <= 0 || height <= 0) return;

	transparent = (transparent - 255) * -1;
	tmpcol = color | ((transparent & 0xff) << 24);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			drawpixel(posx + x, posy + y, tmpcol);
		}
	}
	//debug(1000, "out");
}

void lcd_drawrect(int posx, int posy, int width, int height, long color, int transparent)
{
	//debug(1000, "in");
	fillrect(posx, posy, width, 1, color, transparent);
	fillrect(posx, posy + height - 1, width, 1, color, transparent);
	fillrect(posx, posy, 1, height, color, transparent);
	fillrect(posx + width - 1, posy, 1, height, color, transparent);
	//debug(1000, "out");
}
//*************** GOST LCD

/*
void fillrect(int posx, int posy, int width, int height, long color, int transparent)
{
	int r, g, b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;

	primary->SetDrawingFlags(primary, DSDRAW_NOFX);
	primary->SetColor(primary, r, g, b, transparent);
	primary->FillRectangle(primary, posx, posy, width, height);
	primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC);
}

void drawrect(int posx, int posy, int width, int height, long color, int transparent)
{
	int r, g, b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;

	primary->SetDrawingFlags(primary, DSDRAW_NOFX);
	primary->SetColor(primary, r, g, b, transparent);
	primary->DrawRectangle(primary, posx, posy, width, height);
	primary->Flip(primary, NULL, DSFLIP_WAITFORSYNC);
}
*/

void fillrect(int posx, int posy, int width, int height, long color, int transparent)
{
	if(skinfb != lcdskinfb)
		blitrect(posx, posy, width, height, color, transparent, 0);
	else
		lcd_fillrect(posx, posy, width, height, color, transparent);
}

void drawrect(int posx, int posy, int width, int height, long color, int transparent)
{
	if(skinfb != lcdskinfb)
		blitrect(posx, posy, width, height, color, transparent, 1);
	else
		lcd_drawrect(posx, posy, width, height, color, transparent);
}

void clearrect(int posx, int posy, int width, int height)
{
	fillrect(posx, posy, width, height, 0, 255);
}

void clearscreen(struct skin* node)
{
	m_lock(&status.drawingmutex, 0);
	clearrect(node->rposx, node->rposy, node->rwidth, node->rheight);
	clearshadow(node);
	m_unlock(&status.drawingmutex, 0);
}

void clearscreennolock(struct skin* node)
{
	clearrect(node->rposx, node->rposy, node->rwidth, node->rheight);
	clearshadow(node);
}

//flag 0 = horizontal
//flag 1 = vertical
//flag 2 = horizontal (begin to middle, middle to end)
//flag 3 = vertical (begin to middle, middle to end)
void drawgradient(int posx, int posy, int width, int height, long col1, long col2, int transparent, int flag)
{
	int p, i, x, y, steps, xstep, ystep;
	int xcount = 0, ycount = 0, owidth = width, oheight = height;
	unsigned char r3, g3, b3;
	unsigned long col = 0;

	transparent = (transparent - 255) * -1;

	if(flag == LEFTRIGHT || flag == LEFTMIDDLE)
	{
		if(flag == LEFTMIDDLE) width = width / 2;
		if(width < 100)
			steps = width;
		else
			steps = width / 5;
		xstep = width / steps;
		ystep = height;
	}
	else
	{
		if(flag == TOPMIDDLE) height = height / 2;
		if(height < 100)
			steps = height;
		else
			steps = height / 5;
		xstep = width;
		ystep = height / steps;
	}

	unsigned char r1 = (col1 >> 16) & 0xff;
	unsigned char g1 = (col1 >> 8) & 0xff;
	unsigned char b1 = col1 & 0xff;

	unsigned char r2 = (col2 >> 16) & 0xff;
	unsigned char g2 = (col2 >> 8) & 0xff;
	unsigned char b2 = col2 & 0xff;

	for(i = 0; i < steps; i++)
	{
		p = steps - i;
		r3 = (r1 * p + r2 * i) / steps;
		g3 = (g1 * p + g2 * i) / steps;
		b3 = (b1 * p + b2 * i) / steps;
		col = (transparent << 24) | (r3 << 16) | (g3 << 8) | b3;
		
		for(y = 0; y < ystep; y++)
			for(x = 0; x < xstep; x++)
				drawpixel(posx + x, posy + y, col);

		if(flag == LEFTRIGHT || flag == LEFTMIDDLE)
		{
			posx += xstep;
			xcount += xstep;
		}
		else 
		{
			posy += ystep;
			ycount += ystep;
		}
	}

	if(flag == LEFTMIDDLE || flag == TOPMIDDLE)
	{
		for(i = 0; i < steps; i++)
		{
			p = steps - i;
			r3 = (r2 * p + r1 * i) / steps;
			g3 = (g2 * p + g1 * i) / steps;
			b3 = (b2 * p + b1 * i) / steps;
			col = (transparent << 24) | (r3 << 16) | (g3 << 8) | b3;
		
			for(y = 0; y < ystep; y++)
				for(x = 0; x < xstep; x++)
					drawpixel(posx + x, posy + y, col);
			if(flag == LEFTMIDDLE)
			{
				posx += xstep;
				xcount += xstep;
			}
			else 
			{
				posy += ystep;
				ycount += ystep;
			}
		}
	}

	if(flag == LEFTRIGHT || flag == LEFTMIDDLE)
	{
		if(owidth > xcount)
		{
			for(y = 0; y < ystep; y++)
				for(x = 0; x < (owidth - xcount); x++)
					drawpixel(posx + x, posy + y, col);
		}
	}
	if(flag == TOPBOTTOM || flag == TOPMIDDLE)
	{
		if(oheight > ycount)
		{
			for(y = 0; y < (oheight - ycount); y++)
				for(x = 0; x < xstep; x++)
					drawpixel(posx + x, posy + y, col);
		}
	}
}

void drawtitlebggradient(struct skin* node)
{
	debug(1000, "in");
	if(status.picbordersize > 0)
		drawgradient(node->rposx + status.picbordersize, node->rposy + status.picbordersize, node->rwidth - (status.picbordersize * 2), node->rheight - (node->rheight - node->titlesize), node->titlebgcol, node->titlebgcol2, node->transparent, node->titlegradient);
	else
		drawgradient(node->rposx, node->rposy, node->rwidth, node->rheight - (node->rheight - node->titlesize), node->titlebgcol, node->titlebgcol2, node->transparent, node->titlegradient);
	debug(1000, "out");
}

void drawbggradient(struct skin* node)
{
	debug(1000, "in");
	drawgradient(node->rposx, node->rposy, node->rwidth, node->rheight, node->bgcol, node->bgcol2, node->transparent, node->gradient);
	debug(1000, "out");
}

void drawtitlebgcol(struct skin* node)
{
	debug(1000, "in");
	if(status.picbordersize > 0)
		fillrect(node->rposx + status.picbordersize, node->rposy + status.picbordersize, node->rwidth - (status.picbordersize * 2), node->rheight - (node->rheight - node->titlesize), node->titlebgcol, node->transparent);
	else
		fillrect(node->rposx, node->rposy, node->rwidth, node->rheight - (node->rheight - node->titlesize), node->titlebgcol, node->transparent);
	debug(1000, "out");
}

void drawbginnercol(struct skin* node)
{
	debug(1000, "in");
	fillrect(node->rposx + node->bordersize, node->rposy + node->bordersize, node->rwidth - node->bordersize * 2, node->rheight - node->bordersize * 2, node->bgcol, node->transparent);
	debug(1000, "out");
}

void drawbgcol(struct skin* node)
{
	debug(1000, "in");
	fillrect(node->rposx + node->bgspace, node->rposy + node->bgspace, node->rwidth - (node->bgspace * 2), node->rheight - (node->bgspace * 2), node->bgcol, node->transparent);
	debug(1000, "out");
}

void drawtitle(struct skin* node)
{
	debug(1000, "in");
	if(status.titlelinesize > 0)
		drawstring(node->title, 1, 0, -1, node->iposx, node->rposy + node->bordersize, node->iwidth, node->titlesize - status.titlelinesize, node->titlealign, MIDDLE, node->font, node->fontsize, node->fontcol, node->transparent, 0, NULL, NULL, NULL, node->charspace);
	else
		drawstring(node->title, 1, 0, -1, node->iposx, node->rposy + node->bordersize, node->iwidth, node->titlesize - node->bordersize, node->titlealign, MIDDLE, node->font, node->fontsize, node->fontcol, node->transparent, 0, NULL, NULL, NULL, node->charspace);
	if(status.titlelinesize > 0)
		fillrect(node->rposx, node->rposy + node->titlesize + status.picbordersize, node->rwidth, status.titlelinesize, node->bordercol, node->transparent);
	else if(node->bordersize > 0)
		fillrect(node->rposx, node->rposy + node->titlesize, node->rwidth, node->bordersize, node->bordercol, node->transparent);
	debug(1000, "out");
}

void drawprogressbar(struct skin* node)
{
	debug(1000, "in");
	int val = 0;

	if(node->progresssize > 100) node->progresssize = 100;
	val = ((float)node->iwidth / 100) * node->progresssize;
	fillrect(node->rposx + node->bordersize, node->rposy + node->bordersize, val, node->iheight, node->progresscol, node->transparent);
	debug(1000, "out");
}

void drawmultiprogressbar(struct skin* node)
{
	debug(1000, "in");
	struct epgrecord* epgrecord = node->epgrecord;
	int val1 = 0, val2 = 0;

	while(epgrecord != NULL)
	{
		
		if(epgrecord->posx > 100) epgrecord->posx = 100;
		val1 = ((float)node->iwidth / 100) * epgrecord->posx;
		if(epgrecord->size > 100) epgrecord->size = 100;
		val2 = ((float)node->iwidth / 100) * epgrecord->size;

		if(val2 > val1)
			fillrect(node->rposx + node->bordersize + val1, node->rposy + node->bordersize, val2 - val1, node->iheight, node->progresscol, node->transparent);
		epgrecord = epgrecord->next;
	}
	debug(1000, "out");
}

void drawroundborder(struct skin* node, char* bglt, char* bglb, char* bgrt, char* bgrb)
{
	int i, rad = status.borderradius;

	if(node->borderradius > 0) rad = node->borderradius;
	if(rad > node->rheight / 2) rad = node->rheight / 2;
	int tmpbordersize = rad - node->bordersize;

	//left - top
	for(i = rad; i > tmpbordersize; i--)
		drawcircle(node->rposx + rad, node->rposy + rad, i, 90, 180, node->bordercol, node->transparent, bglt, i == rad);
	//left - bottom
	for(i = rad; i > tmpbordersize; i--)
		drawcircle(node->rposx + rad, node->rposy + node->rheight - 1 - rad, i, 180, 270, node->bordercol, node->transparent, bglb, i == rad);
	//right - top
	for(i = rad; i > tmpbordersize; i--)
		drawcircle(node->rposx + node->rwidth - 1 - rad, node->rposy + rad, i, 0, 90, node->bordercol, node->transparent, bgrt, i == rad);
	//right - bottom
	for(i = rad; i > tmpbordersize; i--)
		drawcircle(node->rposx + node->rwidth - 1 - rad, node->rposy + node->rheight - 1 - rad, i, 270, 360, node->bordercol, node->transparent, bgrb, i == rad);
}

//TODO: not full implemented
void drawpicborder(struct skin* node)
{
	debug(1000, "in");
	int borderwidth = status.picbordersize;
	int borderheight = status.picbordersize;

	//top-left
	//drawpic("/home/nit/titan/skin/bs_tl.png", node->rposx + node->bordersize - borderwidth, node->rposy + node->bordersize - borderheight, 0, 0, borderwidth, borderheight, LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_tl.png", node->rposx, node->rposy, 0, 0, borderwidth, borderheight, LEFT, TOP);
	//top-right
	//drawpic("/home/nit/titan/skin/bs_tr.png", node->rposx - node->bordersize + node->rwidth, node->rposy + node->bordersize - borderheight, 0, 0, borderwidth, borderheight, LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_tr.png", node->rposx + node->rwidth - borderwidth, node->rposy, 0, 0, borderwidth, borderheight, LEFT, TOP);
	//bottom-left
	//drawpic("/home/nit/titan/skin/bs_bl.png", node->rposx + node->bordersize - borderwidth, node->rposy - node->bordersize + node->rheight, 0, 0, borderwidth, borderheight, LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_bl.png", node->rposx, node->rposy + node->rheight - borderheight, 0, 0, borderwidth, borderheight, LEFT, TOP);
	//bottom-right
	//drawpic("/home/nit/titan/skin/bs_br.png", node->rposx - node->bordersize + node->rwidth, node->rposy - node->bordersize + node->rheight, 0, 0, borderwidth, borderheight, LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_br.png", node->rposx + node->rwidth - borderwidth, node->rposy + node->rheight - borderheight, 0, 0, borderwidth, borderheight, LEFT, TOP);

	//top
	//drawpic("/home/nit/titan/skin/bs_t.png", node->rposx + node->bordersize, node->rposy + node->bordersize - borderheight, node->rwidth - (node->bordersize * 2), 0, node->rwidth - (node->bordersize * 2), borderheight, LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_t.png", node->rposx + borderwidth, node->rposy, node->rwidth - (borderwidth * 2), 0, node->rwidth - (borderwidth * 2), borderheight, LEFT, TOP);
	//bottom
	//drawpic("/home/nit/titan/skin/bs_b.png", node->rposx + node->bordersize, node->rposy - node->bordersize + node->rheight, node->rwidth - (node->bordersize * 2), 0, node->rwidth - (node->bordersize * 2), borderheight, LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_b.png", node->rposx + borderwidth, node->rposy + node->rheight - borderheight, node->rwidth - (borderwidth * 2), 0, node->rwidth - (node->bordersize * 2), borderheight, LEFT, TOP);
	//left
	//drawpic("/home/nit/titan/skin/bs_l.png", node->rposx + node->bordersize - borderwidth, node->rposy + node->bordersize, 0, node->rheight - (node->bordersize * 2), borderwidth, node->rheight - (node->bordersize * 2), LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_l.png", node->rposx, node->rposy + borderheight, 0, node->rheight - (borderheight * 2), borderwidth, node->rheight - (borderheight * 2), LEFT, TOP);
	//right
	//drawpic("/home/nit/titan/skin/bs_r.png", node->rposx - node->bordersize + node->rwidth, node->rposy + node->bordersize, 0, node->rheight - (node->bordersize * 2), borderwidth, node->rheight - (node->bordersize * 2), LEFT, TOP);
	drawpic("/home/nit/titan/skin/bs_r.png", node->rposx + node->rwidth - borderwidth, node->rposy + borderheight, 0, node->rheight - (borderheight * 2), borderwidth, node->rheight - (borderheight * 2), LEFT, TOP);
	debug(1000, "out");
}

void drawborder(struct skin* node)
{
	debug(1000, "in");
	if(node->bordersize == 1 && node->rheight > 2 && node->bordertype == 0)
		drawrect(node->rposx, node->rposy, node->rwidth, node->rheight, node->bordercol, node->transparent);
	else if(node->bordersize == 1 && node->rheight <= 2 && node->bordertype == 0)
		fillrect(node->rposx, node->rposy, node->rwidth, node->rheight, node->bordercol, node->transparent);
	else
	{
		if(node->bordertype == 0 || checkbit(node->bordertype, 0) == 1)
			fillrect(node->rposx, node->rposy, node->rwidth, node->bordersize, node->bordercol, node->transparent);
		if(node->bordertype == 0 || checkbit(node->bordertype, 1) == 1)
			fillrect(node->rposx, node->rposy + node->rheight - node->bordersize, node->rwidth, node->bordersize, node->bordercol, node->transparent);
		if(node->bordertype == 0 || checkbit(node->bordertype, 2) == 1)
			fillrect(node->rposx, node->rposy, node->bordersize, node->rheight, node->bordercol, node->transparent);
		if(node->bordertype == 0 || checkbit(node->bordertype, 3) == 1)
			fillrect(node->rposx + node->rwidth - node->bordersize, node->rposy, node->bordersize, node->rheight, node->bordercol, node->transparent);
	}
	debug(1000, "out");
}

void drawscrollbar(struct skin* node)
{
	debug(1000, "in");
	if(node->bordersize == 0)
		drawrect(node->rposx + node->rwidth - node->bordersize - node->scrollbarwidth, node->iposy, node->scrollbarwidth, node->iheight, node->bordercol, node->transparent);
	else
		fillrect(node->rposx + node->rwidth - node->bordersize - node->scrollbarwidth, node->iposy, node->scrollbarbordersize, node->iheight, node->bordercol, node->transparent);

	fillrect(node->rposx + node->rwidth - node->bordersize - node->scrollbarwidth, node->iposy + node->scrollbarpos, node->scrollbarwidth, node->scrollbarheight, node->bordercol, node->transparent);
	debug(1000, "out");
}

void clearshadow(struct skin* node)
{
	debug(1000, "in");

	if(node->shadowsize < 1) return;
	switch(node->shadowpos)
	{
		case BOTTOMLEFT:
		clearrect(node->rposx - node->shadowsize, node->rposy + node->rheight, node->rwidth, node->shadowsize);
		clearrect(node->rposx - node->shadowsize, node->rposy + node->shadowsize, node->shadowsize, node->rheight);
		break;
		case BOTTOMRIGHT:
		clearrect(node->rposx + node->shadowsize, node->rposy + node->rheight, node->rwidth, node->shadowsize);
		clearrect(node->rposx + node->rwidth, node->rposy + node->shadowsize, node->shadowsize, node->rheight);
		break;
		case TOPLEFT:
		clearrect(node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->rwidth, node->shadowsize);
		clearrect(node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->shadowsize, node->rheight);
		break;
		default:
		clearrect(node->rposx + node->shadowsize, node->rposy - node->shadowsize, node->rwidth, node->shadowsize);
		clearrect(node->rposx + node->rwidth, node->rposy - node->shadowsize, node->shadowsize, node->rheight);
		break;
	}
	debug(1000, "out");
}

void drawshadow(struct skin* node)
{
	debug(1000, "in");
	switch(node->shadowpos)
	{
		case BOTTOMLEFT:
		fillrect(node->rposx - node->shadowsize, node->rposy + node->rheight, node->rwidth, node->shadowsize, node->shadowcol, node->transparent);
		fillrect(node->rposx - node->shadowsize, node->rposy + node->shadowsize, node->shadowsize, node->rheight, node->shadowcol, node->transparent);
		break;
		case BOTTOMRIGHT:
		fillrect(node->rposx + node->shadowsize, node->rposy + node->rheight, node->rwidth, node->shadowsize, node->shadowcol, node->transparent);
		fillrect(node->rposx + node->rwidth, node->rposy + node->shadowsize, node->shadowsize, node->rheight, node->shadowcol, node->transparent);
		break;
		case TOPLEFT:
		fillrect(node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->rwidth, node->shadowsize, node->shadowcol, node->transparent);
		fillrect(node->rposx - node->shadowsize, node->rposy - node->shadowsize, node->shadowsize, node->rheight, node->shadowcol, node->transparent);
		break;
		default:
		fillrect(node->rposx + node->shadowsize, node->rposy - node->shadowsize, node->rwidth, node->shadowsize, node->shadowcol, node->transparent);
		fillrect(node->rposx + node->rwidth, node->rposy - node->shadowsize, node->shadowsize, node->rheight, node->shadowcol, node->transparent);
		break;
	}
	debug(1000, "out");
}

//flag 0: del background
//flag 1: don't del background
void drawnode(struct skin* node, int flag)
{
	long color = 0, color2 = 0;
	int len = 0;
	char* bglt = NULL, *bglb = NULL, *bgrt = NULL, *bgrb = NULL;

	debug(1000, "in");
	
	node->flag = setbit(node->flag, 0);

	if(node->bordersize > 0)
	{
		if((node->child != NULL && status.borderradius > 0) || node->borderradius > 0)
		{
			int rad = status.borderradius;

			if(node->borderradius > 0) rad = node->borderradius;
			if(rad > node->rheight / 2) rad = node->rheight / 2;

			bglt = saverect(node->rposx, node->rposy, rad, rad);
			bglb = saverect(node->rposx, node->rposy + node->rheight - rad, rad, rad);
			bgrt = saverect(node->rposx + node->rwidth - rad, node->rposy, rad, rad);
			bgrb = saverect(node->rposx + node->rwidth - rad, node->rposy + node->rheight - rad, rad, rad);
		}
	}

	if(flag == 0 && node->bgcol == -1)
	{
		if(node->child != NULL && status.picbordersize > 0)
		{
			clearrect(node->rposx + node->bordersize, node->rposy + node->bordersize, node->rwidth - node->bordersize * 2, node->rheight - node->bordersize * 2);
		}
		else
			clearscreennolock(node);
	}

	if(node->deaktivcol > -1)
	{
		color = node->deaktivcol;
		color2 = node->deaktivcol;
	}
	else
	{
		color = node->fontcol;
		color2 = node->fontcol2;
	}

	if(node->shadowsize > 0)
		drawshadow(node);
	if(node->bgcol > -1)
	{
		if(node->child != NULL && status.picbordersize > 0)
			drawbginnercol(node);
		else
			drawbgcol(node);
	}
	if(node->child != NULL && status.bgpic != NULL)
		drawpic(status.bgpic, node->iposx, node->iposy, node->iwidth, node->iheight, node->iwidth, node->iheight, node->halign, node->valign);
	if(node->gradient > 0)
		drawbggradient(node);
	if(node->titlebgcol > -1)
		drawtitlebgcol(node);
	if(node->titlegradient > 0)
		drawtitlebggradient(node);
	if(node->progresssize > 0)
		drawprogressbar(node);
	if(node->type & MULTIPROGRESSBAR)
		drawmultiprogressbar(node);
	if(node->selectpic != NULL && !(node->type & FILELIST))
		drawpic(node->selectpic, node->iposx, node->iposy, node->iwidth, node->iheight, node->iwidth, node->iheight, LEFT, TOP);
	if(node->pic != NULL && !(node->type & FILELIST))
		drawpic(node->pic, node->iposx, node->iposy, node->rpicwidth, node->rpicheight, node->iwidth, node->iheight, node->halign, node->valign);
	if(node->input != NULL)
	{
		if(node->type & CHOICEBOX)
			drawstring(node->input, 1, node->poscount, -1, node->iposx, node->iposy, node->iwidth, node->iheight, RIGHT, node->valign, node->font, node->fontsize, color, node->transparent, 0, NULL, NULL, &len, node->charspace);
		if((node->type & INPUTBOX) || (node->type & INPUTBOXNUM))
			drawstring(node->input, 1, node->poscount, node->aktpage, node->iposx, node->iposy, node->iwidth, node->iheight, RIGHT, node->valign, node->font, node->fontsize, color, node->transparent, 0, NULL, NULL, &len, node->charspace);
	}
	if(node->text != NULL)
	{
		if(node->type & TEXTBOX)
		{
			int lastposy = 0;
			drawstring(node->text, node->linecount, node->poscount, -1, node->iposx + node->textposx, node->iposy, node->iwidth - node->textposx, node->iheight, node->halign, node->valign, node->font, node->fontsize, color, node->transparent, node->wrap, NULL, &lastposy, NULL, node->charspace);
			drawstring(node->text2, node->linecount, node->poscount, -1, node->iposx + node->textposx2, lastposy, node->iwidth - node->textposx2, node->iheight - (lastposy - node->iposy), node->halign, node->valign, node->font, node->fontsize2, color2, node->transparent, node->wrap, NULL, &lastposy, NULL, node->charspace);
		}
		else
		{
			int lastposx = 0;
			if(node->textposx2 > 0)
				drawstring(node->text, 1, 0, -1, node->iposx + node->textposx, node->iposy, node->iwidth - node->textposx - (node->iwidth - node->textposx2) - len, node->iheight, node->halign, node->valign, node->font, node->fontsize, color, node->transparent, 0, &lastposx, NULL, NULL, node->charspace);
			else
				drawstring(node->text, 1, 0, -1, node->iposx + node->textposx, node->iposy, node->iwidth - node->textposx - len, node->iheight, node->halign, node->valign, node->font, node->fontsize, color, node->transparent, 0, &lastposx, NULL, NULL, node->charspace);
			if(node->textposx2 > 0)
				drawstring(node->text2, 1, 0, -1, node->iposx + node->textposx2, node->iposy, node->iwidth - node->textposx2 - len, node->iheight, node->halign, node->valign, node->font, node->fontsize2, color2, node->transparent, 0, NULL, NULL, NULL, node->charspace);
			else
				drawstring(node->text2, 1, 0, -1, lastposx, node->iposy, node->iwidth - (lastposx - node->iposx) - len, node->iheight, node->halign, node->valign, node->font, node->fontsize2, color2, node->transparent, 0, NULL, NULL, NULL, node->charspace);
		}
	}
	if(node->filelist != NULL && node->filelist->view > 3)
	{
		char* tmpnr = NULL;

		if(node->filelist->view == 4)
		{
			if(node->filelist->size >= 1073741824)
			{
				tmpnr = oftoa64((double)node->filelist->size / 1073741824, "2");
				tmpnr = ostrcat(tmpnr, "G", 1, 0);
			}
			else if(node->filelist->size >= 1048576)
			{
				tmpnr = oftoa64((double)node->filelist->size / 1048576, "2");
				tmpnr = ostrcat(tmpnr, "M", 1, 0);
			}
			else if(node->filelist->size >= 1024)
			{
				tmpnr = oftoa64((double)node->filelist->size / 1024, "2");
				tmpnr = ostrcat(tmpnr, "K", 1, 0);
			}
			else
			{
				tmpnr = oitoa64(node->filelist->size);
				tmpnr = ostrcat(tmpnr, "B", 1, 0);
			}
		}
		if(node->filelist->view == 5)
		{
			tmpnr = malloc(MINMALLOC);
			if(tmpnr == NULL)
			{
				err("no mem");
				return;
			}

			struct tm* loctime = olocaltime(&node->filelist->date);
			if(loctime != NULL)
				strftime(tmpnr, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
			free(loctime);
		}
		drawstring(tmpnr, 1, node->poscount, -1, node->iposx, node->iposy, node->iwidth, node->iheight, RIGHT, node->valign, node->font, node->fontsize, node->fontcol, node->transparent, 0, NULL, NULL, NULL, node->charspace);
		free(tmpnr);
	}
	if(node->title != NULL)
		drawtitle(node);
	if(node->scrollbar == YES || node->scrollbar == AUTOYES)
		drawscrollbar(node);
	if(node->bordersize > 0)
	{
		if(node->child != NULL && status.picbordersize > 0)
			drawpicborder(node);
		else
			drawborder(node);
		if((node->child != NULL && status.borderradius > 0) || node->borderradius > 0)
			drawroundborder(node, bglt, bglb, bgrt, bgrb);
	}

	free(bglt); free(bglb); free(bgrt); free(bgrb);
	debug(1000, "out");
}

void calcscrollbar(struct skin* node)
{
	debug(1000, "in");

	node->scrollbarheight = node->iheight;

	if(node->pagecount > 1)
	{
		node->scrollbarheight = (float)node->iheight / node->pagecount;
		node->scrollbarpos = ((float)node->iheight / node->pagecount) * (node->aktpage - 1);
		if(node->scrollbar == AUTONO) node->scrollbar = AUTOYES;
	}
	else if(node->scrollbar == AUTOYES)
		node->scrollbar = AUTONO;

	if(node->scrollbar == YES || node->scrollbar == AUTOYES)
	{
		node->iwidth -= (SCROLLBARWIDTH + 5);
		node->scrollbarwidth = SCROLLBARWIDTH;
		node->scrollbarbordersize = SCROLLBARBORDERSIZE;
		if(node->iposy + node->scrollbarpos > node->iposy + node->iheight) node->scrollbarpos = node->iposy + node->iheight;
		if(node->scrollbarpos < 0) node->scrollbarpos = 0;
		if(node->iposy + node->scrollbarpos + node->scrollbarheight > node->iposy + node->iheight) node->scrollbarheight = node->iheight - node->scrollbarpos;
		if(node->scrollbarheight < 1) node->scrollbarheight = 1;
	}
	debug(1000, "out");
}

void calclistboxchild(struct skin* node, struct skin* parent)
{
	if((parent->type & GRID) && (node->type & GRIDBR))
	{
		if(parent->poscount > 0) parent->poscount += node->rheight;
		if(parent->poscount == 0) parent->poscount = 1;
	}
	node->rposy = node->rposy + parent->poscount;
	node->iposy = node->iposy + parent->poscount;
	if((parent->type & LISTBOX) || ((parent->type & FILELIST) && !(parent->type & GRID)))
		parent->poscount += node->rheight;
}

int calclistbox(struct skin* node)
{
	debug(1000, "in");
	struct skin* child = NULL, *last = NULL, *found = NULL;
	int selcol = convertcol("listboxselect");
	int markcol = convertcol("markcol");
	char* selectpic = getskinconfig("selectpic", NULL);

	node->poscount = 0;
	if(node->aktline == 0) node->aktline = 1;
	node->pagecount = 1;
	node->linecount = 0;

	child = node->next;
	while(child != NULL)
	{
		if(child->parentpointer == NULL)
		{
			if(child->parent == NULL)
			{
				child = child->next;
				continue;
			}
			else if(ostrcmp(child->parent, node->name) != 0 || child->hidden == YES)
			{
				child = child->next;
				continue;
			}
		}
		else if(child->parentpointer != node || child->hidden == YES)
		{
			child = child->next;
			continue;
		}

		if(child->locked == YES)
		{
			child = child->next;
			continue;
		}		

		calcrheight(child, node);

		if((node->type & LISTBOX) || ((node->type & FILELIST) && !(node->type & GRID)) || ((node->type & GRID) && (child->type & GRIDBR)))
			node->poscount = node->poscount + child->posy + child->rheight;

		if(node->poscount > node->iheight)
		{
			node->pagecount++;
			node->poscount = child->rheight;
		}

		child->pagecount = node->pagecount;

		if(child->deaktivcol > -1)
		{
			child = child->next;
			continue;
		}

		node->linecount++;
		last = child;

		if(node->aktline == -1 && child->pagecount == node->aktpage)
			node->aktline = node->linecount;

		if(node->aktline == node->linecount)
			found = child;

		child->bordersize = 0;
		if(status.listboxselecttype == 3)
		{
			changeselectpic(child, NULL);
			if(child->bgcol == markcol)
				child->bgcol = child->bordercol;
		}
		if(child->bgcol == selcol || child->bgcol == markcol) //&& status.listboxselecttype == 1)
			child->bgcol = child->bordercol;
		if(child->fontcol == selcol || child->fontcol == markcol) //&& status.listboxselecttype == 2)
			child->fontcol = child->bordercol;

		child = child->next;
	}

	if(found == NULL)
	{
		found = last;
		node->aktline = node->linecount;
	}
	
	if(found != NULL)
	{
		if(node->aktline == -2) node->aktline = node->linecount;
		if(status.listboxselecttype == 0)
		{
			found->bordersize = 1;
			if(status.markmodus > 0)
			{
				found->bordercol = markcol;
				int i = 0;
				struct skin* tmpskin = found;
				for(i = 0; i < status.moveblockcount; i++)
				{
					tmpskin = tmpskin->next;
					if(tmpskin != NULL)
						tmpskin->bordercol = markcol;
					else
						break;
				}
			}
			else
				found->bordercol = selcol;
		}
		else if(status.listboxselecttype == 1)
		{
			if(found->bgcol != selcol && found->bgcol != markcol)
			{
				found->bordercol = found->bgcol;
				int i = 0;
				struct skin* tmpskin = found;
				for(i = 0; i < status.moveblockcount; i++)
				{
					tmpskin = tmpskin->next;
					if(tmpskin != NULL)
						tmpskin->bordercol = found->bgcol;
					else
						break;
				}
			}
			if(status.markmodus > 0)
			{
				found->bgcol = markcol;
				int i = 0;
				struct skin* tmpskin = found;
				for(i = 0; i < status.moveblockcount; i++)
				{
					tmpskin = tmpskin->next;
					if(tmpskin != NULL)
						tmpskin->bgcol = markcol;
					else
						break;
				}
			}
			else
				found->bgcol = selcol;
		}
		else if(status.listboxselecttype == 2)
		{
			if(found->fontcol != selcol && found->fontcol != markcol)
			{
				found->bordercol = found->fontcol;
				int i = 0;
				struct skin* tmpskin = found;
				for(i = 0; i < status.moveblockcount; i++)
				{
					tmpskin = tmpskin->next;
					if(tmpskin != NULL)
						tmpskin->bordercol = found->fontcol;
					else
						break;
				}
			}
			if(status.markmodus > 0)
			{
				found->fontcol = markcol;
				int i = 0;
				struct skin* tmpskin = found;
				for(i = 0; i < status.moveblockcount; i++)
				{
					tmpskin = tmpskin->next;
					if(tmpskin != NULL)
						tmpskin->fontcol = markcol;
					else
						break;
				}
			}
			else
				found->fontcol = selcol;
		}
		else if(status.listboxselecttype == 3)
		{
			if(status.markmodus > 0)
			{
				found->bordersize = 1;
				found->bordercol = markcol;
				int i = 0;
				struct skin* tmpskin = found;
				for(i = 0; i < status.moveblockcount; i++)
				{
					tmpskin = tmpskin->next;
					if(tmpskin != NULL)
					{
						tmpskin->bordersize = 1;
						tmpskin->bordercol = markcol;
					}
					else
						break;
				}
			}
			else
				changeselectpic(found, selectpic);
		}

		if(node->aktpage == -1)
			node->aktpage = found->pagecount;
		node->select = found;
	}

	if(node->aktpage == -1) node->aktpage = 0;
	if(node->aktpage > node->pagecount)
	{
		if(node->pagecount == 0) node->aktpage = 0;
		else node->aktpage = 1;
	}
	node->poscount = 0;
	debug(1000, "out");
	return 0;
}

int calcrwidth(struct skin* node, struct skin* parent)
{
	int scrollbarwidth = 0;

	if(node->prozwidth == 1)
		node->rwidth = ((float)parent->iwidth / 100) * node->width;
	else
		node->rwidth = node->width;

	if(node->scrollbar == YES || node->scrollbar == AUTOYES || node->scrollbar == AUTONO) scrollbarwidth = SCROLLBARWIDTH;

	if(node->rwidth < (node->bordersize * 2) + scrollbarwidth) node->rwidth = (node->bordersize * 2) + scrollbarwidth;

	return 0;
}

int calcrheight(struct skin* node, struct skin* parent)
{
	if(node->prozheight == 1)
		node->rheight = ((float)parent->iheight / 100) * node->height;
	else
	{
		node->rheight = node->height;
		if(node->fontsize == 0) node->fontsize = parent->fontsize;
		if(node->fontsize == 0) node->fontsize = 1;
		if(node->rheight == 0) node->rheight = node->fontsize + 2;
	}

	if(node->rheight < (node->bordersize * 2) + node->titlesize) node->rheight = (node->bordersize * 2) + node->titlesize;

	return 0;
}

int calcrposx(struct skin* node, struct skin* parent)
{
	if(node->prozposx == 1)
		node->rposx = ((float)parent->iwidth / 100) * node->posx;
	else
		node->rposx = node->posx;

	if(node->posx == CENTER || (node->posx == 0 && parent->halign == CENTER))
		node->rposx = parent->iposx + parent->iwidth / 2 - node->rwidth / 2;
	else if(node->posx == LEFT)
		node->rposx = parent->iposx;
	else if(node->posx == RIGHT || (node->posx == 0 && parent->halign == RIGHT))
		node->rposx = parent->iposx + parent->iwidth - node->rwidth;
	else
		node->rposx = parent->iposx + node->rposx;

	if(node->rposx > parent->iposx + parent->iwidth)
		node->rposx = parent->iposx + parent->iwidth;

	return 0;
}

int calcrposy(struct skin* node, struct skin* parent)
{
	if(node->prozposy == 1)
		node->rposy = ((float)parent->iheight / 100) * node->posy;
	else
		node->rposy = node->posy;

	if(node->posy == MIDDLE || (node->posy == 0 && parent->valign == MIDDLE))
		node->rposy = parent->iposy + parent->iheight / 2 - node->rheight / 2;
	else if(node->posy == TOP)
		node->rposy = parent->iposy;
	else if(node->posy == BOTTOM || (node->posy ==0 && parent->valign == BOTTOM))
		node->rposy = parent->iposy + parent->iheight - node->rheight;
	else
		node->rposy = parent->iposy + node->rposy;

	if(node->rposy > parent->iposy + parent->iheight)
		node->rposy = parent->iposy + parent->iheight;

	return 0;
}

int setnodeattr(struct skin* node, struct skin* parent, int screencalc)
{
	if(node != skin) node->flag = clearbit(node->flag, 0);
	if((parent->type & LISTBOX) || (parent->type & FILELIST) || (parent->type & GRID))
		if(node->pagecount != parent->aktpage) return 1;

	debug(1000, "in");
	int shadowlx = 0, shadowrx = 0, shadowoy = 0, shadowuy = 0;
	unsigned int linecount = 0, pagecount = 0, poscount = 0;
	char* tmpstr = NULL;

	if(node->child != NULL && status.picbordersize > 0)
		node->bordersize = status.picbordersize;

	if(node->skinfunc != NULL)
	{
		if(node->funcrettype == FUNCPIC)
		{
			tmpstr = node->skinfunc(node, node->param1, node->param2);
			changepic(node, tmpstr);
		}
		else if(node->funcrettype == FUNCPROGRESS)
		{
			tmpstr = node->skinfunc(node, node->param1, node->param2);
			if(tmpstr != NULL)
			{
				node->hidden = NO;
				node->progresssize = atoi(tmpstr);
			}
			else
				node->hidden = YES;
		}
		else
		{
			tmpstr = node->skinfunc(node, node->param1, node->param2);
			changetext(node, _(tmpstr));
		}
		free(tmpstr);
	}

	if(screencalc != 2)
	{
		if(node->hidden == YES || parent->hidden == YES || node->locked == YES || parent->locked == YES) return 1;
		if(checkbit(parent->flag, 0) == 0) return 1;
	}

	calcrwidth(node, parent);
	if(!(parent->type & LISTBOX) && !(parent->type & FILELIST) && !(parent->type & GRID))
		calcrheight(node, parent);
	calcrposx(node, parent);
	calcrposy(node, parent);

	if(node->height < 0)
		node->rheight = parent->iheight + node->height - (node->rposy - parent->iposy);
	if(node->width < 0)
		node->rwidth = parent->iwidth + node->width - (node->rposx - parent->iposx);

	node->iposx = node->rposx + node->bordersize + node->hspace;
	node->iposy = node->rposy + node->bordersize + node->titlesize + node->vspace;
	node->iwidth = node->rwidth - node->bordersize * 2 - node->hspace * 2;
	node->iheight = node->rheight - node->bordersize * 2 - node->titlesize - node->vspace * 2;

	switch(node->shadowpos)
	{
		case BOTTOMLEFT: shadowlx = shadowuy = node->shadowsize; break;
		case BOTTOMRIGHT: shadowrx = shadowuy = node->shadowsize; break;
		case TOPLEFT: shadowlx = shadowoy = node->shadowsize; break;
		default: shadowrx = shadowoy = node->shadowsize; break;
	}

	if((parent->type & LISTBOX) || (parent->type & FILELIST) || (parent->type & GRID))
		calclistboxchild(node, parent);

	if(node->picprozwidth == 1)
		node->rpicwidth = ((float)node->iwidth / 100) * node->picwidth;
	else
		node->rpicwidth = node->picwidth;
	if(node->picprozheight == 1)
		node->rpicheight = ((float)node->iheight / 100) * node->picheight;
	else
		node->rpicheight = node->picheight;


	if(node->rposx - shadowlx < parent->iposx)
	{
		if(screencalc == 0) err("node (%s posx=%d) out of parent (%s posx=%d)", node->name, node->rposx - shadowlx, parent->name, parent->iposx);
		node->rposx = parent->iposx + shadowlx;
		//return 1;
	}
	if(node->rposy - shadowoy < parent->iposy)
	{
		if(screencalc == 0) err("node (%s posy=%d) out of parent (%s posy=%d)", node->name, node->rposy - shadowoy, parent->name, parent->iposy);
		node->rposy = parent->iposy + shadowoy;
		//return 1;
	}
	if(node->rposx + node->rwidth + shadowrx > parent->iposx + parent->iwidth)
	{
		if(screencalc == 0) err("node (%s posxx=%d) out of parent (%s posxx=%d)", node->name, node->rposx + node->rwidth + shadowrx, parent->name, parent->iposx + parent->iwidth);
		node->rwidth = parent->iwidth - node->rposx - shadowrx;
		//return 1;
	}
	if(node->rposy + node->rheight + shadowuy > parent->iposy + parent->iheight)
	{
		if(screencalc == 0) err("node (%s posyy=%d) out of parent (%s posyy=%d)", node->name, node->rposy + node->rheight + shadowuy, parent->name, parent->iposy + parent->iheight);
		node->rheight = parent->iheight - node->rposy - shadowuy;
		//return 1;
	}

	if(node->font == NULL && parent->font != NULL)
	{
		changefont(node, parent->font);
		if(node->font == NULL)
		{
			err("no memory");
		}
	}

	if(node->fontsize == 0) node->fontsize = parent->fontsize;
	if(node->fontsize == 0) node->fontsize = 1;
	if(node->fontsize2 == 0) node->fontsize2 = parent->fontsize2;
	if(node->fontsize2 == 0) node->fontsize2 = 1;
	if(node->fontcol == 0) node->fontcol = parent->fontcol;
	if(node->fontcol2 == 0) node->fontcol2 = parent->fontcol2;

	if((node->type & INPUTBOX) || (node->type & INPUTBOXNUM))
	{
		if(node->aktpage < 1) node->aktpage = 1;
		checkinputboxnumright(node);
		changeret(node, node->input);
	}

	if((node->type & LISTBOX) || (node->type & FILELIST) || (node->type & GRID))
	{
		if(node->aktpage == 0) node->aktpage = 1;
		calclistbox(node);
		if(node->scrollbar != NO)
			calcscrollbar(node);
	}
	if(node->type & TEXTBOX)
	{
		if(node->aktpage < 1) node->aktpage = 1;
		if(node->text == NULL && node->text2 == NULL)
			node->pagecount = 0;
		else
		{
			if(node->wrap == YES)
				wrapstr(node->text, node->font, node->fontsize, node->iwidth, node->charspace);
			calctext(node->text, node->text2, &node->linecount, &node->pagecount, &node->poscount, node->iheight / node->fontsize, node->aktpage);
		}
		if(node->text2 != NULL) node->scrollbar = NO;
		if(node->scrollbar != NO)
			calcscrollbar(node);
	}

	if((node->type & CHOICEBOX) && node->input != NULL)
	{
		char* pos = NULL;
		calctext(node->input, NULL, &node->linecount, &node->pagecount, &node->poscount, 1, node->aktpage);

		free(node->ret);
		node->ret = NULL;

		if(node->choiceboxvalue != NULL)
		{
			calctext(node->choiceboxvalue, NULL, &linecount, &pagecount, &poscount, 1, node->aktpage);
			pos = strchr(node->choiceboxvalue + poscount, '\n');
			if(pos == NULL)
				changeret(node, node->choiceboxvalue + poscount);
			else
				node->ret = strndup(node->choiceboxvalue + poscount, pos - (node->choiceboxvalue + poscount));
		}
		else if(node->input != NULL)
		{
			pos = strchr(node->input + node->poscount, '\n');
			if(pos == NULL)
				changeret(node, node->input + node->poscount);
			else
				node->ret = strndup(node->input + node->poscount, pos - (node->input + node->poscount));
		}
	}

	//set parent transparent to child
	//if(node->transparent == 0 && parent != skin)
		//node->transparent = parent->transparent;

	debug(1000, "out");
	return 0;
}

int clearscreenalways()
{
	debug(1000, "in");
	int i, ret = 0;

	for(i = 0; i < sizeof(status.drawallways) / sizeof(skin); i++)
	{
		if(status.drawallways[i] != NULL)
		{
			clearrect(status.drawallways[i]->rposx, status.drawallways[i]->rposy, status.drawallways[i]->rwidth, status.drawallways[i]->rheight);
		}
	}

	debug(1000, "out");
	return ret;
}

int drawscreenalways(struct skin* node, int screencalc)
{
	debug(1000, "in");
	int i, ret = 0;

	for(i = 0; i < sizeof(status.drawallways) / sizeof(skin); i++)
	{
		if(status.drawallways[i] != NULL)
		{
			if(node != status.drawallways[i])
			{
				if(status.drawallwaysbg[i] != NULL)
					free(status.drawallwaysbg[i]);
				status.drawallwaysbg[i] = savescreen(status.drawallways[i]);
				ret = drawscreen(status.drawallways[i], screencalc, 1);
			}
		}
	}

	debug(1000, "out");
	return ret;
}

int drawscreennode(struct skin *node, char* nodename, int screencalc)
{
	debug(1000, "in");

	node = getscreennode(node, nodename);
	m_lock(&status.drawingmutex, 0);
	if(node != status.skinerr)
		drawnode(node, 1);

	drawscreenalways(node, screencalc);
	blitfb(0);
	m_unlock(&status.drawingmutex, 0);

	debug(1000, "out");
	return 0;
}

int drawscreennodebyname(char* screenname, char* nodename, int screencalc)
{
	debug(1000, "in");
	struct skin *node;

	node = getscreennodebyname(screenname, nodename);
	m_lock(&status.drawingmutex, 0);
	if(node != status.skinerr)
		drawnode(node, 1);

	drawscreenalways(node, screencalc);
	blitfb(0);
	m_unlock(&status.drawingmutex, 0);

	debug(1000, "out");
	return 0;
}

//flag 0: draw normal with allways
//flag 1: draw without allways
//flag 2: from thread (mutex is set in thread)
//flag 3: unused
//flag 4: same as 0 but animate
//screencalc 0: calculate and draw
//screencalc 1: only calculate without hidden nodes
//screencalc 2: only calculate hidden nodes
int drawscreen(struct skin* node, int screencalc, int flag)
{
	struct fb* merkskinfb = NULL;

	debug(1000, "in");
	int ret;
	struct skin *child = NULL, *parent = NULL, *oldparent = NULL;

	if(node == NULL)
	{
		err("node = NULL");
		return 1;
	}

	if(flag == 0 || flag == 4)
		m_lock(&status.drawingmutex, 0);

	parent = skin;

	ret = setnodeattr(node, parent, screencalc);
	if(ret == 1)
	{
		if(flag == 0 || flag == 4)
			m_unlock(&status.drawingmutex, 0);
		debug(1000, "out -> setnodeattr ret = 1");
		return 1;
	}
	
	if(node->name != NULL && ostrstr(node->name, "LCD_") != NULL)
	{
		merkskinfb = skinfb;
		memset(lcdskinfb->fb, 0, lcdskinfb->varfbsize);
		skinfb = lcdskinfb;
	}

	if(screencalc == 0 || flag == 4)
	{
		if(flag == 0 || flag == 2 || flag == 4) clearscreenalways();
		drawnode(node, 0);
	}
	parent = node;
	oldparent = node;
	child = node->child;

	while(child != NULL)
	{
		if(child->parentpointer != NULL)
			parent = child->parentpointer;
		else if(child->parent != NULL)
		{
			parent = getscreennode(node, child->parent);
			if(parent == status.skinerr) parent = oldparent;
		}
		else
			parent = oldparent;

		if(setnodeattr(child, parent, screencalc) == 0 && screencalc == 0)
			drawnode(child, 1);
		child = child->next;
	}

	if(flag == 0 || flag == 2 || flag == 4)
	{
		if(screencalc == 0)
		{
			drawscreenalways(node, screencalc);

			if(merkskinfb != NULL) 
				pngforlcd();
			else	
			{
				if(flag == 4)
					blitfb(1);
				else
					blitfb(0);
			}
		}
	}

	if(merkskinfb != NULL)
	{
		skinfb = merkskinfb;
		merkskinfb = NULL;
	}
	else
	{
		if(ostrcmp(getconfig("write_fb_to_jpeg", NULL), "yes") == 0)
			write_FB_to_JPEG_file(skinfb->fb, skinfb->width, skinfb->height, "/tmp/fb.jpg", 3);
	}
	if(flag == 0 || flag == 4)
		m_unlock(&status.drawingmutex, 0);
	debug(1000, "out");
	return 0;
}

int drawscreenbyname(char* screenname, int screencalc, int flag)
{
	debug(1000, "in");
	int ret;
	struct skin* node = NULL;

	node = getscreen(screenname);
	if(node == status.skinerr)
	{
		err("screen not found (%s)", screenname);
		return 1;
	}

	ret = drawscreen(node, screencalc, flag);
	debug(1000, "out");
	return ret;
}

int changeinput(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->input);
		if(text != NULL)
		{
			if((node->type & INPUTBOXNUM) && node->mask != NULL && strlen(text) == 0)
				node->input = strdup(node->mask);
			else
				node->input = strdup(text);
		}
		else
		{
			if((node->type & INPUTBOXNUM) && node->mask != NULL)
				node->input = strdup(node->mask);
			else
				node->input = text;
		}
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changetext(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->text);
		if(text != NULL)
			node->text = strdup(text);
		else
			node->text = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changetext2(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->text2);
		if(text != NULL)
			node->text2 = strdup(text);
		else
			node->text2 = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}


int changename(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->name);
		if(text != NULL)
			node->name = strdup(text);
		else
			node->name = strdup("unknown");
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changepicmem(struct skin* node, char* text, int timeout, int del)
{
	debug(1000, "in");
	unsigned long width = 0, height = 0, rowbytes = 0;
	int ret = 1, channels = 0, memfd = -1, length = 0;
	unsigned char* buf = NULL;

	if(node != NULL)
	{
		free(node->pic);
		if(text != NULL)
		{
			node->pic = changepicpath(text);

			if(getpic(node->pic) == NULL)
			{
				length = strlen(node->pic);
				if(node->pic[length - 1] == 'g' && node->pic[length - 2] == 'n' && node->pic[length - 3] == 'p')
					buf = readpng(node->pic, &width, &height, &rowbytes, &channels, 0, 0, 0, 0, 0, 0);
				else if(getconfigint("pichwdecode", NULL) == 1)
					readjpg(node->pic, &width, &height, &rowbytes, &channels, &buf, &memfd);
				else
					buf = loadjpg(node->pic, &width, &height, &rowbytes, &channels, 1);
				addpic(node->pic, buf, memfd, width, height, rowbytes, channels, timeout, del, NULL);
			}
		}
		else
			node->pic = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changepic(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->pic);
		if(text != NULL)
			node->pic = changepicpath(text);
		else
			node->pic = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changeselectpic(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->selectpic);
		if(text != NULL)
			node->selectpic = changepicpath(text);
		else
			node->selectpic = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changetitle(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->title);
		if(text != NULL)
		{
			node->title = strdup(text);
			node->titlesize = node->fontsize + 6;
		}
		else
		{
			node->title = text;
			node->titlesize = 0;
		}
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changemask(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->mask);
		if(text != NULL)
		{
			if((node->type & INPUTBOXNUM) && (node->input == NULL || strlen(node->input) == 0))
			{
				node->mask = strdup(text);
				free(node->input);
				node->input = strdup(text);
			}
			else
				node->mask = strdup(text);
		}
		else
			node->mask = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changeret(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->ret);
		if(text != NULL)
			node->ret = strdup(text);
		else
			node->ret = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changeparent(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->parent);
		if(text != NULL)
			node->parent = strdup(text);
		else
			node->parent = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changefont(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->font);
		if(text != NULL)
			node->font = strdup(text);
		else
			node->font = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changeparam1(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->param1);
		if(text != NULL)
			node->param1 = strdup(text);
		else
			node->param1 = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changeparam2(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->param2);
		if(text != NULL)
			node->param2 = strdup(text);
		else
			node->param2 = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

int changechoiceboxvalue(struct skin* node, char* text)
{
	debug(1000, "in");
	int ret = 1;

	if(node != NULL)
	{
		free(node->choiceboxvalue);
		if(text != NULL)
			node->choiceboxvalue = strdup(text);
		else
			node->choiceboxvalue = text;
		ret = 0;
	}
	debug(1000, "out");

	return ret;
}

#endif
