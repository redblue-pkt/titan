#ifndef MULTIBOOT_H
#define MULTIBOOT_H

int install_image(char* part)
{
	char* imagedir = NULL;
	char* point = NULL;
	char* command = NULL;
		
	imagedir = screendir("/tmp", "*.bz2", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 1200, 0, 600, 0, 0);
	if(imagedir == NULL)
		return 2;
	point = strrchr(imagedir,'/');
	if(point == NULL)
		return 0;
	point[0] = '\0';
	command = ostrcat("ofgwrite -m", part, 0, 0);
	command = ostrcat(command, " ", 1, 0);
	command = ostrcat(command, imagedir, 1, 0);
	int merksec = status.sec;
	status.sec = 0; //deaktiviere Spinner
	system(command);
	status.sec = merksec;
	free(imagedir); imagedir = NULL;
	free(command); command = NULL;
	return 1;
}

void screenmultiboot(void)
{
	int rcret = 0;
	
	DIR *dir;
	struct dirent * member;
	char *tmpstr = NULL;
	char *tmpstr2 = NULL;
	char *tmpstr3 = NULL;
	char *ownpart = NULL;
	char *instpart = NULL;
	char *ownpartname = NULL;
	int neustart = 0;
	int ret = 0;
	int end = 0;
	
	struct skin* screen = getscreen("multiboot");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* partitions = getscreennode(screen, "partitions");

	while(end == 0)
	{
		changechoiceboxvalue(partitions, NULL);
		
		tmpstr = readsys("/boot/STARTUP", 1);
		tmpstr2 = ostrstr(tmpstr, "kernel");
		if(tmpstr2 != NULL)
		{
			tmpstr2[7] = '\0';
			ownpart = ostrcat(tmpstr2+6, NULL, 0, 0);
 	 }
 	 free(tmpstr); tmpstr = NULL;
 	 tmpstr2 = NULL;    		
		
		dir = opendir("/boot"); /* oeffne aktuelles Verzeichnis */
		if (dir)
		{
			while (0 != (member = readdir(dir)))
			{
 	  		if(ostrcmp(".", member->d_name) != 0 && ostrcmp("..", member->d_name) != 0 && ostrcmp("STARTUP", member->d_name) != 0)
 		 		{
	   			tmpstr = ostrcat("/boot/", member->d_name, 0, 0);
  	 			tmpstr2 = readsys(tmpstr, 1);
  		 		tmpstr3 = ostrstr(tmpstr2, "kernel");
 	  			if(tmpstr3 != NULL)
 	  			{
	   				tmpstr3[7] = '\0';
	   				if(ostrcmp(ownpart, tmpstr3+6) == 0)
	   				{
	   					ownpartname = ostrcat(member->d_name, NULL, 0, 0);
	   					printf("----> own: %s\n", ownpartname);
	   				}
	   				addchoicebox(partitions, member->d_name, member->d_name);
 	  			}     		
   				free(tmpstr); tmpstr = NULL;
   				free(tmpstr2); tmpstr2 = NULL;
   				tmpstr3 = NULL;
   			}
			}
			closedir(dir);
			setchoiceboxselection(partitions, ownpartname);
		}

	
	
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
			{ 
				end = 1;
				break;
			}
			if(rcret == getrcconfigint("rcok", NULL)) 
			{
				tmpstr = ostrcat("cp /boot/", partitions->ret, 0, 0);
				tmpstr = ostrcat(tmpstr, " /boot/startup", 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;
				if(textbox("MultiBoot", _("Do you want to start now the selected image?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 650, 200, 0, 0) == 1)
					neustart = 1;
				else
					neustart = 0;
				end = 1;
				break;
			}
			if(rcret == getrcconfigint("rcred", NULL)) 
			{
				tmpstr = ostrcat("/boot/", partitions->ret, 0, 0);
				tmpstr2 = readsys(tmpstr, 1);
				tmpstr3 = ostrstr(tmpstr2, "kernel");
				if(tmpstr3 != NULL)
				{
					tmpstr3[7] = '\0';
					instpart = ostrcat(tmpstr3+6, NULL, 0, 0);
 	 			}
 	 			free(tmpstr); tmpstr = NULL;
 	 			free(tmpstr2); tmpstr2 = NULL;
 	 			tmpstr3 = NULL;    		
				
				if(ostrcmp(ownpartname, partitions->ret) == 0)
				{
					if(textbox("MultiBoot", _("You have chosen the active image\nDo you really want to overwrite this?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 650, 200, 0, 0) == 1)
						ret = install_image(instpart);
					else
						ret = 2;
				}
				else
				{
					if(textbox("MultiBoot", _("Do you really want to overwrite the chosen image?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 650, 200, 0, 0) == 1)
						ret = install_image(instpart);
					else
						ret = 2;
				}	
				if(ret == 1)
					textbox("MultiBoot", _("The image was successfully installed"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 650, 200, 0, 0);	
				if(ret == 0)
					textbox("MultiBoot", _("ERROR: The installation ends with error"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 650, 200, 0, 0);				
				ret = 0;
				free(instpart); instpart = NULL;
				break;
			}
			if(rcret == getrcconfigint("rcgreen", NULL)) 
			{
				textbox("MultiBoot", _("Renaming is not available at the moment."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 650, 200, 0, 0);
				/*tmpstr = screeninputhelp(partitions->ret, 0, 0, 0);
				if(tmpstr != NULL)
				{
					tmpstr = ostrcat("/boot/", tmpstr, 0, 1);
					tmpstr2 = ostrcat("/boot/", partitions->ret, 0, 0);
					tmpstr3 = ostrcat("mv ", tmpstr2, 0, 0);
					tmpstr3 = ostrcat(tmpstr3, " ", 1, 0);
					tmpstr3 = ostrcat(tmpstr3, tmpstr, 1, 0);
					system(tmpstr3);
					free(tmpstr); tmpstr = NULL;
 	 				free(tmpstr2); tmpstr2 = NULL;
 	 				free(tmpstr3); tmpstr3 = NULL;
					break;
				}*/
			}
		}
		delownerrc(screen);
		clearscreen(screen);
		free(ownpart); ownpart = NULL;
		free(ownpartname); ownpartname = NULL;
	}
	if(neustart == 1)
		oshutdown(2, 1);
}


#endif		
	
	