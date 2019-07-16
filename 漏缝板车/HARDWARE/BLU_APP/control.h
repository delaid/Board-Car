#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define D_GESTUER_OFF 1
#define D_GESTUER_ON  2
#define D_FOLLOW_ON   3
#define D_FOLLOW_OFF  4
#define D_AVOID_ON    5
#define D_AVOID_OFF   6
#define D_AVOID_GO_ON	7
#define D_AVOID_GO_OFF	8
extern bool control_sleep;
extern volatile u8 m_Ulta_Gesture_Touch;
/* Direction definitions */
enum {
  Control_None,
  Control_Qian,
  Control_Hou,
  Control_Left,
  Control_Right,
  Control_Stop,
  Control_Standby
};

enum {
	Touch_None,
	Touch_Left,
	Touch_Right,
	Touch_Back,
	Touch_Front
};

extern volatile u8	Control_Touch;
extern volatile u8	Control_Touch_Update;
/*********************************************************************************************************
** Function name:       Control_Mode
** Descriptions:        ����ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Control_Mode(void);

/*********************************************************************************************************
** Function name:       Read_Control
** Descriptions:        ��ȡ��ǰ�Ŀ���״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Read_Control(void);

/*********************************************************************************************************
** Function name:       Control_Mode
** Descriptions:        �ڿ���ģʽ�¶�С�����п��Ʋ������������óɹ�����OK����������ʧ�ܷ���ERROR��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_Control(void);

/*********************************************************************************************************
** Function name:       Control_Tpad_Scan
** Descriptions:        ����ģʽ�µĴ���ɨ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Control_Tpad_Scan(void);

void Gesture_Switch(bool on_off);
#endif
