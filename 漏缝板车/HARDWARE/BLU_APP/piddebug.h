#ifndef __PIDDEBUG_H
#define __PIDDEBUG_H
#include "sys.h"



extern float PID_Buffer[6];

/*********************************************************************************************************
** Function name:       Set_PID
** Descriptions:        设置平衡小车PID参数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_PID(void);

#endif
