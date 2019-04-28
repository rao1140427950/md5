#ifndef _MD5_H_
#define _MD5_H_

#include <string>
#include <cstring>

using namespace std;

typedef unsigned char uint8;
typedef unsigned long uint32;

const uint8 PADDING[64] = { 0x80 };
const char HEX_NUMBERS[16] = {
  '0', '1', '2', '3',
  '4', '5', '6', '7',
  '8', '9', 'a', 'b',
  'c', 'd', 'e', 'f'
};

#define s11 7
#define s12 12
#define s13 17
#define s14 22
#define s21 5
#define s22 9
#define s23 14
#define s24 20
#define s31 4
#define s32 11
#define s33 16
#define s34 23
#define s41 6
#define s42 10
#define s43 15
#define s44 21


#define A ((uint32)0x67452301)
#define B ((uint32)0xEFCDAB89)
#define C ((uint32)0x98BADCFE)
#define D ((uint32)0x10325476)

uint32 F(uint32 x, uint32 y, uint32 z);
uint32 G(uint32 x, uint32 y, uint32 z);
uint32 H(uint32 x, uint32 y, uint32 z);
uint32 I(uint32 x, uint32 y, uint32 z);

uint32 cyclic_shift_left(uint32 x, uint32 n);

uint32 FF(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
uint32 GG(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
uint32 HH(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);
uint32 II(uint32 a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac);

string getMD5(string message);

#endif // _MD5_H_