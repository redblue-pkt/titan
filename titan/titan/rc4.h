#ifndef RC4_H
#define RC4_H

struct rc4ctx
{
	unsigned char S[256];
	unsigned char i;
	unsigned char j;
};

#define SWAPRC4(a, b) { unsigned char temp; temp = (a); (a) = (b); (b) = temp; }

void rc4init(struct rc4ctx *ctx, char *key, size_t keylen)
{
	int i;
	unsigned char j = 0;

	if(key == NULL) return;

	for(i = 0; i < sizeof(ctx->S); i++)
		ctx->S[i] = i;
    
	for(i = 0; i < sizeof(ctx->S); i++)
	{
		j += (ctx->S[i] + key[i % keylen]);
		SWAPRC4(ctx->S[i], ctx->S[j]);
	}
}

void rc4crypt(struct rc4ctx *ctx, char *data, size_t len)
{
	unsigned int i;

	if(data == NULL) return;

	ctx->i = 0;
	ctx->j = 0;

	for(i = 0; i < len; i++)
	{
		unsigned char s;

		ctx->i++;
		ctx->j += ctx->S[ctx->i];

		SWAPRC4(ctx->S[ctx->i], ctx->S[ctx->j]);

		s = ctx->S[ctx->i] + ctx->S[ctx->j];
		data[i] = data[i] ^ ctx->S[s];
	}
}

void rc4(char *data, size_t dlen, char *key, size_t klen)
{
	struct rc4ctx ctx;

	rc4init(&ctx, key, klen);
	rc4crypt(&ctx, data, dlen);
}

#endif
