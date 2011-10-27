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
		}
		dvbnode = dvbnode->next;
	}

	drawscreen(moduleconfig, 0);
	addscreenrc(moduleconfig, listbox);

	while(1)
	{
		rcret = waitrc(moduleconfig, 2000, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcred", NULL))
		{
			//TODO: test (close / open / set status to 0)
			careset((struct dvbdev*)listbox->select->handle, ((struct dvbdev*)listbox->select->handle)->devnr);
		}
		if(rcret == RCTIMEOUT) goto start;
	}

	delmarkedscreennodes(moduleconfig, 1);
	delownerrc(moduleconfig);
	clearscreen(moduleconfig);
}

#endif
