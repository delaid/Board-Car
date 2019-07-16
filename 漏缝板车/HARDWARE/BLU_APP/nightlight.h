#ifndef __NIGHTLIGHT_H
#define __NIGHTLIGHT_H

#include "sys.h"

#define OPEN_LIGHT 1
#define CLOSE_LIGHT 2
#define OPEN_BODY_IR 3
#define CLOSE_BODY_IR 4


typedef struct nightlight_type
{
	 u8 mode;//灯模式
	 u8 brightness;//亮度
	 u16 time;

} nightlight_type;


extern nightlight_type m_nightlight;







/*********************************************************************************************************
** Function name:       NightLight_Mode
** Descriptions:        夜光模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void NightLight_Mode(void);


/*********************************************************************************************************
** Function name:       Set_NightLight
** Descriptions:        设置夜光模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_NightLight(void);



#endif
