#ifndef __UITRA_TIME1_H
#define __UITRA_TIME1_H
#include "sys.h"


#define TRIG		PCout(9)		// PC9


/*********************************************************************************************************
** Function name:       Read_Distane
** Descriptions:        Ŀ��壨STM32F103VET6�����������ջز�����.
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Read_Distance(void);
/*********************************************************************************************************
** Function name:       Uitra_Init
** Descriptions:        Ŀ��壨STM32F103VET6����������ʼ��
** input parameters:    arr���Զ���װ��ֵ.
                        psc��ʱ��Ԥ��Ƶ��.
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uitra_Init(u16 arr,u16 psc);

extern volatile u32 Distance;



#endif
