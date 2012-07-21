#ifndef UNLOCK_H
#define UNLOCK_H

void screenunlock()
{
	int rcret = 0;
	unsigned int hash = 0;
	unsigned long code = 0;
	struct skin* unlock = getscreen("unlock");
	struct skin* listbox = getscreennode(unlock, "listbox");
	struct skin* id = getscreennode(unlock, "id");
	struct skin* code1 = getscreennode(unlock, "code1");
	struct skin* code2 = getscreennode(unlock, "code2");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *cpuid = NULL, *pw = NULL;
	unsigned char* buf = NULL;

	if(status.security == 1)
	{
		textbox(_("Message"), _("Spezialfunctions unlocked, no input required"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 700, 200, 10, 0);
		return;
	}

	cpuid = getcpuid();

	tmpstr = ostrcat(_("RECEIVER ID: "), cpuid, 0, 0);
	changetext(id, tmpstr);
	free(tmpstr); tmpstr = NULL;

	changeinput(code1, "00000");
	changeinput(code2, "00000");

	drawscreen(unlock, 0, 0);
	addscreenrc(unlock, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(unlock, tmp);
		rcret = waitrc(unlock, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			pw = getserialpw();
			buf = oencrypt(pw, cpuid, strlen(cpuid));
			hash = gethash((char*)buf);
			if(hash < 1000000000) hash = hash + 1000000000;
			free(pw); pw = NULL;
			free(buf); buf = NULL;

			tmpstr = ostrcat(tmpstr, code1->ret, 1, 0);
			tmpstr = ostrcat(tmpstr, code2->ret, 1, 0);

			if(tmpstr != NULL)
				code = strtoul(tmpstr, NULL, 10);

			free(tmpstr); tmpstr = NULL;

			if(code != 0 && code == hash)
			{
				status.security = 1;
				int ret = writeserial(cpuid);
				if(ret == 1)
					textbox(_("Message"), _("Can't save Code.\nDelete Plugins to free space"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);

				checkserial(cpuid);
				if(status.security == 1)
				{
					textbox(_("Message"), _("Code ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
					break;
				}
			}
			textbox(_("Message"), _("Code not ok, please try again !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(unlock, 0, 0);
		}
	}

	free(cpuid); cpuid = NULL;
	delownerrc(unlock);
	clearscreen(unlock);
}

#endif
