#ifndef wm2018_H
#define wm2018_H

struct wmFinalMatch {
	//char* day[16];
	char* date[16];
	char* htime[16];
	char* team1[16];
	//char* flag1[16];
	//char* flag2[16];
	char* team2[16];
	char* result[16];
} wmFinalMatches[1];

struct wmFinaleMatch {
	//char* day[16];
	char* date[4];
	char* htime[4];
	char* team1[4];
	//char* flag1[16];
	//char* flag2[16];
	char* team2[4];
	char* result[4];
} wmFinaleMatches[1];

struct wmGroupMatch {
	//char* day[6];
	char* date[6];
	//char* time[6];
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
	char* wmstr = NULL, *wmsearch = NULL, *wmsearch2 = NULL, *wmregexA = NULL, *wmregexB = NULL;
	char* wmstrA1 = NULL, *wmstrA2 = NULL, *wmstrA3 = NULL, *wmstrA4 = NULL;
	char* wmstrB1 = NULL, *wmstrB2 = NULL, *wmstrB3 = NULL, *wmstrB4 = NULL;

	wmstr = malloc(MINMALLOC);
	//wmstr = malloc(sizeof(wmstr));
	wmsearch = malloc(MINMALLOC);
	wmsearch2 = malloc(MINMALLOC);
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
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmga-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmga-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 1:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmgb-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmgb-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 2:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmgc-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmgc-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 3:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmgd-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmgd-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 4:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmge-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmge-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 5:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmgf-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmgf-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 6:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmgg-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmgg-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		case 7:
			wmsearch = ostrcat("fussball/wm-2018/ergebnisse/spieltag-wmgh-saison-2017-2018", NULL, 0, 0);
			wmregexA = ostrcat("<ul class=\"table_content\">(.*?)Runde</div>", NULL, 0, 0);
			wmsearch2 = ostrcat("fussball/wm-2018/tabelle/tabelle-gruppe-wmgh-saison-2017-2018", NULL, 0, 0);
			wmregexB = ostrcat("<div class=\"table_content\" id=\"tablefussball\">(.*?)Runde</div>", NULL, 0, 0);
			break;
		default:
			break;
	}

	if (wmsearch != NULL) {

		//wmstr = readfiletomem("/tmp/_wmstr.html", 0);

		//if (wmstr == NULL) {

			wmstr = gethttp("sportal.de", wmsearch, 80, NULL, NULL, 5000, NULL, 0);
			//writesys("/tmp/_wmstr.html", wmstr, 0);
			debug(20, "********************************************************************************");
			debug(20, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!   live http request    !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			debug(20, "################################################################################");
		//}

		wmstrA1= oregex(wmregexA, wmstr);

		//writesys("/tmp/_wmstrA1.html", wmstrA1, 0);
		//writesys("/tmp/_wmstrB1.html", wmstrB1, 0);

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
		//for(i = 0; i < 100; i++) {
			wmstrA2 = (&retA[i])->part;
			if(ostrstr(wmstrA2, "<span class=\"date\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = ostrcat(wmstrA2, NULL, 0, 0);
				wmstrA3 = string_replace_all("<li class=\"first\"><span class=\"date\">", "", wmstrA3, 1);
				wmstrA3 = string_replace_all("</span><span class=\"time\">", "18 ", wmstrA3, 1);
				wmstrA3 = string_replace_all("</span></li>", " Uhr", wmstrA3, 1);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].date[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<li class=\"heim\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].team1[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<li class=\"auswaerts\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmGroupMatches[wmGroupID].team2[count] = wmstrA3;
				count = count + 1;
			}

			if(ostrstr(wmstrA2, "<li class=\"score\">") != NULL) {
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
				
			}
		
		}
// ********************************************************************************************************
		if (wmsearch2 != NULL) {

		//wmstr = readfiletomem("/tmp/_wmstr.html", 0);

		//if (wmstr == NULL) {
			
			wmstr = gethttp("sportal.de", wmsearch2, 80, NULL, NULL, 5000, NULL, 0);
			//writesys("/tmp/_wmstr.html", wmstr, 0);
			debug(20, "********************************************************************************");
			debug(20, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!   live http request    !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			debug(20, "################################################################################");
		//}


		wmstrB1= oregex(wmregexB, wmstr);

		//writesys("/tmp/_wmstrA1.html", wmstrA1, 0);
		//writesys("/tmp/_wmstrB1.html", wmstrB1, 0);

		free(wmstr); wmstr = NULL;
		free(wmsearch2); wmsearch2 = NULL;
		free(wmregexB); wmregexB = NULL;

		int countB = 0;
		struct splitstr* retB = NULL;
		retB = strsplit(wmstrB1, "\n", &countB);
		//free(wmstrA1); wmstrA1 = NULL;

		max = countB;
		count = 0;
		i = 0;
		for(i = 0; i < max; i++) {
		//for(i = 0; i < 60; i++) {
			wmstrB2 = (&retB[i])->part;


			if(ostrstr(wmstrB2, "<li class=\"first platz\"") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].TableNo[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"veraenderung\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = ostrcat(wmstrB2, NULL, 0, 0);
				wmstrB3 = string_replace("&nbsp;", " ", wmstrB3, 1);
				wmstrB3 = string_replace("style=\"display:none;\" ", "", wmstrB3, 1);
				wmstrB4 = string_striptags(wmstrB3);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB4);
				wmGroupTables[wmGroupID].TableNoV[count] = wmstrB4;
			}

			if(ostrstr(wmstrB2, "<li class=\"verein\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				//wmstrB3 = string_striptags(wmstrB2);
				wmstrB3 = ostrcat(wmstrB2, NULL, 0, 0);
				wmstrB3 = string_replace("&nbsp;", "", wmstrB3, 1);
				wmstrB3 = string_replace("style=\"display:none;\" ", "", wmstrB3, 1);
				wmstrB4 = string_striptags(wmstrB3);
				debug(20, "wmstrB5(%d): %s\n", count, wmstrB4);
				wmGroupTables[wmGroupID].Team[count] = wmstrB4;
			}

			if(ostrstr(wmstrB2, "<li class=\"sp\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Played[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"s\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Won[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"u\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Draw[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"n\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Lost[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"tore\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Goals[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"td\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Diff[count] = wmstrB3;
			}

			if(ostrstr(wmstrB2, "<li class=\"pkt\">") != NULL) {
				debug(30, "wmstrB2(%d): %s\n", i, wmstrB2);
				wmstrB3 = string_striptags(wmstrB2);
				debug(20, "wmstrB3(%d): %s\n", count, wmstrB3);
				wmGroupTables[wmGroupID].Points[count] = wmstrB3;
				count = count + 1;
			}
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
	char* wmstrA1 = NULL, *wmstrA2 = NULL, *wmstrA3 = NULL, *wmstrA4 = NULL, *wmstrA5 = NULL;

	debug(10, "GetWmFinalData IN-0 (%d)\n", wmGroupID);

	//if (wmGroupID != 8) return;

	wmstr = malloc(MINMALLOC);
	wmsearch = malloc(MINMALLOC);
	wmregexA = malloc(MINMALLOC);
	wmstrA1 = malloc(MINMALLOC);
	wmstrA2 = malloc(MINMALLOC);
	wmstrA3 = malloc(MINMALLOC);
	wmstrA4 = malloc(MINMALLOC);
	wmstrA5 = malloc(MINMALLOC);

	debug(10, "GetWmFinalData IN-1 (%d)\n", wmGroupID);

	wmsearch = ostrcat("fussball/wm-2018/spielplan/spielplan-chronologisch-saison-2017-2018", NULL, 0, 0);
	wmregexA = ostrcat("<li class=\"first\">WM 2018 Achtelfinale</li>(.*?)<div class=\"spacerFootExtra\">", NULL, 0, 0);
	printf("[wm2018 Achtelfinale ] wmregexA: %s\n", wmregexA);

	if (wmsearch != NULL) {

		wmstr = gethttp("sportal.de", wmsearch, 80, NULL, NULL, 5000, NULL, 0);
		//writesys("/tmp/_wmstr.html", wmstr, 0);

		wmstrA1= oregex(wmregexA, wmstr);
	printf("[wm2018 Achtelfinale ] wmstrA1: %s\n", wmstrA1);
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
			//printf("[wm2018 A2 ] wmstrA2: %s\n", wmstrA2);
	
			if(ostrstr(wmstrA2, "<span class=\"date\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = ostrcat(wmstrA2, NULL, 0, 0);
				wmstrA3 = string_replace_all("<li class=\"first\"><span class=\"date\">", "", wmstrA3, 1);
				wmstrA3 = string_replace_all("</span><span class=\"time\">", "18 ", wmstrA3, 1);
				wmstrA3 = string_replace_all("</span></li>", " Uhr", wmstrA3, 1);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].date[count] = wmstrA3;
			}

			/*if(ostrstr(wmstrA2, "<td class=\"match-time\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = ostrcat(wmstrA2, NULL, 0, 0);
				wmstrA3 = string_replace("-:-", "", wmstrA3, 1);
				wmstrA4 = string_striptags(wmstrA3);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA4);
			        //printf("[wm2018 Top16 htime ] wmstrA4: %s\n", wmstrA4);
				wmFinalMatches[0].htime[count] = wmstrA4;
			}*/

			if(ostrstr(wmstrA2, "<li class=\"heim\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].team1[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<li class=\"score\">") != NULL) {
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
			}

			if(ostrstr(wmstrA2, "<li class=\"auswaerts\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinalMatches[0].team2[count] = wmstrA3;
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

void GetWmFinaleData(int wmGroupID) {
	char* wmstr = NULL, *wmsearch = NULL, *wmregexA = NULL;
	char* wmstrA1 = NULL, *wmstrA2 = NULL, *wmstrA3 = NULL, *wmstrA4 = NULL, *wmstrA5 = NULL;

	debug(10, "GetWmFinaleData IN-0 (%d)\n", wmGroupID);
	//if (wmGroupID != 9) return;

	wmstr = malloc(MINMALLOC);
	wmsearch = malloc(MINMALLOC);
	wmregexA = malloc(MINMALLOC);
	wmstrA1 = malloc(MINMALLOC);
	wmstrA2 = malloc(MINMALLOC);
	wmstrA3 = malloc(MINMALLOC);
	wmstrA4 = malloc(MINMALLOC);
	wmstrA5 = malloc(MINMALLOC);

	debug(10, "GetWmFinaleData IN-1 (%d)\n", wmGroupID);

	wmsearch = ostrcat("wm-2018", NULL, 0, 0);
	wmregexA = ostrcat("<th class=\"match-round\" colspan=\"10\">Halbfinale</th>(.*?)MAISLABYRINTH ERFURT</span>", NULL, 0, 0);

	//wmsearch = ostrcat("wm-2018-finale", NULL, 0, 0);
	//wmregexA = ostrcat("<!-- .post-header -->(.*?)Hol dir das neue WM Trikot des DFB</h3>", NULL, 0, 0);

	if (wmsearch != NULL) {

		wmstr = gethttp("maislabyrinth-erfurt.de", wmsearch, 80, NULL, NULL, 6000, NULL, 0);
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

			if(ostrstr(wmstrA2, "<th class=\"match-date\" colspan=\"10\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinaleMatches[0].date[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"team-shortname team-shortname-home\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinaleMatches[0].team1[count] = wmstrA3;
			}

			if(ostrstr(wmstrA2, "<td class=\"match-time\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = ostrcat(wmstrA2, NULL, 0, 0);
				wmstrA3 = string_replace("-:-", "", wmstrA3, 1);
				wmstrA4 = string_striptags(wmstrA3);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA4);
				wmFinaleMatches[0].htime[count] = wmstrA4;
			}

			if(ostrstr(wmstrA2, "<td class=\"match-result match-result-0\">") != NULL){
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = ostrcat(wmstrA2, NULL, 0, 0);
				wmstrA3 = string_replace("(-:-)", "", wmstrA3, 1);
				wmstrA4 = string_striptags(wmstrA3);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA4);
				if( (ostrstr(wmstrA4, "&#160;") != NULL) || (ostrstr(wmstrA4, "&#8213;") != NULL) ) {
					wmstrA5 = ostrcat(wmstrA4, NULL, 0, 0);
					wmstrA5 = string_replace("&#160;", " - ", wmstrA5, 1);
					wmstrA5 = string_replace("&#8213;", " - ", wmstrA5, 1);
				} else {
					wmstrA5 = ostrcat(wmstrA4, NULL, 0, 0);
				}
				debug(20, "wmstrA4(%d): %s\n", count, wmstrA5);
			        //printf("[wm2018 result ] wmstrA4: %s\n", wmstrA5);
				wmFinaleMatches[0].result[count] = wmstrA5;
			}

			if(ostrstr(wmstrA2, "<td class=\"team-shortname team-shortname-away\">") != NULL) {
				debug(30, "wmstrA2(%d): %s\n", i, wmstrA2);
				wmstrA3 = string_striptags(wmstrA2);
				debug(20, "wmstrA3(%d): %s\n", count, wmstrA3);
				wmFinaleMatches[0].team2[count] = wmstrA3;
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

void screenwm2018() {

	struct skin* wm2018 = getscreen("wm2018");
	struct skin* titletext = getscreennode(wm2018, "titletext");

	struct skin* date_0 = getscreennode(wm2018, "date_0");
	struct skin* date_1 = getscreennode(wm2018, "date_1");
	struct skin* date_2 = getscreennode(wm2018, "date_2");
	struct skin* date_3 = getscreennode(wm2018, "date_3");
	struct skin* date_4 = getscreennode(wm2018, "date_4");
	struct skin* date_5 = getscreennode(wm2018, "date_5");

	struct skin* team1_0 = getscreennode(wm2018, "team1_0");
	struct skin* team1_1 = getscreennode(wm2018, "team1_1");
	struct skin* team1_2 = getscreennode(wm2018, "team1_2");
	struct skin* team1_3 = getscreennode(wm2018, "team1_3");
	struct skin* team1_4 = getscreennode(wm2018, "team1_4");
	struct skin* team1_5 = getscreennode(wm2018, "team1_5");

	struct skin* flag1_0 = getscreennode(wm2018, "flag1_0");
	struct skin* flag1_1 = getscreennode(wm2018, "flag1_1");
	struct skin* flag1_2 = getscreennode(wm2018, "flag1_2");
	struct skin* flag1_3 = getscreennode(wm2018, "flag1_3");
	struct skin* flag1_4 = getscreennode(wm2018, "flag1_4");
	struct skin* flag1_5 = getscreennode(wm2018, "flag1_5");

	struct skin* result_0 = getscreennode(wm2018, "result_0");
	struct skin* result_1 = getscreennode(wm2018, "result_1");
	struct skin* result_2 = getscreennode(wm2018, "result_2");
	struct skin* result_3 = getscreennode(wm2018, "result_3");
	struct skin* result_4 = getscreennode(wm2018, "result_4");
	struct skin* result_5 = getscreennode(wm2018, "result_5");

	struct skin* flag2_0 = getscreennode(wm2018, "flag2_0");
	struct skin* flag2_1 = getscreennode(wm2018, "flag2_1");
	struct skin* flag2_2 = getscreennode(wm2018, "flag2_2");
	struct skin* flag2_3 = getscreennode(wm2018, "flag2_3");
	struct skin* flag2_4 = getscreennode(wm2018, "flag2_4");
	struct skin* flag2_5 = getscreennode(wm2018, "flag2_5");

	struct skin* team2_0 = getscreennode(wm2018, "team2_0");
	struct skin* team2_1 = getscreennode(wm2018, "team2_1");
	struct skin* team2_2 = getscreennode(wm2018, "team2_2");
	struct skin* team2_3 = getscreennode(wm2018, "team2_3");
	struct skin* team2_4 = getscreennode(wm2018, "team2_4");
	struct skin* team2_5 = getscreennode(wm2018, "team2_5");

// ################################################################

	struct skin* TableNo_0 = getscreennode(wm2018, "TableNo_0");
	struct skin* TableNo_1 = getscreennode(wm2018, "TableNo_1");
	struct skin* TableNo_2 = getscreennode(wm2018, "TableNo_2");
	struct skin* TableNo_3 = getscreennode(wm2018, "TableNo_3");

	struct skin* TableNoV_0 = getscreennode(wm2018, "TableNoV_0");
	struct skin* TableNoV_1 = getscreennode(wm2018, "TableNoV_1");
	struct skin* TableNoV_2 = getscreennode(wm2018, "TableNoV_2");
	struct skin* TableNoV_3 = getscreennode(wm2018, "TableNoV_3");

	struct skin* Flag_0 = getscreennode(wm2018, "Flag_0");
	struct skin* Flag_1 = getscreennode(wm2018, "Flag_1");
	struct skin* Flag_2 = getscreennode(wm2018, "Flag_2");
	struct skin* Flag_3 = getscreennode(wm2018, "Flag_3");

	struct skin* Team_0 = getscreennode(wm2018, "Team_0");
	struct skin* Team_1 = getscreennode(wm2018, "Team_1");
	struct skin* Team_2 = getscreennode(wm2018, "Team_2");
	struct skin* Team_3 = getscreennode(wm2018, "Team_3");

	struct skin* Played_0 = getscreennode(wm2018, "Played_0");
	struct skin* Played_1 = getscreennode(wm2018, "Played_1");
	struct skin* Played_2 = getscreennode(wm2018, "Played_2");
	struct skin* Played_3 = getscreennode(wm2018, "Played_3");

	struct skin* Won_0 = getscreennode(wm2018, "Won_0");
	struct skin* Won_1 = getscreennode(wm2018, "Won_1");
	struct skin* Won_2 = getscreennode(wm2018, "Won_2");
	struct skin* Won_3 = getscreennode(wm2018, "Won_3");

	struct skin* Draw_0 = getscreennode(wm2018, "Draw_0");
	struct skin* Draw_1 = getscreennode(wm2018, "Draw_1");
	struct skin* Draw_2 = getscreennode(wm2018, "Draw_2");
	struct skin* Draw_3 = getscreennode(wm2018, "Draw_3");

	struct skin* Lost_0 = getscreennode(wm2018, "Lost_0");
	struct skin* Lost_1 = getscreennode(wm2018, "Lost_1");
	struct skin* Lost_2 = getscreennode(wm2018, "Lost_2");
	struct skin* Lost_3 = getscreennode(wm2018, "Lost_3");

	struct skin* Goals_0 = getscreennode(wm2018, "Goals_0");
	struct skin* Goals_1 = getscreennode(wm2018, "Goals_1");
	struct skin* Goals_2 = getscreennode(wm2018, "Goals_2");
	struct skin* Goals_3 = getscreennode(wm2018, "Goals_3");

	struct skin* Diff_0 = getscreennode(wm2018, "Diff_0");
	struct skin* Diff_1 = getscreennode(wm2018, "Diff_1");
	struct skin* Diff_2 = getscreennode(wm2018, "Diff_2");
	struct skin* Diff_3 = getscreennode(wm2018, "Diff_3");

	struct skin* Points_0 = getscreennode(wm2018, "Points_0");
	struct skin* Points_1 = getscreennode(wm2018, "Points_1");
	struct skin* Points_2 = getscreennode(wm2018, "Points_2");
	struct skin* Points_3 = getscreennode(wm2018, "Points_3");

// ################################################################

	struct skin* wm2018_last16 = getscreen("wm2018_last16");
	struct skin* titletext_last16 = getscreennode(wm2018_last16, "titletext");

	struct skin* date_0_last16 = getscreennode(wm2018_last16, "date_0");
	struct skin* date_1_last16 = getscreennode(wm2018_last16, "date_1");
	struct skin* date_2_last16 = getscreennode(wm2018_last16, "date_2");
	struct skin* date_3_last16 = getscreennode(wm2018_last16, "date_3");
	struct skin* date_4_last16 = getscreennode(wm2018_last16, "date_4");
	struct skin* date_5_last16 = getscreennode(wm2018_last16, "date_5");
	struct skin* date_6_last16 = getscreennode(wm2018_last16, "date_6");
	struct skin* date_7_last16 = getscreennode(wm2018_last16, "date_7");

	struct skin* team1_0_last16 = getscreennode(wm2018_last16, "team1_0");
	struct skin* team1_1_last16 = getscreennode(wm2018_last16, "team1_1");
	struct skin* team1_2_last16 = getscreennode(wm2018_last16, "team1_2");
	struct skin* team1_3_last16 = getscreennode(wm2018_last16, "team1_3");
	struct skin* team1_4_last16 = getscreennode(wm2018_last16, "team1_4");
	struct skin* team1_5_last16 = getscreennode(wm2018_last16, "team1_5");
	struct skin* team1_6_last16 = getscreennode(wm2018_last16, "team1_6");
	struct skin* team1_7_last16 = getscreennode(wm2018_last16, "team1_7");

	struct skin* flag1_0_last16 = getscreennode(wm2018_last16, "flag1_0");
	struct skin* flag1_1_last16 = getscreennode(wm2018_last16, "flag1_1");
	struct skin* flag1_2_last16 = getscreennode(wm2018_last16, "flag1_2");
	struct skin* flag1_3_last16 = getscreennode(wm2018_last16, "flag1_3");
	struct skin* flag1_4_last16 = getscreennode(wm2018_last16, "flag1_4");
	struct skin* flag1_5_last16 = getscreennode(wm2018_last16, "flag1_5");
	struct skin* flag1_6_last16 = getscreennode(wm2018_last16, "flag1_6");
	struct skin* flag1_7_last16 = getscreennode(wm2018_last16, "flag1_7");

	/*struct skin* htime_0_last16 = getscreennode(wm2018_last16, "htime_0");
	struct skin* htime_1_last16 = getscreennode(wm2018_last16, "htime_1");
	struct skin* htime_2_last16 = getscreennode(wm2018_last16, "htime_2");
	struct skin* htime_3_last16 = getscreennode(wm2018_last16, "htime_3");
	struct skin* htime_4_last16 = getscreennode(wm2018_last16, "htime_4");
	struct skin* htime_5_last16 = getscreennode(wm2018_last16, "htime_5");
	struct skin* htime_6_last16 = getscreennode(wm2018_last16, "htime_6");
	struct skin* htime_7_last16 = getscreennode(wm2018_last16, "htime_7");*/

	struct skin* result_0_last16 = getscreennode(wm2018_last16, "result_0");
	struct skin* result_1_last16 = getscreennode(wm2018_last16, "result_1");
	struct skin* result_2_last16 = getscreennode(wm2018_last16, "result_2");
	struct skin* result_3_last16 = getscreennode(wm2018_last16, "result_3");
	struct skin* result_4_last16 = getscreennode(wm2018_last16, "result_4");
	struct skin* result_5_last16 = getscreennode(wm2018_last16, "result_5");
	struct skin* result_6_last16 = getscreennode(wm2018_last16, "result_6");
	struct skin* result_7_last16 = getscreennode(wm2018_last16, "result_7");

	struct skin* flag2_0_last16 = getscreennode(wm2018_last16, "flag2_0");
	struct skin* flag2_1_last16 = getscreennode(wm2018_last16, "flag2_1");
	struct skin* flag2_2_last16 = getscreennode(wm2018_last16, "flag2_2");
	struct skin* flag2_3_last16 = getscreennode(wm2018_last16, "flag2_3");
	struct skin* flag2_4_last16 = getscreennode(wm2018_last16, "flag2_4");
	struct skin* flag2_5_last16 = getscreennode(wm2018_last16, "flag2_5");
	struct skin* flag2_6_last16 = getscreennode(wm2018_last16, "flag2_6");
	struct skin* flag2_7_last16 = getscreennode(wm2018_last16, "flag2_7");

	struct skin* team2_0_last16 = getscreennode(wm2018_last16, "team2_0");
	struct skin* team2_1_last16 = getscreennode(wm2018_last16, "team2_1");
	struct skin* team2_2_last16 = getscreennode(wm2018_last16, "team2_2");
	struct skin* team2_3_last16 = getscreennode(wm2018_last16, "team2_3");
	struct skin* team2_4_last16 = getscreennode(wm2018_last16, "team2_4");
	struct skin* team2_5_last16 = getscreennode(wm2018_last16, "team2_5");
	struct skin* team2_6_last16 = getscreennode(wm2018_last16, "team2_6");
	struct skin* team2_7_last16 = getscreennode(wm2018_last16, "team2_7");

// ################################################################

	struct skin* wm2018_final = getscreen("wm2018_final");
	struct skin* titletext_final = getscreennode(wm2018_final, "titletext");

	struct skin* date_0_final = getscreennode(wm2018_final, "date_0");
	struct skin* date_1_final = getscreennode(wm2018_final, "date_1");
	struct skin* date_2_final = getscreennode(wm2018_final, "date_2");
	struct skin* date_3_final = getscreennode(wm2018_final, "date_3");

	struct skin* team1_0_final = getscreennode(wm2018_final, "team1_0");
	struct skin* team1_1_final = getscreennode(wm2018_final, "team1_1");
	struct skin* team1_2_final = getscreennode(wm2018_final, "team1_2");
	struct skin* team1_3_final = getscreennode(wm2018_final, "team1_3");

	struct skin* flagge1_0_final = getscreennode(wm2018_final, "flagge1_0");
	struct skin* flagge1_1_final = getscreennode(wm2018_final, "flagge1_1");
	struct skin* flagge1_2_final = getscreennode(wm2018_final, "flagge1_2");
	struct skin* flagge1_3_final = getscreennode(wm2018_final, "flagge1_3");

	struct skin* htime_0_final = getscreennode(wm2018_final, "htime_0");
	struct skin* htime_1_final = getscreennode(wm2018_final, "htime_1");
	struct skin* htime_2_final = getscreennode(wm2018_final, "htime_2");
	struct skin* htime_3_final = getscreennode(wm2018_final, "htime_3");

	struct skin* result_0_final = getscreennode(wm2018_final, "result_0");
	struct skin* result_1_final = getscreennode(wm2018_final, "result_1");
	struct skin* result_2_final = getscreennode(wm2018_final, "result_2");
	struct skin* result_3_final = getscreennode(wm2018_final, "result_3");

	struct skin* flagge2_0_final = getscreennode(wm2018_final, "flagge2_0");
	struct skin* flagge2_1_final = getscreennode(wm2018_final, "flagge2_1");
	struct skin* flagge2_2_final = getscreennode(wm2018_final, "flagge2_2");
	struct skin* flagge2_3_final = getscreennode(wm2018_final, "flagge2_3");

	struct skin* team2_0_final = getscreennode(wm2018_final, "team2_0");
	struct skin* team2_1_final = getscreennode(wm2018_final, "team2_1");
	struct skin* team2_2_final = getscreennode(wm2018_final, "team2_2");
	struct skin* team2_3_final = getscreennode(wm2018_final, "team2_3");


// ################################################################

	int rcret = 0, wmGroupID = 0;
	char* tmpstr = NULL;
	char* tmpstr2 = NULL;
	int i = 0;

	tmpstr = malloc(MINMALLOC);
	tmpstr2 = malloc(MINMALLOC);
	
	//tmpstr = malloc(sizeof(tmpstr));
	//tmpstr2 = malloc(sizeof(tmpstr2));

	for (i = 0; i < 10; i++) {
		wmGroupDataRefresh[i] = 1;
		// low performance!
		//GetWmGroupData(i);
	}

	if ( getconfigint("wm2018_groupid", NULL) > 0  && getconfigint("wm2018_groupid", NULL) < 10 ) {
		wmGroupID = getconfigint("wm2018_groupid", NULL);
		debug(10, "main getconfig: wmGroupID = %d\n", wmGroupID);
	} else {
		wmGroupID = 8;
		debug(10, "main default: wmGroupID = 8 = %d\n", wmGroupID);
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
		debug(10, "main: wmGroupID < 8 (%d)\n", wmGroupID);
		if (wmGroupDataRefresh[wmGroupID] == 1) GetWmGroupData(wmGroupID);
	} else if (wmGroupID == 8) {
		debug(10, "main: wmGroupID == 8 else if (%d)\n", wmGroupID);
		if (wmGroupDataRefresh[wmGroupID] == 1) GetWmFinalData(wmGroupID);
	} else if (wmGroupID == 9) {
		debug(10, "main: wmGroupID == 9 else if (%d)\n", wmGroupID);
		if (wmGroupDataRefresh[wmGroupID] == 1) GetWmFinaleData(wmGroupID);
	}

	tmpstr = ostrcat("FIFA Fußball-Weltmeisterschaft 2018", " - ", 0, 0);

	switch (wmGroupID) {
		case 0: tmpstr = ostrcat(tmpstr, "Gruppe A", 1, 0); break;
		case 1: tmpstr = ostrcat(tmpstr, "Gruppe B", 1, 0); break;
		case 2: tmpstr = ostrcat(tmpstr, "Gruppe C", 1, 0); break;
		case 3: tmpstr = ostrcat(tmpstr, "Gruppe D", 1, 0); break;
		case 4: tmpstr = ostrcat(tmpstr, "Gruppe E", 1, 0); break;
		case 5: tmpstr = ostrcat(tmpstr, "Gruppe F", 1, 0); break;
		case 6: tmpstr = ostrcat(tmpstr, "Gruppe G", 1, 0); break;
		case 7: tmpstr = ostrcat(tmpstr, "Gruppe H", 1, 0); break;
		case 8: tmpstr = ostrcat(tmpstr, "Achtelfinale", 1, 0); break;
		case 9: tmpstr = ostrcat(tmpstr, "Finale Runden", 1, 0); break;
		default: break;
	}

	if (wmGroupID < 8) {

		changetitle(wm2018, tmpstr);
		changetext(titletext, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(date_0, wmGroupMatches[wmGroupID].date[0]);
		changetext(date_1, wmGroupMatches[wmGroupID].date[1]);
		changetext(date_2, wmGroupMatches[wmGroupID].date[2]);
		changetext(date_3, wmGroupMatches[wmGroupID].date[3]);
		changetext(date_4, wmGroupMatches[wmGroupID].date[4]);
		changetext(date_5, wmGroupMatches[wmGroupID].date[5]);

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

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team1[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_0, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team1[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_1, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team1[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_2, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team1[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_3, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team1[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_4, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team1[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_5, tmpstr);

		free(tmpstr); tmpstr = NULL;
		//free(tmpstr2); tmpstr2 = NULL;

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team2[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_0, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team2[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_1, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team2[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_2, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team2[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_3, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team2[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_4, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupMatches[wmGroupID].team2[5], 0, 0);
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

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupTables[wmGroupID].Team[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace_all("Ä,,", "Ae", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_0, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupTables[wmGroupID].Team[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace_all("Ä", "Ae", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_1, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupTables[wmGroupID].Team[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace_all("Ä", "Ae", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_2, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmGroupTables[wmGroupID].Team[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		//tmpstr = string_replace_all("Ä", "Ae", tmpstr, 1); // doesn't work, let's change the filename
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(Flag_3, tmpstr);

		free(tmpstr); tmpstr = NULL;

	}

	if (wmGroupID == 8) {
		changetitle(wm2018_last16, tmpstr);
		changetext(titletext_last16, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(date_0_last16, wmFinalMatches[0].date[0]);
		changetext(date_1_last16, wmFinalMatches[0].date[1]);
		changetext(date_2_last16, wmFinalMatches[0].date[2]);
		changetext(date_3_last16, wmFinalMatches[0].date[3]);
		changetext(date_4_last16, wmFinalMatches[0].date[4]);
		changetext(date_5_last16, wmFinalMatches[0].date[5]);
		changetext(date_6_last16, wmFinalMatches[0].date[6]);
		changetext(date_7_last16, wmFinalMatches[0].date[7]);

		changetext(team1_0_last16, wmFinalMatches[0].team1[0]);
		changetext(team1_1_last16, wmFinalMatches[0].team1[1]);
		changetext(team1_2_last16, wmFinalMatches[0].team1[2]);
		changetext(team1_3_last16, wmFinalMatches[0].team1[3]);
		changetext(team1_4_last16, wmFinalMatches[0].team1[4]);
		changetext(team1_5_last16, wmFinalMatches[0].team1[5]);
		changetext(team1_6_last16, wmFinalMatches[0].team1[6]);
		changetext(team1_7_last16, wmFinalMatches[0].team1[7]);

		/*changetext(htime_0_last16, wmFinalMatches[0].htime[0]);
		changetext(htime_1_last16, wmFinalMatches[0].htime[1]);
		changetext(htime_2_last16, wmFinalMatches[0].htime[2]);
		changetext(htime_3_last16, wmFinalMatches[0].htime[3]);
		changetext(htime_4_last16, wmFinalMatches[0].htime[4]);
		changetext(htime_5_last16, wmFinalMatches[0].htime[5]);
		changetext(htime_6_last16, wmFinalMatches[0].htime[6]);
		changetext(htime_7_last16, wmFinalMatches[0].htime[7]);*/

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

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_0_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_1_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_2_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_3_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_4_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_5_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[6], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_6_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team1[7], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag1_7_last16, tmpstr);

		free(tmpstr); tmpstr = NULL;
		//free(tmpstr2); tmpstr2 = NULL;

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_0_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_1_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_2_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_3_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[4], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_4_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[5], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_5_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[6], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_6_last16, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinalMatches[0].team2[7], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace(" ", "_", tmpstr, 1);
		changepic(flag2_7_last16, tmpstr);

		free(tmpstr); tmpstr = NULL;

	}

	if (wmGroupID == 9) {
		changetitle(wm2018_final, tmpstr);
		changetext(titletext_final, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changetext(date_0_final, wmFinaleMatches[0].date[0]);
		changetext(date_1_final, wmFinaleMatches[0].date[1]);
		changetext(date_2_final, wmFinaleMatches[0].date[2]);
		changetext(date_3_final, wmFinaleMatches[0].date[3]);

		changetext(team1_0_final, wmFinaleMatches[0].team1[0]);
		changetext(team1_1_final, wmFinaleMatches[0].team1[1]);
		changetext(team1_2_final, wmFinaleMatches[0].team1[2]);
		changetext(team1_3_final, wmFinaleMatches[0].team1[3]);

		changetext(htime_0_final, wmFinaleMatches[0].htime[0]);
		changetext(htime_1_final, wmFinaleMatches[0].htime[1]);
		changetext(htime_2_final, wmFinaleMatches[0].htime[2]);
		changetext(htime_3_final, wmFinaleMatches[0].htime[3]);

		changetext(result_0_final, wmFinaleMatches[0].result[0]);
		changetext(result_1_final, wmFinaleMatches[0].result[1]);
		changetext(result_2_final, wmFinaleMatches[0].result[2]);
		changetext(result_3_final, wmFinaleMatches[0].result[3]);

		changetext(team2_0_final, wmFinaleMatches[0].team2[0]);
		changetext(team2_1_final, wmFinaleMatches[0].team2[1]);
		changetext(team2_2_final, wmFinaleMatches[0].team2[2]);
		changetext(team2_3_final, wmFinaleMatches[0].team2[3]);

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team1[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge1_0_final, tmpstr);

		free(tmpstr); tmpstr = NULL;
		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team1[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge1_1_final, tmpstr);

		free(tmpstr); tmpstr = NULL;
		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team1[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge1_2_final, tmpstr);

		free(tmpstr); tmpstr = NULL;
		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team1[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge1_3_final, tmpstr);

		free(tmpstr); tmpstr = NULL;

// ################################################################

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team2[0], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge2_0_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team2[1], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge2_1_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team2[2], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge2_2_final, tmpstr);

		tmpstr2 = ostrcat("%pluginpath%/wm2018/skin/", wmFinaleMatches[0].team2[3], 0, 0);
		tmpstr = string_tolower(tmpstr2);
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
		tmpstr = string_replace_all(" ", "_", tmpstr, 1);
		changepic(flagge2_3_final, tmpstr);

		free(tmpstr); tmpstr = NULL;

	}

// ################################################################

	if (wmGroupID < 8) {
		drawscreen(wm2018, 0, 0);
	} else if (wmGroupID == 8) {
		drawscreen(wm2018_last16, 0, 0);
	} else if (wmGroupID == 9) {
		drawscreen(wm2018_final, 0, 0);
	}

	while(1) {
		rcret = waitrc(wm2018, 0, 0);

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
	addconfigint("wm2018_groupid", wmGroupID);

	delownerrc(wm2018);
	delownerrc(wm2018_last16);
	delownerrc(wm2018_final);
	delmarkedscreennodes(wm2018, 1);
	delmarkedscreennodes(wm2018_last16, 1);
	delmarkedscreennodes(wm2018_final, 1);
	//free all ?
	clearscreen(wm2018);
	clearscreen(wm2018_last16);
	clearscreen(wm2018_final);
}

#endif
