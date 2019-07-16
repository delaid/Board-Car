#ifndef __ANGLE_CALCULATION_H
#define __ANGLE_CALCULATION_H


#include "sys.h"	

void Call_Yaw(int encoder_left,int encoder_right);
void Angle_Calculation(int encoder_left,int encoder_right);

extern float Calculate_Angle;
extern float Mileage_MM;
#endif /* __ANGLE_CALCULATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
