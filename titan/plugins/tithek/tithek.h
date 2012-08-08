#ifndef TITHEK_H
#define TITHEK_H

#define TITHEKPATH "/tmp/tithek"

#include <assert.h>

signed char htod(char c)
{
  c = tolower(c);
  if(isdigit(c))
    return c - '0';

  if(c >= 'a' && c <= 'f')
    return c - 'a';

  return -1;
}

char* unhexlify1(const char *hexstr)
{
	int len = 0;
	char *p, *q, *binstr = NULL;

	if(hexstr == NULL) return NULL;

	len = strlen(hexstr);
	if(len == 0 || len % 2 != 0) return NULL;

	binstr = calloc(1, (len / 2) + 1);
	if(binstr == NULL)
	{
		err("no mem");
		return NULL;
	}

	for(p = hexstr, q = binstr; *p; p += 2, q++)
		sscanf(p, "%2x", (unsigned int*)q);

	return binstr;
}

struct rc4ctx {
    unsigned char S[256];
    unsigned char i;
    unsigned char j;
};


void rc4init(struct rc4ctx *ctx, unsigned char *key, size_t keylen);
void rc4crypt(struct rc4ctx *ctx, unsigned char *data, size_t len);
void rc4(unsigned char *data, size_t dlen, unsigned char *key, size_t klen);


#define SWAP(a,b) { unsigned char temp; temp = (a); (a) = (b); (b) = temp; }

void rc4init(struct rc4ctx *ctx, unsigned char *key, size_t keylen)
{
    int i;
    unsigned char j = 0;

    for (i = 0; i < sizeof(ctx->S); i++) {
	ctx->S[i] = i;
    }
    
    for (i = 0; i < sizeof(ctx->S); i++) {
	j +=  (ctx->S[i] + key[i % keylen]);
	SWAP(ctx->S[i], ctx->S[j]);
    }
}


void rc4crypt(struct rc4ctx *ctx, unsigned char *data, size_t len)
{
    unsigned int i;

    ctx->i = 0;
    ctx->j = 0;

    for (i = 0; i < len; i++) {
	unsigned char s;

	ctx->i++;
	ctx->j += ctx->S[ctx->i];

	SWAP(ctx->S[ctx->i], ctx->S[ctx->j]);

	s = ctx->S[ctx->i] + ctx->S[ctx->j];
	data[i] = data[i] ^ ctx->S[s];
    }
}


void rc4(unsigned char *data, size_t dlen, unsigned char *key, size_t klen)
{
    struct rc4ctx ctx;
    rc4init(&ctx, key, klen);
    rc4crypt(&ctx, data, dlen);
}

/*
static byte[] rc4crypt(byte[] data,byte[] key)
{
	byte[] ret = new byte[data.Length];
	byte[] box = new byte[256];
	int x = 0, y = 0, i = 0;
	byte temp = 0;

	for (x = 0; x < 256; x++)
	{
		box[x] = (byte)x;
	}
    
	x=0;
	for (i = 0; i < 256; i++)
	{
		x = (x + box[i] + key[i % key.Length]) % 256;
		temp = box[i];
		box[i] = box[x];
		box[x] = temp;
	}

	x = y = 0;

	for(i=0;i<data.Length;i++)
	{
		x = (x + 1) % 256;
		y = (y + box[x]) % 256;
		temp = box[y];
		box[y] = box[x];
		box[x] = temp;
		ret[i] = (byte)(data[i] ^ box[(box[x] + box[y]) % 256]);
	}

	return ret;
}
*/
/*
void rc4(unsigned char* ByteInput, unsigned char* pwd, unsigned char* ByteOutput)
{
	unsigned char * temp;
	int i,j=0,t,tmp,tmp2,s[256], k[256];
	for (tmp=0;tmp<256;tmp++)
	{
		s[tmp]=tmp;
		k[tmp]=pwd[(tmp % strlen((char *)pwd))];
	}
	for (i=0;i<256;i++)
	{
		j = (j + s[i] + k[i]) % 256;
		tmp=s[i];
		s[i]=s[j];
		s[j]=tmp;
	}
	
	temp = unsigned char [ (int)strlen((char *)ByteInput) + 1 ] ;
	i=j=0;
	for (tmp=0;tmp<(int)strlen((char *)ByteInput);tmp++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp2=s[i];
		s[i]=s[j];
		s[j]=tmp2;
		t = (s[i] + s[j]) % 256;
		if (s[t]==ByteInput[tmp])
			temp[tmp]=ByteInput[tmp];
		else
			temp[tmp]=s[t]^ByteInput[tmp];
	}
	temp[tmp]='';
	ByteOutput=temp;
}
*/
       
char fromhex(char c);

char *hexlify(char *str)
{
	int l,i; char *t;
	l = strlen(str)*2;
	t = malloc(l);
	if ( t )
	{
		for(i=0; i<l; i++)
		{
			sprintf(t+2*i, "%02x", str[i]);
		}
		return t;
	}
	return NULL;
}

//it will be up to the caller to free the memory...

char* unhexlify(char* hstr)
{
	int count = 0;
	int l, i; 
	char* t;
	char c;
	l = strlen(hstr)/2;
	t = malloc(l);
	if(t)
	{
		for(i=0; i<l; i++)
		{
			count += 1;
			printf("unhexlify (%d)\n", count);
		
			c = fromhex( hstr[2*i+1] ) + 16*fromhex( hstr[2*i] );
			t[i] = c;
		}
	}
	return t;
}

char fromhex(char c)
{
	if(isxdigit(c))
	{
		if(isdigit(c))
			c -= '0';
		else
		{
			c = tolower(c);
			c = c - 'a' + 10;
		}
	}
	else 
		c = 0;

	return c;
}

//flag 0: not used
//flag 1: not used
//flag 2: streamlink allgemein
//flag 4: streamlink youtube
//flag 1000: menu with pin
struct tithek
{
	char* title;
	char* link;
	char* pic;
	char* localname;
	char* menutitle;	
	int flag;
	struct tithek* prev;
	struct tithek* next;
};
struct tithek *tithek = NULL;

struct tithek* addtithek(char *line, int count, struct tithek* last)
{
	//debug(1000, "in");
	struct tithek *newnode = NULL, *prev = NULL, *node = tithek;
	char *link = NULL, *pic = NULL, *title = NULL, *localname = NULL, *menutitle = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct tithek*)malloc(sizeof(struct tithek));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	link = malloc(MINMALLOC);
	if(link == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	pic = malloc(MINMALLOC);
	if(pic == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		return NULL;
	}

	title = malloc(MINMALLOC);
	if(title == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		free(pic);
		return NULL;
	}

	localname = malloc(MINMALLOC);
	if(localname == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		free(pic);
		free(title);
		return NULL;
	}

	menutitle = malloc(MINMALLOC);
	if(menutitle == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		free(pic);
		free(title);
		free(localname);		
		return NULL;
	}

	memset(newnode, 0, sizeof(struct tithek));

	ret = sscanf(line, "%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%d", title, link, pic, localname, menutitle, &newnode->flag);
	if(ret != 6)
	{
		if(count > 0)
		{
			err("tithek line %d not ok or double", count);
		}
		else
		{
			err("add tithek");
		}
		free(link);
		free(pic);
		free(title);
		free(localname);
		free(menutitle);
		free(newnode);
		return NULL;
	}

	newnode->link = ostrcat(link, "", 1, 0);
	newnode->pic = ostrcat(pic, "", 1, 0);
	newnode->title = ostrcat(title, "", 1, 0);
	newnode->localname = ostrcat(localname, "", 1, 0);
	newnode->menutitle = ostrcat(menutitle, "", 1, 0);

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
		tithek = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;
	
	//debug(1000, "out");
	return newnode;
}

int readtithek(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct tithek* last = NULL, *tmplast = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addtithek(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int deltithek(char* link)
{
	debug(1000, "in");
	int ret = 1;
	struct tithek *node = tithek, *prev = tithek;

	while(node != NULL)
	{
		if(ostrcmp(link, node->link) == 0)
		{
			ret = 0;
			if(node == tithek)
			{
				tithek = node->next;
				if(tithek != NULL)
					tithek->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->link);
			node->link = NULL;

			free(node->pic);
			node->pic = NULL;

			free(node->title);
			node->title = NULL;

			free(node->localname);
			node->localname = NULL;

			free(node->menutitle);
			node->menutitle = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return ret;
}

void freetithek()
{
	debug(1000, "in");
	struct tithek *node = tithek, *prev = tithek;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltithek(prev->link);
	}
	debug(1000, "out");
}

char* tithekdownload(char* link, char* localname, char* pw, int pic, int flag)
{
	int ret = 1;
	char* ip = NULL, *pos = NULL, *path = NULL;
	char* tmpstr = NULL, *localfile = NULL;

	if(link == NULL) return NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = ostrcat(path, NULL, 0, 0);

	if(flag == 0)
	{
		localfile = ostrcat(TITHEKPATH, "/", 0, 0);
		if(localname == NULL)
			localfile = ostrcat(localfile, basename(tmpstr), 1, 0);
		else
			localfile = ostrcat(localfile, localname, 1, 0);
	}
	else
	{
		localfile = ostrcat(getconfig("rec_path", NULL), "/", 0, 0);
		if(localname == NULL)
			localfile = ostrcat(localfile, basename(tmpstr), 1, 0);
		else
			localfile = ostrcat(localfile, localname, 1, 0);
	}

	if(flag == 0)
	{
		if(!file_exist(localfile))
		{
			if(pic == 1)
				gethttp(ip, path, 80, localfile, pw, NULL, 0);
			else
				gethttp(ip, path, 80, localfile, pw, NULL, 0);
		}
	}
	else
	{
		if(file_exist(localfile))
			ret = textbox(_("Message"), _("File exist, overwrite?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

		if(ret == 1)
			screendownload("Download", ip, path, 80, localfile, pw, 0);
	}

	free(ip); ip = NULL;
	free(tmpstr); tmpstr = NULL;

	return localfile;
}

int createtithekplay(char* titheklink, struct skin* grid, struct skin* listbox, struct skin* countlabel)
{
	int gridbr = 0, posx = 0, count = 0, sumcount = 0;
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tmpstr = NULL;

	if(ostrstr(titheklink, "http://") != NULL)
		tithekfile = tithekdownload(titheklink, NULL, HTTPAUTH, 0, 0);
	else
		tithekfile = ostrcat(titheklink, NULL, 0, 0);

	delmarkedscreennodes(grid, 1);
	freetithek();
	if(readtithek(tithekfile) != 0) return 1;

	struct tithek* titheknode = tithek;

	while(titheknode != NULL)
	{
		tmp = addlistbox(grid, listbox, tmp, 1);
		if(tmp != NULL)
		{
			sumcount++;
			count++;
			if(gridbr == 0)
				tmp->type = GRIDBR;
			gridbr = 1;
			tmp->wrap = YES;

			tmp->picheight = 230;
			tmp->picwidth = 370;

			tmp->height = 280;
			tmp->width = 390;
			tmp->prozwidth = 0;
			//tmp->bgcol = 0xffffff;
			tmp->bgspace = 1;
			tmp->vspace = 10;
			tmp->hspace = 10;
			tmp->posx = posx;
			//tmp->fontcol = 0x0000ff;
			tmp->halign = CENTER;
			tmp->valign = TEXTBOTTOM;
			changetext(tmp, titheknode->title);
			tmp->handle = (char*)titheknode;
			posx += tmp->width;
			if(count >= 3)
			{
				count = 0;
				posx = 0;
				gridbr = 0;
			}
		}
		titheknode = titheknode->next;
	}

	tmpstr = oitoa(sumcount);
	changetext(countlabel, tmpstr);
	free(tmpstr); tmpstr = NULL;

	free(tithekfile); tithekfile = NULL;
	return 0;
}

void screentithekplay(char* titheklink, char* title, int first)
{
	int rcret = -1, oaktline = 1, oaktpage = -1, ogridcol = 0;

	if(file_exist("/var/bin/audio.elf") || file_exist("/var/swap/bin/audio.elf"))
		textbox(_("Message"), _("Alternativ Audio Firmware not working korrekt with all videos (DTSDOWNMIX)!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
	
	if(first == 1)
	{
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return;
	}
	status.hangtime = 99999;

	struct skin* grid = getscreen("titheklist");
	struct skin* listbox = getscreennode(grid, "listbox");
	struct skin* countlabel = getscreennode(grid, "countlabel");
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	char* tithekpic = NULL;
	
	if(titheklink == NULL) return;
	
	listbox->aktpage = -1;
	listbox->aktline = 1;
	listbox->gridcol = 0;
	listbox->select = NULL;

	if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) return;
				
	drawscreen(grid, 0, 0);
	addscreenrc(grid, listbox);
				
	while(1)
	{
		changetitle(grid, _(title));
		if(listbox->select != NULL && listbox->select->handle != NULL)
		{
			tmp = listbox->select;
			while(tmp != NULL)
			{
				if(tmp->pagecount != listbox->aktpage) break;
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->prev;
			}
			tmp = listbox->select;
			while(tmp != NULL)
			{
				if(tmp->pagecount != listbox->aktpage) break;
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->next;
			}

			drawscreen(grid, 0, 0);
		}
		
		int count = getfilecount(TITHEKPATH);
		if(count > 500)
			delallfiles(TITHEKPATH, ".jpg");
		
		rcret = waitrc(grid, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(((struct tithek*)listbox->select->handle)->flag == 2)
				{
					if(status.security == 1)
					{
						char* tmpstr = tithekdownload((((struct tithek*)listbox->select->handle)->link), NULL, NULL, 0, 1);
						free(tmpstr); tmpstr = NULL;
						drawscreen(grid, 0, 0);
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 4)
				{
					if(status.security == 1)
					{
						char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;
						tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 1);
						tmpstr2 = changefilenameext(((struct tithek*)tmp->handle)->localname, ".mp4");
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							char* tmpstr = tithekdownload(tmpstr1, tmpstr2, NULL, 0, 1);
							free(tmpstr); tmpstr = NULL;
							drawscreen(grid, 0, 0);
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
						free(tmpstr2); tmpstr2 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
			}
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				clearscreen(grid);

				if(((struct tithek*)listbox->select->handle)->flag == 2)
				{
					if(status.security == 1)
					{
						if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
							screenplay((((struct tithek*)listbox->select->handle)->link), 2, 0);				
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 4)
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 1);
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);				
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 5) || (((struct tithek*)listbox->select->handle)->flag == 6) || (((struct tithek*)listbox->select->handle)->flag == 7) || (((struct tithek*)listbox->select->handle)->flag == 8))
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						
						if(((struct tithek*)listbox->select->handle)->flag == 5)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://rtl2now.rtl2.de", "rtl2now", 2);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 6)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://www.superrtlnow.de", "superrtlnow", 2);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 7)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://rtl-now.rtl.de", "rtlnow", 2);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 8)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://www.voxnow.de", "voxnow", 2);
						}
	
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);				
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 9) || (((struct tithek*)listbox->select->handle)->flag == 10) || (((struct tithek*)listbox->select->handle)->flag == 11))
				{
					char* search = textinput("Search", NULL);
					if(search != NULL)
					{
						drawscreen(load, 0, 0);
						search = stringreplacechar(search, ' ', '+');
						char* id = NULL;
						char* line = NULL;
						char* pic = NULL;
						char* title = NULL;	
						char* ip = ostrcat("gdata.youtube.com", NULL, 0, 0);
						char* path = ostrcat("feeds/api/videos?q=", search, 0, 0);
						if(((struct tithek*)listbox->select->handle)->flag == 9)
							path = ostrcat(path, "&max-results=10", 1, 0);
						else if(((struct tithek*)listbox->select->handle)->flag == 10)
							path = ostrcat(path, "&max-results=25", 1, 0);
						else if(((struct tithek*)listbox->select->handle)->flag == 11)
							path = ostrcat(path, "&max-results=50", 1, 0);
									
						char* tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
						tmpstr = string_replace_all("media:thumbnail", "\nthumbnail", tmpstr, 1);
		
						int count = 0;
						int incount = 0;
						int i = 0;
						struct splitstr* ret1 = NULL;
						ret1 = strsplit(tmpstr, "\n", &count);
		
						if(ret1 != NULL)
						{
							int max = count;
							for(i = 0; i < max; i++)
							{
								if(ostrstr(ret1[i].part, "http://i.ytimg.com/vi/") != NULL)
								{
									pic = oregex(".*thumbnail url=\'(http://i.ytimg.com/vi/.*/0.jpg).*", ret1[i].part);
									id = oregex(".*thumbnail url=\'http://i.ytimg.com/vi/(.*)/0.jpg.*", ret1[i].part);
		
									if(id != NULL)
									{
										incount += 1;
										ip = ostrcat("www.youtube.com", NULL, 0, 0);
										path = ostrcat("watch?v=", id, 0, 0);
										title = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
										title = string_resub("<meta name=\"title\" content=\"", "\">", title, 0);
		
										line = ostrcat(line, title, 1, 0);
										line = ostrcat(line, "#http://www.youtube.com/watch?v=", 1, 0);
										line = ostrcat(line, id, 1, 0);
										line = ostrcat(line, "#", 1, 0);
										line = ostrcat(line, pic, 1, 0);
										line = ostrcat(line, "#youtube_search_", 1, 0);
										line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
										line = ostrcat(line, ".jpg#YouTube - Search#4\n", 1, 0);
										free(ip), ip = NULL;
										free(path), path = NULL;
										free(title), title = NULL;
									}
								}
							}
							free(ret1), ret1 = NULL;
		
							if(line != NULL)
							{
								writesys("/tmp/tithek/youtube.search.list", line, 0);
								free(line), line = NULL;
								if(createtithekplay("/tmp/tithek/youtube.search.list", grid, listbox, countlabel) != 0) return;
								drawscreen(grid, 0, 0);
							}
						}
						free(tmpstr), tmpstr = NULL;
					}
					free(search), search = NULL;
					continue;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 12)
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 4);
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);				
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else
				{
					int pincheck = 0;
					if(((struct tithek*)listbox->select->handle)->flag == 1000)
						pincheck = screenpincheck(0, NULL);
					if(pincheck == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat(((struct tithek*)listbox->select->handle)->menutitle, " - ", 0, 0);					
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;					
						if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				drawscreen(grid, 0, 0);
			}			
		}
	}

	freetithek();
	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);

	if(first == 1)
	{
		delallfiles("/tmp/tithek", ".list");
		servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
	}
	status.hangtime = getconfigint("hangtime", NULL);
}

// flag 1 = youtube streamlink
// flag 2 = rtlnow streamlinksrc
// flag 3 = rtlnow streamlink
char* getstreamurl(char* link, char* url, char* name, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* ip = NULL, *pos = NULL, *path = NULL, *pageUrl = NULL, *playpath = NULL, *video_id = NULL, *source = NULL;

	if(flag == 4)
	{
  			printf("111111111111\n");	
		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(link, ";", &count);
		if(ret1 != NULL)
		{
  			printf("22222222222\n");	

			link = ostrcat(ret1[0].part, NULL, 0, 0);
			pageUrl = ostrcat(pageUrl, ret1[1].part, 1, 0);
			playpath = ostrcat(playpath, ret1[2].part, 1, 0);
  			printf("3333333333333\n");	

			video_id = ostrcat(video_id, ret1[3].part, 1, 0);
  			printf("4444444444444\n");	
						
			printf("link: %s\n", link);
			printf("pageUrl: %s\n", pageUrl);
			printf("playpath: %s\n", playpath);											
			printf("video_id: %s\n", video_id);											

		}
		free(ret1), ret1 = NULL;
	}
	
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	char* tmpstr = NULL;
	tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
	char* streamurl = NULL;
		
	if(flag == 1)
	{
		tmpstr = string_resub("\": \"url=", "\", \"", tmpstr, 0);
	
		while(ostrstr(tmpstr, ",url=") != NULL)
			tmpstr = string_replace(",url=", "\nurl=", tmpstr, 1);
	
		tmpstr = string_decode(tmpstr, 0);
	
		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);
		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(ostrstr(ret1[i].part, "type=video/mp4") != NULL)
				{
					streamurl = ostrcat(streamurl, ret1[i].part, 1, 0);
					int count2 = 0;
					struct splitstr* ret2 = NULL;
					ret2 = strsplit(ret1[i].part, "+", &count2);
					if(ret2 != NULL)
					{
						free(streamurl);
						streamurl = ostrcat("", ret2[0].part, 0, 0);
						free(ret2); ret2 = NULL;
					}
				}
			}
			free(ret1); ret1 = NULL;
			
			streamurl = string_replace("url=", "", streamurl, 1);
		}
		free(tmpstr); tmpstr = NULL;
	}
	else if(flag == 2)
	{
		tmpstr = string_resub("data:'", "',", tmpstr, 0);
		debug(99, "tmpstr: %s", tmpstr);

		htmldecode(tmpstr, tmpstr);
		tmpstr = ostrcat(url, tmpstr, 0, 1);
		debug(99, "streamurl: %s", tmpstr);
		streamurl = getstreamurl(tmpstr, url, name, 3);
		free(tmpstr); tmpstr = NULL;
	}
	else if(flag == 3)
	{
//		string_resub("delivery=\"streaming\"><![CDATA[", "]]></filename>", tmpstr, 0);
		tmpstr = string_resub("rtmpe://", ".f4v", tmpstr, 0);
		char* tmpstr9 = NULL;
		tmpstr9 = ostrcat(tmpstr9, tmpstr, 1, 0);
		free(tmpstr), tmpstr = NULL;
		tmpstr = ostrcat("rtmpe://", tmpstr9, 0, 0);
		tmpstr = ostrcat(tmpstr, ".f4v", 1, 0);		
		free(tmpstr9), tmpstr9 = NULL;

		debug(99, "tmpstr: %s", tmpstr);

		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "/", &count);
		if(ret1 != NULL)
		{
			int max = count;
			char* link = NULL;
			char* path = NULL;
			for(i = 0; i < max; i++)
			{
				if(i < 3)
				{
					if(count > i)
						link = ostrcat(link, (&ret1[i])->part, 1, 0);

					if(i == 0)
						link = ostrcat(link, "//", 1, 0);
					else
						link = ostrcat(link, "/", 1, 0);
				}
				else
				{
					if(count > i)
						path = ostrcat(path, (&ret1[i])->part, 1, 0);
					if(i != max - 1)
						path = ostrcat(path, "/", 1, 0);
				}
			}
			free(ret1), ret1 = NULL;

			debug(99, "link: %s", link);
			debug(99, "path: %s", path);
	
			streamurl = ostrcat(link, " swfVfy=1 playpath=mp4:", 0, 0);
			streamurl = ostrcat(streamurl, path, 1, 0);
			streamurl = ostrcat(streamurl, " app=", 1, 0);
			streamurl = ostrcat(streamurl, name, 1, 0);
			streamurl = ostrcat(streamurl, "/_definst_ pageUrl=", 1, 0);
			streamurl = ostrcat(streamurl, url, 1, 0);
			streamurl = ostrcat(streamurl, "/p/ tcUrl=", 1, 0);
			streamurl = ostrcat(streamurl, link, 1, 0);
			streamurl = ostrcat(streamurl, " swfUrl=", 1, 0);
			streamurl = ostrcat(streamurl, url, 1, 0);
			streamurl = ostrcat(streamurl, "/includes/vodplayer.swf", 1, 0);		
	
			if(link != NULL)
				free(link), link = NULL;
	
			if(path != NULL)
				free(path), path = NULL;
		}
		free(tmpstr); tmpstr = NULL;
		debug(99, "streamurl: %s", streamurl);
	}
	if(flag == 4)
	{
  			printf("soooooooooooooooooooooooooo111\n");	
//		printf("tmpstr: %s", tmpstr);
		writesys("/var/usr/local/share/titan/plugins/tithek/list", tmpstr, 0);
//		tmpstr = string_resub("_encxml=", "/0", tmpstr, 0);
  			printf("soooooooooooooooooooooooooo2222\n");
		char* tmpstr_uni = NULL;
		char* b64 = NULL;
		char* key = NULL;

		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "=", &count);
int slen;
		char buf[200];
		if(ret1 != NULL)
		{
			printf("ret1[1].part=%s\n", (ret1[1]).part);
			slen = strlen((ret1[1]).part);
// not workin correct
			tmpstr_uni = unhexlify1(ret1[1].part);
		}
		free(ret1), ret1 = NULL;
		writesys("/var/usr/local/share/titan/plugins/tithek/list_uni", tmpstr_uni, 0);

//////////////
		char* video_id_md5 = NULL;		
		video_id_md5 = MDString(video_id);
		printf("video_id_md5: %s\n", video_id_md5);
/*
		char* gk = NULL;
		gk = ostrcat(gk, "WXpnME1EZGhNRGhpTTJNM01XVmhOREU0WldNNVpHTTJOakptTW1FMU5tVTBNR05pWkRaa05XRXhNVFJoWVRVd1ptSXhaVEV3TnpsbA0KTVRkbU1tSTRNdz09", 1, 0);
		printf("gk: %s\n", gk);		
		
		char* b64_gk = malloc(255);		
		b64dec(b64_gk, gk);
		printf("b64_gk: %s\n", b64_gk);

		char* gk_video_id_md5 = NULL;
		gk_video_id_md5 = ostrcat(b64_gk, video_id_md5, 0, 0);
		free(video_id_md5), video_id_md5 = NULL;
		printf("gk_video_id_md5: %s\n", gk_video_id_md5);
		free(b64_gk), b64_gk = NULL;

		char* b64_gk_video_id_md5 = malloc(255);		
		b64dec(b64_gk_video_id_md5, gk_video_id_md5);
		free(gk_video_id_md5), gk_video_id_md5 = NULL;
		printf("b64_gk_video_id_md5: %s\n", b64_gk_video_id_md5);
*/		
		b64 = ostrcat("c8407a08b3c71ea418ec9dc662f2a56e40cbd6d5a114aa50fb1e1079e17f2b83", video_id_md5, 0, 0);
		printf("b64: %s\n", b64);

		key = MDString(b64);
//		key = MDString(b64_gk_video_id_md5);
//		free(b64_gk_video_id_md5), b64_gk_video_id_md5 = NULL;

		printf("key: %s\n", key);
		printf("slen: %d\n", slen);
		slen = strlen(tmpstr_uni);
		int klen = strlen(key);
		printf("slen: %d\n", slen);
		printf("klen: %d\n", klen);
						
//		rc4(tmpstr_uni, strlen(tmpstr_uni), key, strlen(key));
		rc4(tmpstr_uni, slen, key, klen);
		writesys("/var/usr/local/share/titan/plugins/tithek/list_key", tmpstr_uni, 1);			

		slen = strlen(buf);
		rc4(buf, slen, key, klen);
		writesys("/var/usr/local/share/titan/plugins/tithek/list_keybuf", buf, 1);	
		
		debug(99, "tmpstr: %s\n", tmpstr);
		debug(99, "pageUrl: %s\n", pageUrl);
		debug(99, "playpath: %s\n", playpath);
		debug(99, "video_id: %s\n", video_id);
		//printf("tmpstr_uni: %s\n",tmpstr_uni);

htmldecode(tmpstr_uni, tmpstr_uni);

		if(ostrstr(tmpstr_uni, "connectionurl='rtmp"))
		{
			printf("found rtmp stream\n");
			int count2 = 0;
			int i = 0;
			struct splitstr* ret2 = NULL;
			ret2 = strsplit(tmpstr_uni, " ", &count2);
			if(ret2 != NULL)
			{
				int max = count2;
				for(i = 0; i < max; i++)
				{
					printf("ret2[i].part= (%d/%d) %s\n", i, max, (ret2[i]).part);
					if(ostrstr((ret2[i]).part, "connectionurl='rtmp"))
					{
						printf("add rtmp stream as url\n");					
						url = ostrcat((ret2[i]).part, NULL, 0, 0);
						url = string_resub("connectionurl='", "'", url, 0);
//						htmldecode(url, url);
					}
					else if(ostrstr((ret2[i]).part, "source='"))
					{
						printf("add rtmp stream as source\n");					
						source = ostrcat((ret2[i]).part, NULL, 0, 0);
						source = string_resub("source='", ".flv'", source, 0);
//						htmldecode(source, source);
					}
				}
			}
			
	//		tmpstr = string_resub("connectionurl='", "\0", tmpstr_uni, 0);
	//		debug(99, "tmpstr: %s", tmpstr);
	//printf("tmpstr: %s\n",tmpstr);
		}
		else
			printf("tmpstr_uni not found rtmp: %s\n",tmpstr_uni);


//tmpstr = ostrcat(NULL, "=c3RhcnRfdGltZT0yMDEyMDgwMjEzMjE1NiZlbmRfdGltZT0yMDEyMDgwMjE4MjIwMSZkaWdlc3Q9YWIyYzNmNzM2NTJjYjI4ZDIwODVjYTg5NTM0MGYzZTc=", 0, 0);		
//		streamurl = ostrcat("rtmpe://myvideo3fs.fplive.net/myvideo3/?token=c3RhcnRfdGltZT0yMDEyMDgwMjEzMjE1NiZlbmRfdGltZT0yMDEyMDgwMjE4MjIwMSZkaWdlc3Q9YWIyYzNmNzM2NTJjYjI4ZDIwODVjYTg5NTM0MGYzZTc= tcUrl=rtmpe://myvideo3fs.fplive.net/myvideo3/?token=c3RhcnRfdGltZT0yMDEyMDgwMjEzMjE1NiZlbmRfdGltZT0yMDEyMDgwMjE4MjIwMSZkaWdlc3Q9YWIyYzNmNzM2NTJjYjI4ZDIwODVjYTg5NTM0MGYzZTc= swfVfy=http://is4.myvideo.de/de/player/mingR11q/ming.swf pageUrl=http://www.myvideo.de/watch/8470917/ playpath=flv:movie24/6a/8470917", NULL, 0, 0);
//		streamurl = ostrcat("rtmpe://myvideo3fs.fplive.net/myvideo3/?token=", tmpstr_uni, 0, 0);
		streamurl = ostrcat(url, NULL, 0, 0);
		
		streamurl = ostrcat(streamurl, " ", 1, 0);
//		streamurl = ostrcat(streamurl, "tcUrl=rtmpe://myvideo3fs.fplive.net/myvideo3/?token=", 1, 0);
		streamurl = ostrcat(streamurl, "tcUrl=", 1, 0);
		streamurl = ostrcat(streamurl, url, 1, 0);

//		streamurl = ostrcat(streamurl, tmpstr_uni, 1, 0);
		streamurl = ostrcat(streamurl, " swfVfy=http://is4.myvideo.de/de/player/mingR11q/ming.swf ", 1, 0);
		streamurl = ostrcat(streamurl, pageUrl, 1, 0);
		streamurl = ostrcat(streamurl, " ", 1, 0);
//		streamurl = ostrcat(streamurl, playpath, 1, 0);
		streamurl = ostrcat(streamurl, "playpath=flv:", 1, 0);
		streamurl = ostrcat(streamurl, source, 1, 0);

		


		free(key); key = NULL;		
		free(b64); b64 = NULL;		
		free(url); url = NULL;
		free(source); source = NULL;		
		free(tmpstr_uni); tmpstr_uni = NULL;		
		free(tmpstr); tmpstr = NULL;
		free(pageUrl); pageUrl = NULL;		
		free(playpath); playpath = NULL;
		debug(99, "streamurl: %s", streamurl);

		printf("streamurl: %s\n", streamurl);
	}
	return streamurl;
}

#endif
