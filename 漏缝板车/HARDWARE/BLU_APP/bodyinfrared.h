#ifndef __BODYINFRARED_H
#define __BODYINFRARED_H
#include "sys.h"


#define bodydetection   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)//读取引脚PD0
extern bool Mode_Select_Flag;
/*********************************************************************************************************
** Function name:       Bodyinfrared_Init
** Descriptions:        初始化人体红外检测模块的检测引脚
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Bodyinfrared_Init(void);

/*********************************************************************************************************
** Function name:       BodyDetection
** Descriptions:        人体检测
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void BodyDetection(void);
/*********************************************************************************************************
** Function name:       BodyDetection_Sleep
** Descriptions:        人体检测,长时间没有人时候要关机
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void BodyDetection_Sleep(void);
#endif

