#ifndef __PID_H
#define __PID_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"



//RF遥控相关的变量
typedef struct contorl_flag_type
{
	bool Stop;		// 停止标志
	bool Qian;		// 前进标志
	bool Hou;		// 后退标志
	bool Left;		// 左转标志
	bool Right;		// 右转标志
	bool mode;
	
}contorl_flag_type;

extern int Incremental_PI_Left (int Encoder,int Target);

extern int Incremental_PI_Right (int Encoder,int Target);

/* Exported functions ------------------------------------------------------- */
//整数的绝对值
int myabs(int a);
#endif







