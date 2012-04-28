/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef SCRIPTEXEC_H
#define SCRIPTEXEC_H

void screenscriptexec()
{
	int dirrcret = 0;
	char* tmpstr = NULL, *startdir = NULL;
	char* file = NULL, *ret = NULL;


start:
	startdir = getconfig("scriptexecpath", NULL);

	tmpstr = ostrcat(file, "", 1, 0); file = NULL;
	file = screendir(startdir, "*.sh", basename(tmpstr), &dirrcret, NULL, _("BG-RUN"), getrcconfigint("rcred", NULL), NULL, 0, NULL, 0, NULL, 0, 90, 1, 90, 1, 2);
	free(tmpstr); tmpstr = NULL;

	if(file != NULL && (dirrcret == 0 || dirrcret == 1))
	{
		if(dirrcret == 0)
		{
			tmpstr = ostrcat(file, " 2>&1", 0, 0);
			ret = command(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(dirrcret == 1)
		{
			tmpstr = ostrcat(file, " &", 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		if(ret == NULL) ret = ostrcat(_("Script has no output or started in background."), NULL, 0, 0);
		textbox(_("Script execute"), ret, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
		free(ret); ret = NULL;
		goto start;
	}

	free(file);
}

#endif
