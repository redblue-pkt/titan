#ifndef DLNA_H
#define DLNA_H

extern struct inetwork* inetwork;

struct dlna
{
	char* interface;
	char* port;
	char* name;
	char* dir1;
	char* dir2;
	char* dir3;
	char* dir4;
};

void dlnasetdefault(struct dlna* node)
{
	if(node == NULL) return;
  
	node->interface = ostrcat(node->interface, "eth0", 1, 0);
	node->port = ostrcat(node->port, "8200", 1, 0);
	node->name = ostrcat(node->name, "DLNASERVER", 1, 0);
}

void freedlnacontent(struct dlna* node)
{
	if(node == NULL) return;
  
	free(node->interface); node->interface = NULL;
	free(node->port); node->port = NULL;
	free(node->name); node->name = NULL;
	free(node->dir1); node->dir1 = NULL;
	free(node->dir2); node->dir2 = NULL;
	free(node->dir3); node->dir3 = NULL;
	free(node->dir4); node->dir4 = NULL;
}

int readdlna(const char* filename, struct dlna* node)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *tmpstr = NULL;
	int len = 0;
  
	if(node == NULL) return 1;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		tmpstr = ostrstrcase(fileline, "network_interface=");
		if(tmpstr != NULL)
		{
			tmpstr += 18;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->interface);
				node->interface = ostrcat(tmpstr, NULL, 0, 0);
			}				
		}
		tmpstr = ostrstrcase(fileline, "port=");
		if(tmpstr != NULL)
		{
			tmpstr += 5;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->port);
				node->port = ostrcat(tmpstr, NULL, 0, 0);
			}				
		}
		tmpstr = ostrstrcase(fileline, "friendly_name=");
		if(tmpstr != NULL)
		{
			tmpstr += 14;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->name);
				node->name = ostrcat(tmpstr, NULL, 0, 0);
			}				
		}
		tmpstr = ostrstrcase(fileline, "media_dir=");
		if(tmpstr != NULL && node->dir1 == NULL)
		{
			tmpstr += 10;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->dir1);
				node->dir1 = ostrcat(tmpstr, NULL, 0, 0);
			}
			continue;
		}
		tmpstr = ostrstrcase(fileline, "media_dir=");
		if(tmpstr != NULL && node->dir2 == NULL)
		{
			tmpstr += 10;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->dir2);
				node->dir2 = ostrcat(tmpstr, NULL, 0, 0);
			}
			continue;
		}
		tmpstr = ostrstrcase(fileline, "media_dir=");
		if(tmpstr != NULL && node->dir3 == NULL)
		{
			tmpstr += 10;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->dir3);
				node->dir3 = ostrcat(tmpstr, NULL, 0, 0);
			}
			continue;
		}
		tmpstr = ostrstrcase(fileline, "media_dir=");
		if(tmpstr != NULL && node->dir4 == NULL)
		{
			tmpstr += 10;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(node->dir4);
				node->dir4 = ostrcat(tmpstr, NULL, 0, 0);
			}
			continue;
		}
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int writedlna(const char* filename, struct dlna* node)
{ 
	char* savesettings = NULL;
  
	if(node == NULL) return 1;
	
	if(node->interface != NULL)
	{
		savesettings = ostrcat(savesettings, "network_interface=", 1, 0);
		savesettings = ostrcat(savesettings, node->interface, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}

	if(node->port != NULL)
	{
		savesettings = ostrcat(savesettings, "port=", 1, 0);
		savesettings = ostrcat(savesettings, oitoa(atoi(node->port)), 1, 1);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}

	if(node->name != NULL)
	{
		savesettings = ostrcat(savesettings, "friendly_name=", 1, 0);
		savesettings = ostrcat(savesettings, node->name, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}
  
	if(node->dir1 != NULL)
	{
		savesettings = ostrcat(savesettings, "media_dir=", 1, 0);
		savesettings = ostrcat(savesettings, node->dir1, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}
  
	if(node->dir2 != NULL)
	{
		savesettings = ostrcat(savesettings, "media_dir=", 1, 0);
		savesettings = ostrcat(savesettings, node->dir2, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}
  
	if(node->dir3 != NULL)
	{
		savesettings = ostrcat(savesettings, "media_dir=", 1, 0);
		savesettings = ostrcat(savesettings, node->dir3, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}
  
	if(node->dir4 != NULL)
	{
		savesettings = ostrcat(savesettings, "media_dir=", 1, 0);
		savesettings = ostrcat(savesettings, node->dir4, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);
	}
	
	FILE* fd = fopen(filename, "w");
	if(fd)
	{
		fprintf(fd, "%s\n", savesettings);
		fclose(fd);
	}
	free(savesettings); savesettings = NULL;

	return 0;
}

void screendlna()
{
	int rcret = -1, ret = 0;
	struct skin* dlna = getscreen("dlnasettings");
	struct skin* listbox = getscreennode(dlna, "listbox");
	struct skin* startmode = getscreennode(dlna, "startmode");
	struct skin* interface = getscreennode(dlna, "interface");
	struct skin* port = getscreennode(dlna, "port");
	struct skin* name = getscreennode(dlna, "name");
	struct skin* dir1 = getscreennode(dlna, "dir1");
	struct skin* dir2 = getscreennode(dlna, "dir2");
	struct skin* dir3 = getscreennode(dlna, "dir3");
	struct skin* dir4 = getscreennode(dlna, "dir4");
	struct inetwork* net = inetwork;
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *dlnastop = NULL, *dlnastart = NULL, *dlnaconfig = NULL;

	struct dlna* node = (struct dlna*)malloc(sizeof(struct dlna));
	if(node == NULL)
	{
		err("no mem");
		return;
	}
	memset(node, 0, sizeof(struct dlna));

	dlnasetdefault(node);
	
	if(file_exist("/var/etc/minidlna.conf") == 1)
	{
		dlnastop = "/var/usr/local/share/titan/plugins/dlna/dlna.sh stop /var/etc/minidlna.conf";
		dlnastart = "/var/usr/local/share/titan/plugins/dlna/dlna.sh start /var/etc/minidlna.conf";
		dlnaconfig = "/var/swap/etc/minidlna.conf";	
	}
	else if(file_exist("/var/swap/etc/minidlna.conf") == 1)
	{
		dlnastop = "/var/swap/usr/local/share/titan/plugins/dlna/dlna.sh stop /var/swap/etc/minidlna.conf";
		dlnastart = "/var/swap/usr/local/share/titan/plugins/dlna/dlna.sh start /var/swap/etc/minidlna.conf";
		dlnaconfig = "/var/swap/etc/minidlna.conf";	
	}
	else
	{
		dlnastop = "/mnt/swapextensions/usr/local/share/titan/plugins/dlna/dlna.sh stop /mnt/swapextensions/etc/minidlna.conf";
		dlnastart = "/mnt/swapextensions/usr/local/share/titan/plugins/dlna/dlna.sh start /mnt/swapextensions/etc/minidlna.conf";
		dlnaconfig = "/mnt/swapextensions/etc/minidlna.conf";	
	}
	
	readdlna(dlnaconfig, node);
	
	addchoicebox(startmode, "n", _("no"));
	addchoicebox(startmode, "y", _("yes"));
	setchoiceboxselection(startmode, getownconfig("dlna"));

	while(net != NULL)
	{
		if(net->flag == 1)
			addchoicebox(interface, net->device, net->device);
		net = net->next;
	}
	setchoiceboxselection(interface, node->interface);
  
	changemask(port, "00000");
	changeinput(port, node->port);
	port->input = mask(port->input, 5, "0");

	changeinput(name, node->name);

	changeinput(dir1, node->dir1);
	changeinput(dir2, node->dir2);
	changeinput(dir3, node->dir3);
	changeinput(dir4, node->dir4);

	freedlnacontent(node);
	drawscreen(dlna, 0, 0);
	addscreenrc(dlna, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(dlna, tmp);
		rcret = waitrc(dlna, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcgreen", NULL))
		{
			node->interface = ostrcat(interface->ret, NULL, 0, 0);
			node->port = ostrcat(port->ret, NULL, 0, 0);
			node->name = ostrcat(name->ret, NULL, 0, 0);
			node->dir1 = ostrcat(dir1->ret, NULL, 0, 0);
			node->dir2 = ostrcat(dir2->ret, NULL, 0, 0);
			node->dir3 = ostrcat(dir3->ret, NULL, 0, 0);
			node->dir4 = ostrcat(dir4->ret, NULL, 0, 0);
			writedlna(dlnaconfig, node);
			freedlnacontent(node);
			if(startmode->ret != NULL) addownconfig("dlna", startmode->ret);
			
			if(rcret == getrcconfigint("rcok", NULL)) break;
			
			if(rcret == getrcconfigint("rcgreen", NULL))
			{
				debug(10, "cmd: %s", dlnastop);
				system(dlnastop);
				debug(10, "cmd: %s", dlnastart);
				ret = system(dlnastart);
				if(ret == 0)
					textbox(_("Message"), _("DLNA started."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				else
					textbox(_("Message"), _("DLNA not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				drawscreen(dlna, 0, 0);
			}
		}

		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			debug(10, "cmd: %s", dlnastop);
			system(dlnastop);
			textbox(_("Message"), _("DLNA now stopped"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
			drawscreen(dlna, 0, 0);
		}

		if(rcret == getrcconfigint("rcblue", NULL))
		{
			if(listbox->select != NULL && (ostrcmp(listbox->select->name, "dir1") == 0 || ostrcmp(listbox->select->name, "dir2") == 0 || ostrcmp(listbox->select->name, "dir3") == 0 || ostrcmp(listbox->select->name, "dir4") == 0))
			{
				clearscreen(dlna);
				tmpstr = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 700, 0, 650, 0, 0);
				if(tmpstr != NULL)
					changeinput(listbox->select, tmpstr);
				free(tmpstr); tmpstr = NULL;

				drawscreen(dlna, 0, 0);
			}
		}

	}

	freedlnacontent(node);
	free(node);
	delownerrc(dlna);
	clearscreen(dlna);
}

#endif
