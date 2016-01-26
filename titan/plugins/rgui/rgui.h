/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef RGUI_H
#define RGUI_H

extern struct skin* skin;
extern struct fb* skinfb;

int screenrgui(char* ip)
{
	int rcret = -1, sock = -1, ret = 0, zlen = 0;
	unsigned char* tmpstr = NULL, *buf = NULL;

	if(ip == NULL || ostrcmp(ip, "000.000.000.000") == 0) return 1;

	//clear screen
	drawscreen(skin, 0, 0);

	struct fb* rguiskinfb = addfb("rguiskinfb", 1000, 440, 330, 4, -1, skinfb->fb, 440 * 330 * 4, 0);
	if(rguiskinfb == NULL) return 1;

	ret = sockportopen(&sock, ip, getconfigint("rguidport", NULL), 5000 * 1000);
	if(ret != 0)
	{
		textbox(_("Message"), _("Can't connect to server !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		sockclose(&sock);
    delfb("rguiskinfb");
		return 1;
	}

	buf = malloc(10);
	if(buf == NULL)
	{
		sockclose(&sock);
    delfb("rguiskinfb");
		return 1;
	}
	memset(buf, 0, 10);

	status.spinnertime = 99999999;

	//send 0 byte sequenc to server for connect
	socksend(&sock, buf, 6, 1000 * 1000);
	usleep(5000);
	
	//send rcpower to wakeup remote box
	tmpstr = (unsigned char*)getrcconfig("rcpower", NULL);
	if(tmpstr != NULL)
	{
		memcpy(buf, tmpstr, strlen((char*)tmpstr));
		ret = socksend(&sock, buf, 6, 1000 * 1000);
		memset(buf, 0, 10);
	}

	while(1)
	{
		rcret = waitrc(NULL, 500, 0);

		if(rcret == getrcconfigint("rcpower", NULL)) break;

		if(rcret != RCTIMEOUT)
		{
			tmpstr = (unsigned char*)oitoa(rcret);
			if(tmpstr != NULL)
			{
				memcpy(buf, tmpstr, strlen((char*)tmpstr));
				ret = socksend(&sock, buf, 6, 1000 * 1000);
				free(tmpstr); tmpstr = NULL;
				memset(buf, 0, 10);
				if(ret != 0)
				{
					err("lost connection");
					break;
				}
			}
		}
		else
		{
			ret = sockread(sock, buf, 0, 10, 500 * 1000, 1);
			
			if(ret >= 0)
			{
				int count = atoi((char*)buf);
				memset(buf, 0, 10);
				if(count <= rguiskinfb->varfbsize)
				{
					ret = sockread(sock, rguiskinfb->fb + rguiskinfb->varfbsize, 0, count, 500 * 1000, 1);
					if(ret >= 0)
					{
						char* zbuf = (char*)rguiskinfb->fb;
						ret = ounzip((char*)rguiskinfb->fb + rguiskinfb->varfbsize, count, &zbuf, &zlen, rguiskinfb->varfbsize, 1);
						if(ret == 0)
							blitfb2(rguiskinfb, 0);

						zlen = 0;
					}
				}
				else
					ret = -1;
			}

			if(ret < 0)
			{
				err("lost connection");
				break;
			}

		}
	}
	
	//clear screen
	drawscreen(skin, 0, 0);
	if(textbox(_("Message"), _("Send remote box into standby ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		memcpy(buf, "98989", 5);
		ret = socksend(&sock, buf, 6, 1000 * 1000);
		memset(buf, 0, 10);
		usleep(5000);
	}

	sockclose(&sock);
	free(buf);
	delfb("rguiskinfb");

	//clear screen
	drawscreen(skin, 0, 0);
	status.spinnertime = getconfigint("spinnertime", NULL);

	textbox(_("Message"), _("Connection to server lost !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	return 0;
}

void screenrguimenu()
{
	int rcret = -1;
	struct skin* rgui = getscreen("rgui");
	struct skin* listbox = getscreennode(rgui, "listbox");
	struct skin* rgui1 = getscreennode(rgui, "rgui1");
	struct skin* rgui2 = getscreennode(rgui, "rgui2");
	struct skin* rgui3 = getscreennode(rgui, "rgui3");
	struct skin* rgui4 = getscreennode(rgui, "rgui4");
	struct skin* rgui5 = getscreennode(rgui, "rgui5");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	changemask(rgui1, "000.000.000.000");
	changeinput(rgui1, getconfig("rgui1", NULL));
	changemask(rgui2, "000.000.000.000");
	changeinput(rgui2, getconfig("rgui2", NULL));
	changemask(rgui3, "000.000.000.000");
	changeinput(rgui3, getconfig("rgui3", NULL));
	changemask(rgui4, "000.000.000.000");
	changeinput(rgui4, getconfig("rgui4", NULL));
	changemask(rgui5, "000.000.000.000");
	changeinput(rgui5, getconfig("rgui5", NULL));

	drawscreen(rgui, 0, 0);
	addscreenrc(rgui, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(rgui, tmp);
		rcret = waitrc(rgui, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("rgui1", rgui1, "000.000.000.000");
			addconfigscreencheck("rgui2", rgui2, "000.000.000.000");
			addconfigscreencheck("rgui3", rgui3, "000.000.000.000");
			addconfigscreencheck("rgui4", rgui4, "000.000.000.000");
			addconfigscreencheck("rgui5", rgui5, "000.000.000.000");
	
			if(listbox->select != NULL && listbox->select->ret != NULL)
			{
				tmpstr = fixip(listbox->select->ret, 1);
				screenrgui(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}

			drawscreen(rgui, 0, 0);
			continue;
		}
	}

	delownerrc(rgui);
	clearscreen(rgui);
}

#endif
