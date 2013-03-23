/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef TIWAKEUP_H
#define TIWAKEUP_H

void screentiwakeup()
{
	int rcret = -1, ret = 0;
	struct skin* tiwakeup = getscreen("tiwakeup");
	struct skin* listbox = getscreennode(tiwakeup, "listbox");
	struct skin* tiwakeup1 = getscreennode(tiwakeup, "tiwakeup1");
	struct skin* tiwakeup2 = getscreennode(tiwakeup, "tiwakeup2");
	struct skin* tiwakeup3 = getscreennode(tiwakeup, "tiwakeup3");
	struct skin* tiwakeup4 = getscreennode(tiwakeup, "tiwakeup4");
	struct skin* tiwakeup5 = getscreennode(tiwakeup, "tiwakeup5");
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	changemask(tiwakeup1, "000.000.000.000");
	changeinput(tiwakeup1, getconfig("tiwakeup1", NULL));
	changemask(tiwakeup2, "000.000.000.000");
	changeinput(tiwakeup2, getconfig("tiwakeup2", NULL));
	changemask(tiwakeup3, "000.000.000.000");
	changeinput(tiwakeup3, getconfig("tiwakeup3", NULL));
	changemask(tiwakeup4, "000.000.000.000");
	changeinput(tiwakeup4, getconfig("tiwakeup4", NULL));
	changemask(tiwakeup5, "000.000.000.000");
	changeinput(tiwakeup5, getconfig("tiwakeup5", NULL));

	drawscreen(tiwakeup, 0, 0);
	addscreenrc(tiwakeup, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tiwakeup, tmp);
		rcret = waitrc(tiwakeup, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("tiwakeup1", tiwakeup1, "000.000.000.000");
			addconfigscreencheck("tiwakeup2", tiwakeup2, "000.000.000.000");
			addconfigscreencheck("tiwakeup3", tiwakeup3, "000.000.000.000");
			addconfigscreencheck("tiwakeup4", tiwakeup4, "000.000.000.000");
			addconfigscreencheck("tiwakeup5", tiwakeup5, "000.000.000.000");
	
			if(listbox->select != NULL && listbox->select->ret != NULL)
			{
				drawscreen(load, 0, 0);
				tmpstr = fixip(listbox->select->ret, 1);
				tmpstr = ostrcat("ether-wake.sh ", tmpstr, 0, 1);
				ret = WEXITSTATUS(system(tmpstr));
				free(tmpstr); tmpstr = NULL;
				clearscreen(load);

				if(ret == 0)
					textbox(_("Message"), "Send wakeup ok!", _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				else
					textbox(_("Message"), "Send wakeup not ok!", _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			}

			drawscreen(tiwakeup, 0, 0);
			continue;
		}
	}

	delownerrc(tiwakeup);
	clearscreen(tiwakeup);
}

#endif
