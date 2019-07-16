#include "dance.h"

#define  DANCE_LOVELY 	 0x01
#define  DANCE_PRIDE	 0x02
#define  DANCE_SURPRISED 0x03
#define  DANCE_NEVERS1	 0x04
#define  DANCE_HAPPPY1	 0x05
#define  DANCE_ANGRY1	 0x06
#define  DANCE_NEVERS2	 0x07
#define  DANCE_HAPPPY2	 0x08
#define  DANCE_ANGRY2	 0x09
#define  DANCE_GARGLE	 0x10

volatile bool flag = 0;
volatile uint8_t  shakeparam = SHAKEMAXE;//摇晃幅度变量参数
volatile uint8_t  Dance_Flag = 0x04;	 //跳舞标志，用来区分舞蹈类型
volatile uint16_t Dance_BufLength=0;	 //记录自定义舞蹈数据长度
volatile uint16_t fast_step=0;
volatile uint16_t slow_step=0;
volatile uint16_t custom_step=0;
volatile uint16_t time_count=0;

const short    FastDanceData_angle[10]={  30, -60,  60, -30, 360,-360,  30, -60,  60, -30};
const short    FastDanceData_Move[10] ={   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};
const uint16_t FastDanceData_Time[10] ={   1,   1,   1,   1,   1,   1,   1,   1,   1,   1};
const uint8_t  FastDanceData_TurnV[10]={ 80, 80, 80, 80, 75, 75, 80, 80, 80, 80};
const uint8_t  FastDanceData_MoveV[10]={  25,  25,  25,  25,  25,  25,  25,  25,  25,  25};
const uint8_t  FastDanceData_Sound[10]={   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}; 

const short    SlowDanceData_angle[4] ={360,-360,-360,360};
const short    SlowDanceData_Move[4]  ={  0,   0,   0,  0};
const uint16_t SlowDanceData_Time[4]  ={  5,   5,   5,  5};
const uint8_t  SlowDanceData_TurnV[4] ={ 35,  35,  35, 35};
const uint8_t  SlowDanceData_MoveV[4] ={ 25,  25,  25, 25};
const uint8_t  SlowDanceData_Sound[4] ={  0,   0,   0,  0};
const uint8_t  SlowDanceData_Mode[4]  ={  1,   0,   1,  0};

static short    Dance_Angle[100] = {0};//自定义舞蹈——角度
static short    Dance_Move[100]  = {0};//自定义舞蹈——距离
static uint16_t Dance_Time[100]  = {0};//自定义舞蹈——时间
static uint8_t  Dance_TurnV[100] = {0};//自定义舞蹈——转速
static uint8_t  Dance_MoveV[100] = {0};//自定义舞蹈——速度
static uint8_t  Dance_Sound[100] = {0};//自定义舞蹈——音效




/*********************************************************************************************************
** Function name:       Dance_Mode
** Descriptions:        跳舞模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void Dance_Mode(void)
{
	if(DANCE == Car_Mode)
	{
		switch(Dance_Flag)
		{
			case DANCE_FAST:	Fast_Dance();break;
			case DANCE_SLOW:	Slow_Dance();break;
			case DANCE_CUSTOM:	Custom_Dance();break;
			case DANCE_STOP:	Control_flag.Qian =0;Control_flag.Hou =0;Control_flag.Left =0;Control_flag.Right =0;break;
			case DANCE_SHAKE:	FrontBack_Shake();break;
			default:
				break;
		}
	}
}

/*********************************************************************************************************
** Function name:       Dance_Clear
** Descriptions:        设置当前跳舞模式状态，如果是自定义跳舞，那么同时对数据进行处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Dance_Clear(void)
{
	sound_update= true;
	m_sound_type=STOP_SOUND;
	slow_dance_time=0;
	fast_dance_time=0;
	fast_step=0;
	slow_step=0;
	custom_step=0;
	
	//清零控制模式数据
	Control_flag.Qian   = 0;	// 前进标志
	Control_flag.Hou    = 0;	// 后退标志
	Control_flag.Left   = 0;	// 左转标志
	Control_flag.Right  = 0;	// 右转标志
	Control_flag.Stop   = 0;	// 停止标志
	Control_flag.TurnV  = 0;	// 转速标志
	Control_flag.Acce   = 0;	// 加速标志
	//清零互动模式数据
	ActionControl_TurnAngle = 0;	//在单个动作执行中旋转角度
	ActionControl_MoveDistance = 0;	//在单个动作执行中移动距离
	ActionControl_StayTime = 0;		//在单个动作执行中停留时间
	flag=0;

}
/*********************************************************************************************************
** Function name:       Set_Dance
** Descriptions:        设置当前跳舞模式状态，如果是自定义跳舞，那么同时对数据进行处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_Dance(void)
{
	//判断当前是否处于跳舞模式
	if(DANCE == Car_Mode)
	{	
		Dance_Clear();
		switch(USART3_RX_BUF[4])
		{
			case DANCE_FAST:  Dance_Flag = 0x01;Send_OK();break;//快节奏舞蹈
			case DANCE_SLOW:  Dance_Flag = 0x02;Send_OK();break;//慢节奏舞蹈
			case DANCE_CUSTOM:Dance_BufLength=Dance_DP();Dance_Flag = 0x03;Send_OK();break;//自定义舞蹈，同时对数据进行处理
			case DANCE_STOP:  Dance_Flag = 0x04;break;
			default:
				Send_ERROR();//接收到错误信息
				break;
		}
	}
}


/*********************************************************************************************************
** Function name:       Dance_DP
** Descriptions:        自定义跳舞数据处理  
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
u8 Dance_DP(void)
{
	u8 Dance_numb = 0;
	u8 n = 0;
	u8 i = 5;
	
	//清除之前的缓存数据
	short_clear_buf(Dance_Angle,0,Dance_BufLength);
	short_clear_buf(Dance_Move, 0,Dance_BufLength);
	short_clear_buf((short*)Dance_Time, 0,Dance_BufLength);
	u8_clear_buf(Dance_TurnV, 0,Dance_BufLength);
	u8_clear_buf(Dance_MoveV, 0,Dance_BufLength);
	u8_clear_buf(Dance_Sound, 0,Dance_BufLength);
	
	
	for(Dance_numb = ((USART3_RX_STA&0x00FF)-8);Dance_numb>0;)
	{
		Dance_Angle[n] = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];//跳舞数据
		i += 2;
		Dance_Move[n]  = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];//距离数据
		i += 2;
		Dance_Time[n]  = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];//时间数据
		i += 2;
		if(USART3_RX_BUF[i]<20)USART3_RX_BUF[i]=15;//增加启动速度，防止转不动
		Dance_TurnV[n] =  USART3_RX_BUF[i];//转速数据	
		i++;
		Dance_MoveV[n] =  USART3_RX_BUF[i];//速度数据
		i++;
		Dance_Sound[n] =  USART3_RX_BUF[i];//音效数据
		i++;
		n++;
		Dance_numb -= 9;
	}
	
	return n;
}


/*********************************************************************************************************
** Function name:       Custom_Dance
** Descriptions:        编程跳舞
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Custom_Dance(void)
{
	if(custom_step >= Dance_BufLength)
	{
		custom_step = 0;
		flag=0;
	}
	
	if(!flag)
	{
	
		//发送小鸡哔哔音效
	    switch (Dance_Sound[custom_step])
		{
			case	DANCE_LOVELY: sound_update= true; m_sound_type=BACK_LOVELY_SOUND;
				break;
			case	DANCE_PRIDE:sound_update= true; m_sound_type=BACK_PRIDE_SOUND;
				break;
			case    DANCE_SURPRISED:sound_update= true; m_sound_type=FORN_SURPRISED_SOUND;
				break;
			case	DANCE_NEVERS1:sound_update= true; m_sound_type=RIGHT_NEVERS_SOUND;
				break;
			case	DANCE_HAPPPY1:sound_update= true; m_sound_type=RIGHT_HAPPY_SOUND;
				break;
			case    DANCE_ANGRY1:sound_update= true; m_sound_type=RIGHT_ANGRY_SOUND;
				break;
			case	DANCE_NEVERS2:sound_update= true; m_sound_type=LEFT_NEVERS_SOUND;
				break;
			case	DANCE_HAPPPY2:sound_update= true; m_sound_type=LEFT_HAPPY_SOUND;
				break;
			case    DANCE_ANGRY2:sound_update= true; m_sound_type=LEFT_ANGERY_SOUND;
				break;
			case	DANCE_GARGLE:sound_update= true; m_sound_type=BACK_LOVELY_SOUND;
				break;
			default:
				break;
		}
		
		
		//设置转动速度
		Control_flag.TurnV = Dance_TurnV[custom_step];
		//设置移动速度
		Control_flag.Acce  = Dance_MoveV[custom_step];
		//每个动作只执行一次
		flag = 1;
	}
	
	//执行单个动作
	Single_Action(Dance_Angle[custom_step],Dance_Move[custom_step],Dance_Time[custom_step]);//单个动作
	
	//判断单个动作是否完成
	if(Control_flag.SingleAction)
	{
		//移动到下一个动作执行
		custom_step++;
		//单个动作完成标志清零
		Control_flag.SingleAction = 0;
		flag = 0;
	}
	
}

void Fast_Dance(void)
{
	
	if(fast_step >= 10)
	{
		fast_step = 0;
		flag = 0;
	}	
	
	if(fast_dance_time == 0)
	{
		sound_update	= 	true;
		m_sound_type	=	CHEICKET_SOUND;
		fast_dance_time=6500;
	}
	
	
		 static u8 Flag_Dance_fast_avoid=0;
		 if(Distance<90)
		 {
			 //清零控制模式数据
			Control_flag.Qian   = 0;	// 前进标志
			Control_flag.Hou    = 0;	// 后退标志
			Control_flag.Left   = 0;	// 左转标志
			Control_flag.Right  = 0;	// 右转标志
			Flag_Dance_fast_avoid=1;
		 }	
		if(Flag_Dance_fast_avoid == 1)
		 {
			Single_Action(0,-50,0);if(Control_flag.SingleAction){Flag_Dance_fast_avoid=2;Control_flag.SingleAction = 0;}
		 } 
		 else if(Flag_Dance_fast_avoid == 2)
		 {
			Single_Action(170,0,0);if(Control_flag.SingleAction){Flag_Dance_fast_avoid=0;Control_flag.SingleAction = 0;}
		 }
		 else 
		 {	
			Flag_Dance_fast_avoid =0;
		 }
	
	if(Flag_Dance_fast_avoid==0)
	{
		if(!flag)
		{
			//发送小鸡哔哔音效
			//设置转动速度
			Control_flag.TurnV = FastDanceData_TurnV[fast_step];
			//设置移动速度
			Control_flag.Acce  = FastDanceData_MoveV[fast_step];
			//每个动作只执行一次
			flag = 1;
		}
		
		//执行单个动作
		Single_Action(FastDanceData_angle[fast_step],FastDanceData_Move[fast_step],FastDanceData_Time[fast_step]);//单个动作
		
		//判断单个动作是否完成
		if(Control_flag.SingleAction)
		{
			//移动到下一个动作执行
			fast_step++;
			//单个动作完成标志清零
			Control_flag.SingleAction = 0;
			flag = 0;
		}
	}
	
}


void Slow_Dance(void)
{	
	if(slow_step >= 4)
	{
		slow_step = 0;
		flag = 0;
	}
	
	 if(slow_dance_time == 0)
	{
		sound_update	= 	true;
		m_sound_type	=	BLUE_RIVER_SOUND;
		slow_dance_time=6500;
	}
	
	 static u8 Flag_Dance_slow_avoid=0;
		 if(Distance<90)
		 {
			 //清零控制模式数据
			Control_flag.Qian   = 0;	// 前进标志
			Control_flag.Hou    = 0;	// 后退标志
			Control_flag.Left   = 0;	// 左转标志
			Control_flag.Right  = 0;	// 右转标志
			Flag_Dance_slow_avoid=1;
		 }	
		if(Flag_Dance_slow_avoid == 1)
		 {
			Single_Action(0,-50,0);if(Control_flag.SingleAction){Flag_Dance_slow_avoid=2;Control_flag.SingleAction = 0;}
		 } 
		 else if(Flag_Dance_slow_avoid == 2)
		 {
			Single_Action(170,0,0);if(Control_flag.SingleAction){Flag_Dance_slow_avoid=0;Control_flag.SingleAction = 0;}
		 }
		 else 
		 {	
			Flag_Dance_slow_avoid =0;
		 }
	
	if(Flag_Dance_slow_avoid==0)
	{
		if(!flag)
		{
			//发送小鸡哔哔音效
			
			//设置转动速度
			Control_flag.TurnV = SlowDanceData_TurnV[slow_step];
			//设置移动速度
			Control_flag.Acce  = SlowDanceData_MoveV[slow_step];
			//每个动作只执行一次
			flag = 1;
		}
		
		//执行单个动作
		Single_Action_mode(SlowDanceData_angle[slow_step],SlowDanceData_Move[slow_step],SlowDanceData_Time[slow_step],SlowDanceData_Mode[slow_step]);//单个动作
		
		//判断单个动作是否完成
		if(Control_flag.SingleAction)
		{
			//移动到下一个动作执行
			slow_step++;
			//单个动作完成标志清零
			Control_flag.SingleAction = 0;
			flag = 0;
		}
	}	
}

/*随机数返回值0到5*/
uint8_t Random_Numb(void)
{
	uint16_t numb;
	
	numb = rand();
	
	srand(numb);
	
	return numb%6;
}

/*摇晃*/
void FrontBack_Shake(void)
{
	static uint16_t delay;
	static uint8_t turn_flag;
	static uint8_t count_flag;
	static uint8_t shake_count;//摇晃次数
	static uint16_t count;
	
	
	/*第一次进来初始化内部参数*/
	if(time_count==0)
	{
		turn_flag  = 0;
		count_flag = 1;
		count = 0;
		shake_count=0;
		delay=50;
	}
	
	Random_Numb();
	
	if(time_count<300)
	{
		time_count++;
	}
	else{
		count++;
		switch(count_flag)
		{
			case 0x01:	BalancePoint = ZHONGZHI+shakeparam;if(count==50){count_flag=2;count=0;shake_count++;}break;
			case 0x02:	BalancePoint = ZHONGZHI-shakeparam;if(count==50){count_flag=1;count=0;if(shake_count>3){shake_count=0;count_flag=3;}}break;
			case 0x03:	BalancePoint = ZHONGZHI;           if(count==delay){count_flag=4;count=0;turn_flag=Random_Numb();Control_flag.TurnV=50;}break;
			case 0x04:	{
							switch(turn_flag)
							{
								case 0x00:	Single_Action(  60,0,0);break;//左转60°
								case 0x01:	Single_Action( -60,0,0);break;//右转60°
								case 0x02:	Single_Action(  90,0,0);break;//左转90°
								case 0x03:	Single_Action( -90,0,0);break;//右转90°
								case 0x04:	Single_Action( 120,0,0);break;//左转120°
								case 0x05:	Single_Action(-120,0,0);break;//右转120°
								default:
									break;
							}
							/*判断单个动作是否完成*/
							if(Control_flag.SingleAction)
							{
								Control_flag.SingleAction = 0;
								count_flag = 5;
								count=0;
								switch(Random_Numb()%3)
								{
									case 0x00:	delay =  20;break;//快
									case 0x01:	delay =  50;break;//中
									case 0x02:	delay = 100;break;//慢
									default:
										break;
								}
							}
						}break;
			case 0x05:	BalancePoint = ZHONGZHI;if(count == delay){count_flag=1;count=0;}break;//停顿
			default:
				break;
		}
	}
}



/********************************************END_DANCE_C_FILE********************************************/


