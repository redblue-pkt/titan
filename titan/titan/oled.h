#ifndef OLED_H_
#define OLED_H_

void write2oled(unsigned char *buf, int xres, int yres)
{
	unsigned char* lfb1 = NULL;
	int i = 0;
	int bi = 0;
	
	lfb1 = malloc(xres * yres);
	if(lfb1 == NULL)
		return;
		
	for(i = 0; i <= xres*yres; i++)
	{
		//             R                           B                          G
		lfb1[i] = ((76 * buf[bi+2]) / 255) + ((150 * buf[bi+1]) / 255) + ((28 * buf[bi]) / 255);
		bi = bi + 4;
	}
	
	int lcdfd1 = open("/dev/dbox/oled0", O_RDWR);
	write(lcdfd1, lfb1, xres * yres);
	close(lcdfd1);
	
	free(lfb1);
	return;
}

int oledtext(char *value)
{
	struct skin* OLED_nemesis = NULL;
	
	if(getconfigint("oled_off", NULL) == 1)
		return 0;
	
	if(status.updatevfd == PAUSE)
		OLED_nemesis = getscreen("OLED_nemesis_menu");
	else if(status.standby > 0)
		OLED_nemesis = getscreen("OLED_nemesis_standby");
	else
	{
		if(getskinconfig("OLED_nemesis", NULL) == NULL)
			OLED_nemesis = getscreen("OLED_nemesis");
		else
			OLED_nemesis = getscreen(getskinconfig("OLED_nemesis", NULL));
	}
	
	struct skin* textbox = getscreennode(OLED_nemesis, "textbox");
	
	if(status.standby == 2 && status.epgscanlistthread != NULL)
		changetext(textbox, "EPG-Scan");
	else
		changetext(textbox, value);
	if(getconfigint("oled_mutex", NULL) == 1)
	{
		m_lock(&status.drawingmutex, 0);
		drawscreen(OLED_nemesis, 0, 2);
		m_unlock(&status.drawingmutex, 0);
	}
	else
		drawscreen(OLED_nemesis, 0, 0);
	
	
	
	return 0;
}

struct fb* oledaddfb(int width, int height) 
{
	struct fb *newnode = NULL;
	char *name = ostrcat("oledskinfb", NULL, 0, 0);
	newnode = (struct fb*)malloc(sizeof(struct fb));
		
	unsigned char *newskinfb = calloc(1, 4 * width * height);
	if(newskinfb == NULL)
		return NULL; 
	memset(newnode, 0, sizeof(struct fb));
	newnode->name = name;
	newnode->dev = 999;
	newnode->width = width;
	newnode->height = height;
	newnode->colbytes = 4;
	newnode->pitch = newnode->width * newnode->colbytes;
	newnode->fb = newskinfb;
	newnode->fblong = (unsigned long*)newnode->fb;
	newnode->fd = -1;
	newnode->fixfbsize = 4 * width * height;
	
	return newnode;
}	

#endif	
	