#ifndef __BODYINFRARED_H
#define __BODYINFRARED_H
#include "sys.h"


#define bodydetection   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)//��ȡ����PD0
extern bool Mode_Select_Flag;
/*********************************************************************************************************
** Function name:       Bodyinfrared_Init
** Descriptions:        ��ʼ�����������ģ��ļ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Bodyinfrared_Init(void);

/*********************************************************************************************************
** Function name:       BodyDetection
** Descriptions:        ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BodyDetection(void);
/*********************************************************************************************************
** Function name:       BodyDetection_Sleep
** Descriptions:        ������,��ʱ��û����ʱ��Ҫ�ػ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BodyDetection_Sleep(void);
#endif

