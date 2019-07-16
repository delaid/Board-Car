#include "Key_task.h"

#define TOUCH_KEY_TASK_PRIO					2					//设置触摸任务优先级
#define TOUCH_KEY_STK_SIZE					64					//设置触摸任务堆栈大小
OS_STK TOUCH_KEY_TASK_STK[TOUCH_KEY_STK_SIZE]; 			  		//触摸任务堆栈
void touch_key_task(void *pdata);								//触摸任务函数

OS_EVENT * msg_key;			//按键邮箱事件块	 
OS_EVENT * q_msg;			//消息队列
OS_FLAG_GRP * flags_key;	//按键信号量集

void * MsgGrp[32];			//消息队列存储地址,最大支持256个消息

void Key_task_Create(void)
{   
	msg_key=OSMboxCreate((void*)0);		//创建消息邮箱
	q_msg=OSQCreate(&MsgGrp[0],32);	//创建消息队列
	OSTaskCreate(touch_key_task,(void*)0,(OS_STK*)&TOUCH_KEY_TASK_STK[TOUCH_KEY_STK_SIZE-1],TOUCH_KEY_TASK_PRIO);
}

bool power_low_flag=false;
/*********************************************************************************************************
** Function name:       touch_key_task
** Descriptions:        处理触摸按键采集的数据，
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
//按键测试任务
void touch_key_task(void *pdata)
{
	u8 key;	
	while(1)
	{
       key=key_scan(0);
	   if(key)OSMboxPost(msg_key,(void*)key);//发送消息
 	   delay_ms(20);
	}
}
/*********************************************************************************************************
** Function name:       KeyScan_healder
** Descriptions:        处理触摸按键处理函数，
** input parameters:  
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void  KeyScan_healder(void)
{ 
	uint32_t key=0;	
	uint8_t  err;
	key=(u32)OSMboxPend(msg_key,10,&err); //读取按键信号量

	static bool Mode_Enter_Flag=0;//进入模式选择标志位
	static uint32_t m_key_count=0;//用于检测有没有按下计数
	static uint16_t power_led_count=0;//用于低压RGB灯亮红灯闪烁
	static uint8_t  pre_m=0;//记录先前按下的状态值
	static uint8_t  m  = 0;//初始化按下状态值
	static uint8_t  m_Ulta_Gesture_count;//手势模式的切换标志
	static uint8_t  m_Dance_Fast_Slow_count;//跳舞模式的切换标志
	static uint8_t  m_Night_light_count;//夜灯模式的切换标志
	
	switch (key)
	{
		case KEY_LEFT_PRES:
			if(Mode_Enter_Flag==true) /*进入模式选择后状态值递增*/
			{
				if(m<4) m++; 
				else m=1;
			}
			if(DANCE == Car_Mode && DANCE_SHAKE == Dance_Flag)/*摇晃幅度递增*/
			{
				if(shakeparam<SHAKEMAXE){
					shakeparam++;
				}
			}
			break;
		case KEY_RIGHT_PRES:
			if(Mode_Enter_Flag==true){ /*进入模式选择后状态值递减*/
				if(m>1)	m--; 
				else    m=4;
			}
			if(DANCE == Car_Mode && DANCE_SHAKE == Dance_Flag)/*摇晃幅度递减*/
			{
				if(shakeparam>0){
					shakeparam--;
				}
			}
			break;
		case KEY_MOD_PRES:/*进入模式选择状态*/
			Mode_Enter_Flag=!Mode_Enter_Flag;
			if(Mode_Enter_Flag==true)
				m = 1;
			else
			{
				m = 0;
				Led_mode=LED_CLOSE;					
			}
			break;
		case KEY_FOUNCTION_PRES:/*进入模式后选择功能*/
			if(Car_Mode==CONTROL)/*控制模式下功能选择*/
			{	
				Led_mode=OK_LED_MODE;
				if(m_Ulta_Gesture_count==2){m_Ulta_Gesture_Touch=D_GESTUER_ON;Gesture_Switch(ON);}
				else if(m_Ulta_Gesture_count==1){m_Ulta_Gesture_Touch=D_FOLLOW_ON;}
				else if(m_Ulta_Gesture_count==0){m_Ulta_Gesture_Touch=D_AVOID_GO_ON;}
				if(m_Ulta_Gesture_count<2)
					m_Ulta_Gesture_count++;
				else
					m_Ulta_Gesture_count=0;

			}
			else if(Car_Mode==DANCE)/*控跳舞功能下选择*/
			{	
				Led_mode=OK_LED_MODE;
				Dance_Clear(); 
				if(m_Dance_Fast_Slow_count==0)
				{
					Dance_Flag=DANCE_FAST;
					m_Dance_Fast_Slow_count=1;
				}
				else if(m_Dance_Fast_Slow_count == 1)
				{
					Dance_Flag=DANCE_SLOW;
					m_Dance_Fast_Slow_count=2;
				}
				else if(m_Dance_Fast_Slow_count == 2)
				{
					Dance_Flag=DANCE_SHAKE;
					m_Dance_Fast_Slow_count=0;
				}
			}
			else if(Car_Mode==NIGHTLIGHT)/*夜灯模式下功能选择*/
			{
				Led_mode=OK_LED_MODE;
				
			//	if(m_Night_light_count==0){m_nightlight.mode=CLOSE_LIGHT;}
			    if(m_Night_light_count==0){m_nightlight.mode=OPEN_LIGHT;m_nightlight.brightness=50;m_Night_light_count=1;}
				else{m_nightlight.mode=OPEN_BODY_IR;m_nightlight.brightness=50;m_Night_light_count=0;}
				
			}
			break;
		case KEY_LIGHT_ADD:  if(Car_Mode==NIGHTLIGHT){if(m_nightlight.brightness<100)m_nightlight.brightness++;}/*灯光亮度递增*/
			break;
		case KEY_LIGHT_DECRE:if(Car_Mode==NIGHTLIGHT){if(m_nightlight.brightness>0)m_nightlight.brightness--;}/*灯光亮度递减*/
			break;
		default:
			break;
	 }
	 
	 
	if(pre_m!=m){//模式更新成功
	  m_key_count=0;
	  Mode_update=1;
	}
	pre_m=m;
	  
	if(Mode_Enter_Flag==true)//5S没有操作自动退出
	{
		m_key_count++;
		Control_flag.Stop=1;
		if(m_key_count>50)
		{
			m_key_count=0;
			Mode_Enter_Flag=0;
			m = 0;
			Led_mode=LED_CLOSE;
		}
		
	}
	else
	{
		m_key_count=0;
	}
	 
	 
	
	if(Mode_Enter_Flag==0)/*退出模式选择*/
	{
        
		if(socket_swtich==false)//没有插入插座
		{ 
			if(power_low==true )//电池电压低头顶红灯一闪一闪
			{ 
				power_led_count++;
				if(power_led_count==5)
				{
					RGB_LIGHT_RED;
				}
				else if(power_led_count>=10)
				{
					RGB_LIGHT_BLACK;
					power_led_count=0;
				}
			}
			else
			{  
				if(control_sleep==true || power_off==true)
				{ 
					RGB_LIGHT_BLACK;	
				}
				else
				{
					switch(Car_Mode)//模式灯
					{
						case MODE:		RGB_LIGHT_WHITE;break;//白光==模式选择
						case DRAWPATH:	RGB_LIGHT_GREEN;break;//绿==路径模式
						case DANCE:		RGB_LIGHT_PINK;
						if(Dance_Flag==DANCE_FAST && Control_flag.Stop==0 ){
							Led_mode=DANCE_LED_FAST;
						}
						else if(Dance_Flag==DANCE_SLOW && Control_flag.Stop==0){
							Led_mode=DANCE_LED_SLOW;
						}
						else if(Dance_Flag==DANCE_SHAKE && Control_flag.Stop==0){
							Led_mode=DANCE_LED_SHAKE;
						}
						break;//粉==跳舞模式
						case CONTROL:	RGB_LIGHT_BLUE;break;//蓝色==摇杆模式
						case NIGHTLIGHT:RGB_LIGHT_YELLOW;break;//黄色==夜灯模式			
					}
				}
			}
		}
		else//插入插座，头顶绿灯一闪一闪
		{
			power_led_count++;        
			if(power_led_count==5)
			{
			   RGB_LIGHT_GREEN;
			}
			else if(power_led_count>=10)
			{
				RGB_LIGHT_BLACK;
				power_led_count=0;
			}
		}
		/*********触摸值获取*********/
		switch (key)
		{
			case KEY_LEFT_PRES:		Control_Touch = Touch_Left; Control_Touch_Update =true; break;
			case KEY_RIGHT_PRES:	Control_Touch = Touch_Right;Control_Touch_Update =true; break;
			case KEY_BACK_PRES:		Control_Touch = Touch_Back; Control_Touch_Update =true; break;
			case KEY_FORWARD_PRES:	Control_Touch = Touch_Front;Control_Touch_Update =true; break;
		}

	}
	else/*进入模式选择*/
	{
		switch(m)
		{
			case 1:	Car_Mode=CONTROL;RGB_LIGHT_BLUE;Led_mode=CONTROL_LED_MODE;m_Ulta_Gesture_Touch=D_GESTUER_ON;Gesture_Switch(ON);break;//蓝色==摇杆模式
			case 2:	Car_Mode=NIGHTLIGHT;RGB_LIGHT_YELLOW;Led_mode=NIGHT_LIGHT_LED_MODE;m_nightlight.mode=OPEN_BODY_IR;m_nightlight.brightness=50;break;//黄色==夜灯模式 ,热释常开
			case 3:	Car_Mode=DANCE;RGB_LIGHT_PINK;Led_mode=DANCE_LIGHT_LED_MODE;break;//粉==跳舞模式
			case 4: Car_Mode=DRAWPATH;RGB_LIGHT_GREEN;Led_mode=DRAWPATH_LED_MODE;break;//绿==路径模式
			default:
				break;		
		}

	}
	

}	

	




