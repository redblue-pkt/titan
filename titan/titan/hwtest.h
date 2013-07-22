#ifndef HWTEST_H
#define HWTEST_H

//DVB-S2
#define TEST_S2_ORBITALPOS 0
#define TEST_S2_FREQ 11303000
#define TEST_S2_SYMBOL 22000000
#define TEST_S2_POL _POLAR_HOR
#define TEST_S2_VIDEO_PID 3583
#define TEST_S2_AUDIO_PID 3587
#define TEST_S2_PCR_PID 3583

//test_channel.videoType = V_H264; // 1;
//test_channel.audioType = A_AC3; // 6;

//DVB-T 
#define TEST_T2_FREQ  4820
#define TEST_T2_BW   8
#define TEST_T2_VIDEO_PID 545
#define TEST_T2_AUDIO_PID 546
#define TEST_T2_PCR_PID 545

//test_channel.videoType = V_MP2; //0;
//test_channel.audioType = A_MP1; // 0;

//DVB-C
#define TEST_C2_FREQ   4500
#define TEST_C2_SYMBOL   6900
#define TEST_C2_VIDEO_PID  201
#define TEST_C2_AUDIO_PID  301
#define TEST_C2_PCR_PID  201
#define TEST_C2_CAB_QAM     _QAMMODE_64

//test_channel.videoType = V_MP2; //0;
//test_channel.audioType = A_MP1; // 0;

void screenhwtest()
{
	struct menulist* mlist = NULL, *mbox = NULL;
	
	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			if(dvbnode->feinfo->type == FE_QPSK)
			{
				char* tmpstr = ostrcat("[S]Lock & Search", " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
			if(dvbnode->feinfo->type == FE_OFDM)
			{
				char* tmpstr = ostrcat("[T]Lock & Search", " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
			if(dvbnode->feinfo->type == FE_QAM)
			{
				char* tmpstr = ostrcat("[C]Lock & Search", " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
		}
	}
	addmenulist(&mlist, "Front Display", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Front Key", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Color Bar", NULL, NULL, 0, 0);
	addmenulist(&mlist, "SCART 4:3 / 16:9", NULL, NULL, 0, 0);
	addmenulist(&mlist, "USB Port", NULL, NULL, 0, 0);
	addmenulist(&mlist, "CAM1", NULL, NULL, 0, 0);
	addmenulist(&mlist, "EXIT", NULL, NULL, 0, 0);
	
	while(1)
	{
		mbox = menulistbox(mlist, NULL, "Hardware Test", NULL, NULL, 1, 0);
		
		if(mbox != NULL)
		{
			if(ostrstr(mbox, "[S]Lock & Search") == 0)
			{
				struct transponder* tp = NULL;
				tp = createtransponder(uint64_t id, FE_QPSK, TEST_S2_ORBITALPOS, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
				if(tp != NULL)
				{
				
				}
			}
			
			if(ostrstr(mbox, "[T]Lock & Search") == 0)
			{
				//struct transponder* createtransponder(uint64_t id, FE_OFDM, int orbitalpos, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
			}
			
			if(ostrstr(mbox, "[C]Lock & Search") == 0)
			{
				//struct transponder* createtransponder(uint64_t id, FE_QAM, int orbitalpos, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
			}
			
			if(ostrcmp(mbox, "Front Display") == 0)
			{
				char* tmpstr = NULL;
				for(i = 0; i < 9; i++)
				{
					tmpstr = ostrcat(oitoa(i), oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					writevfd(tmpstr);
					free(tmpstr); tmpstr = NULL;
					usleep(300000);
				}
				free(tmpstr); tmpstr = NULL;
			}
			
			if(ostrcmp(mbox, "Front Key") == 0)
			{
			
			}
			
			if(ostrcmp(mbox, "Color Bar") == 0)
			{
			
			}
			
			if(ostrcmp(mbox, "SCART 4:3 / 16:9") == 0)
			{
			
			}
			
			if(ostrcmp(mbox, "USB Port") == 0)
			{
			
			}
			
			if(ostrcmp(mbox, "CAM1") == 0)
			{
			
			}
			
			if(ostrcmp(mbox, "EXIT") == 0)
			{
				break;
			}
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
}

#endif
