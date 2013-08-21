#ifndef USBRESET_H
#define USBRESET_H

#define USBRESETPATH "/sys/bus/usb/drivers/usb/unbind"
#define USBRESETUNBIND USBRESETPATH"/unbind"
#define USBRESETBIND USBRESETPATH"/bind"

void screenusbreset()
{
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;	
	struct skin* load = getscreen("loading");
	
	// /sys/bus/usb/drivers/usb/unbind
	// /sys/bus/usb/drivers/usb/1-1/product
	
	tmpstr = readsys("", 1);
	addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
	free(tmpstr); tmpstr = NULL;
	
	mbox = menulistbox(mlist, NULL, "USB Reset", NULL, NULL, 1, 0);
	
	if(mbox != NULL)
	{
		drawscreen(load, 0, 0);
	
		writesys(USBRESETUNBIND, value, 0); //unbind
		sleep(1);
		writesys(USBRESETBIND, value, 0); //bind
		
		clearscreen(load);
	}
}

#endif
