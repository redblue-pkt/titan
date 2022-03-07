#ifndef MC_MOUNTER_H
#define MC_MOUNTER_H

char* tmpmounter = NULL;
char* rarMountDir = NULL;
char* rarMountDirSplit = NULL;
char* imgMountDir = NULL;
char* imgMountDirSplit = NULL;
char* isoMountDir = NULL;
char* isoMountDirSplit = NULL;
char* lastpath = NULL;

int rarFS  = 0;
int isoFS  = 0;
int imgFS  = 0; 

int mc_mounter_umount()
{
	debug(50, "rarFS=%d imgFS=%d isoFS=%d", rarFS, imgFS, isoFS);
	char* cmd = NULL;

	if(isoFS == 1)
	{
		cmd = ostrcat(cmd, "/bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3` `mount | grep udf | cut -d ' ' -f3`", 1, 0);
		debug(50, "cmd: %s", cmd);
		system(cmd);
		free(isoMountDir), isoMountDir = NULL;
		free(isoMountDirSplit), isoMountDirSplit = NULL;
		isoFS = 0;
	}
	else if(imgFS == 1)
	{
		cmd = ostrcat(cmd, "/bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3` `mount | grep udf | cut -d ' ' -f3`", 1, 0);
		debug(50, "cmd: %s", cmd);
		system(cmd);
		free(imgMountDir), imgMountDir = NULL;
		free(imgMountDirSplit), imgMountDirSplit = NULL;
		imgFS = 0;
	}
	else if(rarFS == 1)
	{
		cmd = ostrcat("/bin/umount -fl `mount | grep fuse | grep rarfs | grep -v '/dev/' | cut -d ' ' -f3` ", rarMountDir, 0, 0);
		cmd = ostrcat(cmd, " ; killall -9 rarfs", 1, 0);
		debug(50, "cmd: %s", cmd);
		system(cmd);
		free(rarMountDir), rarMountDir = NULL;
		free(rarMountDirSplit), rarMountDirSplit = NULL;
		rarFS = 0;
	}
	free(cmd), cmd = NULL;

	return 0;
}


char* mc_mounter_main(int mode, char* tmpstr, struct skin* filelistpath, struct skin* filelist, struct skin* mc_videoplayer, char* filemask, int tmpview, char* path)
{
	debug(50, "start");

	if(cmpfilenameext(tmpstr, ".rar") == 0)
	{
		debug(50, "found rar file: %s", tmpstr);
		mkdir("/tmp/rarfs", 0777);

		tmpmounter = ostrcat("rarfs \"", tmpstr, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\" /tmp/rarfs", 1, 0);
		debug(50, "mount line1: %s", tmpmounter);
		system(tmpmounter);

		tmpmounter = ostrcat("mount --bind /tmp/rarfs \"", filelistpath->text, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\"", 1, 0);
		debug(50, "line2: %s", tmpmounter);

		free(tmpstr),tmpstr = NULL;
		system(tmpmounter);

		debug(50, "save mc_videoplayerpath: %s", filelistpath->text);
		addconfig("mc_vp_path", filelistpath->text);
		addscreenrc(mc_videoplayer, filelist);
		getfilelist(mc_videoplayer, filelistpath, filelist, path, filemask, tmpview, NULL);

		rarMountDir = ostrcat(rarMountDir, filelistpath->text, 1, 0);
		debug(50, "save rarMountDir: %s", rarMountDir);

		rarMountDirSplit = ostrcat(rarMountDirSplit, filelistpath->text, 1, 0);
		dirname(rarMountDirSplit);
		debug(50, "save rarMountDirSplit: %s", rarMountDirSplit);

		rarFS = 1;
	}
	else if(cmpfilenameext(tmpstr, ".img") == 0)
	{
		debug(50, "found img file: %s", tmpstr);
		mkdir("/tmp/imgfs", 777);
		tmpmounter = ostrcat("mount -o loop \"", tmpstr, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\" /tmp/imgfs", 1, 0); 

		debug(50, "mount line1: %s", tmpmounter);
		system(tmpmounter);

		tmpmounter = ostrcat("mount --bind /tmp/imgfs \"", filelistpath->text, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\"", 1, 0);
		debug(50, "mount line2: %s", tmpmounter);

		free(tmpstr),tmpstr = NULL;
		system(tmpmounter);

		debug(50, "save mc_videoplayerpath: %s", filelistpath->text);
		addconfig("mc_vp_path", filelistpath->text);
		addscreenrc(mc_videoplayer, filelist);
		getfilelist(mc_videoplayer, filelistpath, filelist, path, filemask, tmpview, NULL);

		imgMountDir = ostrcat(imgMountDir, filelistpath->text, 1, 0);
		debug(50, "save imgMountDir: %s", imgMountDir);

		imgMountDirSplit = ostrcat(imgMountDirSplit, filelistpath->text, 1, 0);
		dirname(imgMountDirSplit);
		debug(50, "save imgMountDirSplit: %s", imgMountDirSplit);

		imgFS = 1;
	}
	else if(cmpfilenameext(tmpstr, ".iso") == 0)
	{
		debug(50, "found iso file: %s", tmpstr);
		mkdir("/tmp/isofs", 0777);

		tmpmounter = ostrcat("mount -o loop \"", tmpstr, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\" /tmp/isofs", 1, 0); 

		debug(50, "mount line1: %s", tmpmounter);
		system(tmpmounter);

		tmpmounter = ostrcat("/bin/mount --bind /tmp/isofs \"", filelistpath->text, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\"", 1, 0);
		debug(50, "mount line2: %s", tmpmounter);

		free(tmpstr),tmpstr = NULL;
		system(tmpmounter);

		debug(50, "save mc_videoplayerpath: %s", filelistpath->text);
		addconfig("mc_vp_path", filelistpath->text);
		addscreenrc(mc_videoplayer, filelist);
		getfilelist(mc_videoplayer, filelistpath, filelist, path, filemask, tmpview, NULL);

		isoMountDir = ostrcat(imgMountDir, filelistpath->text, 1, 0);
		debug(50, "save isoMountDir: %s", isoMountDir);

		isoMountDirSplit = ostrcat(isoMountDirSplit, filelistpath->text, 1, 0);
		dirname(isoMountDirSplit);
		debug(50, "save isoMountDirSplit: %s", isoMountDirSplit);

		isoFS = 1;
	}
	return 0;
}

char mc_mounter_chk(struct skin* filelistpath)
{
	debug(50, "start");
	if(rarFS == 1 && rarMountDirSplit != NULL)
	{
		debug(50, "check rarFS=1");
		debug(50, "lastpath=%s", filelistpath->text);
		debug(50, "rarMountDir=%s", rarMountDir);
		debug(50, "rarMountDirSplit=%s", rarMountDirSplit);
		if(strcmp(rarMountDirSplit, filelistpath->text) == 0)
			mc_mounter_umount();
	}
	else if(imgFS == 1 && imgMountDirSplit != NULL)
	{
		debug(50, "check imgFS=1");
		debug(50, "lastpath=%s", filelistpath->text);
		debug(50, "imgMountDir=%s", imgMountDir);
		debug(50, "imgMountDirSplit=%s", imgMountDirSplit);
		if(strcmp(imgMountDirSplit, filelistpath->text) == 0)
			mc_mounter_umount();
	}
	else if(isoFS == 1 && isoMountDirSplit != NULL)
	{
		debug(50, "check isoFS=1");
		debug(50, "lastpath=%s", filelistpath->text);
		debug(50, "isoMountDir=%s", isoMountDir);
		debug(50, "isoMountDirSplit=%s", isoMountDirSplit);
		if(strcmp(isoMountDirSplit, filelistpath->text) == 0)
			mc_mounter_umount();
	}

	return 0;
}

#endif