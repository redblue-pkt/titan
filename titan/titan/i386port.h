#ifndef I386PORT_H
#define I386PORT_H

void fbsave()
{
}

void fbrestore()
{
}

void setfbvarsize(struct fb* newnode)
{
	if(newnode != NULL)
		newnode->varfbsize = newnode->width * newnode->height * newnode->colbytes;
}

void enablemanualblit()
{
}

//flag 0 = no animation
//flag 1 = animation
void blitfb2(struct fb* fbnode, int flag)
{
	int i = 0;

	if(fbnode == NULL) return;

#ifdef NOFB
	if(status.rguidfd > -1)
	{
		unsigned char* buf = NULL;
		buf = scale(fbnode->fb, fbnode->width, fbnode->height, 4, 320, 240, 0);
		if(buf != NULL)
		{
			socksend(&status.rguidfd, (unsigned char*)buf, 320 * 240 * 4, 5000 * 1000);
			free(buf); buf = NULL;
		}
	}
	if(status.write_png == 1 && status.infobaraktiv == 0)
	{
		unsigned char* buf = NULL;
		buf = scale(fbnode->fb, fbnode->width, fbnode->height, 4, 320, 240, 0);
		if(buf != NULL)
		{
			if(writeFBfile.ActBuf == NULL)
			{
				writeFBfile.buf1 = malloc(4 * 320 * 240);
				writeFBfile.ActBuf = writeFBfile.buf1;
				memcpy(writeFBfile.buf1, (unsigned char*)buf, 4 * 320 * 240); 
				addtimer(&fb2png_thread, START, 10000, 1, NULL, NULL, NULL);
			}
			else if(writeFBfile.buf1 == writeFBfile.ActBuf)
			{
				if(writeFBfile.buf2 == NULL)
					writeFBfile.buf2 = malloc(4 * 320 * 240);
				memcpy(writeFBfile.buf2, (unsigned char*)buf, 4 * 320 * 240);
			}
			else if(writeFBfile.buf2 == writeFBfile.ActBuf)
			{
				if(writeFBfile.buf1 == NULL)
					writeFBfile.buf1 = malloc(4 * 320 * 240);
				memcpy(writeFBfile.buf1, (unsigned char*)buf, 4 * 320 * 240);
			}
			//fb2png((unsigned char*)buf, 320, 240, "/tmp/titanlcd.png");
			free(buf); buf = NULL;
		}
	}

	if(fbnode != fb)
	{
		for(i = 0; i < fbnode->height; i++)
		{
			memcpy(fb->fb + (i * fb->pitch), fbnode->fb + (i * fbnode->pitch), fbnode->width * fbnode->colbytes);
		}
	}
	system("killall -9 xloadimage");

	FILE *fd;
	fd=fopen("titan.png", "w");
	fwrite(fb->fb, fb->varfbsize, 1, fd);
	fclose(fd);

	system("fbgrab -f titan.png -w 1280 -h 720 -b 32 titan1.png > /dev/null");
	system("xloadimage titan1.png > /dev/null &");
#endif
}

void setfbtransparent(int value)
{
}

int allocbpamem(size_t size, int *memfd, unsigned char **mem)
{
	return -1;
}

void freebpamem(int memfd, unsigned char* mem, size_t len)
{
}

//mode 0: with fill (draw to skinfb)
//mode 1: without fill (draw to skinfb)
//mode 2: with fill (draw to fb)
//mode 3: without fill (draw to fb)
void blitrect(int posx, int posy, int width, int height, long color, int transparent, int mode)
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

	if(mode == 0 || mode == 2)
	{
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			{
				drawpixel(posx + x, posy + y, tmpcol);
			}
		}
	}
	else if(mode == 1 || mode == 3)
	{
		//topline
		for(x = 0; x < width; x++)
			drawpixel(posx + x, posy, tmpcol);
		//bottomline
		for(x = 0; x < width; x++)
			drawpixel(posx + x, posy + height - 1, tmpcol);
		//leftline
		for(y = 0; y < height; y++)
			drawpixel(posx, posy + y, tmpcol);
		//rightline
		for(y = 0; y < height; y++)
			drawpixel(posx + width - 1, posy + y, tmpcol);
	}
	//debug(1000, "out");
}

int readjpg(const char* filename, unsigned long* width, unsigned long* height, unsigned long* rowbytes, int* channels, unsigned char **mem, int *memfd)
{
	return -1;
}

//flag 0: blit from accelfb to skinfb
//flag 1: blit from skinfb to accelfb
void blitscale(int posx, int posy, int width, int height, int scalewidth, int scaleheight, int flag)
{
}

void blitjpg(unsigned char* buf, int posx, int posy, int width, int height, int scalewidth, int scaleheight, int mwidth, int mheight, int halign, int valign)
{
}

void sighandler(int sig, struct sigcontext ctx)
{

	debug(1000, "in");
	switch(sig)
	{
		case SIGPIPE:
		{
			err("got signal sigpipe but ignore it");
			break;
		}
		case SIGUSR1:
		{
			//todo all configs
			reloadconfig(status.configfile);
			reloadconfig(getconfig("ownconfig", NULL));
			break;
		}
		case SIGSEGV:
		case SIGBUS:
		case SIGABRT:
		{
			//intel
			debugstack((void *)ctx.eip, NULL);
			err("got signal %d, fault address 0x%lx from 0x%lx", sig, ctx.cr2, ctx.eip);

			if(getconfigint("saverun", NULL) == 1 && status.longjumpbuf != NULL)
				siglongjmp(status.longjumpbuf, 1);
			else
				exit(100);
			break;
		}
	}
	debug(1000, "out");
}

#endif
