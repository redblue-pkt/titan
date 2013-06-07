#ifndef PANEL_SETTINGS_WIZARD_H
#define PANEL_SETTINGS_WIZARD_H

void screenpanel_wizard()
{
	if(textbox(_("Message"), _("Do you really want to start the Wizard?\nThe Box will reboot and the Wizard will be started!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		debug(40, "Wizard");
		system("touch /var/etc/.firstwizard");
		sleep(4);
		oshutdown(2, 1);
	}       
}

#endif
