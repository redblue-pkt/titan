#ifndef TITHEK_HEADER_H
#define TITHEK_HEADER_H

//char* createfilename(char* title, struct skin* listbox);
char* createfilename(char* title, char* title2, int flag);
char* jsunpack(char* input);
char* gethttps(char* url, char* localfile, char* data, char* user, char* pass, char* referer, int flag);
//int amazon_login();
//void amazon_init(char* titheklink, char* tithekfile);
//void amazon_deinit();
void localparser_init(char* titheklink, char* tithekfile, int flag);
int b64dec(char* dest, char* src);
char* localparser_hoster(char* link);
int localparser_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
char* list_hoster_streams(char* input);

void titheklog(int debuglevel, char* name1, char* name2, char* name3, char* name4, char* content)
{
	char* tmpstr = NULL;
	
	if(debuglevel != 99) return;

	tmpstr = ostrcat(tmpstr, name1, 1, 0);
	if(name2 != NULL)
	{
		tmpstr = ostrcat(tmpstr, "_", 1, 0);
		tmpstr = ostrcat(tmpstr, name2, 1, 0);
	}
	if(name3 != NULL)
	{
		tmpstr = ostrcat(tmpstr, "_", 1, 0);
		tmpstr = ostrcat(tmpstr, name3, 1, 0);
	}
	if(name4 != NULL)
	{
		tmpstr = ostrcat(tmpstr, "_", 1, 0);
		name4 = string_replace_all("/", ".", name4, 0);
		name4 = stringreplacecharonce(name4, '?', '\0');
		name4 = stringreplacecharonce(name4, '"', '\0');
		name4 = stringreplacecharonce(name4, '+', '\0');
		name4 = stringreplacecharonce(name4, '\'', '\0');
		tmpstr = ostrcat(tmpstr, name4, 1, 0);
	}
	
	writesys(tmpstr, content, 0);
	free(tmpstr);
}
char* hoster(char* url);
void screentithekplay(char* titheklink, char* title, int first);
void screentithek_settings();
char* oltostr(long val, unsigned base);
//int myvideo_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int youtube_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr);
int kinox_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int kinox_search_cast(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr);
//int solarmovie_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int amazon_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int kinox_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int kinox_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int solarmovie_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie4k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie4k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int movie4k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie4k_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie4k_series_listed(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie2k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie2k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int movie2k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie2k_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//int movie2k_series_listed(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
//char* now(char* link, char* url, char* name, char* title, int flag);
//char* nowtv(char* link);
//char* beeg(char* link);
//char* mlehd(char* link);
char* kinox(char* link);
//char* movie4k(char* link);
//char* movie2k(char* link);
char* xvideos(char* link);
//char* solarmovie(char* link);
char* ard(char* link);
char* zdf(char* link);
//char* myvideo_hoster(char* link);
//char* myvideo(char* link);
//char* youtube_hoster(char* link);
//char* youtube(char* link);
//char* amazon_hoster(char* link);
//char* amazon(char* link);
char* putlocker(char* link);
char* filenuke(char* link);
char* streamcloud(char* link);
char* flashx(char* link);
char* vidstream(char* link);
char* xvidstage(char* link);
char* nowvideo(char* link);
char* movshare(char* link);
char* movreel(char* link);
char* novamov(char* link);
char* divxstage(char* link);
char* primeshare(char* link);
char* faststream(char* link);
char* played(char* link);
char* videoweed(char* link);
char* tvtoast(char* link);
char* firedrive(char* link);
char* shared(char* link);
char* thefile(char* link);
char* promptfile(char* link);
char* letwatch(char* link);
char* vidbull(char* link);
char* vodlocker(char* link);
char* vidto(char* link);
char* thevideo(char* link);
char* mightyupload(char* link);
char* cloudzilla(char* link);
char* vivo(char* link);
char* nbastream(char* link);
char* giga(char* link);
//char* nbaondemand(char* link);
char* p2pcast(char* link);
char* vidzi(char* link);
char* vidag(char* link);
char* streamlive(char* link, int incount);
char* cricfree(char* link, int incount);
char* zerocast(char* link, int incount);
char* usachannels(char* link, int incount);
char* liveonlinetv(char* link, int incount);
char* akamaistream(char* link, int incount);
int internetradio_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int internettv_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int kinox_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int movie4k_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int movie2k_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int solarmovie_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int myvideo_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int youtube_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int ard_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int zdf_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int tectime_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int giga_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int beeg_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int xvideos_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int netzkino_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int mlehd_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int filmon_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int tvtoast_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int nowtv_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//int amazon_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int all_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
//void mlehd_userauth();
int screenlistbox(struct skin* grid, struct skin* listbox,struct skin* countlabel, char* title, char* titheklink, int* pagecount, int* tithekexit, int* oaktpage, int* oaktline, int* ogridcol, int flag, int cflag);

#endif
