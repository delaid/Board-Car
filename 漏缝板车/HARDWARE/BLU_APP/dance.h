#ifndef __DANCE_H
#define __DANCE_H
#include "sys.h"


#define DANCE_FAST 		0x01//快节奏舞蹈
#define DANCE_SLOW 		0x02//慢节奏舞蹈
#define DANCE_CUSTOM	0x03//自定义舞蹈
#define DANCE_STOP		0x04
#define DANCE_SHAKE		0x05//前后摇晃
#define SHAKEMAXE		0x05//摇晃最大幅度


extern volatile uint8_t Dance_Flag;
extern volatile uint8_t shakeparam;
void Dance_Clear(void);


void Dance_Mode(void);	//跳舞模式
void Set_Dance(void);	//设置跳舞模式状态
void Read_Dance(void);	//读取当前跳舞模式状态
void Custom_Dance(void);//自定义跳舞
void Fast_Dance(void);
void Slow_Dance(void);
void FrontBack_Shake(void);
uint8_t Dance_DP(void); //自定义跳舞数据处理 

#endif
/********************************************END_DANCE_H_FILE********************************************/
