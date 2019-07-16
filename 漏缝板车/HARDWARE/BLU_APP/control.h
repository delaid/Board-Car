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
** Descriptions:        控制模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Control_Mode(void);

/*********************************************************************************************************
** Function name:       Read_Control
** Descriptions:        读取当前的控制状态
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Read_Control(void);

/*********************************************************************************************************
** Function name:       Control_Mode
** Descriptions:        在控制模式下对小车进行控制操作，操作设置成功发送OK，操作设置失败发送ERROR。
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_Control(void);

/*********************************************************************************************************
** Function name:       Control_Tpad_Scan
** Descriptions:        控制模式下的触碰扫描
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Control_Tpad_Scan(void);

void Gesture_Switch(bool on_off);
#endif
