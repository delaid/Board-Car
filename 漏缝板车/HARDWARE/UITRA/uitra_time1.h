#ifndef __UITRA_TIME1_H
#define __UITRA_TIME1_H
#include "sys.h"


#define TRIG		PCout(9)		// PC9


/*********************************************************************************************************
** Function name:       Read_Distane
** Descriptions:        目标板（STM32F103VET6）超声波接收回波函数.
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Read_Distance(void);
/*********************************************************************************************************
** Function name:       Uitra_Init
** Descriptions:        目标板（STM32F103VET6）超声波初始化
** input parameters:    arr：自动重装载值.
                        psc：时钟预分频数.
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Uitra_Init(u16 arr,u16 psc);

extern volatile u32 Distance;



#endif
