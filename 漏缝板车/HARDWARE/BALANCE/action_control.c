#include "action_control.h"


#define ANGLEMODE		0x00	//角度控制模式选择/0x00:原地转/0x01:单脚向前转/0x02:单脚向后转

static void Move_Distance(short distance);
static void Stay_Time(short time);
static void Angle_Control(short angle_set,u8 mode);
static void Move_Distance(short distance);
static void Turn_Angle(short angle_set);



/*********************************************************************************************************
** Function name:       action_control
** Descriptions:        智能小车动作控制，支持连续动作操作，动作执行顺序角度+移动距离+停顿时间，连续动作完成后，
                        
** input parameters:    *angle_set：角度
						*distance：距离
						*stay_time：停留时间
						number：数组长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void action_control(short *angle_set,short *distance,u16 *stay_time,u32 number)
{
	static bool  Flag;
	static short Num;
	static short i = 0;
	
	if(number<=0)		return ;
	//第一次进来执行一次
	if(Flag == 0)
	{
		Num = number;		// 数组个数
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
** Descriptions:        智能小车单个动作控制，动作执行顺序角度+移动距离+停顿时间，单个动作完成后，
                        bool变量Flag_SingleAction置1.
** input parameters:    angle_set：角度
						distance：距离
						stay_time：停留时间
** output parameters:   无
** Returned value:      无
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
** Descriptions:        智能小车单个动作控制，动作执行顺序角度+移动距离+停顿时间，单个动作完成后，
                        bool变量Flag_SingleAction置1.
** input parameters:    angle_set：角度
						distance：距离
						stay_time：停留时间
** output parameters:   无
** Returned value:      无
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


//0x00:原地转
//0x01:单脚向前转
//0x02:单脚向后转
//extern int Angle_Pwm;
/*********************************************************************************************************
** Function name:       Angle_Control
** Descriptions:        智能小车单个角度控制
** input parameters:    angle_set：角度
						mode：模式（0：无前无后；1：有前无后；2：无前有后）
** output parameters:   无
** Returned value:      无
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


volatile float ActionControl_TurnAngle;		// 角速度积分出来的角度
/*********************************************************************************************************
** Function name:       Turn_Angle
** Descriptions:        转动角度（转动标志控制）
** input parameters:    angle_set：角度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void Turn_Angle(short angle_set)
{
		
	if(Control_flag.Angle)		return ;						// Flag_Angle = 1 则不可进行角度控制
	
	if(angle_set>0)		Control_flag.Left  = 1;					// 输入角度大于0，代表左转，左转标志置1
	if(angle_set<0)		Control_flag.Right = 1;					// 输入角度小于0，代表右转，右转标志置1
	if(angle_set==0)	{Control_flag.Angle = 1;return ;}		// 输入角度等于0，函数结束
	
//	ActionControl_TurnAngle += (Gyro_Turn)*0.0610f/100;			//===角速度积分（累加）得到角度	
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
** Descriptions:        移动距离（前后标志控制）
** input parameters:    distance：距离
** output parameters:   无
** Returned value:      无
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
** Descriptions:        停留时间
** input parameters:    time：时间
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void Stay_Time(short time)
{
	static bool  Flag;
	
	if(Control_flag.Stop_Time)		return ;
	if(time == 0)		{Control_flag.Stop_Time = 1,Flag = 0;return ;}
	//第一次进来执行一次
	if(Flag == 0)
	{
		ActionControl_StayTime = time*10;		// 获取延时数值
		Flag = 1;
	}
	if(ActionControl_StayTime>0)
	ActionControl_StayTime--;
	
	if(ActionControl_StayTime == 0)	Control_flag.Stop_Time = 1,Flag = 0;
	
}


/*********************************************************************************************************
** Function name:       Car_Brake
** Descriptions:        急刹
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void Car_Brake(void)
{

//	PWM_LFFT_FORWARD   = 3000;
//	PWM_LEFT_BACKWARD  = 3000;
//	PWM_RIGHT_FORWARD  = 3000;
//	PWM_RIGHT_BACKWARD = 3000;
}





















