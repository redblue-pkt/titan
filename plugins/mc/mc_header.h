#ifndef MC_HEADER_H
#define MC_HEADER_H

void picplayer(struct skin* picscreen, struct skin* picture, struct skin* picname, char* filename, int flag);
struct clist* addconfigscreen(char *key, struct skin *node);

//void screenmc_audioplayer();
void mc_audioplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, char* filename);

//void mc_videoplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, char* filename);
void screenplayinfobar(char* file, int mode, int playertype, int flag);
void playrcblue(char* file, int playinfobarstatus, int playertype, int flag);
void playrcyellow(char* file, int playinfobarstatus, int playertype, int flag);
void playrcred(char* file, int playinfobarstatus, int playertype, int flag);
void playrcgreen(char* file, int playinfobarstatus, int playertype, int flag);
void playrctext(char* file, int playinfobarstatus, int playertype, int flag);
void playrcok(char* file, int playinfobarstatus, int playertype, int flag);
void playrcplay(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);

//global.h
void setosdtransparent(int value);

// mc_audioplayer.h
//void playereof(struct skin* apskin, struct skin* filelist, struct skin* listbox, struct skin* filelistpath, struct skin* b2, int* skip, int* eof, int* playlist, int flag);
//void playerrandom(struct skin* apskin, struct skin* filelist, struct skin* listbox, struct skin* b3, int* playlist, int flag);
//void showplaylist(struct skin* apskin, struct skin* filelistpath, struct skin* filelist, struct skin* listbox1, struct skin* b2, int mode, int* playlist, int* eof);
//void mc_audioplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, char* filename);

// playlist.h
void getplaylistmaxold(struct skin* playlist, int* maxdirs, int* maxfiles);
struct skin* getplaylistrandomold(struct skin* playlist, int maxdirs, int maxfiles);

// menu.h
struct skin* menu(struct skin* menu);

// filelist.h
void getfilelistmax(struct skin* filelist, int* maxdirs, int* maxfiles);
//void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview, char* selection);
void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview, char* selection);
struct skin* getfilelistrandom(struct skin* filelist, int maxdirs, int maxfiles);

// play.h
void playrcjumpr(char* file, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcjumpf(char* file, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcpause(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcff(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcfr(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playwritevfd(char* file);

// rc.h
int writerc(int keycode);

// mc_settings.h
void screenmc_settings();

// mc_view.h
int screenmc_view();

// mc_sort.h
int screenmc_sort();

// mc_audioplayer_settings.h
void screenmc_audioplayer_settings();

// mc_videoplayer_settings.h
void screenmc_videoplayer_settings();

// mc_pictureplayer_settings.h
void screenmc_pictureplayer_settings();

#endif
