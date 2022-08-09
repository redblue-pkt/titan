#ifndef TITHEK_SETTINGS_H
#define TITHEK_SETTINGS_H

void screentithek_settings()
{
	int rcret = 0;
	struct skin* tmp = NULL;

	struct skin* tithek_settings = getscreen("tithek_settings");
	struct skin* listbox = getscreennode(tithek_settings, "listbox");
	struct skin* view = getscreennode(tithek_settings, "view");
	struct skin* cover = getscreennode(tithek_settings, "cover");
	struct skin* pay = getscreennode(tithek_settings, "pay");
	struct skin* picratio = getscreennode(tithek_settings, "picratio");
	struct skin* hidxxx = getscreennode(tithek_settings, "hidxxx");
//	struct skin* amazon_user = getscreennode(tithek_settings, "amazon_user");
//	struct skin* amazon_pass = getscreennode(tithek_settings, "amazon_pass");
	struct skin* vk_user = getscreennode(tithek_settings, "vk_user");
	struct skin* vk_pass = getscreennode(tithek_settings, "vk_pass");
	struct skin* kinox_url = getscreennode(tithek_settings, "kinox_url");
	struct skin* kinox_pic = getscreennode(tithek_settings, "kinox_pic");
	struct skin* kinox_localhoster = getscreennode(tithek_settings, "kinox_localhoster");
	struct skin* useproxy = getscreennode(tithek_settings, "useproxy");
	struct skin* proxy = getscreennode(tithek_settings, "proxy");
	struct skin* delservice = getscreennode(tithek_settings, "delservice");
	struct skin* iptvurl = getscreennode(tithek_settings, "iptvurl");
	struct skin* servicebouquets_autoupdate = getscreennode(tithek_settings, "servicebouquets_autoupdate");
	struct skin* servicebouquets_autoupdate_msg = getscreennode(tithek_settings, "servicebouquets_autoupdate_msg");

	struct skin* autoupdate = getscreennode(tithek_settings, "autoupdate");
	struct skin* b3 = getscreennode(tithek_settings, "b3");
	struct skin* b4 = getscreennode(tithek_settings, "b4");

	addchoicebox(cover, "0", _("show auto entrys"));
	addchoicebox(cover, "1", _("show 2 entrys"));
	addchoicebox(cover, "2", _("show 6 entrys"));
	addchoicebox(cover, "3", _("show 12 entrys"));	
	addchoicebox(cover, "4", _("show 20 entrys"));
	addchoicebox(cover, "5", _("show 30 entrys"));
	addchoicebox(cover, "6", _("show in list"));
//	addchoicebox(cover, "7", _("show description"));

	setchoiceboxselection(cover, getconfig("tithek_cover", NULL));

	addchoicebox(view, "0", _("show auto entrys"));
	addchoicebox(view, "1", _("show 2 entrys"));
	addchoicebox(view, "2", _("show 6 entrys"));
	addchoicebox(view, "3", _("show 12 entrys"));	
	addchoicebox(view, "4", _("show 20 entrys"));
	addchoicebox(view, "5", _("show 30 entrys"));
	addchoicebox(view, "6", _("show in list"));
//	addchoicebox(view, "7", _("show description"));
	setchoiceboxselection(view, getconfig("tithek_view", NULL));

	addchoicebox(picratio, "0", _("no"));
	addchoicebox(picratio, "1", _("yes"));	
	setchoiceboxselection(picratio, getconfig("tithek_pic_ratio", NULL));

	addchoicebox(pay, "0", _("no"));
	addchoicebox(pay, "1", _("yes"));	
	setchoiceboxselection(pay, getconfig("tithek_pay", NULL));

	addchoicebox(hidxxx, "0", _("no"));
	addchoicebox(hidxxx, "1", _("yes"));	
	setchoiceboxselection(hidxxx, getconfig("tithek_hid_xxx", NULL));

	addchoicebox(autoupdate, "0", _("no"));
	addchoicebox(autoupdate, "1", _("yes"));	
	setchoiceboxselection(autoupdate, getconfig("tithek_autoupdate", NULL));
/*
	changemask(amazon_user, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("tithek_amazon_pass", NULL) == NULL)
		changeinput(amazon_user, getconfig("tithek_amazon_user", NULL));
	else
		changeinput(amazon_user, "****");

	changemask(amazon_pass, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("tithek_amazon_pass", NULL) == NULL)
		changeinput(amazon_pass, getconfig("tithek_amazon_pass", NULL));
	else
		changeinput(amazon_pass, "****");
*/
	changemask(vk_user, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("tithek_vk_pass", NULL) == NULL)
		changeinput(vk_user, getconfig("tithek_vk_user", NULL));
	else
		changeinput(vk_user, "****");

	changemask(vk_pass, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("tithek_vk_pass", NULL) == NULL)
		changeinput(vk_pass, getconfig("tithek_vk_pass", NULL));
	else
		changeinput(vk_pass, "****");

	changemask(kinox_url, "abcdefghijklmnopqrstuvwxyz");
	changeinput(kinox_url, getconfig("tithek_kinox_url", NULL));

	addchoicebox(kinox_pic, "0", _("no"));
	addchoicebox(kinox_pic, "1", _("yes"));	
	setchoiceboxselection(kinox_pic, getconfig("tithek_kinox_pic", NULL));

	addchoicebox(kinox_localhoster, "0", _("http"));
	addchoicebox(kinox_localhoster, "1", _("https"));	
	addchoicebox(kinox_localhoster, "2", _("cloudfare"));	
	setchoiceboxselection(kinox_localhoster, getconfig("tithek_kinox_localhoster", NULL));

	changemask(proxy, "abcdefghijklmnopqrstuvwxyz0123456789://@");
	changeinput(proxy, getconfig("tithek_proxy", NULL));

	changemask(iptvurl, "abcdefghijklmnopqrstuvwxyz0123456789://@");
	changeinput(iptvurl, getconfig("tithek_iptvurl", NULL));

	addchoicebox(useproxy, "0", _("no"));
	addchoicebox(useproxy, "1", _("yes"));	
	setchoiceboxselection(useproxy, getconfig("tithek_useproxy", NULL));

	addchoicebox(delservice, "0", _("no"));
	addchoicebox(delservice, "1", _("yes"));	
	setchoiceboxselection(delservice, getconfig("tithek_delservice", NULL));

	addchoicebox(servicebouquets_autoupdate, "0", _("no"));
	addchoicebox(servicebouquets_autoupdate, "1", _("yes"));	
	setchoiceboxselection(servicebouquets_autoupdate, getconfig("tithek_servicebouquets_autoupdate", NULL));

	addchoicebox(servicebouquets_autoupdate_msg, "0", _("no"));
	addchoicebox(servicebouquets_autoupdate_msg, "1", _("yes"));	
	setchoiceboxselection(servicebouquets_autoupdate_msg, getconfig("tithek_servicebouquets_autoupdate_msg", NULL));

	if(!file_exist("/mnt/swapextensions/etc/.codecpack") && !file_exist("/var/swap/etc/.codecpack") && !file_exist("/var/etc/.codecpack"))
		kinox_url->hidden = YES;

	b3->hidden = YES;
	b4->hidden = YES;

	drawscreen(tithek_settings, 0, 0);
	addscreenrc(tithek_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tithek_settings, tmp);
		rcret = waitrc(tithek_settings, 0, 0);
		tmp = listbox->select;
/*
		if(useproxy->ret != NULL && ostrcmp(useproxy->ret, "0") == 0)
			proxy->hidden = YES;
		else
			proxy->hidden = NO;
*/		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("tithek_view", view, NULL);
			addconfigscreencheck("tithek_cover", cover, NULL);
			addconfigscreencheck("tithek_pay", pay, NULL);
			addconfigscreencheck("tithek_pic_ratio", picratio, NULL);
			addconfigscreencheck("tithek_hid_xxx", hidxxx, NULL);
			addconfigscreencheck("tithek_autoupdate", autoupdate, NULL);
			addconfigscreen("tithek_kinox_url", kinox_url);
			addconfigscreencheck("tithek_kinox_pic", kinox_pic, NULL);
			addconfigscreencheck("tithek_kinox_localhoster", kinox_localhoster, NULL);
			addconfigscreencheck("tithek_useproxy", useproxy, NULL);
			addconfigscreen("tithek_proxy", proxy);
			addconfigscreencheck("tithek_delservice", delservice, NULL);
			addconfigscreen("tithek_iptvurl", iptvurl);
			addconfigscreencheck("tithek_servicebouquets_autoupdate", servicebouquets_autoupdate, NULL);
			addconfigscreencheck("tithek_servicebouquets_autoupdate_msg", servicebouquets_autoupdate_msg, NULL);
/*
			if(amazon_user->ret != NULL && ostrcmp(amazon_user->ret, "****") != 0)
			{
				debug(99, "amazon_user: write");
				debug(99, "amazon_user: %s", amazon_user->ret);
				addconfigscreen("tithek_amazon_user", amazon_user);
			}
			else
			{
				debug(99, "amazon_user: skipped");
			}
			if(amazon_pass->ret != NULL && ostrcmp(amazon_pass->ret, "****") != 0)
			{
				debug(99, "amazon_pass: write");
				debug(99, "amazon_pass: %s", amazon_pass->ret);
				addconfigscreen("tithek_amazon_pass", amazon_pass);
			}
			else
			{
				debug(99, "amazon_pass: skipped");
			}
*/
			if(vk_user->ret != NULL && ostrcmp(vk_user->ret, "****") != 0)
			{
				debug(99, "vk_user: write");
				debug(99, "vk_user: %s", vk_user->ret);
				addconfigscreen("tithek_vk_user", vk_user);
			}
			else
			{
				debug(99, "vk_user: skipped");
			}
			if(vk_pass->ret != NULL && ostrcmp(vk_pass->ret, "****") != 0)
			{
				debug(99, "vk_pass: write");
				debug(99, "vk_pass: %s", vk_pass->ret);
				addconfigscreen("tithek_vk_pass", vk_pass);
			}
			else
			{
				debug(99, "vk_pass: skipped");
			}

			writeallconfig(1);

//			debug(99, "amazon_user read: %s", getconfig("amazon_user", NULL));
//			debug(99, "amazon_pass read: %s", getconfig("amazon_pass", NULL));

			debug(99, "vk_user read: %s", getconfig("tithek_vk_user", NULL));
			debug(99, "vk_pass read: %s", getconfig("tithek_vk_pass", NULL));
			
			break;
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			screenscreensaveradjust();
			drawscreen(tithek_settings, 0, 0);
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			unlink("/mnt/network/cookies");
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			char* tmpstr = NULL;
			char* cmd = NULL;
			if(kinox_localhoster->ret != NULL && ostrcmp(kinox_localhoster->ret, "0") == 0)
				tmpstr = gethttp("checkip.dyndns.org", "/", 80, NULL, NULL, 10000, NULL, 0);
			else if(kinox_localhoster->ret != NULL && ostrcmp(kinox_localhoster->ret, "1") == 0)
				tmpstr = gethttps("http://checkip.dyndns.org/", NULL, NULL, NULL, NULL, NULL, 1);
			else if(kinox_localhoster->ret != NULL && ostrcmp(kinox_localhoster->ret, "2") == 0)
			{
	// new start
				cmd = ostrcat("/tmp/localhoster/hoster.sh cloudflare 'http://checkip.dyndns.org/'", NULL, 0, 0);
				debug(99, "cmd: %s", cmd);

				tmpstr = command(cmd);
				free(cmd), cmd = NULL;
	// new end
			}
			cmd = string_resub("<body>", "</body>", tmpstr, 0);
			free(tmpstr), tmpstr = NULL;

			textbox(_("Message"), cmd, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
			free(cmd), cmd = NULL;
		}

		if(file_exist("/mnt/network/cookies") && (/*ostrcmp(listbox->select->name, "amazon_user") == 0 || ostrcmp(listbox->select->name, "amazon_pass") == 0 || */ostrcmp(listbox->select->name, "vk_user") == 0 || ostrcmp(listbox->select->name, "vk_pass") == 0))
			b4->hidden = NO;			
		else
			b4->hidden = YES;
	
		if(/*ostrcmp(listbox->select->name, "amazon_user") == 0 || ostrcmp(listbox->select->name, "amazon_pass") == 0 || */ostrcmp(listbox->select->name, "vk_user") == 0 || ostrcmp(listbox->select->name, "vk_pass") == 0)
			b3->hidden = NO;
		else
			b3->hidden = YES;

		drawscreen(tithek_settings, 0, 0);
	}

	delownerrc(tithek_settings);
	clearscreen(tithek_settings);
}

#endif
