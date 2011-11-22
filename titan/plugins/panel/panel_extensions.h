#ifndef PANEL_EXTENSIONS_H
#define PANEL_EXTENSIONS_H


void screenpanel_extensions(int mode)
{
	char* section = NULL;
	char* file = NULL;
	char* tmpstr = NULL;

	if (mode == 0)
	{
		free(get_ipk_update());
		tmpstr = get_ipk_section();
		free(section); section = NULL;
//		char* section = ostrcat(section, ipklistbox(NULL, tmpstr, NULL, "Ipk Install - select section", "%pluginpath%/panel/skin/", 1), 1, 1);
		section = ipklistbox(NULL, tmpstr, NULL, "Ipk Install - select section", "%pluginpath%/panel/skin/", 1);
		if(tmpstr == NULL){
			textbox(_("Ipk Install Info"), _("No section found"), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		free(tmpstr); tmpstr = NULL;

		debug(60, "section: %s", section);
		if(section != NULL)
		{
			tmpstr = get_ipk_list(section);
			free(file); file = NULL;
//			file = ostrcat(file, ipklistbox(NULL, tmpstr, "ipkinstall", "Ipk Install - select file", "/tmp/preview/", 2), 1, 1);
			file = ipklistbox(NULL, tmpstr, "ipkinstall", "Ipk Install - select file", "/tmp/preview/", 2);
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

				if(textbox(_("Ipk Install Info"), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
					textbox(_("Ipk Install Info"), _(get_ipk_install(tmpstr)), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
				free(tmpinfo); tmpinfo = NULL;
			}
			free(section); section = NULL;
			free(file); file = NULL;
			free(tmpstr); tmpstr = NULL;
			screenpanel_extensions(0);
		}
	}
	else if (mode == 1)
	{
		tmpstr = get_ipk_listinstall();
		free(file); file = NULL;
//		file = ostrcat(file, ipklistbox(NULL, tmpstr, NULL, "Ipk Remove - select file", "%pluginpath%/panel/skin/", 1), 1, 1);
		file = ipklistbox(NULL, tmpstr, NULL, "Ipk Remove - select file", "%pluginpath%/panel/skin/", 1);
		free(tmpstr); tmpstr = NULL;
		debug(60, "file: %s", file);
		if(file != NULL)
		{
			char* tmpinfo = NULL;
			tmpinfo = ostrcat(tmpinfo, "Remove ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, file, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Ipk Remove Info"), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
				textbox(_("Ipk Remove Info"), _(get_ipk_remove(file)), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);

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
		file = ipklistbox(NULL, tmpstr, NULL, "Ipk Tmp Install - select file", "%pluginpath%/panel/skin/", 0);
		if(tmpstr == NULL){
			textbox(_("Ipk Tmp Info"), _("No IPK found in /tmp"), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		free(tmpstr); tmpstr = NULL;
		debug(60, "file: %s", file);
		if(file != NULL)
		{
			char* tmpinfo = NULL;
			tmpinfo = ostrcat(tmpinfo, "Install ", 1, 0);
			tmpinfo = ostrcat(tmpinfo, file, 1, 0);
			tmpinfo = ostrcat(tmpinfo, " ?", 1, 0);

			if(textbox(_("Ipk Tmp Info"), _(tmpinfo), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
				textbox(_("Ipk Tmp Info"), _(get_ipk_tmpinstall(file)), "EXIT", getrcconfigint("rcexit", NULL), "OK", getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);

			free(tmpinfo); tmpinfo = NULL;
			free(file); file = NULL;
			screenpanel_extensions(2);
		}
		free(file); file = NULL;
	}
}

#endif
