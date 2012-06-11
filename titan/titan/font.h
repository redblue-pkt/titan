#ifndef FONT_H
#define FONT_H

FT_Library library;

FT_Error MyFaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face *aface)
{
	debug(1000, "in");
	FT_Error ret;

	ret = FT_New_Face(library, (char*)face_id, 0, aface);
	if(ret != 0)
		err("Font %s failed", (char*)face_id);

	FT_Select_Charmap(*aface, ft_encoding_unicode);
	debug(1000, "out");
	return ret;
}

struct font* addfont(char *fontname)
{
	debug(1000, "in");
	struct font *newnode = NULL, *node = font;
	char *name = NULL;

	name = ostrcat(fontname, NULL, 0, 0);
	if(name == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode = (struct font*)malloc(sizeof(struct font));	
	if(newnode == NULL)
	{
		err("no memory");
		free(name);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct font));
	newnode->name = name;

	if(node != NULL)
	{
		while(node->next != NULL)
			node = node->next;
		node->next = newnode;
	}
	else
		font = newnode;

	debug(1000, "out");
	return newnode;
}

void delfont(char *name)
{
	debug(1000, "in");
	struct font *node = font, *prev = font;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			if(node == font)
				font = node->next;
			else
				prev->next = node->next;

			if(node->name != NULL)
			{
				free(node->name);
				node->name = NULL;
			}
			if(node->manager != NULL) FTC_Manager_Done(node->manager);

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

int initfont()
{
	debug(1000, "in");
	int ret;

	ret = FT_Init_FreeType(&library);
	if(ret != 0)
	{
		err("FT_Init_FreeType failed. Error: 0x%.2X", ret);
		return 1;
	}

	debug(1000, "out");
	return 0;
}

void deinitfont()
{
	debug(1000, "in");
	FT_Done_FreeType(library);
	debug(1000, "out");
}

int openfont(char *filename)
{
	debug(1000, "in");
	int ret;
	struct font* node = font;

	if(filename == NULL)
	{
		debug(100, "font not defined");
		return 1;
	}

	node = addfont(filename);
	if(node == NULL)
		return 1;

	ret = FTC_Manager_New(library, 1, 2, 0, &MyFaceRequester, NULL, &node->manager);
	if(ret != 0)
	{
		err("FTC_Manager_New failed. Error: 0x%.2X", ret);
		delfont(filename);
		return 1;
	}

	ret = FTC_SBitCache_New(node->manager, &node->cache);
	if(ret != 0)
	{
		err("FTC_SBitCache_New failed. Error: 0x%.2X", ret);
		delfont(filename);
		return 1;
	}

	ret = FTC_Manager_LookupFace(node->manager, (FTC_FaceID)filename, &node->face);
	if(ret != 0)
	{
		err("FTC_Manager_LookupFace failed. Error: 0x%.2X", ret);
		delfont(filename);
		return 1;
	}

	node->desc.face_id = (FTC_FaceID)filename;
	node->use_kerning = FT_HAS_KERNING(node->face);

	debug(1000, "out");
	return 0;
}

struct font* getfont(char *name)
{
	debug(1000, "in");
	struct font *node = font;

	while(node != NULL)
	{
		if(ostrstr(node->name, name) != NULL)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "font not found (%s)", name);
	return NULL;
}

void freefont()
{
	debug(1000, "in");
	struct font *node = font, *prev = font;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delfont(prev->name);
	}
	debug(1000, "out");
}

#endif
