#ifndef OLED_H_
#define OLED_H_

void write2oled(unsigned char *buf, int xres, int yres)
{
	unsigned char* lfb1 = NULL;
	unsigned char* lfb2 = NULL;
	unsigned char byte;
	int i = 0;
	int i1 = 0;
	int bi = 0;
	int ret = 0;
	
	int lcdfd1 = open(getconfig("vfddev", NULL), O_RDWR);
	
	if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
		lfb1 = malloc(xres * yres * 2);
	else if(checkbox("VUDUO4K") == 1 || checkbox("VUULTIMO4K") == 1)
		lfb1 = malloc(480 * 320 * 4);
	else if(checkbox("VUUNO4KSE") == 1)
		lfb1 = malloc(400 * 240 * 4);
	else
		lfb1 = malloc(xres * yres);
	if(lfb1 == NULL)
		return;

	if(checkbox("DM900") != 1 && checkbox("DM920") != 1 && checkbox("VUDUO4K") != 1 && checkbox("VUUNO4KSE") != 1 && checkbox("VUULTIMO4K") != 1)
	{
		
		for(i = 0; i <= xres*yres; i++)
		{
			//             R                           B                          G
			lfb1[i] = ((76 * buf[bi+2]) / 255) + ((150 * buf[bi+1]) / 255) + ((28 * buf[bi]) / 255);
			bi = bi + 4;
		}
	
		
		if(checkchipset("BCM7424") == 1)
		{
			ret = write(lcdfd1, lfb1, xres * yres);
			if(ret != xres * yres)
				err("write to oled - %s - was not ok", getconfig("vfddev", NULL));
		}
		//Die Displays der dm7020hd, dm7025+(?), dm7080 und dm8000 haben 128*64*4bpp, DM800se hat 96*64*16bpp (RGB565), dm800 und dm7025(?) haben 132*64*1bpp (ältere ggf. 120*64 oder 128*64)
		else if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
		{
			bi = 0;
			lfb2 = malloc(xres * yres / 2);
			if(lfb2 == NULL)
				return;
			for(i = 0; i <= xres*yres; i = i + 2)
			{
				byte = lfb1[i] * 15 / 255;
				lfb2[bi] = (byte << 4) & 0xf0;
				byte = lfb1[i+1] * 15 / 255;
				lfb2[bi] |= byte & 0x0f;
				bi = bi + 1;
			}
			ret = write(lcdfd1, lfb2, xres * yres / 2);
			if(ret != xres * yres / 2)
				err("write to oled - %s - was not ok", getconfig("vfddev", NULL));
			free(lfb2);
		}
	}
	else if(checkbox("VUDUO4K") == 1 || checkbox("VUUNO4KSE") == 1  || checkbox("VUULTIMO4K") == 1)
	{
		ret = write(lcdfd1, buf, xres * yres * 4);
		if(ret != xres * yres * 4)
			err("write to oled2 VUnnnO4K - %s - was not ok", getconfig("vfddev", NULL));
	}
	else
	{
		//gggbbbbbrrrrrggg
//		printf("----> write2oled\n");
		for(i = 0; i < xres*yres*2; i = i + 2)
		{
			lfb1[i] = ((buf[bi+1] << 3) & 0xE0) | ((buf[bi] >> 3) & 0x1F);
			lfb1[i+1] = (buf[bi+2] & 0xF8) | ((buf[bi+1] >> 5) & 0x07);
			bi = bi + 4;
		}
		ret = write(lcdfd1, lfb1, xres * yres * 2);
		if(ret != xres * yres * 2)
			err("write to oled2 - %s - was not ok", getconfig("vfddev", NULL));
		free(lfb2);
	}
	close(lcdfd1);
	
	free(lfb1);
	return;
}

int oledtext(char *value)
{
	struct skin* OLED_all = NULL;
	
	if(getconfigint("oled_off", NULL) == 1)
		return 0;

	m_lock(&status.drawingmutex, 0);
	
	if(checkchipset("BCM7424") == 1)
	{
		if(status.updatevfd == PAUSE)
			OLED_all = getscreen("OLED_nemesis_menu");
		else if(status.standby > 0)
			OLED_all = getscreen("OLED_nemesis_standby");
		else
		{
			if(getskinconfig("OLED_nemesis", NULL) == NULL)
				OLED_all = getscreen("OLED_nemesis");
			else
				OLED_all = getscreen(getskinconfig("OLED_nemesis", NULL));
		}
	}
	else if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
	{
		if(status.updatevfd == PAUSE)
			OLED_all = getscreen("OLED_dream1_menu");
		else if(status.standby > 0)
			OLED_all = getscreen("OLED_dream1_standby");
		else
		{
			if(getskinconfig("OLED_dream1", NULL) == NULL)
				OLED_all = getscreen("OLED_dream1");
			else
				OLED_all = getscreen(getskinconfig("OLED_dream1", NULL));
		}
	}
	else if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
	{
		if(status.updatevfd == PAUSE)
			OLED_all = getscreen("OLED_dream2_menu");
		else if(status.standby > 0)
			OLED_all = getscreen("OLED_dream2_standby");
		else
		{
			if(getskinconfig("OLED_dream2", NULL) == NULL)
				OLED_all = getscreen("OLED_dream2");
			else
				OLED_all = getscreen(getskinconfig("OLED_dream2", NULL));
		}
	}
	else if(checkbox("VUDUO4K") == 1 || checkbox("VUULTIMO4K") == 1)		
	{
		if(status.updatevfd == PAUSE)
			OLED_all = getscreen("OLED_vu1_menu");
		else if(status.standby > 0)
			OLED_all = getscreen("OLED_vu1_standby");
		else
		{
			if(getskinconfig("OLED_vu1", NULL) == NULL)
				OLED_all = getscreen("OLED_vu1");
			else
				OLED_all = getscreen(getskinconfig("OLED_vu1", NULL));
		}
	}
	else if(checkbox("VUUNO4KSE") == 1)		
	{
		if(status.updatevfd == PAUSE)
			OLED_all = getscreen("OLED_vu2_menu");
		else if(status.standby > 0)
			OLED_all = getscreen("OLED_vu2_standby");
		else
		{
			if(getskinconfig("OLED_vu2", NULL) == NULL)
				OLED_all = getscreen("OLED_vu2");
			else
				OLED_all = getscreen(getskinconfig("OLED_vu2", NULL));
		}
	}
	
		
	
	struct skin* textbox = getscreennode(OLED_all, "textbox");
	
	if(status.standby == 2 && status.epgscanlistthread != NULL)
		changetext(textbox, "EPG-Scan");
	else
		changetext(textbox, value);

	drawscreen(OLED_all, 0, 2);
	m_unlock(&status.drawingmutex, 0);
	
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
	newnode->data_phys = 0;
	
	return newnode;
}

void initOLEDdream1()
{
#ifndef LCD_IOCTL_ASC_MODE
#define LCDSET                  0x1000
#define LCD_IOCTL_ASC_MODE		(21|LCDSET)
#define	LCD_MODE_ASC			0
#define	LCD_MODE_BIN			1
#endif

	if(checkbox("VUDUO4K") != 1 && checkbox("VUUNO4KSE") != 1  && checkbox("VUULTIMO4K") != 1)
		return; //wird nicht benötigt

	int i=LCD_MODE_BIN;
	//int lcdfd = open("/dev/dbox/oled0", O_RDWR);
	int lcdfd1 = open(getconfig("vfddev", NULL), O_RDWR);
	if(lcdfd1 > -1)
	{
		ioctl(lcdfd1, LCD_IOCTL_ASC_MODE, &i);
		close(lcdfd1);
	}
}	

#endif	
	
