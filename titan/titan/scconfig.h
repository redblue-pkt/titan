#ifndef SCCONFIG_H
#define SCCONFIG_H

void screenscconfig()
{
	int rcret = 0, i = 0;
	struct skin* scconfig = getscreen("scconfig");
	struct skin* listbox = getscreennode(scconfig, "listbox");
	struct skin* load = getscreen("loading");
	struct dvbdev* dvbnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;
	struct skin* tmp = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

start:
	i = 0;
	dvbnode = dvbdev;
	tmp = NULL;
	delmarkedscreennodes(scconfig, 1);

	while(dvbnode != NULL)
	{
		if(dvbnode->type == SCDEV)
		{
			i++;
			tmp = addlistbox(scconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmpstr = ostrcat(tmpstr, _("Slot "), 1, 0);
				tmpnr = oitoa(i);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
				tmpstr = ostrcat(tmpstr, ": ", 1, 0);
				
				uint32_t status = 0;
				dvbnode->fd = scopendirect(dvbnode->dev);
				scgetiscardpresent(dvbnode, &status);
				scclose(dvbnode, -1);
				
				if(status == 1)
					tmpstr = ostrcat(tmpstr, _("card present"), 1, 0);
				else
					tmpstr = ostrcat(tmpstr, _("card not present"), 1, 0);
				
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				tmp->height = listbox->fontsize + 2;
				tmp->handle = (void*)dvbnode;
			}
		}
		dvbnode = dvbnode->next;
	}

	drawscreen(scconfig, 0, 0);
	addscreenrc(scconfig, listbox);

	while(1)
	{
		rcret = waitrc(scconfig, 2000, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
		
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcred", NULL))
		{
			clearscreen(scconfig);
			struct dvbdev *dvbtmp = ((struct dvbdev*)listbox->select->handle);
			if(dvbtmp != NULL)
			{
				drawscreen(load, 0, 0);
				dvbtmp->fd = scopendirect(dvbtmp->dev);
				scsetreset(dvbtmp);
				scclose(dvbtmp, -1);
				clearscreen(load);
			}
			drawscreen(scconfig, 0, 0);
		}
		
		if(rcret == RCTIMEOUT) goto start;
	}

	delmarkedscreennodes(scconfig, 1);
	delownerrc(scconfig);
	clearscreen(scconfig);
}

#endif
