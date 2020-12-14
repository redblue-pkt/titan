#ifndef FB_H
#define FB_H

#ifdef CONFIG_ION
#define ION_HEAP_TYPE_BMEM      (ION_HEAP_TYPE_CUSTOM + 1)
#define ION_HEAP_ID_MASK        (1 << ION_HEAP_TYPE_BMEM)
#define ACCEL_MEM_SIZE          (32*1024*1024)
int m_accel_fd;
#endif




struct fb* getfb(char *name)
{
	struct fb *node = fb;

	while(node != NULL)
	{
		if(ostrstr(node->name, name) != NULL)
			return node;

		node = node->next;
	}
	debug(444, "framebuffer not found (%s)", name);
	return NULL;
}

long getfbsize(int dev)
{
	struct fb* node = fb;
	unsigned long fbmemcount = 0;
	struct fb_fix_screeninfo fix_screeninfo;

	if(fb == NULL)
	{
		err("NULL dedect");
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
	
	return fix_screeninfo.smem_len - fbmemcount;
}

int getfbxres()
{
	struct fb_var_screeninfo var_screeninfo;
	int fd = -1;
	char *fbdev = getconfig("fbdev", NULL);
	
	if(fbdev == NULL)
	{
		err("failed to find fbdev in config file");
		return -1;
	}

	fd = open(fbdev, O_RDWR);
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return -1;
	}
	
	if(ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo) < 0)
	{
		err("failed to read VSCREENINFO");
		close(fd);
		return -1;
	}

	close(fd);
	return var_screeninfo.xres;
}

int getfbyres()
{
	struct fb_var_screeninfo var_screeninfo;
	int fd = -1;
	char *fbdev = getconfig("fbdev", NULL);
	
	if(fbdev == NULL)
	{
		err("failed to find fbdev in config file");
		return -1;
	}

	fd = open(fbdev, O_RDWR);
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return -1;
	}
	
	if(ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo) < 0)
	{
		err("failed to read VSCREENINFO");
		close(fd);
		return -1;
	}

	close(fd);
	return var_screeninfo.yres;
}

struct fb* addfb(char *fbname, int dev, int width, int height, int colbytes, int fd, unsigned char* mmapfb, unsigned long fixfbsize, unsigned long data_phys)
{
	struct fb *newnode = NULL, *node = fb;
	char *name = NULL;
	long ret = 0;

	name = ostrcat(fbname, NULL, 0, 0);
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
	newnode->data_phys = data_phys;
	
	if(ostrcmp(name, FB) == 0)
		setfbvarsize(newnode);
	else if(ostrcmp(name, FB1) == 0)
#ifdef MIPSEL
		newnode->varfbsize = width * height * newnode->colbytes;
#else		
		newnode->varfbsize = 720 * 576 * newnode->colbytes;
#endif
	else
		newnode->varfbsize = width * height * newnode->colbytes;
	
	if(node != NULL)
	{
		while(node->next != NULL)
			node = node->next;
		node->next = newnode;
	}
	else
		fb = newnode;
		
	/*eigener Buffer zB fuer LCD*/
	if(dev == 999)
		return newnode; 
#ifdef CONFIG_ION
	if(ostrcmp(name, FB) == 0)
		ret = getfbsize(dev);
#else
	ret = getfbsize(dev);
#endif
	if(ret < 0)
	{
		err("framebuffermem (%s) to small, needed = %ld", name, ret * -1);
		free(name);
		free(newnode);
		if(newnode == fb) fb = NULL;
		return NULL;
	}

	debug(444, "fbname=%s, fbwidth=%d, fbheight=%d, fbcol=%d, fbsize=%ld, phys_addr=%lu", newnode->name, newnode->width, newnode->height, newnode->colbytes, newnode->varfbsize, newnode->data_phys);
	return newnode;
}

void fb2png_thread()
{
	while (writeFBfile.buf1 != NULL || writeFBfile.buf2 != NULL) {
		if(writeFBfile.buf1 != NULL) {
			writeFBfile.ActBuf = writeFBfile.buf1;
			fb2png(writeFBfile.buf1, 320, 240, "/tmp/titanlcd.png");
			free(writeFBfile.buf1); writeFBfile.buf1 = NULL;
			if(writeFBfile.buf2 != NULL)
				usleep(500000);
		}
		if(writeFBfile.buf2 != NULL) {
			writeFBfile.ActBuf = writeFBfile.buf2;
			fb2png(writeFBfile.buf2, 320, 240, "/tmp/titanlcd.png");
			free(writeFBfile.buf2); writeFBfile.buf2 = NULL;
			if(writeFBfile.buf1 != NULL)
				usleep(500000);
		}
	}
	writeFBfile.ActBuf = NULL;
}

void delfb(char *name)
{
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
}

void freefb()
{
	struct fb *node = fb, *prev = fb;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delfb(prev->name);
	}
}

struct fb* openfb(char *fbdev, int devnr)
{
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

#ifdef CONFIG_ION
	int ion;
#endif

#ifndef NOFB
	fd = open(fbdev, O_RDWR);

// blinking work start
#ifdef MIPSEL
	if (fd < 0)
	{
		err(fbdev);
		goto nolfb;
	}
#endif
// blinking work end
		
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return NULL;
	}
	closeonexec(fd);
	
	//workarounnd
	if(checkchipset("HI3798MV200") == 1)
	{
		printf("---> setframebuffer(1280, 720, 1280, 2880, 32)\n");
		setframebuffer(1280, 720, 1280, 2880, 32);
	}
	if(frameb1 == 1)
	{
		printf("---> setframebuffer(1920, 1080, 1920, 2160, 32);\n");
		setframebuffer(1920, 1080, 1920, 2160, 32);
	}
	
	
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

	debug(444, "%dk video mem", fix_screeninfo.smem_len/1024);
 	unsigned long data_phys = 0;	
#ifdef MIPSEL
	data_phys = fix_screeninfo.smem_start;
#ifdef CONFIG_ION
	/* allocate accel memory here... its independent from the framebuffer */
	ion = open("/dev/ion", O_RDWR | O_CLOEXEC);
	if (ion >= 0)
	{
		struct ion_allocation_data alloc_data;
		struct ion_fd_data share_data;
		struct ion_handle_data free_data;
		struct ion_phys_data phys_data;
		int ret;
		
		debug(444,"Using ION allocator");

		memset(&alloc_data, 0, sizeof(alloc_data));
		alloc_data.len = ACCEL_MEM_SIZE;
		alloc_data.align = 4096; // 4k aligned
		alloc_data.heap_id_mask = ION_HEAP_ID_MASK;
		ret = ioctl(ion, ION_IOC_ALLOC, &alloc_data);
		if (ret < 0)
		{
			err("ION_IOC_ALLOC failed");
			err("failed to allocate accel memory!!!");
			return NULL;
		}

		memset(&phys_data, 0, sizeof(phys_data));
		phys_data.handle = alloc_data.handle;
		ret = ioctl(ion, ION_IOC_PHYS, &phys_data);
		if (ret < 0)
		{
			err("ION_IOC_PHYS failed");
			goto err_ioc_free;
		}

		memset(&share_data, 0, sizeof(share_data));
		share_data.handle = alloc_data.handle;
		ret = ioctl(ion, ION_IOC_SHARE, &share_data);
		if (ret < 0)
		{
			err("ION_IOC_SHARE failed");
			goto err_ioc_free;
		}

		memset(&free_data, 0, sizeof(free_data));
		free_data.handle = alloc_data.handle;
		if (ioctl(ion, ION_IOC_FREE, &free_data) < 0)
			err("ION_IOC_FREE failed");

		m_accel_fd = share_data.fd;
		lfb=(unsigned char*)mmap(0, ACCEL_MEM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, share_data.fd, 0);

		if (lfb)
		{
			debug(444,"%dkB available for acceleration surfaces (via ION).", ACCEL_MEM_SIZE);
			
			node = addfb(FB, devnr, var_screeninfo.xres, var_screeninfo.yres, var_screeninfo.bits_per_pixel / 8, fd, mmapfb, fix_screeninfo.smem_len, data_phys);
			skinfb = addfb(SKINFB, 0, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, share_data.fd, lfb, ACCEL_MEM_SIZE, phys_data.addr);
			accelfb = addfb(ACCELFB, 0, (ACCEL_MEM_SIZE-(skinfb->width*skinfb->height*4)) / 4, 1, 4, share_data.fd, skinfb->fb + skinfb->varfbsize, ACCEL_MEM_SIZE, skinfb->data_phys + skinfb->varfbsize);
		}
		else
		{
			close(m_accel_fd);
			err("mmap lfb failed");
err_ioc_free:
			err("failed to allocate accel memory via ION!!!");
			m_accel_fd = -1;
			memset(&free_data, 0, sizeof(free_data));
			free_data.handle = alloc_data.handle;
			if (ioctl(ion, ION_IOC_FREE, &free_data) < 0)
				err("ION_IOC_FREE");
		}
		close(ion);
	}
	else
	{
		err("failed to open ION device node! no allocate accel memory available !!");
		m_accel_fd = -1;
	}
	if (!lfb)
	{
		err("mmap");
		goto nolfb;
	}
#else
	lfb = (unsigned char*)mmap(0, fix_screeninfo.smem_len, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
	if (!lfb)
	{
		err("mmap");
		goto nolfb;
	}

	if (var_screeninfo.bits_per_pixel != 32)
	{
		debug(444, "Only 32 bit per pixel supported. Framebuffer currently use %d", var_screeninfo.bits_per_pixel);
		closefb();
		return 0;
	}
#endif	
#endif
#ifndef CONFIG_ION
	if(devnr == 0)
		node = addfb(FB, devnr, var_screeninfo.xres, var_screeninfo.yres, var_screeninfo.bits_per_pixel / 8, fd, mmapfb, fix_screeninfo.smem_len, data_phys);
	if(devnr == 1)
		node = addfb(FB1, devnr, var_screeninfo.xres, var_screeninfo.yres, var_screeninfo.bits_per_pixel / 8, fd, mmapfb, fix_screeninfo.smem_len, data_phys);

#endif		

#else
	mmapfb = malloc(16 * 1024 * 1024);
	if(devnr == 0)
		node = addfb(FB, devnr, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, -1, mmapfb, 16*1024*1024, 0);
	if(devnr == 1)
		node = addfb(FB1, devnr, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, -1, mmapfb, 16*1024*1024, 0);

#endif

	return node;

#ifdef MIPSEL
nolfb:
	if (fd >= 0)
	{
		close(fd);
		fd = -1;
	}

	debug(444, "framebuffer not available.");
#endif
	return 0;
// blinking work end
}

void closefb()
{

#ifdef MIPSEL
	if(lfb)
	{
		debug(444, "ms_sync");
		msync(lfb, fix_screeninfo.smem_len, MS_SYNC);
		munmap(lfb, fix_screeninfo.smem_len);
	}
	if(fb->fd >= 0)
	{
		debug(444, "close");
		disablemanualblit();
		close(fb->fd);
		fb->fd = -1;
	}
#endif

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
	if(fb != NULL && fb->fb != NULL)
		memset(fb->fb, 0, fb->fixfbsize);
}

void clearfb(struct fb *node)
{
	if(node != NULL)
		memset(node->fb, 0, node->varfbsize);
}

void blitfb(int flag)
{
	blitfb2(skinfb, flag);
}

void blitfb1()
{
	blitfb2(skinfb, 0);
}

//flag 0: del skinfb
//flag 1: don't del skinfb
void changefbresolution(char *value, int flag)
{
	debug(444, "fb->colbytes: %d", fb->colbytes);
#ifdef MIPSEL
	return;
#endif

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
	if(flag == 0) clearfb(skinfb);
}

static void write_PNG(unsigned char *outbuffer, char *filename, 
				int width, int height, int interlace)
{
 	int i;
	int bit_depth=0, color_type;
	png_bytep row_pointers[height];
	png_structp png_ptr;
	png_infop info_ptr;
	FILE *outfile = fopen(filename, "wb");

	for (i=0; i<height; i++)
		row_pointers[i] = outbuffer + i * 4 * width;
		
	if (!outfile)
	{
		fprintf (stderr, "Error: Couldn't fopen %s.\n", filename);
		exit(EXIT_FAILURE);
	}
    
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
	(png_voidp) NULL, (png_error_ptr) NULL, (png_error_ptr) NULL);
    
	if (!png_ptr)
		err("Error: Couldn't create PNG write struct.");
    
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
		err("Error: Couldn't create PNG info struct.");
	}
    
	png_init_io(png_ptr, outfile);
    
	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    
	bit_depth = 8;
	color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	//color_type = PNG_COLOR_TYPE_RGB;
	png_set_invert_alpha(png_ptr);
	png_set_bgr(png_ptr);

	png_set_IHDR(png_ptr, info_ptr, width, height, 
	bit_depth, color_type, interlace, 
	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
	png_write_info(png_ptr, info_ptr);
    
	printf ("Now writing PNG file\n");
    
	png_write_image(png_ptr, row_pointers);
    
	png_write_end(png_ptr, info_ptr);
	/* puh, done, now freeing memory... */
	png_destroy_write_struct(&png_ptr, &info_ptr);
    
	if (outfile != NULL)
		(void) fclose(outfile);
} 

static void convert_and_write(unsigned char *inbuffer, char *filename, 
				int width, int height, int bits, int interlace)
{
	size_t bufsize = (size_t) width * height * 4;

	unsigned char *outbuffer = malloc(bufsize);

	if (outbuffer == NULL)
		err("Not enough memory");
	
	memset(outbuffer, 0, bufsize);
	write_PNG(inbuffer, filename, width, height, interlace);
	(void) free(outbuffer);
}

int fb2png(unsigned char *buf_p, int width, int height, char *outfile)
{
	int interlace = PNG_INTERLACE_ADAM7;
	int bitdepth = 32;
		
	size_t help = 0;
	while(help <= (width * height * 4))
	{
		buf_p[help+3] = 0x00;
		help = help + 4;
	}
		
	convert_and_write(buf_p, outfile, width, height, bitdepth, interlace);

	return 0;
}

void pngforlcd(unsigned char *fbuffer, int width, int height)
{
	fb2png(fbuffer, width, height, "/tmp/titanlcd.png");
}

void write_FB_to_JPEG_file(unsigned char *inbuffer, int image_width, int image_height, char * filename, int quality)
{		
	size_t helpb = 0;
	size_t help = 0;
	unsigned char *helpbuffer = malloc(image_width*image_height*3);
	
	//delete alpha byte
	while(help <= (image_width*image_height*4))
	{
		helpbuffer[helpb+0] = inbuffer[help+2];
		helpbuffer[helpb+1] = inbuffer[help+1];
		helpbuffer[helpb+2] = inbuffer[help+0];
		help = help + 4;
		helpb = helpb + 3;
	}
	
	JSAMPLE *image_buffer = helpbuffer;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;
	
	JSAMPROW row_pointer[1];
	int row_stride;
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = image_width; 	/* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);
	row_stride = image_width * 3;

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
   
	free(helpbuffer);
}

int setframebuffer(int width, int height, int vwidth, int vheight, int bits)
{
	struct fb_var_screeninfo var_screeninfo;
	int fd = -1;
	char *fbdev = getconfig("fbdev", NULL);
	
	if(fbdev == NULL)
	{
		err("failed to find fbdev in config file");
		return -1;
	}

	fd = open(fbdev, O_RDWR);
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return -1;
	}
	
	if(ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo) < 0)
	{
		err("failed to read VSCREENINFO");
		close(fd);
		return -1;
	}

	var_screeninfo.xres = width;
	var_screeninfo.yres = height;
	var_screeninfo.xres_virtual = vwidth;
	var_screeninfo.yres_virtual = vheight;
	var_screeninfo.bits_per_pixel  = bits;

	if(ioctl(fd, FBIOPUT_VSCREENINFO, &var_screeninfo) < 0)
	{
		err("failed to put VSCREENINFO");
		close(fd);
		return -1;
	}
	
	printf("---> FBIOPUT_VSCREENINFO endet\n");
	system("fbset");
		
	close(fd);
	return 0;	
}

#ifdef OEBUILD 
int saveframebuffer()
{
	int fd = -1;
	char *fbdev = getconfig("fbdev", NULL);
	
	if(fbdev == NULL)
	{
		err("failed to find fbdev in config file");
		return -1;
	}

	fd = open(fbdev, O_RDWR);
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return -1;
	}
	//workaround
	if(checkchipset("HI3798MV200") == 1)
	{
		printf("---> setframebuffer(1280, 720, 1280, 2880, 32)\n");
		setframebuffer(1280, 720, 1280, 2880, 32);
	}
	if(frameb1 == 1)
	{
		printf("---> setframebuffer(1920, 1080, 1920, 2160, 32)\n");
		setframebuffer(1920, 1080, 1920, 2160, 32);
	}
	
	if(ioctl(fd, FBIOGET_VSCREENINFO, &save_var_screeninfo) < 0)
	{
		err("failed to read VSCREENINFO");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;	
}

int restoreframebuffer()
{
	int fd = -1;
	char *fbdev = getconfig("fbdev", NULL);
	
	if(fbdev == NULL)
	{
		err("failed to find fbdev in config file");
		return -1;
	}

	fd = open(fbdev, O_RDWR);
	if(fd == -1)
	{
		perr("failed to open %s", fbdev);
		return -1;
	}
	
	if(ioctl(fd, FBIOPUT_VSCREENINFO, &save_var_screeninfo) < 0)
	{
		err("failed to write VSCREENINFO");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;	
}
#endif


#endif
