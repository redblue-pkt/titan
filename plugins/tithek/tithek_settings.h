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
	struct skin* amazon_user = getscreennode(tithek_settings, "amazon_user");
	struct skin* amazon_pass = getscreennode(tithek_settings, "amazon_pass");
	struct skin* vk_user = getscreennode(tithek_settings, "vk_user");
	struct skin* vk_pass = getscreennode(tithek_settings, "vk_pass");
	struct skin* update = getscreennode(tithek_settings, "update");
	struct skin* b3 = getscreennode(tithek_settings, "b3");
	struct skin* b4 = getscreennode(tithek_settings, "b4");

	addchoicebox(cover, "0", _("show auto entrys"));
	addchoicebox(cover, "1", _("show 2 entrys"));
	addchoicebox(cover, "2", _("show 6 entrys"));
	addchoicebox(cover, "3", _("show 12 entrys"));	
	addchoicebox(cover, "4", _("show 20 entrys"));
	addchoicebox(cover, "5", _("show 30 entrys"));
	addchoicebox(cover, "6", _("show in list"));
	setchoiceboxselection(cover, getconfig("tithek_cover", NULL));

	addchoicebox(view, "0", _("show auto entrys"));
	addchoicebox(view, "1", _("show 2 entrys"));
	addchoicebox(view, "2", _("show 6 entrys"));
	addchoicebox(view, "3", _("show 12 entrys"));	
	addchoicebox(view, "4", _("show 20 entrys"));
	addchoicebox(view, "5", _("show 30 entrys"));
	addchoicebox(view, "6", _("show in list"));
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

	changemask(amazon_user, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("amazon_pass", NULL) == NULL)
		changeinput(amazon_user, getconfig("amazon_user", NULL));
	else
		changeinput(amazon_user, "****");

	changemask(amazon_pass, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("amazon_pass", NULL) == NULL)
		changeinput(amazon_pass, getconfig("amazon_pass", NULL));
	else
		changeinput(amazon_pass, "****");

	changemask(vk_user, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("vk_pass", NULL) == NULL)
		changeinput(vk_user, getconfig("vk_user", NULL));
	else
		changeinput(vk_user, "****");

	changemask(amazon_pass, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("vk_pass", NULL) == NULL)
		changeinput(vk_pass, getconfig("vk_pass", NULL));
	else
		changeinput(vk_pass, "****");

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
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("tithek_view", view, NULL);
			addconfigscreencheck("tithek_cover", cover, NULL);
			addconfigscreencheck("tithek_pay", pay, NULL);
			addconfigscreencheck("tithek_pic_ratio", picratio, NULL);
			addconfigscreencheck("tithek_hid_xxx", hidxxx, NULL);
			addconfigscreencheck("tithek_autoupdate", autoupdate, NULL);
			if(amazon_user->ret != NULL && ostrcmp(amazon_user->ret, "****") != 0)
			{
				debug(99, "amazon_user: write");
				debug(99, "amazon_user: %s", amazon_user->ret);
				addconfigscreen("amazon_user", amazon_user);
			}
			else
			{
				debug(99, "amazon_user: skipped");
			}
			if(amazon_pass->ret != NULL && ostrcmp(amazon_pass->ret, "****") != 0)
			{
				debug(99, "amazon_pass: write");
				debug(99, "amazon_pass: %s", amazon_pass->ret);
				addconfigscreen("amazon_pass", amazon_pass);
			}
			else
			{
				debug(99, "amazon_pass: skipped");
			}
			if(vk_user->ret != NULL && ostrcmp(vk_user->ret, "****") != 0)
			{
				debug(99, "vk_user: write");
				debug(99, "vk_user: %s", vk_user->ret);
				addconfigscreen("vk_user", vk_user);
			}
			else
			{
				debug(99, "vk_user: skipped");
			}
			if(vk_pass->ret != NULL && ostrcmp(vk_pass->ret, "****") != 0)
			{
				debug(99, "vk_pass: write");
				debug(99, "vk_pass: %s", vk_pass->ret);
				addconfigscreen("vk_pass", vk_pass);
			}
			else
			{
				debug(99, "vk_pass: skipped");
			}

			writeallconfig(1);

			debug(99, "amazon_user read: %s", getconfig("amazon_user", NULL));
			debug(99, "amazon_pass read: %s", getconfig("amazon_pass", NULL));

			debug(99, "vk_user read: %s", getconfig("vk_user", NULL));
			debug(99, "vk_pass read: %s", getconfig("vk_pass", NULL));
			
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

		if(file_exist("/mnt/network/cookies") && (ostrcmp(listbox->select->name, "amazon_user") == 0 || ostrcmp(listbox->select->name, "amazon_pass") == 0 || ostrcmp(listbox->select->name, "vk_user") == 0 || ostrcmp(listbox->select->name, "vk_pass") == 0))
			b4->hidden = NO;			
		else
			b4->hidden = YES;
	
		if(ostrcmp(listbox->select->name, "amazon_user") == 0 || ostrcmp(listbox->select->name, "amazon_pass") == 0 || ostrcmp(listbox->select->name, "vk_user") == 0 || ostrcmp(listbox->select->name, "vk_pass") == 0)
			b3->hidden = NO;
		else
			b3->hidden = YES;

		drawscreen(tithek_settings, 0, 0);
	}

	delownerrc(tithek_settings);
	clearscreen(tithek_settings);
}

#endif
