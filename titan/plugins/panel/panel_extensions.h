#ifndef PANEL_EXTENSIONS_H
#define PANEL_EXTENSIONS_H


void screenpanel_extensions(int mode)
{
	char* section = NULL;
	char* file = NULL;
	char* tmpstr = NULL;
	if (mode == 0)
	{
		ipkg_update();
		ipkg_list();
		tmpstr = get_ipk_section();
		free(section); section = NULL;
		section = ipk_listbox(NULL, tmpstr, NULL, "Ipk Install - select section", "%pluginpath%/panel/skin/", 1);
		if(tmpstr == NULL){
			textbox(_("Ipk Install Info"), _("No section found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		free(tmpstr); tmpstr = NULL;

		debug(60, "section: %s", section);
		if(section != NULL)
		{
			free(file); file = NULL;
			file = ipk_listbox(NULL, section, "ipkinstall", "Ipk Install - select file", "/tmp/preview/", 2);
			free(tmpstr); tmpstr = NULL;
			debug(60, "file: %s", file);
			if(file != NULL)
			{
				tmpstr = ostrcat(section, "-", 0, 0);
				tmpstr = ostrcat(tmpstr, file, 1, 0);

				char* tmpinfo = NULL;
				tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
				tmpinfo = ostrcat(tmpinfo, tmpstr, 1, 0);
				tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

				char* installname = NULL;
				struct splitstr* ret1 = NULL;
				int count = 0;
				int i = 0;
				ret1 = strsplit(tmpstr, " ", &count);
				installname = ostrcat("titan-plugin-", tmpstr, 0, 0);
				installname = ostrcat(installname, (&ret1[0])->part, 1, 0);
		
				if(textbox(_("Ipk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
				{
					if(ipkg_install(installname) == 0)
						textbox(_("Ipk Install Info"), _("Install OK"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					else
						textbox(_("Ipk Install Info"), _("Install ERROR"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
					textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
				free(ret1); ret1 = NULL;
				free(installname); installname = NULL;
				free(tmpinfo); tmpinfo = NULL;
			}
			free(section); section = NULL;
			free(file); file = NULL;
			free(tmpstr); tmpstr = NULL;
			freeipkg();
			screenpanel_extensions(0);
		}
		else
			freeipkg();
		
	}
	else if (mode == 1)
	{
		tmpstr = get_ipk_listinstall();
		free(file); file = NULL;
		file = ipk_listbox(NULL, tmpstr, NULL, "Ipk Remove - select file", "%pluginpath%/panel/skin/", 1);
		free(tmpstr); tmpstr = NULL;
		debug(60, "file: %s", file);
		if(file != NULL)
		{
			char* tmpinfo = NULL;
			tmpinfo = ostrcat(tmpinfo, "Remove ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, file, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			char* deinstallname = NULL;
			struct splitstr* ret1 = NULL;
			int count = 0;
			int i = 0;
			ret1 = strsplit(tmpstr, " ", &count);
			deintallname = ostrcat("titan-plugin-", file, 0, 0);
			deinstallname = ostrcat(deinstallname, (&ret1[0])->part, 1, 0);

			if(ipk_remove(deinstallname,1) == 0)
				textbox(_("Ipk Remove Info"), _("Remove OK"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
			else
				textbox(_("Ipk Remove Info"), _("Remove ERROR"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);

			free(ret1); ret1 = NULL;
			free(installname); installname = NULL;
			free(tmpinfo); tmpinfo = NULL;
			free(file); file = NULL;
			screenpanel_extensions(1);
		}
		free(file); file = NULL;
	}
	else if (mode == 2)
	{
		tmpstr = get_ipk_tmplistinstall();
		free(file); file = NULL;
		file = ipk_listbox(NULL, tmpstr, NULL, "Ipk Tmp Install - select file", "%pluginpath%/panel/skin/", 1);
		if(tmpstr == NULL){
			textbox(_("Ipk Tmp Info"), _("No IPK found in /tmp"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		free(tmpstr); tmpstr = NULL;
		debug(60, "file: %s", file);
		if(file != NULL)
		{
			char* tmpinfo = NULL;
			tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, file, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(get_ipk_tmpinstall(file) == 0)
				textbox(_("Ipk Tmp Info"), _("Install OK"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
			else
				textbox(_("Ipk Tmp Info"), _("Install ERROR"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);

			textbox(_("Ipk Tmp Info"), _(get_ipk_tmpinstall(file)), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);

			free(tmpinfo); tmpinfo = NULL;
			free(file); file = NULL;
			screenpanel_extensions(2);
		}
		free(file); file = NULL;
	}
}

#endif
