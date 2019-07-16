#ifndef __NIGHTLIGHT_H
#define __NIGHTLIGHT_H

#include "sys.h"

#define OPEN_LIGHT 1
#define CLOSE_LIGHT 2
#define OPEN_BODY_IR 3
#define CLOSE_BODY_IR 4


typedef struct nightlight_type
{
	 u8 mode;//��ģʽ
	 u8 brightness;//����
	 u16 time;

} nightlight_type;


extern nightlight_type m_nightlight;







/*********************************************************************************************************
** Function name:       NightLight_Mode
** Descriptions:        ҹ��ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void NightLight_Mode(void);


/*********************************************************************************************************
** Function name:       Set_NightLight
** Descriptions:        ����ҹ��ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_NightLight(void);



#endif
