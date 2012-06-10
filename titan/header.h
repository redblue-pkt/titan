/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef HEADER_H
#define HEADER_H

//mediadbcache.h
struct mediadb* getmediadb(char* file);

//mediadbsettings.h
void screenmediadbsettings();

//mediadb.h
void freemediadbcontent(struct mediadb* node);
int findfiles(char* dirname, int type) ;
struct mediadbfilter* getlastmediadbfilter(struct mediadbfilter* node, int flag);
void getmediadbcounts(int* video, int* audio, int* picture);
int readmediadb(const char* filename, int type, int flag);
void mediadbscan();
void createmediadbfilter(int type, char* search, int flag);
struct mediadbfilter* getmediadbfilterrandom(int maxentry);
int getmediadbfiltercount();
void freemediadbfilter(int flag);
void freemediadb(int flag);
void freemediadbcategory(int flag);
int delmediadb(struct mediadb* mnode, int flag);

//eraseswap.h
void screeneraseswap();

//softcam.h
void screensoftcam();

//system_mixed.h
void screensystem_eraseswap();
void screensystem_wizard();

// info.h
void screengetserial();
void screensystem_info(int mode);

//system_update.h
void screensystem_update(int mode);

//system_backup.h
void screensystem_backup();

//system_backup_restore.h
void screensystem_backup_restore();

//settings_bluebutton.h
void screensettings_bluebutton();

//settings_redbutton.h
void screensettings_redbutton();

//extensions.h
void screenfeed();
void screenextensions(int mode, char* path);
void screenextensions_check(int flag);

//...port.h
void fbsave();
void fbrestore();

//mostzap.h
struct mostzap* createmostzap(int serviceid, unsigned long transponderid);
int writemostzap(const char *filename);
int delmostzap(int serviceid, unsigned long transponderid, int flag);
void mostzaptobouquet(struct mainbouquet* mbouquet);

//thumb.h
char* checkthumb(char* path, char* file);

//dvdplayer.h
int dvdstop();
int dvdsetfb();

//channelhistory.h
void delchannelhistory(struct channel* chnode);
void addchannelhistory(struct channel* chnode, char* channellist);
void screenchannelhistory();

//channellist.h
int screenchannellist(struct channel** retchannel, char** retchannellist, int flag);

//rguid.h
void rguidthreadfunc(struct stimerthread* timernode);

//inadyn.h
void screennetwork_inadyn();

//epgsearch.h
void screenepgsearch();

//sock.h
char* gethttp(char* host, char* page, int port, char* filename, char* auth, struct download* dnode, int redirect);
char *get_ip(char *host);
int sockportopen(int *fd, char* ip, int port, int tout);
void sockclose(int *fd);
int socksend(int *fd, unsigned char* data, int count, int timeout);
int sockread(int fd, unsigned char *buf, int pos, int count, int tout, int flag);
int sockcreate(int *fd, char* sockname, int maxconn);
int sockaccept(int *fd, int flag);

//numinput.h
char* numinput(char* title, char* num, char* mask, int isip);

//ipkg.h
void freeipkg();
int ipkg_update(void);
int ipkg_upgrade(void);
int ipkg_list(void);
int ipkg_install(const char* package);
int ipkg_remove(const char* package, int purge);
struct menulist* ipkmenulist(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* section, int showpng, int flag);
int ipkg_list_installed(void);

//frontenddev.h
struct dvbdev* fegetdummy();

//dvrdev.h
void dvrclose(struct dvbdev* node, int fd);
struct dvbdev* dvropen(struct dvbdev* fenode);

//audiodev.h
int audiopause(struct dvbdev* node);
int audioplay(struct dvbdev* node);
int audioclearbuffer(struct dvbdev* node);

//textinput.h
char* textinput(char* title, char* text);

//httpdsettings.h
void screenhttpdsettings();

//help.h
void screenhelp(struct skin* node);

//restoredefault.h
void screenrestoredefault();

//ca.h
int getfreecasession(struct dvbdev* dvbnode, int type, int value);
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
int videofreeze(struct dvbdev* node);
int videocontinue(struct dvbdev* node);
int videofastforward(struct dvbdev* node, int frames);
int videoclearbuffer(struct dvbdev* node);
int videogetpts(struct dvbdev* node, uint64_t* pts);

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
int screendownload(char* title, char* host, char* page, int port, char* filename, char* auth, int flag);

// epgrecord.h
void freeepgrecord(struct epgrecord** first);
int recordstartreal(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode, int tssize);

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
void screennetwork_restart(struct inetwork* net);
void screennetwork_test();
void screennetwork_wlan();

//channel.h
int writechannel(const char *filename);
struct channel* createchannel(char* name, unsigned long transponderid, int providerid, int serviceid, int servicetype, int flag, int videocodec, int audiocodec, int videopid, int audiopid, int protect);
void delchannelbytransponder(unsigned long transponderid);
struct channel* gettmpchannel();
int delchannel(int serviceid, unsigned long transponderid, int flag);
int movechanneldown(struct channel* node);
int movechannelup(struct channel* node);

//transponder.h
struct transponder* gettransponder(unsigned long transponderid);
int writetransponder(const char *filename);
void deltransponderbyorbitalpos(int orbitalpos);
void deltransponder(struct transponder* tpnode);

//sat.h
int writesat(const char *filename);
void delsat(char *name);
struct sat* getsatbyorbitalpos(int orbitalpos);
int movesatdown(struct sat* node);
int movesatup(struct sat* node);

//provider.h
int writeprovider(const char *filename);
void delprovidernotused(struct provider* node);
int moveproviderdown(struct provider* node);
int moveproviderup(struct provider* node);

//httpd.h
void httpdthreadfunc(struct stimerthread* timernode);
void sendoktext(int* connfd, char* text, int auth);

//skinconfig.h
int getskinconfigint(char *key, char *ext);
char* getskinconfig(char *key, char *ext);

//record.h
struct service* getrecordbyname(char* recname, int type);
int recordskipplay(struct service* servicenode, int sekunden);
void recordffrwts(struct service* servicenode, int speed);

//dvdplayer.h
int dvdstart(char* filename);
int dvdisplaying();
void dvdgotmessage();
int dvdkeypress(int key);
int dvdmenuopen();
void dvdchangevideo();
void dvdafterend();

// play.h
void playerffts(int speed);
int screenplay(char* startfile, int startfolder, int flag);
void playrcstop(int playertype, int flag);
void playwritevfd(char* file);
void playstartservice();
void screenplayinfobar(char* file, int mode, int playertype, int flag);
void playrcgreen(char* file, int playinfobarstatus, int playertype, int flag);
void playrcred(char* file, int playinfobarstatus, int playertype, int flag);
void playrcinfo(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcff(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcfr(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcpause(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcplay(char* file, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcjumpr(char* file, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcjumpf(char* file, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);

//inetwork.h
struct inetwork* getinetworkbydevice(char* device);

//fb.h
void blitfb(int flag);
void changefbresolution(char *value);
void setfbtransparent(int value);
struct fb* addfb(char *fbname, int dev, int width, int height, int colbytes, int fd, unsigned char* mmapfb, unsigned long fixfbsize);
void blitfb2(struct fb* fbnode, int flag);
void delfb(char *name);
void fb2png_thread();
int fb2png(unsigned char *buf_p, int width, int height, char *outfile);
void clearfball();

//timerthread.h
struct stimerthread* addtimer(void* func, int aktion, int delay, int count, void* param1, void* param2, struct stimerthread* last);
void deltimer(struct stimerthread *tnode);

//epgscanlist.h
struct epgscanlist* getepgscanlist(int serviceid, unsigned long transponderid);
int writeepgscanlist(const char *filename);
int delepgscanlist(int serviceid, unsigned long transponderid);

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
int dvbfindpmtpid(int fd, int16_t *pmtpid, int *serviceid, int tssize);
int gettsinfo(int fd, unsigned long long* lenpts, unsigned long long* startpts, unsigned long long* endpts, unsigned long long* bitrate, int tssize);

//pin.h
int screenpincheck(int type, char* pin);
void screenpin();

//videomode.h
void screenvideomode(int flag);

//pic.h
void delpic(char* name);
struct pic* getpic(char* name);
void delmarkedpic(int del);
struct pic* addpic(char *name, unsigned char* picbuf, int memfd, unsigned long width, unsigned long height, unsigned long rowbytes, int channels, int timeout, int del, struct pic* last);

//showiframe.h
int singlepicstart(const char *filename, int flag);

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
int playergetinfots(unsigned long long* lenpts, unsigned long long* startpts, unsigned long long* endpts, unsigned long long* aktpts, unsigned long long* bitrate);
int playerisplaying();
void playerafterend();

//filelist.h
void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview, char* selection);

//plugin.h
struct skin* getplugin(char* pluginname);
int loadplugin();

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
struct clist* addownconfig(char *key, char *value);

//infobar.h
void fillinfobar();

//menulist.h
void freemenulist(struct menulist* mlist, int delparam);
void addmenulistall(struct menulist** mlist, char* allname, char* pic, int deaktiv, char* defaultentry);
struct menulist* addmenulist(struct menulist** mlist, char* name, char* text, char* pic, int deaktiv, int defaultentry);
struct menulist* menulistbox(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* defaultpic, int showpng, int flag);
struct menulist* menulistboxext(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* defaultpic, int showpng, int* rcreturn, int flag);
void setmenulistdefault(struct menulist* mlist, char* defaultentry);
void changemenulistparam(struct menulist* mlist, char* param, char* param1);

//skinfunc.h
char* gettime(struct skin* node, char* format);
char* getepgakttitle(struct skin* node);

//skin.h
void restorescreennofree(char* buf, struct skin* node);
unsigned char *loadjpg(char *filename, unsigned long *width, unsigned long *height, unsigned long *rowbytes, int *channels, int denom);
int changeselectpic(struct skin* node, char* text);
void calcautoscale(int width, int height, int mwidth, int mheight, int* scalewidth, int* scaleheight);
void blitscale(int posx, int posy, int width, int height, int scalewidth, int scaleheight, int flag);
unsigned char* scale(unsigned char* buf, int width, int height, int channels, int newwidth, int newheight, int free1);
void blitrect(int posx, int posy, int width, int height, long color, int transparent, int mode);
void fillrect(int posx, int posy, int width, int height, long color, int transparent);
int setnodeattr(struct skin* node, struct skin* parent);
void clearscreennolock(struct skin* node);
void clearshadow(struct skin* node);
char* savescreen(struct skin* node);
void restorescreen(char* buf, struct skin* node);
char* changepicpath(char* picname);
long convertcol(char *value);
void delmarkedscreen(int del);
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
int changepicmem(struct skin* node, char* text, int timeout, int del);
int convertxmlentry(char *value, uint8_t *proz);
int savejpg(char* filename, int width, int height, unsigned char *buf);

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
int writeconfigtmp();
int readconfig(const char *filename, struct clist** tmpconfig);

//rc.h
int waitrc(struct skin* owner, unsigned int timeout, int flag);
void delownerrc(struct skin* owner);
void delrc(int key, struct skin* owner, struct skin* screennode);
int flushrc(unsigned int timeout);

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
void resettvpic();
int resetvmpeg(struct dvbdev* node);
unsigned long readsysul(const char *filename, int line);
char* oitoax(int value);
void closeonexec(int fd);
char* ostrstrcase(char* str, char* sub);
char* getdevcontent(char* devconfig);
char* getxmlentry(char *line, char *searchstr);
void debugstack(void* address, void* address1);
char* string_strip_whitechars(char *text);
int ounzip(char* inbuf, int inlen, char** outbuf, int* outlen, int maxbuf, int flag);
int ozip(char* inbuf, int inlen, char** outbuf, int* outlen, int level);
int delallfiles(char* dir, char* ext);
unsigned long getfilecount(char* dir);
char* readfiletomem(const char* filename, int flag);
int setbit(int value, int bitpos);
char* changefilenameext(char* filename, char* ext);
void destroy();
void htmldecode(char* to, char* from);
void setosdtransparent(int value);
char* string_shortname(char *tmpfilename, int mode);
off64_t getfilesize(char* name);
struct splitstr* strsplit(char *str, char *tok, int* count);
char* strstrip(char *text);
char* fixip(char* ipinput, int flag);
void m_lock(pthread_mutex_t *mutex, int flag);
void m_unlock(pthread_mutex_t *mutex, int flag);
int setcolorformat(char* value);
int setvideomode(char* value, int flag);
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
char* get_ipk_tmpinstall(char* path, char* ipk);
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
int ostrcasecmp(char* value1, char* value2);
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
int setvol(int value);
int getvol();
char* stringreplacechar(char *str, char c1, char c2);
char* getcpuid();
void get_ipk_update();
void getserial();
char* string_decode(char* input, int flag);
char* string_striptags(char* filename);
char* string_resub(char* str, char* str2, char* input);
int cmpfilenameext(char* filename, char* ext);
void killnet();
void checkserial(char* input);
int checkprozess(char* input);
struct regex* regexstruct(char* regex, char* str);
void freeregexstruct(struct regex* node);

//rcconfig.h
int getrcconfigint(char *key, char* ext);
char* getrcconfig(char *key, char *ext);
int reloadconfig(char *filename);

//service.h
struct service* getservicebyservice(struct service* node, int flag);
struct service* getservicebychannel(struct channel* chnode);
struct service* checkservice(struct service* node);
struct service* addservice(struct service* last);
void delservice(struct service* snode, int flag);
int servicestop(struct service *node, int clear, int flag);
int servicestart(struct channel* chnode, char* channellist, char* pin, int flag);
void serviceresetchannelinfo(struct channel* chnode);
struct service* getservice(int type, int flag);
char* servicecheckret(int ret, int flag);

//mainbouquets.h
int writemainbouquet(const char *filename);
int writeallbouquet();
struct mainbouquet* getmainbouquetbybouquetpointer(struct bouquet* bouquetnode);
int movemainbouquetdown(struct mainbouquet* node);
int movemainbouquetup(struct mainbouquet* node);

//listbox.h
struct skin* addlistbox(struct skin* screen, struct skin* listbox, struct skin* last, int del);

//bouquets.h
void delbouquetbychannel(int serviceid, unsigned long transponderid);
struct bouquet* getbouquetbychannelmain(int serviceid, unsigned long transponderid);
void recalcbouquetnr();
void setbouquetchanneltonullmain(int serviceid, unsigned long transponderid);
void delbouquet(int serviceid, unsigned long transponderid, struct bouquet** firstnode);
int movebouquetdown(struct bouquet* node);
int movebouquetup(struct bouquet* node);

//audiotrack.h
void screenaudiotrack();
struct audiotrack* addaudiotrack(struct channel* chnode, char* langdesc, int pid, int audiocodec, struct audiotrack* last);
void freeaudiotrack(struct channel* chnode);

//subtitle.h
void screensubtitle();
struct subtitle* addsubtitle(struct channel* chnode, int subtype, char* langdesc, int pid, int type, int id1, int id2, struct subtitle* last);
void freesubtitle(struct channel* chnode);
int subtitlestop(int flag);
int subtitlepause(int flag);

//linkedchannel.h
struct linkedchannel* addlinkedchannel(struct channel* chnode, int serviceid, unsigned long transponderid, struct linkedchannel* last);
void freelinkedchannel(struct channel* chnode);
void screenlinkedchannel();
struct linkedchannel* getlinkedchannel(struct channel* chnode, int serviceid, unsigned long transponderid);

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

//skinfunc.h
char* getaktchannelname();
char* getrec();

#endif
