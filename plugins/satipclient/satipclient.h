#ifndef SATIPCLIENT_H
#define SATIPCLIENT_H

void screensatipclient()
{
	int rcret = -1, ret = 0, i, j, count1 = 0, count2, restart;
	char* tmpstr = NULL,*tmpstr1 = NULL, *tmpstr2 = NULL, *satipclientstop = NULL, *satipclientstart = NULL, *satipclientrestart = NULL, *satipclientscan = NULL, *satipclientrun = NULL, *satipclientconfig = NULL, *cmd = NULL;
	char* name = NULL, *text = NULL, *type = NULL, *ip = NULL, *showname = NULL, *desc = NULL, *vtunerconf = NULL, *vtunertmp = NULL;

start:

	restart = 0;
	satipclientstop = ostrcat("/etc/init.d/satipclient stop", NULL, 0, 0);
	satipclientstart = ostrcat("/etc/init.d/satipclient start", NULL, 0, 0);
	satipclientrestart = ostrcat("/etc/init.d/satipclient restart", NULL, 0, 0);
	satipclientscan = createpluginpath("/satipclient/files/scan.py", 0);
	satipclientrun = createpluginpath("/satipclient/files/run.sh", 0);
//	satipclientconfig = ostrcat("cp /mnt/network/vtuner.conf /etc", NULL, 0, 0);
//	vtunerconf = ostrcat("/mnt/network/vtuner.conf", NULL, 0, 0);
	satipclientconfig = ostrcat("cp /etc/vtuner.conf /etc", NULL, 0, 0);
	vtunerconf = ostrcat("/etc/vtuner.conf", NULL, 0, 0);
	vtunertmp = ostrcat("/tmp/vtuner.help", NULL, 0, 0);

	struct skin* satipclient = getscreen("satipclientsettings");
	struct skin* listbox = getscreennode(satipclient, "listbox");
	struct skin* tmp = NULL;
	struct skin* node = NULL;
	
	cmd = ostrcat(satipclientrun, " ", 0, 0);
	cmd = ostrcat(cmd, satipclientscan, 1, 0);
	debug(10, "cmd: %s", cmd);
	tmpstr = command(cmd);
	debug(10, "tmpstr: %s", tmpstr);
	free(cmd), cmd = NULL;
	free(tmpstr), tmpstr = NULL;

	addscreenrc(satipclient, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	struct clist *myconfig[LISTHASHSIZE] = {NULL};
	readconfig(vtunerconf, myconfig);

	cmd = ostrcat("cat ", vtunerconf, 0, 0);
	cmd = ostrcat(cmd, " | cut -d'=' -f2", 1, 0);
	tmpstr1 = string_newline(command(cmd));
	
	free(cmd), cmd = NULL;
	cmd = ostrcat("cat ", vtunertmp, 0, 0);
	cmd = ostrcat(cmd, " | cut -d'=' -f2", 1, 0);
	
	struct splitstr* ret1 = NULL;

	ret1 = strsplit(tmpstr1, "\n", &count1);
	if(ret1 != NULL && count1 > 0)
	{
		for(i = 0; i < count1; i++)
		{
			count2 = 0;
			tmpstr2 = string_newline(command(cmd));
			struct splitstr* ret2 = NULL;

			node = addlistbox(satipclient, listbox, node, 1);
			if(node != NULL)
			{
				node->type = CHOICEBOX;
				text = ostrcat(_("VTUNER"), " (", 0, 0);
				text = ostrcat(text, oitoa(i), 1, 1);
				text = ostrcat(text, ")", 1, 0);
				name = ostrcat(oitoa(i), NULL, 1, 0);
				changetext(node, text);
				changename(node, name);

				ret2 = strsplit(tmpstr2, "\n", &count2);
				if(ret2 != NULL && count2 > 0)
				{
					for(j = 0; j < count2; j++)
					{
						desc = oregex(".*desc:(.*),ipaddr:.*", ret2[j].part);
						if(desc != NULL)
						{
							showname = ostrcat(showname, desc, 1, 0);
							showname = ostrcat(showname, " (", 1, 0);
							ip = oregex(".*,ipaddr:(.*),tuner_type:.*", ret2[j].part);
							showname = ostrcat(showname, ip, 1, 0);
							showname = ostrcat(showname, ") ", 1, 0);
							type = oregex(".*,tuner_type:(.*),uuid:.*", ret2[j].part);
							showname = ostrcat(showname, type, 1, 0);
						}
						else
						{
							showname = ostrcat(showname, _("is not used"), 1, 0);
						}

						addchoicebox(node, ret2[j].part, showname);
						printf("addchoicebox: showname=%s entry=%s\n", showname, ret2[j].part);

						free(type), type = NULL;
						free(ip), ip = NULL;
						free(desc), desc = NULL;
						free(showname), showname = NULL;
					}
				}
				free(ret2), ret2 = NULL;
				setchoiceboxselection(node, getlist(myconfig, node->name, NULL));
				free(text), text = NULL;
				free(name), name = NULL;
			}
		}
	}
	free(ret1), ret1 = NULL;
	free(cmd), cmd = NULL;

	drawscreen(satipclient, 0, 0);
	tmp = listbox->select;

	while(1)
	{
		addscreenrc(satipclient, tmp);
		rcret = waitrc(satipclient, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
			addlist(myconfig, listbox->select->name, listbox->select->ret);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			writelist(myconfig, vtunerconf);

			if(textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0) == 1)
				oshutdown(3, 1);
			else
			{
				debug(10, "cmd: %s", satipclientconfig);
				system(satipclientconfig);

				debug(10, "cmd: %s", satipclientrestart);
				ret = system(satipclientrestart);
				if(ret == 0)
					textbox(_("Message"), _("SAT-IP Client restarted and config saved"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
				else
					textbox(_("Message"), _("SAT-IP Client not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			}
			drawscreen(satipclient, 0, 0);			

			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			debug(10, "cmd: %s", satipclientstop);
			system(satipclientstop);
			textbox(_("Message"), _("SAT-IP Client now stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			drawscreen(satipclient, 0, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			writelist(myconfig, vtunerconf);

			debug(10, "cmd: %s", satipclientconfig);
			system(satipclientconfig);

			debug(10, "cmd: %s", satipclientstart);
			ret = system(satipclientstart);

			if(ret == 0)
				textbox(_("Message"), _("SAT-IP Client started."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("SAT-IP Client not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(satipclient, 0, 0);
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			cmd = ostrcat(satipclientrun, " ", 0, 0);
			cmd = ostrcat(cmd, satipclientscan, 1, 0);
			debug(10, "cmd: %s", cmd);
			
			tmpstr = command(cmd);
			debug(10, "tmpstr: %s", tmpstr);
			free(cmd), cmd = NULL;
			

			textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 900, 500, 0, 0);
			free(tmpstr), tmpstr = NULL;
			drawscreen(satipclient, 0, 0);

			restart = 1;
			break;
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			screentunerconfig();
			writeallconfig(1);
			drawscreen(satipclient, 0, 0);
		}
	}

	free(vtunerconf), vtunerconf = NULL;
	free(satipclientstart), satipclientstart = NULL;
	free(satipclientrestart), satipclientrestart = NULL;
	free(satipclientstop), satipclientstop = NULL;
	free(satipclientscan), satipclientscan = NULL;
	free(satipclientconfig), satipclientconfig = NULL;

	delownconfigtmpall();
	delmarkedscreennodes(satipclient, 1);
	delownerrc(satipclient);
	clearscreen(satipclient);

	if(restart == 1) goto start;
}

#endif
