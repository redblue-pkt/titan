#ifndef FB_H
#define FB_H

struct fb* getfb(char *name)
{
	debug(1000, "in");
	struct fb *node = fb;

	while(node != NULL)
	{
		if(ostrstr(node->name, name) != NULL)
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
	
	if(ostrcmp(name, FB) == 0)
		setfbvarsize(newnode);
	else if(ostrcmp(name, FB1) == 0)
		newnode->varfbsize = 720 * 576 * newnode->colbytes;
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

	ret = getfbsize(dev);
	if(ret < 0)
	{
		err("framebuffermem (%s) to small, needed = %ld", name, ret * -1);
		free(name);
		free(newnode);
		if(newnode == fb) fb = NULL;
		return NULL;
	}

	debug(100, "fbname=%s, fbwidth=%d, fbheight=%d, fbcol=%d, fbsize=%ld", newnode->name, newnode->width, newnode->height, newnode->colbytes, newnode->varfbsize);
	debug(1000, "out");
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

void pngforlcd()
{
	fb2png(skinfb->fb, 320, 240, "/tmp/titanlcd.png");
	
/*	FILE *fd;
	fd=fopen("/tmp/titanlcd.raw", "w");
	int help = 0;
	int i = 0;
	while (i < 240) {
		fwrite(skinfb->fb+help,320*4,1,fd);
		help = help + (skinfb->width * 4);
		i++;
	}
	fclose(fd);*/
}

void write_FB_to_JPEG_file(unsigned char *inbuffer, int image_width, int image_height, char * filename, int quality)
{
	
		
	size_t helpb = 0;
	size_t help = 0;
	unsigned char *helpbuffer = malloc(image_width*image_height*3);
	
	//delete alpha byte
	while(help <= (image_width*image_height*4))
	{
		helpbuffer[helpb+0] = inbuffer[help+0];
		helpbuffer[helpb+1] = inbuffer[help+1];
		helpbuffer[helpb+2] = inbuffer[help+2];
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

#endif
