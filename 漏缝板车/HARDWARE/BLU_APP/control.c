#include "control.h"


void Follow_Switch(bool on_off);
void Touch_Switch(bool on_off);

volatile u8	 Control_Touch;	// 控制触摸标志
volatile u8  Control_Touch_Update;
void Gesture_Funtion(void);
void Touch_Funtion(void);
void Follow_Funtion(u32 Distance);
void Advoid_obstacle(void);
void Car_Clear(void);
void Advoid_obstacle_go(void);
volatile u8 m_Ulta_Gesture_Touch=D_GESTUER_ON;

bool control_sleep=false; 
//设置当前的控制状态
/********************************************************************************************************
** Function name:       Control_Mode
** Descriptions:        控制模式下：手势、避障、跟随、走迷宫运行动作
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Control_Mode(void)
{
     static	u8  Pre_m_Ulta_Gesture_Touch;
	//手势是否有更新或触摸是否有更新，更新了就清除数据
	if(gesture_update==1 ||  Pre_m_Ulta_Gesture_Touch!=m_Ulta_Gesture_Touch)
	{
		Car_Clear();
		gesture_update = 0;
	}
	Pre_m_Ulta_Gesture_Touch=m_Ulta_Gesture_Touch;
	
	Touch_Funtion();	//触碰功能
	if(Control_Touch==Touch_None)
	{
		switch (m_Ulta_Gesture_Touch)
		{
			case D_AVOID_ON: Advoid_obstacle();//避障功能
				break;
			case D_AVOID_OFF:
				break;
			case D_GESTUER_ON: Gesture_Funtion();	//手势功能
				break;
			case D_GESTUER_OFF:
				break;
			case D_FOLLOW_ON: Follow_Funtion(Distance);	//跟随功能
				break;
			case D_FOLLOW_OFF:
				break;
			case D_AVOID_GO_ON:Advoid_obstacle_go();//壁障迷宫
			default :
				break;
		}
	}
	
}

/*运动状态数据赋值数据清零*/
void Car_Clear(void)
{
		Control_flag.Qian =0;
		Control_flag.Hou =0;
		Control_flag.Left =0;
		Control_flag.Right =0;
		Control_flag.Angle=0;
		Control_flag.Stop_Time =0;
		Control_flag.Distance=0;
		ActionControl_TurnAngle = 0;
		ActionControl_MoveDistance = 0;
		ActionControl_StayTime = 0;
}

//设置当前的控制状态
/********************************************************************************************************
** Function name:       Control_Protocol
** Descriptions:        控制协议处理并对小车进行控制操作，操作设置成功发送OK，操作设置失败发送ERROR。
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_Control(void)
{
	
    if((USART3_RX_STA&0xFF)==8 && USART3_RX_BUF[4]==0x06)//休眠指令
	{
		control_sleep=true;	
		sound_update= true;
		m_sound_type=STOP_SOUND;
	}
	else
	{
		control_sleep=false;
	
	}
	
	if(m_Ulta_Gesture_Touch==D_AVOID_GO_ON){
		m_Ulta_Gesture_Touch=D_AVOID_GO_OFF;
	}
	
	
	if(CONTROL == Car_Mode)
	{
		if((USART3_RX_STA&0xFF)==8)
		{
       
			switch(USART3_RX_BUF[4])
			{
				case 0x01:	
					Control_flag.Qian=1;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//前进
				case 0x02:	
					Control_flag.Qian=0;Control_flag.Hou=1;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//后退
				case 0x03:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=1;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//左转
				case 0x04:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=1;Control_flag.Stop=0;
				    Send_OK();
				    break;//右转
				case 0x05:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//停止
				case 0x06:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=1;
				    Send_OK();
				    break;//待机
				case 0x09:		sound_update= true;m_sound_type=MODE_CHANGE_SOUND; m_Ulta_Gesture_Touch=D_AVOID_ON;Send_OK();break;//开启壁障
				case 0x0A:	    sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_AVOID_OFF;Send_OK();break;//关闭壁障
				case 0x0B:	    sound_update= true;m_sound_type=MODE_CHANGE_SOUND; m_Ulta_Gesture_Touch=D_GESTUER_ON;Gesture_Switch(ON);Send_OK();break;//开启手势
				case 0x0C:	    sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_GESTUER_OFF;Gesture_Switch(OFF);Send_OK();break;//关闭手势
				case 0x0D:      sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_FOLLOW_ON;Follow_Switch(ON);Send_OK();break;//开启跟随
				case 0x0E:      sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_FOLLOW_OFF;Send_OK();break;//关闭跟随
				case 0x0F:  
					Control_flag.Qian=1;Control_flag.Hou=0;Control_flag.Left=1;Control_flag.Right=0;Control_flag.Stop=0;
					Send_OK();//左前
					break;
				case 0x10:  
					Control_flag.Qian=1;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=1;Control_flag.Stop=0;
					Send_OK();//右前
					break;
				case 0x11:  
					Control_flag.Qian=0;Control_flag.Hou=1;Control_flag.Left=1;Control_flag.Right=0;Control_flag.Stop=0;
					Send_OK();//左后
					break;
				case 0x12:  
					Control_flag.Qian=0;Control_flag.Hou=1;Control_flag.Left=0;Control_flag.Right=1;Control_flag.Stop=0;
					Send_OK();//右后
					break;
				default:
					Send_ERROR();break;//接收到错误信息
			}
		}
		else
		{
			if((USART3_RX_STA&0xFF)==9)
			{
				//前后移动加减速
				if(USART3_RX_BUF[4]==0x07)
				{
					if(USART3_RX_BUF[5]<=100)
					{
						Control_flag.Acce = USART3_RX_BUF[5];//移动速度范围0~100
						Send_OK();
					}else{
						Send_ERROR();//接收到错误信息
					}
				}
				//左右转动加减速
				else if(USART3_RX_BUF[4]==0x08)
				{
					if(USART3_RX_BUF[5]<=100)
					{
						Control_flag.TurnV = USART3_RX_BUF[5];//转动速度范围0~100
						Send_OK();
					}else{
						Send_ERROR();//接收到错误信息
					}
				}
				else{
					Send_ERROR();//接收到错误信息
				}
			}
			else
			{
				//接收到错误信息
				Send_ERROR();
			}
		}
	}
}



/*********************************************************************************************************
** Function name:       Gesture_Function
** Descriptions:        手势功能
** input parameters:    on_off：ON==开启手势功能，OFF==关闭手势功能
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Gesture_Switch(bool on_off)
{
	if(on_off)
	{
		APDS9960.flag = 1;//APDS9960开启手势模式
	}
	else{
		APDS9960.flag = 0;//APDS9960关闭手势模式
	}
}


/*********************************************************************************************************
** Function name:       Follow_Function
** Descriptions:        跟随功能
** input parameters:    on_off：ON==开启跟随功能，OFF==关闭跟随功能
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Follow_Switch(bool on_off)
{
	if(on_off)
	{
		APDS9960.flag = 2;//APDS9960开启接近模式
	}
	else{
		APDS9960.flag = 0;//APDS9960关闭接近模式
	}
}


void Touch_Switch(bool on_off)
{
	if(on_off)
	{
	}
	else{
	}
}
/*********************************************************************************************************
** Function name:        Dir_LeftResult
** Descriptions:        手势向左响应函数
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
short G_Zero[4]   = {0,0,0,0};
short LG_angle[4] = {-90, 90,-180, 180};
short RG_angle[4] = { 90,-90, 180,-180};
short BK_Distance[10]={-500, 500,-800, 800,-1000, 1000,-1600, 1600,-2000, 2000};
short FK_Distance[10]={ 500,-500, 800,-800, 1000,-1000, 1600,-1600, 2000,-2000};

extern volatile uint8_t RandomCount;
volatile uint8_t gesture_lefttimes;
volatile uint8_t gesture_righttimes;
volatile uint8_t RandomCountBuf;
void Dir_LeftResult(void)
{
	switch(gesture_lefttimes)
	{
		case 0x01:	/*第一次左手势先左转90°再右转90°*/
					action_control(&LG_angle[0],&G_Zero[0],(u16 *)&G_Zero[0],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;				
					};
					break;
		case 0x02:	/*第二次左手势先左转180°再右转180°*/
					action_control(&LG_angle[2],&G_Zero[2],(u16 *)&G_Zero[2],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;
					};
					break;
		case 0x03:	/*第三次左手势进入概率自旋*/
					if(RandomCountBuf<35){							//35%概率
						Single_Action(-360,0,0); 				//向左自旋1次
					}else{
						if(RandomCountBuf<65){						//30%概率
							Single_Action(-1080,0,0);			//向左自旋3次
						}else{
							if(RandomCountBuf<80){					//15%概率
								Single_Action(-1800,0,0);		//向左自旋5次
							}else{
								if(RandomCountBuf<95){				//15%概率
									Single_Action(-3600,0,0);	//向左自旋10次
									
								}else{							// 5%概率
									Control_flag.Left = 1;		//一直向左自旋
									APDS9960.gesture = 0;
									m_sound_type=END_SOUND;sound_update=true;
								}
								gesture_lefttimes= 0;			//次数归零
							}
						}
					}
					if(Control_flag.SingleAction)//动作完成
					{
						APDS9960.gesture = 0;
						Control_flag.SingleAction = 0;
						m_sound_type=LEFT_HAPPY_SOUND;sound_update=true;
					}
					else 
					{
						Led_mode=LEFT_LED_MODE;
					}
					break;
		default:
			break;
	}
	
	    if(gesture_righttimes== 0){Led_mode=LEFT_LED_MODE;}
		
				
}
/*********************************************************************************************************
** Function name:        Dir_LeftResult
** Descriptions:        手势向右响应函数
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Dir_RightResult(void)
{

	switch(gesture_righttimes)
	{
		case 0x01:	/*第一次右手势先右转90°再左转90°*/
					action_control(&RG_angle[0],&G_Zero[0],(u16 *)&G_Zero[0],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;
					};
					break;
		case 0x02:	/*第二次右手势先右转180°再左转180°*/
					action_control(&RG_angle[2],&G_Zero[2],(u16 *)&G_Zero[2],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;
					};
					break;
		case 0x03:	/*第三次左手势进入概率自旋*/
					if(RandomCountBuf<35){							//35%概率
						Single_Action(360,0,0); 				//向右自旋1次
					}else{
						if(RandomCountBuf<65){						//30%概率
							Single_Action(1080,0,0);			//向右自旋3次
					
						}else{
							if(RandomCountBuf<80){					//15%概率
								Single_Action(1800,0,0);		//向右自旋5次
							
							}else{
								if(RandomCountBuf<95){				//15%概率
									Single_Action(3600,0,0);	//向右自旋10次
								}else{							// 5%概率
									Control_flag.Right= 1;		//一直向右旋转
									APDS9960.gesture = 0;
									m_sound_type=END_SOUND;sound_update=true;
								}
								gesture_righttimes= 0;			//次数归零
							}
						}
					}
					if(Control_flag.SingleAction)
					{
						APDS9960.gesture = 0;
						Control_flag.SingleAction = 0;
						m_sound_type=LEFT_HAPPY_SOUND;sound_update=true;
					}
					else 
					{
						Led_mode=RIGHT_LED_MODE;
					}
					break;
		default:
			break;
		}		   

		if(gesture_righttimes== 0){Led_mode=RIGHT_LED_MODE;}
		
}
/*********************************************************************************************************
** Function name:        Dir_NearResult
** Descriptions:        手势接近响应函数
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Dir_NearResult(void)
{

	if(RandomCountBuf<35){							//35%概率
		action_control(&G_Zero[0],&BK_Distance[0],(u16 *)&G_Zero[0],2); 				//向后退0.5m
	}else{
		if(RandomCountBuf<65){						//30%概率
			action_control(&G_Zero[0],&BK_Distance[2],(u16 *)&G_Zero[0],2); 			//向后退0.8m
		}else{
			if(RandomCountBuf<80){					//15%概率
				action_control(&G_Zero[0],&BK_Distance[4],(u16 *)&G_Zero[0],2); 		//向后退1.5m,前进1米
			}else{
				if(RandomCountBuf<95){				//15%概率
					action_control(&G_Zero[0],&BK_Distance[6],(u16 *)&G_Zero[0],2);		//概率后退2米前进1.5米；
				}else{							// 5%概率
					action_control(&G_Zero[0],&BK_Distance[8],(u16 *)&G_Zero[0],2);		//概率后退2米前进1.5米；
					APDS9960.gesture = 0;
				}
			}
		}
	}
	if(Control_flag.Much_Action)
	{
		APDS9960.gesture = 0;
		Control_flag.Much_Action = 0;
	};
}
/*********************************************************************************************************
** Function name:        Dir_FarResult
** Descriptions:        手势远离响应函数
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Dir_FarResult(void)
{
	if(RandomCountBuf<35){							//35%概率
		action_control(&G_Zero[0],&FK_Distance[0],(u16 *)&G_Zero[0],2); 				//向后退0.5m
	}else{
		if(RandomCountBuf<65){						//30%概率
			action_control(&G_Zero[0],&FK_Distance[2],(u16 *)&G_Zero[0],2); 			//向后退0.8m
		}else{
			if(RandomCountBuf<80){					//15%概率
				action_control(&G_Zero[0],&FK_Distance[4],(u16 *)&G_Zero[0],2); 		//向后退1.5m,前进1米
			}else{
				if(RandomCountBuf<95){				//15%概率
					action_control(&G_Zero[0],&FK_Distance[6],(u16 *)&G_Zero[0],2);		//概率后退2米前进1.5米；
				}else{							// 5%概率
					action_control(&G_Zero[0],&FK_Distance[8],(u16 *)&G_Zero[0],2);		//概率后退2米前进1.5米；
					APDS9960.gesture = 0;
				}
			}
		}
	}
	if(Control_flag.Much_Action)
	{
		APDS9960.gesture = 0;
		Control_flag.Much_Action = 0;
	};
	
}

/*********************************************************************************************************
** Function name:       Gesture_Funtion
** Descriptions:        手势功能
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Gesture_Funtion(void)
{ 
	//手势效果控制(没有触碰的情况下有效)
	if(APDS9960.flag == APDS_GESTURE)
	{
		switch(APDS9960.gesture)
		{	
			case DIR_DOWN:
				if(gesture_led_update == 1){Led_mode=DOWN_LED_MODE;}
				Control_flag.Stop = 1;
				break;//向下手势==电机向下
			case DIR_UP:
				if(gesture_led_update == 1){Led_mode=UP_LED_MODE;}				
				Control_flag.Stop = 0;
				break;//向上手势==电机启动
			case DIR_LEFT:	
				if(gesture_led_update == 1){ Led_mode=LEFT_LED_MODE; RandomCountBuf=RandomCount;if(gesture_lefttimes<3)	gesture_lefttimes++;}
				Dir_LeftResult();
				break;//向左手势效果
			case DIR_RIGHT:	
				if(gesture_led_update == 1){Led_mode=RIGHT_LED_MODE; RandomCountBuf=RandomCount;if(gesture_righttimes<3)gesture_righttimes++;}
				Dir_RightResult();
				break;//向右手势效果
			case DIR_NEAR:
				if(gesture_led_update == 1){Led_mode=FLASH_LED_MODE; RandomCountBuf=RandomCount;}
				Dir_NearResult();
				break;//接近手势效果
			case DIR_FAR:
				if(gesture_led_update == 1){Led_mode=FLASH_LED_MODE; RandomCountBuf=RandomCount;}					
				Dir_FarResult();
				break;//远离手势效果
			default:
				APDS9960.gesture = 0;
				break;
		}
		gesture_led_update=0;
	}
}

/*********************************************************************************************************
** Function name:       Follow_Funtion
** Descriptions:        跟随功能
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Follow_Funtion(u32 Distance)
{	
//	static u32 distance_old;	//上一时刻距离
//	static u32 distance_new;	//现在时刻距离
//	static u32 bias_old;		//上一时刻偏差
//	static u32 bias_new;		//现在时刻偏差
	
	if(Control_flag.Qian==0 &&  Control_flag.Hou==0)
	{
		if(APDS9960.proximity>240)
		{
			Control_flag.direction=1;
		}
		
//		distance_old = distance_new;			//上一刻距离
//		distance_new = Distance;				//当前距离
//		bias_old = bias_new;					//上一刻偏差
//		bias_new = distance_new-distance_old;	//当前偏差
		
		if(Distance<110)
		{
			Control_flag.direction=1;
		}	
		else if(110<Distance&&Distance<190)
		{
			Control_flag.direction=0;
		}
		else if(190<Distance&&Distance<500)
		{
			Control_flag.direction=2;
		}
		
		if( (Distance>500&&APDS9960.proximity<240) || Distance>500)
		{
			Control_flag.direction=0;
		}				
	}
}

/*********************************************************************************************************
** Function name:       Advoid_obstacle
** Descriptions:        壁障功能函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Advoid_obstacle(void)
{
	 static u8 Flag_avoid=0;

	 if(Distance<105)
	 {
		 Control_flag.Qian = 0;
		 Control_flag.Hou  = 1;
		 Flag_avoid=1;
	 }
      

	 
	 if(Flag_avoid == 1)
	 {
		Single_Action(0,-20,0);if(Control_flag.SingleAction){Flag_avoid=2;Control_flag.SingleAction = 0;}
	 } 
	 else if(Flag_avoid == 2)
	 {
		Single_Action(45,0,0);if(Control_flag.SingleAction){Flag_avoid=3;Control_flag.SingleAction = 0;}
	 }
	 else 
	 {
		 Flag_avoid=0;		 
	 }	 
}


/*********************************************************************************************************
** Function name:       Advoid_obstacle_go
** Descriptions:        走迷宫
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Advoid_obstacle_go(void)
{
	 static u8 Flag_avoid=0;
	 static u16 Go_bostaclecount=0;
	 static u16 Go_bostacle_Mileage_count=1;
	 static u32 Advoid_Mileage_MM;
	 static u32 Pre_Advoid_Mileage_MM;
	
	 if(Distance<90)
	 {
		 Control_flag.Qian = 0;
		 Control_flag.Hou  = 1;
		 Flag_avoid=1;
	 }	
	 
	 if(Flag_avoid==0)
	 {
         
		 Go_bostacle_Mileage_count++;
		 
		 if(Go_bostacle_Mileage_count%100==0){
			 
			Advoid_Mileage_MM+= (Encoder_Left+Encoder_Right)/(2496*2.0f)*207.34f;
			if(Advoid_Mileage_MM<=(Pre_Advoid_Mileage_MM)) Flag_avoid=1;
			Pre_Advoid_Mileage_MM=Advoid_Mileage_MM;
		 }
		
		 if( (Encoder_Left==0 && Encoder_Right==0) )
		 {
			Go_bostaclecount++;
			if(Go_bostaclecount>100)
			{
			    Go_bostaclecount=0;
				Flag_avoid=1;
			}
		 }
		 else
		 {
			  Go_bostaclecount=0;
		 }
		
	 
	 }
     
	 if(Flag_avoid == 1)
	 {
		 
		    Advoid_Mileage_MM=0;
		    Pre_Advoid_Mileage_MM=0;
		    Go_bostacle_Mileage_count=0;
		   
		   if( (Encoder_Left==0 && Encoder_Right==0) )
			{
				Go_bostaclecount++;
				if(Go_bostaclecount>100)
				{
				    Go_bostaclecount=0;
					Flag_avoid=2;
				}
			}
			else
			{
				  Go_bostaclecount=0;
			}
		    Single_Action(0,-20,0);if(Control_flag.SingleAction){Flag_avoid=2;Control_flag.SingleAction = 0;}
			
	 } 
	 else if(Flag_avoid == 2)
	 {   
		     Advoid_Mileage_MM=0;
		    Pre_Advoid_Mileage_MM=0;
		    Go_bostacle_Mileage_count=0;
		 
		    Go_bostaclecount=0;
		    Single_Action(30,0,0);if(Control_flag.SingleAction){Flag_avoid=0;Control_flag.SingleAction = 0;}
	 }
	 else 
	 {   
			 Flag_avoid=0;
			 Control_flag.Qian = 1;
			 Control_flag.Acce=40;
			 Control_flag.TurnV=38;
	 }	
	
	
}

short Touch_LG_angle[5] = {-120,  30, -30,  30, -30};
short Touch_G_Zero[5]   = {0  ,   0,  0,   0,  	 0};
u16 Touch_Time[5]   =     {5  ,	  5,  5,   5,    5};

short Touch_RG_angle[5] = { 120,  -30, 30,  -30, 30};

short Touch_Front_Back_Distance[2] = { 120,0};

short Touch_Front_Back_Angle[2] = { 0,180};

u16 Touch_Front_Back_Time[2] = { 0,0};




/*********************************************************************************************************
** Function name:       Touch_Funtion
** Descriptions:        触碰功能
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Touch_Funtion(void)
{
	//触碰效果控制
	static u8 PreControl_Touch=Touch_None;
	switch(Control_Touch)
	{
		case Touch_Left:	
			           if(PreControl_Touch!=Control_Touch || Control_Touch_Update==true){
						    Control_Touch_Update=false;
							if(RandomCount<50){
								m_sound_type=LEFT_NEVERS_SOUND;
							}
							else{
								m_sound_type=LEFT_ANGERY_SOUND;
							}
								sound_update=true;
								Car_Clear();	
					   }
					   action_control(Touch_LG_angle,Touch_G_Zero,Touch_Time,5);
					   if(Control_flag.Much_Action)
					   {
						Control_flag.Much_Action = 0;
						Control_Touch = Touch_None;
					    };
						break;
		case Touch_Right:	
						if(PreControl_Touch!=Control_Touch  || Control_Touch_Update==true){ 
							Control_Touch_Update=false;
							if(RandomCount<50){
								m_sound_type=RIGHT_NEVERS_SOUND;
							}
							else{
								m_sound_type=RIGHT_ANGRY_SOUND;
							}
								 sound_update=true;
								 Car_Clear();
						}		
						action_control(Touch_RG_angle,Touch_G_Zero,Touch_Time,5);
					    if(Control_flag.Much_Action)
					    {
							Control_flag.Much_Action = 0;
							Control_Touch = Touch_None;
					    };
						break;//右翅膀触摸效果==右转60°
		case Touch_Back:	
						if(PreControl_Touch!=Control_Touch || Control_Touch_Update==true){
							Control_Touch_Update=false;
							if(RandomCount<30){
								m_sound_type=LEFT_HAPPY_SOUND;
							}
							else if(RandomCount<60)
							{
								m_sound_type=RIGHT_HAPPY_SOUND;
							}
							else
							{
								m_sound_type=BACK_LOVELY_SOUND;
							}
							sound_update=true;
							Car_Clear();
						}

							if(Control_flag.Much_Action)
						{
							APDS9960.gesture = 0;
							Control_flag.Much_Action = 0;
						}
						action_control(Touch_Front_Back_Angle,Touch_Front_Back_Distance,Touch_Front_Back_Time,2);
						if(Control_flag.Much_Action){Control_Touch = Touch_None;Control_flag.Much_Action = 0;} 	
						break;//后背心触摸效果==前进200mm
		case Touch_Front:	
					
						if(PreControl_Touch!=Control_Touch  || Control_Touch_Update==true){
							Control_Touch_Update=false;
							if(RandomCount<50)
								m_sound_type=FORN_SURPRISED_SOUND;
							else
							{
								m_sound_type=BACK_PRIDE_SOUND;
							}
							sound_update=true;
							Car_Clear();
						}
						action_control(Touch_Front_Back_Angle,Touch_Front_Back_Distance,Touch_Front_Back_Time,2);
						if(Control_flag.Much_Action){Control_Touch = Touch_None;Control_flag.Much_Action = 0;} 
						break;//前肚子触摸效果==后退200mm
		default:
			break;
	}
	PreControl_Touch=Control_Touch;
}














