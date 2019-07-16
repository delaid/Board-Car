#include "control.h"


void Follow_Switch(bool on_off);
void Touch_Switch(bool on_off);

volatile u8	 Control_Touch;	// ���ƴ�����־
volatile u8  Control_Touch_Update;
void Gesture_Funtion(void);
void Touch_Funtion(void);
void Follow_Funtion(u32 Distance);
void Advoid_obstacle(void);
void Car_Clear(void);
void Advoid_obstacle_go(void);
volatile u8 m_Ulta_Gesture_Touch=D_GESTUER_ON;

bool control_sleep=false; 
//���õ�ǰ�Ŀ���״̬
/********************************************************************************************************
** Function name:       Control_Mode
** Descriptions:        ����ģʽ�£����ơ����ϡ����桢���Թ����ж���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Control_Mode(void)
{
     static	u8  Pre_m_Ulta_Gesture_Touch;
	//�����Ƿ��и��»����Ƿ��и��£������˾��������
	if(gesture_update==1 ||  Pre_m_Ulta_Gesture_Touch!=m_Ulta_Gesture_Touch)
	{
		Car_Clear();
		gesture_update = 0;
	}
	Pre_m_Ulta_Gesture_Touch=m_Ulta_Gesture_Touch;
	
	Touch_Funtion();	//��������
	if(Control_Touch==Touch_None)
	{
		switch (m_Ulta_Gesture_Touch)
		{
			case D_AVOID_ON: Advoid_obstacle();//���Ϲ���
				break;
			case D_AVOID_OFF:
				break;
			case D_GESTUER_ON: Gesture_Funtion();	//���ƹ���
				break;
			case D_GESTUER_OFF:
				break;
			case D_FOLLOW_ON: Follow_Funtion(Distance);	//���湦��
				break;
			case D_FOLLOW_OFF:
				break;
			case D_AVOID_GO_ON:Advoid_obstacle_go();//�����Թ�
			default :
				break;
		}
	}
	
}

/*�˶�״̬���ݸ�ֵ��������*/
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

//���õ�ǰ�Ŀ���״̬
/********************************************************************************************************
** Function name:       Control_Protocol
** Descriptions:        ����Э�鴦����С�����п��Ʋ������������óɹ�����OK����������ʧ�ܷ���ERROR��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_Control(void)
{
	
    if((USART3_RX_STA&0xFF)==8 && USART3_RX_BUF[4]==0x06)//����ָ��
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
				    break;//ǰ��
				case 0x02:	
					Control_flag.Qian=0;Control_flag.Hou=1;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//����
				case 0x03:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=1;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//��ת
				case 0x04:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=1;Control_flag.Stop=0;
				    Send_OK();
				    break;//��ת
				case 0x05:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=0;
				    Send_OK();
				    break;//ֹͣ
				case 0x06:	
					Control_flag.Qian=0;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=0;Control_flag.Stop=1;
				    Send_OK();
				    break;//����
				case 0x09:		sound_update= true;m_sound_type=MODE_CHANGE_SOUND; m_Ulta_Gesture_Touch=D_AVOID_ON;Send_OK();break;//��������
				case 0x0A:	    sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_AVOID_OFF;Send_OK();break;//�رձ���
				case 0x0B:	    sound_update= true;m_sound_type=MODE_CHANGE_SOUND; m_Ulta_Gesture_Touch=D_GESTUER_ON;Gesture_Switch(ON);Send_OK();break;//��������
				case 0x0C:	    sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_GESTUER_OFF;Gesture_Switch(OFF);Send_OK();break;//�ر�����
				case 0x0D:      sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_FOLLOW_ON;Follow_Switch(ON);Send_OK();break;//��������
				case 0x0E:      sound_update= true;m_sound_type=MODE_CHANGE_SOUND;m_Ulta_Gesture_Touch=D_FOLLOW_OFF;Send_OK();break;//�رո���
				case 0x0F:  
					Control_flag.Qian=1;Control_flag.Hou=0;Control_flag.Left=1;Control_flag.Right=0;Control_flag.Stop=0;
					Send_OK();//��ǰ
					break;
				case 0x10:  
					Control_flag.Qian=1;Control_flag.Hou=0;Control_flag.Left=0;Control_flag.Right=1;Control_flag.Stop=0;
					Send_OK();//��ǰ
					break;
				case 0x11:  
					Control_flag.Qian=0;Control_flag.Hou=1;Control_flag.Left=1;Control_flag.Right=0;Control_flag.Stop=0;
					Send_OK();//���
					break;
				case 0x12:  
					Control_flag.Qian=0;Control_flag.Hou=1;Control_flag.Left=0;Control_flag.Right=1;Control_flag.Stop=0;
					Send_OK();//�Һ�
					break;
				default:
					Send_ERROR();break;//���յ�������Ϣ
			}
		}
		else
		{
			if((USART3_RX_STA&0xFF)==9)
			{
				//ǰ���ƶ��Ӽ���
				if(USART3_RX_BUF[4]==0x07)
				{
					if(USART3_RX_BUF[5]<=100)
					{
						Control_flag.Acce = USART3_RX_BUF[5];//�ƶ��ٶȷ�Χ0~100
						Send_OK();
					}else{
						Send_ERROR();//���յ�������Ϣ
					}
				}
				//����ת���Ӽ���
				else if(USART3_RX_BUF[4]==0x08)
				{
					if(USART3_RX_BUF[5]<=100)
					{
						Control_flag.TurnV = USART3_RX_BUF[5];//ת���ٶȷ�Χ0~100
						Send_OK();
					}else{
						Send_ERROR();//���յ�������Ϣ
					}
				}
				else{
					Send_ERROR();//���յ�������Ϣ
				}
			}
			else
			{
				//���յ�������Ϣ
				Send_ERROR();
			}
		}
	}
}



/*********************************************************************************************************
** Function name:       Gesture_Function
** Descriptions:        ���ƹ���
** input parameters:    on_off��ON==�������ƹ��ܣ�OFF==�ر����ƹ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Gesture_Switch(bool on_off)
{
	if(on_off)
	{
		APDS9960.flag = 1;//APDS9960��������ģʽ
	}
	else{
		APDS9960.flag = 0;//APDS9960�ر�����ģʽ
	}
}


/*********************************************************************************************************
** Function name:       Follow_Function
** Descriptions:        ���湦��
** input parameters:    on_off��ON==�������湦�ܣ�OFF==�رո��湦��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Follow_Switch(bool on_off)
{
	if(on_off)
	{
		APDS9960.flag = 2;//APDS9960�����ӽ�ģʽ
	}
	else{
		APDS9960.flag = 0;//APDS9960�رսӽ�ģʽ
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
** Descriptions:        ����������Ӧ����
** input parameters:    
** output parameters:   ��
** Returned value:      ��
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
		case 0x01:	/*��һ������������ת90������ת90��*/
					action_control(&LG_angle[0],&G_Zero[0],(u16 *)&G_Zero[0],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;				
					};
					break;
		case 0x02:	/*�ڶ�������������ת180������ת180��*/
					action_control(&LG_angle[2],&G_Zero[2],(u16 *)&G_Zero[2],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;
					};
					break;
		case 0x03:	/*�����������ƽ����������*/
					if(RandomCountBuf<35){							//35%����
						Single_Action(-360,0,0); 				//��������1��
					}else{
						if(RandomCountBuf<65){						//30%����
							Single_Action(-1080,0,0);			//��������3��
						}else{
							if(RandomCountBuf<80){					//15%����
								Single_Action(-1800,0,0);		//��������5��
							}else{
								if(RandomCountBuf<95){				//15%����
									Single_Action(-3600,0,0);	//��������10��
									
								}else{							// 5%����
									Control_flag.Left = 1;		//һֱ��������
									APDS9960.gesture = 0;
									m_sound_type=END_SOUND;sound_update=true;
								}
								gesture_lefttimes= 0;			//��������
							}
						}
					}
					if(Control_flag.SingleAction)//�������
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
** Descriptions:        ����������Ӧ����
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Dir_RightResult(void)
{

	switch(gesture_righttimes)
	{
		case 0x01:	/*��һ������������ת90������ת90��*/
					action_control(&RG_angle[0],&G_Zero[0],(u16 *)&G_Zero[0],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;
					};
					break;
		case 0x02:	/*�ڶ�������������ת180������ת180��*/
					action_control(&RG_angle[2],&G_Zero[2],(u16 *)&G_Zero[2],2);
					if(Control_flag.Much_Action)
					{
						APDS9960.gesture = 0;
						Control_flag.Much_Action = 0;
					};
					break;
		case 0x03:	/*�����������ƽ����������*/
					if(RandomCountBuf<35){							//35%����
						Single_Action(360,0,0); 				//��������1��
					}else{
						if(RandomCountBuf<65){						//30%����
							Single_Action(1080,0,0);			//��������3��
					
						}else{
							if(RandomCountBuf<80){					//15%����
								Single_Action(1800,0,0);		//��������5��
							
							}else{
								if(RandomCountBuf<95){				//15%����
									Single_Action(3600,0,0);	//��������10��
								}else{							// 5%����
									Control_flag.Right= 1;		//һֱ������ת
									APDS9960.gesture = 0;
									m_sound_type=END_SOUND;sound_update=true;
								}
								gesture_righttimes= 0;			//��������
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
** Descriptions:        ���ƽӽ���Ӧ����
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Dir_NearResult(void)
{

	if(RandomCountBuf<35){							//35%����
		action_control(&G_Zero[0],&BK_Distance[0],(u16 *)&G_Zero[0],2); 				//�����0.5m
	}else{
		if(RandomCountBuf<65){						//30%����
			action_control(&G_Zero[0],&BK_Distance[2],(u16 *)&G_Zero[0],2); 			//�����0.8m
		}else{
			if(RandomCountBuf<80){					//15%����
				action_control(&G_Zero[0],&BK_Distance[4],(u16 *)&G_Zero[0],2); 		//�����1.5m,ǰ��1��
			}else{
				if(RandomCountBuf<95){				//15%����
					action_control(&G_Zero[0],&BK_Distance[6],(u16 *)&G_Zero[0],2);		//���ʺ���2��ǰ��1.5�ף�
				}else{							// 5%����
					action_control(&G_Zero[0],&BK_Distance[8],(u16 *)&G_Zero[0],2);		//���ʺ���2��ǰ��1.5�ף�
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
** Descriptions:        ����Զ����Ӧ����
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Dir_FarResult(void)
{
	if(RandomCountBuf<35){							//35%����
		action_control(&G_Zero[0],&FK_Distance[0],(u16 *)&G_Zero[0],2); 				//�����0.5m
	}else{
		if(RandomCountBuf<65){						//30%����
			action_control(&G_Zero[0],&FK_Distance[2],(u16 *)&G_Zero[0],2); 			//�����0.8m
		}else{
			if(RandomCountBuf<80){					//15%����
				action_control(&G_Zero[0],&FK_Distance[4],(u16 *)&G_Zero[0],2); 		//�����1.5m,ǰ��1��
			}else{
				if(RandomCountBuf<95){				//15%����
					action_control(&G_Zero[0],&FK_Distance[6],(u16 *)&G_Zero[0],2);		//���ʺ���2��ǰ��1.5�ף�
				}else{							// 5%����
					action_control(&G_Zero[0],&FK_Distance[8],(u16 *)&G_Zero[0],2);		//���ʺ���2��ǰ��1.5�ף�
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
** Descriptions:        ���ƹ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Gesture_Funtion(void)
{ 
	//����Ч������(û�д������������Ч)
	if(APDS9960.flag == APDS_GESTURE)
	{
		switch(APDS9960.gesture)
		{	
			case DIR_DOWN:
				if(gesture_led_update == 1){Led_mode=DOWN_LED_MODE;}
				Control_flag.Stop = 1;
				break;//��������==�������
			case DIR_UP:
				if(gesture_led_update == 1){Led_mode=UP_LED_MODE;}				
				Control_flag.Stop = 0;
				break;//��������==�������
			case DIR_LEFT:	
				if(gesture_led_update == 1){ Led_mode=LEFT_LED_MODE; RandomCountBuf=RandomCount;if(gesture_lefttimes<3)	gesture_lefttimes++;}
				Dir_LeftResult();
				break;//��������Ч��
			case DIR_RIGHT:	
				if(gesture_led_update == 1){Led_mode=RIGHT_LED_MODE; RandomCountBuf=RandomCount;if(gesture_righttimes<3)gesture_righttimes++;}
				Dir_RightResult();
				break;//��������Ч��
			case DIR_NEAR:
				if(gesture_led_update == 1){Led_mode=FLASH_LED_MODE; RandomCountBuf=RandomCount;}
				Dir_NearResult();
				break;//�ӽ�����Ч��
			case DIR_FAR:
				if(gesture_led_update == 1){Led_mode=FLASH_LED_MODE; RandomCountBuf=RandomCount;}					
				Dir_FarResult();
				break;//Զ������Ч��
			default:
				APDS9960.gesture = 0;
				break;
		}
		gesture_led_update=0;
	}
}

/*********************************************************************************************************
** Function name:       Follow_Funtion
** Descriptions:        ���湦��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Follow_Funtion(u32 Distance)
{	
//	static u32 distance_old;	//��һʱ�̾���
//	static u32 distance_new;	//����ʱ�̾���
//	static u32 bias_old;		//��һʱ��ƫ��
//	static u32 bias_new;		//����ʱ��ƫ��
	
	if(Control_flag.Qian==0 &&  Control_flag.Hou==0)
	{
		if(APDS9960.proximity>240)
		{
			Control_flag.direction=1;
		}
		
//		distance_old = distance_new;			//��һ�̾���
//		distance_new = Distance;				//��ǰ����
//		bias_old = bias_new;					//��һ��ƫ��
//		bias_new = distance_new-distance_old;	//��ǰƫ��
		
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
** Descriptions:        ���Ϲ��ܺ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        ���Թ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Touch_Funtion(void)
{
	//����Ч������
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
						break;//�ҳ����Ч��==��ת60��
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
						break;//���Ĵ���Ч��==ǰ��200mm
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
						break;//ǰ���Ӵ���Ч��==����200mm
		default:
			break;
	}
	PreControl_Touch=Control_Touch;
}














