#include "Key_task.h"

#define TOUCH_KEY_TASK_PRIO					2					//���ô����������ȼ�
#define TOUCH_KEY_STK_SIZE					64					//���ô��������ջ��С
OS_STK TOUCH_KEY_TASK_STK[TOUCH_KEY_STK_SIZE]; 			  		//���������ջ
void touch_key_task(void *pdata);								//����������

OS_EVENT * msg_key;			//���������¼���	 
OS_EVENT * q_msg;			//��Ϣ����
OS_FLAG_GRP * flags_key;	//�����ź�����

void * MsgGrp[32];			//��Ϣ���д洢��ַ,���֧��256����Ϣ

void Key_task_Create(void)
{   
	msg_key=OSMboxCreate((void*)0);		//������Ϣ����
	q_msg=OSQCreate(&MsgGrp[0],32);	//������Ϣ����
	OSTaskCreate(touch_key_task,(void*)0,(OS_STK*)&TOUCH_KEY_TASK_STK[TOUCH_KEY_STK_SIZE-1],TOUCH_KEY_TASK_PRIO);
}

bool power_low_flag=false;
/*********************************************************************************************************
** Function name:       touch_key_task
** Descriptions:        �����������ɼ������ݣ�
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
//������������
void touch_key_task(void *pdata)
{
	u8 key;	
	while(1)
	{
       key=key_scan(0);
	   if(key)OSMboxPost(msg_key,(void*)key);//������Ϣ
 	   delay_ms(20);
	}
}
/*********************************************************************************************************
** Function name:       KeyScan_healder
** Descriptions:        ������������������
** input parameters:  
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void  KeyScan_healder(void)
{ 
	uint32_t key=0;	
	uint8_t  err;
	key=(u32)OSMboxPend(msg_key,10,&err); //��ȡ�����ź���

	static bool Mode_Enter_Flag=0;//����ģʽѡ���־λ
	static uint32_t m_key_count=0;//���ڼ����û�а��¼���
	static uint16_t power_led_count=0;//���ڵ�ѹRGB���������˸
	static uint8_t  pre_m=0;//��¼��ǰ���µ�״ֵ̬
	static uint8_t  m  = 0;//��ʼ������״ֵ̬
	static uint8_t  m_Ulta_Gesture_count;//����ģʽ���л���־
	static uint8_t  m_Dance_Fast_Slow_count;//����ģʽ���л���־
	static uint8_t  m_Night_light_count;//ҹ��ģʽ���л���־
	
	switch (key)
	{
		case KEY_LEFT_PRES:
			if(Mode_Enter_Flag==true) /*����ģʽѡ���״ֵ̬����*/
			{
				if(m<4) m++; 
				else m=1;
			}
			if(DANCE == Car_Mode && DANCE_SHAKE == Dance_Flag)/*ҡ�η��ȵ���*/
			{
				if(shakeparam<SHAKEMAXE){
					shakeparam++;
				}
			}
			break;
		case KEY_RIGHT_PRES:
			if(Mode_Enter_Flag==true){ /*����ģʽѡ���״ֵ̬�ݼ�*/
				if(m>1)	m--; 
				else    m=4;
			}
			if(DANCE == Car_Mode && DANCE_SHAKE == Dance_Flag)/*ҡ�η��ȵݼ�*/
			{
				if(shakeparam>0){
					shakeparam--;
				}
			}
			break;
		case KEY_MOD_PRES:/*����ģʽѡ��״̬*/
			Mode_Enter_Flag=!Mode_Enter_Flag;
			if(Mode_Enter_Flag==true)
				m = 1;
			else
			{
				m = 0;
				Led_mode=LED_CLOSE;					
			}
			break;
		case KEY_FOUNCTION_PRES:/*����ģʽ��ѡ����*/
			if(Car_Mode==CONTROL)/*����ģʽ�¹���ѡ��*/
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
			else if(Car_Mode==DANCE)/*�����蹦����ѡ��*/
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
			else if(Car_Mode==NIGHTLIGHT)/*ҹ��ģʽ�¹���ѡ��*/
			{
				Led_mode=OK_LED_MODE;
				
			//	if(m_Night_light_count==0){m_nightlight.mode=CLOSE_LIGHT;}
			    if(m_Night_light_count==0){m_nightlight.mode=OPEN_LIGHT;m_nightlight.brightness=50;m_Night_light_count=1;}
				else{m_nightlight.mode=OPEN_BODY_IR;m_nightlight.brightness=50;m_Night_light_count=0;}
				
			}
			break;
		case KEY_LIGHT_ADD:  if(Car_Mode==NIGHTLIGHT){if(m_nightlight.brightness<100)m_nightlight.brightness++;}/*�ƹ����ȵ���*/
			break;
		case KEY_LIGHT_DECRE:if(Car_Mode==NIGHTLIGHT){if(m_nightlight.brightness>0)m_nightlight.brightness--;}/*�ƹ����ȵݼ�*/
			break;
		default:
			break;
	 }
	 
	 
	if(pre_m!=m){//ģʽ���³ɹ�
	  m_key_count=0;
	  Mode_update=1;
	}
	pre_m=m;
	  
	if(Mode_Enter_Flag==true)//5Sû�в����Զ��˳�
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
	 
	 
	
	if(Mode_Enter_Flag==0)/*�˳�ģʽѡ��*/
	{
        
		if(socket_swtich==false)//û�в������
		{ 
			if(power_low==true )//��ص�ѹ��ͷ�����һ��һ��
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
					switch(Car_Mode)//ģʽ��
					{
						case MODE:		RGB_LIGHT_WHITE;break;//�׹�==ģʽѡ��
						case DRAWPATH:	RGB_LIGHT_GREEN;break;//��==·��ģʽ
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
						break;//��==����ģʽ
						case CONTROL:	RGB_LIGHT_BLUE;break;//��ɫ==ҡ��ģʽ
						case NIGHTLIGHT:RGB_LIGHT_YELLOW;break;//��ɫ==ҹ��ģʽ			
					}
				}
			}
		}
		else//���������ͷ���̵�һ��һ��
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
		/*********����ֵ��ȡ*********/
		switch (key)
		{
			case KEY_LEFT_PRES:		Control_Touch = Touch_Left; Control_Touch_Update =true; break;
			case KEY_RIGHT_PRES:	Control_Touch = Touch_Right;Control_Touch_Update =true; break;
			case KEY_BACK_PRES:		Control_Touch = Touch_Back; Control_Touch_Update =true; break;
			case KEY_FORWARD_PRES:	Control_Touch = Touch_Front;Control_Touch_Update =true; break;
		}

	}
	else/*����ģʽѡ��*/
	{
		switch(m)
		{
			case 1:	Car_Mode=CONTROL;RGB_LIGHT_BLUE;Led_mode=CONTROL_LED_MODE;m_Ulta_Gesture_Touch=D_GESTUER_ON;Gesture_Switch(ON);break;//��ɫ==ҡ��ģʽ
			case 2:	Car_Mode=NIGHTLIGHT;RGB_LIGHT_YELLOW;Led_mode=NIGHT_LIGHT_LED_MODE;m_nightlight.mode=OPEN_BODY_IR;m_nightlight.brightness=50;break;//��ɫ==ҹ��ģʽ ,���ͳ���
			case 3:	Car_Mode=DANCE;RGB_LIGHT_PINK;Led_mode=DANCE_LIGHT_LED_MODE;break;//��==����ģʽ
			case 4: Car_Mode=DRAWPATH;RGB_LIGHT_GREEN;Led_mode=DRAWPATH_LED_MODE;break;//��==·��ģʽ
			default:
				break;		
		}

	}
	

}	

	




