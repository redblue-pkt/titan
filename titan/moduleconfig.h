#ifndef MODULECONFIG_H
#define MODULECONFIG_H

void screenmoduleconfig()
{
	int rcret = 0, i = 0;
	struct skin* moduleconfig = getscreen("moduleconfig");
	struct skin* listbox = getscreennode(moduleconfig, "listbox");
	struct skin* tmp = NULL;
	struct dvbdev* dvbnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

start:
	i = 0;
	dvbnode = dvbdev;
	tmp = NULL;
	delmarkedscreennodes(moduleconfig, 1);

	while(dvbnode != NULL)
	{
		if(dvbnode->type == CIDEV)
		{
			i++;
			tmp = addlistbox(moduleconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmpstr = ostrcat(tmpstr, _("Slot "), 1, 0);
				tmpnr = oitoa(i);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
				tmpstr = ostrcat(tmpstr, ": ", 1, 0);
				if(dvbnode->caslot != NULL)
				{
					if(dvbnode->caslot->name == NULL)
						tmpstr = ostrcat(tmpstr, _("unknown"), 1, 0);
					else
						tmpstr = ostrcat(tmpstr, dvbnode->caslot->name, 1, 0);
				}
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				tmpstr = ostrcat(tmpstr, _("Status: "), 1, 0);
				if(dvbnode->caslot != NULL)
				{
					if(dvbnode->caslot->status == 0)
						tmpstr = ostrcat(tmpstr, _("empty"), 1, 0);
					if(dvbnode->caslot->status == 1)
						tmpstr = ostrcat(tmpstr, _("initializing"), 1, 0);
					if(dvbnode->caslot->status == 2)
						tmpstr = ostrcat(tmpstr, _("ready"), 1, 0);
				}
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				tmp->height = (listbox->fontsize * 2) + (2 * 2);
				tmp->type = TEXTBOX;
				tmp->handle = (void*)dvbnode;
			}
			tmp = addlistbox(moduleconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmpstr = ostrcat(tmpstr, "camtype_", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(i - 1), 1, 1);
				tmp->type = CHOICEBOX;
				changetext(tmp, "Module Type");
				addchoicebox(tmp, "0", _("Single Service"));
				addchoicebox(tmp, "1", _("Multiple Service"));
				setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
				changename(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			tmp = addlistbox(moduleconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->height = 20;
				tmp->deaktivcol = 1;
			}
		}
		dvbnode = dvbnode->next;
	}

	drawscreen(moduleconfig, 0, 0);
	addscreenrc(moduleconfig, listbox);

	tmp = listbox->select;
	while(1)
	{
		if(listbox->select != NULL && (listbox->select->type & CHOICEBOX))
			addscreenrc(moduleconfig, tmp);
		rcret = waitrc(moduleconfig, 2000, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			tmp = listbox;
			while(tmp != NULL)
			{
				if(ostrncmp("camtype_", tmp->name, 8) == 0)
					addconfigscreencheck(tmp->name, tmp, "0");
				tmp = tmp->next;
			}
			break;
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcred", NULL))
		{
			if(((struct dvbdev*)listbox->select->handle)->caslot != NULL)
				((struct dvbdev*)listbox->select->handle)->caslot->status = 0;
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
#ifdef CAMSUPP
			if((struct dvbdev*)listbox->select->handle != NULL)
				caappmenu((struct dvbdev*)listbox->select->handle);
#endif
		}
		if(rcret == RCTIMEOUT) goto start;
	}

	delmarkedscreennodes(moduleconfig, 1);
	delownerrc(moduleconfig);
	clearscreen(moduleconfig);
}

#endif
