#ifndef DVDPLAYER_H
#define DVDPLAYER_H

#ifdef DVDPLAYER
struct ddvd *ddvdconfig = NULL;
struct ddvd_resume resumeinfo;
struct stimerthread* dvdtimerthread = NULL;
struct fb* dvdskinfb = NULL;
int tv_aspect = DDVD_16_9;
int tv_policy = DDVD_PAN_SCAN;
int tv_policy2 = DDVD_PAN_SCAN;
char* tv_videomode = NULL;
int dvdmenu = 0;
#endif

int dvd_calc_y_fbscreen()
{
	int y_offset = 0;
	int dvd_aspect = 0;

#ifdef DVDPLAYER
	//0=4:3 2=16:9
	dvd_aspect = ddvd_get_dvd_aspect(ddvdconfig);

	if(dvd_aspect == 0 && tv_policy == DDVD_PAN_SCAN)
	{
		switch(tv_aspect)
		{
			case DDVD_16_10:
				y_offset = -(dvdskinfb->height * 15 / 12 - dvdskinfb->height) / 2;
				break;
			case DDVD_16_9:
				y_offset = (dvdskinfb->height * 3 / 4 - dvdskinfb->height) / 2;
				break;
		}
	}

	if(dvd_aspect >= 2 && tv_aspect == DDVD_4_3 && tv_policy == DDVD_LETTERBOX)
		y_offset = (fb->height - fb->height * 3 / 4) / 2;

	if(dvd_aspect >= 2 && tv_aspect == DDVD_16_10 && tv_policy2 == DDVD_LETTERBOX)
		y_offset = (fb->height - fb->height * 15 / 16) / 2;


#endif
	return y_offset;
}

int dvd_calc_x_fbscreen()
{
	int x_offset = 0;
	int dvd_aspect = 0;

#ifdef DVDPLAYER
	//0=4:3 2=16:9
	dvd_aspect = ddvd_get_dvd_aspect(ddvdconfig);

	if(dvd_aspect == 0 && tv_policy == DDVD_LETTERBOX)
	{
		switch(tv_aspect)
		{
			case DDVD_16_10:
				x_offset = (fb->width - fb->width * 12 / 15) / 2;
				break;
			case DDVD_16_9:
				x_offset = (fb->width - fb->width * 3 / 4) / 2;
				break;
		}
	}

	if(dvd_aspect >= 2 && tv_aspect == DDVD_4_3 && tv_policy == DDVD_PAN_SCAN)
		x_offset = (dvdskinfb->width * 3 / 4 - dvdskinfb->width) / 2;

	if(dvd_aspect >= 2 && tv_aspect == DDVD_16_10 && tv_policy2 == DDVD_PAN_SCAN)
		x_offset = (dvdskinfb->width * 15 / 16 - dvdskinfb->width) / 2;

#endif
	return x_offset;
}

int dvdblit()
{
#ifdef DVDPLAYER
	int x_offset = dvd_calc_x_fbscreen();
	int y_offset = dvd_calc_y_fbscreen();

	m_lock(&status.drawingmutex, 0);

	int leftoffset = status.leftoffset;
	int rightoffset = status.rightoffset;
	int topoffset = status.topoffset;
	int bottomoffset = status.bottomoffset;

	if(leftoffset != 0) status.leftoffset = 0;
	if(rightoffset != 0) status.rightoffset = 0;
	if(topoffset != 0) status.topoffset = 0;
	if(bottomoffset != 0) status.bottomoffset = 0;

	unsigned char* fb = dvdskinfb->fb;
	int width = dvdskinfb->width;
	int height = dvdskinfb->height;

	if(x_offset > 0)
	{
		status.leftoffset = x_offset;
		status.rightoffset = x_offset;
	}
	if(x_offset < 0)
	{
		dvdskinfb->fb -= x_offset * dvdskinfb->colbytes;
		dvdskinfb->width += (x_offset * 2);
	}
	if(y_offset > 0)
	{
		status.topoffset = y_offset;
		status.bottomoffset = y_offset;
	}
	if(y_offset < 0)
	{
		dvdskinfb->fb -= y_offset * dvdskinfb->pitch;
		dvdskinfb->height += (y_offset * 2);
	}

	blitfb2(dvdskinfb, 0);

	if(x_offset < 0)
	{
		dvdskinfb->fb = fb;
		dvdskinfb->width = width;
	}
	if(y_offset < 0)
	{
		dvdskinfb->fb = fb;
		dvdskinfb->height = height;
	}
	
	status.leftoffset = leftoffset;
	status.rightoffset = rightoffset;
	status.topoffset = topoffset;
	status.bottomoffset = bottomoffset;

	m_unlock(&status.drawingmutex, 0);
#endif

	return 0;
}

void dvdchangevideo()
{
#ifdef DVDPLAYER
	char* tmpstr = NULL;

	tmpstr = getaspect();
	if(ostrncmp(tmpstr, "4:3", 3) == 0)
		tv_aspect = DDVD_4_3;
	else if(ostrncmp(tmpstr, "16:10", 5) == 0)
		tv_aspect = DDVD_16_10;
	else
		tv_aspect = DDVD_16_9;
	free(tmpstr); tmpstr = NULL;
	
	tmpstr = getpolicy();
	if(ostrncmp(tmpstr, "bestfit", 7) == 0)
		tv_policy = DDVD_JUSTSCALE;
	else if(ostrncmp(tmpstr, "letterbox", 9) == 0)
		tv_policy = DDVD_LETTERBOX;
	else
		tv_policy = DDVD_PAN_SCAN;
	free(tmpstr); tmpstr = NULL;

	//TODO:
	//tmpstr = getpolicy2();
	//if(ostrncmp(tmpstr, "bestfit", 7) == 0)
	//	tv_policy2 = DDVD_JUSTSCALE;
	//else if(ostrncmp(tmpstr, "letterbox", 9) == 0)
	//	tv_policy2 = DDVD_LETTERBOX;
	//else
	//	tv_policy2 = DDVD_PANSCAN;
	//free(tmpstr); tmpstr = NULL;
	tv_policy2 = tv_policy;
#endif
}

void dvdcleanup()
{
#ifdef DVDPLAYER
	ddvdconfig = NULL;
	dvdtimerthread = NULL;
	delfb("dvdskinfb");
	dvdskinfb = NULL;
	if(tv_videomode != NULL)
	{
		setvideomode(tv_videomode, 0);
		changefbresolution(tv_videomode);
		free(tv_videomode); tv_videomode = NULL;
	}
#endif
}

void dvdthread()
{
	debug(333, "dvd thread start");

#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
	{
		ddvd_run(ddvdconfig);
		ddvd_close(ddvdconfig);
	}
	
	dvdcleanup();
#endif

	debug(333, "dvd thread end");
}

int dvdstart(char* filename)
{
#ifdef DVDPLAYER
	char* tmpstr = NULL;

	if(ddvdconfig != NULL)
	{
		debug(333, "dvdplayer allready running");
		dvdstop();
	}

	if(filename == NULL) return 1;
	
	status.playercan = 0x7FFF;

	ddvdconfig = ddvd_create();
	if(ddvdconfig == NULL) return 1;
	
	ddvd_set_dvd_path(ddvdconfig, filename);
	ddvd_set_ac3thru(ddvdconfig, 0);

	tmpstr = ostrcat(getconfig("lang", NULL), NULL, 0, 0);
	if(tmpstr != NULL && strlen(tmpstr) > 3)
	{
		tmpstr += 3;
		ddvd_set_language(ddvdconfig, tmpstr);
		tmpstr -= 3;
	}
	free(tmpstr); tmpstr = NULL;

	dvdchangevideo();

#ifdef DDVD_SUPPORTS_16_10_SCALING
	ddvd_set_video_ex(ddvdconfig, tv_aspect, tv_policy, tv_policy2, DDVD_PAL);
#else
	ddvd_set_video(ddvdconfig, tv_aspect, tv_policy, DDVD_PAL);
#warning please update libdreamdvd for 16:10 scaling support!
#endif
	if(dvdsetfb() != 0)
	{
		ddvd_close(ddvdconfig);
		ddvdconfig = NULL;
		err("can't set dvdskinfb");
		return 1;
	}

	free(tv_videomode); tv_videomode = NULL;
	tv_videomode = getvideomode();

	//change videomode if greater 720
	if(ostrncmp("1080", tv_videomode, 4) == 0)
	{
		setvideomode("720p50", 0);
		changefbresolution("720p50");
	}
	else 
	{
		free(tv_videomode);
		tv_videomode = NULL;
	}

	ddvd_set_spu(ddvdconfig, -1);
	dvdtimerthread = addtimer(&dvdthread, START, 1000, 1, NULL, NULL, NULL);
	sleep(1);
	if(dvdtimerthread != NULL)
		return 0;
	
	dvdcleanup();
#endif
	return 1;
}

int dvdmenuopen()
{
#ifdef DVDPLAYER
	return dvdmenu;
#else
	return 0;
#endif
}

void dvdgotmessage()
{
#ifdef DVDPLAYER
	if(ddvdconfig == NULL) return;
	switch(ddvd_get_next_message(ddvdconfig, 0))
	{
		case DDVD_COLORTABLE_UPDATE:
		{
			debug(333, "DDVD_COLORTABLE_UPDATE");
/*
			struct ddvd_color ctmp[4];
			ddvd_get_last_colortable(ddvdconfig, ctmp);
			int i = 0;
			while (i < 4)
			{
				rd1[252+i] = ctmp[i].red;
				bl1[252+i] = ctmp[i].blue;
				gn1[252+i] = ctmp[i].green;
				tr1[252+i] = ctmp[i].trans;
				i++;
			}
			if(ioctl(fb, FBIOPUTCMAP, &colormap) == -1)
			{
				err("FBIOPUTCMAP");
				return 1;
			}
*/
			debug(333, "no support for 8bpp framebuffer in dvdplayer yet");
			break;
		}
		case DDVD_SCREEN_UPDATE:
		{
			debug(333, "DVD_SCREEN_UPDATE");

			if(dvdskinfb != NULL)
			{
				int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
				ddvd_get_last_blit_area(ddvdconfig, &x1, &x2, &y1, &y2);
				debug(333, "ddvd blit area: %d %d %d %d", x1, y1, x2, y2);
				if(x1 != 0 || x2 != 0 || y1 != 0 || y2 != 0)
					dvdblit();
			}
			break;
		}
		case DDVD_SHOWOSD_STATE_PLAY:
		{
			debug(333, "DVD_SHOWOSD_STATE_PLAY");
			//TODO: show play icon
			break;
		}
		case DDVD_SHOWOSD_STATE_PAUSE:
		{
			debug(333, "DVD_SHOWOSD_STATE_PAUSE");
			//TODO: show pause icon
			break;
		}
		case DDVD_SHOWOSD_STATE_FFWD:
		{
			debug(333, "DVD_SHOWOSD_STATE_FFWD");
			//TODO: show ff icon
			break;
		}
		case DDVD_SHOWOSD_STATE_FBWD:
		{
			debug(333, "DVD_SHOWOSD_STATE_FBWD");
			//TODO: show fr icon
			break;
		}
		case DDVD_SHOWOSD_STRING:
		{
			debug(333, "DVD_SHOWOSD_STRING");
			break;
		}
		case DDVD_SHOWOSD_AUDIO:
		{
			debug(333, "DVD_SHOWOSD_STRING");
			//TODO: show audio osd
			break;
		}
		case DDVD_SHOWOSD_SUBTITLE:
		{
			debug(333, "DVD_SHOWOSD_SUBTITLE");
			//TODO: show subtitle osd
			break;
		}
		case DDVD_EOF_REACHED:
		{
			debug(333, "DVD_EOF_REACHED");
			//TODO: eof
			break;
		}
		case DDVD_SOF_REACHED:
		{
			debug(333, "DVD_SOF_REACHED");
			//TODO: sof
			break;
		}
		case DDVD_SHOWOSD_ANGLE:
		{
			debug(333, "DVD_ANGLE_INFO");
			//TODO: angel
			break;
		}
		case DDVD_SHOWOSD_TIME:
		{
			break;
		}
		case DDVD_SHOWOSD_TITLESTRING:
		{
//			ddvd_get_title_string(ddvdconfig, ddvd_titlestring);
//			debug(333, "DDVD_SHOWOSD_TITLESTRING: %s", ddvd_titlestring);
			break;
		}
		case DDVD_MENU_OPENED:
		{
			debug(333, "DVD_MENU_OPENED");
			dvdmenu = 1;
			break;
		}
		case DDVD_MENU_CLOSED:
		{
			debug(333, "DVD_MENU_CLOSED");
			dvdmenu = 0;
			break;
		}
#ifdef DDVD_SUPPORTS_PICTURE_INFO
		case DDVD_SIZE_CHANGED:
		{
			debug(333, "DVD_SIZE_CHANGED");
/*
			int changed = m_width != -1 && m_height != -1 && m_aspect != -1;
			ddvd_get_last_size(ddvdconfig, &m_width, &m_height, &m_aspect);
*/
			break;
		}
		case DDVD_PROGRESSIVE_CHANGED:
		{
			debug(333, "DVD_PROGRESSIVE_CHANGED");
/*
			int changed = m_progressive != -1;
			ddvd_get_last_progressive(ddvdconfig, &m_progressive);
*/
			break;
		}
		case DDVD_FRAMERATE_CHANGED:
		{
			debug(333, "DVD_FRAMERATE_CHANGED");
/*
			int changed = m_framerate != -1;
			ddvd_get_last_framerate(m_ddvdconfig, &m_framerate);
*/
			break;
		}
#endif
		default:
			break;
	}
#endif
}

int dvdisplaying()
{
#ifdef DVDPLAYER
        if(dvdtimerthread == NULL)
                return 0;
#endif
        return 1;
}

void dvdafterend()
{
}

int dvdstop()
{
#ifdef DVDPLAYER
	debug(333, "dvd stop");
	int i = 0;

	if(ddvdconfig != NULL)
	{
		ddvd_send_key(ddvdconfig, DDVD_KEY_EXIT);
		while(dvdtimerthread != NULL)
		{
			if(i >= 5) break;
			sleep(1);
			i++;
		}
	}
#endif
	return 0;
}

int dvdgetnumaudiotracks()
{
	unsigned int i = 0;

#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
		ddvd_get_audio_count(ddvdconfig, &i);
#endif

	return i;
}

int dvdgetcuraudiotrack()
{
	int audioid = -1;

#ifdef DVDPLAYER
	int audiotype = 0;
	uint16_t audiolang = 0;

	if(ddvdconfig != NULL)
		ddvd_get_last_audio(ddvdconfig, &audioid, &audiolang, &audiotype);
#endif

	return audioid;
}

int dvdchangeaudiotrack(unsigned int i)
{
#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
		ddvd_set_audio(ddvdconfig, i);
#endif

	return 0;
}

char* dvdgetaudiotrackinfo(unsigned int audioid)
{
	char* tmpstr = NULL;

#ifdef DVDPLAYER
	int audiotype = 0;
	uint16_t audiolang;

	if(ddvdconfig == NULL) return NULL;

	ddvd_get_audio_byid(ddvdconfig, audioid, &audiolang, &audiotype);

	char audiostr[3] = {audiolang >> 8, audiolang, 0};
	tmpstr = ostrcat(tmpstr, audiostr, 1, 0);
	tmpstr = ostrcat(tmpstr, " (", 1, 0);

	switch(audiotype)
	{
		case DDVD_MPEG:
			tmpstr = ostrcat(tmpstr, "MPEG", 1, 0);
			break;
		case DDVD_AC3:
			tmpstr = ostrcat(tmpstr, "AC3", 1, 0);
			break;
		case DDVD_DTS:
			tmpstr = ostrcat(tmpstr, "DTS", 1, 0);
			break;
		case DDVD_LPCM:
			tmpstr = ostrcat(tmpstr, "LPCM", 1, 0);
			break;
		default:
			tmpstr = ostrcat(tmpstr, "unknown", 1, 0);
	}
	tmpstr = ostrcat(tmpstr, ")", 1, 0);
#endif

	return tmpstr;
}

int dvdgetnumsputracks()
{
	unsigned int i = 0;

#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
        	ddvd_get_spu_count(ddvdconfig, &i);
#endif

	return i;
}

int dvdgetcursputrack()
{
	int spuid = -1;

#ifdef DVDPLAYER
	uint16_t spulang = 0;

	if(ddvdconfig != NULL)
		ddvd_get_last_spu(ddvdconfig, &spuid, &spulang);
#endif

	return spuid;
}

int dvdchangesputrack(unsigned int i)
{
#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
		ddvd_set_spu(ddvdconfig, i);
#endif

	return 0;
}

char* dvdgetsputrackinfo(unsigned int spuid)
{
	char* tmpstr = NULL;

#ifdef DVDPLAYER
	uint16_t spulang;

	if(ddvdconfig == NULL) return NULL;

	ddvd_get_spu_byid(ddvdconfig, spuid, &spulang);
	char spustr[3] = {spulang >> 8, spulang, 0};

	tmpstr = ostrcat(tmpstr, spustr, 1, 0);
#endif

	return tmpstr;
}

int dvdgetangelinfo()
{
#ifdef DVDPLAYER
	int current = 0, num = 0;

	if(ddvdconfig != NULL)
		ddvd_get_angle_info(ddvdconfig, &current, &num);
#endif

	return 0;
}


int dvdff(int speed)
{
#ifdef DVDPLAYER
	if(ddvdconfig == NULL) return 1;
	debug(333, "dvd fastforward (%d)", speed);

	if(speed > 0 && speed <= 64)
		ddvd_send_key(ddvdconfig, DDVD_KEY_FFWD);
#endif

	return 0;
}

int dvdfr(int speed)
{
#ifdef DVDPLAYER
	if(ddvdconfig == NULL) return 1;
	debug(333, "dvd rewind (%d)", speed);

	if(speed < 0 && speed >= -64)
		ddvd_send_key(ddvdconfig, DDVD_KEY_FBWD);
#endif

	return 0;
}

int dvdpause()
{
	debug(333, "dvd pause");

#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
		ddvd_send_key(ddvdconfig, DDVD_KEY_PAUSE);
#endif

	return 0;
}

int dvdplay()
{
	debug(333, "dvd play");

#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
		ddvd_send_key(ddvdconfig, DDVD_KEY_PLAY);
#endif

	return 0;
}

int dvdcontinue()
{
	return dvdplay();
}

int dvdgetinfo(int flag)
{
#ifdef DVDPLAYER
	if(ddvdconfig == NULL) return 1;

	switch(flag)
	{
		case 0: //current chapter
		{
			struct ddvd_time info;
			ddvd_get_last_time(ddvdconfig, &info);
			return info.pos_chapter;
		}
		case 1: //tital chapter
		{
			struct ddvd_time info;
			ddvd_get_last_time(ddvdconfig, &info);
			return info.end_chapter;
		}
		case 2: //current title
		{
			struct ddvd_time info;
			ddvd_get_last_time(ddvdconfig, &info);
			return info.pos_title;
		}
		case 3: //total title
		{
			struct ddvd_time info;
			ddvd_get_last_time(ddvdconfig, &info);
			return info.end_title;
		}
		case 4: //teletext pid
		{
			int spuid = 0;
			uint16_t spulang = 0;
			ddvd_get_last_spu(ddvdconfig, &spuid, &spulang);
			return spuid;
		}
	}
#endif
	return 0;
}

int dvdsetfb()
{
#ifdef DVDPLAYER
	int width = 720, height = 576, colbytes = 4;

	if(dvdskinfb == NULL)
		dvdskinfb = addfb("dvdskinfb", 1001, width, height, colbytes, -1, fb->fb + (fb->width * fb->height * fb->colbytes), width * height * colbytes);

	if(dvdskinfb == NULL) return 1;
	if(ddvdconfig == NULL) return 1;
//#ifdef DDVD_SUPPORTS_GET_BLIT_DESTINATION
	ddvd_set_lfb_ex(ddvdconfig, dvdskinfb->fb, dvdskinfb->width, dvdskinfb->height, dvdskinfb->colbytes, dvdskinfb->pitch, 1);
//#else
//	ddvd_set_lfb(ddvdconfig, dvdskinfb->fb, dvdskinfb->width, dvdskinfb->height, dvdskinfb->colbytes, dvdskinfb->pitch);
//#warning please update libdreamdvd for fast scaling
//#endif
#endif
	return 0;
}

unsigned long long int dvdgetlength()
{
	unsigned long long int len = 0;

#ifdef DVDPLAYER
	struct ddvd_time info;

	if(ddvdconfig != NULL)
	{
		ddvd_get_last_time(ddvdconfig, &info);

		len = info.end_hours * 3600;
		len += info.end_minutes * 60;
		len += info.end_seconds;
	}
#endif

	return len;
}

int dvdsetresume()
{
#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
	{
		debug(333, "title=%d, chapter=%d, block=%lu, audioid=%d, audiolock=%d, spuid=%d, spulock=%d", resumeinfo.title, resumeinfo.chapter, resumeinfo.block, resumeinfo.audio_id, resumeinfo.audio_lock, resumeinfo.spu_id, resumeinfo.spu_lock);
		ddvd_set_resume_pos(ddvdconfig, resumeinfo);
	}
#endif
	return 0;
}

int dvdgetresume()
{
#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
	{
		ddvd_get_resume_pos(ddvdconfig, &resumeinfo);
		debug(333, "title=%d, chapter=%d, block=%lu, audioid=%d, audiolock=%d, spuid=%d, spulock=%d", resumeinfo.title, resumeinfo.chapter, resumeinfo.block, resumeinfo.audio_id, resumeinfo.audio_lock, resumeinfo.spu_id, resumeinfo.spu_lock);
	}
#endif
	return 0;
}

int dvdseek(int sec)
{
#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
	{
		debug(333, "seek sec=%d", sec);
		ddvd_skip_seconds(ddvdconfig, sec);
	}
#endif
	return 0;
}

unsigned long long int dvdgetpts()
{
	unsigned long long int pts = 0;

#ifdef DVDPLAYER
	struct ddvd_time info;

	if(ddvdconfig != NULL)
	{
		ddvd_get_last_time(ddvdconfig, &info);

		pts = info.pos_hours * 3600;
		pts += info.pos_minutes * 60;
		pts += info.pos_seconds;
		pts *= 90000;
	}
#endif

	return pts;
}

int dvdseektitle(int title)
{
	debug(333, "seek title %d", title);

#ifdef DVDPLAYER
	if(ddvdconfig != NULL)
		ddvd_set_title(ddvdconfig, title);
#endif

	return 0;
}

int dvdseekchapter(int chapter)
{
	debug(333, "seek chapter %d", chapter);

#ifdef DVDPLAYER
	if(chapter > 0 && ddvdconfig != NULL)
		ddvd_set_chapter(ddvdconfig, chapter);
#endif

	return 0;
}

int dvdkeypress(int key)
{
#ifdef DVDPLAYER
	if(ddvdconfig == NULL) return 1;

	if(key == getrcconfigint("rcleft", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_LEFT);
	if(key == getrcconfigint("rcright", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_RIGHT);
	if(key == getrcconfigint("rcup", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_UP);
	if(key == getrcconfigint("rcdown", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_DOWN);
	if(key == getrcconfigint("rcok", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_OK);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_AUDIO);
	if(key == getrcconfigint("rctext", NULL) || key == getrcconfigint("rcsubtitel", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_SUBTITLE);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_AUDIOMENU);
	if(key == getrcconfigint("rcnext", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_NEXT_CHAPTER);
	if(key == getrcconfigint("rcprev", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_PREV_CHAPTER);
	if(key == getrcconfigint("rcchup", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_NEXT_TITLE);
	if(key == getrcconfigint("rcchdown", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_PREV_TITLE);
	if(key == getrcconfigint("rcmenu", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_MENU);
	if(key == getrcconfigint("rcblue", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_ANGLE);
#endif

	return 0;
}

#endif
