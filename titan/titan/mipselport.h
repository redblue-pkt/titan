#ifndef MIPSELPORT_H
#define MIPSELPORT_H

#ifdef CONFIG_ION

//#include <interfaces/ion.h>
#define ION_HEAP_TYPE_BMEM      (ION_HEAP_TYPE_CUSTOM + 1)
#define ION_HEAP_ID_MASK        (1 << ION_HEAP_TYPE_BMEM)
#define ACCEL_MEM_SIZE          (32*1024*1024)

#elif !defined(FBIO_BLIT)

//#define FBIO_SET_MANUAL_BLIT _IOW('F', 0x20, __u32)
#define FBIO_SET_MANUAL_BLIT _IOW('F', 0x21, __u8)
#define FBIO_BLIT 0x22

#endif

#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#endif

//fuer Funktion memcpy_word
volatile char *memcpy_word_src;
volatile char *memcpy_word_dest;
volatile long  memcpy_word_anzw = 0;

//fuer Funktion memcpy_byte
volatile char *memcpy_byte_src;
volatile char *memcpy_byte_dest;
volatile long  memcpy_byte_anzb = 0;

unsigned char *lfb = NULL;
int g_manual_blit = 0;
struct fb_fix_screeninfo fix_screeninfo;
struct fb_var_screeninfo var_screeninfo;

void memcpy_area(unsigned char* targetADDR, unsigned char* startADDR, long pxAbs, long hight, long widthAbs, long FBwidthAbs);

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
		case H265:
			ret = videosetstreamtype(videonode, H265);
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
#ifndef CONFIG_ION		
		newnode->varfbsize = 1920 * 1080 * newnode->colbytes;
//		newnode->varfbsize = 1920 * 1080 * (newnode->colbytes * 8);
#else
	newnode->varfbsize = newnode->width * newnode->height * newnode->colbytes;
#endif

}

void enablemanualblit()
{
#ifndef CONFIG_ION
	unsigned char tmp = 1;
	if (ioctl(fb->fd, FBIO_SET_MANUAL_BLIT, &tmp)<0)
		perror("FBIO_SET_MANUAL_BLIT");
	else
		g_manual_blit = 1;
#endif
}

void disablemanualblit()
{
#ifndef CONFIG_ION
	unsigned char tmp = 0;
	if (ioctl(fb->fd, FBIO_SET_MANUAL_BLIT, &tmp)<0)
		perror("FBIO_SET_MANUAL_BLIT");
	else
		g_manual_blit = 0;
#endif
}

void blit()
{
#if !defined(CONFIG_ION)
	if (g_manual_blit == 1) {
		if (ioctl(fb->fd, FBIO_BLIT) < 0)
			perror("FBIO_BLIT");
	}
#endif
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
	int doblit = 1;
	struct fb_var_screeninfo var_screeninfo;

	var_screeninfo.xres_virtual = fb->width;
	var_screeninfo.xres = fb->width;
//	if(checkbox("VUSOLO2") == 0)
		var_screeninfo.yres_virtual = fb->height * 2;
//	else
//		var_screeninfo.yres_virtual = fb->height;
	var_screeninfo.yres = fb->height;
	var_screeninfo.height = 0;
	var_screeninfo.width = 0;
	var_screeninfo.xoffset = 0;
	var_screeninfo.yoffset = 0;
//	if(checkbox("VUSOLO2") == 0)
		var_screeninfo.bits_per_pixel = fb->colbytes * 8;
//	else
//		var_screeninfo.bits_per_pixel = fb->colbytes;
	
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
	
	if(checkrealbox("HD51") == 0 && checkbox("DM7020HD") == 0 && checkbox("DM7020HDV2") == 0 && checkbox("DM900") == 0 && checkbox("DM920") == 0 && checkbox("WETEK") == 0 && checkbox("DM520") == 0 && checkbox("DM525") == 0 )
	{
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
	}
	
	int posx = 0;
	int posy = 0;
	int width = 0;
	int height = 0;
	
	if(status.bcm == 1 && status.usedirectfb == 0)
	{
		posx = getconfigint("fbleftoffset", NULL) * 5;
		posy = getconfigint("fbtopoffset", NULL) * 5;
		width = (fb->width - posx) - (getconfigint("fbrightoffset", NULL) * 5);
		height = (fb->height - posy) - (getconfigint("fbbottomoffset", NULL) * 5);
	}
	else
	{
		posx = getconfigint("fbleftoffset", NULL);
		posy = getconfigint("fbtopoffset", NULL);
		width = (720 - posx) - (getconfigint("fbrightoffset", NULL));
		height = (576 - posy) - (getconfigint("fbbottomoffset", NULL));
	}
	//printf("posx:%i posy:%i width:%i height:%i\n", posx, posy, width, height);

	if(flag == 0 || checkbox("ATEMIO5200") == 1 || status.screenanim == 0)
	{
		if(status.bcm == 1 && status.usedirectfb == 0)
			bcm_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0);
#ifdef HAVE_HISILICON_ACCEL
		else if(status.usedirectfb == 0)
			dinobot_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0, 0);
#endif
		blit();
		
		//woraround pixel fehler
		if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
		{
			bcm_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0);
#ifdef HAVE_HISILICON_ACCEL
			dinobot_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0, 0);
#endif
			blit();
		}
		
		return;
	}
	

	int i = 0, max = 1, wstep = 0, hstep = 0;
	int dst_left = 0, dst_width = 0, dst_top = 0, dst_height = 0;
	int mode3d = 0;

	if(flag == 1 && status.screenanim > 0 && mode3d == 0)
	{
		doblit = 0;
		//max = 25;
		max = 8;
		dst_left = posx;
		dst_width = width;
		dst_top = posy;
		dst_height = height;
		
		char* fbleftdev = "/proc/stb/fb/dst_left";
		char* fbwidthdev = "/proc/stb/fb/dst_width";
		char* fbtopdev = "/proc/stb/fb/dst_top";
		char* fbheightdev = "/proc/stb/fb/dst_height";
		
		int screenanimspeed = 0;	
		
		if(status.bcm == 1 && status.usedirectfb == 0)
		{
			if(status.screenanimspeed == 1 || status.screenanimspeed == 0)
				max = 4;
			else if(status.screenanimspeed == 5)
				max = 8;
			else
				screenanimspeed = status.screenanimspeed;
		}
		else
			screenanimspeed = status.screenanimspeed;
			
		if(status.screenanim == 1 || status.screenanim == 3)
		{
			dst_left = (width / 2) - 1;
			dst_width = 2;
		}
		if(status.screenanim == 2 || status.screenanim == 3)
		{
			dst_top = (height / 2) - 1;
			dst_height = 2;
		}
		if(status.screenanim == 4 || status.screenanim == 11)
		{
			dst_top = posy;
			dst_height = height;
			dst_left = posx;
			dst_width = 2;
		}
		if(status.screenanim == 5 || status.screenanim == 12)
		{
			dst_top = posy;
			dst_height = 2;
			dst_left = posx;
			dst_width = width;
		}
		
		wstep = width / max;
		hstep = height / max;
			
		for(i = 0; i <= max; i++)
		{
			if(status.screenanim == 1 || status.screenanim == 3)
			{
				int tmpleft = dst_left - (wstep/2);
				int tmpwidth = dst_width + wstep;
				if(tmpleft < posx)
					tmpleft = posx;
				if(tmpwidth > width)
					tmpwidth = width;
				dst_left = tmpleft;
				dst_width = tmpwidth;
			}
			if(status.screenanim == 2 || status.screenanim == 3)
			{
				int tmptop = dst_top - (hstep/2);
				int tmpheight = dst_height + hstep;
				if(tmptop < posy)
					tmptop = posy;
				if(tmpheight > height)
					tmpheight = height;
				dst_top = tmptop;
				dst_height = tmpheight;
			}
			if(status.screenanim == 4 || status.screenanim == 11)
			{
				int tmpwidth = dst_width + wstep;
				if(tmpwidth > width)
					tmpwidth = width;
				dst_width = tmpwidth;
			}
			if(status.screenanim == 5 || status.screenanim == 12)
			{
				int tmpheight = dst_height + hstep;
				if(tmpheight > height)
					tmpheight = height;
				dst_height = tmpheight;
			}
			dst_width = dst_width+posx;
			dst_height = dst_height+posy;
			if((dst_width + dst_left) > width)
				dst_width = dst_width - dst_left;
							
			if((dst_height + dst_top) > height)
				dst_height = dst_height - dst_top;
								
			//printf("left:%i width:%i top:%i height:%i\n", dst_left, dst_width, dst_top, dst_height);
			//waitvsync();
			if(status.bcm == 1 && status.usedirectfb == 0)
			{
				if(status.screenanim < 10)
					bcm_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, dst_left, dst_top, dst_width, dst_height, 0, 0);
#ifdef HAVE_HISILICON_ACCEL
				if(status.screenanim < 10)
					dinobot_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, dst_left, dst_top, dst_width, dst_height, 0, 0, 0);
#endif
				else
				{
					int dswidth = (dst_width + posx*2) * skinfb->width / (width + posx*2);
					int dsheight = (dst_height + posy*2) * skinfb->height / (height + posy*2);
					int dsleft = skinfb->width - dswidth;
					int dstop = skinfb->height - dsheight;
					if(status.screenanim == 11)
						dstop = 0;
					if(status.screenanim == 12)
						dsleft = 0;
					bcm_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, dsleft, dstop, dswidth, dsheight, dst_left, dst_top, dst_width, dst_height, 0, 0);
#ifdef HAVE_HISILICON_ACCEL
					dinobot_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, dsleft, dstop, dswidth, dsheight, dst_left, dst_top, dst_width, dst_height, 0, 0, 0);
#endif
				}	
			}	
			else
			{
				setfbosddev(fbleftdev, dst_left);
				setfbosddev(fbwidthdev, dst_width);
				setfbosddev(fbtopdev, dst_top);
				setfbosddev(fbheightdev, dst_height);
				usleep(1000);
			}
			if(status.screenanim > 0) usleep(status.screenanimspeed * 1000);
			blit();
		}
		if(status.bcm == 1 && status.usedirectfb == 0)
		{
			bcm_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0);
		}
#ifdef HAVE_HISILICON_ACCEL
		else if(status.usedirectfb == 0)
			dinobot_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0, 0);
#endif
		else
		{
			setfbosddev(fbleftdev, posx);
			setfbosddev(fbwidthdev, width);
			setfbosddev(fbtopdev, posy);
			setfbosddev(fbheightdev, height);
		}
		blit();
	}
	else
	{
		if(status.bcm == 1 && status.usedirectfb == 0)
			bcm_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0);
#ifdef HAVE_HISILICON_ACCEL
		else if(status.usedirectfb == 0)
			dinobot_accel_blit(skinfb->data_phys, skinfb->width, skinfb->height, skinfb->pitch, 0, fb->data_phys, fb->width, fb->height, fb->pitch, 0, 0, skinfb->width, skinfb->height, posx, posy, width, height, 0, 0, 0);
#endif
	}
	if(doblit == 1)
		blit();
	
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
//	blit();
	
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
	char* transparentdev;

	transparentdev = getconfig("transparentdev", NULL);

	if(transparentdev != NULL /*&& checkdev(transparentdev)*/)
	{
		debug(100, "set %s to %d", transparentdev, value);
		writesysint(transparentdev, value, 1);
		return;
	}
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
//#ifdef BCM_ACCEL	
		if(status.bcm == 1 && tmpfb->data_phys != 0)
		{
			bcm_accel_fill(tmpfb->data_phys, tmpfb->width, tmpfb->height, tmpfb->pitch, posx, posy, width, height, tmpcol);
		}
//#endif
#ifdef HAVE_HISILICON_ACCEL
		else if(tmpfb->data_phys != 0)
		{
			dinobot_accel_fill(tmpfb->data_phys, tmpfb->width, tmpfb->height, tmpfb->pitch, posx, posy, width, height, tmpcol);
		}
#endif
		else
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
				{
//					printf("befor memcpy_area\n");
					memcpy(tmpfb->fb + (y + posx) * tmpfb->colbytes, from, xlen);
					// return 100 error on branch 3.4
//  					memcpy_area(tmpfb->fb + (y + posx) * tmpfb->colbytes, from, posx * 4, height-1, width*4, tmpfb->width*4);
					//y = yend;
				}
				
			}
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
return -1;
}

//flag 0: blit from accelfb to skinfb
//flag 1: blit from skinfb to accelfb
void blitscale(int posx, int posy, int width, int height, int scalewidth, int scaleheight, int flag)
{
#ifndef SIMULATE
//#ifdef BLITHELP
	unsigned char *source = NULL; 
	unsigned char *target = NULL;
	int zpitch = 0;
	int zheight = 0;
	int zwidth = 0;
	int qpitch = 0;
	int qheight = 0;
	int qwidth = 0;
	unsigned long source_phys = 0;
	unsigned long target_phys = 0;

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
	if(flag == 0)
	{
		source = accelfb->fb;
		source_phys = accelfb->data_phys;
		target_phys = skinfb->data_phys;

		target = skinfb->fb + (posy * skinfb->pitch) + (posx*4);
		zpitch = skinfb->pitch;
		zheight = skinfb->height;
		zwidth = skinfb->width;
		qpitch = width*4;
		qheight = height;
		qwidth = width;
	}
	else
	{
		source = skinfb->fb;
		source_phys = skinfb->data_phys;
		target_phys = accelfb->data_phys;

		target = accelfb->fb + (posy * accelfb->pitch) + (posx*4);
		zpitch = accelfb->pitch;
		zheight = accelfb->height;
		zwidth = accelfb->width;
		qpitch = width*4;
		qheight = height;
		qwidth = width;
	}
	
	if(status.bcm == 1 && source_phys > 0 && target_phys >0)
	{
		bcm_accel_blit(source_phys, qwidth, qheight, qpitch, 0, target_phys, zwidth, zheight, zpitch, 0, 0, width, height, posx, posy, scalewidth, scaleheight, 0, 0);
		flag = 1;
	}
#ifdef HAVE_HISILICON_ACCEL
	else if(source_phys > 0 && target_phys >0)
	{
		dinobot_accel_blit(source_phys, qwidth, qheight, qpitch, 0, target_phys, zwidth, zheight, zpitch, 0, 0, width, height, posx, posy, scalewidth, scaleheight, 0, 0, 0);
		flag = 1;
	}
#endif
	else
	{
		unsigned char *helpbuf = NULL;
		helpbuf = scale(source, width, height, 4, scalewidth, scaleheight, 0);
	
		size_t helpb = 0;
		size_t helpz = 0;
		size_t help = 0;
	
		while(helpz < scaleheight && helpz < (zheight - posy)) {
			//memcpy(target[help], helpbuf[helpb], scalewidth*4);
			memcpy(target+help, helpbuf+helpb, scalewidth*4);
			help = help + zpitch;
			helpb = helpb + scalewidth*4;
			helpz = helpz + 1;
		}
		free(helpbuf);
	}
	
	
	if(flag == 0)
		blit();

//#endif

/*
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
*/
#endif
}

void blitjpg(unsigned char* buf, int posx, int posy, int width, int height, int scalewidth, int scaleheight, int mwidth, int mheight, int halign, int valign)
//void blitjpg(unsigned char* buf, int posx, int posy, int width, int height, int scalewidth, int scaleheight)
{
#ifndef SIMULATE
#ifdef BLITHELP
	unsigned char *helpbuf = NULL;
	unsigned char *framebuf = NULL;

	if(scalewidth != 0 || scaleheight != 0) {
		helpbuf = scale(buf, width, height, 3, scalewidth, scaleheight, 1);
		if(helpbuf == NULL)
			return;
	}
	else {
		helpbuf = buf;
		scalewidth = width;
		scaleheight = height;
	}
	size_t helpb = 0;
	size_t helpz = 0;
	size_t help = 0;
	
	framebuf = skinfb->fb;
	framebuf = framebuf + (posy * skinfb->pitch) + (posx*4);
	
	while(helpz < scaleheight && helpz < (skinfb->height - posy)) {
		help = 0;
		while(help < (scalewidth*4) && help < (skinfb->pitch - (posx*4))) {
			framebuf[help+0] = helpbuf[helpb+2];
			framebuf[help+1] = helpbuf[helpb+1];
			framebuf[help+2] = helpbuf[helpb+0];
			framebuf[help+3] = 0xff;
			help = help + 4;
			helpb = helpb + 3;
		}
		if(help >= (skinfb->pitch - (posx*4)))
			helpb = (helpz+1) * (scalewidth*3);
		framebuf = framebuf + skinfb->pitch;
		helpz = helpz + 1;
	}
	free(helpbuf);
	blit();
#endif
		 
/*
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
*/

#endif
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

	if(getskinconfigint("minitv", NULL) == 1) 
 		return 0;

	if(node == NULL) return 1;
	if(flag == 0)  vmpegdev = getconfig("vmpegleftdev", NULL);
	if(flag == 1)  vmpegdev = getconfig("vmpegtopdev", NULL);
	if(flag == 2)  vmpegdev = getconfig("vmpegwidthdev", NULL);
	if(flag == 3)  vmpegdev = getconfig("vmpegheightdev", NULL);
	if(flag == 99) vmpegdev = getconfig("vmpegapplydev", NULL);

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
//int setvmpegrect(struct dvbdev* node, int left, int top, int wh, int flag)
int setvmpegrect(struct dvbdev* node, int posx, int posy, int wh, int flag)
{
	int ret = 0;
	int leftoffset = status.leftoffset;
	int rightoffset = status.rightoffset;
	int topoffset = status.topoffset;
	int bottomoffset = status.bottomoffset;

	float rx = (float)720 / (float)(720 - leftoffset - rightoffset);
	float ry = (float)576 / (float)(576 - topoffset - bottomoffset);

	posx = (float)posx / rx;
	posx += leftoffset;

	posy = (float)posy / ry;
	posy += topoffset;

	ret = setvmpeg(node, posx, 0);
	ret = setvmpeg(node, posy, 1);
	
	if(flag == 0)
	{
		wh = wh * (720 - leftoffset - rightoffset) / 720;
		ret = setvmpeg(node, wh, 2);
		ret = setvmpeg(node, (int)((float)wh / 1.2), 3);
	}

	if(flag == 1)
	{
		wh = wh * (576 - topoffset - bottomoffset) / 576;
		ret = setvmpeg(node, wh, 3);
		ret = setvmpeg(node, (int)((float)wh * 1.2), 2);
	}
	
	setvmpeg(node, 0, 99); 
	
	return ret;
}
	
int resetvmpeg(struct dvbdev* node)
{
	int ret = 0;

	ret = setvmpeg(node, 0, 0);
	ret = setvmpeg(node, 0, 1);
	ret = setvmpeg(node, 0, 2);
	ret = setvmpeg(node, 0, 3);
	
	ret = setvmpeg(node, 0, 99);

	status.tvpic = 0;
			
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
		ret = resetvmpeg(status.aktservice->videodev);

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
	fds.events = POLLOUT | POLLPRI | POLLIN;
	
	info->flags = 0;
	
	ret = TEMP_FAILURE_RETRY(poll(&fds, 1, 1000));
	
	if(ret < 0)
	{
		err("poll data");
		return 1; //error
	}
	else if(ret == 0)
		return 0; //timeout
	else if(ret > 0)
	{
		if(fds.revents & POLLOUT)
			info->flags = CA_CI_MODULE_READY;
		if(fds.revents & POLLIN)
			info->flags = CA_CI_MODULE_READY;
		if(fds.revents & POLLPRI)
			info->flags = 0;
	}

	return 0;
}

void memcpy_word(char* dest, char* src, long anzw)
{
	// Folgende Werte m�ssen volatile definiert sein 
	// char* memcpy_word_src ---> pointer Quelle
	// char* memcpy_word_dest ---> pointer Ziehl
	// long  memcpy_word_anzw ---> Anzahl der W�rter (4 byte) die kopiert werden sollen.
	
	memcpy_word_src = src;
	memcpy_word_dest = dest;
	memcpy_word_anzw = anzw;

#ifndef ARM	
	asm(	
				"		lw	  $8, memcpy_word_src		\n"
				"		lw	  $9, memcpy_word_dest	\n"				
				"		lw		$10, memcpy_word_anzw	\n"		
				"		addi	$10, $10, -1					\n"
				"loop1:													\n"
				"		lw	  $11, ($8)							\n"
				"		sw	  $11, ($9)							\n"
				"		addi	$8, $8, 4							\n"
				"		addi	$9, $9, 4							\n"
				"		addi	$10, $10, -1					\n" 
				"		bgez	$10, loop1						\n"
			);
#endif

}

void memcpy_byte(char* dest, char* src, long anzb)
{
	// Folgende Werte m�ssen volatile definiert sein 
	// char* memcpy_byte_src ---> pointer Quelle
	// char* memcpy_byte_dest ---> pointer Ziehl
	// long  memcpy_byte_anzb ---> Anzahl bytes die kopiert werden sollen.
	
	memcpy_byte_src = src;
	memcpy_byte_dest = dest;
	memcpy_byte_anzb = anzb;
	
#ifndef ARM	
	asm (	
				"		li    $12, 4								\n"
				"		lw	  $8, memcpy_byte_src		\n"
				"		lw	  $9, memcpy_byte_dest	\n"				
				"		lw		$10, memcpy_byte_anzb	\n"		
				"word:													\n"
				"		bltu	$10, $12, byte				\n"
				"		lw	  $11, ($8)							\n"
				"		sw	  $11, ($9)							\n"
				"		addi	$8, $8, 4							\n"
				"		addi	$9, $9, 4							\n"
				"		addi	$10, $10, -4					\n"
				"		b			word									\n"
				"byte:													\n"
				"		beqz	$10, end 							\n"
				"		lb	  $11, ($8)							\n"
				"		sb	  $11, ($9)							\n"
				"		addi	$8, $8, 1							\n"
				"		addi	$9, $9, 1							\n"
				"		addi	$10, $10, -1					\n"		
				"		b     byte									\n"
				"end:														\n"
				"		nop													\n"	  
			);
#endif
}


void memcpy_area(unsigned char* targetADDR, unsigned char* startADDR, long pxAbs, long hight, long widthAbs, long FBwidthAbs)
{

#ifndef ARM
	asm(	

				"		lw    $t3, %[targetADDR]													\n"
				"		lw    $t4, %[startADDR]													\n"
				"		lw		$t5, %[pxAbs]															\n"
				"		lw    $t6, %[widthAbs]													\n"
				"		lw    $t7, %[FBwidthAbs]												\n"
				"		lw    $t8, %[hight]															\n"

				"		move  $t0, $t4      # Temp-startADDR							\n"  
				"		move  $t1, $t6      # Temp-widthAbs								\n"
				"		add   $t2, $t5, $t6																\n"
				"		sub   $t7, $t7, $t2 # Diff withAbs und FBwidthAbs	\n"

				"p3NewLine2:																					\n"		
				"		beqz  $t8, p3End																	\n"
				"p3NextWord:																					\n"
 				"   beqz  $t1, p3NewLine1															\n"
				"		lw    $t9, ($t0)																	\n"
				"		sw    $t9, ($t3)																	\n"
				"		addi  $t0, 4																			\n"
				"		addi  $t3, 4																			\n"
				"		addi  $t1, -4																			\n"
				"		b     p3NextWord																	\n"

		
				"p3NewLine1:																					\n"
				"		move  $t0, $t4																		\n"
				"		move  $t1, $t6																		\n"
				"		add   $t3, $t3, $t7																\n"
				"		add   $t3, $t3, $t5																\n"
				"		addi  $t8, -1																			\n"
				"		b     p3NewLine2																	\n" 

				"p3End:																								\n"
				"		nop																								\n"	
				::[targetADDR] "m" (targetADDR), [startADDR] "m" (startADDR), [pxAbs] "m" (pxAbs), [widthAbs] "m" (widthAbs), [FBwidthAbs] "m" (FBwidthAbs), [hight] "m" (hight)
				);
#endif
				
	return;
}

int setfbosddev(char* dev, int value)
{
	char* tmpstr = NULL;
	int ret = 0;

	tmpstr = malloc(10);
	if(tmpstr == NULL)
	{
		err("no mem");
		return 1;
	}
	
	sprintf(tmpstr, "%x", value);
	debug(101, "set %s to %s", dev, tmpstr);

	if(dev != NULL)
	{
		ret = writesys(dev, tmpstr, 1);
		return ret;
	}

	return 0;
}

char* getfbosddev(char* dev)
{
	char *value = NULL;

	if(dev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(dev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	debug(101, "get %s to %s", dev, value);
		
	return value;
}

void setfbosdnull()
{
	char* fbleftdev = "/proc/stb/fb/dst_left";
	char* fbwidthdev = "/proc/stb/fb/dst_width";
	char* fbtopdev = "/proc/stb/fb/dst_top";
	char* fbheightdev = "/proc/stb/fb/dst_height";
	char* fbapplydev = "/proc/stb/fb/dst_apply";
	setfbosddev(fbleftdev, 0);
	setfbosddev(fbwidthdev, 720);
	setfbosddev(fbtopdev, 0);
	setfbosddev(fbheightdev, 576);
	setfbosddev(fbapplydev, 0);
}

void setfbosd()
{
#ifndef ARM
	debug(101, "################## set osd offset ####################");
	debug(101, "status.leftoffset: %d", status.leftoffset);
	debug(101, "status.rightoffset: %d", status.rightoffset);
	debug(101, "status.topoffset: %d", status.topoffset);
	debug(101, "status.bottomoffset: %d", status.bottomoffset);
	
	char* fbleftdev = "/proc/stb/fb/dst_left";
	char* fbwidthdev = "/proc/stb/fb/dst_width";
	char* fbtopdev = "/proc/stb/fb/dst_top";
	char* fbheightdev = "/proc/stb/fb/dst_height";
	
	int fbleft = strtol(getfbosddev(fbleftdev) , NULL, 16);
	int fbwidth = strtol(getfbosddev(fbwidthdev) , NULL, 16);
	int fbtop = strtol(getfbosddev(fbtopdev) , NULL, 16);
	int fbheight = strtol(getfbosddev(fbheightdev) , NULL, 16);
	debug(101, "Setting OSD position: %d %d %d %d", fbleft ,fbwidth ,fbtop ,fbheight);
	
	fbleft = status.leftoffset;
	fbwidth = 720 - status.leftoffset - status.rightoffset;
	fbtop = status.topoffset;
	fbheight = 576 - status.topoffset - status.bottomoffset;
	debug(101, "Setting OSD position changed: %d %d %d %d", fbleft ,fbwidth ,fbtop ,fbheight);
	debug(10, "Setting OSD position changed: %d %d %d %d", fbleft ,fbwidth ,fbtop ,fbheight);
	
	setfbosddev(fbleftdev, fbleft);
	setfbosddev(fbwidthdev, fbwidth);
	setfbosddev(fbtopdev, fbtop);
	setfbosddev(fbheightdev, fbheight);
	debug(101, "######################################################");
#endif
}

int setrtctimemips()
{
	char *rtctimedev = NULL, *tmpstr = NULL;
	int ret = 0;
//	int value = 0;
	
	time_t t = time(NULL);
	struct tm *local = localtime(&t);
 	time_t rawlocal = mktime(local);
 	
 	t = time(NULL);
	struct tm *gmt = gmtime(&t);
	time_t rawgmt = mktime(gmt);
  
	int offset = difftime(rawlocal, rawgmt);

	tmpstr = oitoa(offset);
	rtctimedev = getconfig("rtctime_offsetdev", NULL);
	if(rtctimedev != NULL)
		ret = writesys(rtctimedev, tmpstr, 0);
	else
		ret = writesys("/proc/stb/fp/rtc_offset", tmpstr, 0);
	free(tmpstr); tmpstr = NULL;
	rtctimedev = NULL;
	
	if(ret == 0)
	{
		tmpstr = oitoa(rawlocal);
		rtctimedev = getconfig("rtctimedev", NULL);
		if(rtctimedev != NULL)
			ret = writesys(rtctimedev, tmpstr, 0);
		else
			ret = writesys("/proc/stb/fp/rtc", tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		rtctimedev = NULL;
	}
	return ret;
}

#ifdef CONFIG_ION
void SetMode()
{
	printf("****** SetMode **********\n");
	struct fb_var_screeninfo var_screeninfo;
	
	lfb = fb->fb;
	/* unmap old framebuffer with old size */
	if (lfb)
		//munmap(lfb, stride * screeninfo.yres_virtual);
		munmap(lfb, fb->varfbsize);

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
	struct fb_fix_screeninfo fix;
	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fix)<0)
	{
		perror("FBIOGET_FSCREENINFO");
		printf("fb failed\n");
	}
	/* map new framebuffer */
	lfb=(unsigned char*)mmap(0, fix.line_length * var_screeninfo.yres_virtual, PROT_WRITE|PROT_READ, MAP_SHARED, fb->fd, 0);
	fb->fb = lfb;
	fb->fblong = (unsigned long*)fb->fb;
	memset(lfb, 0, fix.line_length * var_screeninfo.yres_virtual);
	fb->varfbsize = fix.line_length * var_screeninfo.yres_virtual;
	blit();
}
#endif


#endif
