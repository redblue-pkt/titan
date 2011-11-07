#ifndef BLUEKEYACTIONS_H
#define BLUEKEYACTIONS_H

void screenbluekeyactions()
{
	debug(1000, "in");

	char* bluekey = NULL;
	struct skin* pluginnode = NULL;
	void (*startplugin)(void);

	bluekey = getconfig("bluekey", NULL);
	if(bluekey == NULL)
	{
		bluekeyactions_setres();
		return;
	}
	
	debug(60, "bluekey=%s", bluekey);

	if(ostrcmp(bluekey, "Softcam Panel") == 0)
	{
		pluginnode = getplugin("AAF Panel");
		
		if(pluginnode != NULL)
		{
			startplugin = dlsym(pluginnode->pluginhandle, "screenaafpanel_softcam");
			if(startplugin != NULL){
				startplugin();
			}
			free(bluekey); bluekey = NULL;
			return;
		}
	}
	else if(ostrcmp(bluekey, "Auto Resolution (default)") == 0)
	{
		bluekeyactions_setres();
		free(bluekey); bluekey = NULL;
		return;
	}
	else if(ostrcmp(bluekey, "Extentions List") == 0)
	{
		redkeyactions_extensions();
		free(bluekey); bluekey = NULL;
		return;
	}
	else if(ostrcmp(bluekey, "Multi EPG") == 0)
	{
		screenmultiepg(NULL, NULL);
		free(bluekey); bluekey = NULL;
		return;
	}
	else if(ostrcmp(bluekey, "Graphic Multi EPG") == 0)
	{
		screengmultiepg(NULL, NULL);
		free(bluekey); bluekey = NULL;
		return;
	}
	else if(ostrcmp(bluekey, "Sleep Timer") == 0)
	{
		screenpowerofftimer();
		free(bluekey); bluekey = NULL;
		return;
	}
	else if(ostrcmp(bluekey, "Child Protection") == 0)
	{
		screenpin();
		free(bluekey); bluekey = NULL;
		return;
	}
	
	pluginnode = getplugin(bluekey);

	if(pluginnode != NULL)
	{
		startplugin = dlsym(pluginnode->pluginhandle, "start");
		if(startplugin != NULL)
			startplugin();
	}

	debug(1000, "out");
	return;
}

void bluekeyactions_setres()
{
	int m_width;
	char* res = NULL;
	char* res_act = NULL;
	char* res_def = NULL;
	char* res_sd = NULL;
	
//	if(ostrcmp(getconfig("av_videomode_autores", NULL), "blue") == 0)
//	{	
		//if(videoreadqwidth(status.aktservice->videodev) == 0)
		if(getaktvideosize() == 0)
		{
			m_width = status.videosize.w;
			if (m_width == 720)
			{
				res_sd = getconfig("av_videomode_autores_sd", NULL);
				if(res_sd == NULL) 
					res = ostrcat(res, "576i50", 1, 0);
				else
					res = ostrcat(res, res_sd, 1, 0);
			}
			else if (m_width == 1280)
				res = ostrcat(res, "720p50", 1, 0);
			else if (m_width == 1920)
				res = ostrcat(res, "1080i50", 1, 0);	
			else
				m_width = 0;
			if ( m_width > 0)
			{	
				res_act = getvideomode();
				if (ostrcmp(res_act, res) == 0) 
				{
					res_def = getconfig("av_videomode_default", NULL);
					if (res_def == NULL)
						 res_def = res_act;
					setvideomode(res_def);
					changefbresolution(res_def);
					sleep(2);
					screenautores(res_def, 5, 0);		 	
				} else {
					setvideomode(res);
					changefbresolution(res);
					sleep(2);
					screenautores(res, 5, 0);
				}
			}			
		} else {
			textbox(_("Message"), _("ERROR cant read res"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
				
		}
		free(res);
		res = NULL;
//	}	
	return;
}				
#endif
