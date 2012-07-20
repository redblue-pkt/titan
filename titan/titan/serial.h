#ifndef SERIAL_H
#define SERIAL_H

void screenserial()
{
	int rcret = 0;
	unsigned int hash = 0;
	unsigned long code = 0;
	struct skin* serial = getscreen("serial");
	struct skin* listbox = getscreennode(serial, "listbox");
	struct skin* id = getscreennode(serial, "id");
	struct skin* code1 = getscreennode(serial, "code1");
	struct skin* code2 = getscreennode(serial, "code2");
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

	drawscreen(serial, 0, 0);
	addscreenrc(serial, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(serial, tmp);
		rcret = waitrc(serial, 0, 0);
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
				writeserial(cpuid);
				checkserial(cpuid);
				if(status.security == 1)
				{
					textbox(_("Message"), _("Code ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
					break;
				}
			}
			textbox(_("Message"), _("Code not ok, please try again !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(serial, 0, 0);
		}
	}

	free(cpuid); cpuid = NULL;
	delownerrc(serial);
	clearscreen(serial);
}

#endif
