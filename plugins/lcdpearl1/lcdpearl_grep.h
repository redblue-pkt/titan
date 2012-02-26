#include <sys/inotify.h>


void lcd_raw_update()
{
	system("touch /tmp/.imageneu");
	//char* fbgrab = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/fbgrab -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/.titanlcd1.png > /dev/null", 0, 0);
	char* fbgrab = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/fbgrab -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/titanlcd.png > /dev/null", 0, 0);
	system(fbgrab);
	//system("mv /tmp/.titanlcd1.png /tmp/titanlcd.png");
}


int lcd_event_check (int fd)
{
  struct timeval tv;
  tv.tv_sec = 70;
  tv.tv_usec = 0;
 
  fd_set rfds;
  FD_ZERO (&rfds);
  FD_SET (fd, &rfds);
  /* Wait until an event happens or we get interrupted 
     by a signal that we catch */
  return select (FD_SETSIZE, &rfds, NULL, NULL, &tv);
}


void lcd_raw_event()
{
  int fd;
  int wd;
  int rc;
  unsigned int len;
  char *buf;
  buf=malloc((sizeof(struct inotify_event)+16)*1024);
  char* startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/start.sh", 0, 0);

	while (draw == 0 && LCD_Pearl1thread != NULL)
	{
		sleep(1);
	}
	if(LCD_Pearl1thread == NULL)
	{
		free(buf);
		return;
	}
	lcd_raw_update();
	
	fd = inotify_init();
  if ( fd < 0 ) {
   	perror( "inotify_init" );
  }

  wd = inotify_add_watch( fd,  "/tmp/titanlcd.raw", IN_CLOSE_WRITE);

	while(1){
    
    printf("new lcd image???\n");
	
   	rc = lcd_event_check (fd);
   	
   	if (system("ls /tmp/.imageneu")	== 0)
   	{
   		printf("kill\n");
   		system("killall lcd4linux");
   		sleep(1);
   		system(startlcd);
   	}
   	
   	//if (lcd_event_check (fd) > 0)
   	if (rc > 0)
    {
      len=read(fd, buf,(sizeof(struct inotify_event)+16)*1024 );
      lcd_raw_update();
    }
   	//inotify_rm_watch( fd, wd );
		//close( fd );
		if(LCD_Pearl1thread == NULL)
			break;
		//fd = inotify_init();
  	//if ( fd < 0 ) {
    //	perror( "inotify_init" );
  	//}
  	//wd = inotify_add_watch( fd, "/tmp/titanlcd.raw", IN_CLOSE_WRITE);
	}
	free(buf);
	free(startlcd);startlcd=NULL;
}
