#ifndef STYLE_H
#define STYLE_H

struct style* addstyle(char* line, struct style* last)
{
	char *ret = NULL;
	struct style *newnode = NULL, *prev = NULL, *node = NULL;
	int memfd = -1, length;
	int fontsizeadjust = getskinconfigint("fontsizeadjust", NULL);

	newnode = (struct style*)calloc(1, sizeof(struct style));
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
			newnode->name = ostrcat("unknown", NULL, 0, 0);
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
		ret = getxmlentry(line, " picquality=");
		if(ret != NULL)
		{
			newnode->picquality = atoi(ret);
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
		ret = getxmlentry(line, " titlealign=");
		if(ret != NULL)
		{
			newnode->titlealign = convertxmlentry(ret, NULL);
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

			newnode->picmem = 1;
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
			newnode->mask = ret;
	}
	else
		newnode->name = ostrcat("unknown", NULL, 0, 0);
		
	if(last == NULL)
	{
		while(node != NULL)
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
		style = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	return newnode;
}

void addstyletoscreennode(struct skin* node, struct style* snode)
{
	if(node == NULL || snode == NULL) return;

	node->posx = snode->posx;
	node->posy = snode->posy;
	node->width = snode->width;
	node->height = snode->height;
	node->picwidth = snode->picwidth;
	node->picheight = snode->picheight;
	node->picquality = snode->picquality;
	node->textposx = snode->textposx;
	node->textposx2 = snode->textposx2;
	node->halign = snode->halign;
	node->valign = snode->valign;
	node->hidden = snode->hidden;
	node->wrap = snode->wrap;
	node->hspace = snode->hspace;
	node->vspace = snode->vspace;
	node->bgspace = snode->bgspace;
	node->zorder = snode->zorder;
	node->scrollbar = snode->scrollbar;
	node->bordersize = snode->bordersize;
	node->bordertype = snode->bordertype;
	node->bordercol = snode->bordercol;
	node->deaktivcol = snode->deaktivcol;
	node->progresscol = snode->progresscol;
	node->shadowsize = snode->shadowsize;
	node->shadowcol = snode->shadowcol;
	node->shadowpos = snode->shadowpos;
	node->fontsize = snode->fontsize;
	node->fontsize2 = snode->fontsize2;
	node->fontcol = snode->fontcol;
	node->fontcol2 = snode->fontcol2;
	node->charspace = snode->charspace;
	node->borderradius = snode->borderradius;
	node->transparent = snode->transparent;
	node->titlealign = snode->titlealign;
	node->titlebgcol = snode->titlebgcol;
	node->titlebgcol2 = snode->titlebgcol2;
	node->bgcol = snode->bgcol;
	node->bgcol2 = snode->bgcol2;
	node->gradient = snode->gradient;
	node->titlegradient = snode->titlegradient;
	node->skinfunc = snode->skinfunc;
	node->prozposx = snode->prozposx;
	node->prozposy = snode->prozposy;
	node->prozwidth = snode->prozwidth;
	node->prozheight = snode->prozheight;
	node->picprozwidth = snode->picprozwidth;
	node->picprozheight = snode->picprozheight;
	node->funcrettype = snode->funcrettype;
	node->picmem = snode->picmem;

	changemask(node, snode->mask);

	node->param1 = ostrcat(snode->param1, NULL, 0, 0);
	node->param2 = ostrcat(snode->param2, NULL, 0, 0);
	node->input = ostrcat(snode->input, NULL, 0, 0);
	node->pic = ostrcat(snode->pic, NULL, 0, 0);
	node->font = ostrcat(snode->font, NULL, 0, 0);
}

struct style* getstyle(char* name)
{
	struct style *node = style;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
			break;
		node = node->next;
	}
	
	return node;
}

void delstyle(struct style* snode)
{
	struct style *node = style, *prev = style;

	while(node != NULL)
	{
		if(node == snode)
		{
			if(node == style)
				style = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freestyle()
{
	struct style *node = style, *prev = style;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delstyle(prev);
	}
}

/*
add code to skin.h
add read to titan.c
add free to titan.c
*/

#endif
