#ifndef TUNERCONFIG_H
#define TUNERCONFIG_H

void writetunerconfig(struct skin* tunerreceptiondvbs)
{
	struct skin* child = tunerreceptiondvbs->child;

	while(child != NULL)
	{
		if(child->del == 1)
			addconfigscreentmpcheck(child->name, child, "0");
		child = child->next;
	}
	writeconfigtmp();
}

void createsatlist(struct dvbdev* tuner, struct skin* tunerreceptiondvbs, struct skin* listbox)
{
	int i;
	char* maxsatstring = NULL, *satstring = NULL;
	char* orbitalposstring = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;
	struct skin* tmp = NULL;

	satstring = getsatstring(NULL);
	maxsatstring = getmaxsatstring();
	orbitalposstring = getorbitalposstring(NULL);

	for(i = 1; i <= status.maxsat; i++)
	{
		tmpnr = oitoa(i);

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		tmp->type = CHOICEBOX;
		tmpstr = ostrcat(_("Satellite "), tmpnr, 0, 0);
		changetext(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(_("undefined\n"), satstring, 0, 0);
		changeinput(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("0\n", orbitalposstring, 0, 0);
		changechoiceboxvalue(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tuner->feshortname, "_sat", 0, 0);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
		changename(tmp, tmpstr);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;
		tmp->del = 1;

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		tmp->type = CHOICEBOX;
		changetext(tmp, _("LNB"));
		tmpstr = ostrcat(_("Standard LNB\n"), maxsatstring, 0, 0);
		changeinput(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("0\n", maxsatstring, 0, 0);
		changechoiceboxvalue(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tuner->feshortname, "_lnb", 0, 0);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
		changename(tmp, tmpstr);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;
		tmp->del = 1;

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		tmp->type = CHOICEBOX;
		changetext(tmp, _("DiSEqC"));
		tmpstr = ostrcat(_("no\n"), maxsatstring, 0, 0);
		changeinput(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("0\n", maxsatstring, 0, 0);
		changechoiceboxvalue(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tuner->feshortname, "_diseqc", 0, 0);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
		changename(tmp, tmpstr);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;
		tmp->del = 1;

/*
		tmp = addlistbox(tunerreceptiondvbs, listbox, NULL, 1);
		tmp->type = CHOICEBOX;
		changetext(tmp, _("Rotor Position"));
		tmpstr = ostrcat(_("no\n"), maxsatstring, 0, 0);
		changeinput(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;
*/

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		tmp->deaktivcol = 1;
		tmp->height = 20;
		tmp->del = 2;

		free(tmpnr);
	}

	free(satstring);
	free(maxsatstring);
	free(orbitalposstring);
}

int screentunerreceptiondvbs(struct dvbdev* tuner)
{
	int rcret = 0, ret = 0;
	struct skin* tunerreceptiondvbs = getscreen("tunerreceptiondvbs");
	struct skin* listbox = getscreennode(tunerreceptiondvbs, "listbox");
//	struct skin* longitude = getscreennode(tunerreceivedvbs, "longitude");
//	struct skin* east_west = getscreennode(tunerreceivedvbs, "east_west");
//	struct skin* latitude = getscreennode(tunerreceivedvbs, "latitude");
//	struct skin* north_south = getscreennode(tunerreceivedvbs, "north_south");
	struct skin* tmp = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(tuner == NULL || tuner->feshortname == NULL)
	{
		err("out -> NULL detect");
		return 0;
	}

	createsatlist(tuner, tunerreceptiondvbs, listbox);
/*
	addchoicebox(east_west, "0", _("east"));
	addchoicebox(east_west, "1", _("west"));

	addchoicebox(north_south, "0", _("north"));
	addchoicebox(north_south, "0", _("south"));
*/

	drawscreen(tunerreceptiondvbs, 0);
	addscreenrc(tunerreceptiondvbs, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerreceptiondvbs, tmp);
		rcret = waitrc(tunerreceptiondvbs, 0, 0);
		tmp = listbox->select;

		drawscreen(tunerreceptiondvbs, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->text, "LNB") == 0 && listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") != 0)
			{
				clearscreen(tunerreceptiondvbs);
				screenlnb(listbox->select->ret);
				drawscreen(tunerreceptiondvbs, 0);
			}
			if(listbox->select != NULL && ostrcmp(listbox->select->text, "DiSEqC") == 0 && listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") != 0)
			{
				clearscreen(tunerreceptiondvbs);
				screendiseqc(listbox->select->ret);
				drawscreen(tunerreceptiondvbs, 0);
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfig(tunerreceptiondvbs);
			break;
		}
	}

	delconfigtmpall();
	delmarkedscreennodes(tunerreceptiondvbs, 1);
	delmarkedscreennodes(tunerreceptiondvbs, 2);
	delownerrc(tunerreceptiondvbs);
	clearscreen(tunerreceptiondvbs);
	return ret;
}

//TODO: implement
int screentunerreceptiondvbc(struct dvbdev* tuner)
{
	int rcret = 0, ret = 0;
	struct skin* tunerreceptiondvbc = getscreen("tunerreceptiondvbc");
	struct skin* listbox = getscreennode(tunerreceptiondvbc, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(tuner == NULL || tuner->feshortname == NULL)
	{
		err("out -> NULL detect");
		return 0;
	}

	tmp = addlistbox(tunerreceptiondvbc, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, _("type of channelsearch"));
		changename(tmp, "searchtype");
		addchoicebox(tmp, "0", _("Provider"));
		addchoicebox(tmp, "1", _("Freqiency"));
		tmp->type = CHOICEBOX;

		tmpstr = ostrcat(tuner->feshortname, "_searchtype", 0, 0);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;
	}
	tmp = addlistbox(tunerreceptiondvbc, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, _("search EU VHF I"));
		changename(tmp, "searcheuvhfi");
		addchoicebox(tmp, "1", _("yes"));
		addchoicebox(tmp, "0", _("no"));
		tmp->type = CHOICEBOX;

		tmpstr = ostrcat(tuner->feshortname, "_searcheuvhfi", 0, 0);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;
	}

	drawscreen(tunerreceptiondvbc, 0);
	addscreenrc(tunerreceptiondvbc, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerreceptiondvbc, tmp);
		rcret = waitrc(tunerreceptiondvbc, 0, 0);
		tmp = listbox->select;

		drawscreen(tunerreceptiondvbc, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfig(tunerreceptiondvbc);
			break;
		}
	}

	delconfigtmpall();
	delmarkedscreennodes(tunerreceptiondvbc, 1);
	delmarkedscreennodes(tunerreceptiondvbc, 2);
	delownerrc(tunerreceptiondvbc);
	clearscreen(tunerreceptiondvbc);
	return ret;
}

//create the loop string for the choicebox
void createloopstr(struct dvbdev* node, char** loopstr, char** loopstr1)
{
	struct dvbdev* dvbnode = dvbdev;
	char* tmpnr = NULL;

	*loopstr = ostrcat(*loopstr, "direct connect\n", 1, 0);
	*loopstr1 = ostrcat(*loopstr1, "0\n", 1, 0);

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_QPSK && node->feinfo->type == FE_QPSK)
		{
			if(node->adapter != dvbnode->adapter || node->devnr != dvbnode->devnr)
			{
				tmpnr = oitoa(dvbnode->adapter);
				*loopstr = ostrcat(*loopstr, "loop to Tuner ", 1, 0);
				*loopstr = ostrcat(*loopstr, tmpnr, 1, 1);
				*loopstr = ostrcat(*loopstr, "/", 1, 0);
				tmpnr = oitoa(dvbnode->devnr);
				*loopstr = ostrcat(*loopstr, tmpnr, 1, 1);
				*loopstr = ostrcat(*loopstr, "\n", 1, 0);

				*loopstr1 = ostrcat(*loopstr1, dvbnode->feshortname, 1, 0);
				*loopstr1 = ostrcat(*loopstr1, "\n", 1, 0);
			}
		}
		dvbnode = dvbnode->next;
	}
}

//delete all tunerconfig if loop is selected
void deltunerconfig()
{
	struct dvbdev* dvbnode = dvbdev;
	int i;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && getconfig(dvbnode->feshortname, NULL) != NULL)
		{
			for(i = 1; i <= status.maxsat; i++)
			{
				tmpnr = oitoa(i);
				tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
				tmpstr = ostrcat(tmpstr, "_sat", 1, 0);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
				delconfig(tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
				tmpstr = ostrcat(tmpstr, "_lnb", 1, 0);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
				delconfig(tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
				tmpstr = ostrcat(tmpstr, "_diseqc", 1, 0);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
				delconfig(tmpstr);
				free(tmpstr); tmpstr = NULL;

				free(tmpnr); tmpnr = NULL;
			}
		}
		dvbnode = dvbnode->next;
	}
}

void screentunerconfig()
{
	int rcret = 0, ret = 0;
	struct skin* tunerconfig = getscreen("tunerconfig");
	struct skin* listbox = getscreennode(tunerconfig, "listbox");
	struct dvbdev* dvbnode = dvbdev;
	struct skin* tunernode = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpnr = NULL;
	struct skin* tmp = NULL;
	struct channel* chnode = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			tunernode = addlistbox(tunerconfig, listbox, tunernode, 1);
			if(tunernode != NULL)
			{
				tunernode->handle = (char*)dvbnode;

				tmpnr = oitoa(dvbnode->adapter);
				tmpstr = ostrcat(_("Tuner"), tmpnr, 0, 1);
				tmpstr = ostrcat(tmpstr, " /", 1, 0);
				tmpnr = oitoa(dvbnode->devnr);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
				tmpstr = ostrcat(tmpstr, ": ", 1, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				changetext(tunernode, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tunernode->height = listbox->fontsize * 2;
				tunernode->valign = MIDDLE;
				tunernode->type = CHOICEBOX;
				createloopstr(dvbnode, &tmpstr, &tmpstr1);
				changeinput(tunernode, tmpstr);
				changechoiceboxvalue(tunernode, tmpstr1);
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				changename(tunernode, dvbnode->feshortname);

				setchoiceboxselection(tunernode, getconfig(dvbnode->feshortname, NULL));
			}
		}
		dvbnode = dvbnode->next;
	}

	drawscreen(tunerconfig, 0);
	addscreenrc(tunerconfig, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerconfig, tmp);
		rcret = waitrc(tunerconfig, 0, 0);
		tmp =  listbox->select;

		if(listbox->select != NULL)
			addconfigscreentmpcheck(listbox->select->name, listbox->select, "0");

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL && ostrcmp(listbox->select->ret, "0") == 0)
			{
				if(((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_QPSK)
				{
					clearscreen(tunerconfig);
					ret = screentunerreceptiondvbs((struct dvbdev*)listbox->select->handle);
					drawscreen(tunerconfig, 0);
				}
				if(((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_QAM)
				{
					clearscreen(tunerconfig);
					ret = screentunerreceptiondvbc((struct dvbdev*)listbox->select->handle);
					drawscreen(tunerconfig, 0);
				}
				if(((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_OFDM)
				{
					clearscreen(tunerconfig);
					textbox(_("Message"), _("DVB-T not supportet at the moment !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(tunerconfig, 0);
				}
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writeconfigtmp();
			deltunerconfig();
			break;
		}
	}

	delconfigtmpall();
	delmarkedscreennodes(tunerconfig, 1);
	delownerrc(tunerconfig);
	clearscreen(tunerconfig);

	//tune new if tunerconfig saved
	if(ret == 1)
	{
		drawscreen(skin, 0);
		chnode = status.aktservice->channel;
		ret = servicestop(status.aktservice, 1, 1);
		if(ret == 0)
		{
			status.aktservice->channel = NULL;
			status.aktservice->transponder = NULL;
			servicecheckret(servicestart(chnode, NULL, 0), 0);
		}
	}
}

#endif
