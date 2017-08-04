#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "MultiImage";
char plugindesc[] = "Start Images from USB device";
char pluginpic[] = "%pluginpath%/multiimage/MultiImage.png";

int pluginaktiv = 0;
//int pluginversion = PLUGINVERSION;
int pluginversion = 999999;

char* imagepath = NULL;

struct stimerthread* Multi_Image_thread = NULL;

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

void multiimage_thread()
{
	char* cmd = NULL;
	while (Multi_Image_thread->aktion != STOP)
	{
		sleep(3);
		if(file_exist("/tmp/multiende") == 1)
		{
			textbox(_("Message"), _("INFO\nImage extracted"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 500, 200, 0, 0);
			remove("/tmp/multiende");
			break;
		}
	}
	if(Multi_Image_thread->aktion == STOP)
	{
		system("killall multi_unpack.sh");
		system("killall ubireader_extract_files");
		cmd = ostrcat("rm -r ",imagepath, 0, 0);
		system(cmd);
		free(cmd); cmd = NULL;

	}
	else
	{
		if(status.extplugin != NULL)
		{
			free(status.extplugin);
			status.extplugin = NULL;
		}
		//Aufruf des plugins
		status.extplugin = ostrcat("MultiImage", NULL, 0, 0);
		writerc(getrcconfigint("rcexit", NULL));
	}
	free(imagepath); imagepath=NULL;
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
		else
		{
			pos = ostrstr(fileline, "mmcblk1p1");
			if(pos != NULL)
			{
				pos = string_newline(pos);
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


int no_mdev()
{
	char* mdev = NULL;
	char* path = NULL;
	char* dirall = NULL;
	char* cmd = NULL;
	struct skin* tmp = NULL;
	int ret = 0;
	int rcret = 0;
	int ren = 0;

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
		else
		{
			pos = ostrstr(fileline, "mmcblk1p1");
			if(pos != NULL)
			{
				pos = string_newline(pos);
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
		textbox("MultiImage", _("No device available"), _("OK"), getrcconfigint("rcok", NULL),NULL, 0, NULL, 0, NULL, 0, 500, 200, 0, 0);
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

		if(rcret == getrcconfigint("rcred", NULL))
		{
			mdev = ostrcat(mdev, device->ret, 1, 0);
			cmd = ostrcat("umount /dev/", mdev, 0, 0);
			debug(81, "unmount cmd: %s", cmd);
			ret = system(cmd);
			free(cmd); cmd = NULL;
			break;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			mdev = ostrcat(mdev, device->ret, 1, 0);
			path = getconfig("mountpath", NULL);
			path = ostrcat(path, "/", 0, 0);
			path = ostrcat(path, mdev, 1, 0);

			cmd = ostrcat("mount | grep ", mdev, 0, 0);
			cmd = ostrcat(cmd, " | grep ext[2,3,4]", 1, 0);
			ret = system(cmd);
			free(cmd); cmd = NULL;
			if(ret == 0)
			{
				cmd = ostrcat(path, "/", 0, 0);
				cmd = ostrcat(cmd, "titan_multi", 1, 0);
				mkdir(cmd, 777);
				free(cmd); cmd=NULL;
				ren = 1;
			}
			else
			{
				textbox(_("ERROR"), _("No Linux partition found on this device."), _("OK"), getrcconfigint("rcok", NULL),NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				ren = 0;
			}
			free(path); path=NULL;
			free(mdev); mdev=NULL;
			if(ren == 1)
				break;
		}
		drawscreen(multipart, 0, 0);
	}
	delownerrc(multipart);
	clearscreen(multipart);

	if(mdev == NULL)
		return 1;

	if(ren != 1)
	{
		if(textbox("MultiImage", _("All data on this device will be deleted!\nOK?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 650, 200, 0, 0) == 2)
		{
			free(mdev); mdev = NULL;
			return 1;
		}
		//cmd = ostrcat("mkfs.ext2.gui /dev/" , mdev, 0, 0);
		//cmd = ostrcat("mkfs.ext3.gui -E lazy_itable_init=1 /dev/" , mdev, 0, 0);
		cmd = ostrcat("mkfs.ext3.gui -F /dev/" , mdev, 0, 0);
		debug(81, "format cmd: %s", cmd);
		int merksec = status.sec;
		status.sec = 0; //deaktiviere Spinner
		ret = system(cmd);
		status.sec = merksec;
		free(cmd); cmd = NULL;
		if(ret != 0)
		{
			textbox(_("ERROR"), _("Partition could not be created"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 500, 200, 0, 0);
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
		cmd = ostrcat(cmd, "titan_multi", 1, 0);
		mkdir(cmd, 777);
		free(cmd); cmd = NULL;
		free(path); path = NULL;
		free(mdev); mdev = NULL;
		system ("echo MultiImage > /dev/vfd");
	}

	return 0;
}

// type 1 = install image
// type 2 = rename image
int multiimage_install(char* imagefile, char* mdev, int type)
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
	struct skin* b2 = getscreennode(multiinstall, "b2");
	if(type == 1)
	{
		changetext(b2, _("INSTALL"));
		changeinput(imagename, "imagename");
	}
	if(type == 2)
	{
		changetext(b2, _("RENAME"));
		changeinput(imagename, imagefile);
	}

	path = getconfig("mountpath", NULL);
	path = ostrcat(path, "/", 0, 0);
	path = ostrcat(path, mdev, 1, 0);

	path2 = ostrcat(path, "/", 0, 0);
	path2 = ostrcat(path2,"titan_multi", 1, 0);

	changetext(text1, imagefile);


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
			path3 = ostrcat(path2, "/", 0, 0);
			path3 = ostrcat(path3, imagename->ret, 1, 0);
			if(file_exist(path3) == 1)
			{
				free(path3); path3=NULL;
				textbox(_("ERROR"), _("Image already present!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 500, 200, 0, 0);
				continue;
			}
			if(type == 1)
			{
				iname = ostrcat(imagename->ret, NULL, 1, 0);
				rc = 1;
				break;
			}
			if(type == 2)
			{
				temp = ostrcat(path2, "/", 0, 0);
				temp = ostrcat(temp, imagefile, 1, 0);
				cmd = ostrcat("mv ", temp, 0, 0);
				cmd = ostrcat(cmd, " ", 1, 0);
				cmd = ostrcat(cmd, path3, 1, 0);
				system(cmd);
				free(cmd); cmd=NULL;
				free(temp); temp=NULL;
				rc = 2;
				break;
			}
		}
		drawscreen(multiinstall, 0, 0);
	}
	delownerrc(multiinstall);
	clearscreen(multiinstall);
	if(rc == 1)
	{
		if(imagepath != NULL)
			free(imagepath);
		imagepath = NULL;
		imagepath = ostrcat(path2, "/", 0, 0);
		imagepath = ostrcat(imagepath, iname, 1, 0);
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
		textbox(_("Message"), _("Extracting will take a few minutes ...\n\nProcess is running in background.\nA message will be shown when finished."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 300, 10, 0);
		Multi_Image_thread = addtimer(&multiimage_thread, START, 10000, 1, NULL, NULL, NULL);
	}

	free(path); path=NULL;
	free(path2); path2=NULL;
	free(path3); path3=NULL;
	return rc;
}

int multiimage_screen(char* mdev)
{
	struct skin* tmp = NULL;
	char* path = NULL;
	char* cmd = NULL;
	char* tmpstr = NULL;
	char* selimage = NULL;
	struct dirent* dirzeiger;
	DIR *dir = NULL;
	int rcret = 0;
	int rc = 0;
	int test = 0;
	int test2 = 0;

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

	tmpstr = ostrcat(path, "/", 0, 0);
	tmpstr = ostrcat(tmpstr, ".multi_image", 1, 0);
	selimage = readsys(tmpstr, 1);
	free(tmpstr); tmpstr=NULL;

	test2 = 0;
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
					if(selimage != NULL && ostrcmp(selimage, dirzeiger->d_name) == 0)
					{
						tmpstr = ostrcat(dirzeiger->d_name, "  (selected)", 0, 0);
						changetext(chnode1, tmpstr);
						free(tmpstr); tmpstr=NULL;
						test2 = 1;
					}
					else
						changetext(chnode1, dirzeiger->d_name);
				}
			}
		}
		closedir(dir);
		free(selimage); selimage = NULL;
		if(test == 0)
			images->hidden = NO;
		else
		{
			images->hidden = YES;
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
				chnode1->name = ostrcat("Flash", NULL, 0, 0);
				if(test2 == 0)
					changetext(chnode1, "Flash  (selected)");
				else
					changetext(chnode1, "Flash");
			}
		}
	}
	drawscreen(multiimage, 0, 0);
	addscreenrc(multiimage, listbox);
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(multiimage, tmp);
		rcret = waitrc(multiimage, 2000, 0);
		tmp = listbox->select;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				rc = 1;
				tmpstr = ostrcat(path, "/", 0, 0);
				tmpstr = ostrcat(tmpstr, ".multi_image", 1, 0);
				writesys(tmpstr, listbox->select->name, 0);
				free(tmpstr); tmpstr=NULL;
				break;
			}
		}
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
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			rc = 1;
			selimage = ostrcat(listbox->select->name, NULL, 0, 0);
			break;
		}
		if(rcret == getrcconfigint("rcpower", NULL))
		{
			rc = 4;
			break;
		}
		drawscreen(multiimage, 0, 0);
	}
	delownerrc(multiimage);
	delmarkedscreennodes(multiimage, 1);
	clearscreen(multiimage);
	free(path); path=NULL;
	if(selimage != NULL)
	{
		multiimage_install(selimage, mdev, 2);
		free(selimage); selimage = NULL;
	}
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
		if(ret == 1)
			continue;
		if(ret == 2)
		{
			textbox(_("Message"), _("The install process will look in /tmp for an image zip file.\nPlease copy the image to /tmp.\nThe filename must not contain blanks."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
			imagefile = screendir("/tmp", "*.zip *.xz", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 1200, 0, 600, 0, 0);
			if(imagefile != NULL)
			{
				ret = multiimage_install(imagefile, mdev, 1);
				free(imagefile); imagefile=NULL;
				if(ret == 1)
				{
					free(mdev); mdev=NULL;
					return;
				}
			}
		}
		if(ret == 3)
		{
			if(no_mdev() != 0)
				continue;
			free(mdev); mdev=NULL;
			mdev = find_multiimage_dev();
		}
		if(ret == 4)
		{
			oshutdown(2, 1);
			break;
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
