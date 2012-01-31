#include <sys/inotify.h>

void lcd_raw_update()
{
	char* fbgrab = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/fbgrab -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/.titanlcd1.png > /dev/null", 0, 0);
	system(fbgrab);
	system("mv /tmp/.titanlcd1.png /tmp/titanlcd.png");
}


int lcd_event_check (int fd)
{
  fd_set rfds;
  FD_ZERO (&rfds);
  FD_SET (fd, &rfds);
  /* Wait until an event happens or we get interrupted 
     by a signal that we catch */
  return select (FD_SETSIZE, &rfds, NULL, NULL, NULL);
}


void lcd_raw_event()
{
  int fd;
  int wd;

	sleep(2);
	lcd_raw_update();
	
	fd = inotify_init();
  if ( fd < 0 ) {
   	perror( "inotify_init" );
  }
  wd = inotify_add_watch( fd,  "/tmp/titanlcd.raw", IN_CLOSE_WRITE);

	while(1){
    if (lcd_event_check (fd) > 0)
    {
      lcd_raw_update();
    }
   	inotify_rm_watch( fd, wd );
		close( fd );
		if(LCD_Pearl1thread == NULL)
			break;
		fd = inotify_init();
  	if ( fd < 0 ) {
    	perror( "inotify_init" );
  	}
  	wd = inotify_add_watch( fd, "/tmp/titanlcd.raw", IN_CLOSE_WRITE);
	}
}
