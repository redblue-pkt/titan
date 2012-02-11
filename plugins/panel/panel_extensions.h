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

		section = ipk_listbox(NULL, tmpstr, NULL, "Ipk Install - select section", "%pluginpath%/panel/skin/", 1);
		if(tmpstr == NULL)
			textbox(_("Ipk Install Info"), _("No section found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

		free(tmpstr); tmpstr = NULL;

		debug(130, "section: %s", section);
		if(section != NULL)
		{
			file = ipk_listbox(NULL, section, "ipkinstall", "Ipk Install - select file", "/tmp/preview/", 2);
			debug(130, "file: %s", file);
			
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
				
				if(ret1 != NULL)
				{
					installname = ostrcat("titan-plugin-", (&ret1[0])->part, 0, 0);
		
					if(textbox(_("Ipk Install Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
					{
//						char* ipkg_ret = get_ipk_install(installname);
//						textbox(_("Ipk Install Info"), _(ipkg_ret), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
//						free(ipkg_ret); ipkg_ret = NULL;
						char* log = NULL;
						if(ipkg_install(installname) == 0)
						{
							log = readfiletomem("/tmp/ipkg.log", 0);
							textbox(_("Ipk Install Info - Install OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
						}
						else
						{
							log = readfiletomem("/tmp/ipkg.log", 0);
							textbox(_("Ipk Install Info - Install ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
						}
						textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
						free(log), log = NULL;
						unlink("/tmp/ipkg.log");
					}
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
		
		freeipkg();
		
	}
	else if (mode == 1)
	{
		tmpstr = get_ipk_listinstall();
		file = ipk_listbox(NULL, tmpstr, NULL, "Ipk Remove - select file", "%pluginpath%/panel/skin/", 1);
		free(tmpstr); tmpstr = NULL;
		
		debug(130, "file: %s", file);
		if(file != NULL)
		{
			char* tmpinfo = NULL;
			tmpinfo = ostrcat(tmpinfo, "Remove ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, file, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			char* deinstallname = NULL;
			deinstallname = ostrcat("titan-plugin-", file, 0, 0);

			if(textbox(_("Remove Info"), _(tmpinfo), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
			{
//				char* ipkg_ret = get_ipk_remove(deinstallname);
//				textbox(_("Ipk Remove Info"), _(ipkg_ret), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
//				free(ipkg_ret); ipkg_ret = NULL; 

				char* log = NULL;
				if(ipkg_remove(deinstallname,1) == 0)
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					textbox(_("Ipk Remove Info - Remove OK"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
				else
				{
					log = readfiletomem("/tmp/ipkg.log", 0);
					textbox(_("Ipk Remove Info - Remove ERROR"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				}
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				free(log), log = NULL;
				unlink("/tmp/ipkg.log");
			}

			free(deinstallname); deinstallname = NULL;
			free(tmpinfo); tmpinfo = NULL;
			free(file); file = NULL;
			screenpanel_extensions(1);
		}
		free(file); file = NULL;
	}
	else if (mode == 2)
	{
		tmpstr = get_ipk_tmplistinstall();
		file = ipk_listbox(NULL, tmpstr, NULL, "Ipk Tmp Install - select file", "%pluginpath%/panel/skin/", 1);
		
		if(tmpstr == NULL)
			textbox(_("Ipk Tmp Info"), _("No IPK found in /tmp"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		free(tmpstr); tmpstr = NULL;
		
		debug(130, "file: %s", file);
		if(file != NULL)
		{
			char* tmpinfo = NULL;
			tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, file, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Ipk Tmp Info"), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
			{
				char* ipkg_ret = get_ipk_tmpinstall(file);
				textbox(_("Ipk Tmp Info"), _(ipkg_ret), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
				free(ipkg_ret); ipkg_ret = NULL;
				textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), "EXIT", getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			}
//			if(get_ipk_tmpinstall(file) == 0)
//				textbox(_("Ipk Tmp Info"), _("Install OK"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
//			else
//				textbox(_("Ipk Tmp Info"), _("Install ERROR"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
//			textbox(_("Message"), _("Some plugins needs restart.\nIf the plugin is not active\nreboot the box."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);

			free(tmpinfo); tmpinfo = NULL;
			free(file); file = NULL;
			screenpanel_extensions(2);
		}
		free(file); file = NULL;
	}
}

#endif
