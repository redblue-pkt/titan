#ifndef PROMPTFILE_H
#define PROMPTFILE_H

char* promptfile(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmpstr = NULL, *post = NULL, *streamlink = NULL, *chashname = NULL, *chashvalue = NULL, *extra = NULL, *tmpstr2 = NULL, *tmplink = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/promptfile1_get");
	unlink("/tmp/promptfile2_post");
	unlink("/tmp/promptfile3_streamlink");

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}
	
	if(tmplink == NULL || ostrncmp("http", tmplink, 4))
	{
		textbox(_("Message"), _("Hoster Url not http:// or https://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/promptfile1_get", NULL, NULL, NULL, tmpstr);

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

// <input type="hidden" id="chash" name  =  "ch883c3b867d7baa1d9e0176d44e7af34de1281a93" value="b205ee9355067bbc71a3802324d3aa42df661450" />
// <button type="submit" class="gray_btn" onclick='$("#chash").val("bb"+$("#chash").val());//For Kodi devs: we are prepared to rotate our code every hour. So please stop including us in your addons.'>Continue to File</button>

	chashname = oregex("id=\"chash\" name.*\"(.*)\" value=.*", tmpstr);
	chashvalue = oregex("id=\"chash\" name.*value=\"(.*)\" />.*", tmpstr);
	extra = string_resub("onclick='$(\"#chash\").val(\"", "\"+$(\"#chash\").val", tmpstr, 0);
	debug(99, "chashname: %s", chashname);
	debug(99, "extra: %s", extra);
	debug(99, "chashvalue: %s", chashvalue);

	if(chashname != NULL)
	{
		post = ostrcat(chashname, "=", 0, 0);
		post = ostrcat(post, extra, 1, 0);
		post = ostrcat(post, chashvalue, 1, 0);
		free(chashname), chashname = NULL;
		free(extra), extra = NULL;
		free(chashvalue), chashvalue = NULL;
	
		debug(99, "post: %s", post);
	
		free(tmpstr), tmpstr = NULL;
		tmpstr = gethttps(tmplink, NULL, post, NULL, NULL, tmplink, 1);
		titheklog(debuglevel, "/tmp/promptfile2_post", NULL, NULL, NULL, tmpstr);
	}

// src:  "http://www.promptfile.com/file/eyJpIjoiRDFEM0M1MEZGNC00MTBDRTA2RDY5IiwiZSI6MTQ3NDAxNTU0OSwidCI6Im1vYmlsZSIsImgiOiJlZmE1ZmQ2NzBlMDJiOWQ4ZjEzMTBlZjg5NzVlYTBlNzUzZjU1YzViIiwicyI6MX0=" }
	tmpstr2 = string_resub("flowplayer(container", "native_fullscreen", tmpstr, 0);

	if(tmpstr2 != NULL)
		streamlink = oregex("src:.*\"(.*)\" }.*", tmpstr2);

	if(streamlink == NULL)
	{
// $f("player", getjspath()+"player.swf", {
		tmpstr2 = string_resub("function initPlayer", "autoBuffering", tmpstr, 0);
		streamlink = string_resub("url: '", "',", tmpstr2, 0);
	}
	titheklog(debuglevel, "/tmp/promptfile3_streamlink", NULL, NULL, NULL, streamlink);
	
end:

	free(tmpstr); tmpstr = NULL;
	free(tmpstr2); tmpstr2 = NULL;
	free(chashname), chashname = NULL;
	free(chashvalue), chashvalue = NULL;
	free(extra), extra = NULL;
	free(tmplink), tmplink = NULL;
	free(post), post = NULL;

	return streamlink;
}

#endif
