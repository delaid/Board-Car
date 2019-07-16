#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 	 
#define CLOSE_BLUE  PAout(15)=0
#define OPEN_BLUE   PAout(15)=1

#define KEY_LEFT    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)//��ȡ����0
#define KEY_RIGHT   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)//��ȡ����1
#define KEY_BACK    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//��ȡ����2 
#define KEY_MOD     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)//��ȡ����3(WK_UP) 
 

#define KEY_LEFT_PRES 	1	//KEY_LEFT����
#define KEY_RIGHT_PRES	2	//KEY_RIGHT����
#define KEY_MOD_PRES	3	//KEY_MOD����
#define KEY_BACK_PRES   4	//KEY_BACK����(��WK_UP/KEY_UP)
#define KEY_FORWARD_PRES 5	//KEY_FORWARD ����
#define KEY_FOUNCTION_PRES 6 //������ �л�����
#define KEY_NONE	0

#define KEY_LIGHT_ADD	7
#define KEY_LIGHT_DECRE	8

void KEY_Init(void);//IO��ʼ��
u8 key_scan(u8 mode);


#endif
