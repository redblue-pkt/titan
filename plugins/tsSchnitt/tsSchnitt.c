#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "Record cut";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/tsSchnitt/plugin.png";

int pluginaktiv = 0;
int pluginversion = 999999;

int imarker, iindex, ischnitt;
char* recfile = NULL;
char* recname = NULL;
char* cutfile = NULL;

struct stimerthread* tsSchnittThread = NULL;
struct stimerthread* tsSchnittThreadKill = NULL;

void tsSchnitt_thread_kill()
{
	while (tsSchnittThread->aktion != STOP && tsSchnittThreadKill->aktion != STOP)
	{	
		sleep(2);
	}
	if(tsSchnittThread->aktion == STOP)
	{
		system("killall -9 avconv");
	}
	tsSchnittThreadKill = NULL;
}


void tsSchnitt_thread()
{
	char* tmpstr = NULL;
	char* tmpstr2 = NULL;
	char* tmpstr3 = NULL;
	int z = 0;
	int rc;
	unsigned int buf[3];

	tsSchnittThreadKill = addtimer(&tsSchnitt_thread_kill, START, 10000, 1, NULL, NULL, NULL);
	
	if(ischnitt == 0  || ischnitt == 3)
	{
		
		
		off64_t pos;
		off64_t time;
		off64_t mtime;
		tmpstr2 = changefilenameext(recfile, ".ma");
		//tmpstr2 = ostrcat("\"",tmpstr2, 0, 0);
		//tmpstr2 = ostrcat(tmpstr2, "\"", 0, 0);
		printf("----> %s\n",tmpstr2);
		FILE* datei = fopen(tmpstr2, "r");
		z = 0;
		if(datei!= NULL)			
		{
			while(!feof(datei) && z < 2)
			{
				fscanf(datei, "%lld,%lld", &pos, &time);

				if(z == 0)
				{	
					tmpstr = createpluginpath("/tsSchnitt/avconv", 0);
					//tmpstr = ostrcat("/mnt/swapextensions/usr/local/share/titan/plugins/tsSchnitt/avconv", " -ss ", 0, 0);
					tmpstr = ostrcat(tmpstr, " -ss ", 0, 0);
					tmpstr = ostrcat(tmpstr, convert_timesec(time), 0, 0);

					tmpstr = ostrcat(tmpstr, " -i \"",0, 0);
					tmpstr = ostrcat(tmpstr, recfile, 0, 0);
					tmpstr = ostrcat(tmpstr, "\"",0, 0);
					//tmpstr = ostrcat(tmpstr, " -vcodec copy -map 0:v -acodec copy -map 0:a -scodec copy -map 0:s",0, 0);
					tmpstr = ostrcat(tmpstr, " -vcodec copy -map 0:v -acodec copy -map 0:a",0, 0);
					mtime = time;
				}
				if(z == 1)
				{
					tmpstr = ostrcat(tmpstr, " -t ", 0, 0);
					tmpstr = ostrcat(tmpstr, convert_timesec(time - mtime), 0, 0);
					tmpstr = ostrcat(tmpstr, " \"", 0, 0);
					tmpstr = ostrcat(tmpstr, cutfile, 0, 0);
					tmpstr = ostrcat(tmpstr, "\"",0, 0);
				}
				z = z + 1;
			}	
			fclose(datei);	
			free(tmpstr2); tmpstr2 = NULL;													
		}
		if(z < 2)
			ischnitt = 3;
		else
			ischnitt = 1;	

		if(ischnitt == 1)
		{
			printf(" +++ %s +++\n",tmpstr);
			rc = system(tmpstr);
			free(tmpstr);tmpstr= NULL;
			if(rc == 0)
			{
				ischnitt = 2;
				textbox(_("INFO"), _("Schnitt erfolgreich beended"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			}
			else
			{
				remove(cutfile);
				ischnitt = 3;
				textbox(_("ERROR"), _("Schnitt ended mit Fehler !!!!!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			}
		}
		free(tmpstr); tmpstr = NULL;
	}

	if(tsSchnittThreadKill != NULL)
	{	
		tsSchnittThreadKill->aktion = STOP;
		sleep(2);
	}
	tsSchnittThread = NULL;
	printf("ischnitt:--> %i\n",ischnitt);
  return;
}

			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	tmpstr = createpluginpath("/tsSchnitt/skin.xml", 0);
	readscreen(tmpstr, 122, 1);
	free(tmpstr); tmpstr = NULL;
		
	debug(10, "Record cut Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(tsSchnittThread != NULL)
	{
		tsSchnittThread->aktion = STOP;
		sleep(2);
	}
	free(cutfile); cutfile=NULL;
	free(recfile); recfile=NULL;
	free(recname); recname=NULL;
	delmarkedscreen(122);
	pluginaktiv = 0;
	debug(10, "Record cut Plugin loadet !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	char* tmpstr = NULL;
	char* tmpstr2 = NULL;
	int rcret;
	int help;
	struct stat64 rdat;
	struct stat64 cdat;
	
	struct skin* tsschnitt = getscreen("tsSchnitt");
	struct skin* listbox = getscreennode(tsschnitt, "listbox");
	struct skin* film = getscreennode(tsschnitt, "film");
	struct skin* marker = getscreennode(tsschnitt, "marker");
	struct skin* schnitt = getscreennode(tsschnitt, "schnitt");
	struct skin* schnittprog = getscreennode(tsschnitt, "schnittprog");
	struct skin* tmp = NULL;
	
	while(1)
	{

		if(recfile == NULL)
			recfile = ostrcat(recfile, getconfig("tsSchnitt_recfile", NULL), 0, 0);
 		if(recfile == NULL && tsSchnittThread == NULL)
 		{
 			imarker = 0;
 			ischnitt = 0;
 		}
 		else if(tsSchnittThread == NULL)
 		{
		  tmpstr = changefilenameext(recfile, ".ma");
		  if(isfile(tmpstr) == 0)
		  	imarker = 0;
		  else
		  	imarker = 2;
			free(tmpstr); tmpstr=NULL;	
	  	tmpstr = changefilenameext(recfile, ".cut");
	  	cutfile = ostrcat(tmpstr, ".ts", 0, 0); 
	  	free(tmpstr); tmpstr=NULL;
	  	if(isfile(cutfile) == 1 && tsSchnittThread == NULL)
	  		ischnitt = 2;
	  	else if(tsSchnittThread == NULL && ischnitt != 3)
	  		ischnitt = 0;
		}
	
		if(recname == NULL && recfile != NULL)
		{
			tmpstr2 = strrchr(recfile, '/');
			if(tmpstr2 != NULL)
			{
				recname = ostrcat(tmpstr2+1,"" ,0, 0);
				changetext(film, recname);
				tmpstr2 = NULL;
			}
			else
				changetext(film, recfile);
		}
		else if(recfile == NULL)
			changetext(film, recfile);
		
		if(imarker == 0)
			changetext(marker, "no");
		else if(imarker == 2)
			changetext(marker, "yes");
	
		schnittprog->progresssize = 0;
		if(ischnitt == 0)
		{
			changetext(schnitt, "gestopt");
			changetext(schnittprog, "0.0%");
			schnittprog->progresssize = 0;
		}
		else if(ischnitt == 1)
		{
			changetext(schnittprog, "0.0%");
			changetext(schnitt, "in Arbeit");
			if(stat64(cutfile, &cdat) == 0)
			{
 				if(stat64(recfile, &rdat) == 0)
 				{
  					help = (100 * cdat.st_size) / rdat.st_size;
 						schnittprog->progresssize = help;
 						tmpstr2 = ostrcat(oitoa(help),"." ,0, 0);
 						help = ((100 * cdat.st_size) % rdat.st_size) * 10 / rdat.st_size;
 						tmpstr2 = ostrcat(tmpstr2, oitoa(help),0, 0);
 						tmpstr2 = ostrcat(tmpstr2,"%" ,0, 0);
 						changetext(schnittprog, tmpstr2);
 						free(tmpstr2); tmpstr2=NULL;
				}
			}
		}
		else if(ischnitt == 2)
		{
			changetext(schnitt, "fertig");	
			changetext(schnittprog, "0.0%");
			schnittprog->progresssize = 0;
		}	
		else if(ischnitt == 3)
		{
			changetext(schnitt, "fehlerhaft");	
			changetext(schnittprog, "0.0%");
			schnittprog->progresssize = 0;
		}	
		
		drawscreen(tsschnitt, 0, 0);
		addscreenrc(tsschnitt, listbox);
		tmp = listbox->select;
	
		while(1)
		{
			addscreenrc(tsschnitt, tmp);
			rcret = waitrc(tsschnitt, 2000, 0);
			tmp = listbox->select;	
		
			if (rcret == getrcconfigint("rcexit", NULL) && tsSchnittThread == NULL) break;
			if (rcret == getrcconfigint("rcyellow", NULL) && tsSchnittThread != NULL) break;
			if (rcret == getrcconfigint("rcgreen", NULL))
			{
				if(tsSchnittThread == NULL)
				{
					recfile = screendir("/var/media/hdd/movie", "*.ts", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 1200, 0, 600, 0, 0);
					addconfig("tsSchnitt_recfile", recfile);
					free(recfile);recfile=NULL;
					free(recname);recname=NULL;
					break;
				}
				else
				{
					textbox(_("ERROR"), _("cut is running"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				}
			}
			if (rcret == getrcconfigint("rcblue", NULL))
			{
				if(tsSchnittThread == NULL)
				{
					tsSchnittThread = addtimer(&tsSchnitt_thread, START, 10000, 1, NULL, NULL, NULL);
					sleep(1);
					break;
				}
				else
				{
					textbox(_("ERROR"), _("cut is running"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				}
			}
			if (rcret == getrcconfigint("rcred", NULL) && tsSchnittThread != NULL)
			{
				tsSchnittThread->aktion = STOP;
				sleep(2);
				break;
			}
			if(ischnitt == 1)
			{
				changetext(schnitt, "in Arbeit");
				if(stat64(cutfile, &cdat) == 0)
				{
 					if(stat64(recfile, &rdat) == 0)
 					{
  					help = (100 * cdat.st_size) / rdat.st_size;
 						schnittprog->progresssize = help;
 						tmpstr2 = ostrcat(oitoa(help),"." ,0, 0);
 						help = ((100 * cdat.st_size) % rdat.st_size) * 10 / rdat.st_size;
 						tmpstr2 = ostrcat(tmpstr2, oitoa(help),0, 0);
 						tmpstr2 = ostrcat(tmpstr2,"%" ,0, 0);
 						changetext(schnittprog, tmpstr2);
 						free(tmpstr2); tmpstr2=NULL;
					}
				}
			}
			drawscreen(tsschnitt, 0, 0);			
		}
		if (rcret == getrcconfigint("rcexit", NULL)) break;
	}
	delownerrc(tsschnitt);
	clearscreen(tsschnitt); 
				 
			


}