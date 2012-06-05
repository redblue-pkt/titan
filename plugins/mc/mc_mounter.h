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
	debug(50, "[mc_mounter_umount] rarFS=%d imgFS=%d isoFS=%d", rarFS, imgFS, isoFS);

	char* cmd = NULL;

	if(isoFS == 1){
		cmd = ostrcat(cmd, "/bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3`", 1, 0);
		system(cmd);
		debug(50, "[mc_mounter_umount] cmd: %s", cmd);
		free(isoMountDir);
		isoMountDir = NULL;
		free(isoMountDirSplit);
		isoMountDirSplit = NULL;
		isoFS = 0;
	} else if(imgFS == 1){
		cmd = ostrcat(cmd, "/bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3`", 1, 0);
		system(cmd);
		debug(50, "[mc_mounter_umount] cmd: %s", cmd);
		free(imgMountDir);
		imgMountDir = NULL;
		free(imgMountDirSplit);
		imgMountDirSplit = NULL;
		imgFS = 0;
	} else if(rarFS == 1){
		cmd = ostrcat("/bin/umount -fl `mount | grep fuse | grep rarfs | grep -v '/dev/' | cut -d ' ' -f3` ", rarMountDir, 0, 0);
		cmd = ostrcat(cmd, " ; killall -9 rarfs", 1, 0);
		system(cmd);
		debug(50, "[mc_mounter_umount] cmd: %s", cmd);
		free(rarMountDir);
		rarMountDir = NULL;
		free(rarMountDirSplit);
		rarMountDirSplit = NULL;
		rarFS = 0;
	}
	free(cmd);
	cmd = NULL;

	return 0;
}


char* mc_mounter_main(int mode, char* tmpstr, struct skin* filelistpath, struct skin* filelist, struct skin* mc_videoplayer, char* filemask, int tmpview, char* path)
{
	debug(50, "[mc_mounter_main] start");

	if(!strncmp(".rar",tmpstr+strlen(tmpstr)-4,4)){
		debug(50, "[mc_mounter_main] found rar file: %s", tmpstr);
		mkdir("/tmp/rarfs", 777);

		tmpmounter = ostrcat("/sbin/rarfs \"", tmpstr, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\" /tmp/rarfs", 1, 0);
		debug(50, "[mc_mounter_main] mount line1: %s", tmpmounter);
		system(tmpmounter);

		tmpmounter = ostrcat("/bin/mount --bind /tmp/rarfs \"", filelistpath->text, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\"", 1, 0);
		debug(50, "[mc_mounter_main] mount line2: %s", tmpmounter);

		free(tmpstr);
		tmpstr = NULL;
		system(tmpmounter);

		debug(50, "[mc_mounter_main] save mc_videoplayerpath: %s", filelistpath->text);
		addconfig("mc_videoplayerpath", filelistpath->text);
		addscreenrc(mc_videoplayer, filelist);
		getfilelist(mc_videoplayer, filelistpath, filelist, NULL, path, filemask, tmpview, NULL);

		rarMountDir = ostrcat(rarMountDir, filelistpath->text, 1, 0);
		debug(50, "[mc_mounter_main] save rarMountDir: %s", rarMountDir);

		rarMountDirSplit = ostrcat(rarMountDirSplit, filelistpath->text, 1, 0);
		dirname(rarMountDirSplit);
		debug(50, "[mc_mounter_main] save rarMountDirSplit: %s", rarMountDirSplit);

		rarFS = 1;
	} else if(!strncmp(".img",tmpstr+strlen(tmpstr)-4,4)){
		debug(50, "[mc_mounter_main] found img file: %s", tmpstr);
		mkdir("/tmp/imgfs", 777);
		tmpmounter = ostrcat("mount -o loop \"", tmpstr, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\" /tmp/imgfs", 1, 0); 

		debug(50, "[mc_mounter_main] mount line1: %s", tmpmounter);
		system(tmpmounter);

		tmpmounter = ostrcat("/bin/mount --bind /tmp/imgfs \"", filelistpath->text, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\"", 1, 0);
		debug(50, "[mc_mounter_main] mount line2: %s", tmpmounter);

		free(tmpstr);
		tmpstr = NULL;
		system(tmpmounter);

		debug(50, "[mc_mounter_main] save mc_videoplayerpath: %s", filelistpath->text);
		addconfig("mc_videoplayerpath", filelistpath->text);
		addscreenrc(mc_videoplayer, filelist);
		getfilelist(mc_videoplayer, filelistpath, filelist, NULL, path, filemask, tmpview, NULL);

		imgMountDir = ostrcat(imgMountDir, filelistpath->text, 1, 0);
		debug(50, "[mc_mounter_main] save imgMountDir: %s", imgMountDir);

		imgMountDirSplit = ostrcat(imgMountDirSplit, filelistpath->text, 1, 0);
		dirname(imgMountDirSplit);
		debug(50, "[mc_mounter_main] save imgMountDirSplit: %s", imgMountDirSplit);

		imgFS = 1;
	} else if(!strncmp(".iso",tmpstr+strlen(tmpstr)-4,4)){
		debug(50, "[mc_mounter_main] found iso file: %s", tmpstr);
		mkdir("/tmp/isofs", 777);

		tmpmounter = ostrcat("mount -o loop \"", tmpstr, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\" /tmp/isofs", 1, 0); 

		debug(50, "[mc_mounter_main] mount line1: %s", tmpmounter);
		system(tmpmounter);

		tmpmounter = ostrcat("/bin/mount --bind /tmp/isofs \"", filelistpath->text, 0, 0);
		tmpmounter = ostrcat(tmpmounter, "\"", 1, 0);
		debug(50, "[mc_mounter_main] mount line2: %s", tmpmounter);

		free(tmpstr);
		tmpstr = NULL;
		system(tmpmounter);

		debug(50, "[mc_mounter_main] save mc_videoplayerpath: %s", filelistpath->text);
		addconfig("mc_videoplayerpath", filelistpath->text);
		addscreenrc(mc_videoplayer, filelist);
		getfilelist(mc_videoplayer, filelistpath, filelist, NULL, path, filemask, tmpview, NULL);

		isoMountDir = ostrcat(imgMountDir, filelistpath->text, 1, 0);
		debug(50, "[mc_mounter_main] save isoMountDir: %s", isoMountDir);

		isoMountDirSplit = ostrcat(isoMountDirSplit, filelistpath->text, 1, 0);
		dirname(isoMountDirSplit);
		debug(50, "[mc_mounter_main] save isoMountDirSplit: %s", isoMountDirSplit);

		isoFS = 1;
	}
	debug(50, "[mc_mounter_main] end");
return 0;
}

char mc_mounter_chk(struct skin* filelistpath)
{
	debug(50, "[mc_mounter_chk] start");
	if((rarFS == 1) && (rarMountDirSplit != NULL)){
		debug(50, "[mc_mounter_chk] check rarFS=1");
		debug(50, "[mc_mounter_chk] lastpath=%s", filelistpath->text);
		debug(50, "[mc_mounter_chk] rarMountDir=%s", rarMountDir);
		debug(50, "[mc_mounter_chk] rarMountDirSplit=%s", rarMountDirSplit);
		if(strcmp(rarMountDirSplit, filelistpath->text) == 0){
			debug(50, "[mc_mounter_chk] mc_mounter_umount");
			mc_mounter_umount();

		}
	} else if((imgFS == 1) && (imgMountDirSplit != NULL)){
		debug(50, "[mc_mounter_chk] check imgFS=1");
		debug(50, "[mc_mounter_chk] lastpath=%s", filelistpath->text);
		debug(50, "[mc_mounter_chk] imgMountDir=%s", imgMountDir);
		debug(50, "[mc_mounter_chk] imgMountDirSplit=%s", imgMountDirSplit);
		if(strcmp(imgMountDirSplit, filelistpath->text) == 0){
			debug(50, "[mc_mounter_chk] mc_mounter_umount");
			mc_mounter_umount();

		}
	} else if((isoFS == 1) && (isoMountDirSplit != NULL)){
		debug(50, "[mc_mounter_chk] check isoFS=1");
		debug(50, "[mc_mounter_chk] lastpath=%s", filelistpath->text);
		debug(50, "[mc_mounter_chk] isoMountDir=%s", isoMountDir);
		debug(50, "[mc_mounter_chk] isoMountDirSplit=%s", isoMountDirSplit);
		if(strcmp(isoMountDirSplit, filelistpath->text) == 0){
			debug(50, "[mc_mounter_chk] mc_mounter_umount");
			mc_mounter_umount();

		}
	}
	debug(50, "[mc_mounter_chk] end");

return 0;
}

#endif
