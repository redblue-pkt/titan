#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "MultiImage";
char plugindesc[] = "Start Images from USB device";
char pluginpic[] = "%pluginpath%/multiimage/MultiImage.png";

int pluginaktiv = 0;
//int pluginversion = PLUGINVERSION;
int pluginversion = 999999;

struct stimerthread* Multi_Image_thread = NULL;

void multiimage_thread()
{
	while (Multi_Image_thread->aktion != STOP)
	{
		sleep(3);
		if(file_exist("/tmp/multiende") == 1)
		{
			textbox(_("Message"), _("INFO\nImage extracted"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0);
			remove("/tmp/multiende");
			break;
		}
	}
	if(Multi_Image_thread->aktion == STOP)
	{
		system("killall multi_unpack.sh");
		system("killall ubireader_extract_files");
	}
	Multi_Image_thread = NULL;
}


char* find_multiimage_dev()
{
	char* mdev = NULL;
	char* path = NULL;
	
	char* pos = NULL;
	FILE* fd = NULL;
	char* fileline = NULL;
	fd = fopen("/proc/partitions", "r");
	if(fd == NULL)
	{
		err("open /proc/partitions");
		return NULL;
	}
	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return NULL;
	}
	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		pos = ostrstr(fileline, "sd");
		if(pos != NULL)
		{
			pos = string_newline(pos);
			if(strlen(pos) == 4)
			{
				path = getconfig("mountpath", NULL);
				path = ostrcat(path, "/", 0, 0);
				path = ostrcat(path, pos, 1, 0);
				path = ostrcat(path, "/", 1, 0);
				path = ostrcat(path, "titan_multi", 1, 0);
				if(file_exist(path) == 1)
				{
					mdev = ostrcat(mdev, pos, 1, 0);
				}
				free(path); path = NULL;
			}
		}
		if(mdev != NULL)
	 		break;
	}
	free(fileline);
	fclose(fd);	

	return mdev;
}


char* get_dir(char* path)
{
	struct dirent* dirzeiger;
	char* dirall = NULL;
	DIR *dir = opendir(path);
	if (dir != 0)
	{
		struct dirent* dirzeiger;
		while(0 != (dirzeiger = readdir(dir)))
		{
			if(dirzeiger->d_name[0] != '.')
			{
				dirall = ostrcat(dirall, dirzeiger->d_name, 1, 0);
				dirall = ostrcat(dirall, " \n", 1, 0);
			}
		}
		closedir(dir);
	}
	return dirall;
}
	

int no_mdev()
{
	char* mdev = NULL;
	char* path = NULL;
	char* dirall = NULL;
	char* cmd = NULL;
	struct skin* tmp = NULL;
	int ret = 0;	
	int rcret = 0;	
	 
	struct skin* multipart = getscreen("multipart");
	struct skin* listbox = getscreennode(multipart, "listbox");
	struct skin* device = getscreennode(multipart, "device");
	struct skin* text2 = getscreennode(multipart, "textbox2");
			
	char* pos = NULL;
	FILE* fd = NULL;
	char* fileline = NULL;
	fd = fopen("/proc/partitions", "r");
	if(fd == NULL)
	{
		err("open /proc/partitions");
		return 1;
	}
	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return 1;
	}
	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		pos = ostrstr(fileline, "sd");
		if(pos != NULL)
		{
			pos = string_newline(pos);
			if(strlen(pos) == 4)
			{
				if(mdev == NULL)
					mdev = ostrcat(mdev, pos, 1, 0);
				addchoicebox(device, pos, pos);
			}
		}
	}
	free(fileline);
	fclose(fd);	
	if(mdev == NULL)
	{
		textbox("MultiImage", _("No device available"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0);
		return 1;
	}
	setchoiceboxselection(device, mdev);
	path = getconfig("mountpath", NULL);
	path = ostrcat(path, "/", 0, 0);
	path = ostrcat(path, mdev, 1, 0);
	
	dirall = get_dir(path);
	free(path); path = NULL;
	
	changetext(text2, dirall);
	free(dirall); dirall = NULL;
	
	free(mdev); mdev=NULL;
	
	drawscreen(multipart, 0, 0);
	addscreenrc(multipart, listbox);
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(multipart, tmp);
		rcret = waitrc(multipart, 2000, 0);
		tmp = listbox->select; 
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "device") == 0)
		{
			mdev = ostrcat(mdev, device->ret, 1, 0);
			path = getconfig("mountpath", NULL);
			path = ostrcat(path, "/", 0, 0);
			path = ostrcat(path, mdev, 1, 0);
			dirall = get_dir(path);
			free(path); path = NULL;
			changetext(text2, dirall);
			free(dirall); dirall = NULL;
			free(mdev); mdev = NULL;
		}

		if(rcret == getrcconfigint("rcblue", NULL))
		{
			mdev = ostrcat(mdev, device->ret, 1, 0);
			cmd = ostrcat("umount /dev/", mdev, 0, 0);
			debug(81, "unmount cmd: %s", cmd);
			ret = system(cmd);
			free(cmd); cmd = NULL;
			break;
		}
		drawscreen(multipart, 0, 0);
	}
	delownerrc(multipart);
	clearscreen(multipart);
	
	if(mdev == NULL)
		return 1;	
	
	if(textbox("MultiImage", _("All data on this device will be deleted!\nOK?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
	{
		free(mdev); mdev = NULL;
		return 1;
	}
	//cmd = ostrcat("/sbin/cmd.sh mkfs.ext2.gui /dev/" , mdev, 0, 0);
	cmd = ostrcat("mkfs.ext2.gui /dev/" , mdev, 0, 0);
	debug(81, "format cmd: %s", cmd);
	ret = system(cmd);
	free(cmd); cmd = NULL;
	if(ret != 0)
	{
		textbox(_("Message"), _("ERROR\nPartition could not be created"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0);
		return 1;
	}
	path = getconfig("mountpath", NULL);
	path = ostrcat(path, "/", 0, 0);
	path = ostrcat(path, mdev, 1, 0);
	cmd = ostrcat("mount /dev/" , mdev, 0, 0);
	cmd = ostrcat(cmd, " ", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	//system(cmd);
	free(cmd); cmd = NULL;
	cmd = ostrcat(path, "/", 0, 0);
	cmd = ostrcat(cmd, "titan_multi", 0, 0);
	mkdir(cmd, 777);
	free(cmd); cmd = NULL;
	free(path); path = NULL;
	free(mdev); mdev = NULL;
	system ("echo MultiImage > /dev/vfd");	
	
	return 0;
}
	
int multiimage_screen(char* mdev)
{
	struct skin* tmp = NULL;
	char* path = NULL;
	char* cmd = NULL;
	struct dirent* dirzeiger;	
	DIR *dir = NULL;
	int rcret = 0;	
	int rc = 0;
	int test = 0;
	
	struct skin* multiimage = getscreen("multiimage");
	struct skin* listbox = getscreennode(multiimage, "listbox");
	struct skin* images = getscreennode(multiimage, "images");
	struct skin* chnode1 = NULL;
	
	path = getconfig("mountpath", NULL);
	path = ostrcat(path, "/", 0, 0);
	path = ostrcat(path, mdev, 1, 0);
	path = ostrcat(path, "/", 1, 0);
	path = ostrcat(path,"titan_multi", 1, 0);
	
	delmarkedscreennodes(multiimage, 1);
	listbox->aktpage = -1;
	listbox->aktline = 1;
	
	dir = opendir(path);
	if (dir != 0)
	{
		while(0 != (dirzeiger = readdir(dir)))
		{
			if(dirzeiger->d_name[0] != '.')
			{
				test = 1;
				chnode1 = addlistbox(multiimage, listbox, chnode1, 1);
				if(chnode1 != NULL)
				{
					chnode1->posy = images->posy;
					chnode1->width = images->width;
					chnode1->height = images->height;
					chnode1->bordersize = images->bordersize;
					chnode1->bordercol = images->bordercol;
					chnode1->prozwidth = images->prozwidth;
					chnode1->deaktivcol = images->deaktivcol;
					chnode1->name = ostrcat(dirzeiger->d_name, NULL, 0, 0);
					changetext(chnode1, dirzeiger->d_name);
				}
			}
		}
		closedir(dir);
		if(test == 0)
			images->hidden = NO;
		else
			images->hidden = YES;
	}
	drawscreen(multiimage, 0, 0);
	addscreenrc(multiimage, listbox);
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(multiimage, tmp);
		rcret = waitrc(multiimage, 2000, 0);
		tmp = listbox->select; 
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			 rc = 0;
			 break;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL)
			{	
				rc = 1;
				cmd = ostrcat(_("Sure to delete Image?\n"), listbox->select->name, 0, 0);
				if(textbox("MultiImage", cmd, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
				{
					free(cmd); cmd= NULL;
					cmd = ostrcat("rm -r " , path, 0, 0);
					cmd = ostrcat(cmd , "/", 1, 0);
					cmd = ostrcat(cmd , listbox->select->name, 1, 0);
					debug(81, "delete cmd: %s", cmd);
					system(cmd);
				}
				free(cmd); cmd= NULL;
				break;
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			rc = 2;
			break;
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			rc = 3;
			break;
		}
		drawscreen(multiimage, 0, 0);
	}
	delownerrc(multiimage);
	delmarkedscreennodes(multiimage, 1);
	clearscreen(multiimage);
	free(path); path=NULL;
	
	return rc;
}

int multiimage_install(char* imagefile, char* mdev)
{
	int rcret = 0;
	int rc = 0;
	char* path = NULL;
	char* path2 = NULL;
	char* path3 = NULL;
	char* cmd = NULL;
	char* temp = NULL;
	char* iname = NULL;

	struct skin* tmp = NULL;
	struct skin* multiinstall = getscreen("multiinstall");
	struct skin* listbox = getscreennode(multiinstall, "listbox");
	struct skin* text1 = getscreennode(multiinstall, "text1");
	struct skin* imagename = getscreennode(multiinstall, "imagename");
	
	path = getconfig("mountpath", NULL);
	path = ostrcat(path, "/", 0, 0);
	path = ostrcat(path, mdev, 1, 0);

	path2 = ostrcat(path, "/", 0, 0);
	path2 = ostrcat(path2,"titan_multi", 1, 0);
		
	changetext(text1, imagefile);
	changeinput(imagename, "imagename");
	
	drawscreen(multiinstall, 0, 0);
	addscreenrc(multiinstall, listbox);
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(multiinstall, tmp);
		rcret = waitrc(multiinstall, 2000, 0);
		tmp = listbox->select; 
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			textbox("Message", _("The install process will look in /tmp for an image.\n Please copy the image to /tmp as zip file."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, 800, 200, 0, 0);			
			path3 = ostrcat(path2, "/", 0, 0);
			path3 = ostrcat(path3, imagename->ret, 1, 0);
			if(file_exist(path3) == 1)
			{
				free(path3); path3=NULL;
				textbox(_("Message"), _("ERROR\nImage already present!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0);
				continue;
			}
			textbox(_("Message"), _("INFO\nExtracting will take a few minutes ..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			iname = ostrcat(imagename->ret, NULL, 1, 0);
			rc = 1;
			break;
		}
		drawscreen(multiinstall, 0, 0);
	}
	delownerrc(multiinstall);
	clearscreen(multiinstall);
	if(rc == 1)
	{
		temp = createpluginpath("/multiimage/ubireader/ubi_reader-master/scripts/ubireader_extract_files", 0);
		cmd = createpluginpath("/multiimage/multi_unpack.sh", 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, imagefile, 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, iname, 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, path, 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, temp, 1, 0);
		cmd = ostrcat(cmd, " &", 1, 0);
		system(cmd);
		free(cmd); cmd=NULL;
		free(temp); temp=NULL;
		textbox(_("Message"), _("INFO\nExtracting process is running in background.\nA message will be shown when finished."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 250, 10, 0);
		
		Multi_Image_thread = addtimer(&multiimage_thread, START, 10000, 1, NULL, NULL, NULL);
	}

	free(path); path=NULL;	
	free(path2); path2=NULL;	
	free(path3); path3=NULL;	
	return rc;
}

void multi_main(void)
{
	char* mdev = NULL;
	char* imagefile = NULL;
	int ret = 0;
		
	mdev = find_multiimage_dev();
	
	while (mdev == NULL)
	{
		if(textbox("MultiImage", _("No MultiImage device found.\nCreate device?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0) == 2)
			return;
		if(no_mdev() != 0)
			return;		
		mdev = find_multiimage_dev();
	}
	while(1)
	{
		ret = multiimage_screen(mdev);
		
		if(ret == 0)
			break;
		if(ret == 2)
		{
			imagefile = screendir("/tmp", "*.zip", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 1200, 0, 600, 0, 0);
			if(imagefile != NULL)
			{
				ret = multiimage_install(imagefile, mdev);
				free(imagefile); imagefile=NULL;
				if(ret == 1)
					return;
			}
		}
		if(ret == 3)
		{
			if(no_mdev() != 0)
				continue;		
			free(mdev); mdev=NULL;
			mdev = find_multiimage_dev();
		}
	}
	free(mdev); mdev=NULL;
}
	
	
		
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	
	tmpstr = createpluginpath("/multiimage/skin.xml", 0);
	readscreen(tmpstr, 122, 1);

	debug(10, "MultiImage Plugin loaded!!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(Multi_Image_thread != NULL)
	{
		Multi_Image_thread->aktion = STOP;
		while (Multi_Image_thread != NULL)
		{
			sleep(1);
		}
	}
	pluginaktiv = 0;
	debug(10, "MultiImage removed !!!");
	delmarkedscreen(122);
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	if(Multi_Image_thread != NULL)
	{
		if(textbox("MultiImage", _("Image creation running!!!\nStop process?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0) == 2)
			return;
		Multi_Image_thread->aktion = STOP;
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
		while (Multi_Image_thread != NULL)
		{
			sleep(1);
		}
		clearscreen(load);
	}

	multi_main();
}
