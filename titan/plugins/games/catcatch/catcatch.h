#ifndef CATCATCH_H
#define CATCATCH_H

int checktreffer(int nr, int* sum, int* cat, int* value)
{
	int ret = -1;
	
	if(cat[nr] > 0)
	{
		*sum += value[nr];
		cat[nr] = 0;
		value[nr] = 0;
		ret = nr;
	}
	else
		*sum -= 2;
	
	return ret;	
}

void screencatcatch()
{
	int rcret = 0, level = 1, levelsub = 0, i = 0, sum = 0, ret = 0;
	int cat[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int value[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	struct skin* catcatch = getscreen("catcatch");
	struct skin* skinsum = getscreennode(catcatch, "sum");
	struct skin* skinlevel = getscreennode(catcatch, "level");	
	struct skin* node0 = getscreennode(catcatch, "node0");
	struct skin* node1 = getscreennode(catcatch, "node1");
	struct skin* node2 = getscreennode(catcatch, "node2");
	struct skin* node3 = getscreennode(catcatch, "node3");
	struct skin* node4 = getscreennode(catcatch, "node4");
	struct skin* node5 = getscreennode(catcatch, "node5");
	struct skin* node6 = getscreennode(catcatch, "node6");
	struct skin* node7 = getscreennode(catcatch, "node7");
	struct skin* node8 = getscreennode(catcatch, "node8");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;
	
	changetext(skinsum, "0");
	changetext(skinlevel, "1");
	changepic(node0, NULL);
	node0->bordercol = 0x0000FF;
	changepic(node1, NULL);
	node1->bordercol = 0x0000FF;
	changepic(node2, NULL);
	node2->bordercol = 0x0000FF;
	changepic(node3, NULL);
	node3->bordercol = 0x0000FF;
	changepic(node4, NULL);
	node4->bordercol = 0x0000FF;
	changepic(node5, NULL);
	node5->bordercol = 0x0000FF;
	changepic(node6, NULL);
	node6->bordercol = 0x0000FF;
	changepic(node7, NULL);
	node7->bordercol = 0x0000FF;
	changepic(node8, NULL);
	node8->bordercol = 0x0000FF;
	drawscreen(catcatch, 0, 0);

	while(1)
	{
		rcret = waitrc(catcatch, 3000 - levelsub, 0);
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		level = 1; levelsub = 0;
		if(sum > 50) {level = 2; levelsub = 300;}
		if(sum > 100) {level = 3; levelsub = 600;}
		if(sum > 150) {level = 4; levelsub = 900;}
		if(sum > 200) {level = 5; levelsub = 1200;}
		if(sum > 250) {level = 6; levelsub = 1500;}
		if(sum > 300) {level = 7; levelsub = 1800;}
		if(sum > 350) {level = 8; levelsub = 2100;}
		if(sum > 400) {level = 9; levelsub = 2400;}
		if(sum > 450) {level = 10; levelsub = 2700;}
		
		ret = -1;
		if(rcret == getrcconfigint("rc1", NULL))
			ret = checktreffer(0, &sum, cat, value);
		if(rcret == getrcconfigint("rc2", NULL))
			ret = checktreffer(1, &sum, cat, value);
		if(rcret == getrcconfigint("rc3", NULL))
			ret = checktreffer(2, &sum, cat, value);
		if(rcret == getrcconfigint("rc4", NULL))
			ret = checktreffer(3, &sum, cat, value);
		if(rcret == getrcconfigint("rc5", NULL))
			ret = checktreffer(4, &sum, cat, value);
		if(rcret == getrcconfigint("rc6", NULL))
			ret = checktreffer(5, &sum, cat, value);
		if(rcret == getrcconfigint("rc7", NULL))
			ret = checktreffer(6, &sum, cat, value);
		if(rcret == getrcconfigint("rc8", NULL))
			ret = checktreffer(7, &sum, cat, value);
		if(rcret == getrcconfigint("rc9", NULL))
			ret = checktreffer(8, &sum, cat, value);		
		
		//calculate
		for(i = 0; i < 9; i++)
		{
			if(cat[i] > 0)
			{
				cat[i]--;
				if(cat[i] <= 0)
				{
					if(value[i] > 0)
						sum -= value[i];
					value[i] = 0;
				}
			}
		}
		
		int r = getrandomnum(9);
		if(cat[r] <= 0)
		{
			cat[r] = 1;
			if(getrandomnum(4) == 1)
				value[r] = (getrandomnum(4) + 1) * -1;
			else
				value[r] = getrandomnum(4) + 1;
		}
		
		//show treffer
		tmp = NULL;
		if(ret > -1)
		{
			if(ret == 0) tmp = node0;
			else if(ret == 1) tmp = node1;
			else if(ret == 2) tmp = node2;
			else if(ret == 3) tmp = node3;
			else if(ret == 4) tmp = node4;
			else if(ret == 5) tmp = node5;
			else if(ret == 6) tmp = node6;
			else if(ret == 7) tmp = node7;
			else if(ret == 8) tmp = node8;

			if(tmp != NULL)
			{
				changepic(tmp, "%pluginpath%/catcatch/skin/treffer.png");
				tmp->bordercol = 0x0000FF;
				drawscreen(catcatch, 0, 0);
				usleep(100000);
				changepic(tmp, NULL);
			}
		}
		
		//redraw
		tmp = NULL;
		for(i = 0; i < 9; i++)
		{
			if(i == 0) tmp = node0;
			else if(i == 1) tmp = node1;
			else if(i == 2) tmp = node2;
			else if(i == 3) tmp = node3;
			else if(i == 4) tmp = node4;
			else if(i == 5) tmp = node5;
			else if(i == 6) tmp = node6;
			else if(i == 7) tmp = node7;
			else if(i == 8) tmp = node8;
			
			if(tmp != NULL)
			{
				if(cat[i] <= 0)
				{
					changepic(tmp, NULL);
					tmp->bordercol = 0x0000FF;
				}
				else
				{
					switch(value[i])
					{
						case -1: changepic(tmp, "%pluginpath%/catcatch/skin/cat1.png"); tmp->bordercol = 0xFF0000; break;
						case -2: changepic(tmp, "%pluginpath%/catcatch/skin/cat2.png"); tmp->bordercol = 0xFF0000; break;
						case -3: changepic(tmp, "%pluginpath%/catcatch/skin/cat3.png"); tmp->bordercol = 0xFF0000; break;
						case -4: changepic(tmp, "%pluginpath%/catcatch/skin/cat4.png"); tmp->bordercol = 0xFF0000; break;
						case -5: changepic(tmp, "%pluginpath%/catcatch/skin/cat5.png"); tmp->bordercol = 0xFF0000; break;
						case 1: changepic(tmp, "%pluginpath%/catcatch/skin/cat1.png"); tmp->bordercol = 0x00FF00; break;
						case 2: changepic(tmp, "%pluginpath%/catcatch/skin/cat2.png"); tmp->bordercol = 0x00FF00; break;
						case 3: changepic(tmp, "%pluginpath%/catcatch/skin/cat3.png"); tmp->bordercol = 0x00FF00; break;
						case 4: changepic(tmp, "%pluginpath%/catcatch/skin/cat4.png"); tmp->bordercol = 0x00FF00; break;
						case 5: changepic(tmp, "%pluginpath%/catcatch/skin/cat5.png"); tmp->bordercol = 0x00FF00; break;			
					}
				}
			}
		}
		
		tmpstr = oitoa(sum);
		changetext(skinsum, tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		tmpstr = oitoa(level);
		changetext(skinlevel, tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		drawscreen(catcatch, 0, 0);
	}
	
	clearscreen(catcatch);
}

#endif
