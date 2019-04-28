#include "md5.h"

// F(X,Y,Z) =(X&Y)|((~X)&Z)
uint32 F(uint32 x, uint32 y, uint32 z)
{
	return ((x & y) | ((~x) & z));
}

// G(X,Y,Z) =(X&Z)|(Y&(~Z))
uint32 G(uint32 x, uint32 y, uint32 z)
{
	return ((x & z) | (y & (~z)));
}

// H(X,Y,Z) =X^Y^Z
uint32 H(uint32 x, uint32 y, uint32 z)
{
	return (x ^ y ^ z);
}

// I(X,Y,Z)=Y^(X|(~Z))
uint32 I(uint32 x, uint32 y, uint32 z)
{
	return (y ^ (x | (~z)));
}

uint32 cyclic_shift_left(uint32 x, uint32 n)
{
	return ((x << n) | (x >> (32 - n)));
}

uint32 FF(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a += F(b, c, d) + x + ac;
	a = cyclic_shift_left(a, s);
	a += b;
	return a;
}

uint32 GG(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a += G(b, c, d) + x + ac;
	a = cyclic_shift_left(a, s);
	a += b;
	return a;
}

uint32 HH(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a += H(b, c, d) + x + ac;
	a = cyclic_shift_left(a, s);
	a += b;
	return a;
}

uint32 II(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac)
{
	a += I(b, c, d) + x + ac;
	a = cyclic_shift_left(a, s);
	a += b;
	return a;
}

void decode(const uint8* input, uint32* output, uint32 length)
{
	for (uint32 i = 0, j = 0; j < length; ++i, j += 4) 
	{
		output[i] = (input[j]) | ((input[j + 1]) << 8) | ((input[j + 2]) << 16) | ((input[j + 3]) << 24);
	}
}

void encode(const uint32* input, uint8* output, uint32 length)
{
	for (uint32 i = 0, j = 0; j < length; ++i, j += 4)
	{
		output[j] = (uint8)(input[i] & 0xff);
		output[j + 1] = (uint8)((input[i] >> 8) & 0xff);
		output[j + 2] = (uint8)((input[i] >> 16) & 0xff);
		output[j + 3] = (uint8)((input[i] >> 24) & 0xff);
	}
}

void transform(const uint8* block, uint32* state)
{
	uint32 a = state[0];
	uint32 b = state[1];
	uint32 c = state[2];
	uint32 d = state[3];
	uint32 x[16];
	
	decode(block, x, 64);

	// Round 1
	a = FF(a, b, c, d, x[0], s11, 0xd76aa478);
	d = FF(d, a, b, c, x[1], s12, 0xe8c7b756);
	c = FF(c, d, a, b, x[2], s13, 0x242070db);
	b = FF(b, c, d, a, x[3], s14, 0xc1bdceee);
	a = FF(a, b, c, d, x[4], s11, 0xf57c0faf);
	d = FF(d, a, b, c, x[5], s12, 0x4787c62a);
	c = FF(c, d, a, b, x[6], s13, 0xa8304613);
	b = FF(b, c, d, a, x[7], s14, 0xfd469501);
	a = FF(a, b, c, d, x[8], s11, 0x698098d8);
	d = FF(d, a, b, c, x[9], s12, 0x8b44f7af);
	c = FF(c, d, a, b, x[10], s13, 0xffff5bb1);
	b = FF(b, c, d, a, x[11], s14, 0x895cd7be);
	a = FF(a, b, c, d, x[12], s11, 0x6b901122);
	d = FF(d, a, b, c, x[13], s12, 0xfd987193);
	c = FF(c, d, a, b, x[14], s13, 0xa679438e);
	b = FF(b, c, d, a, x[15], s14, 0x49b40821);

	// Round 2
	a = GG(a, b, c, d, x[1], s21, 0xf61e2562);
	d = GG(d, a, b, c, x[6], s22, 0xc040b340);
	c = GG(c, d, a, b, x[11], s23, 0x265e5a51);
	b = GG(b, c, d, a, x[0], s24, 0xe9b6c7aa);
	a = GG(a, b, c, d, x[5], s21, 0xd62f105d);
	d = GG(d, a, b, c, x[10], s22, 0x2441453);
	c = GG(c, d, a, b, x[15], s23, 0xd8a1e681);
	b = GG(b, c, d, a, x[4], s24, 0xe7d3fbc8);
	a = GG(a, b, c, d, x[9], s21, 0x21e1cde6);
	d = GG(d, a, b, c, x[14], s22, 0xc33707d6);
	c = GG(c, d, a, b, x[3], s23, 0xf4d50d87);
	b = GG(b, c, d, a, x[8], s24, 0x455a14ed);
	a = GG(a, b, c, d, x[13], s21, 0xa9e3e905);
	d = GG(d, a, b, c, x[2], s22, 0xfcefa3f8);
	c = GG(c, d, a, b, x[7], s23, 0x676f02d9);
	b = GG(b, c, d, a, x[12], s24, 0x8d2a4c8a);

	// Round 3
	a = HH(a, b, c, d, x[5], s31, 0xfffa3942);
	d = HH(d, a, b, c, x[8], s32, 0x8771f681);
	c = HH(c, d, a, b, x[11], s33, 0x6d9d6122);
	b = HH(b, c, d, a, x[14], s34, 0xfde5380c);
	a = HH(a, b, c, d, x[1], s31, 0xa4beea44);
	d = HH(d, a, b, c, x[4], s32, 0x4bdecfa9);
	c = HH(c, d, a, b, x[7], s33, 0xf6bb4b60);
	b = HH(b, c, d, a, x[10], s34, 0xbebfbc70);
	a = HH(a, b, c, d, x[13], s31, 0x289b7ec6);
	d = HH(d, a, b, c, x[0], s32, 0xeaa127fa);
	c = HH(c, d, a, b, x[3], s33, 0xd4ef3085);
	b = HH(b, c, d, a, x[6], s34, 0x4881d05);
	a = HH(a, b, c, d, x[9], s31, 0xd9d4d039);
	d = HH(d, a, b, c, x[12], s32, 0xe6db99e5);
	c = HH(c, d, a, b, x[15], s33, 0x1fa27cf8);
	b = HH(b, c, d, a, x[2], s34, 0xc4ac5665);

	// Round 4
	a = II(a, b, c, d, x[0], s41, 0xf4292244);
	d = II(d, a, b, c, x[7], s42, 0x432aff97);
	c = II(c, d, a, b, x[14], s43, 0xab9423a7);
	b = II(b, c, d, a, x[5], s44, 0xfc93a039);
	a = II(a, b, c, d, x[12], s41, 0x655b59c3);
	d = II(d, a, b, c, x[3], s42, 0x8f0ccc92);
	c = II(c, d, a, b, x[10], s43, 0xffeff47d);
	b = II(b, c, d, a, x[1], s44, 0x85845dd1);
	a = II(a, b, c, d, x[8], s41, 0x6fa87e4f);
	d = II(d, a, b, c, x[15], s42, 0xfe2ce6e0);
	c = II(c, d, a, b, x[6], s43, 0xa3014314);
	b = II(b, c, d, a, x[13], s44, 0x4e0811a1);
	a = II(a, b, c, d, x[4], s41, 0xf7537e82);
	d = II(d, a, b, c, x[11], s42, 0xbd3af235);
	c = II(c, d, a, b, x[2], s43, 0x2ad7d2bb);
	b = II(b, c, d, a, x[9], s44, 0xeb86d391);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

void update(uint8* buffer, uint32* state, uint32* count, const uint8* input, uint32 len)
{
	uint32 i, index, length;

	index = (uint32)((count[0] >> 3) & 0x3f);
	if ((count[0] += (len << 3)) < (len << 3)) 
	{
		++count[1];
	}
	count[1] += (len >> 29);

	length = 64 - index;

	if (len >= length)
	{

		memcpy(&buffer[index], input, length);
		transform(buffer, state);

		for (i = length; i + 63 < len; i += 64)
		{
			transform(&input[i], state);
		}
		index = 0;
	}
	else
	{
		i = 0;
	}

	/* Buffer remaining input */
	memcpy(&buffer[index], &input[i], len - i);
}

string getMD5(string message)
{
	uint8 bin[8];
	uint32 state[4];
	uint32 count[2];
	uint32 index, length;
	uint8 buffer[64], digest[16];

	count[0] = 0;
	count[1] = 0;
	state[0] = A;
	state[1] = B;
	state[2] = C;
	state[3] = D;

	update(buffer, state, count, (const uint8*)message.c_str(), uint32(message.length()));

	encode(count, bin, 8);
	index = (uint32)((count[0] >> 3) & 0x3f);
	if (index < 56) length = 56 - index;
	else index = 120 - index;
	update(buffer, state, count, PADDING, length);
	update(buffer, state, count, bin, 8);
	encode(state, digest, 16);

	string str;
	for (uint32 i = 0; i < 16; ++i) {
		int t = digest[i];
		int a = t / 16;
		int b = t % 16;
		str.append(1, HEX[a]);
		str.append(1, HEX[b]);
	}
	return str;
}