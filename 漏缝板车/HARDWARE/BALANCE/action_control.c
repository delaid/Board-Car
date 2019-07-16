#include "action_control.h"


#define ANGLEMODE		0x00	//�Ƕȿ���ģʽѡ��/0x00:ԭ��ת/0x01:������ǰת/0x02:�������ת

static void Move_Distance(short distance);
static void Stay_Time(short time);
static void Angle_Control(short angle_set,u8 mode);
static void Move_Distance(short distance);
static void Turn_Angle(short angle_set);



/*********************************************************************************************************
** Function name:       action_control
** Descriptions:        ����С���������ƣ�֧��������������������ִ��˳��Ƕ�+�ƶ�����+ͣ��ʱ�䣬����������ɺ�
                        
** input parameters:    *angle_set���Ƕ�
						*distance������
						*stay_time��ͣ��ʱ��
						number�����鳤��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void action_control(short *angle_set,short *distance,u16 *stay_time,u32 number)
{
	static bool  Flag;
	static short Num;
	static short i = 0;
	
	if(number<=0)		return ;
	//��һ�ν���ִ��һ��
	if(Flag == 0)
	{
		Num = number;		// �������
		Flag = 1;
	}
	
	if(Control_flag.Much_Action == 0)
	{
		if(Control_flag.SingleAction == 0)
		{
			Single_Action(angle_set[i],distance[i],stay_time[i]);
		}
		else {
			Control_flag.SingleAction  = 0;
			Num--;
			i++;
		}
	}
	else
		Num = 0;
	if(Num == 0)	{Flag = 0;i = 0;Control_flag.Much_Action = 1;}
}


/*********************************************************************************************************
** Function name:       Single_Action
** Descriptions:        ����С�������������ƣ�����ִ��˳��Ƕ�+�ƶ�����+ͣ��ʱ�䣬����������ɺ�
                        bool����Flag_SingleAction��1.
** input parameters:    angle_set���Ƕ�
						distance������
						stay_time��ͣ��ʱ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Single_Action_mode(short angle_set,short distance,short stay_time,u8 angle_control_mode)
{
	if(Control_flag.SingleAction  == 0)
	{
		if(Control_flag.Angle == 0)
		{
			Angle_Control(angle_set,angle_control_mode);
	   
		}
		else if(Control_flag.Distance == 0)
			 {    
				 Move_Distance(distance);
			 }
			 else if(Control_flag.Stop_Time == 0)
				  {
					  Stay_Time(stay_time);
				  }
				  else 
					{
					  Control_flag.Angle    = 0;
					  Control_flag.Distance = 0;
					  Control_flag.Stop_Time     = 0;
					  Control_flag.SingleAction  = 1;
				  }
	}
}



/*********************************************************************************************************
** Function name:       Single_Action
** Descriptions:        ����С�������������ƣ�����ִ��˳��Ƕ�+�ƶ�����+ͣ��ʱ�䣬����������ɺ�
                        bool����Flag_SingleAction��1.
** input parameters:    angle_set���Ƕ�
						distance������
						stay_time��ͣ��ʱ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Single_Action(short angle_set,short distance,short stay_time)
{
	if(Control_flag.SingleAction  == 0)
	{
		if(Control_flag.Angle == 0)
		{
			Angle_Control(angle_set,ANGLEMODE);
	   
		}
		else if(Control_flag.Distance == 0)
			 {    
				 Move_Distance(distance);
			 }
			 else if(Control_flag.Stop_Time == 0)
				  {
					  Stay_Time(stay_time);
				  }
				  else 
					{
					  Control_flag.Angle    = 0;
					  Control_flag.Distance = 0; 
					  Control_flag.Stop_Time     = 0;
					  Control_flag.SingleAction  = 1;
				  }
	}
}


//0x00:ԭ��ת
//0x01:������ǰת
//0x02:�������ת
//extern int Angle_Pwm;
/*********************************************************************************************************
** Function name:       Angle_Control
** Descriptions:        ����С�������Ƕȿ���
** input parameters:    angle_set���Ƕ�
						mode��ģʽ��0����ǰ�޺�1����ǰ�޺�2����ǰ�к�
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void Angle_Control(short angle_set,u8 mode)
{
	switch(mode)
	{
		case 0: 
			Control_flag.Qian = 0;Control_flag.Hou = 0;
			break;
		case 1: 
			Control_flag.Qian = 1;
			break;
		case 2: 
			Control_flag.Hou  = 1;
			break;
		default:
			return ;
	}
	Turn_Angle(angle_set);
}


volatile float ActionControl_TurnAngle;		// ���ٶȻ��ֳ����ĽǶ�
/*********************************************************************************************************
** Function name:       Turn_Angle
** Descriptions:        ת���Ƕȣ�ת����־���ƣ�
** input parameters:    angle_set���Ƕ�
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void Turn_Angle(short angle_set)
{
		
	if(Control_flag.Angle)		return ;						// Flag_Angle = 1 �򲻿ɽ��нǶȿ���
	
	if(angle_set>0)		Control_flag.Left  = 1;					// ����Ƕȴ���0��������ת����ת��־��1
	if(angle_set<0)		Control_flag.Right = 1;					// ����Ƕ�С��0��������ת����ת��־��1
	if(angle_set==0)	{Control_flag.Angle = 1;return ;}		// ����Ƕȵ���0����������
	
//	ActionControl_TurnAngle += (Gyro_Turn)*0.0610f/100;			//===���ٶȻ��֣��ۼӣ��õ��Ƕ�	
	ActionControl_TurnAngle += (((( Encoder_Right - Encoder_Left )/2112.0f)*32.5f)/87.0f)*360;
	
	if(angle_set>0)
	{
		if(ActionControl_TurnAngle>(angle_set-1))
		{
			Control_flag.Angle = 1;
			ActionControl_TurnAngle = 0;
			Control_flag.Left  = 0;
			Car_Brake() ;
		}
	}
	
	if(angle_set<0)
	{
		if(ActionControl_TurnAngle<(angle_set+1))
		{
			Control_flag.Angle = 1;
			ActionControl_TurnAngle = 0;
			Control_flag.Right = 0;
			Car_Brake();
		}
	}	
}





volatile int ActionControl_MoveDistance;

/*********************************************************************************************************
** Function name:       Move_Distance
** Descriptions:        �ƶ����루ǰ���־���ƣ�
** input parameters:    distance������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void Move_Distance(short distance)
{
	
	
	
	if(distance>0)		{Control_flag.Qian = 1;Control_flag.Hou  = 0;}
	if(distance<0)		{Control_flag.Qian = 0;Control_flag.Hou  = 1;}
	if(distance==0)		{Control_flag.Distance = 1;return ;}
	
	 ActionControl_MoveDistance += (Encoder_Left+Encoder_Right)/(2112*2.0f)*207.34f;
	
	if(distance>0)
	{
		if(ActionControl_MoveDistance>=distance)
		{
			Control_flag.Distance = 1;
			ActionControl_MoveDistance =0;
			Control_flag.Qian = 0;
			Car_Brake();
		}
	}
	if(distance<0)
	{
		if(ActionControl_MoveDistance<=distance)
		{
			Control_flag.Distance = 1;
			ActionControl_MoveDistance =0;
			Control_flag.Hou = 0;
			Car_Brake();
		}
	}
	
}


volatile  short ActionControl_StayTime;
/*********************************************************************************************************
** Function name:       Stay_Time
** Descriptions:        ͣ��ʱ��
** input parameters:    time��ʱ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void Stay_Time(short time)
{
	static bool  Flag;
	
	if(Control_flag.Stop_Time)		return ;
	if(time == 0)		{Control_flag.Stop_Time = 1,Flag = 0;return ;}
	//��һ�ν���ִ��һ��
	if(Flag == 0)
	{
		ActionControl_StayTime = time*10;		// ��ȡ��ʱ��ֵ
		Flag = 1;
	}
	if(ActionControl_StayTime>0)
	ActionControl_StayTime--;
	
	if(ActionControl_StayTime == 0)	Control_flag.Stop_Time = 1,Flag = 0;
	
}


/*********************************************************************************************************
** Function name:       Car_Brake
** Descriptions:        ��ɲ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
 void Car_Brake(void)
{

//	PWM_LFFT_FORWARD   = 3000;
//	PWM_LEFT_BACKWARD  = 3000;
//	PWM_RIGHT_FORWARD  = 3000;
//	PWM_RIGHT_BACKWARD = 3000;
}





















