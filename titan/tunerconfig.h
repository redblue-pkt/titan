#ifndef TUNERCONFIG_H
#define TUNERCONFIG_H

void writetunerconfigsat(struct dvbdev* tuner, struct skin* tunerreceptiondvbs)
{
	if(tuner == NULL) return;
	if(tunerreceptiondvbs == NULL) return;

	struct skin* child = tunerreceptiondvbs->child;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(child != NULL)
	{
		if(child->del == 1 && child->name != NULL && ostrstr(child->name, "_lnb") == NULL)
		{
			addconfigscreentmpcheck(child->name, child, "0");
			if(ostrstr(child->name, "_sat") != NULL && child->ret != NULL && ostrcmp(child->ret, "0") != 0)
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

void writetunerconfigcable(struct dvbdev* tuner, struct skin* tunerreceptiondvbc)
{
	if(tuner == NULL) return;
	if(tunerreceptiondvbc == NULL) return;

	struct skin* child = tunerreceptiondvbc->child;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(child != NULL)
	{
		if(child->del == 1 && child->name != NULL)
		{
			addconfigscreentmpcheck(child->name, child, "0");
			if(ostrstr(child->name, "_sat") != NULL && child->ret != NULL && ostrcmp(child->ret, "0") != 0)
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

void writetunerconfigterr(struct dvbdev* tuner, struct skin* tunerreceptiondvbt)
{
	if(tuner == NULL) return;
	if(tunerreceptiondvbt == NULL) return;

	struct skin* child = tunerreceptiondvbt->child;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(child != NULL)
	{
		if(child->del == 1 && child->name != NULL)
		{
			addconfigscreentmpcheck(child->name, child, "0");
			if(ostrstr(child->name, "_sat") != NULL && child->ret != NULL && ostrcmp(child->ret, "0") != 0)
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

void createsatlist(struct dvbdev* tuner, struct skin* tunerreceptiondvbs, struct skin* listbox, int maxsat)
{
	int i;
	char* maxsatstring = NULL, *satstring = NULL;
	char* orbitalposstring = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;
	struct skin* tmp = NULL;

	satstring = getsatstring(NULL, FE_QPSK);
	maxsatstring = getmaxsatstring(maxsat);
	orbitalposstring = getorbitalposstring(NULL, FE_QPSK);

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
		changeinput(tmp, tmpstr);

		changechoiceboxvalue(tmp, "0\n1\n2\n3\n4");
		changename(tmp, "sat_type");

		if(maxsat == 1)
			setchoiceboxselection(tmp, "0");
		else if(maxsat == 2)
			setchoiceboxselection(tmp, "1");
		else if(maxsat == 4)
			setchoiceboxselection(tmp, "2");
		else if(maxsat == 8)
			setchoiceboxselection(tmp, "3");
		else if(maxsat == MAXSAT)
			setchoiceboxselection(tmp, "4");

		free(tmpstr); tmpstr = NULL;
		tmp->del = 1;
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

		if(maxsat == 2)
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
		else if(maxsat == 4)
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
		else if(maxsat == 8)
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
		else if(maxsat == MAXSAT)
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
			changeinput(tmp, _("press red"));
			changechoiceboxvalue(tmp, _("press red"));

			tmp->handle = (char*)i;
			tmp->del = 1;
		}

		tmp = addlistbox(tunerreceptiondvbs, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->deaktivcol = 1;
			tmp->height = 20;
			tmp->del = 2;
		}

		free(tmpnr);
	}

	free(satstring);
	free(maxsatstring);
	free(orbitalposstring);
}

void createcablelist(struct dvbdev* tuner, struct skin* tunerreceptiondvbc, struct skin* listbox)
{
	char *satstring = NULL;
	char* orbitalposstring = NULL;
	char* tmpstr = NULL;
	struct skin* tmp = NULL;

	satstring = getsatstring(NULL, FE_QAM);
	orbitalposstring = getorbitalposstring(NULL, FE_QAM);

	tmp = addlistbox(tunerreceptiondvbc, listbox, tmp, 1);
	if(tmp != NULL)
	{
		tmp->type = CHOICEBOX;
		changetext(tmp, _("Provider (DVB-C)"));

		tmpstr = ostrcat(_("undefined"), "\n", 0, 0);
		tmpstr = ostrcat(tmpstr, satstring, 1, 0);

		changeinput(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat("0\n", orbitalposstring, 0, 0);
		changechoiceboxvalue(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(tuner->feshortname, "_sat", 0, 0);
		tmpstr = ostrcat(tmpstr, "1", 1, 0);
		changename(tmp, tmpstr);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;

		tmp->handle = (char*)1;
		tmp->del = 1;
	}

	free(satstring);
	free(orbitalposstring);
}

void createhypridlist(struct dvbdev* tuner, struct skin* tunerreceptionhyprid, struct skin* listbox)
{
	char *satstring = NULL;
	char* orbitalposstring = NULL;
	char* tmpstr = NULL;
	struct skin* tmp = NULL;

	satstring = getsatstring(NULL, FE_QAM);
	orbitalposstring = getorbitalposstring(NULL, FE_QAM);

	char* hypridlist = gethypridtunerchoices(tuner->devnr);
	char* hypridlistvalve = gethypridtunerchoicesvalue(tuner->devnr);
	
	tmp = addlistbox(tunerreceptionhyprid, listbox, tmp, 1);
	if(tmp != NULL)
	{
		tmp->type = CHOICEBOX;
		changetext(tmp, _("Tuner Type"));
		
		changeinput(tmp, hypridlistvalve);
		changechoiceboxvalue(tmp, hypridlist);

		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(tuner->feshortname, "_hyprid", 0, 0);
//		tmpstr = ostrcat(tmpstr, tuner->fehyprid, 1, 0);
		changename(tmp, tmpstr);		
		
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));

		free(tmpstr); tmpstr = NULL;
		tmp->del = 1;
	}

	free(satstring);
	free(orbitalposstring);
}

void createterrlist(struct dvbdev* tuner, struct skin* tunerreceptiondvbt, struct skin* listbox)
{
	char *satstring = NULL;
	char* orbitalposstring = NULL;
	char* tmpstr = NULL;
	struct skin* tmp = NULL;

	satstring = getsatstring(NULL, FE_OFDM);
	orbitalposstring = getorbitalposstring(NULL, FE_OFDM);

	tmp = addlistbox(tunerreceptiondvbt, listbox, tmp, 1);
	if(tmp != NULL)
	{
		tmp->type = CHOICEBOX;
		changetext(tmp, _("Provider (DVB-T)"));

		tmpstr = ostrcat(_("undefined"), "\n", 0, 0);
		tmpstr = ostrcat(tmpstr, satstring, 1, 0);

		changeinput(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat("0\n", orbitalposstring, 0, 0);
		changechoiceboxvalue(tmp, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(tuner->feshortname, "_sat", 0, 0);
		tmpstr = ostrcat(tmpstr, "1", 1, 0);
		changename(tmp, tmpstr);
		setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
		free(tmpstr); tmpstr = NULL;

		tmp->handle = (char*)1;
		tmp->del = 1;
	}

	free(satstring);
	free(orbitalposstring);
}

int screentunerreceptiondvbs(struct dvbdev* tuner)
{
	int rcret = 0, ret = 0, maxsat = 1;
	struct skin* tunerreceptiondvbs = getscreen("tunerreceptiondvbs");
	struct skin* listbox = getscreennode(tunerreceptiondvbs, "listbox");
	struct skin* b3 = getscreennode(tunerreceptiondvbs, "b3");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;
	
	listbox->aktline = 1;
	listbox->aktpage = -1;
	b3->hidden = YES;

	if(tuner == NULL || tuner->feshortname == NULL)
	{
		err("out -> NULL detect");
		return 0;
	}

	tmpstr = ostrcat(tuner->feshortname, "_maxsat", 0, 0);
	maxsat = getconfigint(tmpstr, NULL);
	free(tmpstr); tmpstr = NULL;
	if(maxsat < 1) maxsat = 1;
start:
	createsatlist(tuner, tunerreceptiondvbs, listbox, maxsat);

	b3->usesavebg = 1;
	drawscreen(tunerreceptiondvbs, 2, 0);
	b3->usesavebg = YES;

	drawscreen(tunerreceptiondvbs, 0, 0);
	addscreenrc(tunerreceptiondvbs, listbox);

			
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerreceptiondvbs, tmp);
		rcret = waitrc(tunerreceptiondvbs, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			break;
		}

		if(ostrcmp(listbox->select->text, _("Lnb / Unicable")) == 0)
		{
			b3->hidden = NO;
			changetext(b3, _("EDIT LNB / UNICABLE"));
		}
		else if(ostrcmp(listbox->select->text, _("DiSEqC")) == 0)
		{
			b3->hidden = NO;
			changetext(b3, _("EDIT DiSEqC"));
		}
		else
			b3->hidden = YES;	
		drawscreen(tunerreceptiondvbs, 0, 0);
					
		if(ostrcmp(listbox->select->name, "sat_type") == 0)
		{
			if((ostrcmp(listbox->select->ret, "0") == 0) && (maxsat != 1))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				maxsat = 1;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "1") == 0) && (maxsat != 2))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				maxsat = 2;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "2") == 0) && (maxsat != 4))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				maxsat = 4;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "3") == 0) && (maxsat != 8))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				maxsat = 8;
				goto start;
			}
			else if((ostrcmp(listbox->select->ret, "4") == 0) && (maxsat != MAXSAT))
			{
				delconfigtmpall();
				delmarkedscreennodes(tunerreceptiondvbs, 1);
				delmarkedscreennodes(tunerreceptiondvbs, 2);
				delownerrc(tunerreceptiondvbs);
				clearscreen(tunerreceptiondvbs);
				maxsat = MAXSAT;
				goto start;
			}
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->text, _("Lnb / Unicable")) == 0 && listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") != 0)
			{
				clearscreen(tunerreceptiondvbs);
				screenlnb(tuner->feshortname, listbox->select->handle);
				drawscreen(tunerreceptiondvbs, 0, 0);
			}
			if(listbox->select != NULL && ostrcmp(listbox->select->text, _("DiSEqC")) == 0 && listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") != 0)
			{
				clearscreen(tunerreceptiondvbs);
				screendiseqc(tuner->feshortname, listbox->select->handle);
				drawscreen(tunerreceptiondvbs, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfigsat(tuner, tunerreceptiondvbs);
			tmpstr = ostrcat(tuner->feshortname, "_maxsat", 0, 0);
			addconfigint(tmpstr, maxsat);
			free(tmpstr); tmpstr = NULL;
			writeallconfig(1);
// test reload tuner
			changedvbdev(tuner);

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

int screentunerreceptionhyprid(struct dvbdev* tuner)
{
	int rcret = 0, ret = 0;
	struct skin* tunerreceptionhyprid = getscreen("tunerreceptionhyprid");
	struct skin* listbox = getscreennode(tunerreceptionhyprid, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(tuner == NULL || tuner->feshortname == NULL)
	{
		err("out -> NULL detect");
		return 0;
	}

	createhypridlist(tuner, tunerreceptionhyprid, listbox);

	drawscreen(tunerreceptionhyprid, 0, 0);
	addscreenrc(tunerreceptionhyprid, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerreceptionhyprid, tmp);
		rcret = waitrc(tunerreceptionhyprid, 0, 0);
		tmp = listbox->select;

		drawscreen(tunerreceptionhyprid, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfigcable(tuner, tunerreceptionhyprid);
	
			tmpstr = ostrcat(tuner->feshortname, "_hyprid", 0, 0);
			addconfig(tmpstr, listbox->select->ret);
			free(tmpstr); tmpstr = NULL;
			writeallconfig(1);

			clearscreen(tunerreceptionhyprid);
			char* realname = gethypridtunerchoicesvaluename(tuner->devnr, listbox->select->ret);
			
//			if(realname != NULL && ostrcmp(realname, "DVB-T2") == 0)
			if(realname != NULL && ostrstr(realname, "DVB-T") != NULL)
			{
				ret = screentunerreceptiondvbt(tuner);
				if(ret == 1)
				{
					sethypridtuner(tuner->devnr, listbox->select->ret);
//					free(realname) , realname = NULL;
//					realname = gethypridtunername(tuner->devnr, listbox->select->ret);
//					strcpy(tuner->feinfo->name, realname);
//					textbox(_("Message"), _("They need to switch the tuner Hyprid restart the gui !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
					if(checkbox("SPARK7162") == 1)
					{
						if(textbox(_("Message"), _("Hybrid tuner mode changed, receiver will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
							oshutdown(2, 1);
					}
					changedvbdev(tuner);
				}
			}
//			else if(realname != NULL && ostrcmp(realname, "DVB-C") == 0)
			else if(realname != NULL && ostrstr(realname, "DVB-C") != NULL)
			{
				ret = screentunerreceptiondvbc(tuner);
				if(ret == 1)
				{
					sethypridtuner(tuner->devnr, listbox->select->ret);
//					free(realname) , realname = NULL;
//					realname = gethypridtunername(tuner->devnr, listbox->select->ret);
//					strcpy(tuner->feinfo->name, realname);
//					textbox(_("Message"), _("They need to switch the tuner Hyprid restart the gui !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
					if(checkbox("SPARK7162") == 1)
					{
						if(textbox(_("Message"), _("Hybrid tuner mode changed, receiver will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
							oshutdown(2, 1);
					}
					changedvbdev(tuner);
				}
			}
			free(realname), realname = NULL;
/*
			if(listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "0") == 0)
				ret = screentunerreceptiondvbt(tuner);
			else if(listbox->select->ret != NULL && ostrcmp(listbox->select->ret, "2") == 0)
				ret = screentunerreceptiondvbc(tuner);
*/
			//drawscreen(tunerreceptionhyprid, 0, 0);
			
			break;
		}
	}

	delconfigtmpall();
	delmarkedscreennodes(tunerreceptionhyprid, 1);
	delownerrc(tunerreceptionhyprid);
	clearscreen(tunerreceptionhyprid);
	return ret;
}

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

	createcablelist(tuner, tunerreceptiondvbc, listbox);

	drawscreen(tunerreceptiondvbc, 0, 0);
	addscreenrc(tunerreceptiondvbc, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerreceptiondvbc, tmp);
		rcret = waitrc(tunerreceptiondvbc, 0, 0);
		tmp = listbox->select;

		drawscreen(tunerreceptiondvbc, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfigcable(tuner, tunerreceptiondvbc);
	
			tmpstr = ostrcat(tuner->feshortname, "_maxsat", 0, 0);
			addconfigint(tmpstr, 1);
			free(tmpstr); tmpstr = NULL;
			writeallconfig(1);
// test reload tuner
			changedvbdev(tuner);

			break;
		}
	}

	delconfigtmpall();
	delmarkedscreennodes(tunerreceptiondvbc, 1);
	delownerrc(tunerreceptiondvbc);
	clearscreen(tunerreceptiondvbc);
	return ret;
}

int screentunerreceptiondvbt(struct dvbdev* tuner)
{
	int rcret = 0, ret = 0;
	struct skin* tunerreceptiondvbt = getscreen("tunerreceptiondvbt");
	struct skin* listbox = getscreennode(tunerreceptiondvbt, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(tuner == NULL || tuner->feshortname == NULL)
	{
		err("out -> NULL detect");
		return 0;
	}

	createterrlist(tuner, tunerreceptiondvbt, listbox);

	drawscreen(tunerreceptiondvbt, 0, 0);
	addscreenrc(tunerreceptiondvbt, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerreceptiondvbt, tmp);
		rcret = waitrc(tunerreceptiondvbt, 0, 0);
		tmp = listbox->select;

		drawscreen(tunerreceptiondvbt, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writetunerconfigterr(tuner, tunerreceptiondvbt);

			tmpstr = ostrcat(tuner->feshortname, "_maxsat", 0, 0);
			addconfigint(tmpstr, 1);
			free(tmpstr); tmpstr = NULL;
			writeallconfig(1);
// test reload tuner
			changedvbdev(tuner);

			break;
		}
	}

	delconfigtmpall();
	delmarkedscreennodes(tunerreceptiondvbt, 1);
	delownerrc(tunerreceptiondvbt);
	clearscreen(tunerreceptiondvbt);
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

	*loopstr = ostrcat(*loopstr, _("off"), 1, 0);
	*loopstr = ostrcat(*loopstr, "\n", 1, 0);
	*loopstr1 = ostrcat(*loopstr1, "x\n", 1, 0);
}

//flag 0: del tunerconfig that are loop
//flag 1: del all tunerconfig greater than maxsat
void deltunerconfig(int flag)
{
	struct dvbdev* dvbnode = dvbdev;
	int i, start = 0, end = 0;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			if(flag == 0 && (getconfig(dvbnode->feshortname, NULL) == NULL || ostrcmp(getconfig(dvbnode->feshortname, NULL), "x") == 0))
			{
				dvbnode = dvbnode->next;
				continue;
			}
			if(flag == 0)
			{
				start = 1;
				end = getmaxsat(dvbnode->feshortname);
			}
			else
			{
				start = getmaxsat(dvbnode->feshortname) + 1;
				end = MAXSAT;
			}

			for(i = start; i <= end; i++)
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

				free(tmpnr); tmpnr = NULL;
			}

			if(flag == 0)
			{
				tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
				tmpstr = ostrcat(tmpstr, "_maxsat", 1, 0);
				delconfig(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			
			//del all diseqc settings on single LNB
			if(getmaxsat(dvbnode->feshortname) == 1)
			{
				for(i = 1; i <= MAXSAT; i++)
				{
					tmpnr = oitoa(i);
          
					tmpstr = ostrcat(tmpstr, dvbnode->feshortname, 1, 0);
					tmpstr = ostrcat(tmpstr, "_diseqc", 1, 0);
					tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
					delconfig(tmpstr);
					free(tmpstr); tmpstr = NULL;
  
					free(tmpnr); tmpnr = NULL;
				}
			}
		}
		dvbnode = dvbnode->next;
	}
}

void screentunerconfig()
{
	int rcret = 0, ret = 0, count = 0;
	struct skin* tunerconfig = getscreen("tunerconfig");
	struct skin* listbox = getscreennode(tunerconfig, "listbox");
	struct skin* b4 = getscreennode(tunerconfig, "b4");
	
	struct dvbdev* dvbnode = dvbdev;
	struct skin* tunernode = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	struct skin* tmp = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	b4->hidden = YES;

	if(status.recording > 0 || status.streaming > 0)
	{
		textbox(_("Message"), _("Tunerconfig is not allowed if record\nor stream is running !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		return;
	}

	drawscreen(tunerconfig, 0, 0);

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			tunernode = addlistbox(tunerconfig, listbox, tunernode, 1);
			if(tunernode != NULL)
			{
				count++;
				tunernode->handle = (char*)dvbnode;	
				changetunername(tunernode, dvbnode->adapter, dvbnode->devnr, dvbnode->feinfo->name, dvbnode->fehyprid);
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
				if(count == 1 && dvbnode->feinfo->type == FE_QPSK)
					b4->hidden = NO;
			}
		}
		dvbnode = dvbnode->next;
	}

	drawscreen(tunerconfig, 0, 0);
	addscreenrc(tunerconfig, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tunerconfig, tmp);
		rcret = waitrc(tunerconfig, 0, 0);
		tmp =  listbox->select;

		if(listbox->select != NULL)
			addconfigscreentmpcheck(listbox->select->name, listbox->select, "0");

//		if(ostrstr(listbox->select->text, "DVB-S") != NULL || ostrstr(listbox->select->text, "STV090x") != NULL)		
		if(listbox->select != NULL && listbox->select->handle != NULL && ((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_QPSK)		
			b4->hidden = NO;
		else
			b4->hidden = YES;	

		drawscreen(tunerconfig, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL && ostrcmp(listbox->select->ret, "0") == 0)
			{
				if(((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_QPSK)
				{
					clearscreen(tunerconfig);
					ret = screentunerreceptiondvbs((struct dvbdev*)listbox->select->handle);
					drawscreen(tunerconfig, 0, 0);
				}
				if(((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_QAM)
				{
					clearscreen(tunerconfig);
					if(((struct dvbdev*)listbox->select->handle)->fehyprid == NULL)
						ret = screentunerreceptiondvbc((struct dvbdev*)listbox->select->handle);
					else
						ret = screentunerreceptionhyprid((struct dvbdev*)listbox->select->handle);

					changetunername(tunernode, ((struct dvbdev*)listbox->select->handle)->adapter, ((struct dvbdev*)listbox->select->handle)->devnr, ((struct dvbdev*)listbox->select->handle)->feinfo->name, ((struct dvbdev*)listbox->select->handle)->fehyprid);
					drawscreen(tunerconfig, 0, 0);
				}
				if(((struct dvbdev*)listbox->select->handle)->feinfo->type == FE_OFDM)
				{
					clearscreen(tunerconfig);
					if(((struct dvbdev*)listbox->select->handle)->fehyprid == NULL)
						ret = screentunerreceptiondvbt((struct dvbdev*)listbox->select->handle);
					else
						ret = screentunerreceptionhyprid((struct dvbdev*)listbox->select->handle);
	
					changetunername(tunernode, ((struct dvbdev*)listbox->select->handle)->adapter, ((struct dvbdev*)listbox->select->handle)->devnr, ((struct dvbdev*)listbox->select->handle)->feinfo->name, ((struct dvbdev*)listbox->select->handle)->fehyprid);
					drawscreen(tunerconfig, 0, 0);
				}
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(tunerconfig);
			screenownpos();
			drawscreen(tunerconfig, 0, 0);
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = 1;
			deltranspondertunablestatus();
			writeconfigtmp();
			deltunerconfig(0);
			break;
		}
	}

	deltunerconfig(1);
	delconfigtmpall();
	delmarkedscreennodes(tunerconfig, 1);
	delownerrc(tunerconfig);
	clearscreen(tunerconfig);
	settunerstatus();

	//tune new if tunerconfig saved
	if(ret == 1)
	{
		ret = servicestop(status.aktservice, 1, 1);
		if(ret == 0)
		{
			status.aktservice->transponder = NULL;
			servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 5), 0);
		}
		resettvpic();
		drawscreen(tunerconfig, 0, 0);
		clearscreen(tunerconfig);
		drawscreen(skin, 0, 0);
	}
}

#endif
