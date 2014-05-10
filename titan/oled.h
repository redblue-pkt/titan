#ifndef OLED_H_
#define OLED_H_

void write2oled(unsigned char* buf, int xres, int yres)
{
	unsigned char* lfb = NULL;
	int i = 0;
	int bi = 0;
	
	lfb = malloc(xres * yres);
	if(lfb == NULL)
		return;
		
	for(i = 0; i <= xres*yres; i++)
	{
		//             R                           B                          G
		lfb[i] = ((76 * buf[bi+2]) / 255) + ((150 * buf[bi+1]) / 255) + ((28 * buf[bi]) / 255);
		bi = bi + 4;
	}
	
	int lcdfd = open("/dev/dbox/oled0", O_RDWR);
	write(lcdfd, lfb, xres * yres);
	close(lcdfd);
	
	free(lfb);
	return;
}

#endif	
	