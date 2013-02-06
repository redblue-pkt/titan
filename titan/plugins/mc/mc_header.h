#ifndef MC_HEADER_H
#define MC_HEADER_H

void picplayer(struct skin* picscreen, struct skin* picture, struct skin* picname, char* filename, int flag);

//void screenmc_audioplayer();
void mc_audioplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, struct skin* b12, struct skin* b13, char* filename);

//void mc_videoplayer_infobar(struct skin* apskin, struct skin* infobar, struct skin* spos, struct skin* slen, struct skin* sreverse, struct skin* sprogress, char* filename);
void screenplayinfobar(char* file, char* showname, int mode, int playertype, int flag);
void playrcblue(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcyellow(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcred(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcgreen(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrctext(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcok(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcplay(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);

// mc_settings.h
void screenmc_settings();

// mc_view.h
int screenmc_view();

// mc_audioplayer_settings.h
void screenmc_audioplayer_settings();

// mc_videoplayer_settings.h
void screenmc_videoplayer_settings();

// mc_pictureplayer_settings.h
void screenmc_pictureplayer_settings();

#endif
