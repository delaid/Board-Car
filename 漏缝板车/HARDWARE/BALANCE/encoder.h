#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"	 

#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ
extern void Encoder_Init_TIM4(void);
extern void Encoder_Init_TIM5(void);

int Read_Encoder(u8 TIMX);
#endif
