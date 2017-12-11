#ifndef MULTIBOOT_H
#define MULTIBOOT_H

void screenmultiboot(void)
{
	char* tmpstr = NULL;
	int rcret = 0;
	
	struct skin* screen = getscreen("multiboot");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* partitions = getscreennode(screen, "partitions");

	addchoicebox(partitions, "1", "Startup_1");
	addchoicebox(partitions, "2", "Startup_2");
	addchoicebox(partitions, "3", "Startup_3");
	addchoicebox(partitions, "4", "Startup_4");
	system("cat /boot/STARTUP | cut -d' ' -f2 | cut -dl -f3 > /tmp/boot");
	tmpstr = readsys("/tmp/boot", 1);
	setchoiceboxselection(partitions, tmpstr);
	free(tmpstr); tmpstr = NULL;
	
	drawscreen(screen, 0, 0);
	addscreenrc(screen, listbox);
	
	struct skin* tmp = NULL;
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(screen, tmp);
		rcret = waitrc(screen, 0, 0);
		tmp = listbox->select;
		if(rcret == getrcconfigint("rcexit", NULL))  
			break;
		if(rcret == getrcconfigint("rcok", NULL)) 
		{
			tmpstr = ostrcat("cp /boot/startup_", partitions->ret, 0, 0);
			tmpstr = ostrcat(tmpstr, " /boot/startup", 1, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
			break;
		}
	}
	delownerrc(screen);
	clearscreen(screen);
}


#endif		
	
	