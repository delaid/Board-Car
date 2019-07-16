#ifndef __ENCRYPT_H
#define __ENCRYPT_H
#include "sys.h"

/*****************************************************************************************************************************************
加密运算法则：				|运算法则1|				|运算法则2|				|运算法则3|				|运算法则4|				|运算法则5|
******************************************************************************************************************************************/
#define PASSWORD_01()	{Password[0] ^= 0xAB;	Password[0] >>= 0x03;	Password[0] *= 0x02;	Password[0] += 0x01;	Password[0] |= 0x01;}
#define PASSWORD_02()	{Password[1] <<= 0x03;	Password[1] ^= 0xCA;	Password[1] += 0x03;	Password[1] *= 0x10;	Password[1] -= 0x01;}
#define PASSWORD_03()	{Password[2] >>= 0x05;	Password[2] |= 0x9E;	Password[2] ^= 0x7A;	Password[2] += 0x11;	Password[2] *= 0x02;}
#define PASSWORD_04()	{Password[3] ^= 0x1B;	Password[3] += 0x45;	Password[3] ^= 0x7E;	Password[3] += 0x21;	Password[3] <<= 0x01;}
#define PASSWORD_05()	{Password[4] *= 0x13;	Password[4] += 0x1F;	Password[4] <<= 0x02;	Password[4] |= 0x01;	Password[4] ^= 0xF1;}
#define PASSWORD_06()	{Password[5] -= 0x34;	Password[5] ^= 0xBD;	Password[5] -= 0x51;	Password[5] *= 0x02;	Password[5] |= 0x01;}
#define PASSWORD_07()	{Password[6] += 0x10;	Password[6] &= 0xF1;	Password[6] ^= 0xAB;	Password[6] &= 0x3D;	Password[6] <<= 0x01;}
#define PASSWORD_08()	{Password[7] *= 0x12;	Password[7] <<= 0x01;	Password[7] += 0x1C;	Password[7] |= 0x13;	Password[7] >>= 0x02;}
#define PASSWORD_09()	{Password[8] ^= 0x12;	Password[8] += 0x21;	Password[8] ^= 0xED;	Password[8] *= 0x03;	Password[8] >>= 0x01;}
#define PASSWORD_10()	{Password[9] *= 0x09;	Password[9] <<= 0x02;	Password[9] &= 0xAB;	Password[9] |= 0x25;	Password[9] -= 0x01;}
#define PASSWORD_11()	{Password[10] |= 0x01;	Password[10] <<= 0x02;	Password[10] *= 0x03;	Password[10] += 0x01;	Password[10] <<= 0x01;}
#define PASSWORD_12()	{Password[11] ^= 0x13;	Password[11] += 0x15;	Password[11] &= 0xBC;	Password[11] <<= 0x01;	Password[11] |= 0x03;}


#define MCU_UID_BASE_ADD	((uint32_t)0x1FFFF7E8)
#define KEY_UID_BASE_ADD	((uint32_t)0x0800FFF0)

void Program_Protection(void);
#endif
