#ifndef FB_H
#define FB_H

struct fb* getfb(char *name)
{
	debug(1000, "in");
	struct fb *node = fb;

	while(node != NULL)
	{
		if(strstr(node->name, name) != NULL)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "framebuffer not found (%s)", name);
	return NULL;
}

long getfbsize(int dev)
{
	debug(1000, "in");
	struct fb* node = fb;
	unsigned long fbmemcount = 0;
	struct fb_fix_screeninfo fix_screeninfo;

	if(fb == NULL)
	{
		debug(1000, "out -> NULL dedect");
		return 0;
	}

#ifndef NOFB
	if(ioctl(fb->fd, FBIOGET_FSCREENINFO, &fix_screeninfo) == -1)
	{
		perr("ioctl FBIOGET_FSCREENINFO failed");
		return 0;
	}
#else
	fix_screeninfo.smem_len = 16*1024*1024;
#endif

	while(node != NULL)
	{
		if(node->dev == dev)
			fbmemcount = fbmemcount + node->varfbsize;
		node = node->next;
	}

	debug(1000, "out");
	return fix_screeninfo.smem_len - fbmemcount;
}

struct fb* addfb(char *fbname, int dev, int width, int height, int colbytes, int fd, unsigned char* mmapfb, unsigned long fixfbsize)
{
	debug(1000, "in");
	struct fb *newnode = NULL, *node = fb;
	char *name = NULL;
	long ret = 0;

	name = ostrcat(fbname, "", 0, 0);
	if(name == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode = (struct fb*)malloc(sizeof(struct fb));	
	if(newnode == NULL)
	{
		err("no memory");
		free(name);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct fb));
	newnode->name = name;
	newnode->dev = dev;
	newnode->width = width;
	newnode->height = height;
	newnode->colbytes = colbytes;
	newnode->pitch = newnode->width * newnode->colbytes;
	newnode->fb = mmapfb;
	newnode->fblong = (unsigned long*)newnode->fb;
	newnode->fd = fd;
	newnode->fixfbsize = fixfbsize;
	if(ostrcmp(name, FB) == 0)
	{
#ifdef NOHWBLIT
		newnode->varfbsize = newnode->width * newnode->height * newnode->colbytes;
#else
		newnode->varfbsize = 1920 * 1080 * newnode->colbytes;
#endif
	}
	else if(ostrcmp(name, FB1) == 0)
	{
		newnode->varfbsize = 720 * 576 * newnode->colbytes;
	}
	else
	{
		newnode->varfbsize = width * height * newnode->colbytes;
	}

	if(node != NULL)
	{
		while(node->next != NULL)
			node = node->next;
		node->next = newnode;
	}
	else
		fb = newnode;


	ret = getfbsize(dev);
	if(ret < 0)
	{
		err("framebuffermem (%s) to small, needed = %ld", name, ret * -1);
		free(newnode);
		if(newnode == fb) fb = NULL;
		return NULL;
	}

	debug(100, "fbname=%s, fbwidth=%d, fbheight=%d, fbcol=%d, fbsize=%ld", newnode->name, newnode->width, newnode->height, newnode->colbytes, newnode->varfbsize);
	debug(1000, "out");
	return newnode;
}

void delfb(char *name)
{
	debug(1000, "in");
	struct fb *node = fb, *prev = fb;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			if(node == fb)
				fb = node->next;
			else
				prev->next = node->next;

			free(node->name);
			node->name = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void freefb()
{
	debug(1000, "in");
	struct fb *node = fb, *prev = fb;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delfb(prev->name);
	}
	debug(1000, "out");
}

struct fb* openfb(char *fbdev, int devnr)
{
	debug(1000, "in");
	int fd = -1;
	unsigned char *mmapfb = NULL;
	struct fb_var_screeninfo var_screeninfo;
	struct fb_fix_screeninfo fix_screeninfo;
	struct fb* node = NULL;

	if(fbdev == NULL)
	{
		err("failed to find fbdev in config file");
		return NULL;
	}

#ifndef NOFB
	fd = open(fbdev, O_RDWR);
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return NULL;
	}
	closeonexec(fd);

	if(ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo) == -1)
	{
		perr("ioctl FBIOGET_VSCREENINFO failed");
		close(fd);
		return NULL;
	}

	if(ioctl(fd, FBIOGET_FSCREENINFO, &fix_screeninfo) == -1)
	{
		perr("ioctl FBIOGET_FSCREENINFO failed");
		close(fd);
		return NULL;
	}

	if(!(mmapfb = (unsigned char*)mmap(0, fix_screeninfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)))
	{
		perr("mmap framebuffer");
		close(fd);
		return NULL;
	}

	if(devnr == 0)
		node = addfb(FB, devnr, var_screeninfo.xres, var_screeninfo.yres, var_screeninfo.bits_per_pixel / 8, fd, mmapfb, fix_screeninfo.smem_len);
	if(devnr == 1)
		node = addfb(FB1, devnr, var_screeninfo.xres, var_screeninfo.yres, var_screeninfo.bits_per_pixel / 8, fd, mmapfb, fix_screeninfo.smem_len);

#else

	mmapfb = malloc(16 * 1024 * 1024);
	if(devnr == 0)
		node = addfb(FB, devnr, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, -1, mmapfb, 16*1024*1024);
	if(devnr == 1)
		node = addfb(FB1, devnr, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, -1, mmapfb, 16*1024*1024);

#endif

	debug(1000, "out");
	return node;
}

void closefb()
{
	if(fb != NULL)
	{
		if(fb->fb != NULL)
			munmap(fb->fb, fb->fixfbsize);
		if(fb->fd != -1)
			close(fb->fd);
#ifdef NOFB
	free(fb->fb);
#endif
	}
}

void clearfball()
{
	if(fb->fb != NULL)
		memset(fb->fb, 0, fb->fixfbsize);
}

void clearfb(struct fb *node)
{
	if(node != NULL)
		memset(node->fb, 0, node->varfbsize);
}

void blitfb()
{
	if(skinfb == NULL) return;
#ifndef NOHWBLIT
	if(skinfb == fb) return;

	int mode3d = 0, leftoffset = 0, rightoffset = 0, topoffset = 0, bottomoffset = 0;
	char* mode3dstr = NULL;

	leftoffset = getconfigint("fbleftoffset", NULL);
	rightoffset = getconfigint("fbrightoffset", NULL);
	topoffset = getconfigint("fbtopoffset", NULL);
	bottomoffset = getconfigint("fbbottomoffset", NULL);

	mode3dstr = getconfig("av_mode3d", NULL);
	if(ostrcmp(mode3dstr, "sbs") == 0)
		mode3d = 1;
	else if(ostrcmp(mode3dstr, "tab") == 0)
		mode3d = 2;

	STMFBIO_BLT_DATA  bltData;
	memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));

	bltData.operation  = BLT_OP_COPY;
	bltData.srcOffset  = fb->varfbsize;
	bltData.srcPitch   = skinfb->pitch;
	bltData.src_top    = 0;
	bltData.src_left   = 0;
	bltData.src_right  = skinfb->width;
	bltData.src_bottom = skinfb->height;
	bltData.srcFormat  = SURF_BGRA8888;
	bltData.srcMemBase = STMFBGP_FRAMEBUFFER;

	bltData.dstOffset  = 0;
	bltData.dstPitch   = fb->pitch;
	bltData.dst_left   = 0 + leftoffset;
	bltData.dst_top    = 0 + topoffset;
	if(mode3d == 1)
		bltData.dst_right = (fb->width - rightoffset) / 2;
	else
		bltData.dst_right  = fb->width - rightoffset;
	if(mode3d == 2)
		bltData.dst_bottom = (fb->height - bottomoffset) / 2;
	else
		bltData.dst_bottom = fb->height - bottomoffset;
	bltData.dstFormat  = SURF_BGRA8888;
	bltData.dstMemBase = STMFBGP_FRAMEBUFFER;

	if (ioctl(fb->fd, STMFBIO_BLT, &bltData) < 0)
	{
		perr("ioctl STMFBIO_BLT");
	}
	if(ioctl(fb->fd, STMFBIO_SYNC_BLITTER) < 0)
	{
		perr("ioctl STMFBIO_SYNC_BLITTER");
	}

	if(mode3d != 0)
	{
		if(mode3d == 1)
			bltData.dst_left   = 0 + leftoffset + ((fb->width - rightoffset) / 2);
		if(mode3d == 2)
			bltData.dst_top    = 0 + topoffset + ((fb->height - bottomoffset) / 2);
		bltData.dst_right  = fb->width - rightoffset;
		bltData.dst_bottom = fb->height - bottomoffset;

		if (ioctl(fb->fd, STMFBIO_BLT, &bltData) < 0)
		{
			perr("ioctl STMFBIO_BLT");
		}
		if(ioctl(fb->fd, STMFBIO_SYNC_BLITTER) < 0)
		{
			perr("ioctl STMFBIO_SYNC_BLITTER");
		}
	}
#else
#ifdef NOFB
	if(skinfb != fb)
		memcpy(fb->fb, skinfb->fb, skinfb->varfbsize);
	system("killall -9 xloadimage");

	FILE *fd;
	fd=fopen("titan.png", "w");
	fwrite(fb->fb,fb->varfbsize,1,fd);
	fclose(fd);

	system("fbgrab -f titan.png -w 1280 -h 720 -b 32 titan1.png > /dev/null");
	system("xloadimage titan1.png > /dev/null &");
#endif
#endif
}

void changefbresolution(char *value)
{
	if(ostrcmp("pal", value) == 0)
	{
		fb->width = 720;
		fb->height = 576;
		fb->pitch = fb->width * fb->colbytes;
	}
	else if(ostrncmp("576", value, 3) == 0)
	{
		fb->width = 720;
		fb->height = 576;
		fb->pitch = fb->width * fb->colbytes;
	}
	else if(ostrncmp("720", value, 3) == 0)
	{
		fb->width = 1280;
		fb->height = 720;
		fb->pitch = fb->width * fb->colbytes;
	}
	else if(ostrncmp("1080", value, 4) == 0)
	{
		fb->width = 1920;
		fb->height = 1080;
		fb->pitch = fb->width * fb->colbytes;
	}
	if(skinfb == fb)
	{
		skin->width = skinfb->width;
		skin->height = skinfb->height;
		skin->iwidth = skinfb->width;
		skin->iheight = skinfb->height;
		skin->rwidth = skinfb->width;
		skin->rheight = skinfb->height;
	}
	clearfb(fb);
	clearfb(skinfb);
}

void setfbtransparent(int value)
{
#ifndef SIMULATE
	struct stmfbio_var_screeninfo_ex varEx = {0};

	varEx.layerid  = 0;
	varEx.activate = STMFBIO_ACTIVATE_IMMEDIATE;
	varEx.caps = STMFBIO_VAR_CAPS_OPACITY;
	varEx.opacity = value;


	if(ioctl(fb->fd, STMFBIO_SET_VAR_SCREENINFO_EX, &varEx) < 0)
		perr("STMFBIO_SET_VAR_SCREENINFO_EX");
#endif
}

#endif
