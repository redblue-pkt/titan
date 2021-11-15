#ifndef OCRYPT_H
#define OCRYPT_H

struct ocrypt
{
	unsigned short ax, bx, cx, dx, si, tmp, x1a2, x1a0[16], res, i, inter, cfc, cfd, compte;
	unsigned char cle[33];
	short c;
	int c1, count;
	short d, e;
} oc;

int occode()
{
	oc.dx = oc.x1a2 + oc.i;
	oc.ax = oc.x1a0[oc.i];
	oc.cx = 0x015a;
	oc.bx = 0x4e35;

	oc.tmp = oc.ax;
	oc.ax = oc.si;
	oc.si = oc.tmp;

	oc.tmp = oc.ax;
	oc.ax = oc.dx;
	oc.dx = oc.tmp;

	if(oc.ax != 0)
		oc.ax = oc.ax * oc.bx;

	oc.tmp = oc.ax;
	oc.ax = oc.cx;
	oc.cx = oc.tmp;

	if(oc.ax != 0)
	{
		oc.ax = oc.ax * oc.si;
		oc.cx = oc.ax + oc.cx;
	}

	oc.tmp = oc.ax;
	oc.ax = oc.si;
	oc.si = oc.tmp;
	oc.ax = oc.ax * oc.bx;
	oc.dx = oc.cx + oc.dx;

	oc.ax = oc.ax + 1;

	oc.x1a2 = oc.dx;
	oc.x1a0[oc.i] = oc.ax;

	oc.res = oc.ax ^ oc.dx;
	oc.i = oc.i + 1;

	return 0;
}

int ocassemble()
{
	oc.x1a0[0] = (oc.cle[0] * 256) + oc.cle[1];
	occode();
	oc.inter = oc.res;

	oc.x1a0[1] = oc.x1a0[0] ^ ((oc.cle[2] * 256) + oc.cle[3]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[2] = oc.x1a0[1] ^ ((oc.cle[4] * 256) + oc.cle[5]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[3] = oc.x1a0[2] ^ ((oc.cle[6] * 256) + oc.cle[7]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[4] = oc.x1a0[3] ^ ((oc.cle[8] * 256) + oc.cle[9]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[5] = oc.x1a0[4] ^ ((oc.cle[10] * 256) + oc.cle[11]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[6] = oc.x1a0[5] ^ ((oc.cle[12] * 256) + oc.cle[13]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[7] = oc.x1a0[6] ^ ((oc.cle[14] * 256) + oc.cle[15]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[8] = oc.x1a0[7] ^ ((oc.cle[16] * 256) + oc.cle[17]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[9] = oc.x1a0[8] ^ ((oc.cle[18] * 256) + oc.cle[19]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[10] = oc.x1a0[9] ^ ((oc.cle[20] * 256) + oc.cle[21]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[11] = oc.x1a0[10] ^ ((oc.cle[22] * 256) + oc.cle[23]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[12] = oc.x1a0[11] ^ ((oc.cle[24] * 256) + oc.cle[25]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[13] = oc.x1a0[12] ^ ((oc.cle[26] * 256) + oc.cle[27]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[14] = oc.x1a0[13] ^ ((oc.cle[28] * 256) + oc.cle[29]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.x1a0[15] = oc.x1a0[14] ^ ((oc.cle[30] * 256) + oc.cle[31]);
	occode();
	oc.inter = oc.inter ^ oc.res;

	oc.i = 0;

	return 0;
}

int ocfin()
{
	for(oc.compte = 0; oc.compte <= 31; oc.compte++)
		oc.cle[oc.compte] = 0;

	oc.ax = 0; oc.bx = 0; oc.cx = 0; oc.dx = 0;
	oc.si = 0; oc.tmp = 0; oc.x1a2 = 0;
	oc.x1a0[0] = 0; oc.x1a0[1] = 0; oc.x1a0[2] = 0;
	oc.x1a0[3] = 0; oc.x1a0[4] = 0;
	oc.res = 0; oc.i = 0; oc.inter = 0; oc.cfc = 0;
	oc.cfd = 0; oc.compte = 0; oc.c = 0;

	return 0;
}

unsigned char* oencrypt(char* pw, char* buf, int len)
{
	unsigned long long pos = 0;
	unsigned char* outbuf = NULL;

	if(pw == NULL || buf == NULL) return NULL;

	outbuf = calloc(1, (len * 2) + 1);
	if(outbuf == NULL)
	{
		err("no mem");
		return NULL;
	}

	strcpy((char*)oc.cle, "abcdefghijklmnopqrstuvwxyz012345");
	strcpy((char*)oc.cle, pw);

	while(pos < len)
	{
		oc.c = buf[pos];

		ocassemble();
		oc.cfc = oc.inter >> 8;
		oc.cfd = oc.inter & 255;

		for(oc.compte = 0; oc.compte <= 31; oc.compte++)
			oc.cle[oc.compte] = oc.cle[oc.compte] ^ oc.c;
		oc.c = oc.c ^ (oc.cfc ^ oc.cfd);

		oc.d = (oc.c >> 4);
		oc.e = (oc.c & 15);

		outbuf[pos * 2] = 0x61 + oc.d;
		outbuf[pos * 2 + 1] = 0x61 + oc.e;
		pos++;
	}
	ocfin();

	return outbuf;
}

unsigned char* odecrypt(char* pw, char* buf, int len)
{
	unsigned long long pos = 0;
	unsigned char* outbuf = NULL;

	if(pw == NULL || buf == NULL) return NULL;

	outbuf = calloc(1, (len / 2) + 1);
	if(outbuf == NULL)
	{
		err("no mem");
		return NULL;
	}

	strcpy((char*)oc.cle, "abcdefghijklmnopqrstuvwxyz012345");
	strcpy((char*)oc.cle, pw);

	while(pos < len)
	{
		oc.d = buf[pos];
		oc.e = buf[pos + 1];

		oc.d = oc.d - 0x61;
		oc.d = oc.d << 4;

		oc.e = oc.e - 0x61;
		oc.c = oc.d + oc.e;

		ocassemble();
		oc.cfc = oc.inter >> 8;
		oc.cfd = oc.inter & 255;

		oc.c = oc.c ^ (oc.cfc ^ oc.cfd);

		for(oc.compte = 0; oc.compte <= 31; oc.compte++)
			oc.cle[oc.compte] = oc.cle[oc.compte] ^ oc.c;

		outbuf[pos / 2] = oc.c;
		pos = pos + 2;
	}
	ocfin();

	return outbuf;
}

#endif
