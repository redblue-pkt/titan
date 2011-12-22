#ifndef TUNERCONFIG_H
#define TUNERCONFIG_H

void writetunerconfig(struct dvbdev* tuner, struct skin* tunerreceptiondvbs)
{
	if(tuner == NULL) return;
	if(tunerreceptiondvbs == NULL) return;

	struct skin* child = tunerreceptiondvbs->child;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(child != NULL)
	{
		if(child->del == 1 && child->name != NULL && strstr(child->name, "_lnb") == NULL)
		{
			addconfigscreentmpcheck(child->name, child, "0");
			if(strstr(child->name, "_sat") != NULL && child->ret != NULL && ostrcmp(child->ret, "0") != 0)
			{
				if(child->handle != NULL)
                			tmpnr = oitoa((int)child->handle);
				tmpstr = ostrcat(tuner->feshortname, "_satnr", 0, 0);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
				addconfigtmp(tmpstr, tmpnr);
				free(tmpstr); tmpstr = NULL;
				free(tmpnr); tmpnr = NULL;
			}
		}
		child = child->next;
	}
	writeconfigtmp();
}

void createsatlist(struct dvbdev* tuner, struct skin* tunerreceptiondvbs, struct skin* listbox, int mode, int maxsat)
{
	int i;
	char* maxsatstring = NULL, *satstring = NULL;
	char* orbitalposstring = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;
	struct skin* tmp = NULL;

	satstring = getsatstring(NULL);
	maxsatstring = getmaxsatstring();
	orbitalposstring = getorbitalposstring(NULL);

	tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
	if(tmp != NULL)
	{
		tmp->type = CHOICEBOX;
		changetext(tmp, _("Satelite Type"));
		
		tmpstr = ostrcat(_("Single LNB"), "\n", 0, 0);
		tmpstr = ostrcat(tmpstr, _("DiSEqC A/B"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		tmpstr = ostrcat(tmpstr, _("DiSEqC A/B/C/D"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		tmpstr = ostrcat(tmpstr, _("DiSEqC A/B/C/D/E/F/G/H"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		tmpstr = ostrcat(tmpstr, _("Expert"), 1, 0);
//		changeinput(tmp, _("Single\nDiSEqC A/B\nDiSEqC A/B/C/D\nDiSEqC A/B/C/D/E/F/G/H\nExpert"));
		changeinput(tmp, tmpstr);

		changechoiceboxvalue(tmp, "0\n1\n2\n3\n4");
		changename(tmp, "sat_type");

		if(mode == 0)
			setchoiceboxselection(tmp, "0");
		else if(mode == 1)
			setchoiceboxselection(tmp, "1");
		else if(mode == 2)
			setchoiceboxselection(tmp, "2");
		else if(mode == 3)
			setchoiceboxselection(tmp, "3");
		else if(mode == 4)
			setchoiceboxselection(tmp, "4");

		free(tmpstr); tmpstr = NULL;
		tmp->del = 1;
	}

	if(mode == 4)
	{
		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = CHOICEBOX;
			changetext(tmp, _("Satelite Entrys"));
			changeinput(tmp, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64");
			changechoiceboxvalue(tmp, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64");

			changename(tmp, "sat_max");
			
			tmpstr = oitoa(status.maxsat);
			setchoiceboxselection(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->del = 1;
		}
	}

	tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
	if(tmp != NULL)
	{
		tmp->deaktivcol = 1;
		tmp->height = 20;
		tmp->del = 2;
	}

	for(i = 1; i <= maxsat; i++)
	{
		tmpnr = oitoa(i);

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = CHOICEBOX;
			tmpstr = ostrcat("Sat ", tmpnr, 0, 0);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;

			tmpstr = ostrcat(_("undefined"), "\n", 0, 0);
			tmpstr = ostrcat(tmpstr, satstring, 1, 0);
				
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
		
			tmp->handle = (char*)i;
			tmp->del = 1;
		}

		if(mode == 1)
		{
			tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->type = CHOICEBOX;
				changetext(tmp, _("DiSEqC"));
				tmpstr = ostrcat(_("no"), "\n", 0, 0);
				tmpstr = ostrcat(tmpstr, maxsatstring, 1, 0);
				tmpstr = stringreplacecharonce(tmpstr, '1', 'A');
				tmpstr = stringreplacecharonce(tmpstr, '2', 'B');
				changechoiceboxvalue(tmp, tmpstr);
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

				tmp->handle = (char*)i;
				tmp->del = 1;
			}
		}
		else if(mode == 2)
		{
			tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->type = CHOICEBOX;
				changetext(tmp, _("DiSEqC"));
				tmpstr = ostrcat(_("no"), "\n", 0, 0);
				tmpstr = ostrcat(tmpstr, maxsatstring, 1, 0);
				tmpstr = stringreplacecharonce(tmpstr, '1', 'A');
				tmpstr = stringreplacecharonce(tmpstr, '2', 'B');
				tmpstr = stringreplacecharonce(tmpstr, '3', 'C');
				tmpstr = stringreplacecharonce(tmpstr, '4', 'D');
				changechoiceboxvalue(tmp, tmpstr);
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

				tmp->handle = (char*)i;
				tmp->del = 1;
			}
		}
		else if(mode == 3)
		{
			tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->type = CHOICEBOX;
				changetext(tmp, _("DiSEqC"));
				tmpstr = ostrcat(_("no"), "\n", 0, 0);
				tmpstr = ostrcat(tmpstr, maxsatstring, 1, 0);
				tmpstr = stringreplacecharonce(tmpstr, '1', 'A');
				tmpstr = stringreplacecharonce(tmpstr, '2', 'B');
				tmpstr = stringreplacecharonce(tmpstr, '3', 'C');
				tmpstr = stringreplacecharonce(tmpstr, '4', 'D');
				tmpstr = stringreplacecharonce(tmpstr, '5', 'E');
				tmpstr = stringreplacecharonce(tmpstr, '6', 'F');
				tmpstr = stringreplacecharonce(tmpstr, '7', 'G');
				tmpstr = stringreplacecharonce(tmpstr, '8', 'H');
				changechoiceboxvalue(tmp, tmpstr);
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
				
				tmp->handle = (char*)i;
				tmp->del = 1;
			}
		}
		else if(mode == 4)
		{
			tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->type = CHOICEBOX;
				changetext(tmp, _("DiSEqC"));
				tmpstr = ostrcat(_("no"), "\n", 0, 0);
				tmpstr = ostrcat(tmpstr, maxsatstring, 1, 0);
				changechoiceboxvalue(tmp, tmpstr);
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

				tmp->handle = (char*)i;
				tmp->del = 1;
			}
		}
		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = CHOICEBOX;
			changetext(tmp, _("Lnb / Unicable"));
			changeinput(tmp, _("press ok"));
			changechoiceboxvalue(tmp, _("press ok"));

			tmp->handle = (char*)i;
			tmp->del = 1;
		}

/*
		tmp = addlistbox(tunerreceptiondvbs, listbox, NULL, 1);
		if(tmp != NULL)
		{
			tmp->type = CHOICEBOX;
			changetext(tmp, _("Rotor Position"));
			tmpstr = ostrcat(_("no"), "\n", 0, 0);
			tmpstr = ostrcat(tmpstr, maxsatstring, 1, 0);
			changeinput(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
*/

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->deaktivcol = 1;
			tmp->height = 20;
			tmp->del = 2;
		}

		free(tmpnr);
		if(mode == 0) break;
	}

	free(satstring);
	free(maxsatstring);
	free(orbitalposstring);
}

int screentunerreceptiondvbs(struct dvbdev* tuner)
{
	int rcret = 0, ret = 0, mode = 0, maxsat = 1;
	
	struct skin* tunerreceptiondvbs = getscreen("tunerreceptiondvbs");
	struct skin* listbox = getscreennode(tunerreceptiondvbs, "listbox");
//	struct skin* longitude = getscreennode(tunerreceivedvbs, "longitude");
//	struct skin* east_west = getscreennode(tunerreceivedvbs, "east_west");
//	struct skin* latitude = getscreennode(tunerreceivedvbs, "latitude");
//	struct skin* north_south = getscreennode(tunerreceivedvbs, "north_south");
	struct skin* tmp = NULL;

	struct dvbdev* dvbnode = dvbdev;
	
	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(tuner == NULL || tuner->feshortname == NULL)
	{
		err("out -> NULL detect");
		return 0;
	}

	char* listmode = NULL;
	listmode = ostrcat(tuner->feshortname, "_mode", 0, 0);
	mode = getconfigint(listmode, NULL);

	char* listmax = NULL;
	listmax = ostrcat(tuner->feshortname, "_max", 0, 0);
	maxsat = getconfigint(listmax, NULL);
	if(maxsat == 0)
		maxsat = 1;

start:
	createsatlist(tuner, tunerreceptiondvbs, listbox, mode, maxsat);
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

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			status.maxsat = getconfigint("maxsat", NULL);
			break;
		}
		if(ostrcmp(listbox->select->name, "sat_type") == 0)
		{
			printf("listbox->select->ret: %s\n", listbox->select->ret);
			if((ostrcmp(listbox->select->ret, "0") == 0) && (mode != 0))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				mode = 0;
				maxsat = 1;
				if(status.maxsat <= maxsat)
					status.maxsat = maxsat;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "1") == 0) && (mode != 1))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				mode = 1;
				maxsat = 2;
//				if(status.maxsat <= maxsat)
					status.maxsat = maxsat;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "2") == 0) && (mode != 2))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				mode = 2;
				maxsat = 4;
//				if(status.maxsat <= maxsat)
					status.maxsat = maxsat;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "3") == 0) && (mode != 3))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				mode = 3;
				maxsat = 8;
//				if(status.maxsat <= maxsat)
					status.maxsat = maxsat;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "4") == 0) && (mode != 4))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				mode = 4;
//				status.maxsat = 0;
				goto start;
			}
		}
		else if(ostrcmp(listbox->select->name, "sat_max") == 0)
		{
			if(atoi(listbox->select->ret) != maxsat)
			{
				maxsat = atoi(listbox->select->ret);
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				mode = 4;
//				if(status.maxsat <= maxsat)
					status.maxsat = maxsat;			
				goto start;
			}
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->text, "Lnb / Unicable") == 0 && listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") != 0)
			{
				clearscreen(tunerreceptiondvbs);
				screenlnb(tuner->feshortname, listbox->select->handle);
				drawscreen(tunerreceptiondvbs, 0);
			}
			if(listbox->select != NULL && ostrcmp(listbox->select->text, "DiSEqC") == 0 && listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") != 0)
			{
				clearscreen(tunerreceptiondvbs);
				screendiseqc(tuner->feshortname, listbox->select->handle);
				drawscreen(tunerreceptiondvbs, 0);
			}
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfig(tuner, tunerreceptiondvbs);
			addconfigint(listmode, mode);
			addconfigint(listmax, maxsat);

			int checkmaxsat = 0;
			while(dvbnode != NULL)
			{
				if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
				{
					listmax = ostrcat(tuner->feshortname, "_max", 0, 0);
					maxsat = getconfigint(listmax, NULL);
					if(checkmaxsat < maxsat)
						checkmaxsat = maxsat;
				}
				dvbnode = dvbnode->next;
			}

			if(checkmaxsat != 0)
			{
				status.maxsat = checkmaxsat;
				addconfigint("maxsat", status.maxsat);
			}
									 
			free(listmode), listmode = NULL;
			writeallconfig(0);
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
			writetunerconfig(tuner, tunerreceptiondvbc);
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

	*loopstr = ostrcat(*loopstr, _("direct connect"), 1, 0);
	*loopstr = ostrcat(*loopstr, "\n", 1, 0);
	*loopstr1 = ostrcat(*loopstr1, "0\n", 1, 0);

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_QPSK && node->feinfo->type == FE_QPSK)
		{
			if(node->adapter != dvbnode->adapter || node->devnr != dvbnode->devnr)
			{
				tmpnr = oitoa(dvbnode->adapter);
				*loopstr = ostrcat(*loopstr, _("loop to Tuner"), 1, 0);
				*loopstr = ostrcat(*loopstr, " ", 1, 0);
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
				tmpstr = ostrcat(tmpstr, "_satnr", 1, 0);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
				delconfig(tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
				tmpstr = ostrcat(tmpstr, "_diseqc", 1, 0);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
				delconfig(tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
				tmpstr = ostrcat(tmpstr, "_mode", 1, 0);
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
//	int rcret = 0, ret = 0, mode = 0;
	int rcret = 0, ret = 0;
	struct skin* tunerconfig = getscreen("tunerconfig");
	struct skin* listbox = getscreennode(tunerconfig, "listbox");
//	struct skin* tunemode = getscreennode(tunerconfig, "mode");
	struct dvbdev* dvbnode = dvbdev;
	struct skin* tunernode = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpnr = NULL;
	struct skin* tmp = NULL;

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
				tmpstr = ostrcat(_("Tuner "), tmpnr, 0, 1);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
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
					textbox(_("Message"), _("DVB-T not supportet at the moment !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(tunerconfig, 0);
				}
			}
		}
		if(rcret == getrcconfigint("rcred", NULL))
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
		if(status.aktservice->fedev != NULL)
		{
			status.aktservice->fedev->felasttransponder = NULL;
			status.aktservice->fedev->feaktpolarization = 0;
			status.aktservice->fedev->feakttransponder = NULL;
		}
		ret = servicestop(status.aktservice, 1, 1);
		if(ret == 0)
		{
			status.aktservice->transponder = NULL;
			servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 3), 0);
		}
	}
}

#endif
