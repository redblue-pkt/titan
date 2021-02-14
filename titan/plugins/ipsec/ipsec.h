#ifndef IPSEC_H
#define IPSEC_H

void screenipsec()
{
	int rcret = 0;
	char* tmpstr = NULL;
	char* ret = NULL;
	char* cmd = NULL;
	char* user = NULL;
	char* pass = NULL;
	char* dyndns = NULL;
	char* rekey = NULL;
	char* psk = NULL;
	char* name = NULL;
	char* id = NULL;
	char* ipsecstop = NULL;
	char* ipsecstart = NULL;

	struct skin* ipsec = getscreen("ipsecsettings");
	struct skin* listbox = getscreennode(ipsec, "listbox");	
	struct skin* skin_name = getscreennode(ipsec, "name");
	struct skin* skin_user = getscreennode(ipsec, "user");
	struct skin* skin_pass = getscreennode(ipsec, "pass");
	struct skin* skin_psk = getscreennode(ipsec, "psk");
	struct skin* skin_dyndns = getscreennode(ipsec, "dyndns");
	struct skin* skin_id = getscreennode(ipsec, "id");
	struct skin* skin_rekey = getscreennode(ipsec, "rekey");
	struct skin* tmp = NULL;

	struct skin* node = NULL;

	ipsecstop = ostrcat("ipsec.sh stop", NULL, 0, 0);
	ipsecstart = ostrcat("ipsec.sh start", NULL, 0, 0);

	if(!file_exist("/mnt/network/ipsec.conf"))
		system("cp /etc/titan.restore/mnt/network/ipsec.conf /mnt/network/ipsec.conf");

	if(!file_exist("/mnt/network/ipsec.secret"))
		system("cp /etc/titan.restore/mnt/network/ipsec.secret /mnt/network/ipsec.secret");

	name = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | grep -v %default | sed -nr 's/.*conn ([^ ]+).*/\\1/p'"));
	user = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*xauth_identity=([^=]+).*/\\1/p'"));
	pass = string_newline(command("cat /mnt/network/ipsec.secrets | grep -v '^#' | sed -nr 's/.*: XAUTH \"([^\"]+)\".*/\\1/p'"));
	dyndns = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*right=([^=]+).*/\\1/p'"));
	rekey = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*rekey=([^=]+).*/\\1/p'"));
	psk = string_newline(command("cat /mnt/network/ipsec.secrets | grep -v '^#' | sed -nr 's/.*%any : PSK \"([^\"]+)\".*/\\1/p'"));
	id = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*leftid=keyid:([^:]+).*/\\1/p'"));

	printf("name: %s\n", name);
	printf("user: %s\n", user);
	printf("pass: %s\n", pass);
	printf("dyndns: %s\n", dyndns);
	printf("rekey: %s\n", rekey);
	printf("psk: %s\n", psk);
	printf("id: %s\n", id);

	changemask(skin_name, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_name, name);

	changemask(skin_user, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_user, user);

	changemask(skin_pass, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_pass, pass);

	changemask(skin_dyndns, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_dyndns, dyndns);

	changemask(skin_psk, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_psk, psk);

	changemask(skin_id, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_id, id);

	addchoicebox(skin_rekey, "no", _("no"));
	addchoicebox(skin_rekey, "yes", _("yes"));
	setchoiceboxselection(skin_rekey, rekey);

	drawscreen(ipsec, 0, 0);
	addscreenrc(ipsec, listbox);

	node = addlistbox(ipsec, listbox, node, 1);
	if(node != NULL)
	{
		node->type = CHOICEBOX;
		changetext(node, _("autostart")); changename(node, "ipsec");
		addchoicebox(node, "n", _("no")); addchoicebox(node, "y", _("yes"));
		setchoiceboxselection(node, getownconfig(node->name));
	}
	
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(ipsec, tmp);
		rcret = waitrc(ipsec, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			addownconfigscreentmp(listbox->select->name, listbox->select);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(skin_name->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*conn $%.*/conn ", skin_name->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_user->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*xauth_identity=.*/xauth_identity=", skin_user->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_id->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*leftid=keyid:.*/leftid=keyid:", skin_id->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_dyndns->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*right=.*/right=", skin_dyndns->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_rekey->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*rekey=.*/rekey=", skin_rekey->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_pass->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.secrets | sed 's/.*: XAUTH \".*/: XAUTH \"", skin_pass->ret, 0, 0);
				cmd = ostrcat(cmd,"\"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_psk->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.secrets | sed 's/%any : PSK \".*/%any : PSK \"", skin_psk->ret, 0, 0);
				cmd = ostrcat(cmd,"\"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_user->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.secrets | sed 's/.* : XAUTH/", skin_user->ret, 0, 0);
				cmd = ostrcat(cmd," : XAUTH/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			debug(99, "name: %s", skin_name->ret);
			debug(99, "user: %s", skin_user->ret);
			debug(99, "pass: %s", skin_pass->ret);
			debug(99, "dyndns: %s", skin_dyndns->ret);
			debug(99, "psk: %s", skin_psk->ret);
			debug(99, "id: %s", skin_id->ret);
			debug(99, "rekey: %s", skin_rekey->ret);

			debug(10, "cmd: %s", ipsecstop);
			system(ipsecstop);
			debug(10, "cmd: %s", ipsecstart);
			ret = system(ipsecstart);
			if(ret == 0)
				textbox(_("Message"), _("Ipsec VPN Client started and config saved"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("Ipsec VPN Client not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(ipsec, 0, 0);			
			writeownconfigtmp();
			writeallconfig(1);

			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			debug(10, "cmd: %s", ipsecstop);
			system(ipsecstop);
			textbox(_("Message"), _("Ipsec VPN Client now stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			drawscreen(ipsec, 0, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			debug(10, "cmd: %s", ipsecstop);
			system(ipsecstop);
			debug(10, "cmd: %s", ipsecstart);
			ret = system(ipsecstart);
			if(ret == 0)
				textbox(_("Message"), _("Ipsec VPN Client started."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			else
				textbox(_("Message"), _("Ipsec VPN Client not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(ipsec, 0, 0);
		}
	}

	delownconfigtmpall();
	delmarkedscreennodes(ipsec, 1);
	delownerrc(ipsec);
	clearscreen(ipsec);
}

#endif
