#ifndef HARDDISK_H
#define HARDDISK_H

int checkfilesystemexist(char* filesystem)
{
	int ret = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	tmpstr = readfiletomem("/proc/filesystems", 0);

	tmpstr1 = ostrcat("\t", filesystem, 0, 0);
	tmpstr1 = ostrcat(tmpstr1, "\n", 0, 0);

	if(ostrstr(tmpstr, tmpstr1) != NULL)
		ret = 1;

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;

	return ret;
}

char* blocktogb(unsigned long long size)
{
	char* buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	snprintf(buf, MINMALLOC, "%.2f", (float)size / 2 / 1024 / 1024); //size is in 512KB blocks
	return ostrshrink(buf);
}

struct hdd* gethdd(char* device)
{
	struct hdd *node = hdd;

	while(node != NULL)
	{
		if(ostrcmp(node->device, device) == 0)
			return node;
		node = node->next;
	}
	debug(100, "hdd not found (device=%s)", device);
	return NULL;
}

unsigned long long hddgetsize(char* device, char* partition)
{
	char* file = NULL;
	unsigned long long size = 0;

	file = ostrcat(file , "/sys/block/", 1, 0);
	file = ostrcat(file , device, 1, 0);
	if(partition != NULL)
	{
		file = ostrcat(file , "/", 1, 0);
		file = ostrcat(file , partition, 1, 0);
	}
	file = ostrcat(file , "/size", 1, 0);
	size = readsysull(file, 1);
	debug(80, "file: %s, size: %lu", file, size);
	free(file); file = NULL;

	return size;
}

char* hddgetvendor(char* device)
{
	char* file = NULL, *vendor = NULL;

	file = ostrcat(file , "/sys/block/", 1, 0);
	file = ostrcat(file , device, 1, 0);
	file = ostrcat(file , "/device/vendor", 1, 0);
	vendor = strstrip(readsys(file, 1));
	debug(80, "file: %s, vendor: %s", file, vendor);
	free(file); file = NULL;

	return vendor;
}

char* hddgetmodel(char* device)
{
	char* file = NULL, *model = NULL;

	file = ostrcat(file , "/sys/block/", 1, 0);
	file = ostrcat(file , device, 1, 0);
	file = ostrcat(file , "/device/model", 1, 0);
	model = strstrip(readsys(file, 1));
	debug(80, "file: %s, model: %s", file, model);
	free(file); file = NULL;

	return model;
}

int hddgetremovable(char* device)
{
	char* file = NULL;
	int removable = 0;

	file = ostrcat(file , "/sys/block/", 1, 0);
	file = ostrcat(file , device, 1, 0);
	file = ostrcat(file , "/removable", 1, 0);
	removable = readsysul(file, 1);
	debug(80, "file: %s, removable: %d", file, removable);
	free(file); file = NULL;

	return removable;
}

void screenfilesystem(char* dev)
{
	int i, rcret = 0, count = 1;
	struct skin* screen = getscreen("harddisk_main");
	struct skin* details = getscreennode(screen, "details");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	delmarkedscreennodes(screen, 1);
	listbox->aktpage = -1;
	listbox->aktline = 1;
	changetitle(screen, _("Harddisk Format - Filesystem"));
	if(details != status.skinerr) changetext(details, _("select the format filesystem type for your media drive"));
		
	if(status.expertmodus > 9) count = 5;

	for(i = 0; i < count; i++)
	{
		if(i == 0 && checkfilesystemexist("ext3") == 0) continue;
		else if(i == 1 && checkfilesystemexist("ext4") == 0) continue;
		else if(i == 2 && checkfilesystemexist("ext2") == 0) continue;
		else if(i == 3 && checkfilesystemexist("jfs") == 0) continue;
		else if(i == 4 && checkfilesystemexist("vfat") == 0) continue;
		
		tmp = addlistbox(screen, listbox, tmp, 1);
		if(tmp != NULL)
		{
			if(i == 0) tmpstr = "ext3";
			else if(i == 1) tmpstr = "ext4";	
			else if(i == 2) tmpstr = "ext2";
			else if(i == 3) tmpstr = "jfs";
			else if(i == 4) tmpstr = "vfat";
			tmpstr1 = ostrcat(tmpstr1, getconfig("skinpath", NULL), 1, 0);
			tmpstr1 = ostrcat(tmpstr1, "/skin/", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
			tmpstr1 = ostrcat(tmpstr1, ".png", 1, 0);

			if(!file_exist(tmpstr1))
			{
				free(tmpstr1); tmpstr1 = NULL;
				tmpstr1 = ostrcat(tmpstr1, getconfig("skinpath", NULL), 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/skin/default.png", 1, 0);
			}
			changepic(tmp, tmpstr1);
			free(tmpstr1); tmpstr1 = NULL;

			changename(tmp, tmpstr);
			if(i == 0)
			{
				tmpstr = ostrcat(tmpstr, " (", 0, 0);
				tmpstr = ostrcat(tmpstr, _("default"), 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			else
				changetext(tmp, tmpstr);

			tmp->textposx = 120;
			tmp->height = 50;
			tmp->valign = convertxmlentry("middle", 0);
			tmp->hspace = 5;
		}
	}

	drawscreen(screen, 0, 0);
	addscreenrc(screen, listbox);

	while (1)
	{
		rcret = waitrc(screen, 0, 0);

		if(rcret==getrcconfigint("rcexit", NULL)) break;
		if(listbox->select != NULL && rcret==getrcconfigint("rcok", NULL))
		{
			hddformat(dev, listbox->select->name);
			break;
		}
	}
	delownerrc(screen);
	delmarkedscreennodes(screen, 1);
	clearscreen(screen);
}

void screenconfigurehdd(char* dev)
{
	int i, y = 1, rcret = 0, ret = 0;
	struct skin* screen = getscreen("harddisk_main");
	struct skin* details = getscreennode(screen, "details");
	struct skin* listbox = getscreennode(screen, "listbox");

	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *path = NULL, *mountpath = NULL, *cmd = NULL;
	int record = 0, swapextensions = 0, swapdir = 0, backup = 0;
	delmarkedscreennodes(screen, 1);
	listbox->aktpage = -1;
	listbox->aktline = 1;
	changetitle(screen, _("Harddisk Configure"));
	if(details != status.skinerr) changetext(details, _("select the function for the specified device"));

	mountpath = getconfig("mountpath", NULL);
	mountpath = ostrcat(mountpath, "/", 0, 0);
	mountpath = ostrcat(mountpath, dev, 1, 0);
	mountpath = ostrcat(mountpath, "/", 1, 0);
	
	if(status.expertmodus > 9) y = 4;
	for(i = 0; i < y; i++)
	{			
		tmp = addlistbox(screen, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = CHOICEBOX;
			if(i == 0)
			{
				tmpstr = _("use medium for record");
				tmpstr1 = "record";
				path = ostrcat(mountpath, "movie", 0, 0);
				if(file_exist(path))
				{
					addchoicebox(tmp, "1", _("yes"));			
					addchoicebox(tmp, "0", _("no"));
					record = 1;
				}
				else
				{
					addchoicebox(tmp, "0", _("no"));
					addchoicebox(tmp, "1", _("yes"));			
					record = 0;
				}
				free(path), path = NULL;
			}
			else if(i == 1)
			{
				tmpstr = _("use medium for extension");
				tmpstr1 = "swapextensions";
				path = ostrcat(mountpath, "swapextensions", 0, 0);
				if(file_exist(path))
				{
					addchoicebox(tmp, "1", _("yes"));			
					addchoicebox(tmp, "0", _("no"));
					swapextensions = 1;
				}
				else
				{
					addchoicebox(tmp, "0", _("no"));
					addchoicebox(tmp, "1", _("yes"));			
					swapextensions = 0;
				}
				free(path), path = NULL;
			}
			else if(i == 2)
			{
				tmpstr = _("use medium for swap");
				tmpstr1 = "swapdir";
				path = ostrcat(mountpath, "swapdir", 0, 0);
				if(file_exist(path))
				{
					addchoicebox(tmp, "1", _("yes"));			
					addchoicebox(tmp, "0", _("no"));
					swapdir = 1;
				}
				else
				{
					addchoicebox(tmp, "0", _("no"));
					addchoicebox(tmp, "1", _("yes"));			
					swapdir = 0;
				}
				free(path), path = NULL;
			}
			else if(i == 3)
			{
				tmpstr = _("use medium for backup");
				tmpstr1 = "backup";
				path = ostrcat(mountpath, "backup", 0, 0);
				if(file_exist(path))
				{
					addchoicebox(tmp, "1", _("yes"));			
					addchoicebox(tmp, "0", _("no"));
					backup = 1;
				}
				else
				{
					addchoicebox(tmp, "0", _("no"));
					addchoicebox(tmp, "1", _("yes"));			
					backup = 0;
				}
				free(path), path = NULL;
			}

			tmpstr2 = ostrcat(tmpstr2, getconfig("skinpath", NULL), 1, 0);
			tmpstr2 = ostrcat(tmpstr2, "/skin/", 1, 0);
			tmpstr2 = ostrcat(tmpstr2, tmpstr1, 1, 0);
			tmpstr2 = ostrcat(tmpstr2, ".png", 1, 0);

			if(!file_exist(tmpstr2))
			{
				free(tmpstr2); tmpstr2 = NULL;
				tmpstr2 = ostrcat(tmpstr2, getconfig("skinpath", NULL), 1, 0);
				tmpstr2 = ostrcat(tmpstr2, "/skin/default.png", 1, 0);
			}
			changepic(tmp, tmpstr2);
			free(tmpstr2); tmpstr2 = NULL;

			changetext(tmp, tmpstr);
			changename(tmp, tmpstr1);
			tmp->textposx = 120;
			tmp->height = 50;
			tmp->valign = convertxmlentry("middle", 0);
			tmp->hspace = 5;
		}
	}

	drawscreen(screen, 0, 0);
	tmp = listbox->select;

	while (1)
	{
		addscreenrc(screen, tmp);

		rcret = waitrc(screen, 0, 0);
		tmp = listbox->select;

		if(ostrcmp(listbox->select->name, "record") == 0)
			record = atoi(listbox->select->ret);
		if(ostrcmp(listbox->select->name, "swapextensions") == 0)
			swapextensions = atoi(listbox->select->ret);
		if(ostrcmp(listbox->select->name, "swapdir") == 0)
			swapdir = atoi(listbox->select->ret);
		if(ostrcmp(listbox->select->name, "backup") == 0)
			backup = atoi(listbox->select->ret);

		if(rcret==getrcconfigint("rcexit",NULL)) break;
		if(listbox->select != NULL && rcret == getrcconfigint("rcok", NULL))
		{
			if(record == 0)
			{
				path = ostrcat(mountpath, "movie", 0, 0);
				if(file_exist(path))
				{
					if(textbox("Message", _("Are you sure, you want to delete the record directory on this drive?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
					{
						drawscreen(load, 0, 0);
						cmd = ostrcat("rm -rf ", path, 0, 0);
						system(cmd);
						free(cmd); cmd = NULL;
						system("hotplug.sh delRecord");
						clearscreen(load);
					}
				}
				free(path), path = NULL;
			}
			else if(record == 1)
			{
				path = ostrcat(mountpath, "movie", 0, 0);
				if(!file_exist(path))
				{
					drawscreen(load, 0, 0);
					ret = mkdir(path, 0777);				
					system("hotplug.sh first");
					clearscreen(load);
				}
				free(path), path = NULL;
			}

			if(swapextensions == 0)
			{
				path = ostrcat(mountpath, "swapextensions", 0, 0);
				if(file_exist(path))
				{
					drawscreen(load, 0, 0);
					cmd = ostrcat("rm -rf ", path, 0, 0);
					system(cmd);
					free(cmd); cmd = NULL;
					system("hotplug.sh delSwapextensions");
					clearscreen(load);
				}
				free(path), path = NULL;
			}
			else if(swapextensions == 1)
			{
				path = ostrcat(mountpath, "swapextensions", 0, 0);
				if(!file_exist(path))
				{
					drawscreen(load, 0, 0);
					ret = mkdir(path, 0777);				
					system("hotplug.sh first");
					clearscreen(load);
				}
				free(path), path = NULL;
			}

			if(swapdir == 0)
			{
				path = ostrcat(mountpath, "swapdir", 0, 0);
				if(file_exist(path))
				{
					drawscreen(load, 0, 0);
					system("hotplug.sh delSwap");
					cmd = ostrcat("rm -rf ", path, 0, 0);
					system(cmd);
					free(cmd); cmd = NULL;
					clearscreen(load);
				}
				free(path), path = NULL;
			}
			else if(swapdir == 1)
			{
				path = ostrcat(mountpath, "swapdir", 0, 0);
				if(!file_exist(path))
				{
					drawscreen(load, 0, 0);
					ret = mkdir(path, 0777);				
					system("hotplug.sh first");
					clearscreen(load);
				}
				free(path), path = NULL;
			}

			if(backup == 0)
			{
				path = ostrcat(mountpath, "backup", 0, 0);
				if(file_exist(path))
				{
					drawscreen(load, 0, 0);
					cmd = ostrcat("rm -rf ", path, 0, 0);
					system(cmd);
					free(cmd); cmd = NULL;
					system("hotplug.sh delBackup");
					clearscreen(load);
				}
				free(path), path = NULL;
			}
			else if(backup == 1)
			{
				path = ostrcat(mountpath, "backup", 0, 0);
				if(!file_exist(path))
				{
					drawscreen(load, 0, 0);
					ret = mkdir(path, 0777);				
					system("hotplug.sh first");
					clearscreen(load);
				}
				free(path), path = NULL;
			}
			break;
		}
	}

	free(mountpath), mountpath = NULL;
	delownerrc(screen);
	delmarkedscreennodes(screen, 1);
	clearscreen(screen);
}

void screenharddisk(int mode)
{
	struct skin* screen = getscreen("harddisk_main");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* details = getscreennode(screen, "details");
	struct skin* tmp = NULL;
	struct hdd* hddnode = NULL;
	int tmphangtime = 999999, rcret = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *path = NULL;

	addhddall();
start:
	tmp = NULL;
	hddnode = hdd;

	if(hdd == NULL)
	{
		textbox(_("Harddisk"), _("sorry found no harddisk"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 600, 0, 0);
		return;
	}

	status.hangtime = tmphangtime;
	delmarkedscreennodes(screen, 1);
	if(mode == 0)
	{
		changetitle(screen, _("Harddisk Format"));
		if(details != status.skinerr) changetext(details, _("select the media drive from Device List for formating"));
	}
	else if(mode == 1)
	{
		changetitle(screen, _("Harddisk Fsck"));
		if(details != status.skinerr) changetext(details, _("select the media drive from Device List for fsck"));
	}
	else if(mode == 2)
	{
		changetitle(screen, _("Harddisk Configure"));
		if(details != status.skinerr) changetext(details, _("select the media drive from Device List for configuration"));
	}

	listbox->aktpage = -1;
	listbox->aktline = 1;

	while(hddnode != NULL)
	{

		path = getconfig("mountpath", NULL);
		path = ostrcat(path, "/", 0, 0);
		path = ostrcat(path, hddnode->device, 1, 0);

		if(mode == 2 && !file_exist(path) && hddnode->partition == 0)
		{
			hddnode = hddnode->next;
			free(path); path = NULL;
			continue;
		}
		free(path); path = NULL;

		tmp = addlistbox(screen, listbox, tmp, 1);
		if(tmp != NULL)
		{
			if(hddnode->removable == 0)
			{
				tmpstr = ostrcat(tmpstr, _("HDD "), 1, 0);
				tmpstr1 = ostrcat(tmpstr1, getconfig("skinpath", NULL), 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/skin/hdd.png", 1, 0);
			}
			else
			{
				tmpstr = ostrcat(tmpstr, _("STICK "), 1, 0);
				tmpstr1 = ostrcat(tmpstr1, getconfig("skinpath", NULL), 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/skin/stick.png", 1, 0);
			}
			if(mode == 2)
			{
				tmpstr = ostrcat(tmpstr, hddnode->label, 1, 0);
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, hddnode->device, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);

				path = getconfig("mountpath", NULL);
				path = ostrcat(path, "/", 0, 0);
				path = ostrcat(path, hddnode->device, 1, 0);
				path = ostrcat(path, "/", 1, 0);
				path = ostrcat(path, "swapextensions", 1, 0);
				if(file_exist(path) == 1)
				{
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, _("ext"), 1, 0);
				}
				free(path); path = NULL;

				path = getconfig("mountpath", NULL);
				path = ostrcat(path, "/", 0, 0);
				path = ostrcat(path, hddnode->device, 1, 0);
				path = ostrcat(path, "/", 1, 0);
				path = ostrcat(path, "swapdir", 1, 0);
				if(file_exist(path) == 1)
				{
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, _("swap"), 1, 0);
				}
				free(path); path = NULL;

				path = getconfig("mountpath", NULL);
				path = ostrcat(path, "/", 0, 0);
				path = ostrcat(path, hddnode->device, 1, 0);
				path = ostrcat(path, "/", 1, 0);
				path = ostrcat(path, "backup", 1, 0);
				if(file_exist(path) == 1)
				{
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, _("backup"), 1, 0);
				}
				free(path); path = NULL;

				path = getconfig("mountpath", NULL);
				path = ostrcat(path, "/", 0, 0);
				path = ostrcat(path, hddnode->device, 1, 0);
				path = ostrcat(path, "/", 1, 0);
				path = ostrcat(path, "movie", 1, 0);
				if(file_exist(path) == 1)
				{
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, _("record"), 1, 0);
				}
				free(path); path = NULL;
			}
			else
			{
				tmpstr = ostrcat(tmpstr, "(", 1, 0);
				tmpstr = ostrcat(tmpstr, hddnode->device, 1, 0);
				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, hddnode->label, 1, 0);
				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, hddnode->filesystem, 1, 0);
				tmpstr = ostrcat(tmpstr, ") ", 1, 0);
				tmpstr = ostrcat(tmpstr, hddnode->model, 1, 0);
				tmpstr = ostrcat(tmpstr, " ", 1, 0);
				tmpstr = ostrcat(tmpstr, blocktogb(hddnode->size), 1, 1);
				tmpstr = ostrcat(tmpstr, " GB", 1, 0);
			}
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;

			changename(tmp, hddnode->device);
			tmp->textposx = 120;
			tmp->height = 50;
			tmp->valign = convertxmlentry("middle", 0);
			tmp->hspace = 5;

			if(!file_exist(tmpstr1))
			{
				free(tmpstr1); tmpstr1 = NULL;
				tmpstr1 = ostrcat(tmpstr1, getconfig("skinpath", NULL), 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "/skin/default.png", 1, 0);
			}
			changepic(tmp, tmpstr1);
			free(tmpstr1); tmpstr1 = NULL;
		}
		hddnode = hddnode->next;
	}

	drawscreen(screen, 0, 0);
	addscreenrc(screen, listbox);

	while (1)
	{
		rcret = waitrc(screen, 0, 0);

		if(rcret==getrcconfigint("rcexit",NULL)) break;
		if(listbox->select != NULL && rcret==getrcconfigint("rcok",NULL))
		{
			if(mode == 0)
			{
				tmpstr = ostrcat(listbox->select->name, NULL, 0, 0);
				screenfilesystem(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			else if(mode == 1)
			{
				if(hddfsck(listbox->select->name) == 1)
					continue;
			}
			else if(mode == 2)
			{
				tmpstr = ostrcat(listbox->select->name, NULL, 0, 0);
				screenconfigurehdd(tmpstr);
				free(tmpstr); tmpstr = NULL;
				delownerrc(screen);
				goto start;
			}
			break;
		}
	}

	delownerrc(screen);
	delmarkedscreennodes(screen, 1);
	clearscreen(screen);
}

void hddformat(char* dev, char* filesystem)
{
	int format = 0;
	int large = 0;
	int rc = 0;
	char* cmd = NULL;
	struct hdd* node = NULL;
	resettvpic();

#ifdef OEBUILD
	status.hangtime = 99999;
#endif

	node = gethdd(dev);
	if(node == NULL) return;
	
	if(node->size > 4294967295UL)
	{	
		if(!file_exist("/var/bin/parted") && !file_exist("/var/swap/bin/parted") && !file_exist("/mnt/swapextensions/bin/parted") && !file_exist("/usr/sbin/parted"))
		{
			textbox(_("Message"), _("HDD > 2TB unsupported.\nYou must first install plugin parted, then you can use this panel."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
			return;
		}
		large = 1;
	}

	if(node->partition == 0)
	{
		if(textbox(_("Message"), _("Are you sure you want to remove all Partitions\non this device and create a new Partition 1?\nBox reboots after format"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
		{
			cmd = ostrcat("/sbin/parter.sh /dev/" , dev, 0, 0);
			cmd = ostrcat(cmd , " create 1 ", 1, 0);
			cmd = ostrcat(cmd , filesystem, 1, 0);
			if(large == 1)
				cmd = ostrcat(cmd , " large", 1, 0);
#ifdef OEBUILD
			if(!file_exist("/mnt/logs"))
				 mkdir("/mnt/logs", 777);
		
			if(file_exist("/mnt/logs"))
				cmd = ostrcat(cmd, " > /mnt/logs/format_debug.log 2>&1", 1, 0);
#else
			if(file_exist("/var/swap"))
			{
				if(!file_exist("/var/swap/logs"))
					 mkdir("/var/swap/logs", 777);
			
				if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
					cmd = ostrcat(cmd, " > /var/swap/logs/format_debug.log 2>&1", 1, 0);		
			}
			else if(!file_exist("/etc/.smalbox"))
			{
				if(!file_exist("/mnt/logs"))
					 mkdir("/mnt/logs", 777);
			
				if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
					cmd = ostrcat(cmd, " > /mnt/logs/format_debug.log 2>&1", 1, 0);
			}
#endif
			debug(80, "fdisk create cmd: %s", cmd);
printf("1status.hangtime: %d\n", status.hangtime);
			system(cmd);
printf("2status.hangtime: %d\n", status.hangtime);

			format = 2;
			free(cmd); cmd = NULL;
		}
		
	}
	else
	{
		if(textbox(_("Message"), _("Are you sure you want to format this Partition?\nBox reboots after format"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
		{
			printf("dev: %s\n", dev);

			char* part = oregex(".*([0-9]{1,2}).*", dev);
			printf("part: %s\n", part);

			char* devname = oregex(".*([a-z]{3}).*", dev);
			printf("devname: %s\n", devname);

			cmd = ostrcat("/sbin/parter.sh /dev/" , devname, 0, 0);
			cmd = ostrcat(cmd , " update ", 1, 0);
			cmd = ostrcat(cmd , part, 1, 0);
			cmd = ostrcat(cmd , " ", 1, 0);
			cmd = ostrcat(cmd , filesystem, 1, 0);
			free(part), part = NULL;
			free(devname), devname = NULL;

			if(large == 1)
				cmd = ostrcat(cmd , " large", 1, 0);
#ifdef OEBUILD
			if(!file_exist("/mnt/logs"))
				 mkdir("/mnt/logs", 777);
		
			if(file_exist("/mnt/logs"))
				cmd = ostrcat(cmd, " > /mnt/logs/format_debug.log 2>&1", 1, 0);
#else
			if(file_exist("/var/swap"))
			{
				if(!file_exist("/var/swap/logs"))
					 mkdir("/var/swap/logs", 777);
			
				if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
					cmd = ostrcat(cmd, " > /var/swap/logs/format_debug.log 2>&1", 1, 0);		
			}
			else if(!file_exist("/etc/.smalbox"))
			{
				if(!file_exist("/mnt/logs"))
					 mkdir("/mnt/logs", 777);
			
				if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
					cmd = ostrcat(cmd, " > /mnt/logs/format_debug.log 2>&1", 1, 0);
			}
#endif
printf("3status.hangtime: %d\n", status.hangtime);

			debug(80, "fdisk update cmd: %s", cmd);
			system(cmd);
printf("4status.hangtime: %d\n", status.hangtime);

			format = 1;
			free(cmd); cmd = NULL;
		}
	}

	if(format > 0)
	{
		int record = 0, ext = 0, swap = 0, backup = 0;
		
		if(textbox(_("Message"), _("use medium for record"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
			record = 1;
		
		if(status.expertmodus > 9)
		{
			if(textbox(_("Message"), _("use medium for extension"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
				ext = 1;
			if(textbox(_("Message"), _("use medium for swap"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
				swap = 1;
			if(textbox(_("Message"), _("use medium for backup"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
				backup = 1;
		}
			
#ifndef OEBUILD	
		if(ostrcmp(filesystem, "vfat") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.fat.gui -F 32\" /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "jfs") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.jfs.gui -q\" /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "ext2") == 0)
			cmd = ostrcat("/sbin/cmd.sh mkfs.ext2.gui /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "ext3") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.ext3.gui -T largefile -m0 -O dir_index\" /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "ext4") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.ext4.gui -T largefile -m0 -O dir_index\" /dev/" , dev, 0, 0);
#else
		if(ostrcmp(filesystem, "vfat") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.fat -F 32\" /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "jfs") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.jfs -q\" /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "ext2") == 0)
			cmd = ostrcat("/sbin/cmd.sh mkfs.ext2 /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "ext3") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.ext3 -T largefile -N 262144 -F -F -m0 -O ^metadata_csum -O dir_index,sparse_super\" /dev/" , dev, 0, 0);			
//			cmd = ostrcat("/sbin/cmd.sh \"mkfs.ext3 -T largefile -m0 -O dir_index\" /dev/" , dev, 0, 0);
		else if(ostrcmp(filesystem, "ext4") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"mkfs.ext4 -T largefile -N 262144 -F -F -m0 -O ^metadata_csum -O dir_index,sparse_super\" /dev/" , dev, 0, 0);
//			cmd = ostrcat("/sbin/cmd.sh \"mkfs.ext4 -T largefile -m0 -O dir_index\" /dev/" , dev, 0, 0);
#endif

		if(format == 2) cmd = ostrcat(cmd , "1", 1, 0);
			
		if(record == 1) 
			cmd = ostrcat(cmd , " 1", 1, 0);
		else
			cmd = ostrcat(cmd , " 0", 1, 0);
			
		if(ext == 1) 
			cmd = ostrcat(cmd , " 1", 1, 0);
		else
			cmd = ostrcat(cmd , " 0", 1, 0);
			
		if(swap == 1) 
			cmd = ostrcat(cmd , " 1", 1, 0);
		else
			cmd = ostrcat(cmd , " 0", 1, 0);
			
		if(backup == 1) 
			cmd = ostrcat(cmd , " 1", 1, 0);
		else
			cmd = ostrcat(cmd , " 0", 1, 0);
#ifdef OEBUILD
		if(!file_exist("/mnt/logs"))
			 mkdir("/mnt/logs", 777);
	
		if(file_exist("/mnt/logs"))
			cmd = ostrcat(cmd, " >> /mnt/logs/format_debug.log 2>&1", 1, 0);		
#else
		if(file_exist("/var/swap"))
		{
			if(!file_exist("/var/swap/logs"))
				 mkdir("/var/swap/logs", 777);
		
			if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
				cmd = ostrcat(cmd, " >> /var/swap/logs/format_debug.log 2>&1", 1, 0);		
		}
		else if(!file_exist("/etc/.smalbox"))
		{
			if(!file_exist("/mnt/logs"))
				 mkdir("/mnt/logs", 777);
		
			if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
				cmd = ostrcat(cmd, " >> /mnt/logs/format_debug.log 2>&1", 1, 0);
		}
#endif
		debug(80, "format cmd: %s", cmd);

#ifdef OEBUILD
		printf("5status.hangtime: %d\n", status.hangtime);
		status.hangtime = 99999;
#endif
		rc = system(cmd);
		printf("6status.hangtime: %d\n", status.hangtime);

		free(cmd); cmd = NULL;
		if(rc != 0)
		{
			textbox(_("Message"), _("ERROR\npartition could not be created"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
			return;
		}
	}
		printf("7status.hangtime: %d\n", status.hangtime);
}

int hddfsck(char* dev)
{
	char* cmd = NULL;
	struct hdd* node = NULL;

#ifdef OEBUILD
		printf("1status.hangtime: %d\n", status.hangtime);
		status.hangtime = 99999;
#endif

	node = gethdd(dev);
	if(node == NULL) return 1;
	if(node->filesystem == NULL) return 1;
	debug(80, "device=%s filesystem=%s", dev, node->filesystem);
	resettvpic();

	if(node->size > 524288000UL)
	{	
		textbox(_("Message"), _("Information: This hard drive size can take a file\nsystem check between 30 minutes and 1.5 hours."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
	}

	if(textbox(_("Message"), _("Are you sure you want to check this Partition?\nBox reboots after check"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
	{
		if(ostrcmp(node->filesystem, "vfat") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"fsck.fat.gui -a -v\" /dev/" , dev, 0, 0);
		else if(ostrcmp(node->filesystem, "jfs") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"fsck.jfs.gui -f -p\" /dev/" , dev, 0, 0);
		else if(ostrcmp(node->filesystem, "ext2") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"fsck.ext2.gui -f -p\" /dev/" , dev, 0, 0);
		else if(ostrcmp(node->filesystem, "ext3") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"fsck.ext3.gui -f -p\" /dev/" , dev, 0, 0);
		else if(ostrcmp(node->filesystem, "ext4") == 0)
			cmd = ostrcat("/sbin/cmd.sh \"fsck.ext4.gui -f -p\" /dev/" , dev, 0, 0);
#ifdef OEBUILD
		if(!file_exist("/mnt/logs"))
			 mkdir("/mnt/logs", 777);
	
		if(file_exist("/mnt/logs"))
			cmd = ostrcat(cmd, " > /mnt/logs/fsck_debug.log 2>&1", 1, 0);
#else
		if(file_exist("/var/swap"))
		{
			if(!file_exist("/var/swap/logs"))
				 mkdir("/var/swap/logs", 777);
		
			if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
				cmd = ostrcat(cmd, " > /var/swap/logs/fsck_debug.log 2>&1", 1, 0);		
		}
		else if(!file_exist("/etc/.smalbox"))
		{
			if(!file_exist("/mnt/logs"))
				 mkdir("/mnt/logs", 777);
		
			if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
				cmd = ostrcat(cmd, " > /mnt/logs/fsck_debug.log 2>&1", 1, 0);
		}
#endif
		printf("2status.hangtime: %d\n", status.hangtime);

		debug(80, "fsck cmd: %s", cmd);
		system(cmd);
		printf("3status.hangtime: %d\n", status.hangtime);

		free(cmd); cmd = NULL;
	}

	return 0;
}

void screenharddisksleep()
{
	char* sleepWert = NULL;
	int rcret = 0;
	struct skin* sleep_config = getscreen("harddisk_main");
	struct skin* details = getscreennode(sleep_config, "details");
	struct skin* listbox = getscreennode(sleep_config, "listbox");
	struct skin* node = NULL;
	struct skin* tmp = NULL;
	
	/*int m_with = sleep_config->width;
	int m_height = sleep_config->height;
	int m_prozwidth = sleep_config->prozwidth;
	int m_prozheight= sleep_config->prozheight;
	
	sleep_config->prozheight = YES;
	sleep_config->prozwidth = YES;
	sleep_config->width = 40;
	sleep_config->height = 30;*/

	delmarkedscreennodes(sleep_config, 1);
	listbox->aktpage = -1;
	listbox->aktline = 1;
	changetitle(sleep_config, _("Harddisk Sleep"));
	if(details != status.skinerr) changetext(details, _("set here the default sleeptime for all your media drives"));
	
	node = addlistbox(sleep_config, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("Time to sleep")); changename(node, "timetosleep");
		addchoicebox(node, "0", _("off"));
		addchoicebox(node, "300", _("5 min")); addchoicebox(node, "600", _("10 min"));
		addchoicebox(node, "900", _("15 min")); addchoicebox(node, "3600", _("60 min"));
	
		sleepWert = getconfig("timetosleep", NULL);
		if(sleepWert == NULL)
			setchoiceboxselection(node, "0");
		else
			setchoiceboxselection(node, sleepWert);
	}
		
	drawscreen(sleep_config, 0, 0);
	tmp = listbox->select;
	
	while(1)
	{
		addscreenrc(sleep_config, tmp);
		rcret = waitrc(sleep_config, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(listbox->select != NULL && listbox->select->ret != NULL && rcret == getrcconfigint("rcok", NULL))
		{
			addconfig("timetosleep", listbox->select->ret);
			//settimetosleep(atoi(listbox->select->ret));
			break;
		}
	}

	/*sleep_config->width = m_with;
	sleep_config->height = m_height;
	sleep_config->prozheight = m_prozheight;
	sleep_config->prozwidth = m_prozwidth;*/
	delmarkedscreennodes(sleep_config, 1);
	delownerrc(sleep_config);
	clearscreen(sleep_config);
}

//flag 0: lock
//flag 1: no lock
void delhdd(char* device, int flag)
{
	if(flag == 0) m_lock(&status.hddmutex, 13);
	struct hdd *node = hdd, *prev = hdd;
	
	while(node != NULL)
	{
		if(ostrcmp(node->device, device) == 0)
		{
			if(node == hdd)
			{
				hdd = node->next;
				if(hdd != NULL)
					hdd->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->device); node->device = NULL;
			free(node->vendor); node->vendor = NULL;
			free(node->model); node->model = NULL;
			free(node->label); node->label = NULL;
			free(node->uuid); node->uuid = NULL;
			free(node->filesystem); node->filesystem = NULL;
	
			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	if(flag == 0) m_unlock(&status.hddmutex, 13);
}

//flag 0: lock
//flag 1: no lock
struct hdd* addhdd(char* device, int partition, unsigned long long size, int removable, char* vendor, char *model, char* label, char* filesystem, char* uuid, int timetosleep, struct hdd* last, int flag)
{
	if(flag == 0) m_lock(&status.hddmutex, 13);
	struct hdd *newnode = NULL, *prev = NULL, *node = hdd;

	newnode = (struct hdd*)malloc(sizeof(struct hdd));	
	if(newnode == NULL)
	{
		err("no memory");
		if(flag == 0) m_unlock(&status.hddmutex, 13);
		return NULL;
	}
	memset(newnode, 0, sizeof(struct hdd));

	newnode->device = device;
	newnode->partition = partition;
	newnode->size = size;
	newnode->removable = removable;
	newnode->vendor = vendor;
	newnode->model = model;
	newnode->label = label;
	newnode->uuid = uuid;
	newnode->filesystem = filesystem;
	newnode->read = 0;
	newnode->write = 0;
	newnode->sleeptime = timetosleep;
	newnode->notchanged = 0;

	debug(80, "add hdd %s", device);
	
	if(last == NULL)
	{
		while(node != NULL && strcoll(newnode->device, node->device) > 0)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
	{
		hdd = newnode;
	}
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	
	//show message for new, not configured devices
	//check only partition 1
	if(status.standby == 0 && newnode->partition == 1 && ostrstr(newnode->device, "1") != NULL)
	{
		char* tmpstr = NULL, *backup = NULL, *movie = NULL, *swapextensions = NULL, *swapfile = NULL, *checkfile = NULL;
		int newdev = 1, ret = 0;
		
		tmpstr = ostrcat("/autofs/", newnode->device, 0, 0);
		checkfile = ostrcat(tmpstr, "/.titandev", 0, 0);
		if(file_exist(checkfile) == 0)
		{
			backup = ostrcat(tmpstr, "/backup", 0, 0);
			movie = ostrcat(tmpstr, "/movie", 0, 0);
			swapextensions = ostrcat(tmpstr, "/swapextensions", 0, 0);
			swapfile = ostrcat(tmpstr, "/swapfile", 0, 0);
			
			if(file_exist(movie) == 1 || file_exist(swapextensions)== 1 || file_exist(backup) == 1 || file_exist(swapfile) == 1)
				newdev = 0;
				
			ret = writesys(checkfile, "titan", 1);
			if(ret == 0)
			{
				sync();
				if(newdev == 1)
					textbox("Message", _("Found new Stick/HDD.\nYou can configure it in Harddisk Menu."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 15, 0);
			}
		}
		free(tmpstr); tmpstr = NULL;
		free(backup); backup = NULL;
		free(movie); movie = NULL;
		free(swapextensions); swapextensions = NULL;
		free(swapfile); swapfile = NULL;				
		free(checkfile); checkfile = NULL;				
	}
	
	if(flag == 0) m_unlock(&status.hddmutex, 13);
	return newnode;
}

//flag = 0: mutex lock
//flag = 1: no mutex lock
void freehdd(int flag)
{
	if(flag == 0) m_lock(&status.hddmutex, 13);
	struct hdd *node = hdd, *prev = hdd;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delhdd(prev->device, 1);
	}
	if(flag == 0) m_unlock(&status.hddmutex, 13);
}

int addhddall()
{
	m_lock(&status.hddmutex, 13);
	FILE* fd = NULL;
	FILE* fd2 = NULL;
	char* fileline = NULL, *dev = NULL, *pos = NULL, *part = NULL;
	char* tmpstr = NULL;
	char* tmpstr2 = NULL;
	int partition = 0;
	int timetosleep = 0;
	struct hdd *node = hdd;
	struct hdd *nodedev = hdd;
	int w1, w2, w3, w4, w5;
		
	fd = fopen("/proc/partitions", "r");
	if(fd == NULL)
	{
		err("open /proc/partitions");
		m_unlock(&status.hddmutex, 13);
		return 1;
	}

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		m_unlock(&status.hddmutex, 13);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		pos = ostrstr(fileline, "sd");
		if(pos != NULL)
		{
			partition = 0;
			part = NULL;
			pos = string_newline(pos);
			if(strlen(pos) == 3)
			{
				free(dev); dev = NULL;
				dev = ostrcat(pos, NULL, 0, 0);
				if(getconfig("timetosleep", NULL) == NULL)
					timetosleep = 0;
				else 
					timetosleep = atoi(getconfig("timetosleep", NULL));
			}
			if(strlen(pos) == 4)
			{
				part = pos;
				partition = atoi(&pos[3]);
			}
			tmpstr = ostrcat(tmpstr, "#", 1, 0);
			tmpstr = ostrcat(tmpstr, pos, 1, 0);
			//if(gethdd(pos) == NULL) 
			nodedev = gethdd(pos);
			if(nodedev == NULL) 
				nodedev = addhdd(ostrcat(pos, NULL, 0, 0), partition, hddgetsize(dev, part), hddgetremovable(dev), hddgetvendor(dev), hddgetmodel(dev), get_label(part), get_filesystem(part), get_uuid(part), timetosleep, NULL, 1);
			
			//HDD SleepTimer start
			if(nodedev != NULL) {
				if(nodedev->sleeptime != timetosleep) {
					nodedev->read = 0;
					nodedev->write = 0;
					nodedev->sleeptime = timetosleep;
					nodedev->notchanged = 0;
				}
				if(strlen(pos) == 3 && timetosleep > 0) {
					free(tmpstr2);tmpstr2=NULL;
					tmpstr2 = ostrcat(tmpstr2, "/sys/block/", 1, 0);
					tmpstr2 = ostrcat(tmpstr2, nodedev->device, 1, 0);
					tmpstr2 = ostrcat(tmpstr2, "/stat", 1, 0);
					fd2 = fopen(tmpstr2, "r");
					if(fd2 != NULL) {
						fscanf(fd2, "%d%d%d%d%d", &w1,&w2,&w3,&w4,&w5);
						fclose(fd2);
						fd2 = NULL;
						if((nodedev->read != w1) || (nodedev->write != w5)) {
							nodedev->read = w1;
							nodedev->write = w5;
							nodedev->notchanged = 0;
						} else {
							if (nodedev->notchanged < nodedev->sleeptime) {
								nodedev->notchanged = nodedev->notchanged + (status.addhddall->delay / 1000);
								if (nodedev->notchanged >= nodedev->sleeptime) {
									free(tmpstr2);tmpstr2=NULL;
#ifdef MIPSEL
									tmpstr2 = ostrcat(tmpstr2, "sdparm -C stop /dev/", 1, 0);
#else
									tmpstr2 = ostrcat(tmpstr2, "hd-idle -t ", 1, 0);
#endif
									tmpstr2 = ostrcat(tmpstr2, nodedev->device, 1, 0);
									printf("cmd: %s\n", tmpstr2);
									system(tmpstr2);
								}
							}
						}
					}
				}
			}
			//HDD SleepTimer end
		
		}
	}

	//check for removed devs
	while(node != NULL && tmpstr != NULL)
	{
		if(ostrstr(tmpstr, node->device) == NULL)
		{
			debug(80, "remove %s", node->device);
			delhdd(node->device, 1);
		}
		node = node->next;
	}

	if(node != NULL && tmpstr == NULL)
	{
		debug(80, "remove all");
		freehdd(1);
	}

	free(tmpstr);
	free(tmpstr2);
	free(dev);
	free(fileline);
	fclose(fd);
	m_unlock(&status.hddmutex, 13);
	return 0;
}

#endif
