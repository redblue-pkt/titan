#ifndef SH4PORT_H
#define SH4PORT_H

#include <bpamem.h>
#include <libmmeimage.h>
#include <linux/stmfb.h>

struct stmfbio_output_configuration outcfg;
struct stmfbio_outputinfo outinfo;
struct stmfbio_planeinfo planemode;
struct stmfbio_var_screeninfo_ex infoex;

void fbsave()
{
  outcfg.outputid = STMFBIO_OUTPUTID_MAIN;
  if(ioctl(fb->fd, STMFBIO_GET_OUTPUT_CONFIG, &outcfg) < 0)
  {
    perr("ioctl STMFBIO_GET_OUTPUT_CONFIG");
  }

	outinfo.outputid = STMFBIO_OUTPUTID_MAIN;
	if(ioctl(fb->fd, STMFBIO_GET_OUTPUTINFO, &outinfo) < 0)
  {
		perr("ioctl STMFBIO_GET_OUTPUTINFO");
  }

	planemode.layerid = 0;
	if(ioctl(fb->fd, STMFBIO_GET_PLANEMODE, &planemode) < 0)
  {
    perr("ioctl STMFBIO_GET_PLANEMODE");
  }

  if(ioctl(fb->fd, STMFBIO_GET_VAR_SCREENINFO_EX, &infoex) < 0)
  {
    perr("ioctl STMFBIO_GET_VAR_SCREENINFO_EX");
  }
}

void fbrestore()
{
  if(ioctl(fb->fd, STMFBIO_SET_VAR_SCREENINFO_EX, &infoex) < 0)
  {
    perr("ioctl STMFBIO_SET_VAR_SCREENINFO_EX");
  }

	if(ioctl(fb->fd, STMFBIO_SET_PLANEMODE, &planemode) < 0)
  {
		perr("ioctl STMFBIO_SET_PLANEMODE");
  }

	if(ioctl(fb->fd, STMFBIO_SET_VAR_SCREENINFO_EX, &infoex) < 0)
  {
		perr("ioctl STMFBIO_SET_VAR_SCREENINFO_EX");
  }

	if(ioctl(fb->fd, STMFBIO_SET_OUTPUTINFO, &outinfo) < 0)
  {
		perr("ioctl STMFBIO_SET_OUTPUTINFO");
  }

	if(ioctl(fb->fd, STMFBIO_SET_OUTPUT_CONFIG, &outcfg) < 0)
  {
		perr("ioctl STMFBIO_SET_OUTPUT_CONFIG");
  }
}

void setfbvarsize(struct fb* newnode)
{
	if(newnode != NULL)
		newnode->varfbsize = 1920 * 1080 * newnode->colbytes;
}

void enablemanualblit()
{
}

//flag 0 = no animation
//flag 1 = animation
void blitfb2(struct fb* fbnode, int flag)
{
	int i = 0, max = 1, wstep = 0, hstep = 0, ret = 0;
	unsigned char buf[10];

	if(fbnode == NULL) return;
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

	int mode3d = 0;
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
}

void setfbtransparent(int value)
{
	struct stmfbio_var_screeninfo_ex varEx = {0};

	varEx.layerid  = 0;
	varEx.activate = STMFBIO_ACTIVATE_IMMEDIATE;
	varEx.caps = STMFBIO_VAR_CAPS_OPACITY;
	varEx.opacity = value;


	if(ioctl(fb->fd, STMFBIO_SET_VAR_SCREENINFO_EX, &varEx) < 0)
		perr("STMFBIO_SET_VAR_SCREENINFO_EX");
}

int allocbpamem(size_t size, int *memfd, unsigned char **mem)
{
	*memfd = open("/dev/bpamem0", O_RDWR);
	
	if(*memfd < 0)
	{
		err("cannot access /dev/bpamem0! err = %d", (int)*memfd);
		return -1;
	}

	BPAMemAllocMemData bpa_data;
	bpa_data.bpa_part = "LMI_VID";
	bpa_data.mem_size = size;
	int res;
	res = ioctl(*memfd, BPAMEMIO_ALLOCMEM, &bpa_data); // request memory from bpamem
	if(res)
	{
		err("cannot alloc required bpa mem");
		close(*memfd);
		return -1;
	}

	char* bpa_mem_device = NULL;
	bpa_mem_device = malloc(30);
	if(bpa_mem_device == NULL)
	{
		err("no mem");
		return -1;
	}

	sprintf(bpa_mem_device, "/dev/bpamem%d", bpa_data.device_num);
	close(*memfd);

	*memfd = open(bpa_mem_device, O_RDWR);

	if(*memfd < 0)
	{
		free(bpa_mem_device);
		err("cannot access %s! err = %d", bpa_mem_device, (int)*memfd);
		return -1;
	}

	*mem = (unsigned char *)mmap(0, bpa_data.mem_size, PROT_WRITE|PROT_READ, MAP_SHARED, *memfd, 0);

	if(*mem == MAP_FAILED) 
	{
		err("could not map bpa mem");
		free(bpa_mem_device);
		ioctl(*memfd, BPAMEMIO_FREEMEM);
		close(*memfd);
		return -1;
	}
	
	free(bpa_mem_device);
	return 0;
}

void freebpamem(int memfd, unsigned char* mem, size_t len)
{
	munmap(mem, len);
	ioctl(memfd, BPAMEMIO_FREEMEM);
	close(memfd);
}

//mode 0: with fill (draw to skinfb)
//mode 1: without fill (draw to skinfb)
//mode 2: with fill (draw to fb)
//mode 3: without fill (draw to fb)
void blitrect(int posx, int posy, int width, int height, long color, int transparent, int mode)
{
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
}

int readjpg(const char* filename, unsigned long* width, unsigned long* height, unsigned long* rowbytes, int* channels, unsigned char **mem, int *memfd)
{
	FILE *fp;
	unsigned int temp1, temp2, temp3, temp4;

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
	
	temp3 = temp1;
	temp4 = temp2;
	
	if(temp3 > skinfb->width) temp3 = skinfb->width;
	if(temp4 > skinfb->height) temp4 = skinfb->height;
		
	if(decode_jpeg_noalloc(fp, temp1, temp2, 200, 200, (char *)*mem, 1) == LIBMMEIMG_SUCCESS)
	{
		fclose(fp);
		return 0;
	}
	
	fclose(fp);
	freebpamem(*memfd, *mem, temp1 * temp2 * (*channels));
	err("hardware decode error");

	return -1;
}

//flag 0: blit from accelfb to skinfb
//flag 1: blit from skinfb to accelfb
void blitscale(int posx, int posy, int width, int height, int scalewidth, int scaleheight, int flag)
{
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
}

void blitjpg(unsigned char* buf, int posx, int posy, int width, int height, int scalewidth, int scaleheight, int mwidth, int mheight, int halign, int valign)
{
	STMFBIO_BLT_EXTERN_DATA blt_data;
	memset(&blt_data, 0, sizeof(STMFBIO_BLT_EXTERN_DATA));
	
	//auto scale to mwidth / mheight
	if(scalewidth == 1 && scaleheight == 1)
		calcautoscale(width, height, mwidth, mheight, &scalewidth, &scaleheight);
		
	if(scalewidth == 0) scalewidth = width;
	if(scaleheight == 0) scaleheight = height;

	if(scalewidth > mwidth) scalewidth = mwidth;
	if(scaleheight > mheight) scaleheight = mheight;

	if(halign == CENTER)
		posx += mwidth / 2 - width / 2;
	else if(halign == RIGHT)
		posx += mwidth - width;
	if(valign == MIDDLE)
		posy += mheight / 2 - height / 2;
	else if(valign == BOTTOM)
		posy += mheight - height;

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
			/* sh4
			unsigned long sc_regs[16];
			unsigned long sc_pc; //programm counter register
			unsigned long sc_pr; //procedure register
			unsigned long sc_sr; //status register
			unsigned long sc_gbr;
			unsigned long sc_mach;
			unsigned long sc_macl; */

			debugstack((void *)ctx.sc_pr, (void *)ctx.sc_pc);
			err("got signal %d, programm counter reg: 0x%lx,  procedure reg: 0x%lx", sig, ctx.sc_pc, ctx.sc_pr);

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
