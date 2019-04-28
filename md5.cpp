#include <iostream>
#include <iomanip>

using namespace std;

#define A 0x67452301
#define B 0xEFCDAB89
#define C 0x98BADCFE
#define D 0x10325476

#define MAX_MOVEBITS 25

typedef unsigned char byte;

string info = "16711083";

void length2bin(byte *infoLength_64bits, unsigned long infoLength);
void info2bin(string info, byte* info_448bits, unsigned long infoLength);
void concatInfoLength(byte* info_512bits, byte* info_448bits, byte* infoLength_64bits);
void f_func(byte* rst, byte* x, byte* y, byte* z, int length);
void g_func(byte* rst, byte* x, byte* y, byte* z, int length);
void h_func(byte* rst, byte* x, byte* y, byte* z, int length);
void i_func(byte* rst, byte* x, byte* y, byte* z, int length);
void divide_16x32bits(byte rst[16][32], byte* info_512bits);
void cyclic_shift_left(byte* data, int move_bits, int length);
void ff_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t);
void gg_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t);
void hh_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t);
void ii_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t);
void getMD5(unsigned long md5[4], byte data[16][32]);


int main()
{
	byte infoLength_64bits[64];
	byte info_448bits[448];
	byte info_512bits[512];
	byte info_16x32bits[16][32];
	unsigned long infoLength = unsigned long(8 * info.length());
	unsigned long md5[4];

	length2bin(infoLength_64bits, infoLength);
	info2bin(info, info_448bits, infoLength);
	concatInfoLength(info_512bits, info_448bits, infoLength_64bits);
	divide_16x32bits(info_16x32bits, info_512bits);
	getMD5(md5, info_16x32bits);


	cout << "Original info: " << info << endl;
	cout << "MD5 value: " << setw(8) << setfill('0') << hex << md5[0] << "-" << setw(8) << setfill('0') << hex;
	cout << setw(8) << setfill('0') << md5[1] << "-" << setw(8) << setfill('0') << hex << md5[2] << "-" << hex << md5[3] << endl;
}

void length2bin(byte* infoLength_64bits, unsigned long infoLength)
{
	for (int i = 0; i < 32; i++) infoLength_64bits[i] = (infoLength >> i) & 0x00000001;
	for (int i = 32; i < 64; i++) infoLength_64bits[i] = 0;
}

void info2bin(string info, byte* info_448bits, unsigned long infoLength)
{
	for (int i = 0; i < info.length(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			info_448bits[i * 8 + j] = (byte(info[i]) >> j) & 0x01;
		}
	}
	info_448bits[infoLength] = 1;
	for (int i = infoLength + 1; i < 448; i++) info_448bits[i] = 0;
}

void concatInfoLength(byte* info_512bits, byte* info_448bits, byte* infoLength_64bits)
{
	for (int i = 0; i < 448; i++) info_512bits[i] = info_448bits[i];
	for (int i = 0; i < 64; i++) info_512bits[448 + i] = infoLength_64bits[i];
}

byte do_not(byte x)
{
	if (x > 0) return 0;
	else return 1;
}

// F(X,Y,Z) =(X&Y)|((~X)&Z)
void f_func(byte* rst, byte* x, byte* y, byte* z, int length)
{
	for (int i = 0; i < length; i++)
	{
		rst[i] = (x[i] & y[i]) | (do_not(x[i]) & z[i]);
	}
}

// G(X,Y,Z) =(X&Z)|(Y&(~Z))
void g_func(byte* rst, byte* x, byte* y, byte* z, int length)
{
	for (int i = 0; i < length; i++)
	{
		rst[i] = (x[i] & z[i]) | (do_not(z[i]) & y[i]);
	}
}

// H(X,Y,Z) =X^Y^Z
void h_func(byte* rst, byte* x, byte* y, byte* z, int length)
{
	for (int i = 0; i < length; i++)
	{
		rst[i] = x[i] & y[i] & z[i];
	}
}

// I(X,Y,Z)=Y^(X|(~Z))
void i_func(byte* rst, byte* x, byte* y, byte* z, int length)
{
	for (int i = 0; i < length; i++)
	{
		rst[i] = y[i] & (x[i] | do_not(z[i]));
	}
}

void divide_16x32bits(byte rst[16][32], byte* info_512bits)
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			rst[i][j] = info_512bits[i * 16 + j];
		}
	}
}

void cyclic_shift_left(byte* data, int move_bits, int length)
{
	byte temp[MAX_MOVEBITS];

	for (int i = 0; i < MAX_MOVEBITS; i++) temp[i] = data[i];
	for (int i = 0; i < length - move_bits; i++) data[i] = data[i + move_bits];
	for (int i = length - move_bits; i < length; i++) data[i] = temp[i - length + move_bits];
}

void long2bin(byte* rst, unsigned long x)
{
	for (int i = 0; i < 32; i++) rst[i] = (x >> i) & 0x00000001;
}

void add_2x32bits(byte* rst, byte* a, byte* b)
{
	byte temp = 0;

	for (int i = 0; i < 32; i++)
	{
		temp = temp + a[i] + b[i];
		rst[i] = temp % 2;
		temp /= 2;
	}
}

void add_4x32bits(byte* rst, byte* a, byte* b, byte* c, byte* d)
{
	byte temp1[32], temp2[32];

	add_2x32bits(temp1, a, b);
	add_2x32bits(temp2, c, d);
	add_2x32bits(rst, temp1, temp2);
}

void ff_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t)
{
	byte temp[32], t_32bits[32], temp_add4[32];

	f_func(temp, b_32bits, c_32bits, d_32bits, 32);
	long2bin(t_32bits, t);
	add_4x32bits(temp_add4, a_32bits, temp, m_32bits, t_32bits);
	cyclic_shift_left(temp_add4, s, 32);
	add_2x32bits(a_32bits, temp_add4, b_32bits);
}

void gg_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t)
{
	byte temp[32], t_32bits[32], temp_add4[32];

	g_func(temp, b_32bits, c_32bits, d_32bits, 32);
	long2bin(t_32bits, t);
	add_4x32bits(temp_add4, a_32bits, temp, m_32bits, t_32bits);
	cyclic_shift_left(temp_add4, s, 32);
	add_2x32bits(a_32bits, temp_add4, b_32bits);
}

void hh_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t)
{
	byte temp[32], t_32bits[32], temp_add4[32];

	h_func(temp, b_32bits, c_32bits, d_32bits, 32);
	long2bin(t_32bits, t);
	add_4x32bits(temp_add4, a_32bits, temp, m_32bits, t_32bits);
	cyclic_shift_left(temp_add4, s, 32);
	add_2x32bits(a_32bits, temp_add4, b_32bits);
}

void ii_func(byte* a_32bits, byte* b_32bits, byte* c_32bits, byte* d_32bits, byte* m_32bits, int s, unsigned long t)
{
	byte temp[32], t_32bits[32], temp_add4[32];

	i_func(temp, b_32bits, c_32bits, d_32bits, 32);
	long2bin(t_32bits, t);
	add_4x32bits(temp_add4, a_32bits, temp, m_32bits, t_32bits);
	cyclic_shift_left(temp_add4, s, 32);
	add_2x32bits(a_32bits, temp_add4, b_32bits);
}

void getMD5(unsigned long md5[4], byte data[16][32])
{
	byte a_32bits[32], b_32bits[32], c_32bits[32], d_32bits[32];
	byte A_32bits[32], B_32bits[32], C_32bits[32], D_32bits[32];
	unsigned long temp;
	

	long2bin(a_32bits, unsigned long(A));
	long2bin(b_32bits, unsigned long(B));
	long2bin(c_32bits, unsigned long(C));
	long2bin(d_32bits, unsigned long(D));

	ff_func(a_32bits, b_32bits, c_32bits, d_32bits, data[0], 7, unsigned long(0xd76aa478));
	ff_func(d_32bits, a_32bits, b_32bits, c_32bits, data[1], 12, unsigned long(0xe8c7b756));
	ff_func(c_32bits, d_32bits, a_32bits, b_32bits, data[2], 17, unsigned long(0x242070db));
	ff_func(b_32bits, c_32bits, d_32bits, a_32bits, data[3], 22, unsigned long(0xc1bdceee));
	ff_func(a_32bits, b_32bits, c_32bits, d_32bits, data[4], 7, unsigned long(0xf57c0faf));
	ff_func(d_32bits, a_32bits, b_32bits, c_32bits, data[5], 12, unsigned long(0x4787c62a));
	ff_func(c_32bits, d_32bits, a_32bits, b_32bits, data[6], 17, unsigned long(0xa8304613));
	ff_func(b_32bits, c_32bits, d_32bits, a_32bits, data[7], 22, unsigned long(0xfd469501));
	ff_func(a_32bits, b_32bits, c_32bits, d_32bits, data[8], 7, unsigned long(0x698098d8));
	ff_func(d_32bits, a_32bits, b_32bits, c_32bits, data[9], 12, unsigned long(0x8b44f7af));
	ff_func(c_32bits, d_32bits, a_32bits, b_32bits, data[10], 17, unsigned long(0xffff5bb1));
	ff_func(b_32bits, c_32bits, d_32bits, a_32bits, data[11], 22, unsigned long(0x895cd7be));
	ff_func(a_32bits, b_32bits, c_32bits, d_32bits, data[12], 7, unsigned long(0x6b901122));
	ff_func(d_32bits, a_32bits, b_32bits, c_32bits, data[13], 12, unsigned long(0xfd987193));
	ff_func(c_32bits, d_32bits, a_32bits, b_32bits, data[14], 17, unsigned long(0xa679438e));
	ff_func(b_32bits, c_32bits, d_32bits, a_32bits, data[15], 22, unsigned long(0x49b40821));
													
	gg_func(a_32bits, b_32bits, c_32bits, d_32bits, data[1], 5, unsigned long(0xf61e2562));
	gg_func(d_32bits, a_32bits, b_32bits, c_32bits, data[6], 9, unsigned long(0xc040b340));
	gg_func(c_32bits, d_32bits, a_32bits, b_32bits, data[11], 14, unsigned long(0x265e5a51));
	gg_func(b_32bits, c_32bits, d_32bits, a_32bits, data[0], 20, unsigned long(0xe9b6c7aa));
	gg_func(a_32bits, b_32bits, c_32bits, d_32bits, data[5], 5, unsigned long(0xd62f105d));
	gg_func(d_32bits, a_32bits, b_32bits, c_32bits, data[10], 9, unsigned long(0x02441453));
	gg_func(c_32bits, d_32bits, a_32bits, b_32bits, data[15], 14, unsigned long(0xd8a1e681));
	gg_func(b_32bits, c_32bits, d_32bits, a_32bits, data[4], 20, unsigned long(0xe7d3fbc8));
	gg_func(a_32bits, b_32bits, c_32bits, d_32bits, data[9], 5, unsigned long(0x21e1cde6));
	gg_func(d_32bits, a_32bits, b_32bits, c_32bits, data[14], 9, unsigned long(0xc33707d6));
	gg_func(c_32bits, d_32bits, a_32bits, b_32bits, data[3], 14, unsigned long(0xf4d50d87));
	gg_func(b_32bits, c_32bits, d_32bits, a_32bits, data[8], 20, unsigned long(0x455a14ed));
	gg_func(a_32bits, b_32bits, c_32bits, d_32bits, data[13], 5, unsigned long(0xa9e3e905));
	gg_func(d_32bits, a_32bits, b_32bits, c_32bits, data[2], 9, unsigned long(0xfcefa3f8));
	gg_func(c_32bits, d_32bits, a_32bits, b_32bits, data[7], 14, unsigned long(0x676f02d9));
	gg_func(b_32bits, c_32bits, d_32bits, a_32bits, data[12], 20, unsigned long(0x8d2a4c8a));
													
	hh_func(a_32bits, b_32bits, c_32bits, d_32bits, data[5], 4, unsigned long(0xfffa3942));
	hh_func(d_32bits, a_32bits, b_32bits, c_32bits, data[8], 11, unsigned long(0x8771f681));
	hh_func(c_32bits, d_32bits, a_32bits, b_32bits, data[11], 16, unsigned long(0x6d9d6122));
	hh_func(b_32bits, c_32bits, d_32bits, a_32bits, data[14], 23, unsigned long(0xfde5380c));
	hh_func(a_32bits, b_32bits, c_32bits, d_32bits, data[1], 4, unsigned long(0xa4beea44));
	hh_func(d_32bits, a_32bits, b_32bits, c_32bits, data[4], 11, unsigned long(0x4bdecfa9));
	hh_func(c_32bits, d_32bits, a_32bits, b_32bits, data[7], 16, unsigned long(0xf6bb4b60));
	hh_func(b_32bits, c_32bits, d_32bits, a_32bits, data[10], 23, unsigned long(0xbebfbc70));
	hh_func(a_32bits, b_32bits, c_32bits, d_32bits, data[13], 4, unsigned long(0x289b7ec6));
	hh_func(d_32bits, a_32bits, b_32bits, c_32bits, data[0], 11, unsigned long(0xeaa127fa));
	hh_func(c_32bits, d_32bits, a_32bits, b_32bits, data[3], 16, unsigned long(0xd4ef3085));
	hh_func(b_32bits, c_32bits, d_32bits, a_32bits, data[6], 23, unsigned long(0x04881d05));
	hh_func(a_32bits, b_32bits, c_32bits, d_32bits, data[9], 4, unsigned long(0xd9d4d039));
	hh_func(d_32bits, a_32bits, b_32bits, c_32bits, data[12], 11, unsigned long(0xe6db99e5));
	hh_func(c_32bits, d_32bits, a_32bits, b_32bits, data[15], 16, unsigned long(0x1fa27cf8));
	hh_func(b_32bits, c_32bits, d_32bits, a_32bits, data[2], 23, unsigned long(0xc4ac5665));
													
	ii_func(a_32bits, b_32bits, c_32bits, d_32bits, data[0], 6, unsigned long(0xf4292244));
	ii_func(d_32bits, a_32bits, b_32bits, c_32bits, data[7], 10, unsigned long(0x432aff97));
	ii_func(c_32bits, d_32bits, a_32bits, b_32bits, data[14], 15, unsigned long(0xab9423a7));
	ii_func(b_32bits, c_32bits, d_32bits, a_32bits, data[5], 21, unsigned long(0xfc93a039));
	ii_func(a_32bits, b_32bits, c_32bits, d_32bits, data[12], 6, unsigned long(0x655b59c3));
	ii_func(d_32bits, a_32bits, b_32bits, c_32bits, data[3], 10, unsigned long(0x8f0ccc92));
	ii_func(c_32bits, d_32bits, a_32bits, b_32bits, data[10], 15, unsigned long(0xffeff47d));
	ii_func(b_32bits, c_32bits, d_32bits, a_32bits, data[1], 21, unsigned long(0x85845dd1));
	ii_func(a_32bits, b_32bits, c_32bits, d_32bits, data[8], 6, unsigned long(0x6fa87e4f));
	ii_func(d_32bits, a_32bits, b_32bits, c_32bits, data[15], 10, unsigned long(0xfe2ce6e0));
	ii_func(c_32bits, d_32bits, a_32bits, b_32bits, data[6], 15, unsigned long(0xa3014314));
	ii_func(b_32bits, c_32bits, d_32bits, a_32bits, data[13], 21, unsigned long(0x4e0811a1));
	ii_func(a_32bits, b_32bits, c_32bits, d_32bits, data[4], 6, unsigned long(0xf7537e82));
	ii_func(d_32bits, a_32bits, b_32bits, c_32bits, data[11], 10, unsigned long(0xbd3af235));
	ii_func(c_32bits, d_32bits, a_32bits, b_32bits, data[2], 15, unsigned long(0x2ad7d2bb));
	ii_func(b_32bits, c_32bits, d_32bits, a_32bits, data[9], 21, unsigned long(0xeb86d391));

	long2bin(A_32bits, unsigned long(A));
	long2bin(B_32bits, unsigned long(B));
	long2bin(C_32bits, unsigned long(C));
	long2bin(D_32bits, unsigned long(D));
	add_2x32bits(a_32bits, a_32bits, A_32bits);
	add_2x32bits(b_32bits, b_32bits, B_32bits);
	add_2x32bits(c_32bits, c_32bits, C_32bits);
	add_2x32bits(d_32bits, d_32bits, D_32bits);

	temp = 0;
	for (int i = 0; i < 32; i++) temp |= (unsigned long(a_32bits[i]) << i);
	md5[0] = temp;
	temp = 0;
	for (int i = 0; i < 32; i++) temp |= (unsigned long(b_32bits[i]) << i);
	md5[1] = temp;
	temp = 0;
	for (int i = 0; i < 32; i++) temp |= (unsigned long(c_32bits[i]) << i);
	md5[2] = temp;
	temp = 0;
	for (int i = 0; i < 32; i++) temp |= (unsigned long(d_32bits[i]) << i);
	md5[3] = temp;
}
