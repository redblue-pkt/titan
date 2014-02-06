#ifndef VIDEOMODE_H
#define VIDEOMODE_H

//flag 0: show all
//flag 1: no subchannel
//flag 2: no subchannel, no resolution change
void screenvideomode(int flag)
{
	char* skinname = "videomode";
	char* tmpstr = NULL;
	char* tmpstr1 = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;

	tmpstr = getpolicy();
	tmpstr1 = ostrcat(tmpstr1, getpolicychoices(), 1, 1);

	if(flag == 0) addmenulist(&mlist, _("Subchannel"), NULL, NULL, 0, 0);
	if(flag == 0 || flag == 1) addmenulist(&mlist, _("Resolution Settings"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Aspect Settings"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("3D Mode"), NULL, NULL, 0, 0);
	addmenulist(&mlist, NULL, NULL, NULL, 1, 0);
	addmenulistall(&mlist, tmpstr1, NULL, 0, tmpstr);

	if(flag == 0)
		mbox = menulistbox(mlist, skinname, _("Videomode"), NULL, NULL, 1, 1);
	else
		mbox = menulistbox(mlist, skinname, _("Videomode"), NULL, NULL, 1, 0);

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;

	if(mbox != NULL)
	{
		if(ostrcmp(mbox->name, _("Subchannel")) == 0)
		{
			freemenulist(mlist, 1); mlist = NULL;
			screenlinkedchannel();
			return;
		}
		else if(ostrcmp(mbox->name, _("Resolution Settings")) == 0)
		{
			skinname = "resolutionsettings";
			freemenulist(mlist, 1); mlist = NULL;
			tmpstr = getvideomode();
			tmpstr1 = getvideomodechoices();
			addmenulistall(&mlist, tmpstr1, NULL, 0, tmpstr);
			mbox = menulistbox(mlist, skinname, _("Resolution Settings"), NULL, NULL, 1, 0);
			free(tmpstr1); tmpstr1 = NULL;
			if(mbox != NULL && ostrcmp(tmpstr, mbox->name) != 0)
			{
				setvideomode(mbox->name, 0);
				changefbresolution(mbox->name, 0);
				int tret = textbox(_("Message"), _("Is this Videomode ok ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				if(tret == 0 || tret == 2)
				{
					setvideomode(tmpstr, 0); 
					changefbresolution(tmpstr, 0);
				}
			}
			free(tmpstr); tmpstr = NULL;
		}
		else if(ostrcmp(mbox->name, _("Aspect Settings")) == 0)
		{
			skinname = "aspectsettings";
			freemenulist(mlist, 1); mlist = NULL;
			tmpstr = getaspect();
			tmpstr1 = getaspectchoices();
			addmenulistall(&mlist, tmpstr1, NULL, 0, tmpstr);
			mbox = menulistbox(mlist, skinname, _("Aspect Settings"), NULL, NULL, 1, 0);
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
			if(mbox != NULL)
				setaspect(mbox->name);
		}
		else if(ostrcmp(mbox->name, _("3D Mode")) == 0)
		{
			skinname = "3dsettings";
			freemenulist(mlist, 1); mlist = NULL;
			tmpstr = getmode3d();
			tmpstr1 = getmode3dchoices();
			addmenulistall(&mlist, tmpstr1, NULL, 0, tmpstr);
			mbox = menulistbox(mlist, skinname, _("3D Mode"), NULL, NULL, 1, 0);
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
			if(mbox != NULL)
      {
        blitfb(0);
				setmode3d(mbox->name);
      }
		}
		else
		{
			tmpstr = getpolicychoices();
			if(tmpstr != NULL && ostrstr(tmpstr, mbox->name) != NULL)
				setpolicy(mbox->name);
			free(tmpstr); tmpstr = NULL;
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
}

#endif
