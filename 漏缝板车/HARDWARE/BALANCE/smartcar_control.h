#ifndef __SMARTCAR_CONTROL_H
#define __SMARTCAR_CONTROL_H
/* Includes ------------------------------------------------------------------*/
#include "sys.h"	

void SmartCar_control(void);
void Set_Pwm(int motor1,int motor2);
extern void Get_Angle(u8 way);
extern void SmartCar_control_init(void);

#endif
