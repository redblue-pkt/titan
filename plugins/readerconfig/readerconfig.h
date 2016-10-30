#ifndef READERCONFIG_H
#define READERCONFIG_H

struct oscam
{
	char* enable;
	char* label;
	char* device;
	char* rest;
	struct oscam* next;
};
struct oscam* oscam = NULL;

struct oscam* addoscam(char *enable, char* label, char* device, char* rest, struct oscam* last)
{
	struct oscam *newnode = NULL, *prev = NULL, *node = oscam;

	newnode = (struct oscam*)calloc(1, sizeof(struct oscam));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(last != NULL)
	{
		last->enable = ostrcat(enable, NULL, 0, 0);
		last->label = ostrcat(label, NULL, 0, 0);
		last->device = ostrcat(device, NULL, 0, 0);
	}
	newnode->rest = ostrcat(rest, NULL, 0, 0);

	if(last == NULL)
	{
		while(node != NULL)
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
		oscam = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	return newnode;
}

int readoscam(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	char *enable = NULL, *label = NULL, *device = NULL, *rest = NULL;
	int linecount = 0, len = 0;
	struct oscam* last = NULL;

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
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		linecount++;

		fileline = strstrip(fileline);

		if(ostrstrcase(fileline, "enable") == fileline)
		{
			char* pos = strchr(fileline, '=');
			if(pos != NULL)
			{
				pos++;
				free(enable); enable = NULL;
				enable = ostrcat(strstrip(pos), NULL, 0, 0);
			}
		}
		else if(ostrstrcase(fileline, "Label") == fileline)
		{
			char* pos = strchr(fileline, '=');
			if(pos != NULL)
			{
				pos++;
				free(label); label = NULL;
				label = ostrcat(strstrip(pos), NULL, 0, 0);
			}
		}
		else if(ostrstrcase(fileline, "device") == fileline)
		{
			char* pos = strchr(fileline, '=');
			if(pos != NULL)
			{
				pos++;
				free(device); device = NULL;
				device = ostrcat(strstrip(pos), NULL, 0, 0);
			}
		}
		else
		{
			rest = ostrcat(rest, fileline, 1, 0);
			rest = ostrcat(rest, "\n", 1, 0);
		}

		if(ostrstrcase(fileline, "[reader]") == fileline)
		{
			//add to struct
			last = addoscam(enable, label, device, rest, last);

			free(enable); enable = NULL;
			free(label); label = NULL;
			free(device); device = NULL;
			free(rest); rest = NULL;
		}
	}

	//add to struct
	last = addoscam(enable, label, device, rest, last);

	free(enable); enable = NULL;
	free(label); label = NULL;
	free(device); device = NULL;
	free(rest); rest = NULL;

	free(fileline);
	fclose(fd);
	return 0;
}

int deloscam(struct oscam* onode)
{
	int ret = 1;
	struct oscam *node = oscam, *prev = oscam;

	while(node != NULL)
	{
		if(onode == node)
		{
			ret = 0;
			if(node == oscam)
				oscam = node->next;
			else
				prev->next = node->next;

			free(node->enable);
			node->enable = NULL;

			free(node->label);
			node->label = NULL;

			free(node->device);
			node->device = NULL;

			free(node->rest);
			node->rest = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	return ret;
}

void freeoscam()
{
	struct oscam *node = oscam, *prev = oscam;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deloscam(prev);
	}
}

int writeoscam(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct oscam *node = oscam;
	int ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		if(node->rest != NULL) ret1 = fprintf(fd, "%s", node->rest);
		if(node->enable != NULL) ret2 = fprintf(fd, "enable = %s\n", node->enable);
		if(node->label != NULL) ret3 = fprintf(fd, "Label = %s\n", node->label);
		if(node->device != NULL) ret4 = fprintf(fd, "device = %s\n", node->device);
		if(ret1 < 0 || ret2 < 0 || ret3 < 0 || ret4 < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	return 0;
}

void screenoscamconfig(struct oscam* node, char* file)
{
	int rcret = -1;
	struct skin* oscamconfig = getscreen("readerconfig");
	struct skin* listbox = getscreennode(oscamconfig, "listbox");
	struct skin* enable = getscreennode(oscamconfig, "enable");
	struct skin* device = getscreennode(oscamconfig, "device");
	struct skin* menutitle = getscreennode(oscamconfig, "menutitle");
	struct skin* tmp = NULL;

	if(node == NULL) return;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	addchoicebox(enable, "0", _("no"));
	addchoicebox(enable, "1", _("yes"));
	setchoiceboxselection(enable, node->enable);

	if(checkbox("ATEMIO-NEMESIS") == 1)
	{
		addchoicebox(device, "/dev/sci1", _("first slot"));
		addchoicebox(device, "/dev/sci0", _("second slot"));
	}
	else
	{
		addchoicebox(device, "/dev/sci0", _("first slot"));
		addchoicebox(device, "/dev/sci1", _("second slot"));
	}
	setchoiceboxselection(device, node->device);

	if(node->device == NULL || (ostrcmp(node->device, "/dev/sci0") != 0 && ostrcmp(node->device, "/dev/sci1") != 0))
		device->hidden = YES;
	else
		device->hidden = NO;

	char* tmpstr = NULL;
	tmpstr = ostrcat(_("Reader Configuration"), " : ", 0, 0);
	tmpstr = ostrcat(tmpstr, file, 1, 0);
	changetext(menutitle, tmpstr);
	free(tmpstr), tmpstr = NULL;

	drawscreen(oscamconfig, 0, 0);
	addscreenrc(oscamconfig, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(oscamconfig, tmp);
		rcret = waitrc(oscamconfig, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			free(node->enable);
			node->enable = ostrcat(enable->ret, NULL, 0, 0);

			if(node->device != NULL && (ostrcmp(node->device, "/dev/sci0") == 0 || ostrcmp(node->device, "/dev/sci1") == 0))
			{
				free(node->device);
				node->device = ostrcat(device->ret, NULL, 0, 0);
			}
			break;
		}
	}

	delownerrc(oscamconfig);
	clearscreen(oscamconfig);
}

char* getoscamconfig()
{
	int count = 0;
	char* tmpstr = NULL;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmlist = NULL;

	if(file_exist("/mnt/swapextensions/keys/oscam.server"))
	{
		tmpmlist = addmenulist(&mlist, _("Use config from Flash (permanent)"), NULL, NULL, 0, 0);
		changemenulistparam(tmpmlist, "/mnt/swapextensions", NULL, NULL, NULL);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/mnt/swapextensions", NULL, 0, 0);
		count++;
	}

	if(file_exist("/var/keys/oscam.server"))
	{
		tmpmlist = addmenulist(&mlist, _("Use config from Flash (temporary)"), NULL, NULL, 0, 0);
		changemenulistparam(tmpmlist, "/var", NULL, NULL, NULL);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/var", NULL, 0, 0);
		count++;
	}

	if(file_exist("/var/swap/keys/oscam.server"))
	{
		tmpmlist = addmenulist(&mlist, _("Use config from Stick or HDD"), NULL, NULL, 0, 0);
		changemenulistparam(tmpmlist, "/var/swap", NULL, NULL, NULL);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/var/swap", NULL, 0, 0);
		count++;
	}

	if(count > 1)
	{
		free(tmpstr); tmpstr = NULL;
		mbox = menulistbox(mlist, NULL, _("Reader Configuration"), _("Choose your Config File from the following list"), NULL, NULL, 0, 0);
		if(mbox != NULL)
			tmpstr = ostrcat(mbox->param, NULL, 0, 0);
	}
	else
		textbox(_("Message"), _("No config file found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);

	freemenulist(mlist, 0); mlist = NULL;
	return tmpstr;
}

void screenoscam(char* cfgfile)
{
	int rcret = -1;
	struct skin* skinoscam = getscreen("reader");
	struct skin* listbox = getscreennode(skinoscam, "listbox");
	struct skin* menutitle = getscreennode(skinoscam, "menutitle");
	struct skin* b2 = getscreennode(skinoscam, "b2");

	struct skin* tmp = NULL;
	struct oscam* node = NULL;
	char* tmpstr = NULL, *file = NULL, *cmd = NULL, *dvbapi = NULL;

	if(cfgfile == NULL)
	{
		tmpstr = getoscamconfig();
		if(tmpstr == NULL) return;
		dvbapi = ostrcat(tmpstr, "/keys/oscam.dvbapi", 0, 0);
		file = ostrcat(tmpstr, "/keys/oscam.server", 0, 0);
		free(tmpstr), tmpstr = NULL;
	}
	else
	{
		cmd = ostrcat("/sbin/emu.sh keydir ", cfgfile, 0, 0);
		tmpstr = string_newline(command(cmd));
		dvbapi = ostrcat(tmpstr, "/oscam.dvbapi", 0, 0);
		file = ostrcat(tmpstr, "/oscam.server", 0, 0);
		free(cmd), cmd = NULL;
		free(tmpstr), tmpstr = NULL;
	}

	readoscam(file);
	listbox->aktline = 1;
	listbox->aktpage = -1;

	tmpstr = ostrcat(_("Reader Selection"), " : ", 0, 0);
	tmpstr = ostrcat(tmpstr, file, 1, 0);
	changetext(menutitle, tmpstr);
	free(tmpstr), tmpstr = NULL;

	if(file_exist(dvbapi))
		changetext(b2, _("Dvbapi (enabled)"));
	else
		changetext(b2, _("Dvbapi (disabled)"));

start:

	tmp = NULL;
	node = oscam;
	delmarkedscreennodes(skinoscam, 1);
	delownerrc(skinoscam);

	while(node != NULL)
	{
		if(node->label == NULL || node->enable == NULL)
		{
			node = node->next;
			continue;
		}
		tmp = addlistbox(skinoscam, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(node->label, NULL, 0, 0);
			tmpstr = ostrcat(tmpstr, " -> ", 1, 0);
			if(ostrcmp(node->enable, "0") == 0)
				tmpstr = ostrcat(tmpstr, _("off"), 1, 0);
			else
			{
				tmpstr = ostrcat(tmpstr, _("on"), 1, 0);
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, _("Reader active"), 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
			}

			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->del = 1;
			tmp->handle = (char*)node;
		}
		node = node->next;
	}

	drawscreen(skinoscam, 0, 0);
	addscreenrc(skinoscam, listbox);

	while(1)
	{
		rcret = waitrc(skinoscam, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			//write oscam
			if(writeoscam(file) == 0)
			{
				textbox(_("Message"), _("Oscam config written to medium !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				if(textbox(_("Message"), _("Restart Oscam ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
				{
					startinternreader(0);
					char* cmd = NULL;
					cmd = ostrcat("emu.sh restart" , NULL, 0, 0);
					system(cmd);
					free(cmd);
				}
				drawscreen(skinoscam, 0, 0);
				break;
			}
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				screenoscamconfig((struct oscam*)listbox->select->handle, file);
				goto start;
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			int ret = 1;

			tmpstr = ostrcat(dvbapi, ".disable", 0, 0);
			if(tmpstr != NULL)
			{
				if(file_exist(dvbapi) == 1)
				{
					ret = rename(dvbapi, tmpstr);
					free(tmpstr); tmpstr = NULL;
					tmpstr = ostrcat(tmpstr, _("Oscam dvbapi config disabled !"), 0, 0);
					changetext(b2, _("Dvbapi (disabled)"));
				}
				else if(file_exist(tmpstr) == 1)
				{
					ret = rename(tmpstr, dvbapi);
					free(tmpstr); tmpstr = NULL;
					tmpstr = ostrcat(tmpstr, _("Oscam dvbapi config enabled !"), 0, 0);
					changetext(b2, _("Dvbapi (enabled)"));
				}
			}

			if(ret == 0)
			{
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				if(textbox(_("Message"), _("Restart Oscam ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
				{
					startinternreader(0);
					char* cmd = NULL;
					cmd = ostrcat("emu.sh restart" , NULL, 0, 0);
					system(cmd);
					free(cmd);
				}
			}
			free(tmpstr); tmpstr = NULL;
			drawscreen(skinoscam, 0, 0);
		}
	}

	startinternreader(1);

	free(dvbapi); dvbapi = NULL;
	free(file); file = NULL;
	delmarkedscreennodes(skinoscam, 1);
	freeoscam();
	delownerrc(skinoscam);
	clearscreen(skinoscam);
}

#endif
