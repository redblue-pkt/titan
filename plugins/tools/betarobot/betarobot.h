#ifndef BETAROBOT_H
#define BETAROBOT_H

betarobotthread()
{
	int max = 0, akt = 0, rccode = -1, r = 0;
	
	//get max rc entry
	m_lock(&status.clistmutex, 12);
	struct clist **clist = rcconfig;
	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];

		while(node != NULL)
		{
			max++;
			node = node->next;
		}
	}
	m_unlock(&status.clistmutex, 12);
	
	while(1)
	{
		r = getrandomnum(max);
		akt = 0, rccode = -1;
		
		m_lock(&status.clistmutex, 12);
		clist = rcconfig
		for(i = 0; i < LISTHASHSIZE; i++)
		{
			node = clist[i];
	
			while(node != NULL)
			{
				akt++;
				if(akt == r)
				{
					if(node->value != NULL)
						rccode = atoi(node->value);
					break;		
				}
				node = node->next;
			}
		}
		m_unlock(&status.clistmutex, 12);
		
		if(rccode != -1)
			writerc(rccode);
	}
}

void screenbetarobot()
{
	int pinret = 0;
	
	if(textbox(_("Message"), _("This Plugin is only for Betatester and can change your Settings!\nRealy run it?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 300, 0, 0) != 1) return;

	if(!file_exist("/etc/.beta"))
		pinret = screenpincheck(3, NULL);
	if(pinret != 0) return;
	
	writerc(getrcconfigint("rcexit", NULL));
	writerc(getrcconfigint("rcexit", NULL));
	writerc(getrcconfigint("rcexit", NULL));
	writerc(getrcconfigint("rcexit", NULL));
	writerc(getrcconfigint("rcexit", NULL));	
	addtimer(&betarobotthread, START, 1000, 1, NULL, NULL, NULL);
}

#endif
