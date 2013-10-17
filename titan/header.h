/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef HEADER_H
#define HEADER_H

//copyfile.h
int screencopy(char* title, char* from, char* to, int flag);

//httpdfunc.h
char* webrectimersend(char* param, int fmt);

//hwtest.h
void screenhwtest();

//colorpicker.h
char* screencolorpicker(char* color, int screencalc, int filelistview, int flag);

//epgscanlist.h
struct epgscanlist* addepgscanlist(char *line, int count, struct epgscanlist* last);

//channelslot.h
int writechannelslot(char *filename);
int delchannelslot(int serviceid, uint64_t transponderid);
struct channelslot* addchannelslot(char *line, int count, struct channelslot* last);
struct channelslot* getchannelslot(int serviceid, uint64_t transponderid);

//fancontrol.h
void screenpanel_settings_fancontrol();

//pip.h
int pipstop(struct service *node, int flag);
int pipstart(struct channel* chnode, char* pin, int flag);

//bgdownload.h
void screenbgdownload();
int startbgdownload(char* host, char* page, int port, char* filename, char* auth, int timeout, int flag);

//log.h
void screenlog();

//timeshiftsettings.h
void screentimeshiftsettings();

//tpchoice.h
struct transponder* tpchoicescreen(int orbitalpos, int flag);

//extepg.h
int readmhw(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag);
int readmhw2(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag);
int readopentv(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag);

//mediadbedit.h
void screenmediadbedit(char* file, int id, int flag);

//shortepg.h
void screenshortepg(struct channel* chnode, struct epg* epgnode, int flag);

//dirsort.h
int screendirsort();

// newsletter.h
void screennewsletter();
void newsletterthreadfunc(struct stimerthread* self);

//oldentry.h
struct oldentry* addoldentry(void* entry, int type, time_t del, struct oldentry* last);
void deloldentry(struct oldentry *entry, int flag);

//id3.h
struct id3tag* getid3(char* file, char* id, int flag);
void freeid3(struct id3tag* node);

//sh4port.h and other ports
int videodiscontinuityskip(struct dvbdev* node, int flag);

//system_infos.h
void screensystem_infos(int mode);

//system_infos_sysinfo.h
void screensystem_infos_sysinfo(int mode);

//textinputhist.h
char* textinputhist(char* title, char* text, char* histname);

//unicable.h
void screenunicable(struct skin* loftype, struct skin* lofl, struct skin* lofh, struct skin* satcr, struct skin* satcrfrequ2);

//rc4.h
void rc4(char *data, size_t dlen, char *key, size_t klen);

//md5.h
char* MDString (char *string);
char* MDFile (char *string);
char* MDPrint (unsigned char digest[16]);

//unlock.h
void screenunlock();

//serial.h
void screenserial();

//ocrypt.h
unsigned char* oencrypt(char* pw, char* buf, int len);
unsigned char* odecrypt(char* pw, char* buf, int len);

//mediadbcache.h
struct mediadb* getmediadb(char* path, char* file, int flag);

//mediadbsettings.h
void screenmediadbsettings();

//mediadb.h
int writemediadb(const char *filename, struct mediadb* cmediadb);
int delmediadbfilter(struct mediadbfilter* mnode, int flag);
struct mediadb* createmediadb(struct mediadb* update, char* id, int type, char* title, char* year, char* released, char* runtime, char* genre, char* director, char* writer, char* actors, char* plot, char* poster, char* rating, char* votes, char* path, char* file, char* shortname, char* fileinfo, int flag, int postercount);
void freemediadbcontent(struct mediadb* node);
int findfiles(char* dirname, int type, int onlydir, int onlycount, int first);
struct mediadbfilter* getlastmediadbfilter(struct mediadbfilter* node, int flag);
void getmediadbcounts(int* video, int* audio, int* picture);
int readmediadb(const char* filename, int type, int flag);
void mediadbscan();
int createmediadbfilter(int type, char* search, int flag);
struct mediadbfilter* getmediadbfilterrandom(int maxentry);
int getmediadbfiltercount();
void freemediadbfilter(int flag);
void freemediadb(int flag);
void freemediadbcategory(int flag);
int delmediadb(struct mediadb* mnode, int flag);
void mediadbfindfilecb(char* path, char* file, int type, char* id, int flag);
char* createshortname(char* file, int *isrec, int *iscam, int flag);

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
void screenextensions(int mode, char* path, char* defentry, int first);
void screenextensions_check(int flag);

//...port.h
void fbsave();
void fbrestore();

//mostzap.h
struct mostzap* createmostzap(int serviceid, uint64_t transponderid);
int writemostzap(const char *filename);
int delmostzap(int serviceid, uint64_t transponderid, int flag);
void mostzaptobouquet(struct mainbouquet* mbouquet);

//thumb.h
char* checkthumb(char* path, char* file);

//dvdplayer.h
int dvdstop();
int dvdsetfb();
void playerresetts();

//channelhistory.h
void delchannelhistory(struct channel* chnode);
void addchannelhistory(struct channel* chnode, char* channellist);
void screenchannelhistory();
void freechannelhistory();

//channellist.h
int screenchannellist(struct channel** retchannel, char** retchannellist, int flag);

//rguid.h
void rguidthreadfunc(struct stimerthread* timernode);

//inadyn.h
void screennetwork_inadyn();

//epgsearch.h
void screenepgsearch(char* text);

//sock.h
char* gethttp(char* host, char* page, int port, char* filename, char* auth, int timeout, struct download* dnode, int redirect);
char *get_ip(char *host);
int sockportopen(int *fd, char* ip, int port, int tout);
void sockclose(int *fd);
int socksend(int *fd, unsigned char* data, int count, int timeout);
int sockread(int fd, unsigned char *buf, int pos, int count, int tout, int flag);
int sockcreate(int *fd, char* sockname, int maxconn);
int sockaccept(int *fd, int flag);
char* gethttpreal(char* host, char* page, int port, char* filename, char* auth, struct download* dnode, int redirect, char* header, long* clen, int timeout, int flag);
int sockreceive(int *fd, unsigned char* data, int count, int timeout);
void gethttpstruct(struct stimerthread* timernode, struct download* node, int flag);
void gethttpstructmsg(struct stimerthread* timernode, struct download* node, int flag);

//numinput.h
char* numinput(char* title, char* num, char* mask, int isip);

//frontenddev.h
struct dvbdev* fegetdummy();
void settunerstatus();

//dvrdev.h
void dvrclose(struct dvbdev* node, int fd);
struct dvbdev* dvropen(struct dvbdev* fenode);

//audiodev.h
int audiopause(struct dvbdev* node);
int audioplay(struct dvbdev* node);
int audioclearbuffer(struct dvbdev* node);
int audiostop(struct dvbdev* node);

//textinput.h
char* textinput(char* title, char* text);

//httpdsettings.h
void screenhttpdsettings();

//help.h
void screenhelp(struct skin* node);

//restoredefault.h
void screenrestoredefault();
void screenrestoredefaultchoice();

//ca.h
int getfreecasession(struct dvbdev* dvbnode, int type, int value);
void caappmenu(struct dvbdev* dvbnode);

//cam.h
int getcaservicebyslot(struct caslot* caslot, int flag);
void caservicedel(struct service* snode, struct caslot* caslot);
void sendcapmt(struct service* node, int clear, int flag);

//gmultiepg.h
int screengmultiepg(struct channel* chnode, struct epg* epgnode, int flag);
int selectchannelgmepg(struct skin* listbox);

//multiepg.h
int screenmultiepg(struct channel* chnode, struct epg* epgnode, int flag);

//harddisk.h
int hddfsck(char* dev);
void hddformat(char* dev, char* filesystem);
int addhddall();

//autores.h
void autoresthreadfunc(struct stimerthread* self, char* text, int timeout);
void screenautores(char* text, int timeout, int flag);

// timeshift.h
void timeshiftpause();
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
int videoslowmotion(struct dvbdev* node, int frames);
int videostop(struct dvbdev* node, int clearscreen);
int videoplay(struct dvbdev* node);

// scan.h
unsigned int satblindscan(struct stimerthread* timernode, int onlycalc);
unsigned int cableblindscan(struct stimerthread* timernode, int onlycalc);
unsigned int terrblindscan(struct stimerthread* timernode, int onlycalc);
int findchannel(struct dvbdev* fenode, struct transponder* tpnode, unsigned char *buf, uint8_t* lastsecnr, struct skin* scan, struct skin* listbox, int ichangename, int flag);
void screenscanconfig(int flag);

// screensaver.h
int showscreensaver();
int initscreensaver();
int deinitscreensaver();

// screensaveradjust.h
void screenscreensaveradjust();

//dir.h
char* screendir(char* path, char* mask, char* selection, int *dirrcret, char* ext, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int prozwidth, int height, int prozheight, int flag);
char* screendirreal(char* path, char* mask, char* selection, int *dirrcret, char* ext, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int prozwidth, int height, int prozheight, int holdselection, int flag);
void readlabelext(struct skin* label, char* filename, char* ext);

// mainplaylist.h
int writeallplaylist();
int writemainplaylist(const char *filename);
struct mainplaylist* getmainplaylistbyplaylistpointer(struct playlist* playlistnode);
struct mainplaylist* screenmainplaylist(int flag);

//playlist.h
int getplaylistmax(struct playlist* plist);
struct playlist* getplaylistrandom(struct playlist* plist, int max);
void getplaylistmaxold(struct skin* playlist, int* maxdirs, int* maxfiles);
struct skin* getplaylistrandomold(struct skin* playlist, int maxdirs, int maxfiles);

// screensaveradjust.h
void screensaveradjust();

//download.h
int screendownload(char* title, char* host, char* page, int port, char* filename, char* auth, int timeout, int flag);

// epgrecord.h
void freeepgrecord(struct epgrecord** first);
int recordstartreal(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode, int tssize);

//titan.c
void oshutdown(int exitcode, int flag);
int createstartscreen();
struct channel *channel;

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
void screennetwork_restart(struct inetwork* net, int flag);
void screennetwork_test();
void screennetwork_wlan();

//channel.h
int writechannel(const char *filename);
struct channel* createchannel(char* name, uint64_t transponderid, int providerid, int serviceid, int servicetype, int flag, int videocodec, int audiocodec, int videopid, int audiopid, int protect);
void delchannelbytransponder(uint64_t transponderid);
struct channel* gettmpchannel();
int delchannel(int serviceid, uint64_t transponderid, int flag);
int movechanneldown(struct channel* node);
int movechannelup(struct channel* node);
struct channel* sortchannel();

//transponder.h
struct transponder* gettransponder(uint64_t transponderid);
int writetransponder(const char *filename);
void deltransponderbyorbitalpos(int orbitalpos);
void deltransponder(struct transponder* tpnode);

//sat.h
int writesat(const char *filename);
void delsat(char *name);
struct sat* getsatbyorbitalpos(int orbitalpos);
int movesatdown(struct sat* node);
int movesatup(struct sat* node);
int sat2bouquet(int orbitalpos, int flag);

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
int screenplay(char* startfile, char* showname, int startfolder, int flag);
void playrcstop(int playertype, int flag);
void playwritevfd(char* file, char* showname);
void playstartservice();
void screenplayinfobar(char* file, char* showname, int mode, int playertype, int flag);
void playrcgreen(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
int playrcred(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcinfo(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcff(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcfr(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcpause(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcplay(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcjumpr(char* file, char* showname, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void playrcjumpf(char* file, char* showname, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);
void get_mediadb_scan_info();
void playrcblue(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcyellow(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrctext(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
void playrcok(char* file, char* showname, int playinfobarstatus, int playertype, int flag);
int playcheckdirrcret(char* file, int dirrcret);
void playrcjumpto(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag);

//inetwork.h
struct inetwork* getinetworkbydevice(char* device);
struct inetwork* getinetworkfirstwlan();

//fb.h
void blitfb(int flag);
void blitfb1();
void changefbresolution(char *value, int flag);
void setfbtransparent(int value);
struct fb* addfb(char *fbname, int dev, int width, int height, int colbytes, int fd, unsigned char* mmapfb, unsigned long fixfbsize);
void blitfb2(struct fb* fbnode, int flag);
void delfb(char *name);
void fb2png_thread();
int fb2png(unsigned char *buf_p, int width, int height, char *outfile);
void clearfball();

//timerthread.h
struct stimerthread* addtimer(void* func, int aktion, int delay, int count, void* param1, void* param2, struct stimerthread* last);
void deltimer(struct stimerthread *tnode, int flag);
struct stimerthread* gettimerbythread(pthread_t thread);

//epgscanlist.h
struct epgscanlist* getepgscanlist(int serviceid, uint64_t transponderid);
int writeepgscanlist(const char *filename);
int delepgscanlist(int serviceid, uint64_t transponderid);

//rectimer.h
int readrectimer(char *filename);
int writerectimer(const char *filename, int flag);
int recordcheckcrypt(struct dvbdev* fenode, int servicetype);
int recordstart(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode);
char* recordcheckret(struct stimerthread* timernode, int ret, int flag);
void checkrecepgret(int ret);
int addrecepg(struct channel* chnode, struct epg* epgnode, char* channellist);
struct rectimer* getrectimerbyservice(struct service* servicenode);
void delrectimer(struct rectimer* rectimernode, int write, int flag);
int rectimergetaktday();
int addrectimer(char *buf);
struct rectimer* addrectimernode(char* line, struct rectimer* last);
int checkrectimerconflict(struct rectimer* recnode);

//dvb.h
unsigned char* dvbgetsdt(struct dvbdev* fenode, int secnr, int timeout);
int dvbreadfd(int fd, unsigned char *buf, int pos, int count, int tout, int flag);
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
void playerslowts(int speed);
void playerff(int speed);
void playerfr(int speed);
void playercontinue();
void playerpause();
int playerstop();
void playerseek(float sec);
int playerstart(char* file);
unsigned long long playergetpts();
double playergetlength();
int playergetinfots(unsigned long long* lenpts, unsigned long long* startpts, unsigned long long* endpts, unsigned long long* aktpts, unsigned long long* bitrate, int flag);
int playerisplaying();
void playerafterend();
int playergetbuffersize();
int playergetbufferstatus();

//filelist.h
void getfilelist(struct skin* input, struct skin* filelistpath, struct skin* filelist, char* path, char* filemask, int tmpview, char* selection);
int createfilelist(struct skin* screen, struct skin* node, int view);
void getfilelistmax(struct skin* filelist, int* maxdirs, int* maxfiles);
struct skin* getfilelistrandom(struct skin* filelist, int maxdirs, int maxfiles);
int filelistflt(char* filter, char* name);

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
void changemenulistparam(struct menulist* mlist, char* param, char* param1, char* param2, char* param3);

//skinfunc.h
char* gettime(struct skin* node, char* format);
char* getepgakttitle(struct skin* node);

//skin.h
void* convertfunc(char *value, uint8_t *rettype);
void restorescreennofree(char* buf, struct skin* node);
unsigned char *loadjpg(char *filename, unsigned long *width, unsigned long *height, unsigned long *rowbytes, int *channels, int denom);
int changeselectpic(struct skin* node, char* text);
void calcautoscale(int width, int height, int mwidth, int mheight, int* scalewidth, int* scaleheight);
void blitscale(int posx, int posy, int width, int height, int scalewidth, int scaleheight, int flag);
unsigned char* scale(unsigned char* buf, int width, int height, int channels, int newwidth, int newheight, int free1);
void blitrect(int posx, int posy, int width, int height, long color, int transparent, int mode);
void fillrect(int posx, int posy, int width, int height, long color, int transparent);
int setnodeattr(struct skin* node, struct skin* parent, int screencalc);
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
struct skin* checkscreennode(struct skin *node, char* nodename);
struct skin* getscreen(char* screenname);
int calcrheight(struct skin* node, struct skin* parent);
int drawscreen(struct skin* node, int screencalc, int flag);
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
unsigned char* savejpg(char* filename, int width, int height, int channels, int newwidth, int newheight, int quality, unsigned char *buf);

//volume.h
void screenvolumeup();
void screenvolumedown();

//mute.h
void screenmute(struct skin* screen, struct skin* node, int flag);

//menu.h
int menucall(struct skin* menunode, struct skin* menuentry, int check);

//textbox.h
int textbox(char* title, char* text, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int height, int timeout, int flag);

//config.h
struct clist* addconfig(char *key, char *value);
void delconfigtmpall();
struct clist* addconfigscreencheck(char *key, struct skin *node, char* check);
struct clist* addconfigscreen(char *key, struct skin *node);
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
struct clist* addconfigint(char *key, int value);

//rc.h
int waitrcext(struct skin* owner, unsigned int timeout, int screencalc, int filelistview);
int waitrc(struct skin* owner, unsigned int timeout, int flag);
void delownerrc(struct skin* owner);
struct rc* addrc(int key, void *func, struct skin* screen, struct skin *screennode);
void delrc(int key, struct skin* owner, struct skin* screennode);
int flushrc(unsigned int timeout);
int writerc(int keycode);

//rcfunc.h
void inputboxchar(struct skin* screen, struct skin* inputbox, char zeichen, int screencalc, int filelistview, int flag);
void inputboxff(struct skin* screen, struct skin* inputbox, int screencalc, int filelistview, int flag);
void inputboxfr(struct skin* screen, struct skin* inputbox, int screencalc, int filelistview, int flag);
void inputboxright(struct skin* screen, struct skin* inputbox, int screencalc, int filelistview, int flag);
void inputboxleft(struct skin* screen, struct skin* inputbox, int screencalc, int filelistview, int flag);
int addscreenrc(struct skin* screen, struct skin* node);
void checkinputboxnumright(struct skin* inputbox);
int delscreenrc(struct skin* screen, struct skin* node);

//spinner.h
void screenspinner();

//global.h
char* createpluginpath(char* text, int flag);
void delspezchar(char* text, int flag);
int osystem(char* cmd, int timeout);
char* mask(char* input, int count, char* maskchar);
char* getboxtype();
char* getfilenameext(char* filename);
void startnet();
int checkdirext(char* dir, char* ext);
char* string_replace_all(char *search, char *replace, char *string, int free1);
struct tm* olocaltime(time_t *value);
void setskinnodeslocked(int flag);
char* stringreplacecharonce(char *str, char c1, char c2);
int getrandom(int max);
int clearbit(int value, int bitpos);
int checkbit(int value, int bitpos);
char* addmountpart(char* filename, int free1);
char* string_removechar(char *str);
unsigned int gethash(char* str);
char* oregex(char* regex, char* str);
int resettvpic();
unsigned long readsysul(const char *filename, int line);
char* oitoax(int value);
void closeonexec(int fd);
char* ostrstrcase(char* str, char* sub);
char* getdevcontent(char* devconfig);
char* getxmlentry(char *line, char *searchstr);
void debugstack(int sig, void* address, void* address1);
char* string_strip_whitechars(char *text);
int ounzip(char* inbuf, int inlen, char** outbuf, int* outlen, int maxbuf, int flag);
int ozip(char* inbuf, int inlen, char** outbuf, int* outlen, int level);
int delallfiles(char* dir, char* ext);
unsigned long getfilecount(char* dir);
char* readfiletomem(const char* filename, int flag);
char* readbintomem(const char* filename, size_t size);
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
int setcolorformat(char* value, int flag);
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
char* ostrstr(char* str, char* search);
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
char* ollutoa(uint64_t value);
int ostrcmp(char* value1, char* value2);
int ostrcasecmp(char* value1, char* value2);
char* ostrshrink(char* value);
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
char* string_decode(char* input, int flag);
char* string_striptags(char* filename);
char* string_resub(char* str, char* str2, char* input, int dir);
int cmpfilenameext(char* filename, char* ext);
void killnet();
void checkserial(char* input);
int checkprozess(char* input);
struct regex* regexstruct(char* regex, char* str);
void freeregexstruct(struct regex* node);
char fromhex(char c);
char* unhexlify(char *hexstr);
int isdir(char* name);
void htmldecode3(char* to, char* from);
char* string_deltags(char* str);
char* readfromlinetoline(char* str, int start, int end, int flag);
int checkdev(char* dev);
char* getvideomode();
int checkinternet();
struct splitstr* oregexsplit(char* regex, char *str, char *tok, int* count);
void freeoregexsplit(struct splitstr* tmparray, int len);
char* fixport(char* input, int flag);
char* getispip();
int getlfiletype(char* filename);
unsigned long long getfreespace(char* dir);
char* getfilenamepng(char* filename);
char* htmlencode(char* from);
void waitmsgbar(int sec, int exit, int flag);

//rcconfig.h
int getrcconfigint(char *key, char* ext);
char* getrcconfig(char *key, char *ext);
int reloadconfig(char *filename);

//service.h
struct service* getservicebyrecname(char* recname, int type, int flag);
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
struct mainbouquet* addmainbouquet(char *line, int count, struct mainbouquet* last);
struct mainbouquet* screenmainbouquet();
int mainbouquet2epgscanlist(struct mainbouquet* mnode);
int mainbouquet2channelslot(struct mainbouquet* mnode, int slot);
void delmainbouquet(char *name, int flag);

//listbox.h
struct skin* addlistbox(struct skin* screen, struct skin* listbox, struct skin* last, int del);

//bouquets.h
void delbouquetbychannel(int serviceid, uint64_t transponderid);
struct bouquet* getbouquetbychannelmain(int serviceid, uint64_t transponderid);
void recalcbouquetnr();
void setbouquetchanneltonullmain(int serviceid, uint64_t transponderid);
void delbouquet(int serviceid, uint64_t transponderid, struct bouquet** firstnode);
int movebouquetdown(struct bouquet* node);
int movebouquetup(struct bouquet* node);
struct bouquet* addbouquet(struct bouquet **firstnode, char *line, int type, int count, struct bouquet* last);
struct bouquet* sortbouquet(struct bouquet **nodeaddr);

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
void changelastsubtitle(struct lastsubtitle* lsnode, int pid, int id2);
void dellastsubtitle(struct lastsubtitle* lsnode);
struct lastsubtitle* addlastsubtitle(char* line, int count, struct lastsubtitle* last);
struct lastsubtitle* getlastsubtitle(uint64_t transponderid, int serviceid);
int subtitlestartlast();
int writelastsubtitle(const char *filename);

//linkedchannel.h
struct linkedchannel* addlinkedchannel(struct channel* chnode, int serviceid, uint64_t transponderid, time_t starttime, time_t endtime, struct linkedchannel* last);
void freelinkedchannel(struct channel* chnode);
void screenlinkedchannel();
struct linkedchannel* getlinkedchannel(struct channel* chnode, int serviceid, uint64_t transponderid, time_t starttime, time_t endtime);
void dellinkedchannel(struct channel* chnode, struct linkedchannel* lnode, int flag);
void deloldlinkedchannel();

//zap.h
void zapup();
void zapdown();

//epg.h
int screensingleepg(struct channel* chnode, struct epg* epgnode, int flag);
int screenepg(struct channel* chnode, struct epg* epgnode, int flag);

//eit.h
void epgthreadfunc(struct stimerthread* self);
struct epg* getepgnext(struct channel* chnode);
int writeepg(const char* filename);
void freeepg(struct channel* chnode);
struct epg* getepgakt(struct channel* chnode);
struct epg* getepgbytime(struct channel* chnode, time_t akttime);
char* epgdescunzip(struct epg* epgnode);
struct epg* addoldentryepg(struct channel* chnode, struct epg* newnode, int flag);
void deloldentryepg(struct epg* node);
void clearepgentry(struct epg* node);

//standby.h
void screenstandby();

//channelcache.h
void delchannelcache(int serviceid, uint64_t transponderid);
struct channelcache* modifychannelcache(int serviceid, uint64_t transponderid, struct channel*);
struct channel* getchannel(int serviceid, uint64_t transponderid);

// vfd.h
int setvfdicon(vfdicons id, int onoff);
int writevfd(char *value);
int writevfdmenu(char *value);

//skinfunc.h
char* getaktchannelname();
char* getrec();

//marker.h
struct marker* addmarkernode(off64_t pos);
int delmarkernode(off64_t pos);
int delmarker(char* timetext);
int getmarker(char* dateiname);
int putmarker(char* dateiname);
int setmarker();
int jumpmarker(char* timetext);
void screenmarker();


#endif
