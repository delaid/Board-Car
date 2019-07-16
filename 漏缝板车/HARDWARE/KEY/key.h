#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 	 
#define CLOSE_BLUE  PAout(15)=0
#define OPEN_BLUE   PAout(15)=1

#define KEY_LEFT    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)//读取按键0
#define KEY_RIGHT   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)//读取按键1
#define KEY_BACK    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//读取按键2 
#define KEY_MOD     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)//读取按键3(WK_UP) 
 

#define KEY_LEFT_PRES 	1	//KEY_LEFT按下
#define KEY_RIGHT_PRES	2	//KEY_RIGHT按下
#define KEY_MOD_PRES	3	//KEY_MOD按下
#define KEY_BACK_PRES   4	//KEY_BACK按下(即WK_UP/KEY_UP)
#define KEY_FORWARD_PRES 5	//KEY_FORWARD 按下
#define KEY_FOUNCTION_PRES 6 //长按后背 切换功能
#define KEY_NONE	0

#define KEY_LIGHT_ADD	7
#define KEY_LIGHT_DECRE	8

void KEY_Init(void);//IO初始化
u8 key_scan(u8 mode);


#endif
