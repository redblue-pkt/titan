#ifndef PANEL_EXTENSIONS_H
#define PANEL_EXTENSIONS_H

extern struct hdd* hdd;

//flag 0: from start
//flag 1: from menu
void screenpanel_extensions_check(int flag)
{
	int i = 0, treffer = 0;
	struct hdd *node = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	if(status.security == 1)
	{
		addhddall();
		node = hdd;

		while(node != NULL)
		{
			tmpstr = ostrcat("/autofs/", node->device, 0, 0);
			tmpstr1 = get_ipk_tmplistinstall(tmpstr);
			free(tmpstr1); tmpstr1 = NULL;

			if(tmpstr1 != NULL)
			{
				treffer = 1;
				screenpanel_extensions(2, tmpstr);
			}

			free(tmpstr); tmpstr = NULL;
			node = node->next;
		}

		if(flag == 1 && treffer == 0)
			textbox(_("Ipk Install Info"), _("No IPK fount on media !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
}

void screenpanel_extensions(int mode, char* path)
{
	char* tmpstr = NULL, *tmpinfo = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	struct menulist* mlist1 = NULL, *mbox1 = NULL;
	
	if(mode == 0)
	{
		ipkg_update();
		ipkg_list();

		mbox = ipkmenulist(mlist, NULL, "Ipk Install - select section", "%pluginpath%/panel/skin", NULL, 1, 0);

		if(mbox != NULL)
		{
			debug(130, "section: %s", mbox->name);
			mbox1 = ipkmenulist(mlist1, "ipkinstall", "Ipk Install - select file", "/tmp/preview", mbox->name, 2, 1);
			
			if(mbox1 != NULL)
			{
				debug(130, "file: %s", mbox1->name);
				tmpstr = ostrcat("titan-plugin-", mbox->name, 0, 0);
				tmpstr = ostrcat(tmpstr, "-", 1, 0);
				tmpstr = ostrcat(tmpstr, mbox1->param, 1, 0);

				tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
				tmpinfo = ostrcat(tmpinfo, "-", 1, 0);
				tmpinfo = ostrcat(tmpinfo, mbox1->name, 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

				if(textbox(_("Ipk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
				{
					char* log = NULL;
					if(ipkg_install(tmpstr) == 0)
					{
						log = readfiletomem("/tmp/ipkg.log", 0);
						if(log == NULL) log = ostrcat("Install success", NULL, 0, 0);
						textbox(_("Ipk Install Info - Install OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					}
					else
					{
						log = readfiletomem("/tmp/ipkg.log", 0);
						if(log == NULL) log = ostrcat("Install error", NULL, 0, 0);
						textbox(_("Ipk Install Info - Install ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					}
					textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
					loadplugin();
					free(log), log = NULL;
					unlink("/tmp/ipkg.log");
				}
			}
		}
			
		freemenulist(mlist1, 1); mlist1 = NULL;
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenpanel_extensions(0);
	}
	else if(mode == 1)
	{
		ipkg_list_installed();
		mbox = ipkmenulist(mlist, NULL, "Ipk Remove - select file", "%pluginpath%/panel/skin", NULL, 1, 2);
		
		if(mbox != NULL)
		{
			debug(130, "file: %s", mbox->name);

			tmpstr = ostrcat("titan-plugin-", mbox->name, 0, 0);

			tmpinfo = ostrcat(tmpinfo, "Remove ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Remove Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
			{
				char* log = NULL;
				if(ipkg_remove(tmpstr, 1) == 0)
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					if(log == NULL) log = ostrcat("Remove success", NULL, 0, 0);
					textbox(_("Ipk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
				else
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					if(log == NULL) log = ostrcat("Remove error", NULL, 0, 0);
					textbox(_("Ipk Remove Info - Remove ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				free(log), log = NULL;
				unlink("/tmp/ipkg.log");
			}
		}
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		freeipkg();
		if(mbox != NULL) screenpanel_extensions(1);
	}
	else if(mode == 2)
	{
		if(path == NULL)
			tmpstr = get_ipk_tmplistinstall("/tmp");
		else
			tmpstr = get_ipk_tmplistinstall(path);
    
    if(tmpstr == NULL || strlen(tmpstr) == 0)
    {
      textbox(_("Message"), _("No plugin found."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
    }
    else
    {
		  addmenulistall(&mlist, tmpstr, NULL, 0, NULL);
		  mbox = menulistbox(mlist, NULL, "Ipk Tmp Install - select file", "%pluginpath%/panel/skin", "/skin/plugin.png", 1, 0);
    }
		
		free(tmpstr); tmpstr = NULL;
		
		if(mbox != NULL)
		{
			debug(130, "file: %s", mbox->name);

			tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, mbox->name, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Ipk Tmp Info"), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
			{
				char* log = NULL;

				if(path == NULL)
					log = get_ipk_tmpinstall("/tmp", mbox->name);
				else
					log = get_ipk_tmpinstall(path, mbox->name);

				if(log == NULL) log = ostrcat("No output found !", NULL, 0, 0);
				textbox(_("Ipk Tmp Info"), log, "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
				free(log); log = NULL;
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
				loadplugin();
			}
		}
		freemenulist(mlist, 1); mlist = NULL;
		free(tmpstr); tmpstr = NULL;
		free(tmpinfo); tmpinfo = NULL;
		if(mbox != NULL) screenpanel_extensions(2);
	}
}

#endif
