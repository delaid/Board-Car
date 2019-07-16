/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ACTION_CONTROL_H
#define __ACTION_CONTROL_H

#include "sys.h"	

extern void Car_Brake(void);
//void Single_Action(short angle_set,short distance,short stay_time);
void Single_Action(short angle_set,short distance,short stay_time);
void action_control(short *angle_set,short *distance,u16 *stay_time,u32 number);
void Single_Action_mode(short angle_set,short distance,short stay_time,u8 angle_control_mode);
extern volatile float ActionControl_TurnAngle;
extern volatile int ActionControl_MoveDistance;
extern volatile  short ActionControl_StayTime;
#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
