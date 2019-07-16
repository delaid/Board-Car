#ifndef __POWER_H
#define __POWER_H

#include "sys.h"

//#define BAT_CHARGE 1
//#define BAT_GOOD  2
//#define BAT_LOW	  3	

void Power_task_Create(void);								//电源管理任务函数
extern volatile bool socket_swtich;
extern  volatile bool power_low;
extern volatile uint16_t m_bat_volt;
extern volatile bool power_off;
#endif

