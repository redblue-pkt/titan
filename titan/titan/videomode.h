#ifndef VIDEOMODE_H
#define VIDEOMODE_H

void screenvideomode()
{
	char* skinname = "videomode";
	char* tmpstr = NULL;
	char* tmpstr1 = NULL;

	tmpstr = getpolicy();

	tmpstr1 = ostrcat(tmpstr1, "Subchannel\n", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "Resolution Settings\n", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "Aspect Settings\n", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "3D Mode\n", 1, 0);	
	tmpstr1 = ostrcat(tmpstr1, "\t\n", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, getpolicychoices(), 1, 1);

	char* mbox = menulistbox(tmpstr, tmpstr1, skinname, NULL, NULL, 1, 1);

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;

	if(mbox != NULL)
	{
		tmpstr = getpolicychoices();
		if(strstr(tmpstr, mbox) != NULL)
			setpolicy(mbox);
		free(tmpstr); tmpstr = NULL;
	}

	if(mbox != NULL)
	{
//		if(!strncmp("Subchannel", mbox, 15))
		if(ostrcmp(mbox, "Subchannel") == 0)
		{
			free(mbox); mbox = NULL;
			screenlinkedchannel();
			return;
		}
		else if(ostrcmp(mbox, "Resolution Settings") == 0)
		{
			skinname = "resolutionsettings";
			free(mbox); mbox = NULL;
			tmpstr = getvideomode();
			tmpstr1 = getvideomodechoices();
			mbox = menulistbox(tmpstr, tmpstr1, skinname, NULL, NULL, 1, 0);
			free(tmpstr1); tmpstr1 = NULL;
			if(mbox != NULL && ostrcmp(tmpstr, mbox) != 0)
			{
				setvideomode(mbox);
				changefbresolution(mbox);
				int tret = textbox(_("Message"), _("Is this Videomode ok ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				if(tret == 0 || tret == 1)
				{
					setvideomode(tmpstr); 
					changefbresolution(tmpstr);
				}
			}
			free(tmpstr); tmpstr = NULL;
		}
		else if(ostrcmp(mbox, "Aspect Settings") == 0)
		{
			skinname = "aspectsettings";
			free(mbox); mbox = NULL;
			tmpstr = getaspect();
			tmpstr1 = getaspectchoices();
			mbox = menulistbox(tmpstr, tmpstr1, skinname, NULL, NULL, 1, 0);
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
			if(mbox != NULL)
				setaspect(mbox);
		}
		else if(ostrcmp(mbox, "3D Mode") == 0)
		{
			skinname = "3dsettings";
			free(mbox); mbox = NULL;
			tmpstr = getmode3d();
			tmpstr1 = getmode3dchoices();
			mbox = menulistbox(tmpstr, tmpstr1, skinname, NULL, NULL, 1, 0);
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
			if(mbox != NULL)
				setmode3d(mbox);
		}
	}

	free(mbox); mbox = NULL;
}

#endif
