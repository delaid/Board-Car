#ifndef __SMAR_TIME_TASK_H
#define __SMAR_TIME_TASK_H
#include "sys.h"  

void smar_time_init(void);
extern volatile u32 fast_dance_time;
extern volatile u32 slow_dance_time;
extern volatile u32 Body_time;
extern int  Left_encoder_Target;
extern int  Right_encoder_Target;
#endif

