#ifndef __PID_H
#define __PID_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"



//RFң����صı���
typedef struct contorl_flag_type
{
	bool Stop;		// ֹͣ��־
	bool Qian;		// ǰ����־
	bool Hou;		// ���˱�־
	bool Left;		// ��ת��־
	bool Right;		// ��ת��־
	bool mode;
	
}contorl_flag_type;

extern int Incremental_PI_Left (int Encoder,int Target);

extern int Incremental_PI_Right (int Encoder,int Target);

/* Exported functions ------------------------------------------------------- */
//�����ľ���ֵ
int myabs(int a);
#endif







