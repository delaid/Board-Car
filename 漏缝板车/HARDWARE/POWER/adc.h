#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
/**************************************************************************/
#define Battery_Ch  0
void Adc_Init(void);
u16 Get_Adc(u8 ch);
uint16_t Get_battery_volt(void); 
int Get_socket_volt(void);
#endif 

















