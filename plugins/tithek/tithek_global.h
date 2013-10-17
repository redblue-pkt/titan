#ifndef TITHEK_GLOBAL_H
#define TITHEK_GLOBAL

char* hoster(char* url)
{
	debug(99, "url: %s", url);
	char* streamurl = NULL, *tmplink = NULL;

/*
		if re.match(".*?http://www.putlocker.com/(file|embed)/", link, re.S):
		elif re.match(".*?http://www.sockshare.com/(file|embed)/", link, re.S):
		elif re.match(".*?http://www.videoslasher.com/embed/", link, re.S):
		elif re.match('.*?http://faststream.in', link, re.S):
		elif re.match('.*?http:/.*?flashx.tv', link, re.S):
		elif re.match('.*?http://streamcloud.eu', link, re.S):
		elif re.match('.*?http://vidstream.in', link, re.S):
		elif re.match('.*?http://xvidstage.com', link, re.S):
		elif re.match('.*?http://embed.nowvideo.eu', link, re.S):
		elif re.match('.*?.movshare.net', link, re.S):
		elif re.match('.*?(embed.divxstage.eu|divxstage.eu/video)', link, re.S):
		elif re.match('.*?videoweed.es', link, re.S):
		elif re.match('.*?novamov.com', link, re.S):
		elif re.match('.*primeshare', link, re.S):
		elif re.match('.*?videomega.tv', link, re.S):
		elif re.match('.*?bitshare.com', link, re.S):
		elif re.match('.*?http://movreel.com/', link, re.S):
		elif re.match('.*?uploadc.com', link, re.S):
		elif re.match('.*?http://filenuke.com', link, re.S):
		elif re.match('.*?http://www.youtube.com/watch', link, re.S):
		elif re.match('.*?http://www.mightyupload.com/embed', link, re.S):
		elif re.match('.*?180upload', link, re.S):
		elif re.match('.*?ecostream.tv', link, re.S):

	tmplink = ostrcat(url, NULL, 0, 0);

	if(ostrstr(tmpstr, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}


						if(ostrcmp(tmphname, "Sockshare") == 0)
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Putlocker") == 0)
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Filenuke") == 0)
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamclou") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamcloud") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "VidStream") == 0)
							hname = ostrcat("VidStream.in", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Flashx") == 0)
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Xvidstage") == 0)
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Movshare") == 0)
							hname = ostrcat("MovShare.net", NULL, 0, 0);
						else if(ostrcmp(tmphname, "MovReel") == 0)
							hname = ostrcat("MovReel.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "NovaMov") == 0)
							hname = ostrcat("NovaMov", NULL, 0, 0);
						else if(ostrcmp(tmphname, "DivXStage") == 0)
							hname = ostrcat("DivXStage", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else
						{
							hname = ostrcat(tmphname, " (coming soon)", 0, 0);
							type = 66;
						}												
*/

	tmplink = ostrcat(url, NULL, 0, 0);
	string_tolower(tmplink);

	if(ostrstr(tmplink, "sockshare") != NULL)
		streamurl = putlocker(url);
	else if(ostrstr(tmplink, "putlocker") != NULL)
		streamurl = putlocker(url);
	else if(ostrstr(tmplink, "filenuke") != NULL)
		streamurl = filenuke(url);
	else if(ostrstr(tmplink, "streamcloud") != NULL)
		streamurl = streamcloud(url);
	else if(ostrstr(tmplink, "vidstream") != NULL)
		streamurl = vidstream(url);
	else if(ostrstr(tmplink, "flashx") != NULL)
		streamurl = flashx(url);
	else if(ostrstr(tmplink, "xvidstage") != NULL)
		streamurl = xvidstage(url);
	else if(ostrstr(tmplink, "nowvideo") != NULL)
		streamurl = nowvideo(url);
	else if(ostrstr(tmplink, "movshare") != NULL)
		streamurl = movshare(url);
	else if(ostrstr(tmplink, "movreel") != NULL)
		streamurl = movreel(url);
	else if(ostrstr(tmplink, "novamov") != NULL)
		streamurl = novamov(url);
	else if(ostrstr(tmplink, "divxstage") != NULL)
		streamurl = divxstage(url);
	else if(ostrstr(tmplink, "primeshare") != NULL)
		streamurl = primeshare(url);
	else
		textbox(_("Message"), _("The hoster is not yet supported !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);


	debug(99, "streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "streamurl2: %s", streamurl);

	free(tmplink), tmplink = NULL;
	return streamurl;
}

#endif