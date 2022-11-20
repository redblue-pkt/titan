#ifndef PLAYER_H
#define PLAYER_H

// playercan bits:
// 0 policy
// 1 auditraklist
// 2 subtitle
// 3 videomode
// 4 powerofftimer
// 5 videosettings
// 6 stop
// 7 ff
// 8 fr
// 9 pause
// 10 play
// 11 jump/seek reverse
// 12 jump/seek forward
// 13 changecodec
// 14 infobar
// 15 slowmotion

#ifdef EPLAYER3
Context_t * player = NULL; 
extern OutputHandler_t OutputHandler; 
extern PlaybackHandler_t PlaybackHandler; 
extern ContainerHandler_t ContainerHandler; 
extern ManagerHandler_t ManagerHandler;

#ifdef EXTEPLAYER3
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#if defined(OEBUILD) || defined(SH4)
#include <inttypes.h>
#include <stdarg.h>
#endif
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#if defined(OEBUILD) || defined(SH4)
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <pthread.h>
#endif
#include "common.h"
#if defined(OEBUILD) || defined(SH4)
//#include "misc.h"
#endif
extern int ffmpeg_av_dict_set(const char *key, const char *value, int flags);
extern void       aac_software_decoder_set(const int32_t val);
extern void  aac_latm_software_decoder_set(const int32_t val);
extern void       dts_software_decoder_set(const int32_t val);
extern void       wma_software_decoder_set(const int32_t val);
extern void       ac3_software_decoder_set(const int32_t val);
extern void      eac3_software_decoder_set(const int32_t val);
extern void       mp3_software_decoder_set(const int32_t val);
#if defined(OEBUILD) || defined(SH4)
extern void       amr_software_decoder_set(const int32_t val);
extern void    vorbis_software_decoder_set(const int32_t val);
extern void      opus_software_decoder_set(const int32_t val);
#endif
extern void            rtmp_proto_impl_set(const int32_t val);
extern void        flv2mpeg4_converter_set(const int32_t val);
#if defined(OEBUILD) || defined(SH4)
extern void        sel_program_id_set(const int32_t val);
#endif
extern void pcm_resampling_set(int32_t val);
extern void stereo_software_decoder_set(int32_t val);
extern void insert_pcm_as_lpcm_set(int32_t val);
extern void progressive_playback_set(int32_t val);

static void SetBuffering()
{
    static char buff[2048];
    memset( buff, '\0', sizeof(buff));
    if( setvbuf(stderr, buff, _IOLBF, sizeof(buff)) )
    {
        printf("SetBuffering: failed to change the buffer of stderr\n");
    }
    
    // make fgets not blocking 
    int flags = fcntl(stdin->_fileno, F_GETFL, 0); 
    fcntl(stdin->_fileno, F_SETFL, flags | O_NONBLOCK); 
}

#if defined(OEBUILD) || defined(SH4)
static int g_pfd[2] = {-1, -1}; /* Used to wake terminate thread and kbhit */
static int isPlaybackStarted = 0;
static pthread_mutex_t playbackStartMtx;

static int32_t g_windows_width = 1280;
static int32_t g_windows_height = 720;
static char *g_graphic_sub_path;

const char* GetGraphicSubPath()
{
    return g_graphic_sub_path;
}

int32_t GetGraphicWindowWidth()
{
    return g_windows_width;
}

int32_t GetGraphicWindowHeight()
{
    return g_windows_height;
}

void E2iSendMsg(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void E2iStartMsg(void)
{
    flockfile(stderr);
}

void E2iEndMsg(void)
{
    funlockfile(stderr);
}

static void TerminateWakeUp()
{
    int ret = write(g_pfd[1], "x", 1);
    if (ret != 1) {
        printf("TerminateWakeUp write return %d\n", ret);
    }
}

#endif

static int HandleTracks(const Manager_t *ptrManager, const PlaybackCmd_t playbackSwitchCmd, const char *argvBuff)
{
    int commandRetVal = 0;
    
    if (NULL == ptrManager || NULL == argvBuff || 2 != strnlen(argvBuff, 2))
    {
        return -1;
    }
    
    switch (argvBuff[1]) 
    {
        case 'l': 
        {
            TrackDescription_t *TrackList = NULL;
            ptrManager->Command(player, MANAGER_LIST, &TrackList);
            if( NULL != TrackList) 
            {
                int i = 0;
#if defined(OEBUILD) || defined(SH4)
//                E2iStartMsg();
//                E2iSendMsg("{\"%c_%c\": [", argvBuff[0], argvBuff[1]);
		fprintf(stderr, "{\"%c_%c\": [", argvBuff[0], argvBuff[1]);
#else
		fprintf(stderr, "{\"%c_%c\": [", argvBuff[0], argvBuff[1]);
#endif
                for (i = 0; TrackList[i].Id >= 0; ++i) 
                {
                    if(0 < i)
                    {
#if defined(OEBUILD) || defined(SH4)
                        E2iSendMsg(", ");
#else
			fprintf(stderr, ", ");
#endif
                    }
#if defined(OEBUILD) || defined(SH4)
//                   E2iSendMsg("{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}", TrackList[i].Id , TrackList[i].Encoding, TrackList[i].Name);
	            fprintf(stderr, "{\"%c_%c\": [", argvBuff[0], argvBuff[1]);
#else
		    fprintf(stderr, "{\"%c_%c\": [", argvBuff[0], argvBuff[1]);
#endif
                    free(TrackList[i].Encoding);
                    free(TrackList[i].Name);
                }
                E2iSendMsg("]}\n");
                E2iEndMsg();
                free(TrackList);
            }
            else
            {
                // not tracks 
#if defined(OEBUILD) || defined(SH4)
//                E2iSendMsg("{\"%c_%c\": []}\n", argvBuff[0], argvBuff[1]);
		fprintf(stderr, "{\"%c_%c\": []}\n", argvBuff[0], argvBuff[1]);
#else
		fprintf(stderr, "{\"%c_%c\": []}\n", argvBuff[0], argvBuff[1]);
#endif
            }
            break;
        }
        case 'c': 
        {
            
            TrackDescription_t *track = NULL;
            ptrManager->Command(player, MANAGER_GET_TRACK_DESC, &track);
            if (NULL != track) 
            {
                if ('a' == argvBuff[0] || 's' == argvBuff[0])
                {
#if defined(OEBUILD) || defined(SH4)
                    E2iSendMsg("{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}}\n", argvBuff[0], argvBuff[1], track->Id , track->Encoding, track->Name);
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}}\n", argvBuff[0], argvBuff[1], track->Id , track->Encoding, track->Name);
#else
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}}\n", argvBuff[0], argvBuff[1], track->Id , track->Encoding, track->Name);
#endif
                }
                else // video
                {
#if defined(OEBUILD) || defined(SH4)
                    E2iSendMsg("{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\",\"w\":%d,\"h\":%d,\"f\":%u,\"p\":%d,\"an\":%d,\"ad\":%d}}\n",
                    argvBuff[0], argvBuff[1], track->Id , track->Encoding, track->Name, track->width, track->height, track->frame_rate, track->progressive, track->aspect_ratio_num, track->aspect_ratio_den);
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\",\"w\":%d,\"h\":%d,\"f\":%u,\"p\":%d,\"an\":%d,\"ad\":%d}}\n", \
                    argvBuff[0], argvBuff[1], track->Id , track->Encoding, track->Name, track->width, track->height, track->frame_rate, track->progressive, track->aspect_ratio_num, track->aspect_ratio_den);
#else
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\",\"w\":%d,\"h\":%d,\"f\":%u,\"p\":%d,\"an\":%d,\"ad\":%d}}\n", \
                    argvBuff[0], argvBuff[1], track->Id , track->Encoding, track->Name, track->width, track->height, track->frame_rate, track->progressive, track->aspect_ratio_num, track->aspect_ratio_den);
#endif
                }
                free(track->Encoding);
                free(track->Name);
                free(track);
            }
            else
            {
                // no tracks
                if ('a' == argvBuff[0] || 's' == argvBuff[0])
                {
#if defined(OEBUILD) || defined(SH4)
					E2iSendMsg("{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}}\n", argvBuff[0], argvBuff[1], -1, "", "");
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}}\n", argvBuff[0], argvBuff[1], -1, "", "");
#else
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\"}}\n", argvBuff[0], argvBuff[1], -1, "", "");
#endif
                }
                else // video
                {
#if defined(OEBUILD) || defined(SH4)
					E2iSendMsg("{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\",\"w\":%d,\"h\":%d,\"f\":%u,\"p\":%d}}\n", argvBuff[0], argvBuff[1], -1, "", "", -1, -1, 0, -1);
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\",\"w\":%d,\"h\":%d,\"f\":%u,\"p\":%d}}\n", argvBuff[0], argvBuff[1], -1, "", "", -1, -1, 0, -1);
#else
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"e\":\"%s\",\"n\":\"%s\",\"w\":%d,\"h\":%d,\"f\":%u,\"p\":%d}}\n", argvBuff[0], argvBuff[1], -1, "", "", -1, -1, 0, -1);
#endif
                }
            }
            break;
        }
        default: 
        {
            /* switch command available only for audio and subtitle tracks */
            if ('a' == argvBuff[0] || 's' == argvBuff[0])
            {
                int ok = 0;
                int id = -1;
                if ('i' == argvBuff[1])
                {
                    int idx = -1;
                    ok = sscanf(argvBuff+2, "%d", &idx);
                    if (idx >= 0)
                    {
                        TrackDescription_t *TrackList = NULL;
                        ptrManager->Command(player, MANAGER_LIST, &TrackList);
                        if( NULL != TrackList) 
                        {
                            int i = 0;
                            for (i = 0; TrackList[i].Id >= 0; ++i) 
                            {
                                if (idx == i)
                                {
                                    id = TrackList[i].Id;
                                }
                                free(TrackList[i].Encoding);
                                free(TrackList[i].Name);
                            }
                            free(TrackList);
                        }
                    }
                    else
                    {
                        id = idx;
                    }
                }
                else
                {
                    ok = sscanf(argvBuff+1, "%d", &id);
                }
                
                if(id >= 0 || (1 == ok && id == -1))
                {
                    commandRetVal = player->playback->Command(player, playbackSwitchCmd, (void*)&id);
#if defined(OEBUILD) || defined(SH4)
					E2iSendMsg("{\"%c_%c\":{\"id\":%d,\"sts\":%d}}\n", argvBuff[0], 's', id, commandRetVal);
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"sts\":%d}}\n", argvBuff[0], 's', id, commandRetVal);
#else
				    fprintf(stderr, "{\"%c_%c\":{\"id\":%d,\"sts\":%d}}\n", argvBuff[0], 's', id, commandRetVal);
#endif
                }
            }
            break;
        }
    }
    
    return commandRetVal;
}


static void UpdateVideoTrack()
{
    HandleTracks(player->manager->video, (PlaybackCmd_t)-1, "vc");
}
#endif
#endif

#ifdef EPLAYER4
//#define GST_VERSION_MAJOR (0)
GstElement *pipeline = NULL;
gint m_framerate;
unsigned long long m_gst_startpts = 0;
CustomData data;
GstElement *video_sink = NULL;
//struct stimerthread* status.subtitlethread = NULL;
uint32_t buf_pos_ms = 0;
uint32_t duration_ms = 0;
int subtitleflag = 0;
char *subtext = NULL;
#else
//struct stimerthread* status.subtitlethread = NULL;
#endif
//titan player

//flag 0: from play
//flag 1: from timeshift
//flag 2: from playrcjumpr
int playerstartts(char* file, int flag)
{
	int fd = -1, ret = 0, tssize = 188;
	int16_t pmtpid = 0;
	int serviceid = 0;
	int supermagic = -1;
	int lastpos = 0;
	struct channel* chnode = NULL;
	struct service* snode = NULL;
	struct dvbdev* fenode = NULL;
	struct dvbdev* dvrnode = NULL;
	status.prefillbuffer = 0;
#ifdef EPLAYER4
	status.bufferpercent = 0;
#endif
	//supermagic = getsupermagic(file);
	printf("player--> playerstartts flag:%i\n", flag);
	addconfig("lastplayertype", "1");

	if(supermagic == NFS_SUPER_MAGIC || supermagic == SMB_SUPER_MAGIC)
	{
		debug(150, "use O_DIRECT to open file %s", file);
		fd = open(file, O_RDONLY | O_LARGEFILE | O_NONBLOCK | O_DIRECT);
	}
	else
		fd = open(file, O_RDONLY | O_LARGEFILE | O_NONBLOCK);

	if(fd < 0)
	{
		perr("open player file");
		return 1;
	}

	fenode = fegetdummy();
	dvrnode = dvropen(fenode);
	if(dvrnode == NULL)
	{
		err("find dvr dev");
		close(fd);
		return 1;
	}
	printf("player-> dvrnode: %i:%i\n", dvrnode->devnr, dvrnode->adapter);
	if(flag == 0 || flag == 2)
	{
		//TODO: funktion to get tssize from file content
		if(cmpfilenameext(file, ".mts") == 0) tssize = 192;
		if(cmpfilenameext(file, ".m2ts") == 0) tssize = 192;
		
		ret = dvbfindpmtpid(fd, &pmtpid, &serviceid, tssize);
		if(ret == 1)
		{
			err("find sid/pmt pid");
			close(fd);
			dvrclose(dvrnode, -1);
			return 1;
		}
		
		lastpos = 0;
		if(status.mcaktiv == 0 && flag == 0 && getconfigint("showlastpos", NULL) == 1)
		{ 
			char* fileseek = changefilenameext(file, ".se");
			FILE* fbseek = fopen(fileseek, "r");
			if(fbseek != NULL)
			{
//				ret = textbox(_("Message"), _("Start at last position ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 10, 0);
//printf("ret=%d\n", ret);
//				if(ret == 0 || ret == 1)
				if(textbox(_("Message"), _("Start at last position ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0) == 1)
				{
					char* skip1 = calloc(1, 20);
					if(skip1 != NULL)
					{
						fscanf(fbseek, "%s", skip1);
						off64_t seekpos = atoll(skip1);
						seekpos = seekpos - (seekpos % tssize);
						lseek64(fd, atoll(skip1), SEEK_SET);
						lastpos = 1;
					}
					free(skip1); skip1 = NULL;
				}
				fclose(fbseek);
			}
			free(fileseek); fileseek = NULL;
		}	
		
		status.autoseek = 0;
		
		if(flag == 0)
		{
			delmarkernode(-1);
			char* filemarker = changefilenameext(file, ".ma");
			getmarker(filemarker);
			free(filemarker); filemarker=NULL;
		}
		
		if(status.playmarker != NULL)
		{
			char* testfile = changefilenameext(file, ".as");
			FILE* testseek = fopen(testfile, "r");
			if(testseek != NULL)
			{
				if(lastpos == 0)
					lseek64(fd, status.playmarker->pos, SEEK_SET);
				status.autoseek = 2;
				addtimer(&markerautoseek_thread, START, 10000, 1, NULL, NULL, NULL);
				fclose(testseek);
			}
			free(testfile); testfile = NULL;
		}
		printf("player--> create channel\n");		
		delchannel(serviceid, 0, 1);
		chnode = createchannel("player", 0, 0, serviceid, 99, 0, -1, -1, -1, -1, 0, -1, NULL, NULL);
		if(chnode != NULL) chnode->pmtpid = pmtpid;
	}
	else
		chnode = status.aktservice->channel;

	if(chnode == NULL)
	{
		err("create channel");
		close(fd);
		dvrclose(dvrnode, -1);
		return 1;
	}

	if(flag == 1)
	{
		ret = servicestart(chnode, NULL, NULL, 2);
		if(ret != 0)
		{
			err("start play");
			close(fd);
			dvrclose(dvrnode, -1);
			return 1;
		}
		
		//on permanent timeshift seek to end, and a little back (eof problem)
		if(status.timeshifttype == 1)
		{
			if(status.timeshiftpos > 0)
				lseek64(fd, status.timeshiftpos, SEEK_SET);
			else
			{
				unsigned long long pos = lseek64(fd, 0, SEEK_END);
				pos -= 10000000;
				pos = pos - (pos & tssize);
				lseek64(fd, -pos, SEEK_END);
			}
		}
	}

	printf("player--> recordstartreal.. start\n");
	ret = recordstartreal(NULL, fd, dvrnode->fd, RECPLAY, 0, NULL, tssize);
	printf("player--> recordstartreal.. stop ret:%i\n",ret);
	if(ret != 0)
	{
		err("start play thread");
		close(fd);
		dvrclose(dvrnode, -1);
		return 1;
	}

	snode = getservice(RECORDPLAY, 0);
	if(snode != NULL)
	{
		int dupfd = -1;
		snode->recname = ostrcat(file, NULL, 0, 0);

		dupfd = open(snode->recname, O_RDONLY | O_LARGEFILE);
		if(dupfd > -1)
			gettsinfo(dupfd, &snode->lenpts, &snode->startpts, &snode->endpts, &snode->bitrate, snode->tssize);

		if(flag == 1)
		{
			snode->lenpts = 0;
			snode->endpts = 0;
		}
		else
		{
			if(getservicebyrecname(file, 1, 0) != NULL) //playfile is recording, so len can change
			{
				snode->lenpts = 0;
				snode->endpts = 0;
			}
			else if(dupfd > -1)
				snode->endoffile = lseek64(dupfd , 0, SEEK_END);
		}
		close(dupfd);
	}

	if(flag == 0 || flag == 2)
	{
		ret = servicestart(chnode, NULL, NULL, 1);
		if(ret != 0)
		{
			err("start play");
			if(snode != NULL) snode->recendtime = 1;
			close(fd);
			dvrclose(dvrnode, -1);
			return 1;
		}
		//status.playercan = 0x7EFF;
		status.playercan = 0xFFFF;	
	}

	return 0;
}

//flag 0: from play
//flag 1: from timeshift
//flag 2: from playrcjumpr/playerafterendts
//flag1 0: stop from rcstop
//flag1 1: stop from servicestop
void playerstopts(int flag, int flag1)
{
	int ret = 0;
	struct service* snode = NULL;
	struct channel* node = NULL;

	snode = getservice(RECORDPLAY, flag1);

	if(snode != NULL && snode->recsrcfd >= 0 && flag == 0 && flag1 == 0)
	{
		char* fileseek = changefilenameext(snode->recname, ".se");
		FILE* fbseek = fopen(fileseek, "w");
		if(fbseek != NULL)
		{
			off64_t pos = getcurrentpos(snode);
			if(pos <= 0)
				pos = lseek64(snode->recsrcfd, 0, SEEK_CUR);
			fprintf(fbseek,"%lld", pos);
			fclose(fbseek);
		}
		free(fileseek); fileseek=NULL;
		char* filemarker = changefilenameext(snode->recname, ".ma");
		ret = putmarker(filemarker);
		free(filemarker); filemarker=NULL;
		delmarkernode(-1);
	}
	
	if(snode != NULL) snode->recendtime = 1;
	
	if(flag == 0 || flag == 2)
	{
		playerslowts(0);
		playerffts(0);

		ret = servicestop(status.aktservice, 1, 1);
		if(ret == 1)
		{
			debug(150, "can't stop ts playback service");	
		}
		else
			status.aktservice->channel = NULL;

				
		node = gettmpchannel();
		if(node != NULL && ostrcmp(node->name, "player") == 0)
			delchannel(node->serviceid, node->transponderid, 1);
	}
}

void playerresetts()
{
#ifdef DREAMBOX
	videofreeze(status.aktservice->videodev);
	dmxstart(status.aktservice->dmxaudiodev);
	audioplay(status.aktservice->audiodev);
	audiopause(status.aktservice->audiodev);
#else
	audiostop(status.aktservice->audiodev);
	videostop(status.aktservice->videodev, 0);
#endif

#ifdef MIPSEL
	if(checkbox("DM7020HD") == 0 && checkbox("DM7020HDV2") == 0 && vubox1 == 0)
	{
		videoclearbuffer(status.aktservice->videodev);
		audioclearbuffer(status.aktservice->audiodev);
	}
#endif

#ifdef DREAMBOX
	videoslowmotion(status.aktservice->videodev, 0);
	videofastforward(status.aktservice->videodev, 0);
	videocontinue(status.aktservice->videodev);
	audiocontinue(status.aktservice->audiodev);
#else
	videoplay(status.aktservice->videodev);
	audioplay(status.aktservice->audiodev);
#endif
}

void playercontinuets()
{
	videocontinue(status.aktservice->videodev);
#ifdef MIPSEL
	if(checkchipset("HI3798MV200") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || vubox1 == 1)
		audioplay(status.aktservice->audiodev);
	audiocontinue(status.aktservice->audiodev);
#else
	audioplay(status.aktservice->audiodev);
#endif
}

void playerpausets()
{
	videofreeze(status.aktservice->videodev);
	audiopause(status.aktservice->audiodev);
}

//flag 0: with lock
//flag 1: without lock
int playerseekts(struct service* servicenode, int sekunden, int flag)
{
	off64_t offset = 0;
	off64_t endoffile = 0;
	off64_t currentpos = 0;
	//off64_t fdptspos = 0;
	//int ret = 0;
	unsigned long long lenpts = 0;
	unsigned long long startpts = 0;
	unsigned long long endpts = 0;
	unsigned long long bitrate = 0;
	//unsigned long long aktpts = 0;
	//unsigned long long fdpts = 0;
	//int aktsekunden = 0;
	int sekundenoff = 0;
	
	if(servicenode == NULL) return 1;

	if(servicenode->recsrcfd < 0)
	{
		err("source fd not ok");
		return 1;
	}
	
	if(flag == 0) m_lock(&status.tsseekmutex, 15);

/*
	ret = videogetpts(status.aktservice->videodev, &aktpts);
	if(ret == 0)
	{
		aktsekunden = aktpts / 90000;
	}
	else
		aktsekunden = 0;
	ret = getpts(servicenode->recsrcfd, 0, 0, 256 * 1024, &fdpts, &fdptspos, -1, servicenode->tssize);
	if(ret == 0 && aktsekunden != 0)
	{
		sekundenoff = fdpts / 90000 - aktsekunden ;
		//currentpos = lseek64(servicenode->recsrcfd, fdptspos, SEEK_SET);
	}
	else
		sekundenoff = 0;
*/
	
	currentpos = lseek64(servicenode->recsrcfd, 0, SEEK_CUR);

	lenpts = servicenode->lenpts;
	startpts = servicenode->startpts;
	endpts = servicenode->endpts;
	bitrate = servicenode->bitrate;
	if(gettsinfo(servicenode->recsrcfd, &lenpts, &startpts, &endpts, &bitrate, servicenode->tssize) != 0)
	{
		err("can't read ts info");
		lseek64(servicenode->recsrcfd, currentpos, SEEK_SET);
		if(flag == 0) m_unlock(&status.tsseekmutex, 15);
		return 1;
	}
	playerpausets();
	if(servicenode->endoffile > 0)
		endoffile = servicenode->endoffile - (servicenode->tssize * 2);
	else
		endoffile = lseek64(servicenode->recsrcfd , -servicenode->tssize * 2, SEEK_END);

/*
	ret = videoclearbuffer(status.aktservice->videodev);
	ret = audioclearbuffer(status.aktservice->audiodev);
	ret = videodiscontinuityskip(status.aktservice->videodev, 0);
*/

	if(sekunden >= 0)
	{
		if(sekundenoff != 0)
			offset = (bitrate / 8) * (sekunden - sekundenoff);
		else
			offset = (bitrate / 8) * sekunden - 5000000;
		offset = offset - (offset % servicenode->tssize);
		if(currentpos + offset > endoffile)
		{
			offset = endoffile - currentpos;
			offset = offset - (offset % servicenode->tssize);
		}
	}
	else
	{
		sekunden = sekunden * -1;
		if(sekundenoff != 0)
			offset = (bitrate / 8) * (sekunden + sekundenoff);
		else
			offset = (bitrate / 8) * sekunden;
		if(offset > 0) offset += 5000000;
		offset = offset - (offset % servicenode->tssize);
		if(currentpos - offset < 0)
			offset = currentpos;
		offset = offset * -1;
	}
	offset += currentpos;
	currentpos = lseek64(servicenode->recsrcfd, offset, SEEK_SET);
	
	if(checkbox("DM7020HD") != 1 && checkbox("DM7020HDV2") != 1 && vubox1 != 1)
		playerresetts();
	else
	{
		videoclearbuffer(status.aktservice->videodev);
		audioclearbuffer(status.aktservice->audiodev);
	}	
	playercontinuets();

	if(flag == 0) m_unlock(&status.tsseekmutex, 15);
	return 0;
}

void playerffts(int speed)
{
#ifdef MIPSEL
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || vubox1 == 1)
	{
		//audiopause(status.aktservice->audiodev);
		audiostop(status.aktservice->audiodev);
		dmxstop(status.aktservice->dmxaudiodev);
		videoslowmotion(status.aktservice->videodev, 0);
		videofastforward(status.aktservice->videodev, speed);
		videocontinue(status.aktservice->videodev);
		//audiocontinue(status.aktservice->audiodev);
	}
	else
	{
	audiostop(status.aktservice->audiodev);
#ifdef DREAMBOX 
	dmxstop(status.aktservice->dmxaudiodev);
#endif

	videoslowmotion(status.aktservice->videodev, 0);
	videofastforward(status.aktservice->videodev, speed);
	videocontinue(status.aktservice->videodev);
	}
#else	
	videofastforward(status.aktservice->videodev, speed);
#endif
}

void playerslowts(int speed)
{
#ifdef MIPSEL
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || vubox1 == 1)
	{
		audiostop(status.aktservice->audiodev);
		dmxstop(status.aktservice->dmxaudiodev);
		//audiopause(status.aktservice->audiodev);
		videoslowmotion(status.aktservice->videodev, speed);
		videofastforward(status.aktservice->videodev, 0);
		videocontinue(status.aktservice->videodev);
		//audiocontinue(status.aktservice->audiodev);
	}
	else
	{
	audiostop(status.aktservice->audiodev);
	
#ifdef DREAMBOX 
	dmxstop(status.aktservice->dmxaudiodev);
#endif	

	videoslowmotion(status.aktservice->videodev, speed);
	videofastforward(status.aktservice->videodev, 0);
	videocontinue(status.aktservice->videodev);
	}
#else		
	videoslowmotion(status.aktservice->videodev, speed);
#endif
}

//flag = 0 --> recordplay
//flag = 1 --> timeshift
void playerfrts(int speed, int flag)
{
	if(flag == 1)
		videocontinue(status.aktservice->videodev);
	if(speed == -2)
	{
		videoclearbuffer(status.aktservice->videodev);
		audioclearbuffer(status.aktservice->audiodev);
	}
	speed *= -1;
#ifdef MIPSEL
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || vubox1 == 1)
	{
		//audiopause(status.aktservice->audiodev);
		audiostop(status.aktservice->audiodev);
		dmxstop(status.aktservice->dmxaudiodev);
		videoslowmotion(status.aktservice->videodev, 0);
		videofastforward(status.aktservice->videodev, speed);
		videocontinue(status.aktservice->videodev);
		//audiocontinue(status.aktservice->audiodev);
	}
	else
	{
	audiostop(status.aktservice->audiodev);
#ifdef DREAMBOX 
	dmxstop(status.aktservice->dmxaudiodev);
#endif
	videoslowmotion(status.aktservice->videodev, 0);
	videofastforward(status.aktservice->videodev, speed);
	videocontinue(status.aktservice->videodev);
	}
#else	
	videofastforward(status.aktservice->videodev, speed);
#endif
}
	

//flag = 0 --> play ts
//flag = 1 --> timeshift
//flag = 2 --> timeshift, not in play mode (only recording)
int playergetinfots(unsigned long long* lenpts, unsigned long long* startpts, unsigned long long* endpts, unsigned long long* aktpts, unsigned long long* bitrate, int flag)
{
	int ret = 0, dupfd = -1;
	double ratio = 0;
	struct service* snode = NULL;
	unsigned long long lenpts1 = 0;
	unsigned long long startpts1 = 0;
	unsigned long long endpts1 = 0;
	unsigned long long bitrate1 = 0;
	unsigned long long endoffile1 = 0;
	unsigned long long aktpos = 0;
	
	if(flag == 2)
		snode = getservice(RECORDTIMESHIFT, 0);
	else
		snode = getservice(RECORDPLAY, 0);
		
	if(snode == NULL) return 1;

	if(snode->lenpts > 0 && snode->startpts > 0 && snode->endpts > 0 && snode->bitrate > 0 && snode->endoffile > 0)
	{
		if(lenpts != NULL) *lenpts = snode->lenpts;
		if(startpts != NULL) *startpts = snode->startpts;
		if(endpts != NULL) *endpts = snode->endpts;
		if(bitrate != NULL) *bitrate = snode->bitrate;

		//ret = videogetpts(status.aktservice->videodev, aktpts);
		if(aktpts != NULL)
		{
			m_lock(&status.tsseekmutex, 15);
			if(flag == 2)
				aktpos = lseek64(snode->recdstfd , 0, SEEK_CUR);
			else
				aktpos = lseek64(snode->recsrcfd , 0, SEEK_CUR);
			m_unlock(&status.tsseekmutex, 15);

			ratio = (double)snode->endoffile / (double)(snode->endpts - snode->startpts);
			if(ratio == 0) ratio = 1;
			*aktpts = ((double)aktpos / ratio);
			*aktpts += snode->startpts;
		}

		return ret;
	}
	
	dupfd = open(snode->recname, O_RDONLY | O_LARGEFILE);
	if(dupfd < 0)
	{
		err("copy source fd not ok");
		return 1;
	}

	lenpts1 = snode->lenpts;
	startpts1 = snode->startpts;
	endpts1 = snode->endpts;
	bitrate1 = snode->bitrate;
	if(gettsinfo(dupfd, &lenpts1, &startpts1, &endpts1, &bitrate1, snode->tssize) != 0)
	{
		err("can't read ts info");
		return 1;
	}

	if(lenpts != NULL) *lenpts = lenpts1;
	if(startpts != NULL) *startpts = startpts1;
	if(endpts != NULL) *endpts = endpts1;
	if(bitrate != NULL) *bitrate = bitrate1;

	//ret = videogetpts(status.aktservice->videodev, aktpts);
	if(aktpts != NULL)
	{
		m_lock(&status.tsseekmutex, 15);
		if(flag == 2)
			aktpos = lseek64(snode->recdstfd, 0, SEEK_CUR);
		else
			aktpos = lseek64(snode->recsrcfd, 0, SEEK_CUR);
		m_unlock(&status.tsseekmutex, 15);

		if(snode->endoffile <= 0)
			endoffile1 = lseek64(dupfd, 0, SEEK_END);
		else
			endoffile1 = snode->endoffile;

		if(endpts1 == 0)
			ratio = 1;
		else
			ratio = (double)endoffile1 / (double)(endpts1 - startpts1);

		if(ratio == 0) ratio = 1;
		*aktpts = ((double)aktpos / ratio);
		*aktpts += startpts1;
	}

	close(dupfd);
	return ret;
}

void playerchangeaudiotrackts()
{
	screenaudiotrack();
}

void playerchangesubtitletrackts()
{
	screensubtitle();
}

int playerisplayingts()
{
	struct service* snode = getservice(RECORDPLAY, 0);

	if(snode == NULL)
		return 0;
	return 1;
}

void playerafterendts()
{
	playerstopts(2, 0);
}

#ifdef EPLAYER4
void playersubtitleclean(char* data, int len)
{
	char* zeichen = NULL;
	int found = 1;
	
	while(found == 1)
	{
		found = 0;
		zeichen = NULL;
		zeichen = strstr(data, "&apos;"); 
		if(zeichen != NULL)
		{
			zeichen[0] = '\'';
			memcpy(zeichen+1, zeichen+6, len - (zeichen - data + 1));
			found = 1;
		}
		zeichen = NULL;
		zeichen = strstr(data, "&amp;"); 
		if(zeichen != NULL)
		{
			zeichen[0] = '&';
			memcpy(zeichen+1, zeichen+5, len - (zeichen - data + 1));
			found = 1;
		}
		zeichen = NULL;
		zeichen = strstr(data, "&quot;"); 
		if(zeichen != NULL)
		{
			zeichen[0] = '"';
			memcpy(zeichen+1, zeichen+6, len - (zeichen - data + 1));
			found = 1;
		}
		zeichen = NULL;
		zeichen = strstr(data, "&lt;"); 
		if(zeichen != NULL)
		{
			zeichen[0] = '<';
			memcpy(zeichen+1, zeichen+4, len - (zeichen - data + 1));
			found = 1;
		}
		zeichen = NULL;
		zeichen = strstr(data, "&gt;"); 
		if(zeichen != NULL)
		{
			zeichen[0] = '<';
			memcpy(zeichen+1, zeichen+4, len - (zeichen - data + 1));
			found = 1;
		}
		//workaround da keine Aufbereitung
		zeichen = NULL;
		zeichen = strstr(data, "<i>"); 
		if(zeichen != NULL)
		{
			memcpy(zeichen, zeichen+3, len - (zeichen - data + 1));
			found = 1;
		}
		zeichen = NULL;
		zeichen = strstr(data, "</i>"); 
		if(zeichen != NULL)
		{
			memcpy(zeichen, zeichen+4, len - (zeichen - data + 1));
			found = 1;
		}
	}
}
#endif

#ifndef SIMULATE
#ifdef EPLAYER4
void playersubtitle_gst_thread()
{
	debug(300, "start");

	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* subtitle = getscreen("extplayer_subtitle");
	char* bg = NULL;
	int count = 0;
	
	subtitle->bgcol = -1;

//	setnodeattr(subtitle, framebuffer, 0);
//	bg = savescreen(subtitle);

	while(status.subtitlethread != NULL && status.subtitlethread->aktion != STOP)
	{
		debug(300, "start while1");

		if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL)
			goto subend;

		if(duration_ms != 0)
		{
			count = 0;
//new in
			if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL)
				goto subend;

			if(ostrstr(subtext, "<i>") != NULL)
			{
				string_deltags(subtext);
				string_decode(subtext, 1);

				subtitle->fontcol = convertcol("yellow");
			}
			else
				subtitle->fontcol = convertcol("white");
//new out
			changetext(subtitle, subtext);
//			count = duration_ms / 100;
			count = duration_ms / 90;
			debug(300, "status.writeplayersub=%d count=%d send duration_ms=%d subtext: %s", status.writeplayersub, count, duration_ms, subtext);

			if(status.writeplayersub == 1)
				drawscreen(subtitle, 0, 0);
			while(count > 0 && status.subtitlethread != NULL && status.subtitlethread->aktion != STOP)
			{
				debug(300, "while2 duration count: %d == 0", count);

				if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL)
					goto subend;
				usleep(100000);
				count = count - 1;
			}
subend:
			duration_ms = 0;
			if(status.subtitlethread != NULL)
				status.subtitlethread->aktion = STOP;
		}
		else
			usleep(100000);
	}
	free(subtext); subtext = NULL;

//	if(status.writeplayersub == 1)
//	{
//		restorescreen(bg, subtitle);
//		blitfb(0);
//	}
	if(status.subtitlethread != NULL)
		status.subtitlethread = NULL;
	debug(300, "end");
}
#endif
#ifdef EXTEPLAYER3
void playersubtitle_ext_thread(struct stimerthread* timernode, char* input, int flag)
{
	debug(300, "start");

	uint32_t sub_duration_ms = 0;
	uint32_t sub_pts_ms = 0;
	char *sub_text = NULL;
	int sub_pts_sec = 0;
	int sub_duration_sec = 0;

	char* sub_duration = oregex(".*duration=(.*);pts=.*", input);

	if(sub_duration != NULL)
	{
		sub_duration_ms = atoi(sub_duration);
		sub_duration_sec = sub_duration_ms / 1000 + 1;
	}

	char* sub_pts = oregex(".*;pts=(.*);trackid=.*", input);

	if(sub_pts != NULL)
	{
		sub_pts_ms = atoi(sub_pts);
		sub_pts_sec = sub_pts_ms / 90000;
	}

	char* sub_trackid = oregex(".*;trackid=(.*);subtext.*", input);
	sub_text = oregex(".*;subtext=(.*).*", input);

	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* subtitle = getscreen("extplayer_subtitle");
	char* bg = NULL;
	int count = 0;

	subtitle->bgcol = -1;

//	setnodeattr(subtitle, framebuffer, 0);
//	bg = savescreen(subtitle);

	while(status.subtitlethread != NULL && status.subtitlethread->aktion != STOP)
	{
		if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL)
			goto subend;

		if(sub_duration_ms != 0)
		{
			int64_t pts = 0;
			int sec = 0;

			if(player && player->playback)
			{
				player->playback->Command(player, PLAYBACK_PTS, &pts);
				sec = pts / 90000;
			}

			while(sec < sub_pts_sec && status.subtitlethread != NULL && status.subtitlethread->aktion != STOP)
			{
				debug(300, "while1 subpts=pts count: %d == %d", sec, sub_pts_sec);

				if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL && status.subtitlethread != NULL)
					goto subend;

				sleep(1);
				sec++;
			}

			count = 0;
//new in
			if(ostrstr(sub_text, "<i>") != NULL)
			{
				string_deltags(sub_text);
				string_decode(sub_text, 1);
				subtitle->fontcol = convertcol("yellow");
			}
			else
				subtitle->fontcol = convertcol("white");
//new out

			changetext(subtitle, sub_text);

//			printf("send sub_duration_ms=%d sub_duration_sec=%d sub_text: %s\n",sub_duration_ms, sub_duration_sec , sub_text);
			debug(150, "send sub_duration_ms=%d sub_duration_sec=%d sub_text: %s",sub_duration_ms, sub_duration_sec , sub_text);

			count = sub_duration_ms;
			
			if(status.writeplayersub == 1)
				drawscreen(subtitle, 0, 0);

			while(count > 0 && status.subtitlethread != NULL && status.subtitlethread->aktion != STOP)
			{
				debug(300, "while2 duration count: %d == 0", count);

				if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL)
				{
					goto subend;
				}

				usleep(100000);
				count = count - 1;
			}
subend:
			sub_duration_ms = 0;
// crash sometimes
			if(status.subtitlethread != NULL)
				status.subtitlethread->aktion = STOP;
		}
		else
			usleep(100000);
	}

//	if(status.writeplayersub == 1)
//	{
//		restorescreen(bg, subtitle);
//		blitfb(0);
//	}

	free(sub_text); sub_text = NULL;
	if(status.subtitlethread != NULL)
		status.subtitlethread = NULL;

	debug(300, "end");
}
#endif
#endif

#ifdef EPLAYER4
void playersubtitleAvail(GstElement *subsink, GstBuffer *buffer, gpointer user_data)
{
//	printf("++++++ subtitelflag: %i\n", subtitleflag);
	if(subtitleflag == 0 || subtitleflag == 2) return;

#if GST_VERSION_MAJOR < 1
	gint64 buf_pos = GST_BUFFER_TIMESTAMP(buffer);
	gint64 duration_ns = GST_BUFFER_DURATION(buffer);
#endif	
	time_t running_pts = 0;
	gint64 pos = 0;
	int32_t decoder_ms;
	GstFormat fmt = GST_FORMAT_TIME;

#if GST_VERSION_MAJOR < 1
	if (!gst_element_query_position(pipeline, &fmt, &pos))
#else
	if (!gst_element_query_position(pipeline, fmt, &pos))
#endif
	{
		err("gst_element_query_position failed");
		return;
	}
	running_pts = pos / 11111LL;
	decoder_ms = running_pts / 90;
		
	if(status.subtitlethread == NULL)
		status.subtitlethread = addtimer(&playersubtitle_gst_thread, START, 10000, 1, NULL, NULL, NULL);

    if (!buffer)
        return;

    GstMapInfo map;
    if (!gst_buffer_map(buffer, &map, GST_MAP_READ))
    {
        gst_buffer_unref(buffer);
        return;
    }

    if (GST_BUFFER_PTS_IS_VALID(buffer) && GST_BUFFER_DURATION_IS_VALID(buffer))
    {
        gchar *data = g_strndup((const gchar *) map.data, map.size);
		subtext = ostrcat(data, NULL, 0, 0);
    }

	gint64 buf_pos = GST_BUFFER_PTS(buffer);
	gint64 duration_ns = GST_BUFFER_DURATION(buffer);



//	while(duration_ms != 0 && status.subtitlethread != NULL)
	while(duration_ms != 0 && status.subtitlethread->aktion != STOP)
	{
		debug(300, "while3 duration count: %d == 0", duration_ms);
		usleep(100000);
	}


//old
/*
	size_t len = map.size;
//	size_t len = GST_BUFFER_SIZE(buffer);

	if(subtext != NULL)
		free(subtext);
	subtext = malloc(len+10);
	if(subtext == NULL)
	{
		err("no mem");
		return;
	}
#if GST_VERSION_MAJOR < 1
	printf("playersubtitleAvail skip sprintf data\n");
#else
	guint8 *data;
//	gsize size;
//	GstMapInfo map;
//	gst_buffer_map(buffer, &map, GST_MAP_READ);
	data = map.data;
	sprintf(subtext, "%s", data);
//	sprintf(subtext, "%s", GST_BUFFER_DATA(buffer));

// test write subs data as png
//	char* picfilename = NULL;
//	picfilename = ostrcat("/tmp/sub.", oitoa(time(NULL)), 0, 1);
//	picfilename = ostrcat(picfilename, ".png", 1, 0);

	sprintf(subtext, "%s", map.data);
//	writesys(picfilename, subtext, 0);

#endif

	playersubtitleclean(subtext, len+10);
*/
//old

	double convert_fps = 1.0;
	buf_pos_ms  = (buf_pos / 1000000ULL) * convert_fps;
	duration_ms = duration_ns / 1000000ULL;

	//printf("++++++ buff_pos  : %u\n", buf_pos_ms);
//	printf("++++++ decoder_ms: %i\n", decoder_ms);
}
#endif

#ifdef EPLAYER4
// set http extra-header and user-agent
void playbinNotifySource(GObject *object, GParamSpec *unused, char* file)
{
	printf("[player.h] playbinNotifySource: %s\n", file);
	GstElement *source = NULL;
	g_object_get(object, "source", &source, NULL);
	if (source)
	{
		if (g_object_class_find_property(G_OBJECT_GET_CLASS(source), "cookiejar-file-name") != 0)
		{
			printf("[player.h] set cookiejar-file-name: /mnt/network/cookies\n");
			g_object_set(G_OBJECT(source), "cookiejar-file-name", "/mnt/network/cookies", NULL);
		}

		if(ostrstr(file, "|") != NULL)
		{
			if (g_object_class_find_property(G_OBJECT_GET_CLASS(source), "extra-headers") != 0)
			{
	#if GST_VERSION_MAJOR < 1
				GstStructure *extras = gst_structure_empty_new("extras");
	#else
				GstStructure *extras = gst_structure_new_empty("extras");
	#endif
				char* tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *tmpstr4 = NULL;
				tmpstr1 = ostrcat(file, NULL, 0, 0);
				int count1 = 0, i = 0;
				struct splitstr* ret1 = NULL;
				ret1 = strsplit(tmpstr1, "|", &count1);
	
				for(i = 0; i < count1; i++)
				{
					if(i == 0)
					{
						printf("[player.h] playbinNotifySource: skip url string: %s\n", ret1[i].part);
						continue;
					}
					tmpstr2 = ostrcat(ret1[i].part, NULL, 0, 0);
	
					int count2 = 0, i2 = 0;
					struct splitstr* ret2 = NULL;
					ret2 = strsplit(tmpstr2, "&", &count2);

					if(ret2 != NULL)
					{
						for(i2 = 0; i2 < count2; i2++)
						{
							int count3 = 0, i3 = 0;	
							struct splitstr* ret3 = NULL;
							tmpstr3 = ostrcat(ret2[i2].part, NULL, 0, 0);
							ret3 = strsplit(tmpstr3, "=", &count3);
					
							if(ret3 != NULL)
							{
								int max = count3 - 1;
								for(i3 = 0; i3 < max; i3++)
								{
									if(ostrstr(ret3[i3].part, "User-Agent") != NULL)
									{
			//							printf("[player.h] skip set user-agent: %s\n", ret2[1].part);
										printf("[player.h] set user-agent: %s\n", ret3[i3 + 1].part);
										g_object_set(G_OBJECT(source), "user-agent", ret3[i3 + 1].part, NULL);
									}
									else
									{
										if (g_object_class_find_property(G_OBJECT_GET_CLASS(source), "extra-headers") != 0)
										{					
											GValue header;
											// eDebug("setting extra-header '%s:%s'", name.c_str(), value.c_str());
											printf("[player.h] set extra-header %s: %s\n", ret3[i3].part, ret3[i3 + 1].part);
											
											tmpstr4 = ostrcat(ret3[i3 + 1].part, NULL, 0, 0);
											htmldecode(tmpstr4, tmpstr4);
											printf("[player.h] set extra-header decode %s: %s\n", ret3[i3].part, tmpstr4);
			
											memset(&header, 0, sizeof(GValue));
											g_value_init(&header, G_TYPE_STRING);
											//value
											g_value_set_string(&header, tmpstr4);
											//name
											gst_structure_set_value(extras, ret3[i3].part, &header);
											free(tmpstr4), tmpstr4 = NULL;
										}
									}
								}
							}
							free(ret3), ret3 = NULL;
							free(tmpstr3), tmpstr3 = NULL;
						}
					}
					free(ret2), ret2 = NULL;
					free(tmpstr2), tmpstr2 = NULL;
				}

				free(ret1), ret1 = NULL;
				free(tmpstr1), tmpstr1 = NULL;

				if (gst_structure_n_fields(extras) > 0)
				{
					g_object_set(G_OBJECT(source), "extra-headers", extras, NULL);
				}
				gst_structure_free(extras);
			}
		}

		if (g_object_class_find_property(G_OBJECT_GET_CLASS(source), "ssl-strict") != 0)
		{
			printf("[player.h] set ssl-strict\n");
			g_object_set(G_OBJECT(source), "ssl-strict", FALSE, NULL);
		}
	}
	gst_object_unref(source);
}
#endif

//extern player
int playerstart(char* file)
{
	char * tmpfile = NULL;
	status.prefillbuffer = 0;
	int sec = 0;
	
	addconfig("lastplayertype", "0");
	
	free(status.actplay);
	status.actplay = ostrcat(file, NULL, 0, 0);
	
	status.subtitlethread = NULL;
	status.writeplayersub = 1;
	 
	if(status.mcaktiv == 0 && status.actplay != NULL && getconfigint("showlastpos", NULL) == 1)
	{ 
			char* fileseek = changefilenameext(file, ".pts");
			FILE* fbseek = fopen(fileseek, "r");
			if(fbseek != NULL)
			{
				if(textbox(_("Message"), _("Start at last position ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0) == 1)
				{
					char* skip1 = calloc(1, 20);
					if(skip1 != NULL)
					{
						fscanf(fbseek, "%s", skip1);
						off64_t seekpos = atoll(skip1);
						sec = seekpos / 90000;
						printf("++++ seek sec: %d\n", sec);
#if defined (EXTGST)
						if(getconfigint("extplayer_type", NULL) == 1)
						{
#endif

#ifdef EPLAYER4
							sec = sec * 2;	
#endif				
#if defined (EXTGST)
						}
#endif

					}
					free(skip1); skip1 = NULL;
				}
				fclose(fbseek);
			}
			free(fileseek); fileseek = NULL;
	}	

#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 1)
	{
#endif	

	#ifdef EPLAYER4
		status.prefillbuffercount = 0;
		status.bufferpercent = 0;
	#endif

#if defined (EXTGST)
	}
#endif
	if(file != NULL)
	{
#if defined (EXTGST)
		if(getconfigint("extplayer_type", NULL) == 0)
		{
#endif
	#ifdef EPLAYER3
			//use eplayer

			if(player != NULL)
			{
				debug(150, "eplayer allready running");
				playerstop();
			}

			player = calloc(1, sizeof(Context_t));

			if(player == NULL)
			{
				err("no mem");
				return 1;
			}

			if(ostrstr(file, "://") == NULL)
				tmpfile = ostrcat("file://", file, 0, 0);
			else
				tmpfile = ostrcat(file, NULL, 0, 0);

			if(tmpfile == NULL)
			{
				err("no mem");
				free(player); player = NULL;
				return 1;
			}
	// move to mc
	//		set_player_sound(0);

			if(ostrstr(tmpfile, "file://") == NULL)
				status.playercan = 0x4650;
			else
				status.playercan = 0xFFFF;

			player->playback = &PlaybackHandler;
			player->output = &OutputHandler;
			player->container = &ContainerHandler;
			player->manager = &ManagerHandler;

	//#ifndef EXTEPLAYER3		
			//add container befor open, so we can set buffer size
			char* extffm = getfilenameext(tmpfile);
			if(extffm != NULL)
			{
				player->container->Command(player, CONTAINER_ADD, extffm);
				free(extffm); extffm = NULL;
			}

			//select container_ffmpeg, if we does not found a container with extensions
			if(player->container->selectedContainer == NULL)
				player->container->Command(player, CONTAINER_ADD, "mp3");
	//#endif

			if(player && player->container && player->container->selectedContainer)
			{
		#ifndef EXTEPLAYER3
				int32_t size = getconfigint("playerbuffersize", NULL);
				int32_t seektime = getconfigint("playerbufferseektime", NULL);
		#else
				int32_t* size = (int32_t*)getconfigint("playerbuffersize", NULL);
				int32_t* seektime = (int32_t*)getconfigint("playerbufferseektime", NULL);

				if(strstr(tmpfile, "http://") == tmpfile || strstr(tmpfile, "https://") == tmpfile)
					progressive_playback_set(1);

	//			if(ostrcmp(getconfig("av_ac3mode", NULL), "downmix") == 0)
			#ifndef MIPSEL
				int tmpdownmix = 0;
				char* downmix = readfiletomem(getconfig("ac3dev", NULL), 1);
	//			printf("ac3dev: %s\n",getconfig("ac3dev", NULL));
				debug(150, "ac3dev=%s", getconfig("ac3dev", NULL));

	//			printf("downmix: %s\n",downmix);
				debug(150, "downmix=%s", downmix);

	//			if(ostrcmp(downmix, "downmix") == 0)
				if(ostrstr(downmix, "downmix") != NULL)
					tmpdownmix = 1;
				else
					tmpdownmix = 0;

				debug(150, "tmpdownmix=%d", tmpdownmix);

				free(downmix), downmix = NULL;
				if(tmpdownmix != status.downmix)
				{
					debug(150, "change downmix=%d to downmix=%d", status.downmix, tmpdownmix);
				}

	//			printf("status.downmix: %d\n",status.downmix);
				debug(150, "tmpdownmix=%d", tmpdownmix);
				debug(150, "status.downmix=%d", status.downmix);

				if(tmpdownmix == 1)
				{
					debug(150, "enable dts downmix");
					dts_software_decoder_set(1);
					stereo_software_decoder_set(1);
				}
			#endif
//			container_set_ffmpeg_buf_size(size);
		#endif

				player->container->selectedContainer->Command(player, CONTAINER_SET_BUFFER_SIZE, (void*)&size);
				player->container->selectedContainer->Command(player, CONTAINER_SET_BUFFER_SEEK_TIME, (void*)&seektime);			
			}
			
			debug(150, "eplayername = %s", player->output->Name);
		#ifdef EXTEPLAYER3
			// make sure to kill myself when parent dies
			prctl(PR_SET_PDEATHSIG, SIGKILL);

			SetBuffering();
		#endif
			//Registrating output devices
			player->output->Command(player, OUTPUT_ADD, "audio");
			player->output->Command(player, OUTPUT_ADD, "video");
			player->output->Command(player, OUTPUT_ADD, "subtitle");
		#if defined(OEBUILD) || defined(SH4)
	//		int32_t* size = (int32_t*)getconfigint("playerbuffersize", NULL);
	//printf("size: %d\n", size);
	//size = 10485760;
	//printf("size2: %d\n", size);

	//		player->output->Command(player, OUTPUT_SET_BUFFER_SIZE, (void*)&size);
		#endif
		#ifndef EXTEPLAYER3
			//for subtitle
	//		SubtitleOutputDef_t subout;

	//		subout.screen_width = skinfb->width;
	//		subout.screen_height = skinfb->height;
	//		subout.framebufferFD = skinfb->fd;
	//		subout.destination = (uint32_t *)skinfb->fb;
	//		subout.destStride = skinfb->pitch;
	//		subout.shareFramebuffer = 1;
	//		subout.framebufferBlit = blitfb1;

	//		player->output->subtitle->Command(player, (OutputCmd_t)OUTPUT_SET_SUBTITLE_OUTPUT, (void*)&subout);

			if(player->playback->Command(player, PLAYBACK_OPEN, tmpfile) < 0)
			{
				free(player); player = NULL;
				free(tmpfile);
				return 1;
			}
		#else

	//uint32_t linuxDvbBufferSizeMB = 1024*1024*10;
	//		player->output->Command(player, OUTPUT_SET_BUFFER_SIZE, &linuxDvbBufferSizeMB);

			PlaybackDieNowRegisterCallback(TerminateWakeUp);
			player->manager->video->Command(player, MANAGER_REGISTER_UPDATED_TRACK_INFO, UpdateVideoTrack);
			if (strncmp(file, "rtmp", 4) && strncmp(file, "ffrtmp", 4))
			{
			    player->playback->noprobe = 1;
			}
		
			char* audioFile = NULL;
			PlayFiles_t playbackFiles = {tmpfile, NULL};
			if('\0' != audioFile[0])
			{
			    playbackFiles.szSecondFile = audioFile;
			}

			//for subtitle
	//		SubtitleOutputDef_t subout;

	//		subout.screen_width = skinfb->width;
	//		subout.screen_height = skinfb->height;
	//		subout.framebufferFD = skinfb->fd;
	//		subout.destination = (uint32_t *)skinfb->fb;
	//		subout.destStride = skinfb->pitch;
	//		subout.shareFramebuffer = 1;
	//		subout.framebufferBlit = blitfb1;

	//		player->output->subtitle->Command(player, (OutputCmd_t)OUTPUT_SET_SUBTITLE_OUTPUT, (void*)&subout);

			if(player->playback->Command(player, PLAYBACK_OPEN, &playbackFiles) < 0)
			{
				free(player); player = NULL;
				free(tmpfile);
				return 1;
			}
		#endif
			player->output->Command(player, OUTPUT_OPEN, NULL);
			player->playback->Command(player, PLAYBACK_PLAY, NULL);

			free(tmpfile);
	 
	 		if(sec > 0)
	 		{
	 			usleep(100000);
	 			playerseek(sec);
	 		}
			return 0;
	#endif
#if defined (EXTGST)
		}
		else
		{
#endif
	#ifdef EPLAYER4
			int flags = 0x47; //(GST_PLAY_FLAG_VIDEO | GST_PLAY_FLAG_AUDIO | GST_PLAY_FLAG_NATIVE_VIDEO | GST_PLAY_FLAG_TEXT);
			
			if(pipeline != NULL)
			{
				debug(150, "eplayer allready running");
				playerstop();
			}
			
			if(ostrstr(file, "://") == NULL)
				tmpfile = ostrcat("file://", file, 0, 0);
			else
				tmpfile = ostrcat(file, NULL, 0, 0);

			printf("[player.h] file: %s\n", file);

			if(tmpfile == NULL)
			{
				err("no mem");
				free(pipeline); pipeline = NULL;
				return 1;
			}

			if(ostrstr(tmpfile, "file://") == NULL)
				//status.playercan = 0x7E7F;
				//status.playercan = 0x7EFF;
				status.playercan = 0xFEFF;
			else
				//status.playercan = 0x7E7F;
				//status.playercan = 0x7EFF;
				status.playercan = 0xFEFF;
			
			m_framerate = -1;
	#if GST_VERSION_MAJOR < 1
			pipeline = gst_element_factory_make("playbin2", "playbin");
	#else
			pipeline = gst_element_factory_make("playbin", "playbin");
	#endif

	// enable buffersize start
			int size = getconfigint("playerbuffersize", NULL);
			printf("size: %d\n",size);
			
			if(size > 0 && ostrstr(tmpfile, "file://") == NULL)
				status.prefillbuffer = 1;

	/*
		    if (g_object_class_find_property(G_OBJECT_GET_CLASS(pipeline), "user-agent") != 0)
				printf("11111111111111\n");
		    if (g_object_class_find_property(G_OBJECT_GET_CLASS(pipeline), "cookie") != 0)
				printf("22222222222222\n");
		    if (g_object_class_find_property(G_OBJECT_GET_CLASS(pipeline), "extra-headers") != 0)
				printf("33333333333333\n");

			if(ostrstr(file, "|User-Agent=") != NULL || ostrstr(file, "|Cookie=") != NULL || ostrstr(file, "|Referer=") != NULL)
			{
				char* tmpstr = NULL, *tmpstr1 = NULL;
				tmpstr = ostrcat(file, NULL, 0, 0);
	//			tmpstr = string_replace("|User-Agent=", "|", tmpstr, 1);
				int count1 = 0, i = 0;
				struct splitstr* ret1 = NULL;
				ret1 = strsplit(tmpstr, "|", &count1);

				int max = count1;
				for(i = 0; i < max; i++)
				{
					if(ostrstr(ret1[i].part, "User-Agent=") != NULL)
					{
						tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
						tmpstr1 = string_replace("User-Agent=", "", tmpstr1, 1);
						printf("[player.h] set user-agent: %s\n", tmpstr1);
						g_object_set(G_OBJECT(pipeline), "user-agent", tmpstr1, NULL);
						free(tmpstr1), tmpstr1 = NULL;
					}
					if(ostrstr(ret1[i].part, "Cookie=") != NULL)
					{
						tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
						tmpstr1 = string_replace("Cookie=", "", tmpstr1, 1);
						printf("[player.h] set cookie: %s\n", tmpstr1);

						gchar **cookie;
	//					cookie = g_strsplit ("foo=1234,bar=9871615348162523726337x99FB", ",", -1);
						cookie = g_strsplit (tmpstr1, ",", -1);
						g_object_set (G_OBJECT(pipeline), "http-headers", cookie, NULL);
						g_strfreev (cookie);
						free(tmpstr1), tmpstr1 = NULL;
					}
					if(ostrstr(ret1[i].part, "Referer=") != NULL)
					{
						tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
						tmpstr1 = string_replace("Referer=", "", tmpstr1, 1);
						printf("[player.h] set referer dummy: %s\n", tmpstr1);
						free(tmpstr1), tmpstr1 = NULL;
					}
				}
				free(ret1), ret1 = NULL;
				free(tmpstr), tmpstr = NULL;
	//			g_object_set(G_OBJECT(pipeline), "user-agent", "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:30.0) Gecko/20100101 Firefox/30.0", NULL);
				stringreplacechar(tmpfile, '|', '\0');
				printf("tmpfile changed: %s\n", tmpfile);
			}
	*/

	// strip url
			stringreplacechar(tmpfile, '|', '\0');

			g_object_set(G_OBJECT(pipeline), "buffer-duration", size * GST_SECOND, NULL);
			g_object_set(G_OBJECT(pipeline), "buffer-size", size, NULL);
	// enable buffersizeend

			g_object_set(G_OBJECT(pipeline), "uri", tmpfile, NULL);
			g_object_set(G_OBJECT(pipeline), "flags", flags, NULL);
			free(tmpfile); tmpfile = NULL;

	///////////////////
	// srt subs start
			const char *filename = file;
			const char *ext = strrchr(filename, '.');
			if (!ext)
				ext = filename + strlen(filename);

			GstElement *subsink = gst_element_factory_make("subsink", "subtitle_sink");
			if (!subsink)
				printf("sorry, can't play: missing gst-plugin-subsink\n");
			else
			{
	//			m_subs_to_pull_handler_id = g_signal_connect (subsink, "new-buffer", G_CALLBACK (gstCBsubtitleAvail), this);
				g_signal_connect (subsink, "new-buffer", G_CALLBACK (playersubtitleAvail), NULL);

				g_object_set (G_OBJECT (subsink), "caps", gst_caps_from_string("text/plain; text/x-plain; text/x-raw; text/x-pango-markup; video/x-dvd-subpicture; subpicture/x-pgs"), NULL);

	#if GST_VERSION_MAJOR < 1
				g_object_set (G_OBJECT (subsink), "caps", gst_caps_from_string("text/plain; text/x-plain; text/x-raw; text/x-pango-markup; video/x-dvd-subpicture; subpicture/x-pgs"), NULL);
	#else
				g_object_set (G_OBJECT (subsink), "caps", gst_caps_from_string("text/plain; text/x-plain; text/x-raw; text/x-pango-markup; subpicture/x-dvd; subpicture/x-pgs"), NULL);
	#endif

				g_object_set (G_OBJECT (pipeline), "text-sink", subsink, NULL);
				subtitleflag = 1;
				//g_object_set (G_OBJECT (pipeline), "current-text", -1, NULL);
			}

	//////////////////////////
	// enable soup user-agent / extra-headers
			g_signal_connect(G_OBJECT(pipeline), "notify::source", G_CALLBACK(playbinNotifySource), file);
	//////////////////////////

			printf("[player.h] file changed: %s\n", file);

	//gpointer this;
	//memset (&this, 0, sizeof (this));

			GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	#if GST_VERSION_MAJOR < 1
	//		gst_bus_set_sync_handler(bus, gstBusSyncHandler, this);
			gst_bus_set_sync_handler(bus, GST_BUS_DROP, NULL);
	#else
	//		gst_bus_set_sync_handler(bus, gstBusSyncHandler, this, NULL);
			gst_bus_set_sync_handler(bus, GST_BUS_DROP, NULL, NULL);
	#endif

			gst_object_unref(bus);
			char srt_filename[ext - filename + 5];
			strncpy(srt_filename,filename, ext - filename);
			srt_filename[ext - filename] = '\0';
			strcat(srt_filename, ".srt");

			if(access(srt_filename, R_OK) >= 0)
			{
				printf("found srt1: %s\n",srt_filename);
				printf("found srt2: %s\n",g_filename_to_uri(srt_filename, NULL, NULL));
				g_object_set(G_OBJECT (pipeline), "suburi", g_filename_to_uri(srt_filename, NULL, NULL), NULL);	
			}
	// srt end	

	///////////////////
	//		CustomData data;
			memset (&data, 0, sizeof (data));
			data.pipeline = pipeline;
	//		GstBus *bus;
	//		bus = gst_element_get_bus (pipeline);
			
			// Start playing //

			GstStateChangeReturn ret;
			ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
			g_object_set (G_OBJECT (pipeline), "current-text", 0, NULL);
			if(ret == GST_STATE_CHANGE_FAILURE)
			{
				g_printerr ("Unable to set the pipeline to the playing state.\n");
				gst_object_unref (pipeline);
				return -1;
			}
			else if(ret == GST_STATE_CHANGE_NO_PREROLL)
			{
				data.is_live = TRUE;
			}

			data.loop = g_main_loop_new (NULL, FALSE);
			data.pipeline = pipeline;
			gst_bus_add_signal_watch (bus);
	//		g_signal_connect (bus, "message", G_CALLBACK (cb_message), &data);
	//		status.prefillbuffer = 1;

	//analyze_streams(data);

			int count = 0;
			m_gst_startpts = 0;
			while(m_gst_startpts == 0 && count < 5)
			{
				count++;
				sleep(1);
				m_gst_startpts = playergetpts();
			}

			if(sec > 0)
			{
				usleep(100000);	
				playerseek(sec); 
			}

			return 0;
	#endif
#if defined (EXTGST)
		}
#endif
	}
	
	return 1;
}

#ifdef EPLAYER4
int setBufferSize(int size)
{
	int m_buffer_size = size;
	g_object_set (G_OBJECT (pipeline), "buffer-size", m_buffer_size, NULL);
	return 0;
}
#endif

void playerinit(int argc, char* argv[])
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 1)
	{
#endif
	#ifdef EPLAYER4
	//	GstBus *bus;
	//	GstStateChangeReturn ret;
	//	gint flags;
		gst_init(&argc, &argv);
	#endif
#if defined (EXTGST)
	}
#endif
}

#ifdef EPLAYER4
int gstbuscall(GstBus *bus, GstMessage *msg, CustomData *data)
{
	int ret = 1;
	if(!pipeline) return 0;
	if(!msg) return ret;

	gchar *sourceName = NULL;
	GstObject *source = GST_MESSAGE_SRC(msg);

	if(!GST_IS_OBJECT(source)) return ret;
	sourceName = gst_object_get_name(source);

	debug(150, "gst type: %s", GST_MESSAGE_TYPE_NAME(msg));

	switch(GST_MESSAGE_TYPE(msg))
	{
		case GST_MESSAGE_EOS:
			debug(150, "gst player eof");
			ret = 0;
			break;
		case GST_MESSAGE_STATE_CHANGED:
			debug(150, "gst message state changed");
			if(GST_MESSAGE_SRC(msg) != GST_OBJECT(pipeline))
				break; 

			GstState old_state, new_state, pending_state;
			gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
			if(GST_MESSAGE_SRC(msg) == GST_OBJECT(pipeline))
			{
				if(new_state == GST_STATE_PLAYING)
				{
					/* Once we are in the playing state, analyze the streams */
					analyze_streams(data);
				}
			}

			if(old_state == new_state) break;
	
			debug(150, "gst state change %s -> %s", gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
	
			GstStateChange transition = (GstStateChange)GST_STATE_TRANSITION(old_state, new_state);

/*			switch(transition)
			{
				case GST_STATE_CHANGE_NULL_TO_READY:
					break;
				case GST_STATE_CHANGE_READY_TO_PAUSED:

//					GstElement *appsink = gst_bin_get_by_name(GST_BIN(pipeline), "subtitle_sink");
// 					if(appsink)
// 					{
// 						g_object_set(G_OBJECT(appsink), "max-buffers", 2, NULL);
// 						g_object_set(G_OBJECT(appsink), "sync", FALSE, NULL);
// 						g_object_set(G_OBJECT(appsink), "emit-signals", TRUE, NULL);
// 						gst_object_unref(appsink);
// 					}

					break;
				case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
					//if(m_sourceinfo.is_streaming && m_streamingsrc_timeout )
						//m_streamingsrc_timeout->stop();
					break;
				case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
					break;
				case GST_STATE_CHANGE_PAUSED_TO_READY:
					break;
				case GST_STATE_CHANGE_READY_TO_NULL:
					ret = 0;
					break;

			}
*/
			break;
		case GST_MESSAGE_ERROR:
			debug(150, "gst player error");

			gchar *gdebug1;
			GError *err;

			gst_message_parse_error(msg, &err, &gdebug1);
			g_free(gdebug1);

			debug(150, "gst error: %s (%i) from %s", err->message, err->code, sourceName);
			if(err->domain == GST_STREAM_ERROR)
			{
				if(err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND )
				{
					//if(g_strrstr(sourceName, "videosink"))
					//	m_event((iPlayableService*)this, evUser+11);
					//else if ( g_strrstr(sourceName, "audiosink") )
					//	m_event((iPlayableService*)this, evUser+10);
				}
			}
			g_error_free(err);
			break;
		case GST_MESSAGE_INFO:
			debug(150, "gst player info");

/*
			gchar *gdebug2;
			GError *inf;
	
			gst_message_parse_info(msg, &inf, &gdebug2);
			g_free(gdebug2);
			if(inf->domain == GST_STREAM_ERROR && inf->code == GST_STREAM_ERROR_DECODE )
			{
				//if(g_strrstr(sourceName, "videosink"))
				//	m_event((iPlayableService*)this, evUser+14);
			}
			g_error_free(inf);
*/
			break;
		case GST_MESSAGE_TAG:
			debug(150, "gst player tag");
			break;
		//case GST_MESSAGE_ASYNC_DONE:
		//	debug(150, "gst player async done");
		//	break;
		case GST_MESSAGE_ELEMENT:
			debug(150, "GST_MESSAGE_ELEMENT");
			const GstStructure *msgstruct = gst_message_get_structure(msg);
			if (msgstruct)
			{
				const gchar *eventname = gst_structure_get_name(msgstruct);
				if (!strcmp(eventname, "eventFrameRateChanged") || !strcmp(eventname, "eventFrameRateAvail"))
				{
					gst_structure_get_int (msgstruct, "frame_rate", &m_framerate);
				}
			}
			debug(150, "gst player element");
			break;
		case GST_MESSAGE_BUFFERING:
			debug(150, "gst player buffering");

/*
			GstBufferingMode mode;
			gst_message_parse_buffering(msg, &(m_bufferInfo.bufferPercent));
			gst_message_parse_buffering_stats(msg, &mode, &(m_bufferInfo.avgInRate), &(m_bufferInfo.avgOutRate), &(m_bufferInfo.bufferingLeft));
			//m_event((iPlayableService*)this, evBuffering);
*/

// playback stream jerky on start
//			if (data->is_live) break;
//			gst_message_parse_buffering (msg, &status.bufferpercent);

			if(status.prefillbuffer == 1)
			{
				status.prefillbuffercount++;
				if(status.prefillbuffercount >= 200 && status.bufferpercent < 2)
				{
					printf("status.prefillbuffercount >= 200 set status.prefillbuffer=2\n"); 
					status.prefillbuffer = 2;
				}
			}
			debug(150, "status.prefillbuffercount=%d status.cleaninfobar=%d status.prefillbuffer=%d status.bufferpercent=%d",status.prefillbuffercount , status.cleaninfobar, status.prefillbuffer, status.bufferpercent);
			
			if(status.prefillbuffer == 1) 
 			{
//				gint percent = 0;
				if (data->is_live)
				{
					printf("data->is_live break: status.cleaninfobar=%d status.prefillbuffer=%d status.bufferpercent=%d\n", status.cleaninfobar, status.prefillbuffer, status.bufferpercent); 
					break;
				}

				gst_message_parse_buffering (msg, &status.bufferpercent);
				g_print ("Buffering (%3d%%)\r", status.bufferpercent);

				if (status.bufferpercent < 100)
				{
					gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
					struct skin* waitmsgbar = getscreen("waitmsgbar");
					struct skin* load = getscreen("loading");
	
					waitmsgbar->progresssize = status.bufferpercent;
					char* tmpstr = NULL;
					tmpstr = ostrcat(_("Buffering Stream - Cancel with Exit"), " (", 0, 0);
					tmpstr = ostrcat(tmpstr, oitoa(waitmsgbar->progresssize), 1, 0);
					tmpstr = ostrcat(tmpstr, "%)", 1, 0);
					changetext(waitmsgbar, tmpstr);
					free(tmpstr); tmpstr = NULL;
	
					drawscreen(load, 0, 0);
					drawscreen(waitmsgbar, 0, 0);
					status.cleaninfobar = 0;
				}
				else
				{
					drawscreen(skin, 0, 0);
					gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
					status.prefillbuffer = 0;
					status.cleaninfobar = 1;
				}
	
			} 
			else if(status.prefillbuffer == 2) 
			{
				drawscreen(skin, 0, 0);
				gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
				status.prefillbuffer = 0;
				status.cleaninfobar = 1;
			}
			else if(status.cleaninfobar == 1)
			{
				drawscreen(skin, 0, 0);
				status.cleaninfobar = 0;
			}
			else
			{
//			printf("else\n"); 
				gst_message_parse_buffering (msg, &status.bufferpercent);
				g_print ("Buffering (%3d%%)\r", status.bufferpercent);
//				drawscreen(skin, 0, 0);
//				status.cleaninfobar = 0;
			}
			break;
 
/*
			GstBufferingMode mode;
			gst_message_parse_buffering(msg, &(status.bufferpercent));
			gst_message_parse_buffering_stats(msg, &mode, &(status.avgInRate), &(status.avgOutRate), &(status.bufferingLeft));

//			printf("#########################################################\n");
//			printf("Buffering %u percent done\n", status.bufferpercent);
//			printf("avgInRate %d\n", status.avgInRate);
//			printf("avgOutRate %d\n", status.avgOutRate);
//			printf("bufferingLeft %lld\n", status.bufferingLeft);
					
			if(status.prefillbuffer == 1)
			{
				printf("status.prefillbuffer Buffering %u percent done\n", status.bufferpercent);

				if (status.bufferpercent == 100)
				{
					GstState state;
					gst_element_get_state(pipeline, &state, NULL, 0LL);
					if (state != GST_STATE_PLAYING)
					{
						// eDebug("start playing");
						gst_element_set_state (pipeline, GST_STATE_PLAYING);
					}
//					m_ignore_buffering_messages = 5;
					status.prefillbuffer = 0;
				}
				else if (status.bufferpercent == 0)
				{
					// eDebug("start pause");
					gst_element_set_state (pipeline, GST_STATE_PAUSED);
//					m_ignore_buffering_messages = 0;
				}
			}
*/
/*
				GstBufferingMode mode;
				printf("GST_STATE_PAUSED\n");
				gst_element_set_state (pipeline, GST_STATE_PAUSED);


				gst_message_parse_buffering(msg, &(m_bufferInfo.bufferPercent));
				// eDebug("Buffering %u percent done", m_bufferInfo.bufferPercent);
				gst_message_parse_buffering_stats(msg, &mode, &(m_bufferInfo.avgInRate), &(m_bufferInfo.avgOutRate), &(m_bufferInfo.bufferingLeft));
				m_event((iPlayableService*)this, evBuffering);
				if (m_use_prefillbuffer && !m_is_live && --m_ignore_buffering_messages <= 0)
				{
					if (m_bufferInfo.bufferPercent == 100)
					{
						GstState state;
						gst_element_get_state(pipeline, &state, NULL, 0LL);
						if (state != GST_STATE_PLAYING)
						{
							// eDebug("start playing");
							gst_element_set_state (pipeline, GST_STATE_PLAYING);
						}
						m_ignore_buffering_messages = 5;
					}
					else if (m_bufferInfo.bufferPercent == 0)
					{
						// eDebug("start pause");
						gst_element_set_state (pipeline, GST_STATE_PAUSED);
						m_ignore_buffering_messages = 0;
					}
					else
					{
						m_ignore_buffering_messages = 0;
					}
				}

*/
			break;
		case GST_MESSAGE_STREAM_STATUS:
			debug(150, "gst player stream status");

/*
			GstStreamStatusType type;
			GstElement *owner;

			gst_message_parse_stream_status(msg, &type, &owner);
			if(type == GST_STREAM_STATUS_TYPE_CREATE && m_sourceinfo.is_streaming)
			{
				if(GST_IS_PAD(source))
					owner = gst_pad_get_parent_element(GST_PAD(source));
				else if(GST_IS_ELEMENT(source))
					owner = GST_ELEMENT(source);
				else
					owner = NULL;
				if(owner)
				{
					GstElementFactory *factory = gst_element_get_factory(GST_ELEMENT(owner));
					const gchar *name = gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(factory));
					if (!strcmp(name, "souphttpsrc"))
					{
						//m_streamingsrc_timeout->start(10 * 1000, true);
						g_object_set(G_OBJECT(owner), "timeout", 10, NULL);
					}
					
				}
				if(GST_IS_PAD(source))
					gst_object_unref(owner);
			}
*/
			break;
		default:
			debug(150, "gst player unknown message");
			break;
	}
	g_free(sourceName);
	return ret;
}
#endif

int playergetbuffersize()
{
	int ret = 0;
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->container && player->container->selectedContainer)
			player->container->selectedContainer->Command(player, CONTAINER_GET_BUFFER_SIZE, (void*)&ret);
	//		player->output->Command(player, OUTPUT_GET_BUFFER_SIZE, &ret);
	#endif
#if defined (EXTGST)
	}
#endif

	return ret;
}

int playergetbufferstatus()
{
	int ret = 0;
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->container && player->container->selectedContainer)
			player->container->selectedContainer->Command(player, CONTAINER_GET_BUFFER_STATUS, (void*)&ret);
	//		player->output->Command(player, OUTPUT_GET_BUFFER_STATUS, &ret);
	#endif
#if defined (EXTGST)
	}
#endif
	printf("playergetbufferstatus: %d\n", ret);
	return ret;
}

int playerstopbuffer()
{
	int ret = 0;
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->container && player->container->selectedContainer)
			player->container->selectedContainer->Command(player, CONTAINER_STOP_BUFFER, NULL);
	#endif
#if defined (EXTGST)
	}
#endif
	return ret;
}

int playerisplaying()
{
#ifdef SIMULATE
	return 1;
#endif

#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player != NULL && player->playback != NULL && player->playback->isPlaying)
			return 1;
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		int ret = 1;

		if(pipeline)
		{
			GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
			GstMessage *message = NULL;
	//use global variale, with static var crash
	//		CustomData *data = NULL;
			while((message = gst_bus_pop(bus)))
			{
				ret = gstbuscall(bus, message, &data);
				gst_message_unref(message);

			}		
	// fix buffering on filenuke
			if(status.cleaninfobar == 0 && status.prefillbuffer == 1 && status.bufferpercent == 0)
				gst_element_set_state (pipeline, GST_STATE_PAUSED);
				
	// eof workaround for some mp4 files.
			gint64 pts = 0, len = 0, rest = 0;
			gint64 nanos_pts = 0, nanos_len = 0;

			len = playergetlength();
			nanos_len = len * 1000000000;
			if(nanos_len < 0) nanos_len = 0;

			pts = playergetpts();
			nanos_pts = pts * 11111;

			rest = nanos_len - nanos_pts;
	//		printf("rest: %lld\n", nanos_len - nanos_pts);

			debug(150, "status.pause=%d status.playspeed=%d status.slowspeed=%d status.prefillbuffer=%d rest=%lld", status.pause, status.playspeed, status.slowspeed, status.prefillbuffer, rest);
			if(rest > 4000000000LL || status.pts != pts || pts == 0 || status.pause == 1 || status.playspeed != 0 || status.slowspeed != 0 /*|| status.prefillbuffer == 1*/)
			{
	//			debug(150, "status.pts=%llu / pts=%llu\n", status.pts, pts);
				status.pts = pts;
			}
			else
			{
				debug(150, "gst player eof - workaround (rest=%lld)", rest);
				ret = 0;
			}
	// eof workaround done
		}
		else
			ret = 0;

		return ret;
	#endif
#if defined (EXTGST)
	}
#endif
	return 0;
}

void playerplay()
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_PLAY, NULL);
	#endif 
#if defined (EXTGST)
	}
	else
	{
#endif

	#ifdef EPLAYER4
		if(pipeline)
			gst_element_set_state(pipeline, GST_STATE_PLAYING);
		if(subtitleflag == 2)
			subtitleflag = 1;
	#endif
#if defined (EXTGST)
	}
#endif
}

int playerstop()
{
	status.writeplayersub = 0;

	if(status.actplay != NULL)
	{
		char* fileseek = changefilenameext(status.actplay, ".pts");
		FILE* fbseek = fopen(fileseek, "w");
		if(fbseek != NULL)
		{
			off64_t pts = playergetpts();
			fprintf(fbseek,"%lld", pts);
			fclose(fbseek);
		}
		free(fileseek); fileseek=NULL;
		free(status.actplay); status.actplay=NULL;
	}
	 
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_STOP, NULL);
		if(player && player->container && player->container->selectedContainer)
			player->container->selectedContainer->Command(player, CONTAINER_STOP, NULL);
		if(player && player->output)
		{
			player->output->Command(player, OUTPUT_CLOSE, NULL);
			player->output->Command(player, OUTPUT_DEL, (void*)"audio");
			player->output->Command(player, OUTPUT_DEL, (void*)"video");
			player->output->Command(player, OUTPUT_DEL, (void*)"subtitle");
		}
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_CLOSE, NULL);

		PlaybackDieNow(1);

	//	if((status.play == 0 || status.pause == 1) && status.subtitlethread != NULL)
	//		status.subtitlethread->aktion = STOP;
						
		free(player);
		player = NULL;

	// move to mc
	//	set_player_sound(1);
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		subtitleflag = 0;
		if(status.subtitlethread != 0)
			status.subtitlethread->aktion = STOP;
		if(video_sink)
		{
			gst_object_unref (video_sink);
			video_sink = NULL;
		}
		if(pipeline)
		{
			gst_element_set_state(pipeline, GST_STATE_NULL);
			gst_object_unref(GST_OBJECT(pipeline));
			pipeline = NULL;
		}
	#endif
#if defined (EXTGST)
	}
#endif
	writesysint("/proc/sys/vm/drop_caches", 3, 0);
	return 0;
}

void playerafterend()
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player != NULL && player->playback != NULL)
			playerstop();
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		if(pipeline)
			playerstop();
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerpause()
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(status.playspeed != 0 || status.slowspeed != 0)
			playerff(0);
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_PAUSE, NULL);
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		if(pipeline)
			gst_element_set_state(pipeline, GST_STATE_PAUSED);
	#endif
#if defined (EXTGST)
	}
#endif
}

void playercontinue()
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_CONTINUE, NULL);
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		if(pipeline)
		{
			if(status.playspeed != 0 || status.slowspeed != 0)
			{
				//subtitle sync bug... start
				gint64 time_nanoseconds = 0;
				GstFormat fmt = GST_FORMAT_TIME;
	#if GST_VERSION_MAJOR < 1
				if (!gst_element_query_position(pipeline, &fmt, &time_nanoseconds))
	#else
				if (!gst_element_query_position(pipeline, fmt, &time_nanoseconds))
	#endif
				{
					err("gst_element_query_position failed");
					return;
				}
				time_nanoseconds = time_nanoseconds - 90000;
				if (!gst_element_seek (pipeline, 1, GST_FORMAT_TIME, (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),GST_SEEK_TYPE_SET, time_nanoseconds,GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
					printf("seekTo failed");
				//subtitle sync bug... end

				//playersend_ff_fr_event(1);
			}		
			gst_element_set_state(pipeline, GST_STATE_PLAYING);
			if(subtitleflag == 2)
				subtitleflag = 1;
		}
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerff(int speed)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3

		int speedmap = 0;
	/*
		if(speed == 0)
		{
			if(player && player->playback)
			{
				player->playback->Command(player, PLAYBACK_FASTFORWARD, &speedmap);
				return;
			}
		}
	*/
		if (speed < 1) speed = 1;
		if (speed > 7) speed = 7;

		switch(speed)
		{
			case 1: speedmap = 1; break;
			case 2: speedmap = 3; break;
			case 3: speedmap = 7; break;
			case 4: speedmap = 15; break;
			case 5: speedmap = 31; break;
			case 6: speedmap = 63; break;
			case 7: speedmap = 127; break;

			case -1: speedmap = -5; break;
			case -2: speedmap = -10; break;
			case -3: speedmap = -20; break;
			case -4: speedmap = -40; break;
			case -5: speedmap = -80; break;
			case -6: speedmap = -160; break;
			case -7: speedmap = -320; break;
			
		}

		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_FASTFORWARD, &speedmap);
		
	#ifdef MIPSEL
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_CONTINUE, NULL);
	#endif
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		gdouble rate = 0;
		subtitleflag = 2;
		if (speed < 1) speed = 1;
		if (speed > 7) speed = 7;

		switch(speed)
		{
			case 1: rate = 2; break;
			case 2: rate = 4; break;
			case 3: rate = 8; break;
			case 4: rate = 16; break;
			case 5: rate = 32; break;
			case 6: rate = 64; break;
			case 7: rate = 128; break;
		}
		playersend_ff_fr_event(rate);
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerslow(int speed)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		int speedmap = 0;
	#ifdef EXTEPLAYER3
		if (speed < 2) speed = 2;
		if (speed > 8) speed = 8;

		switch(speed)
		{
			case 2: speedmap = 2; break;
			case 4: speedmap = 4; break;
			case 8: speedmap = 8; break;
		}
	#else
		if (speed < 1) speed = 1;
		if (speed > 7) speed = 7;

		switch(speed)
		{
			case 1: speedmap = 1; break;
			case 2: speedmap = 3; break;
			case 3: speedmap = 7; break;
			case 4: speedmap = 15; break;
			case 5: speedmap = 31; break;
			case 6: speedmap = 63; break;
			case 7: speedmap = 127; break;
		}
	#endif
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_SLOWMOTION, &speedmap);

	#ifdef MIPSEL
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_CONTINUE, NULL);
	#endif
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		gdouble rate = 0;
		if (speed < 1) speed = 1;
		if (speed > 7) speed = 7;
			
		switch(speed)
		{
			case 1: rate = 0.8; break;
			case 2: rate = 0.7; break;
			case 3: rate = 0.6; break;
			case 4: rate = 0.5; break;
			case 5: rate = 0.3; break;
			case 6: rate = 0.2; break;
			case 7: rate = 0.1; break;
		}
		gst_element_set_state(pipeline, GST_STATE_PLAYING);
		playersend_ff_fr_event(rate);
		
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerfr(int speed)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		int speedmap = 0;

		if (speed > -1) speed = -1;
		if (speed < -7) speed = -7;

		switch(speed)
		{
			case -1: speedmap = -5; break;
			case -2: speedmap = -10; break;
			case -3: speedmap = -20; break;
			case -4: speedmap = -40; break;
			case -5: speedmap = -80; break;
			case -6: speedmap = -160; break;
			case -7: speedmap = -320; break;
		}

		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_FASTBACKWARD, &speedmap);

	#ifdef MIPSEL
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_CONTINUE, NULL);
	#endif
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		gdouble rate = 0;
		
		if (speed > -1) speed = -1;
		if (speed < -7) speed = -7;

		switch(speed)
		{
			case -1: rate = -2; break;
			case -2: rate = -4; break;
			case -3: rate = -8; break;
			case -4: rate = -16; break;
			case -5: rate = -32; break;
			case -6: rate = -64; break;
			case -7: rate = -128; break;
		}
		playersend_ff_fr_event(rate);
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerseek(float sec)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
	#ifdef EXTEPLAYER3
		int64_t sectmp = (int64_t)sec;
		if(player && player->playback)	
			player->playback->Command(player, PLAYBACK_SEEK, (void*)&sectmp);
	#else
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_SEEK, (void*)&sec);
	#endif
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		gint64 nanos_pts = 0, nanos_len = 0;
		gint64 pts = 0, len = 0;
		//GstFormat fmt = GST_FORMAT_TIME;
			
		if(pipeline)
		{
			len = playergetlength();
			nanos_len = len * 1000000000;
			if(nanos_len < 0) nanos_len = 0;

			pts = playergetpts();
			nanos_pts = pts * 11111;
			nanos_pts = nanos_pts + (sec * 1000000000);
			if(nanos_pts < 0) nanos_pts = 0;

			if(nanos_pts >= nanos_len)
			{
				debug(150, "gst skip seeking");
	//			playerstop();
			}
			else
				gst_element_seek(pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, nanos_pts, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
		}
	#endif
#if defined (EXTGST)
	}
#endif
}

#ifdef EPLAYER4
audiotype_t gstCheckAudioPad(GstStructure* structure)
{
	if(!structure)
		return atUnknown;

	if(gst_structure_has_name(structure, "audio/mpeg"))
	{
		gint mpegversion, layer = -1;
		if(!gst_structure_get_int (structure, "mpegversion", &mpegversion))
			return atUnknown;

		switch(mpegversion)
		{
			case 1:
				{
					gst_structure_get_int(structure, "layer", &layer);
					if(layer == 3)
						return atMP3;
					else
						return atMPEG;
					break;
				}
			case 2:
				return atAAC;
			case 4:
				return atAAC;
			default:
				return atUnknown;
		}
	}

	else if(gst_structure_has_name(structure, "audio/x-ac3") || gst_structure_has_name(structure, "audio/ac3"))
		return atAC3;
	else if(gst_structure_has_name(structure, "audio/x-dts") || gst_structure_has_name(structure, "audio/dts"))
		return atDTS;
#if GST_VERSION_MAJOR < 1
	else if(gst_structure_has_name(structure, "audio/x-raw-int"))
#else
	else if(gst_structure_has_name(structure, "audio/x-raw"))
#endif
		return atPCM;

	return atUnknown;
}


subtype_t getSubtitleType(GstPad* pad, gchar *g_codec)
{
g_codec = NULL;
	subtype_t type = stUnknown;
#if GST_VERSION_MAJOR < 1
	GstCaps* caps = gst_pad_get_negotiated_caps(pad);
#else
	GstCaps* caps = gst_pad_get_current_caps(pad);
#endif
	if (!caps && !g_codec)
	{
		caps = gst_pad_get_allowed_caps(pad);
	}

	if (caps && !gst_caps_is_empty(caps))
	{
		GstStructure* str = gst_caps_get_structure(caps, 0);
		if (str)
		{
			const gchar *g_type = gst_structure_get_name(str);
			// eDebug("getSubtitleType::subtitle probe caps type=%s", g_type ? g_type : "(null)");
			if (g_type)
			{
#if GST_VERSION_MAJOR < 1
				if ( !strcmp(g_type, "video/x-dvd-subpicture") )
#else
				if ( !strcmp(g_type, "subpicture/x-dvd") )
#endif
					type = stVOB;
				else if ( !strcmp(g_type, "text/x-pango-markup") )
					type = stSRT;
				else if ( !strcmp(g_type, "text/plain") || !strcmp(g_type, "text/x-plain") || !strcmp(g_type, "text/x-raw") )
					type = stPlainText;
				else if ( !strcmp(g_type, "subpicture/x-pgs") )
					type = stPGS;
				else
					printf("getSubtitleType::unsupported subtitle caps %s (%s)\n", g_type, g_codec ? g_codec : "(null)");
//					eDebug("getSubtitleType::unsupported subtitle caps %s (%s)", g_type, g_codec ? g_codec : "(null)");
			}
		}
	}
	else if ( g_codec )
	{
		// eDebug("getSubtitleType::subtitle probe codec tag=%s", g_codec);
		if ( !strcmp(g_codec, "VOB") )
			type = stVOB;
		else if ( !strcmp(g_codec, "SubStation Alpha") || !strcmp(g_codec, "SSA") )
			type = stSSA;
		else if ( !strcmp(g_codec, "ASS") )
			type = stASS;
		else if ( !strcmp(g_codec, "SRT") )
			type = stSRT;
		else if ( !strcmp(g_codec, "UTF-8 plain text") )
			type = stPlainText;
		else
			printf("getSubtitleType::unsupported subtitle codec %s\n", g_codec);
	}
	else
		printf("getSubtitleType::unidentifiable subtitle stream!\n");

	return type;
}

#endif

void playerfreetracklist(char** TrackList)
{
	int i = 0;

	if(TrackList != NULL)
	{
		while(TrackList[i] != NULL)
		{
			free(TrackList[i]);
			free(TrackList[i + 1]);
			i += 2;
		}
	}
}

char** playergettracklist(int type)
{
	#ifdef EXTEPLAYER3
		#ifdef EPLAYER3
			TrackDescription_t *TrackList = NULL;
			char ** TrackList2 = NULL;
		#else
			char ** TrackList = NULL;
		#endif
	#else
		char ** TrackList = NULL;
	#endif

	#ifdef EPLAYER4
		char ** TrackList4 = NULL;
	#endif

#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->manager)
		{
			switch(type)
			{
				case 1:
					if(player->manager->audio)
					{
						player->manager->audio->Command(player, MANAGER_LIST, &TrackList);
						debug(150, "Audio Track List");
					}
					break;
				case 2:
					if(player->manager->subtitle)
					{
						player->manager->subtitle->Command(player, MANAGER_LIST, &TrackList);
						debug(150, "Subtitle Track List");
					}
					break;
				default:
					if(player->manager->video)
					{
						player->manager->video->Command(player, MANAGER_LIST, &TrackList);
						debug(150, "Video Track List");
					}
			}

//#if defined (EXTGST)
//			if(getconfigint("extplayer_type", NULL) == 0)
//			{
//#endif
		#ifdef EXTEPLAYER3

			TrackList2 = calloc(1, sizeof(char *) * ((100 * 2) + 1));

			char* tmpstr = NULL;
			if( NULL != TrackList) 
			{
				debug(150, "Track List");

			    int i = 0;
			    for (i = 0; TrackList[i].Id >= 0; ++i) 
			    {
					tmpstr = ostrcat(oitoa(TrackList[i].Id), ": ", 1, 0);
					tmpstr = ostrcat(tmpstr, TrackList[i].Name, 1, 0);
					TrackList2[i * 2] = ostrcat(tmpstr, NULL, 0, 0);
					TrackList2[(i * 2) + 1] = strdup(TrackList[i].Encoding);
			        free(TrackList[i].Encoding);
			        free(TrackList[i].Name);
					debug(150, "%s - %s", TrackList2[i * 2], TrackList2[i * 2 + 1]);
			    }
			    free(TrackList);
			}
			free(tmpstr), tmpstr = NULL;
		#else
			int i = 0;
			if(TrackList != NULL)
			{
				while(TrackList[i] != NULL)
				{
					string_newline(TrackList[i]);
					i += 2;
				}
				
				debug(150, "Track List");
				i = 0;
				while(TrackList[i] != NULL)
				{
					debug(150, "%s - %s", TrackList[i], TrackList[i + 1]);
					i += 2;
				}
			}
		#endif
//#if defined (EXTGST)
//			}
//#endif
		}
	#endif
#if defined (EXTGST)
	}
#endif

//////////////////////////////NEUER CODE //////////////////////////////
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 1)
	{
#endif
	#ifdef EPLAYER4
		TrackList4 = calloc(1, sizeof(char *) * ((100 * 2) + 1));
		
		if(pipeline != NULL)
		{
			gint i, n_video = 0, n_audio = 0, n_text = 0;
			
			g_object_get(pipeline, "n-video", &n_video, NULL);
			g_object_get(pipeline, "n-audio", &n_audio, NULL);
			g_object_get(pipeline, "n-text", &n_text, NULL);

			switch(type)
			{
				case 1:
					for(i = 0; i < n_audio; i++)
					{
						GstTagList *tags = NULL;
						gchar *g_codec = NULL, *g_lang = NULL;
						char* tmpstr = NULL;
						GstPad* pad = 0;
						
						g_signal_emit_by_name (pipeline, "get-audio-pad", i, &pad);
	#if GST_VERSION_MAJOR < 1
						GstCaps* caps = gst_pad_get_negotiated_caps(pad);
	#else
						GstCaps* caps = gst_pad_get_current_caps(pad);
	#endif
						if(!caps)
							continue;
						
						GstStructure* str = gst_caps_get_structure(caps, 0);
						const gchar *g_type = gst_structure_get_name(str);

						g_signal_emit_by_name(pipeline, "get-audio-tags", i, &tags);

	#if GST_VERSION_MAJOR < 1
						if(tags && gst_is_tag_list(tags))
	#else
						if(tags && GST_IS_TAG_LIST(tags))
	#endif
						{
							if(gst_tag_list_get_string(tags, GST_TAG_AUDIO_CODEC, &g_codec))
							{
								printf("Audio Codec: %s\n", g_codec);
		
								tmpstr = ostrcat(oitoa(i), ": ", 1, 0);
								tmpstr = ostrcat(tmpstr, g_codec, 1, 0);
								if(g_codec != NULL && g_type != NULL)
									tmpstr = ostrcat(tmpstr, " (", 1, 0);
								tmpstr = ostrcat(tmpstr, (gchar*)g_type, 1, 0);
								if(g_codec != NULL && g_type != NULL)
									tmpstr = ostrcat(tmpstr, ")", 1, 0);

								printf("Tracklist tmpstr: %s\n", tmpstr);

								TrackList4[i * 2] = ostrcat(tmpstr, NULL, 0, 0);
								g_free(tmpstr); tmpstr = NULL;
								g_free(g_codec); g_codec = NULL;
							}
							if(gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &g_lang))
							{
								printf("Audio Lang: %s\n", g_lang);
								TrackList4[(i * 2) + 1] = ostrcat(g_lang, NULL, 0, 0);
								g_free(g_lang); g_lang = NULL;
							}
							gst_tag_list_free(tags);
						}
						else
						{
							printf("Audio Codec: %s\n", g_codec);
							
							tmpstr = ostrcat(oitoa(i), ": ", 1, 0);
							tmpstr = ostrcat(tmpstr, g_codec, 1, 0);
							if(g_codec != NULL && g_type != NULL)
								tmpstr = ostrcat(tmpstr, " (", 1, 0);
							tmpstr = ostrcat(tmpstr, (gchar*)g_type, 1, 0);
							if(g_codec != NULL && g_type != NULL)
								tmpstr = ostrcat(tmpstr, ")", 1, 0);

							printf("Tracklist tmpstr: %s\n", tmpstr);				
							TrackList4[i * 2] = ostrcat(tmpstr, NULL, 0, 0);

							g_free(tmpstr); tmpstr = NULL;
							g_free(g_codec); g_codec = NULL;
						}
					}
					break;
				case 2:
				printf("case2 subs\n");

					for(i = 0; i < n_text; i++)
					{
						GstTagList *tags = NULL;
						gchar *g_codec = NULL, *g_lang = NULL;
						GstPad* pad = 0;
						char* tmpstr = NULL;

						g_signal_emit_by_name (pipeline, "get-text-pad", i, &pad);
						printf("case2 subs 1 SubTitle Type: %d\n", getSubtitleType(pad, g_codec));

	#if GST_VERSION_MAJOR < 1
						GstCaps* caps = gst_pad_get_negotiated_caps(pad);
	#else
						GstCaps* caps = gst_pad_get_current_caps(pad);
	#endif
						if (!caps && !g_codec)
						{
							caps = gst_pad_get_allowed_caps(pad);
						}
							
						GstStructure* str = gst_caps_get_structure(caps, 0);
						const gchar *g_type = gst_structure_get_name(str);

						g_signal_emit_by_name(pipeline, "get-text-tags", i, &tags);
						
	#if GST_VERSION_MAJOR < 1
						if (tags && gst_is_tag_list(tags))
	#else
						if (tags && GST_IS_TAG_LIST(tags))
	#endif
						{
				printf("eServiceMP3::subtitle stream=%i language=%s codec=%s", i, g_lang, g_codec);
	
							if(gst_tag_list_get_string(tags, GST_TAG_SUBTITLE_CODEC, &g_codec))
							{
								printf("case2 subs 2 SubTitle Codec: %s\n", g_codec);
								tmpstr = ostrcat(oitoa(i), ": ", 1, 0);
								tmpstr = ostrcat(tmpstr, g_codec, 1, 0);
								if(g_codec != NULL && g_type != NULL)
									tmpstr = ostrcat(tmpstr, " (", 1, 0);
								tmpstr = ostrcat(tmpstr, (gchar*)g_type, 1, 0);
								if(g_codec != NULL && g_type != NULL)
									tmpstr = ostrcat(tmpstr, ")", 1, 0);

								printf("case2 subs 2 Tracklist tmpstr: %s\n", tmpstr);
								TrackList4[i * 2] = ostrcat(tmpstr, NULL, 0, 0);
								g_free(g_codec); g_codec = NULL;
							}
							else
							{
								printf("case2 subs 3 SubTitle Codec: %s\n", g_codec);
								tmpstr = ostrcat(oitoa(i), ": ", 1, 0);
								tmpstr = ostrcat(tmpstr, "unsuported codec", 1, 0);
								if(g_type != NULL)
								{
									tmpstr = ostrcat(tmpstr, " (", 1, 0);
									tmpstr = ostrcat(tmpstr, (gchar*)g_type, 1, 0);
									tmpstr = ostrcat(tmpstr, ")", 1, 0);
								}

								printf("case2 subs 3 Tracklist tmpstr: %s\n", tmpstr);
								TrackList4[i * 2] = ostrcat(tmpstr, NULL, 0, 0);
								g_free(g_codec); g_codec = NULL;
							}
							if(gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &g_lang))
							{
								printf("case2 subs 4 SubTitle Lang: %s\n", g_lang);
								TrackList4[(i * 2) + 1] = ostrcat(g_lang, NULL, 0, 0);
								g_free(g_lang); g_lang = NULL;
							}
							gst_tag_list_free(tags);
						}
						else
						{
							printf("case2 subs 3 SubTitle Codec: %s\n", g_codec);
							
							tmpstr = ostrcat(oitoa(i), ": ", 1, 0);
							tmpstr = ostrcat(tmpstr, g_codec, 1, 0);
							if(g_codec != NULL && g_type != NULL)
								tmpstr = ostrcat(tmpstr, " (", 1, 0);
							tmpstr = ostrcat(tmpstr, (gchar*)g_type, 1, 0);
							if(g_codec != NULL && g_type != NULL)
								tmpstr = ostrcat(tmpstr, ")", 1, 0);

							printf("case2 subs 3 Tracklist tmpstr: %s\n", tmpstr);		
							TrackList4[i * 2] = ostrcat(tmpstr, NULL, 0, 0);

							g_free(tmpstr); tmpstr = NULL;
							g_free(g_codec); g_codec = NULL;				
						}
					}
					break;
				case 222:
				printf("case2 subs\n");

					for(i = 0; i < n_text; i++)
					{
											char* tmpstr = NULL;
											
						gchar *g_codec = NULL, *g_lang = NULL;
						GstTagList *tags = NULL;
						g_signal_emit_by_name (pipeline, "get-text-tags", i, &tags);
//						subtitleStream subs;
//						subs.language_code = "und";
		#if GST_VERSION_MAJOR < 1
						if (tags && gst_is_tag_list(tags))
		#else
						if (tags && GST_IS_TAG_LIST(tags))
		#endif
						{
//							if (gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &g_lang))
//							{
//								subs.language_code = g_lang;
//								g_free(g_lang);
//							}
							if(gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &g_lang))
							{
								printf("case2 subs 2 SubTitle Lang: %s\n", g_lang);
								TrackList4[(i * 2) + 1] = ostrcat(g_lang, NULL, 0, 0);
								g_free(g_lang); g_lang = NULL;
							}
//							gst_tag_list_get_string(tags, GST_TAG_SUBTITLE_CODEC, &g_codec);

							GstPad* pad = 0;
							g_signal_emit_by_name (pipeline, "get-text-pad", i, &pad);
						
		#if GST_VERSION_MAJOR < 1
							GstCaps* caps = gst_pad_get_negotiated_caps(pad);
		#else
							GstCaps* caps = gst_pad_get_current_caps(pad);
		#endif
							if (!caps && !g_codec)
							{
								caps = gst_pad_get_allowed_caps(pad);
							}

							gst_object_unref(pad);
							
							GstStructure* str = gst_caps_get_structure(caps, 0);
							const gchar *g_type = gst_structure_get_name(str);
							gst_tag_list_get_string(tags, GST_TAG_SUBTITLE_CODEC, &g_codec);

//							if(gst_tag_list_get_string(tags, GST_TAG_SUBTITLE_CODEC, &g_codec))
//							{
								printf("case2 subs 2 SubTitle Codec: %s\n", g_codec);
								tmpstr = ostrcat(oitoa(i), ": ", 1, 0);
								tmpstr = ostrcat(tmpstr, g_codec, 1, 0);
								if(g_codec != NULL && g_type != NULL)
									tmpstr = ostrcat(tmpstr, " (", 1, 0);
								tmpstr = ostrcat(tmpstr, (gchar*)g_type, 1, 0);
								if(g_codec != NULL && g_type != NULL)
									tmpstr = ostrcat(tmpstr, ")", 1, 0);

								printf("case2 subs 2 Tracklist tmpstr: %s\n", tmpstr);
								TrackList4[i * 2] = ostrcat(tmpstr, NULL, 0, 0);
								g_free(g_codec); g_codec = NULL;
//							}
							gst_tag_list_free(tags);
						}

						//eDebug("[eServiceMP3] subtitle stream=%i language=%s codec=%s", i, subs.language_code.c_str(), g_codec ? g_codec : "(null)");

//						GstPad* pad = 0;
//						g_signal_emit_by_name (pipeline, "get-text-pad", i, &pad);
//						if ( pad )
//							g_signal_connect (G_OBJECT (pad), "notify::caps", G_CALLBACK (gstTextpadHasCAPS), this);

	//					subs.type = getSubtitleType(pad, g_codec);
//						gst_object_unref(pad);
//						g_free(g_codec);
	//					m_subtitleStreams.push_back(subs);

					}
					break;

				default:
				printf("case default video\n");

					for(i = 0; i < n_video; i++)
					{
						GstTagList *tags = NULL;
						gchar *g_codec = NULL, *g_lang = NULL;
						
						g_signal_emit_by_name(pipeline, "get-video-tags", i, &tags);
						
	#if GST_VERSION_MAJOR < 1
						if (tags && gst_is_tag_list(tags))
	#else
						if (tags && GST_IS_TAG_LIST(tags))
	#endif
						{
							if(gst_tag_list_get_string(tags, GST_TAG_VIDEO_CODEC, &g_codec))
							{
								printf("Video Codec: %s\n", g_codec);
								TrackList4[i * 2] = ostrcat(g_codec, NULL, 0, 0);
								g_free(g_codec); g_codec = NULL;
							}
							if(gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &g_lang))
							{
								printf("Video Lang: %s\n", g_lang);
								TrackList4[(i * 2) + 1] = ostrcat(g_lang, NULL, 0, 0);
								g_free(g_lang); g_lang = NULL;
							}
							gst_tag_list_free(tags);
						}
					}
			}
		}
	#endif
#if defined (EXTGST)
	}
#endif
//////////////////////////////NEUER CODE //////////////////////////////
printf("return tracklist\n");
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 1)
{
printf("return tracklist4\n");

		return TrackList4;
}
	else
	{
#endif
	#ifdef EXTEPLAYER3
	#ifdef EPLAYER3
printf("return tracklist2\n");

		return TrackList2;
	#else
printf("return tracklist\n");
		return TrackList;
	#endif
	#else
printf("return tracklist4b\n");

		return TrackList4;
	#endif
#if defined (EXTGST)
	}
#endif

}



//*CurTrackEncoding and *CurTrackName be freed
void playergetcurtrac(int type, int *CurTrackId, char** CurTrackEncoding, char** CurTrackName)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->manager)
		{
			switch(type)
			{
				case 1:
					if(player->manager->audio)
					{
						player->manager->audio->Command(player, MANAGER_GET, CurTrackId);
						player->manager->audio->Command(player, MANAGER_GETENCODING, CurTrackEncoding);
						player->manager->audio->Command(player, MANAGER_GETNAME, CurTrackName);
					}
					break;
				case 2:
					if(player->manager->subtitle)
					{
						player->manager->subtitle->Command(player, MANAGER_GET, CurTrackId);
						player->manager->subtitle->Command(player, MANAGER_GETENCODING, CurTrackEncoding);
						player->manager->subtitle->Command(player, MANAGER_GETNAME, CurTrackName);
					}
					break;
				default:
					if(player->manager->video)
					{
						player->manager->video->Command(player, MANAGER_GET, CurTrackId);
						player->manager->video->Command(player, MANAGER_GETENCODING, CurTrackEncoding);
						player->manager->video->Command(player, MANAGER_GETNAME, CurTrackName);
					}
			}

			if(CurTrackId != NULL)
				debug(150, "Current Track ID: %d", *CurTrackId);
			if(*CurTrackEncoding != NULL)
				debug(150, "Current Track Enc: %s", *CurTrackEncoding);
			if(*CurTrackName != NULL)
				debug(150, "Current Track Name: %s", *CurTrackName);
		}
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		if(pipeline != NULL)
		{
			switch(type)
			{
				case 1:
					g_object_get(G_OBJECT(pipeline), "current-audio", CurTrackId, NULL);
					break;
				case 2:
					if(subtitleflag == 0)
						*CurTrackId = -1;
					else 
						g_object_get(G_OBJECT(pipeline), "current-text", CurTrackId, NULL);
					break;
			}
			if(CurTrackId != NULL) {
				debug(150, "Current Track ID: %d", *CurTrackId);
			}
		}
	#endif
#if defined (EXTGST)
	}
#endif
}

unsigned long long playergetpts2()
{
	int64_t pts = 0;
	int64_t sec = 0;
	sec = 0;
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
		{
			player->playback->Command(player, PLAYBACK_PTS, &pts);
			sec = pts / 90000;
			fprintf(stderr, "{\"J\":{\"ms\":%lld}}\n", pts / 90);

			debug(150, "Pts = %02d:%02d:%02d (%lld sec)", (int)((sec / 60) / 60) % 60, (int)(sec / 60) % 60, (int)sec % 60, sec);

		}
	#endif
#if defined (EXTGST)
	}
#endif
	if(pts < 0) pts = 0;
	return pts;
}

unsigned long long playergetpts()
{
#ifdef EXTEPLAYER3
	#ifdef EPLAYER3
		int64_t pts = 0;
		int64_t sec = 0;
	#else
		unsigned long long pts = 0;
		unsigned long long sec = 0;
	#endif
#else
	unsigned long long pts = 0;
	unsigned long long sec = 0;
#endif

#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
	#ifdef EXTEPLAYER3
		#ifdef EPLAYER3
			int64_t pts = 0;
			int64_t sec = 0;
		#else
			unsigned long long pts = 0;
			unsigned long long sec = 0;
		#endif
	#endif
	}
	else
	{
		#ifdef EPLAYER4
			unsigned long long pts = 0;
			unsigned long long sec = 0;
		#endif
	}
#endif





#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
		{
			player->playback->Command(player, PLAYBACK_PTS, &pts);
			sec = pts / 90000;
	#ifdef EXTEPLAYER3
			debug(150, "Pts = %02d:%02d:%02d (%lld sec)", (int)((sec / 60) / 60) % 60, (int)(sec / 60) % 60, (int)sec % 60, sec);
	#else
			debug(150, "Pts = %02d:%02d:%02d (%llu.0000 sec)", (int)((sec / 60) / 60) % 60, (int)(sec / 60) % 60, (int)sec % 60, sec);
	#endif
		}
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		GstFormat fmt = GST_FORMAT_TIME; //Returns time in nanosecs
		
	/*
		if(pipeline)
		{
	#if GST_VERSION_MAJOR < 1
			gst_element_query_position(pipeline, &fmt, (gint64*)&pts);
	#else
			gst_element_query_position(pipeline, fmt, (gint64*)&pts);
	#endif
			sec = pts / 1000000000;
			pts = sec * 90000;
			debug(150, "Pts = %02d:%02d:%02d (%llu.0000 sec)", (int)((sec / 60) / 60) % 60, (int)(sec / 60) % 60, (int)sec % 60, sec);
		}
	*/

		if(pipeline)
		{
			gint64 pos;
			GstElement *sink;
			pts = 0;

			g_object_get(G_OBJECT (pipeline), "audio-sink", &sink, NULL);

			if(!sink) g_object_get (G_OBJECT (pipeline), "video-sink", &sink, NULL);
			if(!sink) return 0;

			gchar *name = gst_element_get_name(sink);
			gboolean use_get_decoder_time = ostrstr(name, "dvbaudiosink") || ostrstr(name, "dvbvideosink");
			g_free(name);

			if(use_get_decoder_time) g_signal_emit_by_name(sink, "get-decoder-time", &pos);

			gst_object_unref(sink);

	#if GST_VERSION_MAJOR < 1
			if(!use_get_decoder_time && !gst_element_query_position(pipeline, &fmt, &pos))
	#else
			if(!use_get_decoder_time && !gst_element_query_position(pipeline, fmt, &pos))
	#endif
				return 0;

			/* pos is in nanoseconds. we have 90 000 pts per second. */
			pts = pos / 11111;
			pts = pts - m_gst_startpts;
			sec = pts / 90000;
			debug(150, "StartPTS = %llu Pts = %02d:%02d:%02d (%llu.0000 sec)", m_gst_startpts, (int)((sec / 60) / 60) % 60, (int)(sec / 60) % 60, (int)sec % 60, sec);
		}
	#endif
#if defined (EXTGST)
}
#endif
	if(pts < 0) pts = 0;
	return pts;
}

double playergetlength2()
{
	int64_t length = 0;
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
		{
			player->playback->Command(player, PLAYBACK_LENGTH, (void*)&length);
			if(length < 0) length = 0;
			debug(150, "Length = %02d:%02d:%02d (%.4f sec)", (int)((length / 60) / 60) % 60, (int)(length / 60) % 60, (int)length % 60, length);
			debug(150, "Length2 = %02d:%02d:%02d (%lld sec)", (int)((length / 60) / 60) % 60, (int)(length / 60) % 60, (int)length % 60, length);

		            fprintf(stderr, "{\"PLAYBACK_LENGTH\":{\"length\":%lld}}\n", length);

		}
	#endif
#if defined (EXTGST)
	}
#endif
	return length;
}

double playergetlength()
{
#ifdef EXTEPLAYER3
	#ifdef EPLAYER3
		int64_t length = 0;
	#else
		double length = 0;
	#endif
#else
	double length = 0;
#endif

#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
	#ifdef EXTEPLAYER3
		#ifdef EPLAYER3
			int64_t length = 0;
		#else
			double length = 0;
		#endif
	#endif
	}
	else
	{
		#ifdef EPLAYER4
			double length = 0;
		#endif
	}
#endif

#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
		{
			player->playback->Command(player, PLAYBACK_LENGTH, &length);
			if(length < 0) length = 0;
	#ifdef EXTEPLAYER3
			debug(150, "Length = %02d:%02d:%02d (%lld sec)", (int)((length / 60) / 60) % 60, (int)(length / 60) % 60, (int)length % 60, length);
	#else
			debug(150, "Length = %02d:%02d:%02d (%.4f sec)", (int)((length / 60) / 60) % 60, (int)(length / 60) % 60, (int)length % 60, length);
	#endif
		}
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		GstFormat fmt = GST_FORMAT_TIME; //Returns time in nanosecs
		gint64 len;

		if(pipeline)
		{
	#if GST_VERSION_MAJOR < 1
			gst_element_query_duration(pipeline, &fmt, &len);
	#else
			gst_element_query_duration(pipeline, fmt, &len);
	#endif
			length = len / 1000000000;
			if(length < 0) length = 0;
			debug(150, "Length = %02d:%02d:%02d (%.4f sec)", (int)((length / 60) / 60) % 60, (int)(length / 60) % 60, (int)length % 60, length);
		}
	#endif
#if defined (EXTGST)
	}
#endif
	return length;
}

char* playergetinfo(char* tag)
{
	char* ret = NULL;
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		char *tags[] = {"Title", "Artist", "Album", "Year", "Genre", "Comment", "Track", "Copyright", "TestLibEplayer", NULL};
		int i = 0;

		if(player && player->playback)
		{
			while(tags[i] != NULL)
			{
				ret = tags[i];
				if(ostrcmp(tag, ret) == 0)
				{
					player->playback->Command(player, PLAYBACK_INFO, &ret);
					break;
				}

				i++;
			}
		}
	#endif
#if defined (EXTGST)
	}
#endif
	return ret;
}

void playerchangeaudiotrack(int num)
{
	debug(150, "change audiotrac to %d\n", num);
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_SWITCH_AUDIO, (void*)&num);
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		if(pipeline != NULL)
			g_object_set(G_OBJECT(pipeline), "current-audio", num, NULL);	
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerchangesubtitletrack(int num)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->playback)
			player->playback->Command(player, PLAYBACK_SWITCH_SUBTITLE, (void*)&num);
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		printf("player: set subtitle: %d\n", num);
		if(pipeline != NULL)
			g_object_set(G_OBJECT(pipeline), "current-text", num, NULL);
		subtitleflag = 1;	
	#endif
#if defined (EXTGST)
	}
#endif
}

void playerstopsubtitletrack()
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
	#ifdef EPLAYER3
		if(player && player->output && player->output->subtitle)
			player->output->subtitle->Command(player, (OutputCmd_t)OUTPUT_STOP, NULL);
	#ifndef EXTEPLAYER3
		if(player && player->container && player->container->assContainer)
		{
			player->container->assContainer->Command(player, CONTAINER_STOP, NULL);
			player->container->assContainer->Command(player, CONTAINER_INIT, NULL);
		}
		if(player && player->manager && player->manager->subtitle)
		{
			int onlycurrent = 1;
			player->manager->subtitle->Command(player, MANAGER_DEL, (void*)&onlycurrent);
		}
	#endif
	#endif
#if defined (EXTGST)
	}
	else
	{
#endif
	#ifdef EPLAYER4
		printf("player: stop subtitle\n");
		subtitleflag = 0;
		//if(pipeline != NULL)
		//	g_object_set(G_OBJECT(pipeline), "current-text", -1, NULL);
		if(status.subtitlethread != NULL)
			status.subtitlethread->aktion = STOP;
	#endif
#if defined (EXTGST)
	}
#endif
}

int playerjumpts(struct service* servicenode, int sekunden, int *startpts, off64_t *poslastpts, off64_t *bitrate, int vpid, int tssize)
{
	int adaptation = 0;
	int payload = 0;
	int pes = 0;
	int tspid = 0;
	
	off64_t pts  = 0;
	uint64_t aktpts = 0;
	long long unsigned int lenpts = 0;
	long long unsigned int startpts1 = 0;
	long long unsigned int endpts = 0;
	long long unsigned int aktbitrate = 0;
	off64_t ziehlpts = 0;

	off64_t curpos = 0;
	off64_t newpos = 0;
	off64_t jump = 0;

	int kleiner = 0;
	int groesser = 0;
	int gleich = 0;
	int len = 0;
	int i = 0;
	int ret = 0;

	if(servicenode == NULL) return -1;

	int buflen = tssize * 15000;
	char *buf = malloc(buflen);
	if(buf == NULL)
		return -1;
	
	curpos = lseek64(servicenode->recsrcfd, 0, SEEK_CUR);	
	int dupfd = open(servicenode->recname, O_RDONLY | O_LARGEFILE);
	newpos = lseek64(dupfd, curpos, SEEK_SET);
	
	if (*startpts == 0)
	{
		if(videogetpts(status.aktservice->videodev, &aktpts) == 0)
		{
				ziehlpts = (aktpts / 90000) + sekunden;
		}
		else
			return 1;
	}
	else
	{
		ziehlpts = *startpts + sekunden;
	}
	*startpts = ziehlpts;

	if(*bitrate == 0)
	{
		lenpts = servicenode->lenpts;
		startpts1 = servicenode->startpts;
		endpts = servicenode->endpts;
		aktbitrate = servicenode->bitrate;
		ret = gettsinfo(dupfd, &lenpts, &startpts1, &endpts, &aktbitrate, servicenode->tssize);
		if(ret != 0) 
		{
			err("can't read ts info");
		}
		else
			*bitrate = aktbitrate;
		newpos = lseek64(dupfd, curpos, SEEK_SET);
	}
	else
		aktbitrate = *bitrate;
		
	if(*poslastpts == 0)
		*poslastpts = curpos;
	
	if(sekunden > 0)
	{
		err("not implemented");
		return 1;
	}	
	else if(sekunden < 0)
	{
		sekunden = sekunden * -1;
		if(aktbitrate != 0)
		{
			jump = (aktbitrate / 8) * sekunden;
			jump = jump + (curpos - *poslastpts);
			jump = jump + (jump % servicenode->tssize);
			newpos = lseek64(dupfd, -jump, SEEK_CUR);
		}
		else
			newpos = lseek64(dupfd, - buflen, SEEK_CUR);
		if(newpos < 0)
			newpos = lseek64(dupfd, tssize, SEEK_SET);
	}
	len = read(dupfd, buf, buflen);
	for(i = 0; i < len; i = i + 1)
	{
		if (buf[i] == 0x47 && buf[i+tssize] == 0x47)
		{
			newpos = lseek64(dupfd, newpos + i, SEEK_SET);
			break;
		}
	}
	if(i >= len)
	{
		newpos = lseek64(dupfd, curpos, SEEK_SET);	
		return 1;
	} 
	while(1)
	{
	len = read(dupfd, buf, buflen);

		if(len > 0)
		{
			for(i = 0; i <= len-tssize; i = i + tssize)
			{
				payload = 0;

				tspid = (buf[i+1] & 0x1F) << 8;
				tspid = tspid + (buf[i+2] & 0xFF);
				pes = buf[i+1] & 0x40;

				if(tspid == vpid)
				{	
					adaptation = buf[i+3] & 0x30;
					if(adaptation == 16)
					{
						payload = 4;
					}
					if(adaptation == 32)
					{
						//printf("adaptation field only\n");
					}
					if(adaptation == 48)
					{
						payload = buf[i+4] & 0xFF;
						payload = payload + 5;
					}
					if(payload != 0)
					{
						if(pes == 64)
						{
							if(buf[i+payload+7] & 0x80) //PTS
							{
								pts = ((unsigned long long)(buf[i+payload+9] & 0xE)) << 29;
								pts |= ((unsigned long long)(buf[i+payload+10] & 0xFF)) << 22;
								pts |= ((unsigned long long)(buf[i+payload+11] & 0xFE)) << 14;
								pts |= ((unsigned long long)(buf[i+payload+12] & 0xFF)) << 7;
								pts |= ((unsigned long long)(buf[i+payload+13] & 0xFE)) >> 1;
								
								if(pts / 90000 == ziehlpts)
								{
									gleich = newpos + i;
									break;
								}
								else if(pts / 90000 > ziehlpts)
								{									
									groesser = newpos + i;
									break;
								}
								else
								{
									kleiner = newpos + i;
								}
							}
						}
					}
				}
			}
			if(gleich != 0)
			{
				close(dupfd);
				free(buf);buf = NULL;
				*poslastpts = lseek64(servicenode->recsrcfd, gleich, SEEK_SET);
				return 0;
			}
			else if(groesser != 0 && kleiner != 0)
			{
				close(dupfd);
				free(buf);buf = NULL;
				*poslastpts = lseek64(servicenode->recsrcfd, kleiner, SEEK_SET);
				return 0;
			}
			else if(groesser != 0)
			{
				if((newpos - buflen)  < 0)
				{
					close(dupfd);
					free(buf);buf = NULL;
					*poslastpts = 0;
					return -1	;
				}
				else
				{
					newpos = lseek64(dupfd, -(buflen * 2), SEEK_CUR);
				}
			}
		}
		else
		{
			if(kleiner == 0)
			{
				close(dupfd);
				free(buf);buf = NULL;
				newpos = lseek64(servicenode->recsrcfd, curpos, SEEK_SET);
				*poslastpts = 0;
				return -1;
			}
			else
			{
				close(dupfd);
				free(buf);buf = NULL;
				*poslastpts = lseek64(servicenode->recsrcfd, kleiner, SEEK_SET);
				return 0;
			}
		}
	}
}

//praez	= 1 .... sekunden
//			=	2 .... zehntel
//			= 3 .... hundertstel
//			= 4 .... volle Uebereinstimmung 
//
//type	= 0 .... alle
//			= 1 .... nur PCR
//			= 2 .... nur Video
//			= 3 .... nur Audio
//
//flag = 0 --> play ts
//flag = 1 --> timeshift
//flag = 2 --> timeshift, not in play mode (only recording)
//flag = 9 --> dataset mode
// 
off64_t playergetptspos(unsigned long long fpts, off64_t pos, int dir, int praez, int type, int flag, char* dsn)
{
	unsigned long long pts;
	int ret = 0, dupfd = -1, left = 0, tssize = 0, recbsize = 0;
	unsigned char* buf = NULL;
	unsigned char *payload;
	int pid = 0, pusi = 0;
	unsigned char* packet;
	struct service* snode;
	
	if(type > 3)
	{
		printf("type %i nicht unterstützt\n", type);
		return -1;
	}
	
	if(flag == 2)
		snode = getservice(RECORDTIMESHIFT, 0);
	else if(flag != 9)
		snode = getservice(RECORDPLAY, 0);
		
	if(flag == 9) 
	{
		tssize = 188;
		recbsize = tssize * 1024 * 10;
		dupfd = open(dsn, O_RDONLY | O_LARGEFILE );
	}
	else
	{
		tssize = snode->tssize;
		recbsize = snode->tssize * 1024 * 10;
		dupfd = open(snode->recname, O_RDONLY | O_LARGEFILE);
	}

	if(dupfd < 0)
	{
		err("copy source fd not ok");
		return -1;
	}

	buf = malloc(recbsize);
	if(buf == NULL)
	{
		err("no mem");
		return -1;
	}
	packet = buf;
	if(dir > 0)  
		pos = lseek64(dupfd, pos, SEEK_SET);
	else
		pos = lseek64(dupfd, pos - recbsize, SEEK_SET);
	
	ret = read(dupfd,  buf, recbsize);
	close(dupfd);
	left = 0;
	
	if(buf[0] != 0x47)
	{
		while(left < tssize)
		{
			if(buf[left] == 0x47) break;
			left++;
		}
		if(left >= tssize)
		{
			free(buf);
			return -1;
		}	
	}
	pts = 0;
	while(left <= recbsize - tssize)
	{
		if(pts != 0)
		{
			switch( praez )
  	  {
        case 1 :	if(fpts / 90000 != pts / 90000)
        						pts = 0;
        					break;
        case 2 :	if(fpts / 9000 != pts / 9000)
        						pts = 0;
        					break;
        case 3 :	if(fpts / 900 != pts / 900)
        						pts = 0;
        					break;		
        case 4 :	if(fpts != pts )
        						pts = 0;
        					break;
				default :	free(buf); return -1; break;
			}
			if(pts != 0)
			{ 	
				pos = pos + left - tssize;
				free(buf);
				return pos;
			}
		}
		packet = buf + left;
		left = left + tssize;
						
		pid = ((packet[1] << 8) | packet[2]) & 0x1FFF;
		pusi = !!(packet[1] & 0x40);
		//check for adaption field
		if(packet[3] & 0x20)
		{
			if(type > 1)continue;
			if(packet[4] >= 183) continue;
			if(packet[4])
			{
				if(packet[5] & 0x10) //PCR present
				{
					pts = ((unsigned long long)(packet[6] & 0xFF)) << 25;
					pts |= ((unsigned long long)(packet[7] & 0xFF)) << 17;
					pts |= ((unsigned long long)(packet[8] & 0xFE)) << 9;
					pts |= ((unsigned long long)(packet[9] & 0xFF)) << 1;
					pts |= ((unsigned long long)(packet[10] & 0x80)) >> 7;
					continue;
				}
			}
			payload = packet + packet[4] + 4 + 1;
		} else
			payload = packet + 4;
		
		if(type == 1) continue;
		if(!pusi) continue;
		
		if (payload[0] || payload[1] || (payload[2] != 1))
			continue;
		
			//stream use extension mechanism def in ISO 13818-1 Amendment 2
		if(payload[3] == 0xFD)
		{
			if(payload[7] & 1) //PES extension flag
			{
				int offs = 0;
				if(payload[7] & 0x80) offs += 5; //pts avail
				if(payload[7] & 0x40) offs += 5; //dts avail
				if(payload[7] & 0x20) offs += 6; //escr avail
				if(payload[7] & 0x10) offs += 3; //es rate
				if(payload[7] & 0x8) offs += 1; //dsm trickmode
				if(payload[7] & 0x4) offs += 1; //additional copy info
				if(payload[7] & 0x2) offs += 2; //crc
				if(payload[8] < offs) continue;

				uint8_t pef = payload[9 + offs++]; //pes extension field
				if(pef & 1) //pes extension flag 2
				{
					if(pef & 0x80) offs += 16; //private data flag
					if(pef & 0x40) offs += 1; //pack header field flag
					if(pef & 0x20) offs += 2; //program packet sequence counter flag
					if(pef & 0x10) offs += 2; //P-STD buffer flag
					if(payload[8] < offs) continue;

					uint8_t stream_id_extension_len = payload[9 + offs++] & 0x7F;
					if(stream_id_extension_len >= 1)
					{
						if(payload[8] < (offs + stream_id_extension_len)) continue;
						//stream_id_extension_bit (should not set)
						if(payload[9 + offs] & 0x80) continue;
						switch(payload[9 + offs])
						{
							case 0x55 ... 0x5f: break; //VC-1
							case 0x71: break; //AC3 / DTS
							case 0x72: break; //DTS - HD
							default:
								printf("skip unknwn stream_id_extension %02x\n", payload[9 + offs]);
								continue;
						}
					}
					else
						continue;
				}
				else
					continue;
			}
			else
				continue;
		}
		//drop non-audio, non-video packets because other streams
		//can be non-compliant.
		//0xC0 = audio, 0xE0 = video
		else if(((payload[3] & 0xE0) != 0xC0) && ((payload[3] & 0xF0) != 0xE0))
			continue;

		if((payload[7] & 0x80) && ((payload[3] & 0xF0) != 0xE0) && (type == 0 || type == 2)) //PTS video
		{
			pts = ((unsigned long long)(payload[9] & 0xE)) << 29;
			pts |= ((unsigned long long)(payload[10] & 0xFF)) << 22;
			pts |= ((unsigned long long)(payload[11] & 0xFE)) << 14;
			pts |= ((unsigned long long)(payload[12] & 0xFF)) << 7;
			pts |= ((unsigned long long)(payload[13] & 0xFE)) >> 1;
			continue;
		}
		if((payload[7] & 0x80) && ((payload[3] & 0xE0) != 0xC0) && (type == 0 || type == 3)) //PTS audio
		{
			pts = ((unsigned long long)(payload[9] & 0xE)) << 29;
			pts |= ((unsigned long long)(payload[10] & 0xFF)) << 22;
			pts |= ((unsigned long long)(payload[11] & 0xFE)) << 14;
			pts |= ((unsigned long long)(payload[12] & 0xFF)) << 7;
			pts |= ((unsigned long long)(payload[13] & 0xFE)) >> 1;
			continue;
		}
	}
	free(buf);
	return recbsize * -1;
}

#ifdef EPLAYER4
/* Extract some metadata from the streams and print it on the screen */
static void analyze_streams(CustomData *data)
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 1)
	{
#endif
		gint i;
		GstTagList *tags;
		gchar *str;
		guint rate;

		/* Read some properties */
		g_object_get(pipeline, "n-video", &data->n_video, NULL);
		g_object_get(pipeline, "n-audio", &data->n_audio, NULL);
		g_object_get(pipeline, "n-text", &data->n_text, NULL);

		g_print("%d video stream(s), %d audio stream(s), %d text stream(s)\n", data->n_video, data->n_audio, data->n_text);

		g_print ("\n");
		for(i = 0; i < data->n_video; i++)
		{
			tags = NULL;
			/* Retrieve the stream's video tags */
			g_signal_emit_by_name(pipeline, "get-video-tags", i, &tags);
			if(tags)
			{
				g_print("video stream %d:\n", i);
				gst_tag_list_get_string(tags, GST_TAG_VIDEO_CODEC, &str);
				g_print("  codec: %s\n", str ? str : "unknown");
				g_free(str);
				gst_tag_list_free(tags);
			}
		}

		g_print("\n");
		for(i = 0; i < data->n_audio; i++)
		{
			tags = NULL;
			g_signal_emit_by_name(pipeline, "get-audio-tags", i, &tags);
			if(tags)
			{
				/* Retrieve the stream's audio tags */
				g_print("audio stream %d:\n", i);
				if(gst_tag_list_get_string (tags, GST_TAG_AUDIO_CODEC, &str))
				{
					g_print("  codec: %s\n", str);
					g_free(str);
				}
				if(gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str))
				{
					g_print("  language: %s\n", str);
					g_free(str);
				}
				if(gst_tag_list_get_uint (tags, GST_TAG_BITRATE, &rate))
				{
					g_print("  bitrate: %d\n", rate);
				}
				gst_tag_list_free(tags);
			}
		}

		g_print("\n");
		for(i = 0; i < data->n_text; i++)
		{
			tags = NULL;
			/* Retrieve the stream's subtitle tags */
			g_print("subtitle stream %d:\n", i);
			g_signal_emit_by_name(pipeline, "get-text-tags", i, &tags);
			if(tags)
			{
				if(gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str))
				{
					g_print("  language: %s\n", str);
					g_free(str);
				}
				gst_tag_list_free(tags);
			}
			else
			{
				g_print("  no tags found\n");
			}
		}

		g_object_get(pipeline, "current-video", &data->current_video, NULL);
		g_object_get(pipeline, "current-audio", &data->current_audio, NULL);
		g_object_get(pipeline, "current-text", &data->current_text, NULL);

		g_print("\n");
		g_print("Currently playing video stream %d, audio stream %d and subtitle stream %d\n", data->current_video, data->current_audio, data->current_text);
		g_print("Type any number and hit ENTER to select a different subtitle stream\n");
#if defined (EXTGST)
	}
#endif
}
#endif

#ifdef EPLAYER4
void playersend_ff_fr_event(gdouble rate) {
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 1)
	{
#endif
		gint64 position;
		GstFormat format = GST_FORMAT_TIME;
		GstEvent *seek_event;
	   
		/* Obtain the current position, needed for the seek event */
	#if GST_VERSION_MAJOR < 1
		if (!gst_element_query_position (pipeline, &format, &position)) {
	#else
		if (!gst_element_query_position (pipeline, format, &position)) {
	#endif
			g_printerr ("Unable to retrieve current position.\n");
			return;
		}
	   
		/* Create the seek event */
		if (rate > 0)
		{	
			seek_event = gst_event_new_seek (rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,GST_SEEK_TYPE_SET, position, GST_SEEK_TYPE_NONE, 0);
	  } 
		else
		{
			seek_event = gst_event_new_seek (rate, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE,GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_SET, position);
		}
	   
		if (video_sink == NULL) {
			/* If we have not done so, obtain the sink through which we will send the seek events */
			g_object_get (pipeline, "video-sink", &video_sink, NULL);
		}
	   
		/* Send the event */
		gst_element_send_event (video_sink, seek_event);
	   
		g_print ("Current rate: %g\n", rate);
#if defined (EXTGST)
	}
#endif
}
#endif

#ifdef EXTEPLAYER3
char* getsubtext()
{
#if defined (EXTGST)
	if(getconfigint("extplayer_type", NULL) == 0)
	{
#endif
		char* tmpstr = NULL;
		if(player && player->container && player->container->selectedContainer)
			player->container->selectedContainer->Command(player, CONTAINER_GET_SUBTEXT, (void*)&tmpstr);

	//	if(status.subtitlethread == NULL)
		if(status.play == 1 && status.pause == 0 && tmpstr != NULL)
		{
			printf("[player.h] getsubtext tmpstr: %s\n", tmpstr);
			status.subtitlethread = addtimer(&playersubtitle_ext_thread, START, 10000, 1, (void*)tmpstr, NULL, NULL);
		}
		return subtext;
#if defined (EXTGST)
	}
#endif
	// add this line complile: warning: control reaches end of non-void function
	return NULL;
}
#endif

#endif
