#ifndef DVDPLAYER_H
#define DVDPLAYER_H

#ifdef DVDPLAYER
struct ddvd *ddvdconfig = NULL;
struct ddvd_resume resumeinfo;
#endif

int dvdstart(char* filename)
{
#ifdef DVDPLAYER
	int aspect = DDVD_16_9;
	int policy = DDVD_PAN_SCAN;
	int policy2 = DDVD_PAN_SCAN;
	char* tmpstr = NULL;

	if(filename == NULL) return 1;

	ddvdconfig = ddvd_create();
	ddvd_set_dvd_path(ddvdconfig, filename);
	ddvd_set_ac3thru(ddvdconfig, 0);

	tmpstr = ostrcat(getconfig("lang", NULL), NULL, 0, 0);
	if(tmpstr != NULL && strlen(tmpstr) > 3)
	{
		tmpstr += 3;
		ddvd_set_language(ddvdconfig, tmpstr);
	}
	free(tmpstr); tmpstr = NULL;

	tmpstr = getaspect();
	if(ostrncmp(tmpstr, "4:3", 3) == 0)
		aspect = DDVD_4_3;
	else if(ostrncmp(tmpstr, "16:10", 5) == 0)
		aspect = DDVD_16_10;
	free(tmpstr); tmpstr = NULL;
	
	tmpstr = getpolicy();
	if(ostrncmp(tmpstr, "bestfit", 7) == 0)
		policy = DDVD_JUSTSCALE;
	else if(ostrncmp(tmpstr, "letterbox", 9) == 0)
		policy = DDVD_LETTERBOX;
	free(tmpstr); tmpstr = NULL;

#ifdef DDVD_SUPPORTS_16_10_SCALING
	//TODO: tmpstr = getpolicy2();
	if(ostrncmp(tmpstr, "bestfit", 7) == 0)
		policy2 = DDVD_JUSTSCALE;
	else if(ostrncmp(tmpstr, "letterbox", 9) == 0)
		policy2 = DDVD_LETTERBOX;
	free(tmpstr); tmpstr = NULL;

	ddvd_set_video_ex(ddvdconfig, aspect, policy, policy2, DDVD_PAL);
#else
	ddvd_set_video(ddvdconfig, aspect, policy, DDVD_PAL);
#warning please update libdreamdvd for 16:10 scaling support!
#endif

	dvdpause();
	return 0;
#endif
	return 1;
}

/*
void eServiceDVD::gotThreadMessage(const int &msg)
{
	switch(msg)
	{
	case 1: // thread stopped
		m_state = stStopped;
		m_event(this, evStopped);
		break;
	}
}
*/

void dvdgotmessage()
{
#ifdef DVDPLAYER
	switch(ddvd_get_next_message(ddvdconfig, 1))
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
/*
			if (m_subtitle_widget) {
				int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
				int x_offset = 0, y_offset = 0, width = 720, height = 576;
				ddvd_get_last_blit_area(ddvdconfig, &x1, &x2, &y1, &y2);
				
#ifdef DDVD_SUPPORTS_GET_BLIT_DESTINATION
				ddvd_get_blit_destination(ddvdconfig, &x_offset, &y_offset, &width, &height);
				debug(333, "values got from ddvd: %d %d %d %d", x_offset, y_offset, width, height);

				y_offset = -y_offset;
				width -= x_offset * 2;
				height -= y_offset * 2;
#endif
				eRect dest(x_offset, y_offset, width, height);

				if (dest.width() && dest.height())
					m_subtitle_widget->setPixmap(m_pixmap, eRect(x1, y1, (x2-x1)+1, (y2-y1)+1), dest);
			}
*/
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
			//m_event(this, evUser+5);
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
			static struct ddvd_time last_info;
			struct ddvd_time info;

//			debug(333, "DVD_SHOWOSD_TIME");
			ddvd_get_last_time(ddvdconfig, &info);

/*
			if(info.pos_chapter != last_info.pos_chapter )
				m_event(this, evUser+8); // chapterUpdated
			if(info.pos_title != last_info.pos_title )
				m_event(this, evUser+9); // titleUpdated

			memcpy(&last_info, &info, sizeof(struct ddvd_time));
*/
			break;
		}
		case DDVD_SHOWOSD_TITLESTRING:
		{
//			ddvd_get_title_string(ddvdconfig, ddvd_titlestring);
//			debug(333, "DDVD_SHOWOSD_TITLESTRING: %s", ddvd_titlestring);
/*
			loadCuesheet();
			if (!m_cue_pts)
				unpause();
			m_event(this, evStart);
*/
			break;
		}
		case DDVD_MENU_OPENED:
		{
			debug(333, "DVD_MENU_OPENED");
/*
			m_state = stMenu;
			m_event(this, evSeekableStatusChanged);
			m_event(this, evUser+11);
*/
			break;
		}
		case DDVD_MENU_CLOSED:
		{
			debug(333, "DVD_MENU_CLOSED");
/*
			m_state = stRunning;
			m_event(this, evSeekableStatusChanged);
			m_event(this, evUser+12);
*/
			break;
		}
#ifdef DDVD_SUPPORTS_PICTURE_INFO
		case DDVD_SIZE_CHANGED:
		{
			debug(333, "DVD_SIZE_CHANGED");
/*
			int changed = m_width != -1 && m_height != -1 && m_aspect != -1;
			ddvd_get_last_size(ddvdconfig, &m_width, &m_height, &m_aspect);
			if (changed)
				m_event((iPlayableService*)this, evVideoSizeChanged);
*/
			break;
		}
		case DDVD_PROGRESSIVE_CHANGED:
		{
			debug(333, "DVD_PROGRESSIVE_CHANGED");
/*
			int changed = m_progressive != -1;
			ddvd_get_last_progressive(ddvdconfig, &m_progressive);
			if (changed)
				m_event((iPlayableService*)this, evVideoProgressiveChanged);
*/
			break;
		}
		case DDVD_FRAMERATE_CHANGED:
		{
			debug(333, "DVD_FRAMERATE_CHANGED");
/*
			int changed = m_framerate != -1;
			ddvd_get_last_framerate(m_ddvdconfig, &m_framerate);
			if (changed)
				m_event((iPlayableService*)this, evVideoFramerateChanged);
*/
			break;
		}
#endif
		default:
			break;
	}
#endif
}

int dvdstop()
{
#ifdef DVDPLAYER
	debug(333, "dvd stop");

	if(ddvdconfig != NULL)
	{
		ddvd_send_key(ddvdconfig, DDVD_KEY_EXIT);
		ddvd_close(ddvdconfig);
		ddvdconfig = NULL;
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

void dvdthread()
{
	debug(333, "dvd thread start");

#ifdef DVDPLAYER
	ddvd_run(ddvdconfig);
#endif

	debug(333, "dvd thread end");
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

int dvbsetfb()
{
#ifdef DVDPLAYER
	if(ddvdconfig == NULL) return 1;
#ifdef DDVD_SUPPORTS_GET_BLIT_DESTINATION
	ddvd_set_lfb_ex(ddvdconfig, skinfb->fb, skinfb->width, skinfb->height, skinfb->colbytes, skinfb->pitch, 1);
#else
	ddvd_set_lfb(ddvdconfig, skinfb->fb, skinfb->width, skinfb->height, skinfb->colbytes, skinfb->pitch);
#warning please update libdreamdvd for fast scaling
#endif
#endif
	return 0;
}

/*
RESULT eServiceDVD::enableSubtitles(eWidget *parent, ePyObject tuple)
{
	eSize size = eSize(720, 576);

	int pid = -1;

	if ( tuple != Py_None )
	{		
		ePyObject entry;
		int tuplesize = PyTuple_Size(tuple);
		if (!PyTuple_Check(tuple))
			goto error_out;
		if (tuplesize < 1)
			goto error_out;
		entry = PyTuple_GET_ITEM(tuple, 1);
		if (!PyInt_Check(entry))
			goto error_out;
		pid = PyInt_AsLong(entry)-1;

		ddvd_set_spu(m_ddvdconfig, pid);
		m_event(this, evUser+7);
	}
	eDebug("eServiceDVD::enableSubtitles %i", pid);

	if (!m_pixmap)
	{
		run(); // start the thread
	}

}
*/

unsigned long long int dvdgetlength()
{
	unsigned long long int pts = 0;

#ifdef DVDPLAYER
	struct ddvd_time info;

	if(ddvdconfig != NULL)
	{
		ddvd_get_last_time(ddvdconfig, &info);

		pts = info.end_hours * 3600;
		pts += info.end_minutes * 60;
		pts += info.end_seconds;
		pts *= 90000;
	}
#endif

	return pts;
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
	if(key == getrcconfigint("rcblue", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_AUDIO);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_SUBTITLE);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_AUDIOMENU);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_NEXT_CHAPTER);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_PREV_CHAPTER);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_NEXT_TITLE);
	if(key == getrcconfigint("rcyellow", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_PREV_TITLE);
	if(key == getrcconfigint("rcmenu", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_MENU);
	if(key == getrcconfigint("rcmenu", NULL))
		ddvd_send_key(ddvdconfig, DDVD_KEY_ANGLE);
#endif

	return 0;
}

#endif
