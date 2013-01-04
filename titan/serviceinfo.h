#ifndef SERVICEINFO_H
#define SERVICEINFO_H

void clearserviceinfo(struct skin* l1, struct skin* l2, struct skin* l3, struct skin* l4, struct skin* l5, struct skin* l6, struct skin* l7, struct skin* l8, struct skin* l9, struct skin* l10, struct skin* l11, struct skin* l12, struct skin* l13, struct skin* l14, struct skin* l15, struct skin* l16, struct skin* l17, struct skin* l18)
{
	changetext(l1, NULL);
	changetext(l2, NULL);
	changetext(l3, NULL);
	changetext(l4, NULL);
	changetext(l5, NULL);
	changetext(l6, NULL);
	changetext(l7, NULL);
	changetext(l8, NULL);
	changetext(l9, NULL);
	changetext(l10, NULL);
	changetext(l11, NULL);
	changetext(l12, NULL);
	changetext(l13, NULL);
	changetext(l14, NULL);
	changetext(l15, NULL);
	changetext(l16, NULL);
	changetext(l17, NULL);
	changetext(l18, NULL);
}

void fillservice(struct skin* l1, struct skin* l2, struct skin* l3, struct skin* l4, struct skin* l5, struct skin* l6)
{
	char* tmpstr = NULL;
	unsigned long tmpnr = 0;
	
	if(status.aktservice != NULL)
	{
		if(status.aktservice->channel != NULL)
		{
			changetext(l1, status.aktservice->channel->name);
			if(status.aktservice->channel->provider != NULL)
				changetext(l2, status.aktservice->channel->provider->name);
		}
	}
	tmpstr = getaspect();
	changetext(l3, _(tmpstr));
	free(tmpstr); tmpstr = NULL;
	tmpstr = getvideomode();
	changetext(l4, _(tmpstr));
	free(tmpstr); tmpstr = NULL;

	tmpstr = getdevcontent("vmpegxresdev");
	if(tmpstr != NULL)
	{
		tmpnr = strtol(tmpstr, 0, 16);
		free(tmpstr); tmpstr = NULL;
		tmpstr = olutoa(tmpnr);
		changetext(l5, tmpstr);
	}
	free(tmpstr); tmpstr = NULL;

	tmpstr = getdevcontent("vmpegyresdev");
	if(tmpstr != NULL)
	{
		tmpnr = strtol(tmpstr, 0, 16);
		free(tmpstr); tmpstr = NULL;
		tmpstr = olutoa(tmpnr);
		changetext(l6, tmpstr);
	}
	free(tmpstr); tmpstr = NULL;
}

void hiddeservice(struct skin* name, struct skin* provider, struct skin* aspectratio, struct skin* resolution, struct skin* xres, struct skin* yres, int hidden)
{
	name->hidden = hidden;
	provider->hidden = hidden;
	aspectratio->hidden = hidden;
	resolution->hidden = hidden;
	xres->hidden = hidden;
	yres->hidden = hidden;
}

void fillpids(struct skin* l1, struct skin* l2, struct skin* l3, struct skin* l4, struct skin* l5, struct skin* l6, struct skin* l7, struct skin* l8, struct skin* l9)
{
	char* tmpnr = NULL;

	if(status.aktservice != NULL)
	{
		if(status.aktservice->channel != NULL)
		{
			tmpnr = oitoa(status.aktservice->channel->videopid);
			changetext(l1, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa(status.aktservice->channel->audiopid);
			changetext(l2, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa(status.aktservice->channel->pcrpid);
			changetext(l3, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa(status.aktservice->channel->pmtpid);
			changetext(l4, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa(status.aktservice->channel->txtpid);
			changetext(l5, tmpnr);
			free(tmpnr); tmpnr = NULL;

			tmpnr = oitoa(status.aktservice->channel->transponderid & 0xffff);
			changetext(l6, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa((status.aktservice->channel->transponderid >> 16) & 0xffff);
			changetext(l7, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa(status.aktservice->channel->serviceid);
			changetext(l8, tmpnr);
			free(tmpnr); tmpnr = NULL;
			tmpnr = ollutoa(status.aktservice->channel->transponderid);
			changetext(l9, tmpnr);
			free(tmpnr); tmpnr = NULL;
		}
	}
}


void hiddepids(struct skin* videopid, struct skin* audiopid, struct skin* pcrpid, struct skin* pmtpid, struct skin* txtpid, struct skin* tsid, struct skin* onid, struct skin* sid, struct skin* id, int hidden)
{
	videopid->hidden = hidden;
	audiopid->hidden = hidden;
	pcrpid->hidden = hidden;
	pmtpid->hidden = hidden;
	txtpid->hidden = hidden;
	tsid->hidden = hidden;
	onid->hidden = hidden;
	sid->hidden = hidden;
	id->hidden = hidden;
}

int filltuner(struct skin* l1, struct skin* l2, struct skin* l3, struct skin* l4, struct skin* l5, struct skin* l6, struct skin* l7, struct skin* l8, struct skin* l9, struct skin* l10, struct skin* l11, struct skin* l12, struct skin* l13, struct skin* l14, struct skin* l15, struct skin* l16, struct skin* l17, struct skin* l18)
{
	char* tmpnr = NULL;
	char* tmpstr = NULL;

	if(status.aktservice != NULL)
	{
		if(status.aktservice->fedev != NULL && status.aktservice->transponder != NULL)
		{
			tmpnr = oitoa(status.aktservice->fedev->adapter);
			tmpstr = ostrcat(tmpnr, "/", 1, 0);
			tmpnr = oitoa(status.aktservice->fedev->devnr);
			tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
			changetext(l1, tmpstr);
			free(tmpstr); tmpstr = NULL;

			tmpstr = fegettypestr(status.aktservice->fedev);
			changetext(l2, tmpstr);
			free(tmpstr); tmpstr = NULL;

			if(status.aktservice->transponder->fetype == FE_QPSK)
			{
				tmpstr = transpondergetsystemstr(status.aktservice->transponder, 0);
				changetext(l3, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}

			tmpstr = transpondergetmodulationstr(status.aktservice->transponder, -1, 0);
			changetext(l4, tmpstr);
			free(tmpstr); tmpstr = NULL;

			tmpnr = oitoa(status.aktservice->transponder->orbitalpos);
			changetext(l5, tmpnr);
			free(tmpnr); tmpnr = NULL;

			tmpnr = oitoa(status.aktservice->transponder->frequency);
			changetext(l6, tmpnr);
			free(tmpnr); tmpnr = NULL;

			if(status.aktservice->transponder->fetype == FE_QPSK || status.aktservice->transponder->fetype == FE_QAM)
			{
				tmpnr = oitoa(status.aktservice->transponder->symbolrate);
				changetext(l7, tmpnr);
				free(tmpnr); tmpnr = NULL;
			}

			if(status.aktservice->transponder->fetype == FE_QPSK)
			{
				tmpstr = transpondergetpolarizationstr(status.aktservice->transponder, 0);
				changetext(l8, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}

			if(status.aktservice->transponder->fetype == FE_QPSK || status.aktservice->transponder->fetype == FE_QAM)
			{
				tmpstr = transpondergetfecstr(status.aktservice->transponder, -1, 0);
				changetext(l9, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}

			if(status.aktservice->transponder->fetype == FE_QPSK && status.aktservice->transponder->system == 1)
			{
				tmpstr = transpondergetinversionstr(status.aktservice->transponder, 0);
				changetext(l10, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergetrolloffstr(status.aktservice->transponder, 0);
				changetext(l11, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergetpilotstr(status.aktservice->transponder, 0);
				changetext(l12, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}

			if(status.aktservice->transponder->fetype == FE_OFDM)
			{
				tmpstr = transpondergetfecstr(status.aktservice->transponder, -1, 0);
				changetext(l13, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergetfecstr(status.aktservice->transponder, -1, 3);
				changetext(l14, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergetbandwidthstr(status.aktservice->transponder, 0);
				changetext(l15, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergettransmissionstr(status.aktservice->transponder, 0);
				changetext(l16, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergetguardintervalstr(status.aktservice->transponder, 0);
				changetext(l17, tmpstr);
				free(tmpstr); tmpstr = NULL;

				tmpstr = transpondergethierarchystr(status.aktservice->transponder, 0);
				changetext(l18, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
		}
	}

	return 0;
}

void hiddetuner(struct skin* tuner, struct skin* type, struct skin* system, struct skin* modulation, struct skin* orbitalpos, struct skin* frequency, struct skin* symbolrate, struct skin* polarization, struct skin* inversion, struct skin* fec, struct skin* rolloff, struct skin* pilot, struct skin* hp, struct skin* lp, struct skin* bandwidth, struct skin* transmission, struct skin* guardinterval, struct skin* hierarchy, int hidden)
{
	tuner->hidden = hidden;
	type->hidden = hidden;
	system->hidden = hidden;
	modulation->hidden = hidden;
	orbitalpos->hidden = hidden;
	frequency->hidden = hidden;
	symbolrate->hidden = hidden;
	polarization->hidden = hidden;
	fec->hidden = hidden;
	inversion->hidden = hidden;
	rolloff->hidden = hidden;
	pilot->hidden = hidden;
	hp->hidden = hidden;
	lp->hidden = hidden;
	bandwidth->hidden = hidden;
	transmission->hidden = hidden;
	guardinterval->hidden = hidden;
	hierarchy->hidden = hidden;

	if(status.aktservice != NULL && status.aktservice->transponder != NULL)
	{
		if(status.aktservice->transponder->fetype != FE_QPSK) system->hidden = YES;
		if(status.aktservice->transponder->fetype != FE_QPSK && status.aktservice->transponder->fetype != FE_QAM) symbolrate->hidden = YES;
		if(status.aktservice->transponder->fetype != FE_QPSK) polarization->hidden = YES;
		if(status.aktservice->transponder->fetype != FE_QPSK && status.aktservice->transponder->fetype != FE_QAM) fec->hidden = YES;
		if(status.aktservice->transponder->fetype != FE_QPSK || (status.aktservice->transponder->fetype == FE_QPSK && status.aktservice->transponder->system == 0))
		{
			inversion->hidden = YES;
			rolloff->hidden = YES;
			pilot->hidden = YES;
		}
		if(status.aktservice->transponder->fetype != FE_OFDM)
		{
			hp->hidden = YES;
			lp->hidden = YES;
			bandwidth->hidden = YES;
			transmission->hidden = YES;
			guardinterval->hidden = YES;
			hierarchy->hidden = YES;
		}
	}
}

int screenserviceinfo(void)
{
	int rcret = 0;
	struct skin* serviceinfo = getscreen("serviceinfo");
	struct skin* name = getscreennode(serviceinfo, "name");
	struct skin* provider = getscreennode(serviceinfo, "provider");
	struct skin* aspectratio = getscreennode(serviceinfo, "aspectratio");
	struct skin* resolution = getscreennode(serviceinfo, "resolution");
	struct skin* xres = getscreennode(serviceinfo, "xres");
	struct skin* yres = getscreennode(serviceinfo, "yres");

	struct skin* videopid = getscreennode(serviceinfo, "videopid");
	struct skin* audiopid = getscreennode(serviceinfo, "audiopid");
	struct skin* pcrpid = getscreennode(serviceinfo, "pcrpid");
	struct skin* pmtpid = getscreennode(serviceinfo, "pmtpid");
	struct skin* txtpid = getscreennode(serviceinfo, "txtpid");
	struct skin* tsid = getscreennode(serviceinfo, "tsid");
	struct skin* onid = getscreennode(serviceinfo, "onid");
	struct skin* sid = getscreennode(serviceinfo, "sid");
	struct skin* id = getscreennode(serviceinfo, "id");

	struct skin* tuner = getscreennode(serviceinfo, "tuner");
	struct skin* type = getscreennode(serviceinfo, "type");
	struct skin* system = getscreennode(serviceinfo, "system");
	struct skin* modulation = getscreennode(serviceinfo, "modulation");
	struct skin* orbitalpos = getscreennode(serviceinfo, "orbitalpos");
	struct skin* frequency = getscreennode(serviceinfo, "frequency");
	struct skin* symbolrate = getscreennode(serviceinfo, "symbolrate");
	struct skin* polarization = getscreennode(serviceinfo, "polarization");
	struct skin* inversion = getscreennode(serviceinfo, "inversion");
	struct skin* fec = getscreennode(serviceinfo, "fec");
	struct skin* rolloff = getscreennode(serviceinfo, "rolloff");
	struct skin* pilot = getscreennode(serviceinfo, "pilot");
	struct skin* hp = getscreennode(serviceinfo, "hp");
	struct skin* lp = getscreennode(serviceinfo, "lp");
	struct skin* bandwidth = getscreennode(serviceinfo, "bandwidth");
	struct skin* transmission = getscreennode(serviceinfo, "transmission");
	struct skin* guardinterval = getscreennode(serviceinfo, "guardinterval");
	struct skin* hierarchy = getscreennode(serviceinfo, "hierarchy");

	struct skin* l1 = getscreennode(serviceinfo, "l1");
	struct skin* l2 = getscreennode(serviceinfo, "l2");
	struct skin* l3 = getscreennode(serviceinfo, "l3");
	struct skin* l4 = getscreennode(serviceinfo, "l4");
	struct skin* l5 = getscreennode(serviceinfo, "l5");
	struct skin* l6 = getscreennode(serviceinfo, "l6");
	struct skin* l7 = getscreennode(serviceinfo, "l7");
	struct skin* l8 = getscreennode(serviceinfo, "l8");
	struct skin* l9 = getscreennode(serviceinfo, "l9");
	struct skin* l10 = getscreennode(serviceinfo, "l10");
	struct skin* l11 = getscreennode(serviceinfo, "l11");
	struct skin* l12 = getscreennode(serviceinfo, "l12");
	struct skin* l13 = getscreennode(serviceinfo, "l13");
	struct skin* l14 = getscreennode(serviceinfo, "l14");
	struct skin* l15 = getscreennode(serviceinfo, "l15");
	struct skin* l16 = getscreennode(serviceinfo, "l16");
	struct skin* l17 = getscreennode(serviceinfo, "l17");
	struct skin* l18 = getscreennode(serviceinfo, "l18");

	clearserviceinfo(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18);
	fillservice(l1, l2, l3, l4, l5, l6);
	hiddeservice(name, provider, aspectratio, resolution, xres, yres, NO);
	hiddepids(videopid, audiopid, pcrpid, pmtpid, txtpid, tsid, onid, sid, id, YES);
	hiddetuner(tuner, type, system, modulation, orbitalpos, frequency, symbolrate, polarization, inversion, fec, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, YES);

	drawscreen(serviceinfo, 0, 0);

	while(1)
	{
		rcret = waitrc(serviceinfo, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			clearserviceinfo(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18);
			fillservice(l1, l2, l3, l4, l5, l6);
			hiddeservice(name, provider, aspectratio, resolution, xres, yres, NO);
			hiddepids(videopid, audiopid, pcrpid, pmtpid, txtpid, tsid, onid, sid, id, YES);
			hiddetuner(tuner, type, system, modulation, orbitalpos, frequency, symbolrate, polarization, inversion, fec, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, YES);
			drawscreen(serviceinfo, 0, 0);
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearserviceinfo(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18);
			fillpids(l1, l2, l3, l4, l5, l6, l7, l8, l9);
			hiddeservice(name, provider, aspectratio, resolution, xres, yres, YES);
			hiddepids(videopid, audiopid, pcrpid, pmtpid, txtpid, tsid, onid, sid, id, NO);
			hiddetuner(tuner, type, system, modulation, orbitalpos, frequency, symbolrate, polarization, inversion, fec, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, YES);
			drawscreen(serviceinfo, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			clearserviceinfo(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18);
			filltuner(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16, l17, l18);
			hiddeservice(name, provider, aspectratio, resolution, xres, yres, YES);
			hiddepids(videopid, audiopid, pcrpid, pmtpid, txtpid, tsid, onid, sid, id, YES);
			hiddetuner(tuner, type, system, modulation, orbitalpos, frequency, symbolrate, polarization, inversion, fec, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, NO);
			drawscreen(serviceinfo, 0, 0);
		}
	}

	delownerrc(serviceinfo);
	clearscreen(serviceinfo);
	return 0;
}

#endif
