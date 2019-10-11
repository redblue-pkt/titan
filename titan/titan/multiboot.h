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

char* getmultinames(int part)
{
	FILE *datei;
  char *line = NULL;
  char *pos = NULL;
  char *link = NULL;
  char *data = NULL;
  char *cmd  = NULL;
  char *wert = NULL;
  char *idir = NULL;
  
  line = malloc(MINMALLOC);
	if(line == NULL)
	{
		//err("no mem");
		return NULL;
	}
  
  if(part == 1)
  {
  	if(islink("/dev/block/by-name/linuxrootfs"))
  	{
  		link = ostrcat("/dev/block/by-name/linuxrootfs", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/linuxrootfs1/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/linuxrootfs1", NULL, 0, 0);
  	}
  	else
  	{
  		if(checkchipset("HI3798MV200") == 1)
  		{
  			link = ostrcat("/dev/block/by-name/userdata", NULL, 0, 0);
  			data = ostrcat("/tmp/multi/linuxrootfs1/etc/issue", NULL, 0, 0);
  			idir = ostrcat("/tmp/multi/linuxrootfs1", NULL, 0, 0);
  		}
  		else
  		{
  			link = ostrcat("/dev/block/by-name/rootfs1", NULL, 0, 0);
  			data = ostrcat("/tmp/multi/etc/issue", NULL, 0, 0);
  			idir = ostrcat("/tmp/multi/etc", NULL, 0, 0);
  		}
  	}
  }
  else if(part == 2)
  {
  	if(islink("/dev/block/by-name/userdata"))
  	{
  		link = ostrcat("/dev/block/by-name/userdata", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/linuxrootfs2/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/linuxrootfs2", NULL, 0, 0);
  	}
  	else
  	{
  		link = ostrcat("/dev/block/by-name/rootfs2", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/etc", NULL, 0, 0);
  	}
  }
  else if(part == 3)
  {
  	if(islink("/dev/block/by-name/userdata"))
  	{
  		link = ostrcat("/dev/block/by-name/userdata", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/linuxrootfs3/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/linuxrootfs3", NULL, 0, 0);
  	}
  	else
  	{
  		link = ostrcat("/dev/block/by-name/rootfs3", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/etc", NULL, 0, 0);
  	}
  }
  else if(part == 4)
  {
  	if(islink("/dev/block/by-name/userdata"))
  	{
 	  	link = ostrcat("/dev/block/by-name/userdata", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/linuxrootfs4/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/linuxrootfs4", NULL, 0, 0);
		}
		else
  	{
  		link = ostrcat("/dev/block/by-name/rootfs4", NULL, 0, 0);
  		data = ostrcat("/tmp/multi/etc/issue", NULL, 0, 0);
  		idir = ostrcat("/tmp/multi/etc", NULL, 0, 0);
  	}
	}
  
  system("mkdir /tmp/multi");
  if(islink(link))
  {
  		cmd = ostrcat("mount ", link, 0, 0);
  		cmd = ostrcat(cmd, " /tmp/multi", 1, 0);
  		if(part == 1) wert = ostrcat("1-", NULL, 0, 0);
  		if(part == 2) wert = ostrcat("2-", NULL, 0, 0);
  		if(part == 3) wert = ostrcat("3-", NULL, 0, 0);
  		if(part == 4) wert = ostrcat("4-", NULL, 0, 0);
  		if( system(cmd) != -1)
  		{
  			if(file_exist(idir))
  			{
  				datei = fopen (data, "r");
  				if(datei != NULL)
					{
						fgets(line, 50, datei);
						if(ostrstr(line, "opennfr") == NULL)
						{
							fgets(line, 50, datei);
						}
						pos = strstr(line, "\\n");
						if(pos != NULL)
						{
							pos[0] = '\0';
							//printf ("line: %s\n", line);
							wert = ostrcat(wert, line, 1, 0);
						}
						fclose (datei);
					}
					else
					wert = ostrcat(wert, "unknown", 1, 0);
					system("umount /tmp/multi");
				}
				else
				{
					wert = ostrcat(wert, "free", 1, 0);
					system("umount /tmp/multi");
				}
			}
			else
				wert = ostrcat(wert, "free", 1, 0);
			free(cmd);
	}
	else
		wert = ostrcat("can not be determined", NULL, 0, 0);
	system("rmdir /tmp/multi");

	free(link);
	free(data);
	free(line);
	free(idir);
	
	return wert;
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
	char *partname = NULL;
	int neustart = 0;
	int ret = 0;
	int end = 0;
	int part = 0;
	
	struct skin* screen = getscreen("multiboot");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* partitions = getscreennode(screen, "partitions");

	while(end == 0)
	{
		changechoiceboxvalue(partitions, NULL);
		
		tmpstr = readsys("/boot/STARTUP", 1);
		
		if(checkchipset("HI3798MV200") == 1)
		{
			tmpstr2 = ostrstr(tmpstr, "rootsubdir");
			if(tmpstr2 != NULL)
			{
				tmpstr2[23] = '\0';
				ownpart = ostrcat(tmpstr2+22, NULL, 0, 0);
				printf("----> own part: %s\n", ownpart);
 	 		}
 	 	}
		else
		{
			tmpstr2 = ostrstr(tmpstr, "kernel");
			if(tmpstr2 != NULL)
			{
				tmpstr2[7] = '\0';
				ownpart = ostrcat(tmpstr2+6, NULL, 0, 0);
 	 		}
 	 	}
 	 free(tmpstr); tmpstr = NULL;
 	 tmpstr2 = NULL;    		
		
		dir = opendir("/boot"); /* oeffne aktuelles Verzeichnis */
		if (dir)
		{
			if(islink("/dev/block/by-name/linuxrootfs"))
				part = 1;
			else if(islink("/dev/block/by-name/rootfs1"))
				part = 1;
			else if(islink("/dev/block/by-name/linuxkernel1"))
				part = 1;
				
			while (0 != (member = readdir(dir)))
			{
 	  		if(ostrcmp(".", member->d_name) != 0 && ostrcmp("..", member->d_name) != 0 && ostrcmp("STARTUP", member->d_name) != 0)
 		 		{
	   			if(checkchipset("HI3798MV200") == 1)
					{
	   				if(ostrstr(member->d_name, "STARTUP_LINUX") != NULL)
	   				{
	   			 		tmpstr = ostrcat("/boot/", member->d_name, 0, 0);
  	 					tmpstr2 = readsys(tmpstr, 1);
  		 				tmpstr3 = ostrstr(tmpstr2, "rootsubdir");
 	  					if(tmpstr3 != NULL)
 	  					{
 	  						tmpstr3[23] = '\0';
	   						if(ostrcmp(ownpart, tmpstr3+22) == 0)
	   						{
	   							ownpartname = ostrcat(member->d_name, NULL, 0, 0);
	   							printf("----> own: %s\n", ownpartname);
	   						}
	   					}
	   					if(part > 0 && part < 5)
	   					{
	   						partname = getmultinames(part);
	   						addchoicebox(partitions, member->d_name, partname);
	   						free(partname); partname = NULL;
	   						part = part + 1;
	   					}
	   					else
	   						addchoicebox(partitions, member->d_name, member->d_name);
	   				}
	   				else
	   					continue;
	   			}
	   			else
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
	   					if(part > 0 && part < 5)
	   					{
	   						partname = getmultinames(part);
	   						addchoicebox(partitions, member->d_name, partname);
	   						free(partname); partname = NULL;
	   						part = part + 1;
	   					}
	   					else
	   						addchoicebox(partitions, member->d_name, member->d_name);
	   				}
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
	
	