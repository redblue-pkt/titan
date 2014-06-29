#ifndef WM2014_H
#define WM2014_H

struct wmFinalMatch {
	char* day[16];
	char* date[16];
	char* time[16];
	char* team1[16];
	char* team2[16];
	char* result[16];
} wmFinalMatches[1];

struct wmGroupMatch {
	char* day[6];
	char* date[6];
	char* time[6];
	char* team1[6];
	char* team2[6];
	char* result[6];
} wmGroupMatches[8];

struct wmGroupTable {
	char* TableNo[4];
	char* TableNoV[4];
	char* Team[4];
	char* Played[4];
	char* Won[4];	// Sieg
	char* Draw[4];	// Unentschieden
	char* Lost[4];	// Niederlage
	char* Goals[4];
	char* Diff[4];
	char* Points[4];
} wmGroupTables[8];

int wmGroupDataRefresh [10];

void GetWmGroupData(int wmGroupID) {
	char* wmstr = NULL, *wmsearch = NULL, *wmregexA = NULL, *wmregexB = NULL;
	char* wmstrA1 = NULL, *wmstrA2 = NULL, *wmstrA3 = NULL, *wmstrA4 = NULL;
	char* wmstrB1 = NULL, *wmstrB2 = NULL, *wmstrB3 = NULL, *wmstrB4 = NULL;

	wmstr = malloc(MINMALLOC);
	wmsearch = malloc(MINMALLOC);
	wmregexA = malloc(MINMALLOC);
	wmregexB = malloc(MINMALLOC);

	wmstrA1 = malloc(MINMALLOC);
	wmstrA2 = malloc(MINMALLOC);
	wmstrA3 = malloc(MINMALLOC);
	wmstrA4 = malloc(MINMALLOC);

	wmstrB1 = malloc(MINMALLOC);
	wmstrB2 = malloc(MINMALLOC);
	wmstrB3 = malloc(MINMALLOC);
	wmstrB4 = malloc(MINMALLOC);

	debug(10, "GetWmGroupData IN(%d)\n", wmGroupID);

	switch (wmGroupID) {
		case 0:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/gruppea/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe A der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe A der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 1:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/gruppea/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe B der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe B der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 2:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/gruppea/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe C der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe C der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 3:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/grupped/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe D der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe D der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 4:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/grupped/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe E der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe E der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 5:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/grupped/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe F der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe F der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 6:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/gruppeg/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe G der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe G der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		case 7:
			wmsearch = ostrcat("wm/2014/endrunde/vorrunde/gruppeg/", NULL, 0, 0);
			wmregexA = ostrcat("<h2>Die Spiele der Vorrunde der Gruppe H der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			wmregexB = ostrcat("<h2>Die Tabelle der Vorrunde der Gruppe H der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);
			break;
		default:
			break;
	}

	if (wmsearch != NULL) {

		//wmstr = readfiletomem("/tmp/_wmstr.html", 0);

		//if (wmstr == NULL) {
			wmstr = gethttp("fussballdaten.de", wmsearch, 80, NULL, NULL, 5000, NULL, 0);
			//writesys("/tmp/_wmstr.html", wmstr, 0);
			debug(20, "********************************************************************************");
			debug(20, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!   live http request    !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			debug(20, "################################################################################");
		//}

		wmstrA1= oregex(wmregexA, wmstr);
		wmstrB1= oregex(wmregexB, wmstr);

		//writesys("/tmp/_wmstrA1.html", wmstrA1, 0);
		//writesys("/tmp/_wmstrB1.html", wmstrB1, 0);

		free(wmstr); wmstr = NULL;
		free(wmsearch); wmsearch = NULL;
		free(wmregexA); wmregexA = NULL;
		free(wmregexB); wmregexB = NULL;

		int countA = 0;
		struct splitstr* retA = NULL;
		retA = strsplit(wmstrA1, "\n", &countA);
		//free(wmstrA1); wmstrA1 = NULL;

		int max = countA;
		int count = 0;
		int i = 0;
		//for(i = 0; i < max; i++) {
		for(i = 0; i < 150; i++) {
			wmstrA2 = (&retA[i])->part;

			if(ostrstr(wmstrA2, "<td class=\"WT\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].day[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"Tag\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].date[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"Uhrzeit\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].time[count] = wmstrA3;
			}

			if( (ostrstr(wmstrA2, "<td class=\"Heim") != NULL) && (ostrstr(wmstrA2, "\"Flagge") != NULL) ) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].team1[count] = wmstrA3;
			}

			if( (ostrstr(wmstrA2, "<td class=\"Gast") != NULL) && (ostrstr(wmstrA2, "\"Flagge") != NULL) ) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].team2[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"Ergebnis") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				if( (ostrstr(wmstrA3, "&#160;") != NULL) || (ostrstr(wmstrA3, "&#8213;") != NULL) ) {
					wmstrA4 = ostrcat(wmstrA3, NULL, 0, 0);
					wmstrA4 = string_replace("&#160;", " - ", wmstrA4, 1);
					wmstrA4 = string_replace("&#8213;", " - ", wmstrA4, 1);
				} else {
					wmstrA4 = ostrcat(wmstrA3, NULL, 0, 0);
				}
				debug(20, "wmstrA4(%d): %s\n", count, wmstrA4);
				wmGroupMatches[wmGroupID].result[count] = wmstrA4;
				count = count + 1;
			}
		}

// ********************************************************************************************************

		int countB = 0;
		struct splitstr* retB = NULL;
		retB = strsplit(wmstrB1, "\n", &countB);
		//free(wmstrA1); wmstrA1 = NULL;

		max = countB;
		count = 0;
		i = 0;
		//for(i = 0; i < max; i++) {
		for(i = 0; i < 60; i++) {
			wmstrB2 = (&retB[i])->part;


			if(ostrstr(wmstrB2, "<td class=\"Platz\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].TableNo[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"VPlatz\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].TableNoV[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"Verein\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Team[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"Spiele") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Played[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"S") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Won[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"U") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Draw[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"N") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Lost[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"Torverhaeltnis") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Goals[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"Diff") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Diff[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<td class=\"Punkte") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Points[count] = wmstrB3;
				count = count + 1;
			}
		}
	}

	//free(wmstrA2); wmstrA2 = NULL;
	//free(wmstrA3); wmstrA3 = NULL;
	//free(wmstrA4); wmstrA4 = NULL;

	//free(wmstrB2); wmstrB2 = NULL;
	//free(wmstrB3); wmstrB3 = NULL;
	//free(wmstrB4); wmstrB4 = NULL;

	wmGroupDataRefresh[wmGroupID] = 0;

	return;
}

void GetWmFinalData(int wmGroupID) {
	char* wmstr = NULL, *wmsearch = NULL, *wmregexA = NULL;
	char* wmstrA1 = NULL, *wmstrA2 = NULL, *wmstrA3 = NULL, *wmstrA4 = NULL;

	debug(10, "GetWmFinalData IN-0 (%d)\n", wmGroupID);

	if( (wmGroupID != 8) && (wmGroupID != 9) ) return;

	wmstr = malloc(MINMALLOC);
	wmsearch = malloc(MINMALLOC);
	wmregexA = malloc(MINMALLOC);

	wmstrA1 = malloc(MINMALLOC);
	wmstrA2 = malloc(MINMALLOC);
	wmstrA3 = malloc(MINMALLOC);
	wmstrA4 = malloc(MINMALLOC);

	debug(10, "GetWmFinalData IN-1 (%d)\n", wmGroupID);

	wmsearch = ostrcat("wm/2014/endrunde/finale/", NULL, 0, 0);
	wmregexA = ostrcat("<h2>Die Spiele des Achtelfinales der Weltmeisterschaft 2014</h2>(.*?)</table>", NULL, 0, 0);

	if (wmsearch != NULL) {

		wmstr = gethttp("fussballdaten.de", wmsearch, 80, NULL, NULL, 5000, NULL, 0);
		//writesys("/tmp/_wmstr.html", wmstr, 0);

		wmstrA1= oregex(wmregexA, wmstr);

		//writesys("/tmp/_wmstrFinalA1.html", wmstrA1, 0);

		free(wmstr); wmstr = NULL;
		free(wmsearch); wmsearch = NULL;
		free(wmregexA); wmregexA = NULL;

		int countA = 0;
		struct splitstr* retA = NULL;
		retA = strsplit(wmstrA1, "\n", &countA);
		//free(wmstrA1); wmstrA1 = NULL;

		int max = countA;
		int count = 0;
		int i = 0;
		for(i = 0; i < max; i++) {
		//for(i = 0; i < 150; i++) {
			wmstrA2 = (&retA[i])->part;

			if(ostrstr(wmstrA2, "<td class=\"WT\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].day[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"Tag\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].date[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"Uhrzeit\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].time[count] = wmstrA3;
			}

			if( (ostrstr(wmstrA2, "<td class=\"Heim") != NULL) && (ostrstr(wmstrA2, "\"Flagge") != NULL) ) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].team1[count] = wmstrA3;
			}

			if( (ostrstr(wmstrA2, "<td class=\"Gast") != NULL) && (ostrstr(wmstrA2, "\"Flagge") != NULL) ) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].team2[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"Ergebnis") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				if( (ostrstr(wmstrA3, "&#160;") != NULL) || (ostrstr(wmstrA3, "&#8213;") != NULL) ) {
					wmstrA4 = ostrcat(wmstrA3, NULL, 0, 0);
					wmstrA4 = string_replace("&#160;", " - ", wmstrA4, 1);
					wmstrA4 = string_replace("&#8213;", " - ", wmstrA4, 1);
				} else {
					wmstrA4 = ostrcat(wmstrA3, NULL, 0, 0);
				}
				debug(20, "wmstrA4(%d): %s\n", count, wmstrA4);
				wmFinalMatches[0].result[count] = wmstrA4;
				count = count + 1;
			}
		}


	}

	//free(wmstrA2); wmstrA2 = NULL;
	//free(wmstrA3); wmstrA3 = NULL;
	//free(wmstrA4); wmstrA4 = NULL;

	wmGroupDataRefresh[wmGroupID] = 0;

	return;
}

void screenwm2014() {

	struct skin* wm2014 = getscreen("wm2014");
	struct skin* titletext = getscreennode(wm2014, "titletext");

	struct skin* day_0 = getscreennode(wm2014, "day_0");
	struct skin* day_1 = getscreennode(wm2014, "day_1");
	struct skin* day_2 = getscreennode(wm2014, "day_2");
	struct skin* day_3 = getscreennode(wm2014, "day_3");
	struct skin* day_4 = getscreennode(wm2014, "day_4");
	struct skin* day_5 = getscreennode(wm2014, "day_5");

	struct skin* date_0 = getscreennode(wm2014, "date_0");
	struct skin* date_1 = getscreennode(wm2014, "date_1");
	struct skin* date_2 = getscreennode(wm2014, "date_2");
	struct skin* date_3 = getscreennode(wm2014, "date_3");
	struct skin* date_4 = getscreennode(wm2014, "date_4");
	struct skin* date_5 = getscreennode(wm2014, "date_5");

	struct skin* time_0 = getscreennode(wm2014, "time_0");
	struct skin* time_1 = getscreennode(wm2014, "time_1");
	struct skin* time_2 = getscreennode(wm2014, "time_2");
	struct skin* time_3 = getscreennode(wm2014, "time_3");
	struct skin* time_4 = getscreennode(wm2014, "time_4");
	struct skin* time_5 = getscreennode(wm2014, "time_5");

	struct skin* team1_0 = getscreennode(wm2014, "team1_0");
	struct skin* team1_1 = getscreennode(wm2014, "team1_1");
	struct skin* team1_2 = getscreennode(wm2014, "team1_2");
	struct skin* team1_3 = getscreennode(wm2014, "team1_3");
	struct skin* team1_4 = getscreennode(wm2014, "team1_4");
	struct skin* team1_5 = getscreennode(wm2014, "team1_5");

	struct skin* flag1_0 = getscreennode(wm2014, "flag1_0");
	struct skin* flag1_1 = getscreennode(wm2014, "flag1_1");
	struct skin* flag1_2 = getscreennode(wm2014, "flag1_2");
	struct skin* flag1_3 = getscreennode(wm2014, "flag1_3");
	struct skin* flag1_4 = getscreennode(wm2014, "flag1_4");
	struct skin* flag1_5 = getscreennode(wm2014, "flag1_5");

	struct skin* result_0 = getscreennode(wm2014, "result_0");
	struct skin* result_1 = getscreennode(wm2014, "result_1");
	struct skin* result_2 = getscreennode(wm2014, "result_2");
	struct skin* result_3 = getscreennode(wm2014, "result_3");
	struct skin* result_4 = getscreennode(wm2014, "result_4");
	struct skin* result_5 = getscreennode(wm2014, "result_5");

	struct skin* flag2_0 = getscreennode(wm2014, "flag2_0");
	struct skin* flag2_1 = getscreennode(wm2014, "flag2_1");
	struct skin* flag2_2 = getscreennode(wm2014, "flag2_2");
	struct skin* flag2_3 = getscreennode(wm2014, "flag2_3");
	struct skin* flag2_4 = getscreennode(wm2014, "flag2_4");
	struct skin* flag2_5 = getscreennode(wm2014, "flag2_5");

	struct skin* team2_0 = getscreennode(wm2014, "team2_0");
	struct skin* team2_1 = getscreennode(wm2014, "team2_1");
	struct skin* team2_2 = getscreennode(wm2014, "team2_2");
	struct skin* team2_3 = getscreennode(wm2014, "team2_3");
	struct skin* team2_4 = getscreennode(wm2014, "team2_4");
	struct skin* team2_5 = getscreennode(wm2014, "team2_5");

// ################################################################

	struct skin* TableNo_0 = getscreennode(wm2014, "TableNo_0");
	struct skin* TableNo_1 = getscreennode(wm2014, "TableNo_1");
	struct skin* TableNo_2 = getscreennode(wm2014, "TableNo_2");
	struct skin* TableNo_3 = getscreennode(wm2014, "TableNo_3");

	struct skin* TableNoV_0 = getscreennode(wm2014, "TableNoV_0");
	struct skin* TableNoV_1 = getscreennode(wm2014, "TableNoV_1");
	struct skin* TableNoV_2 = getscreennode(wm2014, "TableNoV_2");
	struct skin* TableNoV_3 = getscreennode(wm2014, "TableNoV_3");

	struct skin* Flag_0 = getscreennode(wm2014, "Flag_0");
	struct skin* Flag_1 = getscreennode(wm2014, "Flag_1");
	struct skin* Flag_2 = getscreennode(wm2014, "Flag_2");
	struct skin* Flag_3 = getscreennode(wm2014, "Flag_3");

	struct skin* Team_0 = getscreennode(wm2014, "Team_0");
	struct skin* Team_1 = getscreennode(wm2014, "Team_1");
	struct skin* Team_2 = getscreennode(wm2014, "Team_2");
	struct skin* Team_3 = getscreennode(wm2014, "Team_3");

	struct skin* Played_0 = getscreennode(wm2014, "Played_0");
	struct skin* Played_1 = getscreennode(wm2014, "Played_1");
	struct skin* Played_2 = getscreennode(wm2014, "Played_2");
	struct skin* Played_3 = getscreennode(wm2014, "Played_3");

	struct skin* Won_0 = getscreennode(wm2014, "Won_0");
	struct skin* Won_1 = getscreennode(wm2014, "Won_1");
	struct skin* Won_2 = getscreennode(wm2014, "Won_2");
	struct skin* Won_3 = getscreennode(wm2014, "Won_3");

	struct skin* Draw_0 = getscreennode(wm2014, "Draw_0");
	struct skin* Draw_1 = getscreennode(wm2014, "Draw_1");
	struct skin* Draw_2 = getscreennode(wm2014, "Draw_2");
	struct skin* Draw_3 = getscreennode(wm2014, "Draw_3");

	struct skin* Lost_0 = getscreennode(wm2014, "Lost_0");
	struct skin* Lost_1 = getscreennode(wm2014, "Lost_1");
	struct skin* Lost_2 = getscreennode(wm2014, "Lost_2");
	struct skin* Lost_3 = getscreennode(wm2014, "Lost_3");

	struct skin* Goals_0 = getscreennode(wm2014, "Goals_0");
	struct skin* Goals_1 = getscreennode(wm2014, "Goals_1");
	struct skin* Goals_2 = getscreennode(wm2014, "Goals_2");
	struct skin* Goals_3 = getscreennode(wm2014, "Goals_3");

	struct skin* Diff_0 = getscreennode(wm2014, "Diff_0");
	struct skin* Diff_1 = getscreennode(wm2014, "Diff_1");
	struct skin* Diff_2 = getscreennode(wm2014, "Diff_2");
	struct skin* Diff_3 = getscreennode(wm2014, "Diff_3");

	struct skin* Points_0 = getscreennode(wm2014, "Points_0");
	struct skin* Points_1 = getscreennode(wm2014, "Points_1");
	struct skin* Points_2 = getscreennode(wm2014, "Points_2");
	struct skin* Points_3 = getscreennode(wm2014, "Points_3");

// ################################################################

	struct skin* wm2014_last16 = getscreen("wm2014_last16");
	struct skin* titletext_last16 = getscreennode(wm2014_last16, "titletext");

	struct skin* day_0_last16 = getscreennode(wm2014_last16, "day_0");
	struct skin* day_1_last16 = getscreennode(wm2014_last16, "day_1");
	struct skin* day_2_last16 = getscreennode(wm2014_last16, "day_2");
	struct skin* day_3_last16 = getscreennode(wm2014_last16, "day_3");
	struct skin* day_4_last16 = getscreennode(wm2014_last16, "day_4");
	struct skin* day_5_last16 = getscreennode(wm2014_last16, "day_5");
	struct skin* day_6_last16 = getscreennode(wm2014_last16, "day_6");
	struct skin* day_7_last16 = getscreennode(wm2014_last16, "day_7");

	struct skin* date_0_last16 = getscreennode(wm2014_last16, "date_0");
	struct skin* date_1_last16 = getscreennode(wm2014_last16, "date_1");
	struct skin* date_2_last16 = getscreennode(wm2014_last16, "date_2");
	struct skin* date_3_last16 = getscreennode(wm2014_last16, "date_3");
	struct skin* date_4_last16 = getscreennode(wm2014_last16, "date_4");
	struct skin* date_5_last16 = getscreennode(wm2014_last16, "date_5");
	struct skin* date_6_last16 = getscreennode(wm2014_last16, "date_6");
	struct skin* date_7_last16 = getscreennode(wm2014_last16, "date_7");

	struct skin* time_0_last16 = getscreennode(wm2014_last16, "time_0");
	struct skin* time_1_last16 = getscreennode(wm2014_last16, "time_1");
	struct skin* time_2_last16 = getscreennode(wm2014_last16, "time_2");
	struct skin* time_3_last16 = getscreennode(wm2014_last16, "time_3");
	struct skin* time_4_last16 = getscreennode(wm2014_last16, "time_4");
	struct skin* time_5_last16 = getscreennode(wm2014_last16, "time_5");
	struct skin* time_6_last16 = getscreennode(wm2014_last16, "time_6");
	struct skin* time_7_last16 = getscreennode(wm2014_last16, "time_7");

	struct skin* team1_0_last16 = getscreennode(wm2014_last16, "team1_0");
	struct skin* team1_1_last16 = getscreennode(wm2014_last16, "team1_1");
	struct skin* team1_2_last16 = getscreennode(wm2014_last16, "team1_2");
	struct skin* team1_3_last16 = getscreennode(wm2014_last16, "team1_3");
	struct skin* team1_4_last16 = getscreennode(wm2014_last16, "team1_4");
	struct skin* team1_5_last16 = getscreennode(wm2014_last16, "team1_5");
	struct skin* team1_6_last16 = getscreennode(wm2014_last16, "team1_6");
	struct skin* team1_7_last16 = getscreennode(wm2014_last16, "team1_7");

	struct skin* flag1_0_last16 = getscreennode(wm2014_last16, "flag1_0");
	struct skin* flag1_1_last16 = getscreennode(wm2014_last16, "flag1_1");
	struct skin* flag1_2_last16 = getscreennode(wm2014_last16, "flag1_2");
	struct skin* flag1_3_last16 = getscreennode(wm2014_last16, "flag1_3");
	struct skin* flag1_4_last16 = getscreennode(wm2014_last16, "flag1_4");
	struct skin* flag1_5_last16 = getscreennode(wm2014_last16, "flag1_5");
	struct skin* flag1_6_last16 = getscreennode(wm2014_last16, "flag1_6");
	struct skin* flag1_7_last16 = getscreennode(wm2014_last16, "flag1_7");

	struct skin* result_0_last16 = getscreennode(wm2014_last16, "result_0");
	struct skin* result_1_last16 = getscreennode(wm2014_last16, "result_1");
	struct skin* result_2_last16 = getscreennode(wm2014_last16, "result_2");
	struct skin* result_3_last16 = getscreennode(wm2014_last16, "result_3");
	struct skin* result_4_last16 = getscreennode(wm2014_last16, "result_4");
	struct skin* result_5_last16 = getscreennode(wm2014_last16, "result_5");
	struct skin* result_6_last16 = getscreennode(wm2014_last16, "result_6");
	struct skin* result_7_last16 = getscreennode(wm2014_last16, "result_7");

	struct skin* flag2_0_last16 = getscreennode(wm2014_last16, "flag2_0");
	struct skin* flag2_1_last16 = getscreennode(wm2014_last16, "flag2_1");
	struct skin* flag2_2_last16 = getscreennode(wm2014_last16, "flag2_2");
	struct skin* flag2_3_last16 = getscreennode(wm2014_last16, "flag2_3");
	struct skin* flag2_4_last16 = getscreennode(wm2014_last16, "flag2_4");
	struct skin* flag2_5_last16 = getscreennode(wm2014_last16, "flag2_5");
	struct skin* flag2_6_last16 = getscreennode(wm2014_last16, "flag2_6");
	struct skin* flag2_7_last16 = getscreennode(wm2014_last16, "flag2_7");

	struct skin* team2_0_last16 = getscreennode(wm2014_last16, "team2_0");
	struct skin* team2_1_last16 = getscreennode(wm2014_last16, "team2_1");
	struct skin* team2_2_last16 = getscreennode(wm2014_last16, "team2_2");
	struct skin* team2_3_last16 = getscreennode(wm2014_last16, "team2_3");
	struct skin* team2_4_last16 = getscreennode(wm2014_last16, "team2_4");
	struct skin* team2_5_last16 = getscreennode(wm2014_last16, "team2_5");
	struct skin* team2_6_last16 = getscreennode(wm2014_last16, "team2_6");
	struct skin* team2_7_last16 = getscreennode(wm2014_last16, "team2_7");

// ################################################################

	struct skin* wm2014_final = getscreen("wm2014_final");
	struct skin* titletext_final = getscreennode(wm2014_final, "titletext");

	struct skin* day_0_final = getscreennode(wm2014_final, "day_0");
	struct skin* day_1_final = getscreennode(wm2014_final, "day_1");
	struct skin* day_2_final = getscreennode(wm2014_final, "day_2");
	struct skin* day_3_final = getscreennode(wm2014_final, "day_3");
	struct skin* day_4_final = getscreennode(wm2014_final, "day_4");
	struct skin* day_5_final = getscreennode(wm2014_final, "day_5");
	struct skin* day_6_final = getscreennode(wm2014_final, "day_6");
	struct skin* day_7_final = getscreennode(wm2014_final, "day_7");

	struct skin* date_0_final = getscreennode(wm2014_final, "date_0");
	struct skin* date_1_final = getscreennode(wm2014_final, "date_1");
	struct skin* date_2_final = getscreennode(wm2014_final, "date_2");
	struct skin* date_3_final = getscreennode(wm2014_final, "date_3");
	struct skin* date_4_final = getscreennode(wm2014_final, "date_4");
	struct skin* date_5_final = getscreennode(wm2014_final, "date_5");
	struct skin* date_6_final = getscreennode(wm2014_final, "date_6");
	struct skin* date_7_final = getscreennode(wm2014_final, "date_7");

	struct skin* time_0_final = getscreennode(wm2014_final, "time_0");
	struct skin* time_1_final = getscreennode(wm2014_final, "time_1");
	struct skin* time_2_final = getscreennode(wm2014_final, "time_2");
	struct skin* time_3_final = getscreennode(wm2014_final, "time_3");
	struct skin* time_4_final = getscreennode(wm2014_final, "time_4");
	struct skin* time_5_final = getscreennode(wm2014_final, "time_5");
	struct skin* time_6_final = getscreennode(wm2014_final, "time_6");
	struct skin* time_7_final = getscreennode(wm2014_final, "time_7");

	struct skin* team1_0_final = getscreennode(wm2014_final, "team1_0");
	struct skin* team1_1_final = getscreennode(wm2014_final, "team1_1");
	struct skin* team1_2_final = getscreennode(wm2014_final, "team1_2");
	struct skin* team1_3_final = getscreennode(wm2014_final, "team1_3");
	struct skin* team1_4_final = getscreennode(wm2014_final, "team1_4");
	struct skin* team1_5_final = getscreennode(wm2014_final, "team1_5");
	struct skin* team1_6_final = getscreennode(wm2014_final, "team1_6");
	struct skin* team1_7_final = getscreennode(wm2014_final, "team1_7");

	struct skin* flag1_0_final = getscreennode(wm2014_final, "flag1_0");
	struct skin* flag1_1_final = getscreennode(wm2014_final, "flag1_1");
	struct skin* flag1_2_final = getscreennode(wm2014_final, "flag1_2");
	struct skin* flag1_3_final = getscreennode(wm2014_final, "flag1_3");
	struct skin* flag1_4_final = getscreennode(wm2014_final, "flag1_4");
	struct skin* flag1_5_final = getscreennode(wm2014_final, "flag1_5");
	struct skin* flag1_6_final = getscreennode(wm2014_final, "flag1_6");
	struct skin* flag1_7_final = getscreennode(wm2014_final, "flag1_7");

	struct skin* result_0_final = getscreennode(wm2014_final, "result_0");
	struct skin* result_1_final = getscreennode(wm2014_final, "result_1");
	struct skin* result_2_final = getscreennode(wm2014_final, "result_2");
	struct skin* result_3_final = getscreennode(wm2014_final, "result_3");
	struct skin* result_4_final = getscreennode(wm2014_final, "result_4");
	struct skin* result_5_final = getscreennode(wm2014_final, "result_5");
	struct skin* result_6_final = getscreennode(wm2014_final, "result_6");
	struct skin* result_7_final = getscreennode(wm2014_final, "result_7");

	struct skin* flag2_0_final = getscreennode(wm2014_final, "flag2_0");
	struct skin* flag2_1_final = getscreennode(wm2014_final, "flag2_1");
	struct skin* flag2_2_final = getscreennode(wm2014_final, "flag2_2");
	struct skin* flag2_3_final = getscreennode(wm2014_final, "flag2_3");
	struct skin* flag2_4_final = getscreennode(wm2014_final, "flag2_4");
	struct skin* flag2_5_final = getscreennode(wm2014_final, "flag2_5");
	struct skin* flag2_6_final = getscreennode(wm2014_final, "flag2_6");
	struct skin* flag2_7_final = getscreennode(wm2014_final, "flag2_7");

	struct skin* team2_0_final = getscreennode(wm2014_final, "team2_0");
	struct skin* team2_1_final = getscreennode(wm2014_final, "team2_1");
	struct skin* team2_2_final = getscreennode(wm2014_final, "team2_2");
	struct skin* team2_3_final = getscreennode(wm2014_final, "team2_3");
	struct skin* team2_4_final = getscreennode(wm2014_final, "team2_4");
	struct skin* team2_5_final = getscreennode(wm2014_final, "team2_5");
	struct skin* team2_6_final = getscreennode(wm2014_final, "team2_6");
	struct skin* team2_7_final = getscreennode(wm2014_final, "team2_7");

// ################################################################

	int rcret = 0, wmGroupID = 0;
	char* tmpstr = NULL;
	char* tmpstr2 = NULL;
	int i = 0;

	tmpstr = malloc(MINMALLOC);
	tmpstr2 = malloc(MINMALLOC);

	for (i = 0; i < 9; i++) {
		wmGroupDataRefresh[i] = 1;
		// low performance!
		//GetWmGroupData(i);
	}

	if ( getconfigint("wm2014_groupid", NULL) > 0 && getconfigint("wm2014_groupid", NULL) < 10 ) {
		wmGroupID = getconfigint("wm2014_groupid", NULL);
		debug(10, "main getconfig: wmGroupID = %d\n", wmGroupID);
	} else {
		wmGroupID = 8;
		debug(10, "main default: wmGroupID = %d\n", wmGroupID);
	}

/*
	int j = 0;
	char tmpstr3[2];
	for (j = 0; j < 6; j++) {
		//sprintf(tmpstr3, "%d", j);
		tmpstr3 = oitoa(j);
		tmpstr2 = ostrcat("day_", tmpstr3, 0, 0);
		debug(10, "TEST: (%s)\n", tmpstr2);
	}
	free(tmpstr2); tmpstr2 = NULL;
*/

start:

	if (wmGroupID < 8) {
		debug(20, "main: wmGroupID < 8 (%d)\n", wmGroupID);
		if (wmGroupDataRefresh[wmGroupID] == 1) GetWmGroupData(wmGroupID);
	} else {
		debug(20, "main: wmGroupID < 8 = else (%d)\n", wmGroupID);
		if (wmGroupDataRefresh[wmGroupID] == 1) GetWmFinalData(wmGroupID);
	}

	tmpstr = ostrcat("2014 FIFA World Cup Brazil", " - ", 0, 0);

	switch (wmGroupID) {
		case 0: tmpstr = ostrcat(tmpstr, "Group A", 1, 0); break;
		case 1: tmpstr = ostrcat(tmpstr, "Group B", 1, 0); break;
		case 2: tmpstr = ostrcat(tmpstr, "Group C", 1, 0); break;
		case 3: tmpstr = ostrcat(tmpstr, "Group D", 1, 0); break;
		case 4: tmpstr = ostrcat(tmpstr, "Group E", 1, 0); break;
		case 5: tmpstr = ostrcat(tmpstr, "Group F", 1, 0); break;
		case 6: tmpstr = ostrcat(tmpstr, "Group G", 1, 0); break;
		case 7: tmpstr = ostrcat(tmpstr, "Group H", 1, 0); break;
		case 8: tmpstr = ostrcat(tmpstr, "Last 16", 1, 0); break;
		case 9: tmpstr = ostrcat(tmpstr, "Finals", 1, 0); break;
		default: break;
	}

	if (wmGroupID < 8) {

		changetitle(wm2014, tmpstr);
		changetext(titletext, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(day_0, wmGroupMatches[wmGroupID].day[0]);
		changetext(day_1, wmGroupMatches[wmGroupID].day[1]);
		changetext(day_2, wmGroupMatches[wmGroupID].day[2]);
		changetext(day_3, wmGroupMatches[wmGroupID].day[3]);
		changetext(day_4, wmGroupMatches[wmGroupID].day[4]);
		changetext(day_5, wmGroupMatches[wmGroupID].day[5]);

		changetext(date_0, wmGroupMatches[wmGroupID].date[0]);
		changetext(date_1, wmGroupMatches[wmGroupID].date[1]);
		changetext(date_2, wmGroupMatches[wmGroupID].date[2]);
		changetext(date_3, wmGroupMatches[wmGroupID].date[3]);
		changetext(date_4, wmGroupMatches[wmGroupID].date[4]);
		changetext(date_5, wmGroupMatches[wmGroupID].date[5]);

		changetext(time_0, wmGroupMatches[wmGroupID].time[0]);
		changetext(time_1, wmGroupMatches[wmGroupID].time[1]);
		changetext(time_2, wmGroupMatches[wmGroupID].time[2]);
		changetext(time_3, wmGroupMatches[wmGroupID].time[3]);
		changetext(time_4, wmGroupMatches[wmGroupID].time[4]);
		changetext(time_5, wmGroupMatches[wmGroupID].time[5]);

		changetext(team1_0, wmGroupMatches[wmGroupID].team1[0]);
		changetext(team1_1, wmGroupMatches[wmGroupID].team1[1]);
		changetext(team1_2, wmGroupMatches[wmGroupID].team1[2]);
		changetext(team1_3, wmGroupMatches[wmGroupID].team1[3]);
		changetext(team1_4, wmGroupMatches[wmGroupID].team1[4]);
		changetext(team1_5, wmGroupMatches[wmGroupID].team1[5]);

		changetext(result_0, wmGroupMatches[wmGroupID].result[0]);
		changetext(result_1, wmGroupMatches[wmGroupID].result[1]);
		changetext(result_2, wmGroupMatches[wmGroupID].result[2]);
		changetext(result_3, wmGroupMatches[wmGroupID].result[3]);
		changetext(result_4, wmGroupMatches[wmGroupID].result[4]);
		changetext(result_5, wmGroupMatches[wmGroupID].result[5]);

		changetext(team2_0, wmGroupMatches[wmGroupID].team2[0]);
		changetext(team2_1, wmGroupMatches[wmGroupID].team2[1]);
		changetext(team2_2, wmGroupMatches[wmGroupID].team2[2]);
		changetext(team2_3, wmGroupMatches[wmGroupID].team2[3]);
		changetext(team2_4, wmGroupMatches[wmGroupID].team2[4]);
		changetext(team2_5, wmGroupMatches[wmGroupID].team2[5]);

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team1[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_0, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team1[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_1, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team1[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_2, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team1[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_3, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team1[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_4, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team1[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_5, tmpstr);

		free(tmpstr); tmpstr = NULL;
		//free(tmpstr2); tmpstr2 = NULL;

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team2[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_0, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team2[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_1, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team2[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_2, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team2[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_3, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team2[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_4, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupMatches[wmGroupID].team2[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_5, tmpstr);

		free(tmpstr); tmpstr = NULL;

// ################################################################
// ################################################################

		changetext(TableNo_0, wmGroupTables[wmGroupID].TableNo[0]);
		changetext(TableNo_1, wmGroupTables[wmGroupID].TableNo[1]);
		changetext(TableNo_2, wmGroupTables[wmGroupID].TableNo[2]);
		changetext(TableNo_3, wmGroupTables[wmGroupID].TableNo[3]);

		changetext(TableNoV_0, wmGroupTables[wmGroupID].TableNoV[0]);
		changetext(TableNoV_1, wmGroupTables[wmGroupID].TableNoV[1]);
		changetext(TableNoV_2, wmGroupTables[wmGroupID].TableNoV[2]);
		changetext(TableNoV_3, wmGroupTables[wmGroupID].TableNoV[3]);

		changetext(Team_0, wmGroupTables[wmGroupID].Team[0]);
		changetext(Team_1, wmGroupTables[wmGroupID].Team[1]);
		changetext(Team_2, wmGroupTables[wmGroupID].Team[2]);
		changetext(Team_3, wmGroupTables[wmGroupID].Team[3]);

		changetext(Played_0, wmGroupTables[wmGroupID].Played[0]);
		changetext(Played_1, wmGroupTables[wmGroupID].Played[1]);
		changetext(Played_2, wmGroupTables[wmGroupID].Played[2]);
		changetext(Played_3, wmGroupTables[wmGroupID].Played[3]);

		changetext(Won_0, wmGroupTables[wmGroupID].Won[0]);
		changetext(Won_1, wmGroupTables[wmGroupID].Won[1]);
		changetext(Won_2, wmGroupTables[wmGroupID].Won[2]);
		changetext(Won_3, wmGroupTables[wmGroupID].Won[3]);

		changetext(Draw_0, wmGroupTables[wmGroupID].Draw[0]);
		changetext(Draw_1, wmGroupTables[wmGroupID].Draw[1]);
		changetext(Draw_2, wmGroupTables[wmGroupID].Draw[2]);
		changetext(Draw_3, wmGroupTables[wmGroupID].Draw[3]);

		changetext(Lost_0, wmGroupTables[wmGroupID].Lost[0]);
		changetext(Lost_1, wmGroupTables[wmGroupID].Lost[1]);
		changetext(Lost_2, wmGroupTables[wmGroupID].Lost[2]);
		changetext(Lost_3, wmGroupTables[wmGroupID].Lost[3]);

		changetext(Goals_0, wmGroupTables[wmGroupID].Goals[0]);
		changetext(Goals_1, wmGroupTables[wmGroupID].Goals[1]);
		changetext(Goals_2, wmGroupTables[wmGroupID].Goals[2]);
		changetext(Goals_3, wmGroupTables[wmGroupID].Goals[3]);

		changetext(Diff_0, wmGroupTables[wmGroupID].Diff[0]);
		changetext(Diff_1, wmGroupTables[wmGroupID].Diff[1]);
		changetext(Diff_2, wmGroupTables[wmGroupID].Diff[2]);
		changetext(Diff_3, wmGroupTables[wmGroupID].Diff[3]);

		changetext(Points_0, wmGroupTables[wmGroupID].Points[0]);
		changetext(Points_1, wmGroupTables[wmGroupID].Points[1]);
		changetext(Points_2, wmGroupTables[wmGroupID].Points[2]);
		changetext(Points_3, wmGroupTables[wmGroupID].Points[3]);

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupTables[wmGroupID].Team[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace("ü", "ue", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_0, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupTables[wmGroupID].Team[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace("ü", "ue", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_1, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupTables[wmGroupID].Team[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace("ü", "ue", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_2, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmGroupTables[wmGroupID].Team[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace("ü", "ue", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_3, tmpstr);

		free(tmpstr); tmpstr = NULL;

	}

	if (wmGroupID == 8) {
		changetitle(wm2014_last16, tmpstr);
		changetext(titletext_last16, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(day_0_last16, wmFinalMatches[0].day[0]);
		changetext(day_1_last16, wmFinalMatches[0].day[1]);
		changetext(day_2_last16, wmFinalMatches[0].day[2]);
		changetext(day_3_last16, wmFinalMatches[0].day[3]);
		changetext(day_4_last16, wmFinalMatches[0].day[4]);
		changetext(day_5_last16, wmFinalMatches[0].day[5]);
		changetext(day_6_last16, wmFinalMatches[0].day[6]);
		changetext(day_7_last16, wmFinalMatches[0].day[7]);

		changetext(date_0_last16, wmFinalMatches[0].date[0]);
		changetext(date_1_last16, wmFinalMatches[0].date[1]);
		changetext(date_2_last16, wmFinalMatches[0].date[2]);
		changetext(date_3_last16, wmFinalMatches[0].date[3]);
		changetext(date_4_last16, wmFinalMatches[0].date[4]);
		changetext(date_5_last16, wmFinalMatches[0].date[5]);
		changetext(date_6_last16, wmFinalMatches[0].date[6]);
		changetext(date_7_last16, wmFinalMatches[0].date[7]);

		changetext(time_0_last16, wmFinalMatches[0].time[0]);
		changetext(time_1_last16, wmFinalMatches[0].time[1]);
		changetext(time_2_last16, wmFinalMatches[0].time[2]);
		changetext(time_3_last16, wmFinalMatches[0].time[3]);
		changetext(time_4_last16, wmFinalMatches[0].time[4]);
		changetext(time_5_last16, wmFinalMatches[0].time[5]);
		changetext(time_6_last16, wmFinalMatches[0].time[6]);
		changetext(time_7_last16, wmFinalMatches[0].time[7]);

		changetext(team1_0_last16, wmFinalMatches[0].team1[0]);
		changetext(team1_1_last16, wmFinalMatches[0].team1[1]);
		changetext(team1_2_last16, wmFinalMatches[0].team1[2]);
		changetext(team1_3_last16, wmFinalMatches[0].team1[3]);
		changetext(team1_4_last16, wmFinalMatches[0].team1[4]);
		changetext(team1_5_last16, wmFinalMatches[0].team1[5]);
		changetext(team1_6_last16, wmFinalMatches[0].team1[6]);
		changetext(team1_7_last16, wmFinalMatches[0].team1[7]);

		changetext(result_0_last16, wmFinalMatches[0].result[0]);
		changetext(result_1_last16, wmFinalMatches[0].result[1]);
		changetext(result_2_last16, wmFinalMatches[0].result[2]);
		changetext(result_3_last16, wmFinalMatches[0].result[3]);
		changetext(result_4_last16, wmFinalMatches[0].result[4]);
		changetext(result_5_last16, wmFinalMatches[0].result[5]);
		changetext(result_6_last16, wmFinalMatches[0].result[6]);
		changetext(result_7_last16, wmFinalMatches[0].result[7]);

		changetext(team2_0_last16, wmFinalMatches[0].team2[0]);
		changetext(team2_1_last16, wmFinalMatches[0].team2[1]);
		changetext(team2_2_last16, wmFinalMatches[0].team2[2]);
		changetext(team2_3_last16, wmFinalMatches[0].team2[3]);
		changetext(team2_4_last16, wmFinalMatches[0].team2[4]);
		changetext(team2_5_last16, wmFinalMatches[0].team2[5]);
		changetext(team2_6_last16, wmFinalMatches[0].team2[6]);
		changetext(team2_7_last16, wmFinalMatches[0].team2[7]);

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_0_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_1_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_2_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_3_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_4_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_5_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[6], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_6_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[7], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_7_last16, tmpstr);

		free(tmpstr); tmpstr = NULL;
		//free(tmpstr2); tmpstr2 = NULL;

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_0_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_1_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_2_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_3_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_4_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_5_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[6], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_6_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[7], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_7_last16, tmpstr);

		free(tmpstr); tmpstr = NULL;

	}

	if (wmGroupID == 9) {
		changetitle(wm2014_final, tmpstr);
		changetext(titletext_final, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(day_0_final, wmFinalMatches[0].day[8]);
		changetext(day_1_final, wmFinalMatches[0].day[9]);
		changetext(day_2_final, wmFinalMatches[0].day[10]);
		changetext(day_3_final, wmFinalMatches[0].day[11]);
		changetext(day_4_final, wmFinalMatches[0].day[12]);
		changetext(day_5_final, wmFinalMatches[0].day[13]);
		changetext(day_6_final, wmFinalMatches[0].day[14]);
		changetext(day_7_final, wmFinalMatches[0].day[15]);

		changetext(date_0_final, wmFinalMatches[0].date[8]);
		changetext(date_1_final, wmFinalMatches[0].date[9]);
		changetext(date_2_final, wmFinalMatches[0].date[10]);
		changetext(date_3_final, wmFinalMatches[0].date[11]);
		changetext(date_4_final, wmFinalMatches[0].date[12]);
		changetext(date_5_final, wmFinalMatches[0].date[13]);
		changetext(date_6_final, wmFinalMatches[0].date[14]);
		changetext(date_7_final, wmFinalMatches[0].date[15]);

		changetext(time_0_final, wmFinalMatches[0].time[8]);
		changetext(time_1_final, wmFinalMatches[0].time[9]);
		changetext(time_2_final, wmFinalMatches[0].time[10]);
		changetext(time_3_final, wmFinalMatches[0].time[11]);
		changetext(time_4_final, wmFinalMatches[0].time[12]);
		changetext(time_5_final, wmFinalMatches[0].time[13]);
		changetext(time_6_final, wmFinalMatches[0].time[14]);
		changetext(time_7_final, wmFinalMatches[0].time[15]);

		changetext(team1_0_final, wmFinalMatches[0].team1[8]);
		changetext(team1_1_final, wmFinalMatches[0].team1[9]);
		changetext(team1_2_final, wmFinalMatches[0].team1[10]);
		changetext(team1_3_final, wmFinalMatches[0].team1[11]);
		changetext(team1_4_final, wmFinalMatches[0].team1[12]);
		changetext(team1_5_final, wmFinalMatches[0].team1[13]);
		changetext(team1_6_final, wmFinalMatches[0].team1[14]);
		changetext(team1_7_final, wmFinalMatches[0].team1[15]);

		changetext(result_0_final, wmFinalMatches[0].result[8]);
		changetext(result_1_final, wmFinalMatches[0].result[9]);
		changetext(result_2_final, wmFinalMatches[0].result[10]);
		changetext(result_3_final, wmFinalMatches[0].result[11]);
		changetext(result_4_final, wmFinalMatches[0].result[12]);
		changetext(result_5_final, wmFinalMatches[0].result[13]);
		changetext(result_6_final, wmFinalMatches[0].result[14]);
		changetext(result_7_final, wmFinalMatches[0].result[15]);

		changetext(team2_0_final, wmFinalMatches[0].team2[8]);
		changetext(team2_1_final, wmFinalMatches[0].team2[9]);
		changetext(team2_2_final, wmFinalMatches[0].team2[10]);
		changetext(team2_3_final, wmFinalMatches[0].team2[11]);
		changetext(team2_4_final, wmFinalMatches[0].team2[12]);
		changetext(team2_5_final, wmFinalMatches[0].team2[13]);
		changetext(team2_6_final, wmFinalMatches[0].team2[14]);
		changetext(team2_7_final, wmFinalMatches[0].team2[15]);

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[8], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_0_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[9], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_1_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[10], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_2_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[11], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_3_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[12], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_4_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[13], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_5_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[14], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_6_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team1[15], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_7_final, tmpstr);

		free(tmpstr); tmpstr = NULL;
		//free(tmpstr2); tmpstr2 = NULL;

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[8], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_0_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[9], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_1_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[10], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_2_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[11], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_3_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[12], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_4_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[13], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_5_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[14], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_6_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2014/skin/", wmFinalMatches[0].team2[15], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_7_final, tmpstr);

		free(tmpstr); tmpstr = NULL;

	}

// ################################################################

	if (wmGroupID < 8) {
		drawscreen(wm2014, 0, 0);
	} else if (wmGroupID == 8) {
		drawscreen(wm2014_last16, 0, 0);
	} else if (wmGroupID == 9) {
		drawscreen(wm2014_final, 0, 0);
	}

	while(1) {
		rcret = waitrc(wm2014, 0, 0);

		//if(rcret == getrcconfigint("rcok", NULL)) continue;	
		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcgreen", NULL)) {
			/*for (i = 0; i < 8; i++) {
				//wmGroupDataRefresh[i] = 1;

				// low performance!
				//GetWmGroupData(i);
			}*/
			// or
			wmGroupDataRefresh[wmGroupID] = 1;
			goto start;
		}

		if(rcret == getrcconfigint("rc1", NULL)) {
			wmGroupID = 0;
			goto start;
		}

		if(rcret == getrcconfigint("rc2", NULL)) {
			wmGroupID = 1;
			goto start;
		}

		if(rcret == getrcconfigint("rc3", NULL)) {
			wmGroupID = 2;
			goto start;
		}

		if(rcret == getrcconfigint("rc4", NULL)) {
			wmGroupID = 3;
			goto start;
		}

		if(rcret == getrcconfigint("rc5", NULL)) {
			wmGroupID = 4;
			goto start;
		}

		if(rcret == getrcconfigint("rc6", NULL)) {
			wmGroupID = 5;
			goto start;
		}

		if(rcret == getrcconfigint("rc7", NULL)) {
			wmGroupID = 6;
			goto start;
		}

		if(rcret == getrcconfigint("rc8", NULL)) {
			wmGroupID = 7;
			goto start;
		}

		if(rcret == getrcconfigint("rc9", NULL)) {
			wmGroupID = 8;
			goto start;
		}

		if(rcret == getrcconfigint("rc0", NULL)) {
			wmGroupID = 9;
			goto start;
		}
	}

	debug(10, "main addconfigint: wmGroupID = %d\n", wmGroupID);
	addconfigint("wm2014_groupid", wmGroupID);

	delownerrc(wm2014);
	delownerrc(wm2014_last16);
	delownerrc(wm2014_final);
	delmarkedscreennodes(wm2014, 1);
	delmarkedscreennodes(wm2014_last16, 1);
	delmarkedscreennodes(wm2014_final, 1);
	//free all ?
	clearscreen(wm2014);
	clearscreen(wm2014_last16);
	clearscreen(wm2014_final);
}

#endif
