#ifndef MIPSELPORT_H
#define MIPSELPORT_H

#ifndef FBIO_BLIT
//#define FBIO_SET_MANUAL_BLIT _IOW('F', 0x20, __u32)
#define FBIO_SET_MANUAL_BLIT _IOW('F', 0x21, __u8)
#define FBIO_BLIT 0x22
#endif

#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#endif

unsigned char *lfb = NULL;
int g_manual_blit = 0;
struct fb_fix_screeninfo fix_screeninfo;
struct fb_var_screeninfo var_screeninfo;

int setmixer(struct dvbdev* node, int left, int right)
{
	return audiosetmixer(node, left, right);
}

int setencoding(struct channel* chnode, struct dvbdev* videonode)
{
	int ret = 1;
	if(chnode == NULL) return ret;
	
	switch(chnode->videocodec)
	{
		case MPEGV:
			ret = videosetstreamtype(videonode, 0);
			break;
		case MPEG4V:
			ret = videosetstreamtype(videonode, 4);
			break;
		case H264:
			ret = videosetstreamtype(videonode, 1);
			break;
		case VC1:
			ret = videosetstreamtype(videonode, 3);
			break;
		default:
			ret = videosetstreamtype(videonode, 0);
	}
	return ret;
}

int videodiscontinuityskip(struct dvbdev* node, int flag)
{
	return 0;
}

void fbsave()
{
}

void fbrestore()
{
}

void setfbvarsize(struct fb* newnode)
{
	if(newnode != NULL)
		newnode->varfbsize = 1920 * 1080 * newnode->colbytes;
//		newnode->varfbsize = 1920 * 1080 * (newnode->colbytes * 8);

}

void enablemanualblit()
{
	unsigned char tmp = 1;
	if (ioctl(fb->fd, FBIO_SET_MANUAL_BLIT, &tmp)<0)
		perror("FBIO_SET_MANUAL_BLIT");
	else
		g_manual_blit = 1;
}

void disablemanualblit()
{
	unsigned char tmp = 0;
	if (ioctl(fb->fd, FBIO_SET_MANUAL_BLIT, &tmp)<0)
		perror("FBIO_SET_MANUAL_BLIT");
	else
		g_manual_blit = 0;
}

void blit()
{
	if (g_manual_blit == 1) {
		if (ioctl(fb->fd, FBIO_BLIT) < 0)
			perror("FBIO_BLIT");
	}
}

int waitvsync()
{
	int c = 0;
	return ioctl(fb->fd, FBIO_WAITFORVSYNC, &c);
}

void fbsetoffset(int x, int y)
{
	struct fb_var_screeninfo var_screeninfo;

	var_screeninfo.xoffset = x;
	var_screeninfo.yoffset = y;

	if(ioctl(fb->fd, FBIOPAN_DISPLAY, &var_screeninfo) < 0)
	{
		perr("FBIOPAN_DISPLAY");
	}
}

//flag 0 = no animation
//flag 1 = animation
void blitfb2(struct fb* fbnode, int flag)
{
	struct fb_var_screeninfo var_screeninfo;

	var_screeninfo.xres_virtual = fb->width;
	var_screeninfo.xres = fb->width;
	var_screeninfo.yres_virtual = fb->height * 2;
	var_screeninfo.yres = fb->height;
	var_screeninfo.height = 0;
	var_screeninfo.width = 0;
	var_screeninfo.xoffset = 0;
	var_screeninfo.yoffset = 0;
	var_screeninfo.bits_per_pixel = fb->colbytes * 8;
	
	switch(fb->colbytes)
	{
		case 2:
			var_screeninfo.transp.offset = 15;
			var_screeninfo.transp.length = 1;
			var_screeninfo.red.offset = 10;
			var_screeninfo.red.length = 5;
			var_screeninfo.green.offset = 5;
			var_screeninfo.green.length = 5;
			var_screeninfo.blue.offset = 0;
			var_screeninfo.blue.length = 5;
			break;
		case 4:
			var_screeninfo.transp.offset = 24;
			var_screeninfo.transp.length = 8;
			var_screeninfo.red.offset = 16;
			var_screeninfo.red.length = 8;
			var_screeninfo.green.offset = 8;
			var_screeninfo.green.length = 8;
			var_screeninfo.blue.offset = 0;
			var_screeninfo.blue.length = 8;
			break;
	}
	
	debug(444, "FB: line_length %d", fix_screeninfo.line_length);
	debug(444, "FB: var_screeninfo.xres %d", var_screeninfo.xres);
	debug(444, "FB: var_screeninfo.yres %d", var_screeninfo.yres);
	debug(444, "FB: var_screeninfo.xres_virt %d", var_screeninfo.xres_virtual);
	debug(444, "FB: var_screeninfo.yres_virt %d", var_screeninfo.yres_virtual);
	debug(444, "FB: var_screeninfo.xoffset %d", var_screeninfo.xoffset);
	debug(444, "FB: var_screeninfo.yoffset %d", var_screeninfo.yoffset);
	debug(444, "FB: var_screeninfo.bits_per_pixel %d", var_screeninfo.bits_per_pixel);
	debug(444, "FB: var_screeninfo.grayscale %d", var_screeninfo.grayscale);

	if(ioctl(fb->fd, FBIOPUT_VSCREENINFO, &var_screeninfo) < 0)
	{
		var_screeninfo.yres_virtual = fb->height;
		if(ioctl(fb->fd, FBIOPUT_VSCREENINFO, &var_screeninfo) < 0)
		{
			perr("FBIOPUT_VSCREENINFO");
		}
		debug(444, "FB: double buffering not available");
	}
	else
	{
		debug(444, "FB: double buffering available!");
	}

	ioctl(fb->fd, FBIOGET_VSCREENINFO, &var_screeninfo);
	if ((var_screeninfo.xres!=fb->width) && (var_screeninfo.yres!=fb->height) && (var_screeninfo.bits_per_pixel!=fb->colbytes))
	{
		debug(444, "SetMode failed: wanted: %dx%dx%d, got %dx%dx%d",
			fb->width, fb->height, fb->colbytes,
			var_screeninfo.xres, var_screeninfo.yres, var_screeninfo.bits_per_pixel);
	}
/*	
	int xRes, yRes, stride, bpp;
	
	xRes=var_screeninfo.xres;
	yRes=var_screeninfo.yres;
	bpp=var_screeninfo.bits_per_pixel;
//	fb_fix_screeninfo fix;
//	struct fb_fix_screeninfo fix_screeninfo;
	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fix_screeninfo)<0)
	{
		perror("FBIOGET_FSCREENINFO");
		printf("fb failed\n");
	}
	stride=fix_screeninfo.line_length;
	memset(lfb, 0, stride*yRes);
*/
	blit();
	
/*
	int i = 0, max = 1, wstep = 0, hstep = 0, ret = 0;
	unsigned char buf[10];

	if(fbnode == NULL) return;
#ifndef NOHWBLIT
	if(fbnode == fb) return;

	if(status.rguidfd > -1)
	{
		m_lock(&status.accelfbmutex, 16);
		
		int zlen = 0;
		char* zbuf = NULL;
		blitscale(0, 0, fbnode->width, fbnode->height, 440, 330, 1);
		ret = ozip((char*)accelfb->fb, 440 * 330 * 4, &zbuf, &zlen, 1);

		if(ret == 0)
		{
			memset(buf, 0, 10);
			char* tmpnr = oitoa(zlen);
			memcpy(buf, tmpnr, strlen(tmpnr));
			free(tmpnr); tmpnr = NULL;
			socksend(&status.rguidfd, buf, 10, 5000 * 1000);
			socksend(&status.rguidfd, (unsigned char*)zbuf, zlen, 5000 * 1000);
		}
		free(zbuf); zbuf = NULL;
		zlen = 0;

		m_unlock(&status.accelfbmutex, 16);
	}

	if(status.write_png == 1 && status.infobaraktiv == 0)
	{
		m_lock(&status.accelfbmutex, 16);
		blitscale(0, 0, fbnode->width, fbnode->height, 320, 240, 1);
		if(writeFBfile.ActBuf == NULL)
		{
			writeFBfile.buf1 = malloc(4 * 320 * 240);
			writeFBfile.ActBuf = writeFBfile.buf1;
			memcpy(writeFBfile.buf1, accelfb->fb, 4 * 320 * 240); 
			addtimer(&fb2png_thread, START, 10000, 1, NULL, NULL, NULL);
		}
		else if(writeFBfile.buf1 == writeFBfile.ActBuf)
		{
			if(writeFBfile.buf2 == NULL)
				writeFBfile.buf2 = malloc(4 * 320 * 240);
			memcpy(writeFBfile.buf2, accelfb->fb, 4 * 320 * 240);
		}
		else if(writeFBfile.buf2 == writeFBfile.ActBuf)
		{
			if(writeFBfile.buf1 == NULL)
				writeFBfile.buf1 = malloc(4 * 320 * 240);
			memcpy(writeFBfile.buf1, accelfb->fb, 4 * 320 * 240);
		}
		m_unlock(&status.accelfbmutex, 16);
	}

	int mode3d = 0, leftoffset = 0, rightoffset = 0, topoffset = 0, bottomoffset = 0;
	char* mode3dstr = NULL;

	if(status.leftoffset != 0) blitrect(0, 0, status.leftoffset, fb->height, 0, 255, 2);
	if(status.rightoffset != 0) blitrect(fb->width - status.rightoffset, 0, status.rightoffset, fb->height, 0, 255, 2);
	if(status.topoffset != 0) blitrect(0, 0, fb->width, status.topoffset, 0, 255, 2);
	if(status.bottomoffset != 0) blitrect(0, fb->height - status.bottomoffset, fb->width, status.bottomoffset, 0, 255, 2);

	mode3dstr = getconfig("av_mode3d", NULL);
	if(ostrcmp(mode3dstr, "sbs") == 0)
		mode3d = 1;
	else if(ostrcmp(mode3dstr, "tab") == 0)
		mode3d = 2;

	STMFBIO_BLT_DATA  bltData;
	memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));

	bltData.operation  = BLT_OP_COPY;
	bltData.srcOffset  = fbnode->fb - fb->fb;
	bltData.srcPitch   = fbnode->pitch;
	bltData.src_top    = 0;
	bltData.src_left   = 0;
	bltData.src_right  = fbnode->width;
	bltData.src_bottom = fbnode->height;
	bltData.srcFormat  = SURF_BGRA8888;
	bltData.srcMemBase = STMFBGP_FRAMEBUFFER;

	bltData.dstOffset  = 0;
	bltData.dstPitch   = fb->pitch;
	bltData.dst_left   = status.leftoffset;
	bltData.dst_top    = status.topoffset;
	if(mode3d == 1)
		bltData.dst_right = (fb->width - status.rightoffset) / 2;
	else
		bltData.dst_right = fb->width - status.rightoffset;
	if(mode3d == 2)
		bltData.dst_bottom = (fb->height - status.bottomoffset) / 2;
	else
		bltData.dst_bottom = fb->height - status.bottomoffset;
	bltData.dstFormat  = SURF_BGRA8888;
	bltData.dstMemBase = STMFBGP_FRAMEBUFFER;

	if(flag == 1 && status.screenanim > 0 && mode3d == 0)
	{
		int width = (fb->width - status.rightoffset) - status.leftoffset;
		int height = (fb->height - status.topoffset) - status.bottomoffset;
		max = 25;
		if(status.screenanim == 1 || status.screenanim == 3)
		{
			bltData.dst_left = (width / 2) - 1;
			bltData.dst_right = (width / 2) + 1;
		}
		if(status.screenanim == 2 || status.screenanim == 3)
		{
			bltData.dst_top = (height / 2) - 1;
			bltData.dst_bottom = (height / 2) + 1;
		}
		wstep = width / max;
		hstep = height / max;
	}

	for(i = 0; i < max; i++)
	{

		if(status.screenanim == 1 || status.screenanim == 3)
		{
			int tmpleft = bltData.dst_left - wstep;
			int tmpright = bltData.dst_right + wstep;
			if(tmpleft < status.leftoffset)
				tmpleft = status.leftoffset;
			if(tmpright > fb->width - status.rightoffset)
				tmpright = fb->width - status.rightoffset;
			bltData.dst_left = tmpleft;
			bltData.dst_right = tmpright;
		}

		if(status.screenanim == 2 || status.screenanim == 3)
		{
			int tmptop = bltData.dst_top - hstep;
			int tmpbottom = bltData.dst_bottom + hstep;
			if(tmptop < status.topoffset)
				tmptop = status.topoffset;
			if(tmpbottom > fb->height - status.bottomoffset)
				tmpbottom = fb->height - status.bottomoffset;
			bltData.dst_top = tmptop;
			bltData.dst_bottom = tmpbottom;
		}

		if(status.screenanim > 0) usleep(status.screenanimspeed * 1000);

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
				bltData.dst_left = 0 + status.leftoffset + ((fb->width - status.rightoffset) / 2);
			if(mode3d == 2)
				bltData.dst_top = 0 + status.topoffset + ((fb->height - status.bottomoffset) / 2);
			bltData.dst_right  = fb->width - status.rightoffset;
			bltData.dst_bottom = fb->height - status.bottomoffset;

			if (ioctl(fb->fd, STMFBIO_BLT, &bltData) < 0)
			{
				perr("ioctl STMFBIO_BLT");
			}
			if(ioctl(fb->fd, STMFBIO_SYNC_BLITTER) < 0)
			{
				perr("ioctl STMFBIO_SYNC_BLITTER");
			}
		}
	}
#else
#ifdef NOFB
	if(status.rguidfd > -1)
	{
		char* buf = NULL;
		buf = scale(fbnode->fb, fbnode->width, fbnode->height, 4, 320, 240, 0);
		if(buf != NULL)
		{
			socksend(&status.rguidfd, (unsigned char*)buf, 320 * 240 * 4, 5000 * 1000);
			free(buf); buf = NULL;
		}
	}
	if(status.write_png == 1 && status.infobaraktiv == 0)
	{
		char* buf = NULL;
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
#endif
*/
}

void setfbtransparent(int value)
{
/*
#ifndef SIMULATE
	struct stmfbio_var_screeninfo_ex varEx = {0};

	varEx.layerid  = 0;
	varEx.activate = STMFBIO_ACTIVATE_IMMEDIATE;
	varEx.caps = STMFBIO_VAR_CAPS_OPACITY;
	varEx.opacity = value;


	if(ioctl(fb->fd, STMFBIO_SET_VAR_SCREENINFO_EX, &varEx) < 0)
		perr("STMFBIO_SET_VAR_SCREENINFO_EX");
#endif
*/
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
	int y, x;
	unsigned long tmpcol;
	struct fb* tmpfb = NULL;
	
	if(posx < 0) posx = 0;
	if(posy < 0) posy = 0;
	if(mode < 2)
		tmpfb = skinfb;
	else
		tmpfb = fb;
		
	if(posx > tmpfb->width) posx = tmpfb->width;
	if(posy > tmpfb->height) posy = tmpfb->height;
	if(posx + width > tmpfb->width) width = tmpfb->width - posx;
	if(posy + height > tmpfb->height) height = tmpfb->height - posy;

	if(width <= 0 || height <= 0) return;

	transparent = (transparent - 255) * -1;
	tmpcol = color | ((transparent & 0xff) << 24);

	if(mode == 0 || mode == 2)
	{
		int yend = (posy + height) * tmpfb->width;
		posy *= tmpfb->width;
		int xend = posx + width;
		int xlen = (xend - posx) * tmpfb->colbytes;
		int r = 0;
		unsigned char* from = tmpfb->fb + (posy + posx) * tmpfb->colbytes;

		for(y = posy; y < yend; y += tmpfb->width)
		{
			if(r == 0)
			{
				r = 1;
				for(x = posx; x < xend; x++)
					drawpixelfastfb(tmpfb, x, y, tmpcol);
			}
			else
				memcpy(tmpfb->fb + (y + posx) * tmpfb->colbytes, from, xlen);
		}
	}
	else if(mode == 1 || mode == 3)
	{
		//topline
		for(x = 0; x < width; x++)
			drawpixelfb(tmpfb, posx + x, posy, tmpcol);
		//bottomline
		for(x = 0; x < width; x++)
			drawpixelfb(tmpfb, posx + x, posy + height - 1, tmpcol);
		//leftline
		for(y = 0; y < height; y++)
			drawpixelfb(tmpfb, posx, posy + y, tmpcol);
		//rightline
		for(y = 0; y < height; y++)
			drawpixelfb(tmpfb, posx + width - 1, posy + y, tmpcol);
	}

/*
	unsigned long tmpcol;

	transparent = (transparent - 255) * -1;
	tmpcol = color | ((transparent & 0xff) << 24);

	STMFBIO_BLT_DATA  bltData;
	memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));

	if(posx < 0) posx = 0;
	if(posy < 0) posy = 0;
	if(mode < 2)
	{
		if(posx > skinfb->width) posx = skinfb->width;
		if(posy > skinfb->height) posy = skinfb->height;
		if(posx + width > skinfb->width) width = skinfb->width - posx;
		if(posy + height > skinfb->height) height = skinfb->height - posy;
	}
	else
	{
		if(posx > fb->width) posx = fb->width;
		if(posy > fb->height) posy = fb->height;
		if(posx + width > fb->width) width = fb->width - posx;
		if(posy + height > fb->height) height = fb->height - posy;
	}

	if(width <= 0 || height <= 0) return;

	if(mode == 0 || mode == 2)
		bltData.operation  = BLT_OP_FILL;
	else if(mode == 1 || mode == 3)
		bltData.operation  = BLT_OP_DRAW_RECTANGLE;
	bltData.colour     = tmpcol;
	bltData.srcFormat  = SURF_ARGB8888;
	bltData.srcMemBase = STMFBGP_FRAMEBUFFER;

	if(status.usedirectfb == 1)
		bltData.dstOffset  = 0;
	else
	{
		if(mode < 2)
			bltData.dstOffset  = fb->varfbsize;
		else
			bltData.dstOffset  = 0;
	}
	if(mode < 2)
		bltData.dstPitch   = skinfb->pitch;
	else
		bltData.dstPitch   = fb->pitch;
	bltData.dst_top    = posy;
	bltData.dst_left   = posx;
	bltData.dst_bottom = posy + height;
	bltData.dst_right  = posx + width;
	bltData.dstFormat  = SURF_ARGB8888;
	bltData.dstMemBase = STMFBGP_FRAMEBUFFER;

	if(ioctl(fb->fd, STMFBIO_BLT, &bltData) < 0)
	{
		perr("ioctl STMFBIO_BLT");
	}
	if(ioctl(fb->fd, STMFBIO_SYNC_BLITTER) < 0)
	{
		perr("ioctl STMFBIO_SYNC_BLITTER");
	}
*/
}

int readjpg(const char* filename, unsigned long* width, unsigned long* height, unsigned long* rowbytes, int* channels, unsigned char **mem, int *memfd)
{
/*
#ifndef SIMULATE
	FILE *fp;
	unsigned int temp1, temp2;

	if(filename == NULL) return -1;

	debug(200, "hardware decode picture (%s)...", filename);

	if (!(fp = fopen(filename, "rb")))
		return -1;
	 
	if(get_jpeg_img_size(fp, &temp1, &temp2) != LIBMMEIMG_SUCCESS)
		return -1;
	
	*width = temp1;
	*height = temp2;
	*channels = 3;
	*rowbytes = *channels * temp1;
	
	if(allocbpamem(temp1 * temp2 * (*channels), memfd, mem) != 0)
	{
		fclose(fp);
		return -1;
	}
		
	if(decode_jpeg_noalloc(fp, temp1, temp2, temp1, temp2, (char *)*mem, 1) == LIBMMEIMG_SUCCESS)
	{
		fclose(fp);
		return 0;
	}
	
	fclose(fp);
	freebpamem(*memfd, *mem, temp1 * temp2 * (*channels));
	err("hardware decode error");
#endif
	return -1;
*/
}

//flag 0: blit from accelfb to skinfb
//flag 1: blit from skinfb to accelfb
void blitscale(int posx, int posy, int width, int height, int scalewidth, int scaleheight, int flag)
{
/*
#ifndef SIMULATE
	STMFBIO_BLT_DATA  blt_data;
	memset(&blt_data, 0, sizeof(STMFBIO_BLT_DATA));

	if(scalewidth == 0) scalewidth = width;
	if(scaleheight == 0) scaleheight = height;

	if(posx < 0) posx = 0;
	if(posx > skinfb->width) posx = skinfb->width;
	if(posy < 0) posy = 0;
	if(posy > skinfb->height) posy = skinfb->height;
	if(posx + scalewidth > skinfb->width) scalewidth = skinfb->width - posx;
	if(posy + scaleheight > skinfb->height) scaleheight = skinfb->height - posy;
	
	if(width <= 0 || height <= 0 || scalewidth <= 0 || scaleheight <= 0) return;
	
	if(flag == 1 && (scalewidth * scaleheight * 4) > accelfb->varfbsize)
	{
		err("accelfb to small %d -> %lu ", scalewidth * scaleheight * 4, accelfb->varfbsize);
		return;
	}

	blt_data.operation  = BLT_OP_COPY;
	
	if(flag == 0)
	{
		if(status.usedirectfb == 1)
			blt_data.srcOffset  = fb->varfbsize;
		else
			blt_data.srcOffset  = fb->varfbsize + skinfb->varfbsize;
	}
	else
	{
		if(status.usedirectfb == 1)
			blt_data.srcOffset  = 0;
		else
			blt_data.srcOffset  = fb->varfbsize;
	}
	
	blt_data.srcPitch   = width * 4;
	blt_data.src_top    = 0;
	blt_data.src_left   = 0;
	blt_data.src_right  = width;
	blt_data.src_bottom = height;
	blt_data.srcFormat  = SURF_ARGB8888;
	blt_data.srcMemBase = STMFBGP_FRAMEBUFFER;
	
	if(flag == 0)
	{
		if(status.usedirectfb == 1)
		{
			blt_data.dstOffset  = 0;
			blt_data.dstPitch   = fb->pitch;
		}
		else
		{
			blt_data.dstOffset  = fb->varfbsize;
			blt_data.dstPitch   = skinfb->pitch;
		}
	}
	else
	{
		if(status.usedirectfb == 1)
		{
			blt_data.dstOffset  = fb->varfbsize;
			blt_data.dstPitch   = skinfb->pitch;
		}
		else
		{
			blt_data.dstOffset  = fb->varfbsize + skinfb->varfbsize;
			blt_data.dstPitch   = scalewidth * 4;
		}
	}
	
	blt_data.dst_left   = posx;
	blt_data.dst_top    = posy;
	blt_data.dst_right  = posx + scalewidth;
	blt_data.dst_bottom = posy + scaleheight;
	blt_data.dstFormat  = SURF_ARGB8888;
	blt_data.dstMemBase = STMFBGP_FRAMEBUFFER;
	
	if (ioctl(fb->fd, STMFBIO_BLT, &blt_data) < 0)
	{
		perr("ioctl STMFBIO_BLT");
	}
	if(ioctl(fb->fd, STMFBIO_SYNC_BLITTER) < 0)
	{
		perr("ioctl STMFBIO_SYNC_BLITTER");
	}
#endif
*/
}

void blitjpg(unsigned char* buf, int posx, int posy, int width, int height, int scalewidth, int scaleheight, int mwidth, int mheight, int halign, int valign)
//void blitjpg(unsigned char* buf, int posx, int posy, int width, int height, int scalewidth, int scaleheight)
{
/*
#ifndef SIMULATE
	STMFBIO_BLT_EXTERN_DATA blt_data;
	memset(&blt_data, 0, sizeof(STMFBIO_BLT_EXTERN_DATA));

	blt_data.operation  = BLT_OP_COPY;
	blt_data.ulFlags    = 0;
	blt_data.srcOffset  = 0;
	blt_data.srcPitch   = width * 3;
	blt_data.dstOffset  = 0;
	blt_data.dstPitch   = skinfb->pitch;
	blt_data.src_top    = 0;
	blt_data.src_left   = 0;
	blt_data.src_right  = width;
	blt_data.src_bottom = height;
	blt_data.dst_left   = posx;
	blt_data.dst_top    = posy;
	
	if(scalewidth == 0)  scalewidth = width;
	if(scaleheight == 0) scaleheight = height;
	
	blt_data.dst_right  = posx + scalewidth;
	blt_data.dst_bottom = posy + scaleheight;
	blt_data.srcFormat  = SURF_BGR888;
	blt_data.dstFormat  = SURF_ARGB8888;
	blt_data.srcMemBase = (char *)buf;
	blt_data.dstMemBase = (char *)skinfb->fb;
	blt_data.srcMemSize = width * height * 3;
	blt_data.dstMemSize = skinfb->pitch * skinfb->height;
	
	if(ioctl(skinfb->fd, STMFBIO_BLT_EXTERN, &blt_data) < 0)
	{
		err("ioctl STMFBIO_BLT_EXTERN");
	}
	if(ioctl(fb->fd, STMFBIO_SYNC_BLITTER) < 0)
	{
		perr("ioctl STMFBIO_SYNC_BLITTER");
	}
#endif
*/
}

void initsignal(struct sigaction* sa)
{
	sigaction(SIGILL, sa, NULL);
	sigaction(SIGBUS, sa, NULL);
	sigaction(SIGFPE, sa, NULL);
	sigaction(SIGUSR1, sa, NULL);
	sigaction(SIGSEGV, sa, NULL);
	sigaction(SIGUSR2, sa, NULL);
	sigaction(SIGPIPE, sa, NULL);
	sigaction(SIGALRM, sa, NULL);
//	sigaction(SIGSTKFLT, sa, NULL);
	sigaction(SIGABRT, sa, NULL);

	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTRAP, SIG_IGN);
	signal(SIGXCPU, SIG_IGN);
	signal(SIGXFSZ, SIG_IGN);
	signal(SIGVTALRM, SIG_IGN);
	signal(SIGPROF, SIG_IGN);
	signal(SIGPOLL, SIG_IGN);
	signal(SIGRTMAX, SIG_IGN);
	signal(SIGPWR, SIG_IGN);
	signal(SIGSYS, SIG_IGN);
}

void sighandler(int sig, struct sigcontext ctx)
{
	switch(sig)
	{
		case SIGALRM:
		{
			err("got signal sigalrm but ignore it");
			break;
		}
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
		case SIGUSR2: //called if hanging mainthread detect
		{
			debugstack(sig, NULL, NULL);
			break;
		}
		case SIGILL:
		case SIGBUS:
		case SIGFPE:
		case SIGSEGV:
//		case SIGSTKFLT:
		{
/*
#ifdef SIMULATE
			//intel
			debugstack((void *)ctx.eip, NULL);
			err("got signal %d, fault address 0x%lx from 0x%lx", sig, ctx.cr2, ctx.eip);
#else
			// sh4
			//unsigned long sc_regs[16];
			//unsigned long sc_pc; //programm counter register
			//unsigned long sc_pr; //procedure register
			//unsigned long sc_sr; //status register
			//unsigned long sc_gbr;
			//unsigned long sc_mach;
			//unsigned long sc_macl;

			debugstack(sig, (void *)ctx.sc_pr, (void *)ctx.sc_pc);
			err("got signal %d (%s), programm counter reg: 0x%lx,  procedure reg: 0x%lx", sig, strsignal(sig), ctx.sc_pc, ctx.sc_pr);
#endif
*/
			if(getconfigint("saverun", NULL) == 1 && status.longjumpbuf != NULL)
				siglongjmp(status.longjumpbuf, 999);
			else
				exit(100);
			break;
		}
	}
}

int setvmpeg(struct dvbdev* node, int value, int flag)
{
	debug(4440, "in");
	char* vmpegdev = NULL, *tmpstr = NULL, *buf = NULL;
	int ret = 0;

	if(node == NULL) return 1;
	if(flag == 0) vmpegdev = getconfig("vmpegleftdev", NULL);
	if(flag == 1) vmpegdev = getconfig("vmpegtopdev", NULL);
	if(flag == 2) vmpegdev = getconfig("vmpegwidthdev", NULL);
	if(flag == 3) vmpegdev = getconfig("vmpegheightdev", NULL);

	if(vmpegdev != NULL)
	{
		buf = malloc(MINMALLOC);
		if(buf == NULL)
		{
			err("no mem");
			return 1;
		}
		
		tmpstr = malloc(10);
		if(tmpstr == NULL)
		{
			err("no mem");
			free(buf);
			return 1;
		}
		
		snprintf(buf, MINMALLOC, vmpegdev, node->devnr);
		snprintf(tmpstr, 10, "%x", value);
		debug(444, "set %s to %s", buf, tmpstr);
		status.tvpic = 1;
		ret = writesys(buf, tmpstr, 1);
		
		free(tmpstr);
		free(buf);
		return ret;
	}

	debug(4440, "out");
	return 0;
}

//flag 0: wh = width
//flag 1: wh = height
int setvmpegrect(struct dvbdev* node, int left, int top, int wh, int flag)
{
	int ret = 0;
	
	if(flag == 0)
	{
		ret = setvmpeg(node, wh, 2);
		ret = setvmpeg(node, wh / 1.4, 3);
	}
	if(flag == 1)
	{
		ret = setvmpeg(node, wh, 3);
		ret = setvmpeg(node, wh * 1.3, 2);
	}
		
	ret = setvmpeg(node, left, 0);
	ret = setvmpeg(node, top, 1);
	
	return ret;
}

int resetvmpeg(struct dvbdev* node)
{
	int ret = 0;

	ret = setvmpeg(node, 0, 0);
	ret = setvmpeg(node, 0, 1);
	ret = setvmpeg(node, 0, 2);
	ret = setvmpeg(node, 0, 3);
	
	return ret;
}

int resettvpic()
{
/*
	if(status.tvpic == 1 && status.aktservice != NULL)
	{
		status.tvpic = 0;
		resetvmpeg(status.aktservice->videodev);
	}
*/
	int ret = 0;

	if(status.tvpic > 0 && status.aktservice != NULL)
	{
		status.tvpic = 0;
		ret = resetvmpeg(status.aktservice->videodev);
	}

	return ret;
}

int cagetslotinfo(struct dvbdev* node, ca_slot_info_t* info)
{
	int ret = 0;
	
	if(node == NULL || info == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	struct pollfd fds;
	
	fds.fd = node->fd;
	fds.events = POLLPRI | POLLIN;
	
	info->flags = 0;
	
	ret = TEMP_FAILURE_RETRY(poll(&fds, 1, 300));
	
	if(ret < 0)
	{
		err("poll data");
		return 1; //error
	}
	else if(ret == 0)
		return 0; //timeout
	else if(ret > 0)
	{
		if(fds.revents & POLLIN)
			info->flags = 0;
		if(fds.revents & POLLPRI)
			info->flags = CA_CI_MODULE_READY;
	}

	return 0;
}

#endif
