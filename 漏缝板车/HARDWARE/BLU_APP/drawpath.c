#include "drawpath.h"



u8 Path_BufLength;						// 画路径模式下接收的数据长度
u8 Path_Data_Update;					// 路径数据更新标志
u8 usddflag = 0;						// 路径数据颠倒标志
volatile u8 pdwtflag = 0;				// 路径数据等待标志

short Angle[150]={0};					// 存储路径角度
short Move[150]={0}; 					// 存储路径距离
/*********************************************************************************************************
** Function name:       DrawPath_Mode
** Descriptions:        当前处于画路径模式，对画路径协议数据进行处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void DrawPath_Mode(void)
{
	static u8 i = 0;
	static u8 time  = 0;
	static u8 times = 0;
	
	if(DRAWPATH == Car_Mode)
	{
		if(Path_Data_Update)//判断路径数据是否更新
		{
			i=0;
			Control_flag.Qian	 =0;
			Control_flag.Hou	 =0;
			Control_flag.Left	 =0;
			Control_flag.Right	 =0;
			Control_flag.Angle	 =0;
			Control_flag.Stop_Time =0;
			Control_flag.Distance=0;
			ActionControl_TurnAngle = 0;
			ActionControl_MoveDistance = 0;
			ActionControl_StayTime = 0;
			Control_flag.TurnV= 55;
			Control_flag.Acce = 55;
			Path_Data_Update = 0;
			
		}
		
		static u8 Flag_Draw_avoid=0;
		static u8 Draw_Path_count=0;
	    static u16 Go_bostaclecount=0;

		time++;
		
		 if(Distance<80)
		 {
			 //清零控制模式数据
			Control_flag.Qian   = 0;	// 前进标志
			Control_flag.Hou    = 0;	// 后退标志
			Control_flag.Left   = 0;	// 左转标志
			Control_flag.Right  = 0;	// 右转标志
			Control_flag.Stop   = 0;	// 停止标志
			times++;
			//Flag_Draw_avoid=1;
			Draw_Path_count++;
		 }	
		 
		 if(time<20)
		 {
			 if(times>10)
				 Flag_Draw_avoid=1;
		 }
		 else{
			 time  = 0;
			 times = 0;
		 }

		if(Flag_Draw_avoid == 1)
		 {
			Single_Action(0,-10,0);if(Control_flag.SingleAction){Flag_Draw_avoid=2;Control_flag.SingleAction = 0;}
		 } 
		 else if(Flag_Draw_avoid == 2)
		 {
			if(Draw_Path_count%3==0){
			Single_Action(45,0,0);if(Control_flag.SingleAction){Flag_Draw_avoid=0;Control_flag.SingleAction = 0;}
		    }
			else{
			Single_Action(170,0,0);if(Control_flag.SingleAction){Flag_Draw_avoid=0;Control_flag.SingleAction = 0;}
		    }
		 }
		 else 
		 {	
			Flag_Draw_avoid=0;
		 }
		
		if(Flag_Draw_avoid==0)
		{
			
			
			if(Path_BufLength!=0 && (Encoder_Left==0 && Encoder_Right==0)){
			
				if( (Encoder_Left==0 && Encoder_Right==0) )
				{
					Go_bostaclecount++;
					if(Go_bostaclecount>100)
					{
					   Go_bostaclecount=0;
						Flag_Draw_avoid=1;
					}
				}
				else
				{
					  Go_bostaclecount=0;
				}
					
			}
#if 0			
			//判断路径数据是否过来了
			if(pdwtflag)
			{
				if(1 == usddflag)
				{
					if(0==i)
					{
						Single_Action(180,Move[i],0);		//单个动作	
					}
					else
					{
						Single_Action(Angle[i],Move[i],0);	//单个动作	
					}
					
					if(Control_flag.SingleAction)//判断单个动作是否完成
					{
						i++;
						Control_flag.SingleAction = 0;
					}
				}
				else
				{
					Single_Action(Angle[i],Move[i],0);	//单个动作	
					if(Control_flag.SingleAction)		//判断单个动作是否完成
					{
						i++;
						Control_flag.SingleAction = 0;
					}
				}
				
				if(i>=(Path_BufLength/2))//判断是否完成走路径
				{
#if 0				
					if(Path_BufLength!=0)
					{
						m_sound_type=END_SOUND;sound_update=true;
					}
					//清除之前的缓存
					memset(Angle,0,200);
					memset(Move,0,200);
					//数据长度归零
					Path_BufLength = 0;
					i=0;
#endif
					//路径数据颠倒
					DrawPath_UpsideDown();
					i=0;
					usddflag = 1;
				}
			}
#endif
			
#if 1
			Single_Action(Angle[i],Move[i],0);	//单个动作	
			if(Control_flag.SingleAction)		//判断单个动作是否完成
			{
				i++;
				Control_flag.SingleAction = 0;
			}
			
			if(i>=(Path_BufLength/2))			//判断是否完成走路径
			{
				
				if(Path_BufLength!=0)
				{
					m_sound_type=END_SOUND;sound_update=true;
				}
				//清除之前的缓存
				memset(Angle,0,200);
				memset(Move,0,200);
				//数据长度归零
				Path_BufLength = 0;
				i=0;
			}
#endif		
	    }
	}
}

/*********************************************************************************************************
** Function name:       Set_DrawPath
** Descriptions:        处理并设置路径数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_DrawPath(void)
{
	//判断当前是否处于画路径模式
	if(DRAWPATH == Car_Mode)
	{

		Path_Data_Update = 1;
		memset(Angle,0,200);
		memset(Move,0,200);
		Path_BufLength = DrawPath_DP();
		Send_OK();
	}
}


/*********************************************************************************************************
** Function name:       Read_DrawPath
** Descriptions:        读取路径状态
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Read_DrawPath(void)
{
	
}


/*********************************************************************************************************
** Function name:       DrawPath_DP
** Descriptions:        当前处于画路径模式，获取uart3的接收缓存数据并进行转换
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
u8 DrawPath_DP(void)
{
	u8 Path_numb = 0;
	u8 n = 0;
	u8 i = 4;
	u8 angle = 0;
	u8 move  = 0;
	short Path_data = 0;
	pdwtflag = 1;
	for(Path_numb = ((USART3_RX_STA&0x00FF)-7); Path_numb>0;)
	{
		Path_data = (USART3_RX_BUF[i+1]<<8)|USART3_RX_BUF[i];
		n++;
		i += 2;
		Path_numb -= 2;
		if(n%2)
		{
			Angle[angle] = -Path_data;
			angle++;
		}
		else{
			Move[move] = Path_data;
			move++;
		}
	}
	
	USART3_RX_STA = 0;
	return n;
}

//路径规划数据颠倒
void DrawPath_UpsideDown(void)
{
	uint8_t	n=0;
	uint8_t	m=(Path_BufLength/2)-1;
	uint8_t i=(Path_BufLength/2)/2;
	short buff[2];
	
	for(n=0;n<i;)
	{
		buff[0]  = -Angle[n];
		Angle[n] = -Angle[m];
		Angle[m] = buff[0];
		buff[1]  = Move[n];
		Move[n]  = Move[m];
		Move[m]  = buff[1];
		m--;
		n++;
	}
}
