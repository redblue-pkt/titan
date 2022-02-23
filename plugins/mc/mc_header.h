#ifndef MC_HEADER_H
#define MC_HEADER_H

//menu.h 
struct skin* menu(struct skin* menu);

// mc_global.h
void picplayer(struct skin* picscreen, struct skin* picture, struct skin* picname, char* filename, int flag);

// mc_settings.h
void screenmc_settings();

// mc_view.h
int screenmc_view();

// mc_audioplayer_settings.h
void screenmc_audioplayer_settings();
void mc_audioplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, struct skin* b12, struct skin* b13, char* filename);

// mc_videoplayer_settings.h
void screenmc_videoplayer_settings();

// mc_pictureplayer_settings.h
void screenmc_pictureplayer_settings();

// mc_iptvplayer_settings.h
void screenmc_iptvplayer_settings();
void mc_iptvplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, struct skin* b12, struct skin* b13, char* filename);
char* gethttps(char* url, char* localfile, char* data, char* user, char* pass, char* referer, int flag);

// record
void createrecthumbfirstthread(struct stimerthread* self, char* dname, char* filename);

#endif
