#ifndef HEADER_H
#define HEADER_H

//linkedchannel.h
void screenlinkedchannel();
struct linkedchannel* getlinkedchannel(struct channel* chnode, int serviceid, unsigned long transponderid);

//httpdsettings.h
void screenhttpdsettings();

//ca.h
int getfreecasession(struct dvbdev* dvbnode, int type, int value);

//help.h
void screenhelp(struct skin* node);

//restoredefault.h
void screenrestoredefault();

//ca.h
void caappmenu(struct dvbdev* dvbnode);

//cam.h
int getcaservicebyslot(struct caslot* caslot, int flag);
void caservicedel(struct service* snode, struct caslot* caslot);
void sendcapmt(struct service* node, int clear, int flag);

//gmultiepg.h
void screengmultiepg(struct channel* chnode, struct epg* epgnode, int flag);

//multiepg.h
void screenmultiepg(struct channel* chnode, struct epg* epgnode, int flag);

//harddisk.h
void hddfsck(char* dev);
void hddformat(char* dev, char* filesystem);
int addhddall();

//autores.h
void autoresthreadfunc(struct stimerthread* self, char* text, int timeout);
void screenautores(char* text, int timeout, int flag);

// timeshift.h
void timeshiftstop(int flag);

// keyaktions.h
void keyactions(int key, int flag);
void keyactions_setres();

//dmxdev.h
struct dvbdev* dmxgetlast(int adapter);

// harddisk.h
char* harddisk_listbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng);

// videodev.h
int videoreadqwidth(struct dvbdev* node);

// scan.h
int findchannel(struct transponder* tpnode, unsigned char *buf, uint8_t* lastsecnr, struct skin* scan, struct skin* listbox, int flag);
void screenscanconfig(int flag);

// mainplaylist.h
struct mainplaylist* screenmainplaylist(int flag);

// screensaver.h
int showscreensaver();
int initscreensaver();
int deinitscreensaver();

// screensaveradjust.h
void screenscreensaveradjust();

//dir.h
char* screendir(char* path, char* mask, char* selection, int *dirrcret, char* ext, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int prozwidth, int height, int prozheight, int flag);

// mainplaylist.h
int writeallplaylist();
int writemainplaylist(const char *filename);
struct mainplaylist* getmainplaylistbyplaylistpointer(struct playlist* playlistnode);

// screensaveradjust.h
void screensaveradjust();

//download.h
int screendownload(char* title, char* host, char* page, int port, char* filename, int flag);

// epgrecord.h
void freeepgrecord(struct epgrecord** first);

//titan.c
void oshutdown(int exitcode, int flag);
int createstartscreen();

// powerofftimer.h
void screenpowerofftimer(void);

// videosettings.h
void screenvideosettings();

// harddisk.h
void screenharddisk(int mode);
void screenharddisksleep();

// network.h
void screennetwork(int mode);
void screennetwork_adapter();
void screennetwork_restart();
void screennetwork_test();

//channel.h
int writechannel(const char *filename);

//transponder.h
struct transponder* gettransponder(unsigned long transponderid);
int writetransponder(const char *filename);

//sat.h
int writesat(const char *filename);

//provider.h
int writeprovider(const char *filename);

//httpd.h
void httpdthreadfunc(struct stimerthread* timernode);
void sendoktext(int* connfd, char* text, int auth);

//skinconfig.h
int getskinconfigint(char *key, char *ext);

//record.h
struct service* getrecordbyname(char* recname, int type);

// play.h
void screenplay(int startfolder, int flag);

//inetwork.h
struct inetwork* getinetworkbydevice(char* device);

//fb.h
void blitfb();
void changefbresolution(char *value);
void setfbtransparent(int value);

//timerthread.h
struct stimerthread* addtimer(void* func, int aktion, int delay, int count, void* param1, void* param2, struct stimerthread* last);
void deltimer(struct stimerthread *tnode);

//epgscanlist.h
struct epgscanlist* getepgscanlist(int serviceid, int transponderid);
int writeepgscanlist(const char *filename);

//rectimer.h
int writerectimer(const char *filename, int flag);
int recordcheckcrypt(struct dvbdev* fenode, int servicetype);
int recordstart(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode);
char* recordcheckret(struct stimerthread* timernode, int ret, int flag);
void checkrecepgret(int ret);
int addrecepg(struct channel* chnode, struct epg* epgnode, char* channellist);
struct rectimer* getrectimerbyservice(struct service* servicenode);
void delrectimer(struct rectimer* rectimernode, int write, int flag);
int rectimergetaktday();

//dvb.h
unsigned char* dvbgetsdt(struct dvbdev* fenode, int secnr, int timeout);
int dvbreadfd(int fd, unsigned char *buf, int pos, int count, int tout);
int dvbwrite(int fd, unsigned char* buf, int count, int tout);
int dvbgetdate(time_t* time, int timeout);

//pin.h
int screenpincheck(int type, char* pin);
void screenpin();

//videomode.h
void screenvideomode();

//pic.h
void delpic(char* name);
struct pic* getpic(char* name);
void delmarkedpic(int del);

//showiframe.h
int singlepicstart(const char *filename);

//player.h
void playerff(int speed);
void playerfr(int speed);
void playercontinue();
void playerpause();
int playerstop();
void playerseek(float sec);
int playerstart(char* file);
unsigned long long int playergetpts();
double playergetlength();

//filelist.h
//void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview);
void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview, char* selection);

//plugin.h
struct skin* getplugin(char* pluginname);

//listbox.h
int setlistboxselection(struct skin* listbox, char* childname);

//list.h
struct clist* addlist(struct clist **clist, char *key1, char *value1);
void freelist(struct clist** clist);
struct clist* addlisttmp(struct clist **clist, char *key1, char *value1);

//choicebox.h
int setchoiceboxselection(struct skin* choicebox, char* value);
int addchoicebox(struct skin* choicebox, char* value, char* text);

//ownconfig.h
char* getownconfig(char *key);
char* getownconfigq(char *key);
struct clist* addownconfigscreentmp(char *key, struct skin *node);
struct clist* addownconfigscreenqtmp(char *key, struct skin *node);
struct clist* addownconfigscreentmpcheck(char *key, struct skin *node, char* check);
int writeownconfigtmp();
void delownconfigtmpall();

//infobar.h
void fillinfobar();

//menulist.h
char* menulistbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng, int flag);

//skinfunc.h
char* gettime(char* format);

//skin.h
int setnodeattr(struct skin* node, struct skin* parent);
void clearscreennolock(struct skin* node);
void clearshadow(struct skin* node);
char* savescreen(struct skin* node);
void restorescreen(char* buf, struct skin* node);
char* changepicpath(char* picname);
long convertcol(char *value);
void delmarkedscreen(int del);
char* get_ipk_update();
void delscreen(struct skin *node);
void delscreennode(struct skin *node, char* nodename);
int readscreen(char *filename, int del, int flag);
int readjpg(const char* filename, unsigned long* width, unsigned long* height, unsigned long* rowbytes, int* channels, unsigned char **mem, int *memfd);
unsigned char* readpng(const char* filename, unsigned long* width, unsigned long* height, unsigned long* rowbytes, int* channels, int posx, int posy, int mwidth, int mheight, int halign, int valign);
void delmarkedscreennodes(struct skin* node, int mark);
void clearscreen(struct skin* node);
struct skin* getscreennode(struct skin *node, char* nodename);
struct skin* getscreen(char* screenname);
int calcrheight(struct skin* node, struct skin* parent);
int drawscreen(struct skin* node, int flag);
struct skin* addscreennode(struct skin* node, char* line, struct skin* last);
int changetext(struct skin* node, char* text);
int changetext2(struct skin* node, char* text);
int changeinput(struct skin* node, char* text);
int changename(struct skin* node, char* text);
int changeret(struct skin* node, char* text);
int changepic(struct skin* node, char* text);
int changetitle(struct skin* node, char* text);
int changefont(struct skin* node, char* text);
int changemask(struct skin* node, char* text);
int changepicmem(struct skin* node, char* text, int del);
int convertxmlentry(char *value, uint8_t *proz);

//volume.h
void screenvolumeup();
void screenvolumedown();

//menu.h
int menucall(struct skin* menunode, struct skin* menuentry, int check);

//textbox.h
int textbox(char* title, char* text, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int height, int timeout, int flag);

//config.h
struct clist* addconfig(char *key, char *value);
void delconfigtmpall();
struct clist* addconfigscreencheck(char *key, struct skin *node, char* check);
struct clist* addconfigdef(char *key, char *value);
struct clist* addconfigtmp(char *key, char *value);
char* getconfig(char *key, char *ext);
char* getconfignotmp(char *key, char *ext);
int getconfigint(char *key, char* ext);
void delconfig(char *key);
void delconfigtmp(char *key);
struct clist* addconfiginttmp(char *key, int value);

//rc.h
int waitrc(struct skin* owner, unsigned int timeout, int flag);
void delownerrc(struct skin* owner);
void delrc(int key, struct skin* owner, struct skin* screennode);

//rcfunc.h
void inputboxchar(struct skin* screen, struct skin* inputbox, char zeichen, int flag);
void inputboxff(struct skin* screen, struct skin* inputbox, int flag);
void inputboxfr(struct skin* screen, struct skin* inputbox, int flag);
void inputboxright(struct skin* screen, struct skin* inputbox, int flag);
void inputboxleft(struct skin* screen, struct skin* inputbox, int flag);
int addscreenrc(struct skin* screen, struct skin* node);
void checkinputboxnumright(struct skin* inputbox);
int delscreenrc(struct skin* screen, struct skin* node);

//spinner.h
void screenspinner();

//global.h
char* string_shortname(char *tmpfilename, int mode);
off64_t getfilesize(char* name);
struct splitstr* strsplit(char *str, char *tok, int* count);
char* strstrip(char *text);
char* fixip(char* ipinput, int flag);
void m_lock(pthread_mutex_t *mutex, int flag);
void m_unlock(pthread_mutex_t *mutex, int flag);
int setcolorformat(char* value);
int setvideomode(char* value);
char* convert_timesec(int sec);
int writesys(const char *filename, char *value, int flag);
int setsystime(time_t* newtime);
int checkbox(char* box);
int setpolicy(char* value);
char* getpolicy();
char* string_newline(char* str);
char* get_label(char* device);
char* string_quote(char* str);
int file_exist(char* filename);
int string_find(char* str, char* filename);
char* createpath(char* dir, char* file);
char* get_ipk_install(char* ipk);
char* get_ipk_tmpinstall(char* ipk);
char* get_ipk_listinstall();
char* get_ipk_tmplistinstall();
char* string_toupper(char *str);
char* get_ipk_remove(char* ipk);
char* get_ipk_section();
char* get_ipk_list(char* section);
char* string_tolower(char *str);
int isfile(char* name);
char* command(char* input);
char* oitoa(int value);
char* olutoa(unsigned long value);
int ostrcmp(char* value1, char* value2);
char* ostrcat(char* value1, char* value2, int free1, int free2);
int ostrncmp(char* value1, char* value2, int count);
char* readsys(const char *filename, int line);
char* string_replace_remove_last_chars(char *search, char *replace, char *string, int free1);
char* string_replace(char *search, char *replace, char *string, int free1);
int writesysint(const char *filename, int value, int flag);
char* string_remove_whitechars(char *text);
void setfanspeed(int speed, int aktion);
int writeallconfig(int flag);
int checkemu();

//rcconfig.h
int getrcconfigint(char *key, char* ext);

//service.h
struct service* getservicebyservice(struct service* node, int flag);
struct service* getservicebychannel(struct channel* chnode);
struct service* checkservice(struct service* node);
struct service* addservice(struct service* last);
void delservice(struct service* snode, int flag);
int servicestop(struct service *node, int clear, int flag);
int servicestart(struct channel* chnode, char* channellist, char* pin, int flag);
void serviceresetchannelinfo(struct channel* chnode);

//mainbouquets.h
int writemainbouquet(const char *filename);
int writeallbouquet();
struct mainbouquet* getmainbouquetbybouquetpointer(struct bouquet* bouquetnode);

//listbox.h
struct skin* addlistbox(struct skin* screen, struct skin* listbox, struct skin* last, int del);

//bouquets.h
void delbouquetbychannel(int serviceid, int transponderid);
struct bouquet* getbouquetbychannelmain(int serviceid, int transponderid);
void recalcbouquetnr();

//audiotrack.h
struct audiotrack* addaudiotrack(struct channel* chnode, char* langdesc, int pid, int audiocodec, struct audiotrack* last);
void freeaudiotrack(struct channel* chnode);

//subtitle.h
struct subtitle* addsubtitle(struct channel* chnode, int subtype, char* langdesc, int pid, int type, int id1, int id2, struct subtitle* last);
void freesubtitle(struct channel* chnode);
int subtitlestop(int flag);
int subtitlepause(int flag);

//linkedchannel.h
struct linkedchannel* addlinkedchannel(struct channel* chnode, int serviceid, unsigned long transponderid, struct linkedchannel* last);
void freelinkedchannel(struct channel* chnode);

//zap.h
void zapup();
void zapdown();

//epg.h
void screensingleepg(struct channel* chnode, struct epg* epgnode, int flag);
void screenepg(struct channel* chnode, struct epg* epgnode, int flag);
int writeepg(const char* filename);
void freeepg(struct channel* chnode);
struct epg* getepgakt(struct channel* chnode);
struct epg* getepgbytime(struct channel* chnode, time_t akttime);
char* epgdescunzip(struct epg* epgnode);

//standby.h
void screenstandby();

//channelcache.h
void delchannelcache(int serviceid, unsigned long transponderid);
struct channelcache* modifychannelcache(int serviceid, unsigned long transponderid, struct channel*);
struct channel* getchannel(int serviceid, unsigned long transponderid);

// vfd.h
int setvfdicon(vfdicons id, int onoff);
int writevfd(char *value);

#endif
