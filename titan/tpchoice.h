#ifndef TPCHOICE_H
#define TPCHOICE_H

//flag 0: manual scan (DVB-S)
//flag 1: auto scan (all)
//flag 2: manual scan (DVB-C)
//flag 3: manual scan (DVB-T)
struct transponder* tpchoicescreen(int orbitalpos, int flag)
{
	int rcret = 0;
	struct skin* menulist = getscreen("menulist");
	struct skin* listbox = getscreennode(menulist, "listbox");
	struct skin* tmp = 0;
	struct transponder* node = transponder;
	char* tmpstr = NULL;

	if(flag == 1) return;

	listbox->aktpage = -1;
	listbox->aktline = 1;
	changetitle(menulist, _("Transponderlist"));

	while(node != NULL)
	{
		if(orbitalpos != node->orbitalpos) 
		{
			node = node->next;
			continue;
		}
		if(flag == 0 && node->fetype != FE_QPSK)
		{
			node = node->next;
			continue;
		}
		if(flag == 2 && node->fetype != FE_QAM)
		{
			node = node->next;
			continue;
		}
		if(flag == 3 && node->fetype != FE_OFDM)
		{
			node = node->next;
			continue;
		}

		tmp = addlistbox(menulist, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(tmpstr, _("ID"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, ollutoa(node->id), 1, 1);
			tmpstr = ostrcat(tmpstr, " / ", 1, 0);
			tmpstr = ostrcat(tmpstr, _("FREQU"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, oitoa(node->frequency / 1000), 1, 1);
			tmpstr = ostrcat(tmpstr, " / ", 1, 0);
			if(node->fetype == FE_QPSK)
				tmpstr = ostrcat(tmpstr, _("SAT"), 1, 0);
			else if(node->fetype == FE_QAM)
				tmpstr = ostrcat(tmpstr, _("CABLE"), 1, 0);
			else if(node->fetype == FE_OFDM)
				tmpstr = ostrcat(tmpstr, _("TERR"), 1, 0);
			else
				tmpstr = ostrcat(tmpstr, _("UNKNOWN"), 1, 0);

			tmpstr = ostrcat(tmpstr, " / ", 1, 0);
			tmpstr = ostrcat(tmpstr, _("SAT"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, oitoa(node->orbitalpos), 1, 1);

			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->handle = (void*)node;
		}
		node = node->next;
	}
	node = NULL;

	addscreenrc(menulist, listbox);
	drawscreen(menulist, 0, 0);

	while(1)
	{
		rcret = waitrc(menulist, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
				node = (struct transponder*)listbox->select->handle;
			break;
		}
	}

	delownerrc(menulist);
	delmarkedscreennodes(menulist, 1);
	clearscreen(menulist);

	return node;
}

#endif
