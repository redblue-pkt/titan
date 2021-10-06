#ifndef CACC_H
#define CACC_H

#define xx_debug    0
#define yy_debug    0

#ifdef QUIET
char authie[7];
char devie[7];
#endif

static uint8_t NullPMT[50]={0x9F,0x80,0x32,0x2E,0x03,0x6E,0xA7,0x37,0x00,0x00,0x1B,0x15,0x7D,0x00,0x00,0x03,0x15,0x7E,0x00,0x00,0x03,0x15,0x7F,0x00,0x00,0x06,0x15,0x80,0x00,0x00,0x06,0x15,0x82,0x00,0x00,0x0B,0x08,0x7B,0x00,0x00,0x05,0x09,0x42,0x00,0x00,0x06,0x15,0x81,0x00,0x00};
#ifdef MIPSEL
static const char *descrambler_filename = "/dev/dvb/adapter0/ca0";
#else
static const char *descrambler_filename = "/dev/dvb/adapter0/ca3";
#endif
static int desc_fd = -1;
static int desc_user_count = 0;

static int testwork = 0;

unsigned char dh_p[256] = {       /* prime */
	0xd6, 0x27, 0x14, 0x7a, 0x7c, 0x0c, 0x26, 0x63, 0x9d, 0x82, 0xeb, 0x1f, 0x4a, 0x18, 0xff, 0x6c,
	0x34, 0xad, 0xea, 0xa6, 0xc0, 0x23, 0xe6, 0x65, 0xfc, 0x8e, 0x32, 0xc3, 0x33, 0xf4, 0x91, 0xa7,
	0xcc, 0x88, 0x58, 0xd7, 0xf3, 0xb3, 0x17, 0x5e, 0xb0, 0xa8, 0xeb, 0x5c, 0xd4, 0xd8, 0x3a, 0xae,
	0x8e, 0x75, 0xa1, 0x50, 0x5f, 0x5d, 0x67, 0xc5, 0x40, 0xf4, 0xb3, 0x68, 0x35, 0xd1, 0x3a, 0x4c,
	0x93, 0x7f, 0xca, 0xce, 0xdd, 0x83, 0x29, 0x01, 0xc8, 0x4b, 0x76, 0x81, 0x56, 0x34, 0x83, 0x31,
	0x92, 0x72, 0x65, 0x7b, 0xac, 0xd9, 0xda, 0xa9, 0xd1, 0xd3, 0xe5, 0x77, 0x58, 0x6f, 0x5b, 0x44,
	0x3e, 0xaf, 0x7f, 0x6d, 0xf5, 0xcf, 0x0a, 0x80, 0x0d, 0xa5, 0x56, 0x4f, 0x4b, 0x85, 0x41, 0x0f,
	0x13, 0x41, 0x06, 0x1f, 0xf3, 0xd9, 0x65, 0x36, 0xae, 0x47, 0x41, 0x1f, 0x1f, 0xe0, 0xde, 0x69,
	0xe5, 0x86, 0x2a, 0xa1, 0xf2, 0x48, 0x02, 0x92, 0x68, 0xa6, 0x37, 0x9f, 0x76, 0x4f, 0x7d, 0x94,
	0x5d, 0x10, 0xe5, 0xab, 0x5d, 0xb2, 0xf3, 0x12, 0x8c, 0x79, 0x03, 0x92, 0xa6, 0x7f, 0x8a, 0x78,
	0xb0, 0xba, 0xc5, 0xb5, 0x31, 0xc5, 0xc8, 0x22, 0x6e, 0x29, 0x02, 0x40, 0xab, 0xe7, 0x5c, 0x23,
	0x33, 0x7f, 0xcb, 0x86, 0xc7, 0xb4, 0xfd, 0xaa, 0x44, 0xcd, 0x9c, 0x9f, 0xba, 0xac, 0x3a, 0xcf,
	0x7e, 0x31, 0x5f, 0xa8, 0x47, 0xce, 0xca, 0x1c, 0xb4, 0x77, 0xa0, 0xec, 0x9a, 0x46, 0xd4, 0x79,
	0x7b, 0x64, 0xbb, 0x6c, 0x91, 0xb2, 0x38, 0x01, 0x65, 0x11, 0x45, 0x9f, 0x62, 0x08, 0x6f, 0x31,
	0xcf, 0xc4, 0xba, 0xdc, 0xd0, 0x03, 0x91, 0xf1, 0x18, 0x1f, 0xcb, 0x4d, 0xfc, 0x73, 0x5a, 0xa2,
	0x15, 0xb8, 0x3c, 0x8d, 0x80, 0x92, 0x1c, 0xa1, 0x03, 0xd0, 0x83, 0x2f, 0x5f, 0xe3, 0x07, 0x69
};

unsigned char dh_g[256] = {       /* generator */
	0x95, 0x7d, 0xd1, 0x49, 0x68, 0xc1, 0xa5, 0xf1, 0x48, 0xe6, 0x50, 0x4f, 0xa1, 0x10, 0x72, 0xc4,
	0xef, 0x12, 0xec, 0x2d, 0x94, 0xbe, 0xc7, 0x20, 0x2c, 0x94, 0xf9, 0x68, 0x67, 0x0e, 0x22, 0x17,
	0xb5, 0x5c, 0x0b, 0xca, 0xac, 0x9f, 0x25, 0x9c, 0xd2, 0xa6, 0x1a, 0x20, 0x10, 0x16, 0x6a, 0x42,
	0x27, 0x83, 0x47, 0x42, 0xa0, 0x07, 0x52, 0x09, 0x33, 0x97, 0x4e, 0x30, 0x57, 0xd8, 0xb7, 0x1e,
	0x46, 0xa6, 0xba, 0x4e, 0x40, 0x6a, 0xe9, 0x1a, 0x5a, 0xa0, 0x74, 0x56, 0x92, 0x55, 0xc2, 0xbd,
	0x44, 0xcd, 0xb3, 0x33, 0xf7, 0x35, 0x46, 0x25, 0xdf, 0x84, 0x19, 0xf3, 0xe2, 0x7a, 0xac, 0x4e,
	0xee, 0x1a, 0x86, 0x3b, 0xb3, 0x87, 0xa6, 0x66, 0xc1, 0x70, 0x21, 0x41, 0xd3, 0x58, 0x36, 0xb5,
	0x3b, 0x6e, 0xa1, 0x55, 0x60, 0x9a, 0x59, 0xd3, 0x85, 0xd8, 0xdc, 0x6a, 0xff, 0x41, 0xb6, 0xbf,
	0x42, 0xde, 0x64, 0x00, 0xd0, 0xee, 0x3a, 0xa1, 0x8a, 0xed, 0x12, 0xf9, 0xba, 0x54, 0x5c, 0xdb,
	0x06, 0x24, 0x49, 0xe8, 0x47, 0xcf, 0x5b, 0xe4, 0xbb, 0xc0, 0xaa, 0x8a, 0x8c, 0xbe, 0x73, 0xd9,
	0x02, 0xea, 0xee, 0x8d, 0x87, 0x5b, 0xbf, 0x78, 0x04, 0x41, 0x9e, 0xa8, 0x5c, 0x3c, 0x49, 0xde,
	0x88, 0x6d, 0x62, 0x21, 0x7f, 0xf0, 0x5e, 0x2d, 0x1d, 0xfc, 0x47, 0x0d, 0x1b, 0xaa, 0x4e, 0x0d,
	0x78, 0x20, 0xfe, 0x57, 0x0f, 0xca, 0xdf, 0xeb, 0x3c, 0x84, 0xa7, 0xe1, 0x61, 0xb2, 0x95, 0x98,
	0x07, 0x73, 0x8e, 0x51, 0xc6, 0x87, 0xe4, 0xcf, 0xf1, 0x5f, 0x86, 0x99, 0xec, 0x8d, 0x44, 0x92,
	0x2c, 0x99, 0xf6, 0xc0, 0xf4, 0x39, 0xe8, 0x05, 0xbf, 0xc1, 0x56, 0xde, 0xfe, 0x93, 0x75, 0x06,
	0x69, 0x87, 0x83, 0x06, 0x51, 0x80, 0xa5, 0x6e, 0xa6, 0x19, 0x7d, 0x3b, 0xef, 0xfb, 0xe0, 0x4a
};

/* Byte 0 to 15 are AES Key, Byte 16 to 31 are IV */

int descrambler_set_key(struct dvbdev* node, int index, int parity, unsigned char *data)
{
	debug(620, "start");
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	int rc = 0;
	struct ca_descr_data d;

#ifdef MIPSEL
	if(desc_fd == -1)
		descrambler_open();
	if (desc_fd > 0)
	{
		d.index = index;
		d.parity = parity;
		d.data_type = CA_DATA_KEY;
		d.length = 16;
		d.data = data;

		printf("[titan] DESCR_DATA: -> ");
		hexdump(data, 32);

		if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkchipset("BCM7424") == 1)
			rc = ioctl(desc_fd, CA_SET_DESCR_DATA_DREAM, &d);
		else
			rc = ioctl(desc_fd, CA_SET_DESCR_DATA, &d);
		
		if (rc)
			printf("[titan] #### ERROR: CA_DATA_KEY - CA_SET_DESCR_DATA index=0x%04x parity=0x%04x (errno=%d %s)\n", index, parity, errno, strerror(errno));
		else
			printf("[titan] #### CA_DATA_KEY - CA_SET_DESCR_DATA index=0x%04x parity=0x%04x\n", index, parity);
		
		d.index = index;
		d.parity = parity;
		d.data_type = CA_DATA_IV;
		d.length = 16;
		d.data = data + 16;

		if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
			rc = ioctl(desc_fd, CA_SET_DESCR_DATA_DREAM, &d);
		else
			rc = ioctl(desc_fd, CA_SET_DESCR_DATA, &d);
		
		if (rc)
			printf("[titan] #### ERROR: CA_DATA_IV - CA_SET_DESCR_DATA index=0x%04x parity=0x%04x (errno=%d %s)\n", index, parity, errno, strerror(errno));
		else
			printf("[titan] #### CA_DATA_IV - CA_SET_DESCR_DATA index=0x%04x parity=0x%04x\n", index, parity);	
		ciplusrun = 1;
	}
	/*if(testwork == 0)
	{
		if(checkbox("DM900") == 1)
				descrambler_deinit();
	}
	else
		testwork = 0;*/
	//descrambler_close(); 

#else
	
	index |= 0x100;
	
	if(desc_fd == -1)
		descrambler_open();
	if (desc_fd > 0)
	{
		d.index = index;
		d.parity = parity;
		d.data_type = CA_DATA_KEY;
		d.length = 32;
		d.data = data;

		printf("[titan] Index: %d Parity: (%d) -> ", d.index, d.parity);
		hexdump(d.data, 32);
		
		//if (ioctl(desc_fd, CA_SET_DESCR_DATA, &d))
		rc = ioctl(desc_fd, CA_SET_DESCR_DATA, &d);
		if (rc)
			printf("###############\nERROR - CA_SET_DESCR_DATA -> CA_DATA_KEY\n###############\n");
		else
			printf("###############\nCA_SET_DESCR_DATA -> CA_DATA_KEY RCode: %i\n###############\n", rc);

	}
	descrambler_close();
#endif

	return 0;
}

void resendKey(struct dvbdev* dvbnode)
{
	debug(620, "resendKey");
//	if (!tslot->SidBlackListed && (tslot->inUse || tslot->slot == cCA::GetInstance()->GetLiveSlot()))
//		descrambler_set_key((int)tslot->source, tslot->lastParity, tslot->lastKey);
//	if (!dvbnode->caslot->sidblacklisted && (dvbnode->caslot->inuse || dvbnode->caslot->slot == cCA::GetInstance()->GetLiveSlot()))
//		descrambler_set_key((int)dvbnode->caslot->source, dvbnode->caslot->lastParity, dvbnode->caslot->lastKey);

	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);
	descrambler_set_key(dvbnode, 0, cc_data->slot->lastParity, cc_data->slot->lastKey);
}

int aes_xcbc_mac_init(struct aes_xcbc_mac_ctx *ctx, const uint8_t *key)
{
	debug(620, "start");
	AES_KEY aes_key;
	int y, x;

	AES_set_encrypt_key(key, 128, &aes_key);

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 16; x++)
			ctx->K[y][x] = y + 1;
		AES_ecb_encrypt(ctx->K[y], ctx->K[y], &aes_key, 1);
	}

	/* setup K1 */
	AES_set_encrypt_key(ctx->K[0], 128, &ctx->key);

	memset(ctx->IV, 0, 16);
	ctx->buflen = 0;

	return 0;
}

int aes_xcbc_mac_process(struct aes_xcbc_mac_ctx *ctx, const uint8_t *in, unsigned int len)
{
	debug(620, "start");
	while (len) {
		if (ctx->buflen == 16) {
			AES_ecb_encrypt(ctx->IV, ctx->IV, &ctx->key, 1);
			ctx->buflen = 0;
		}
		ctx->IV[ctx->buflen++] ^= *in++;
		--len;
	}

	return 0;
}

int aes_xcbc_mac_done(struct aes_xcbc_mac_ctx *ctx, uint8_t *out)
{
	debug(620, "start");
	int i;

	if (ctx->buflen == 16) {
		/* K2 */
		for (i = 0; i < 16; i++)
			ctx->IV[i] ^= ctx->K[1][i];
	} else {
		ctx->IV[ctx->buflen] ^= 0x80;
		/* K3 */
		for (i = 0; i < 16; i++)
			ctx->IV[i] ^= ctx->K[2][i];
	}

	AES_ecb_encrypt(ctx->IV, ctx->IV, &ctx->key, 1);
	memcpy(out, ctx->IV, 16);

	return 0;
}

void descrambler_close(void)
{
	debug(620, "descrambler_close");
	close(desc_fd);
	desc_fd = -1;
}

void hexdump(const uint8_t *data, unsigned int len)
{
	while (len--)
		printf("%02x ", *data++);
	printf("\n");
// move to end is better log
	debug(620, "start");
}

#ifdef MIPSEL
int descrambler_set_pid(int index, int enable, int pid)
{
	struct ca_pid p;
	unsigned int flags = 0x80;

	if(desc_fd == -1)
		descrambler_open();
	if (desc_fd > 0)
	{
		/*if(checkbox("DM900") == 1)
			cainit(desc_fd);*/  // GOst test 
		if (index)
			flags |= 0x40;

		if (enable)
			flags |= 0x20;

		p.pid = pid;
		p.index = flags;
		
		if (ioctl(desc_fd, CA_SET_PID, &p) == -1)
		{
			printf("[titan] **** ERROR: CA_SET_PID pid=0x%04x index=0x%04x (errno=%d %s)\n", p.pid, p.index, errno, strerror(errno));
		}
		else
		{
			printf("[titan] **** CA_SET_PID pid=0x%04x index=0x%04x\n", p.pid, p.index);
		}
		testwork = 1;
		//descrambler_close();
	}
	return 0;
}
#else
/* we don't use this for ci cam ! */
/*
int descrambler_set_pid(int index, int enable, int pid)
{
	debug(620, "start");
	struct ca_pid p;
	if (enable)
		p.index = index;
	else
		p.index = -1;

	p.pid = pid;

	if (ioctl(desc_fd, CA_SET_PID, &p))
		printf("CA_SET_PID\n");

	return 0;
}
*/
#endif

int descrambler_open(void)
{
	debug(620, "descrambler_open");
	if(desc_fd == -1)
	{
		desc_fd = open(descrambler_filename, O_RDWR | O_NONBLOCK );
		debug(620, "descrambler_filename: %s desc_fd: %d", descrambler_filename, desc_fd);
	}
	if (desc_fd <= 0) {
		printf("cannot open %s\n", descrambler_filename);
		return 0;
	}
	//if (ioctl(desc_fd, CA_RESET, NULL))
	//	printf("**** ERROR: CA_RESET (errno=%d %s)\n", errno, strerror(errno));

	return 1;
}

int descrambler_init(void)
{
	debug(620, "descrambler_init");
	desc_user_count++;
	debug(620, "desc_user_count: %d", desc_user_count);

	return 0;
}

void descrambler_deinit(void)
{
	debug(620, "descrambler_deinit");
	desc_user_count--;
	if (desc_user_count <= 0 && desc_fd > 0)
	{
		cainit(desc_fd);
		descrambler_close();
		desc_user_count = 0;
	}
}

static int pkcs_1_mgf1(const uint8_t *seed, unsigned long seedlen, uint8_t *mask, unsigned long masklen)
{
	debug(620, "start");
	unsigned long hLen, x;
	uint32_t counter;
	uint8_t *buf;

	/* get hash output size */
	hLen = 20;      /* SHA1 */

	/* allocate memory */
	buf = (uint8_t*)malloc(hLen);
	if (buf == NULL) {
		printf("error mem\n");
		return -1;
	}

	/* start counter */
	counter = 0;

	while (masklen > 0) {
		/* handle counter */
		BYTE32(buf, counter);
		++counter;

		/* get hash of seed || counter */
		unsigned char buffer[0x18];
		memcpy(buffer, seed, seedlen);
		memcpy(buffer + 0x14, buf, 4);
		SHA1(buffer, 0x18, buf);

		/* store it */
		for (x = 0; x < hLen && masklen > 0; x++, masklen--)
			*mask++ = buf[x];
	}

	free(buf);

	return 0;
}

static int pkcs_1_pss_encode(const uint8_t *msghash, unsigned int msghashlen,
			     unsigned long saltlen, unsigned long modulus_bitlen,
			     uint8_t *out, unsigned int outlen)
{
	debug(620, "start");
	unsigned char *DB, *mask, *salt, *hash;
	unsigned long x, y, hLen, modulus_len;
	int err = -1;
	unsigned char *hashbuf;
	unsigned int hashbuflen;

	hLen = 20;      /* SHA1 */
	modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

	/* allocate ram for DB/mask/salt/hash of size modulus_len */
	DB = (unsigned char*)malloc(modulus_len);
	mask = (unsigned char*)malloc(modulus_len);
	salt = (unsigned char*)malloc(modulus_len);
	hash = (unsigned char*)malloc(modulus_len);

	hashbuflen = 8 + msghashlen + saltlen;
	hashbuf = (unsigned char*)malloc(hashbuflen);

	if (!(DB && mask && salt && hash && hashbuf)) {
		printf("out of memory\n");
		goto LBL_ERR;
	}

	/* generate random salt */
	if (saltlen > 0) {
		if (get_random(salt, saltlen) != (long)saltlen) {
			printf("rnd failed\n");
			goto LBL_ERR;
		}
	}

	/* M = (eight) 0x00 || msghash || salt, hash = H(M) */
	memset(hashbuf, 0, 8);
	memcpy(hashbuf + 8, msghash, msghashlen);
	memcpy(hashbuf + 8 + msghashlen, salt, saltlen);
	SHA1(hashbuf, hashbuflen, hash);

	/* generate DB = PS || 0x01 || salt, PS == modulus_len - saltlen - hLen - 2 zero bytes */
	x = 0;
	memset(DB + x, 0, modulus_len - saltlen - hLen - 2);
	x += modulus_len - saltlen - hLen - 2;
	DB[x++] = 0x01;
	memcpy(DB + x, salt, saltlen);
	x += saltlen;

	err = pkcs_1_mgf1(hash, hLen, mask, modulus_len - hLen - 1);
	if (err)
		goto LBL_ERR;

	/* xor against DB */
	for (y = 0; y < (modulus_len - hLen - 1); y++)
		DB[y] ^= mask[y];

	/* output is DB || hash || 0xBC */
	if (outlen < modulus_len) {
		err = -1;
		printf("error overflow\n");
		goto LBL_ERR;
	}

	/* DB len = modulus_len - hLen - 1 */
	y = 0;
	memcpy(out + y, DB, modulus_len - hLen - 1);
	y += modulus_len - hLen - 1;

	/* hash */
	memcpy(out + y, hash, hLen);
	y += hLen;

	/* 0xBC */
	out[y] = 0xBC;

	/* now clear the 8*modulus_len - modulus_bitlen most significant bits */
	out[0] &= 0xFF >> ((modulus_len << 3) - (modulus_bitlen - 1));

	err = 0;
LBL_ERR:
	free(hashbuf);
	free(hash);
	free(salt);
	free(mask);
	free(DB);

	return err;
}

/* DH */

int dh_gen_exp(uint8_t *dest, int dest_len, uint8_t *dh_g, int dh_g_len, uint8_t *dh_p, int dh_p_len)
{
	debug(620, "start");
#ifdef OEBUILD
// source from https://github.com/catalinii/minisatip/blob/master/src/ca.c
    DH *dh;
    BIGNUM *p, *g;
    const BIGNUM *priv_key;
    int len;
    unsigned int gap;

    dh = DH_new();

    p = BN_bin2bn(dh_p, dh_p_len, 0);
    g = BN_bin2bn(dh_g, dh_g_len, 0);
    DH_set0_pqg(dh, p, NULL, g);
    DH_set_flags(dh, DH_FLAG_NO_EXP_CONSTTIME);

    DH_generate_key(dh);

    DH_get0_key(dh, NULL, &priv_key);
    len = BN_num_bytes(priv_key);
    if (len > dest_len) {
        printf("len > dest_len\n");
        return -1;
    }

    gap = dest_len - len;
    memset(dest, 0, gap);
    BN_bn2bin(priv_key, &dest[gap]);

    DH_free(dh);

    return 0;
#else
	DH *dh;
	int len;
	unsigned int gap;

	dh = DH_new();

	dh->p = BN_bin2bn(dh_p, dh_p_len, 0);
	dh->g = BN_bin2bn(dh_g, dh_g_len, 0);
	dh->flags |= DH_FLAG_NO_EXP_CONSTTIME;

	DH_generate_key(dh);

	len = BN_num_bytes(dh->priv_key);
	if (len > dest_len) {
		printf("len > dest_len\n");
		return -1;
	}

	gap = dest_len - len;
	memset(dest, 0, gap);
	BN_bn2bin(dh->priv_key, &dest[gap]);

	DH_free(dh);

	return 0;
#endif
}

/* dest = base ^ exp % mod */
int dh_mod_exp(uint8_t *dest, int dest_len, uint8_t *base, int base_len, uint8_t *mod, int mod_len, uint8_t *exp, int exp_len)
{
	debug(620, "start");

	BIGNUM *bn_dest, *bn_base, *bn_exp, *bn_mod;
	BN_CTX *ctx;
	int len;
	unsigned int gap;

	bn_base = BN_bin2bn(base, base_len, NULL);
	bn_exp = BN_bin2bn(exp, exp_len, NULL);
	bn_mod = BN_bin2bn(mod, mod_len, NULL);
	ctx = BN_CTX_new();

	bn_dest = BN_new();
	BN_mod_exp(bn_dest, bn_base, bn_exp, bn_mod, ctx);
	BN_CTX_free(ctx);

	len = BN_num_bytes(bn_dest);
	if (len > dest_len) {
		printf("len > dest_len\n");
		return -1;
	}

	gap = dest_len - len;
	memset(dest, 0, gap);
	BN_bn2bin(bn_dest, &dest[gap]);

	BN_free(bn_dest);
	BN_free(bn_mod);
	BN_free(bn_exp);
	BN_free(bn_base);

	return 0;
}

int dh_dhph_signature(uint8_t *out, uint8_t *nonce, uint8_t *dhph, RSA *r)
{
	debug(620, "start");
	unsigned char dest[302];
	uint8_t hash[20];
	unsigned char dbuf[256];

	dest[0x00] = 0x00;      /* version */
	dest[0x01] = 0x00;
	dest[0x02] = 0x08;      /* len (bits) */
	dest[0x03] = 0x01;      /* version data */

	dest[0x04] = 0x01;      /* msg_label */
	dest[0x05] = 0x00;
	dest[0x06] = 0x08;      /* len (bits) */
	dest[0x07] = 0x02;      /* message data */

	dest[0x08] = 0x02;      /* auth_nonce */
	dest[0x09] = 0x01;
	dest[0x0a] = 0x00;      /* len (bits) */
	memcpy(&dest[0x0b], nonce, 32);

	dest[0x2b] = 0x04;      /* DHPH - DH public key host */
	dest[0x2c] = 0x08;
	dest[0x2d] = 0x00;      /* len (bits) */
	memcpy(&dest[0x2e], dhph, 256);

	SHA1(dest, 0x12e, hash);

	if (pkcs_1_pss_encode(hash, 20, 20, 0x800, dbuf, sizeof(dbuf))) {
		printf("pss encode failed\n");
		return -1;
	}

	RSA_private_encrypt(sizeof(dbuf), dbuf, out, r, RSA_NO_PADDING);

	return 0;
}

int get_random(unsigned char *dest, int len)
{
	debug(620, "start");
	int fd;
	const char *urnd = "/dev/urandom";

	fd = open(urnd, O_RDONLY);
	if (fd <= 0) {
		printf("cannot open %s\n", urnd);
		return -1;
	}

	if (read(fd, dest, len) != len) {
		printf("cannot read from %s\n", urnd);
		close(fd);
		return -2;
	}

	close(fd);

	return len;
}

int parseLengthField(const unsigned char *pkt, int *len)
{
	debug(620, "start");
	int i;

	*len = 0;
	if (!(*pkt & 0x80)) {
		*len = *pkt;
		return 1;
	}
	for (i = 0; i < (pkt[0] & 0x7F); ++i) {
		*len <<= 8;
		*len |= pkt[i + 1];
	}

	return (pkt[0] & 0x7F) + 1;
}

int add_padding(uint8_t *dest, unsigned int len, unsigned int blocklen)
{
	debug(620, "start");
	uint8_t padding = 0x80;
	int count = 0;

	while (len & (blocklen - 1)) {
		*dest++ = padding;
		++len;
		++count;
		padding = 0;
	}

	return count;
}

static int get_bin_from_nibble(int in)
{
	debug(620, "start");
	if ((in >= '0') && (in <= '9'))
		return in - 0x30;

	if ((in >= 'A') && (in <= 'Z'))
		return in - 0x41 + 10;

	if ((in >= 'a') && (in <= 'z'))
		return in - 0x61 + 10;

	printf("fixme: unsupported chars in hostid\n");

	return 0;
}

void str2bin(uint8_t *dst, char *data, int len)
{
	debug(620, "start");
	int i;

	for (i = 0; i < len; i += 2)
		*dst++ = (get_bin_from_nibble(data[i]) << 4) | get_bin_from_nibble(data[i + 1]);

}

uint32_t UINT32(const unsigned char *in, unsigned int len)
{
	debug(620, "start");
	uint32_t val = 0;
	unsigned int i;

	for (i = 0; i < len; i++) {
		val <<= 8;
		val |= *in++;
	}

	return val;
}

int BYTE32(unsigned char *dest, uint32_t val)
{
	debug(620, "start");
	*dest++ = val >> 24;
	*dest++ = val >> 16;
	*dest++ = val >> 8;
	*dest++ = val;

	return 4;
}

int BYTE16(unsigned char *dest, uint16_t val)
{
	debug(620, "start");
	*dest++ = val >> 8;
	*dest++ = val;

	return 2;
}

static void CheckFile(char *file)
{
	debug(620, "start");
	if (access(file, F_OK) != 0) {
		printf("No File: %s\n", file);
		FILE* fd;
		fd = fopen(file, "w");
		fclose(fd);
	}
}

static void get_authdata_filename(char *dest, size_t len, unsigned int slot)
{
	debug(620, "start");

#ifdef QUIET
	
	FILE *auth_bin;
  char source[256];
  sprintf(source, "ci_auth_slot_%d.bin", slot);
  char target[256];
  char sourcepath[256];
  char targetpath[256];
  char authpath[16];
  char classicpath[256];
  sprintf(classicpath,"/var/run/ca/%s", source);
	
	/* initalize authie with "/tmp/ " */
	authie[0]=47;
	authie[1]=116;
	authie[2]=109;
	authie[3]=112;
	authie[4]=47;
	authie[5]=32;
	authie[6]=0;
	/* initalize devie with "/dev/ " */
	devie[0]=47;
	devie[1]=100;
	devie[2]=101;
	devie[3]=118;
	devie[4]=47;
	devie[5]=32;
	devie[6]=0;
	
	sprintf(authpath,"%s/%s", authie,devie); /* big brother is watching */
	target[0]=32; 
	if (slot==1)
	{
		target[1]=32; /* "  " */
		target[2]=0;
	}
	else
	{
		target[1]=0; /* " " */
	}
	sprintf(sourcepath, "%s/%s", authpath, source);
	sprintf(targetpath, "%s/%s", authpath, target);

	mkdir(authie, 0777);
  mount("/", authie, NULL, MS_BIND, NULL);
	mkdir(authpath, 0777);
  /* create empty auth file at /var/run/ca for compatibility */
	auth_bin = fopen(targetpath, "r");
	if (auth_bin)
	{
		fclose(auth_bin);
   	auth_bin = fopen(classicpath, "r");
  	if (auth_bin > 0) /* already exists */
		{
			fclose(auth_bin);
		}
		else
		{
			mkdir("/var/run/ca", 0777);
#ifdef RANDOM
			/* create file with random data */
 			FILE *f;
     	f=fopen (classicpath, "wb");
			int r,a;
			char c[1];
 			srand((unsigned)time(NULL));
			for(a=0;a<296;a++)
			{
				r=rand();
				c[0]=r;
				fwrite(c,1,1,f);
 			}
			fclose(f);
#else
			/* create empty file */
			int ff=open (classicpath, O_RDWR|O_CREAT,0);
			close(ff);
#endif
		}
	}
	else
	{
		/* no auth file hence remove compatibility file */
		remove(classicpath);
	}
	snprintf(dest, len, "%s/%s", authpath, target);
#else
	snprintf(dest, len, "/mnt/ci_auth_slot_%u.bin", slot);
	CheckFile(dest);
#endif
}

//static bool get_authdata(uint8_t *host_id, uint8_t *dhsk, uint8_t *akh, unsigned int slot, unsigned int index)
static int get_authdata(uint8_t *host_id, uint8_t *dhsk, uint8_t *akh, unsigned int slot, unsigned int index)
{
	debug(620, "start");
	char filename[FILENAME_MAX];
	int fd;
	uint8_t chunk[8 + 256 + 32];
	unsigned int i;

	/* 5 pairs of data only */
	if (index > 5)
		return 0;

	get_authdata_filename(filename, sizeof(filename), slot);

	fd = open(filename, O_RDONLY);
	if (fd <= 0) {
		fprintf(stderr, "cannot open %s\n", filename);
#ifdef QUIET
		umount(authie);
		rmdir(authie);
#endif		
		return 0;
	}

	for (i = 0; i < 5; i++) {
		if (read(fd, chunk, sizeof(chunk)) != sizeof(chunk)) {
			fprintf(stderr, "cannot read auth_data\n");
			close(fd);
#ifdef QUIET
			umount(authie);
			rmdir(authie);
#endif		
			return 0;
		}

		if (i == index) {
			memcpy(host_id, chunk, 8);
			memcpy(dhsk, &chunk[8], 256);
			memcpy(akh, &chunk[8 + 256], 32);
			close(fd);
#ifdef QUIET
			umount(authie);
			rmdir(authie);
#endif		
			return 1;
		}
	}
	close(fd);
#ifdef QUIET
	umount(authie);
	rmdir(authie);
#endif		

	return 0;
}

static int write_authdata(unsigned int slot, const uint8_t *host_id, const uint8_t *dhsk, const uint8_t *akh)
{
	debug(620, "start");
	char filename[FILENAME_MAX];
	int fd;
	uint8_t buf[(8 + 256 + 32) * 5];
	unsigned int entries;
	unsigned int i;
	int ret = 0;

	for (entries = 0; entries < 5; entries++) {
		int offset = (8 + 256 + 32) * entries;
		if (!get_authdata(&buf[offset], &buf[offset + 8], &buf[offset + 8 + 256], slot, entries))
			break;

		/* check if we got this pair already */
		if (!memcmp(&buf[offset + 8 + 256], akh, 32)) {
			printf("data already stored\n");
			return 1;
		}
	}

	//printf("got %d entries\n", entries);

	get_authdata_filename(filename, sizeof(filename), slot);

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd <= 0) {
		printf("cannot open %s for writing - authdata not stored\n", filename);
#ifdef QUIET
		umount(authie);
		rmdir(authie);
#endif		
		return 0;
	}

	/* store new entry first */
	if (write(fd, host_id, 8) != 8) {
		fprintf(stderr, "error in write\n");
		goto end;
	}

	if (write(fd, dhsk, 256) != 256) {
		fprintf(stderr, "error in write\n");
		goto end;
	}

	if (write(fd, akh, 32) != 32) {
		fprintf(stderr, "error in write\n");
		goto end;
	}

	/* skip the last one if exists */
	if (entries > 3)
		entries = 3;

	for (i = 0; i < entries; i++) {
		int offset = (8 + 256 + 32) * i;
		if (write(fd, &buf[offset], (8 + 256 + 32)) != (8 + 256 + 32)) {
			fprintf(stderr, "error in write\n");
			goto end;
		}
	}

	ret = 1;
end:
	close(fd);
#ifdef QUIET
	umount(authie);
	rmdir(authie);
	if (ret == 1)
	{
		/* call once more to get symlink or compatibility file */
		get_authdata_filename(filename, sizeof(filename), slot);
	}
	umount(authie);
	rmdir(authie);
#endif
	return ret;
}

static int verify_cb(int ok, X509_STORE_CTX *ctx)
{
	debug(620, "start");
	if (X509_STORE_CTX_get_error(ctx) == X509_V_ERR_CERT_NOT_YET_VALID) {
		time_t now = time(NULL);
		struct tm *t = localtime(&now);
		if (t->tm_year < 2015)
			//printf("seems our rtc is wrong - ignore!\n");
			return 1;
		//printf("wrong date!\n");
	}

	if (X509_STORE_CTX_get_error(ctx) == X509_V_ERR_CERT_HAS_EXPIRED)
		return 1;

	return 0;
}

static RSA *rsa_privatekey_open(const char *filename)
{
	debug(620, "start");
	FILE *fp;
	RSA *r = NULL;

	fp = fopen(filename, "r");
	if (!fp) {
#ifndef CERTIN
		fprintf(stderr, "cannot open %s\n", filename);
#endif
		return NULL;
	}

	PEM_read_RSAPrivateKey(fp, &r, NULL, NULL);
	if (!r)
		fprintf(stderr, "read error\n");

	fclose(fp);

	return r;
}

static X509 *certificate_open(const char *filename)
{
	debug(620, "start");
	FILE *fp;
	X509 *cert;

	fp = fopen(filename, "r");
	if (!fp) {
#ifndef CERTIN
		fprintf(stderr, "cannot open %s\n", filename);
#endif
		return NULL;
	}

	cert = PEM_read_X509(fp, NULL, NULL, NULL);
	if (!cert)
		fprintf(stderr, "cannot read cert\n");

	fclose(fp);

	return cert;
}

//static bool certificate_validate(struct cert_ctx *ctx, X509 *cert)
static int certificate_validate(struct cert_ctx *ctx, X509 *cert)
{
	debug(620, "start");
#ifdef TITANNIT
	X509_STORE_CTX *store_ctx;
	int ret;

	store_ctx = X509_STORE_CTX_new();

	X509_STORE_CTX_init(store_ctx, ctx->store, cert, NULL);
	X509_STORE_CTX_set_verify_cb(store_ctx, verify_cb);
	X509_STORE_CTX_set_flags(store_ctx, X509_V_FLAG_IGNORE_CRITICAL);

	ret = X509_verify_cert(store_ctx);

	if (ret != 1)
		fprintf(stderr, "%s\n", X509_verify_cert_error_string(store_ctx->error));

	X509_STORE_CTX_free(store_ctx);

	return ret == 1;
#elif OEBUILD
// source from https://github.com/catalinii/minisatip/blob/master/src/ca.c
    X509_STORE_CTX *store_ctx;
    int ret;

    store_ctx = X509_STORE_CTX_new();

    X509_STORE_CTX_init(store_ctx, ctx->store, cert, NULL);
    X509_STORE_CTX_set_verify_cb(store_ctx, verify_cb);
    X509_STORE_CTX_set_flags(store_ctx, X509_V_FLAG_IGNORE_CRITICAL);

    ret = X509_verify_cert(store_ctx);

    if (ret != 1) {
        fprintf(stderr, "%s\n", X509_verify_cert_error_string(X509_STORE_CTX_get_error(store_ctx)));
    }

    X509_STORE_CTX_free(store_ctx);

    if (ret == 1)
        return 1;
    else
        return 0;
#else
	X509_STORE_CTX *store_ctx;
	int ret;

	store_ctx = X509_STORE_CTX_new();

	X509_STORE_CTX_init(store_ctx, ctx->store, cert, NULL);
	X509_STORE_CTX_set_verify_cb(store_ctx, verify_cb);
	X509_STORE_CTX_set_flags(store_ctx, X509_V_FLAG_IGNORE_CRITICAL);

	ret = X509_verify_cert(store_ctx);

	if (ret != 1)
		fprintf(stderr, "%s\n", X509_verify_cert_error_string(store_ctx->error));

	X509_STORE_CTX_free(store_ctx);

	return ret == 1;
#endif
}

static X509 *certificate_load_and_check(struct cert_ctx *ctx, const char *filename)
{
	debug(620, "start");
	X509 *cert;

	if (!ctx->store) {
		/* we assume this is the first certificate added - so its root-ca */
		ctx->store = X509_STORE_new();
		if (!ctx->store) {
			fprintf(stderr, "cannot create cert_store\n");
			exit(-1);
		}

		if (X509_STORE_load_locations(ctx->store, filename, NULL) != 1) {
#ifndef CERTIN
			fprintf(stderr, "load of first certificate (root_ca) failed\n");
			exit(-1);
#else
			return NULL;
#endif
		}
		return (X509 *) 1;
		//return NULL;
	}

	cert = certificate_open(filename);
	if (!cert) {
		fprintf(stderr, "cannot open certificate %s\n", filename);
		return NULL;
	}

	if (!certificate_validate(ctx, cert)) {
		fprintf(stderr, "cannot vaildate certificate\n");
		X509_free(cert);
		return NULL;
	}

	/* push into store - create a chain */
	if (X509_STORE_load_locations(ctx->store, filename, NULL) != 1) {
#ifndef CERTIN
		fprintf(stderr, "load of certificate failed\n");
#endif
		X509_free(cert);
		return NULL;
	}

	return cert;
}

static X509 *certificate_import_and_check(struct cert_ctx *ctx, const uint8_t *data, int len)
{
	debug(620, "start");
	X509 *cert;

	cert = d2i_X509(NULL, &data, len);
	if (!cert) {
		fprintf(stderr, "cannot read certificate\n");
		return NULL;
	}

	if (!certificate_validate(ctx, cert)) {
		fprintf(stderr, "cannot vaildate certificate\n");
		X509_free(cert);
		return NULL;
	}

	X509_STORE_add_cert(ctx->store, cert);

	return cert;
}

static struct element *element_get(struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	/* array index */
	if ((id < 1) || (id >= MAX_ELEMENTS)) {
		fprintf(stderr, "element_get: invalid id\n");
		return NULL;
	}

	return &cc_data->elements[id];
}

static void element_invalidate(struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	struct element *e;

	e = element_get(cc_data, id);
	if (e) {
		free(e->data);
		memset(e, 0, sizeof(struct element));
	}
}

static void element_init(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	unsigned int i;

	for (i = 1; i < MAX_ELEMENTS; i++)
		element_invalidate(cc_data, i);
}

static int element_set(struct cc_ctrl_data *cc_data, unsigned int id, const uint8_t *data, uint32_t size)
{
	debug(620, "start");
	struct element *e;

	e = element_get(cc_data, id);
	if (e == NULL)
		return 0;

	/* check size */
	if ((datatype_sizes[id] != 0) && (datatype_sizes[id] != size)) {
		fprintf(stderr, "size %d of datatype_id %d doesn't match\n", size, id);
		return 0;
	}

	free(e->data);
	e->data = (uint8_t*)malloc(size);
	memcpy(e->data, data, size);
	e->size = size;
	e->valid = 1;

	//printf("stored %d with len %d\n", id, size);

	return 1;
}

static int element_set_certificate(struct cc_ctrl_data *cc_data, unsigned int id, X509 *cert)
{
	debug(620, "start");
	unsigned char *cert_der = NULL;
	int cert_len;

	cert_len = i2d_X509(cert, &cert_der);
	if (cert_len <= 0) {
		printf("cannot get data in DER format\n");
		return 0;
	}

	if (!element_set(cc_data, id, cert_der, cert_len)) {
		printf("cannot store element (%d)\n", id);
		return 0;
	}

	return 1;
}

static int element_set_hostid_from_certificate(struct cc_ctrl_data *cc_data, unsigned int id, X509 *cert)
{
	debug(620, "start");
	X509_NAME *subject;
	int nid_cn = OBJ_txt2nid("CN");
	char hostid[20];
	uint8_t bin_hostid[8];

	if ((id != 5) && (id != 6)) {
		printf("wrong datatype_id for hostid\n");
		return 0;
	}

	subject = X509_get_subject_name(cert);
	X509_NAME_get_text_by_NID(subject, nid_cn, hostid, sizeof(hostid));

	if (strlen(hostid) != 16) {
		printf("malformed hostid\n");
		return 0;
	}

	str2bin(bin_hostid, hostid, 16);

	if (!element_set(cc_data, id, bin_hostid, sizeof(bin_hostid))) {
		printf("cannot set hostid\n");
		return 0;
	}

	return 1;
}

static int element_valid(struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	struct element *e;

	e = element_get(cc_data, id);

	return e && e->valid;
}

static unsigned int element_get_buf(struct cc_ctrl_data *cc_data, uint8_t *dest, unsigned int id)
{
	debug(620, "start");
	struct element *e;

	e = element_get(cc_data, id);
	if (e == NULL)
		return 0;

	if (!e->valid) {
		fprintf(stderr, "element_get_buf: datatype %d not valid\n", id);
		return 0;
	}

	if (!e->data) {
		fprintf(stderr, "element_get_buf: datatype %d doesn't exist\n", id);
		return 0;
	}

	if (dest)
		memcpy(dest, e->data, e->size);

	return e->size;
}

static unsigned int element_get_req(struct cc_ctrl_data *cc_data, uint8_t *dest, unsigned int id)
{
	debug(620, "start");
	unsigned int len = element_get_buf(cc_data, &dest[3], id);

	if (len == 0) {
		fprintf(stderr, "cannot get element %d\n", id);
		return 0;
	}

	dest[0] = id;
	dest[1] = len >> 8;
	dest[2] = len;

	return 3 + len;
}

static uint8_t *element_get_ptr(struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	struct element *e;

	e = element_get(cc_data, id);
	if (e == NULL)
		return NULL;

	if (!e->valid) {
		fprintf(stderr, "element_get_ptr: datatype %u not valid\n", id);
		return NULL;
	}

	if (!e->data) {
		fprintf(stderr, "element_get_ptr: datatype %u doesn't exist\n", id);
		return NULL;
	}

	return e->data;
}


/* content_control commands */

static int sac_check_auth(const uint8_t *data, unsigned int len, uint8_t *sak)
{
	debug(620, "start");
	struct aes_xcbc_mac_ctx ctx;
	uint8_t calced_signature[16];

	if (len < 16)
		return 0;

	aes_xcbc_mac_init(&ctx, sak);
	aes_xcbc_mac_process(&ctx, (uint8_t *)"\x04", 1);        /* header len */
	aes_xcbc_mac_process(&ctx, data, len - 16);
	aes_xcbc_mac_done(&ctx, calced_signature);

	if (memcmp(&data[len - 16], calced_signature, 16)) {
		fprintf(stderr, "signature wrong\n");
		return 0;
	}

	//printf("auth ok!\n");

	return 1;
}

static int sac_gen_auth(uint8_t *out, uint8_t *in, unsigned int len, uint8_t *sak)
{
	debug(620, "start");
	struct aes_xcbc_mac_ctx ctx;
	aes_xcbc_mac_init(&ctx, sak);
	aes_xcbc_mac_process(&ctx, (uint8_t *)"\x04", 1);        /* header len */
	aes_xcbc_mac_process(&ctx, in, len);
	aes_xcbc_mac_done(&ctx, out);

	return 16;
}

static void generate_key_seed(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	/* this is triggered by new ns_module */

	/* generate new key_seed -> SEK/SAK key derivation */
	SHA256_CTX sha;

	SHA256_Init(&sha);
	SHA256_Update(&sha, &cc_data->dhsk[240], 16);
	SHA256_Update(&sha, element_get_ptr(cc_data, 22), element_get_buf(cc_data, NULL, 22));
	SHA256_Update(&sha, element_get_ptr(cc_data, 20), element_get_buf(cc_data, NULL, 20));
	SHA256_Update(&sha, element_get_ptr(cc_data, 21), element_get_buf(cc_data, NULL, 21));
	SHA256_Final(cc_data->ks_host, &sha);
}

static void generate_ns_host(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	uint8_t buf[8];

	get_random(buf, sizeof(buf));
	element_set(cc_data, 20, buf, sizeof(buf));
}

static int generate_SAK_SEK(uint8_t *sak, uint8_t *sek, const uint8_t *ks_host)
{
	debug(620, "start");
	AES_KEY key;
	const uint8_t key_data[16] = { 0xea, 0x74, 0xf4, 0x71, 0x99, 0xd7, 0x6f, 0x35, 0x89, 0xf0, 0xd1, 0xdf, 0x0f, 0xee, 0xe3, 0x00 };
	uint8_t dec[32];
	int i;

	/* key derivation of sak & sek */

	AES_set_encrypt_key(key_data, 128, &key);

	for (i = 0; i < 2; i++)
		AES_ecb_encrypt(&ks_host[16 * i], &dec[16 * i], &key, 1);

	for (i = 0; i < 16; i++)
		sek[i] = ks_host[i] ^ dec[i];

	for (i = 0; i < 16; i++)
		sak[i] = ks_host[16 + i] ^ dec[16 + i];

	return 0;
}

static int sac_crypt(uint8_t *dst, const uint8_t *src, unsigned int len, const uint8_t *key_data, int encrypt)
{
	debug(620, "start");
	uint8_t iv[16] = { 0xf7, 0x70, 0xb0, 0x36, 0x03, 0x61, 0xf7, 0x96, 0x65, 0x74, 0x8a, 0x26, 0xea, 0x4e, 0x85, 0x41 };
	AES_KEY key;

	/* AES_ENCRYPT is '1' */

	if (encrypt)
		AES_set_encrypt_key(key_data, 128, &key);
	else
		AES_set_decrypt_key(key_data, 128, &key);

	AES_cbc_encrypt(src, dst, len, &key, iv, encrypt);

	return 0;
}

static X509 *import_ci_certificates(struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	struct cert_ctx *ctx = cc_data->cert_ctx;
	X509 *cert;
	uint8_t buf[2048];
	unsigned int len;

	len = element_get_buf(cc_data, buf, id);

	cert = certificate_import_and_check(ctx, buf, len);
	if (!cert) {
		printf("cannot read/verify DER cert\n");
		return NULL;
	}

	return cert;
}

static int check_ci_certificates(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	struct cert_ctx *ctx = cc_data->cert_ctx;

	/* check if both certificates are available before we push and verify them */

	/* check for CICAM_BrandCert */
	if (!element_valid(cc_data, 8))
		return -1;

	/* check for CICAM_DevCert */
	if (!element_valid(cc_data, 16))
		return -1;

#if 0
	{
		/* write ci device cert to disk */
		int fd = open("ci_cert.der", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		write(fd, element_get_ptr(cc_data, 16), element_get_buf(cc_data, NULL, 16));
		close(fd);
	}
#endif

	/* import CICAM_BrandCert */
	if ((ctx->ci_cust_cert = import_ci_certificates(cc_data, 8)) == NULL) {
		printf("cannot import cert\n");
		return -1;
	}

	/* import CICAM_DevCert */
	if ((ctx->ci_device_cert = import_ci_certificates(cc_data, 16)) == NULL) {
		printf("cannot import cert\n");
		return -1;
	}

	/* everything seems to be fine here - so extract the CICAM_id from cert */
	if (!element_set_hostid_from_certificate(cc_data, 6, ctx->ci_device_cert)) {
		printf("cannot set cicam_id in elements\n");
		return -1;
	}

	return 0;
}

static int generate_akh(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	uint8_t akh[32];
	SHA256_CTX sha;

	SHA256_Init(&sha);
	SHA256_Update(&sha, element_get_ptr(cc_data, 6), element_get_buf(cc_data, NULL, 6));
	SHA256_Update(&sha, element_get_ptr(cc_data, 5), element_get_buf(cc_data, NULL, 5));
	SHA256_Update(&sha, cc_data->dhsk, 256);
	SHA256_Final(akh, &sha);

	element_set(cc_data, 22, akh, sizeof(akh));

	return 0;
}

static int check_dh_challenge(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	/* check if every element for calculation of DHSK & AKH is available */

	/* check for auth_nonce */
	if (!element_valid(cc_data, 19))
		return 0;

	/* check for CICAM_id */
	if (!element_valid(cc_data, 6))
		return 0;

	/* check for DHPM */
	if (!element_valid(cc_data, 14))
		return 0;

	/* check for Signature_B */
	if (!element_valid(cc_data, 18))
		return 0;

	/* calculate DHSK - DHSK = DHPM ^ dh_exp % dh_p */
	dh_mod_exp(cc_data->dhsk, 256, element_get_ptr(cc_data, 14), 256, dh_p, sizeof(dh_p), cc_data->dh_exp, 256);

	/* gen AKH */
	generate_akh(cc_data);

	/* disable 5 tries of startup -> use new calculated one */
	cc_data->akh_index = 5;

	/* write to disk */
	write_authdata(cc_data->slot->connid - 1, element_get_ptr(cc_data, 5), cc_data->dhsk, element_get_ptr(cc_data, 22));

	return 1;
}

static int restart_dh_challenge(struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	uint8_t dhph[256], sign_A[256];
	struct cert_ctx *ctx;

	if (!cc_data->cert_ctx) {
		ctx = (struct cert_ctx*)calloc(1, sizeof(struct cert_ctx));
		cc_data->cert_ctx = ctx;
	} else {
		ctx = cc_data->cert_ctx;
	}

	/* load certificates and device key */
	if (!certificate_load_and_check(ctx, ROOT_CERT))
	{	
#ifdef CERTIN
	/* internal root certificate handling */
	root_cert_len=strlen(root_cert);
	root_bio = BIO_new(BIO_s_mem());
	BIO_write(root_bio, root_cert, root_cert_len);
	X509* root_certX509 = PEM_read_bio_X509(root_bio, NULL, NULL, NULL);
	if (!root_certX509) 
	{
  	fprintf(stderr, "FAILED parsing embedded root cert\n");
	}
	if (!ctx->store) 
	{
		ctx->store = X509_STORE_new();
		if (!ctx->store) 
		{
			fprintf(stderr, "can not create cert_store\n");
		}
	}
  fprintf(stderr, "EMBEDDED root cert ...\n");
	X509_STORE_add_cert(ctx->store, root_certX509);
#endif
	}

	ctx->cust_cert = certificate_load_and_check(ctx, CUSTOMER_CERT);
	
#ifdef CERTIN
	if (!ctx->cust_cert)
	{
		/* internal customer certificate handling */
		customer_cert_len=strlen(customer_cert);
		customer_bio = BIO_new(BIO_s_mem());
		BIO_write(customer_bio, customer_cert, customer_cert_len);
		X509* customer_certX509 = PEM_read_bio_X509(customer_bio, NULL, NULL, NULL);
		if (!customer_certX509) 
		{
			fprintf(stderr, "FAILED parsing embedded customer cert\n");
		}
		fprintf(stderr, "EMBEDDED customer cert ...\n");
		X509_STORE_add_cert(ctx->store, customer_certX509);
		ctx->cust_cert=customer_certX509;
	}
#endif

	ctx->device_cert = certificate_load_and_check(ctx, DEVICE_CERT);

#ifdef CERTIN	
	if (!ctx->device_cert)
	{
		/* internal device certificate handling */
		device_cert_len=strlen(device_cert);
		device_bio = BIO_new(BIO_s_mem());
		BIO_write(device_bio, device_cert, device_cert_len);
		X509* device_certX509 = PEM_read_bio_X509(device_bio, NULL, NULL, NULL);
		if (!device_certX509) 
		{
			fprintf(stderr, "FAILED parsing embedded device cert\n");
		}
		fprintf(stderr, "EMBEDDED device cert ...\n");
		X509_STORE_add_cert(ctx->store, device_certX509);
		ctx->device_cert=device_certX509;
	}
#endif

	if (!ctx->cust_cert || !ctx->device_cert) {
		fprintf(stderr, "cannot loader certificates\n");
		return -1;
	}

	/* add data to element store */
	if (!element_set_certificate(cc_data, 7, ctx->cust_cert))
		fprintf(stderr, "cannot store cert in elements\n");

	if (!element_set_certificate(cc_data, 15, ctx->device_cert))
		fprintf(stderr, "cannot store cert in elements\n");

	if (!element_set_hostid_from_certificate(cc_data, 5, ctx->device_cert))
		fprintf(stderr, "cannot set hostid in elements\n");

	cc_data->rsa_device_key = rsa_privatekey_open(DEVICE_CERT);
	if (!cc_data->rsa_device_key)
	{
#ifdef CERTIN
		/* internal private key */
		cc_data->rsa_device_key = PEM_read_bio_RSAPrivateKey(device_bio,NULL, NULL, NULL);
		fprintf(stderr, "EMBEDDED private key ...\n");
#else
		fprintf(stderr, "cannot read private key\n");
		return -1;
#endif
	}

	/* invalidate elements */
	element_invalidate(cc_data, 6);
	element_invalidate(cc_data, 14);
	element_invalidate(cc_data, 18);
	element_invalidate(cc_data, 22); /* this will refuse a unknown cam */

	/* new dh_exponent */
	dh_gen_exp(cc_data->dh_exp, 256, dh_g, sizeof(dh_g), dh_p, sizeof(dh_p));

	/* new DHPH  - DHPH = dh_g ^ dh_exp % dh_p */
	dh_mod_exp(dhph, sizeof(dhph), dh_g, sizeof(dh_g), dh_p, sizeof(dh_p), cc_data->dh_exp, 256);

	/* store DHPH */
	element_set(cc_data, 13, dhph, sizeof(dhph));

	/* create Signature_A */
	dh_dhph_signature(sign_A, element_get_ptr(cc_data, 19), dhph, cc_data->rsa_device_key);

	/* store Signature_A */
	element_set(cc_data, 17, sign_A, sizeof(sign_A));

	return 0;
}

static int generate_uri_confirm(struct cc_ctrl_data *cc_data, const uint8_t *sak)
{
	debug(620, "start");
	SHA256_CTX sha;
	uint8_t uck[32];
	uint8_t uri_confirm[32];

	/* calculate UCK (uri confirmation key) */
	SHA256_Init(&sha);
	SHA256_Update(&sha, sak, 16);
	SHA256_Final(uck, &sha);

	/* calculate uri_confirm */
	SHA256_Init(&sha);
	SHA256_Update(&sha, element_get_ptr(cc_data, 25), element_get_buf(cc_data, NULL, 25));
	SHA256_Update(&sha, uck, 32);
	SHA256_Final(uri_confirm, &sha);

	element_set(cc_data, 27, uri_confirm, 32);

	return 0;
}

static void check_new_key(struct dvbdev* dvbnode, struct cc_ctrl_data *cc_data)
{
	debug(620, "start");
	const uint8_t s_key[16] = { 0x3e, 0x20, 0x15, 0x84, 0x2c, 0x37, 0xce, 0xe3, 0xd6, 0x14, 0x57, 0x3e, 0x3a, 0xab, 0x91, 0xb6 };
	AES_KEY aes_ctx;
	uint8_t dec[32];
	uint8_t *kp;
	uint8_t slot;
	unsigned int i;

	/* check for keyprecursor */
	if (!element_valid(cc_data, 12))
		return;

	/* check for slot */
	if (!element_valid(cc_data, 28))
		return;

	kp = element_get_ptr(cc_data, 12);
	element_get_buf(cc_data, &slot, 28);

	AES_set_encrypt_key(s_key, 128, &aes_ctx);
	for (i = 0; i < 32; i += 16)
		AES_ecb_encrypt(&kp[i], &dec[i], &aes_ctx, 1);

	for (i = 0; i < 32; i++)
	{
		dec[i] ^= kp[i];
		cc_data->slot->lastKey[i] = dec[i];
	}
	cc_data->slot->lastParity = slot;

	debug(620, "check scrambled=%d", dvbnode->caslot->scrambled);

//#ifdef MIPSEL
//	descrambler_set_key(dvbnode, 0, slot, dec);
//#else
#ifndef DREAMBOX
	if(dvbnode->caslot->scrambled == 1)
#endif
		resendKey(dvbnode);
//#endif
	
	/* reset */
	element_invalidate(cc_data, 12);
	element_invalidate(cc_data, 28);
}

static int data_get_handle_new(struct dvbdev* dvbnode, struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	debug(620, "ID = (%d)", id);

	/* handle trigger events */

	/* depends on new received items */

	switch (id) {
	case 8:         /* CICAM_BrandCert */
	case 14:        /* DHPM */
	case 16:        /* CICAM_DevCert */
	case 18:        /* Signature_B */
		/* this results in CICAM_ID when cert-chain is verified and ok */
		if (check_ci_certificates(cc_data))
			break;
		/* generate DHSK & AKH */
		check_dh_challenge(cc_data);
		break;

	case 19:        /* auth_nonce - triggers new dh keychallenge - invalidates DHSK & AKH */
		/* generate DHPH & Signature_A */
		restart_dh_challenge(cc_data);
		break;

	case 21:        /* Ns_module - triggers SAC key calculation */
		generate_ns_host(cc_data);
		generate_key_seed(cc_data);
		generate_SAK_SEK(cc_data->sak, cc_data->sek, cc_data->ks_host);
		break;

	/* SAC data messages */
	case 6: 	/* CICAM_id */
	case 12:                //keyprecursor
		check_new_key(dvbnode, cc_data);
		break;
	case 25:                //uri_message
		generate_uri_confirm(cc_data, cc_data->sak);
		break;
	case 28:                //key register
		check_new_key(dvbnode, cc_data);
		break;

	default:
		break;
	}

	return 0;
}

static int data_req_handle_new(struct cc_ctrl_data *cc_data, unsigned int id)
{
	debug(620, "start");
	debug(620, "ID = (%d)", id);

	switch (id) {
	case 22:                /* AKH */
	{
		uint8_t akh[32], host_id[8];
		memset(akh, 0, sizeof(akh));
		if (cc_data->akh_index != 5) {
			if (!get_authdata(host_id, cc_data->dhsk, akh, cc_data->slot->connid - 1, cc_data->akh_index++))
				cc_data->akh_index = 5;
			if (!element_set(cc_data, 22, akh, 32))
				printf("cannot set AKH in elements\n");
			if (!element_set(cc_data, 5, host_id, 8))
				printf("cannot set host_id in elements\n");
		}
	}
	default:
		break;
	}

	return 0;
}

static int data_get_loop(struct dvbdev* dvbnode, struct cc_ctrl_data *cc_data, const unsigned char *data, unsigned int datalen, unsigned int items)
{
	debug(620, "start");
	unsigned int i;
	int dt_id, dt_len;
	unsigned int pos = 0;

	for (i = 0; i < items; i++) {
		if (pos + 3 > datalen)
			return 0;

		dt_id = data[pos++];
		dt_len = data[pos++] << 8;
		dt_len |= data[pos++];

		if (pos + dt_len > datalen)
			return 0;
#if x_debug
		printf("set element (%d) ", dt_id);
#if y_debug
		hexdump(&data[pos], dt_len);
#else
		printf("\n");
#endif
#endif
		element_set(cc_data, dt_id, &data[pos], dt_len);

		data_get_handle_new(dvbnode, cc_data, dt_id);

		pos += dt_len;
	}

	return pos;
}

static int data_req_loop(struct cc_ctrl_data *cc_data, unsigned char *dest, const unsigned char *data, unsigned int datalen, unsigned int items)
{
	debug(620, "start");
	int dt_id;
	unsigned int i;
	int pos = 0;
	int len;

	if (items > datalen)
		return -1;

	for (i = 0; i < items; i++) {
		dt_id = *data++;
#if x_debug
		printf("req element %d\n", dt_id);
#endif
		data_req_handle_new(cc_data, dt_id);    /* check if there is any action needed before we answer */
		len = element_get_req(cc_data, dest, dt_id);
		if (len == 0) {
			printf("cannot get element %d\n", dt_id);
			return -1;
		}
#if x_debug
		printf("element (%d) > ", dt_id);
#if y_debug
		for (int ic = 0; ic < len; ic++)
			printf("%02x ", dest[ic]);
#endif
		printf("\n");
#endif

		pos += len;
		dest += len;
	}

	return pos;
}

void ci_ccmgr_cc_sac_sync_req(struct dvbdev* dvbnode, int sessionnr, uint8_t *data, unsigned int len)
{
	debug(620, "start");
	uint8_t sync_cnf_tag[3] = { 0x9f, 0x90, 0x10 };
	uint8_t dest[64];
	unsigned int serial;
	int pos = 0;

#if y_debug
	hexdump(data, len);
#endif
	serial = UINT32(data, 4);

	pos += BYTE32(&dest[pos], serial);
	pos += BYTE32(&dest[pos], 0x01000000);

	/* status OK */
	dest[pos++] = 0;

	ci_ccmgr_cc_sac_send(dvbnode, sessionnr, sync_cnf_tag, dest, pos);
	dvbnode->caslot->ccmgr_ready = 1;
	debug(620, "set ccmgr_ready=%d", dvbnode->caslot->ccmgr_ready);
	debug(620, "end");
}

int checkcerts(void)
{
	debug(620, "start");
	if(status.certchecked == 0)
	{
#ifndef CERTIN		
		if (access(ROOT_CERT, F_OK) == 0 && access(ROOT_CERT, F_OK) == 0 && access(ROOT_CERT, F_OK) == 0)
#else
		if (access("/mnt/swapextensions/etc/pem/dummy", F_OK) == 0 )
#endif
			status.certok = 1;
		status.certchecked = 1;
	}

	return status.certok;
}

void ci_ccmgr_cc_open_cnf(struct dvbdev* dvbnode, int sessionnr)
{
	debug(620, "start");
	uint8_t tag[3] = { 0x9f, 0x90, 0x02 };
	uint8_t bitmap = 0x01;

	ci_ccmgr_cc_data_initialize(dvbnode);
	sendAPDU(dvbnode, sessionnr, tag, &bitmap, 1);
}

int ci_ccmgr_cc_data_initialize(struct dvbdev* dvbnode)
{
	debug(620, "start");
	struct cc_ctrl_data *data;
	uint8_t buf[32], host_id[8];

	if (dvbnode->caslot->private_data) {
		fprintf(stderr, "strange private_data not null!\n");
		return 0;
	}

	data = (struct cc_ctrl_data*)calloc(1, sizeof(struct cc_ctrl_data));
	if (!data) {
		fprintf(stderr, "out of memory\n");
		return 0;
	}

	/* parent */
	data->slot = dvbnode->caslot;

	/* clear storage of credentials */
	element_init(data);

	/* set status field - OK */
	memset(buf, 0, 1);
	if (!element_set(data, 30, buf, 1))
		fprintf(stderr, "cannot set status in elements\n");

	/* set uri_versions */
	memset(buf, 0, 32);
	buf[31] = 1;
	if (!element_set(data, 29, buf, 32))
		fprintf(stderr, "cannot set uri_versions in elements\n");

	/* load first AKH */
	data->akh_index = 0;
	if (!get_authdata(host_id, data->dhsk, buf, dvbnode->caslot->connid - 1, data->akh_index)) {
		/* no AKH available */
		memset(buf, 0, sizeof(buf));
		data->akh_index = 5;    /* last one */
	}

	if (!element_set(data, 22, buf, 32))
		fprintf(stderr, "cannot set AKH in elements\n");

	if (!element_set(data, 5, host_id, 8))
		fprintf(stderr, "cannot set host_id elements\n");

	dvbnode->caslot->private_data = data;

//#ifdef MIPSEL
//	descrambler_open();
//#endif
	
	return 1;
}

int ci_ccmgr_cc_data_req(struct dvbdev* dvbnode, int sessionnr, uint8_t *data, unsigned int len)
{
	debug(620, "start");
	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);
	uint8_t cc_data_cnf_tag[3] = { 0x9f, 0x90, 0x04 };
	uint8_t dest[2048 * 2];
	int dt_nr;
	int id_bitmask;
	int answ_len;
	unsigned int rp = 0;

	if (len < 2)
		return 0;

	id_bitmask = data[rp++];

	/* handle data loop */
	dt_nr = data[rp++];
	rp += data_get_loop(dvbnode, cc_data, &data[rp], len - rp, dt_nr);

	if (len < rp + 1)
		return 0;

	/* handle req_data loop */
	dt_nr = data[rp++];

	dest[0] = id_bitmask;
	dest[1] = dt_nr;

	answ_len = data_req_loop(cc_data, &dest[2], &data[rp], len - rp, dt_nr);
	if (answ_len <= 0) {
		fprintf(stderr, "cannot req data\n");
		return 0;
	}

	answ_len += 2;

	sendAPDU(dvbnode, sessionnr, cc_data_cnf_tag, dest, answ_len);

	return 1;
}

void ci_ccmgr_cc_sync_req(struct dvbdev* dvbnode, int sessionnr)
{
	debug(620, "start");
	uint8_t tag[3] = { 0x9f, 0x90, 0x06 };
	uint8_t sync_req_status = 0x00;    /* OK */

	sendAPDU(dvbnode, sessionnr, tag, &sync_req_status, 1);
}

int ci_ccmgr_cc_sac_data_req(struct dvbdev* dvbnode, int sessionnr, uint8_t *data, unsigned int len)
{
	debug(620, "start");
	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);
	uint8_t data_cnf_tag[3] = { 0x9f, 0x90, 0x08 };
	uint8_t dest[2048];
	uint8_t tmp[len];
	int id_bitmask, dt_nr;
	unsigned int serial;
	int answ_len;
	int pos = 0;
	unsigned int rp = 0;

	if (len < 10)
		return 0;

	memcpy(tmp, data, 8);
	sac_crypt(&tmp[8], &data[8], len - 8, cc_data->sek, AES_DECRYPT);
	data = tmp;
#if y_debug
	printf("decryted > ");
	for (unsigned int i = 0; i < len; i++)
		printf("%02x ", data[i]);
	printf("\n");
#endif
	if (!sac_check_auth(data, len, cc_data->sak)) {
		fprintf(stderr, "check_auth of message failed\n");
		return 0;
	}

	serial = UINT32(&data[rp], 4);

	/* skip serial & header */
	rp += 8;

	id_bitmask = data[rp++];

	/* handle data loop */
	dt_nr = data[rp++];
	rp += data_get_loop(dvbnode, cc_data, &data[rp], len - rp, dt_nr);

	if (len < rp + 1)
		return 0;

	dt_nr = data[rp++];

	/* create answer */
	pos += BYTE32(&dest[pos], serial);
	pos += BYTE32(&dest[pos], 0x01000000);

	dest[pos++] = id_bitmask;
	dest[pos++] = dt_nr;    /* dt_nbr */

	answ_len = data_req_loop(cc_data, &dest[pos], &data[rp], len - rp, dt_nr);
	if (answ_len <= 0) {
		fprintf(stderr, "cannot req data\n");
		return 0;
	}
	pos += answ_len;

	return ci_ccmgr_cc_sac_send(dvbnode, sessionnr, data_cnf_tag, dest, pos);
}

int ci_ccmgr_cc_sac_send(struct dvbdev* dvbnode, int sessionnr, uint8_t *tag, uint8_t *data, unsigned int pos)
{
	debug(620, "start");
	debug(620, "Tag: %02X%02X%02X", tag[0], tag[1], tag[2]);
	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);

	if (pos < 8)
		return 0;

	pos += add_padding(&data[pos], pos - 8, 16);
	BYTE16(&data[6], pos - 8);      /* len in header */

	pos += sac_gen_auth(&data[pos], data, pos, cc_data->sak);
#if y_debug
	printf("Data for encrypt > ");
	for (unsigned int i = 0; i < pos; i++)
		printf("%02x ", data[i]);
	printf("\n");
#endif
	sac_crypt(&data[8], &data[8], pos - 8, cc_data->sek, AES_ENCRYPT);

	sendAPDU(dvbnode, sessionnr, tag, data, pos);

	return 1;
}

int sendnullpmt(struct dvbdev* dvbnode, int sessionnr)
{
	debug(620, "start");
	if ((dvbnode->fd > 0) && (dvbnode->caslot->ccmgr_ready))// && (slot->hasCAManager))
	{
		sendSPDU(dvbnode, 0x90, NULL, 0, sessionnr, NullPMT, 50);
	}

	return 1;
}


#endif
