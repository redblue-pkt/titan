#ifndef PANEL_SETTINGS_FANCONTROL_H
#define PANEL_SETTINGS_FANCONTROL_H

void screenpanel_settings_fancontrol()
{
	int rcret = 0;
	int speed = 0;
	char* speedWert = NULL;
	
	struct skin* panel_fancontrol = getscreen("panel_settings_fancontrol");
	struct skin* fanprogress = getscreennode(panel_fancontrol, "fanprogress");

	speedWert = getconfig("fanspeed", NULL);
	if (speedWert != NULL) 
	{
		if(ostrcmp(speedWert, "115") == 0)
			speed = 0;
		else if(ostrcmp(speedWert, "130") == 0)
			speed = 25;
		else if(ostrcmp(speedWert, "145") == 0)
			speed = 50;
		else if(ostrcmp(speedWert, "155") == 0)
			speed = 75;
		else if(ostrcmp(speedWert, "170") == 0)
			speed = 100;
	}
	else 
		speed = 100;
		
	fanprogress->progresssize = speed;
	drawscreen(panel_fancontrol, 0, 0);

	while(1)
	{
		rcret = waitrc(panel_fancontrol, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) {
			setfanspeed(-1, 0);
			break;
		}
		if(rcret == getrcconfigint("rcgreen", NULL)){
			setfanspeed(speed, 1);
			break;
		}
		if(rcret == getrcconfigint("rcleft", NULL)){
			if(speed > 0)
				speed = speed - 25;
			fanprogress->progresssize = speed;
			setfanspeed(speed, 0);
			drawscreen(panel_fancontrol, 0, 0);
		}
		if(rcret == getrcconfigint("rcright", NULL)){
			if(speed < 100)
				speed = speed + 25;
			fanprogress->progresssize = speed;
			setfanspeed(speed, 0);
			drawscreen(panel_fancontrol, 0, 0);
		}
	}
}

#endif