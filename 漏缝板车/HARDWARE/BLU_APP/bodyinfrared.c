#include "bodyinfrared.h"

bool Mode_Select_Flag = 0;//ģʽѡ���־��1��������ģʽѡ�� 0���˳���ģʽѡ��
/*********************************************************************************************************
** Function name:       Bodyinfrared_Init
** Descriptions:        ��ʼ�����������ģ��ļ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Bodyinfrared_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);								//PORTDʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;											//PD0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 										//���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);												//��ʼ��IO����	
}


/*********************************************************************************************************
** Function name:       BodyDetection
** Descriptions:        ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BodyDetection(void)
{ 
	
	//����ҹ��ģʽ�£����˳���ģʽѡ����ҹ����Ч��������Ч
	 switch(m_nightlight.mode)
	{
		case OPEN_LIGHT:LED_Brightness(m_nightlight.brightness);break;
		case CLOSE_LIGHT:LED_Brightness(0);break;
		case OPEN_BODY_IR:
			if((NIGHTLIGHT == Car_Mode) && (APDS9960.flag == APDS_LIGHT))
			{
				if(APDS9960.whitelight <= 100 && bodydetection ==1)
				{
					LED_Brightness(m_nightlight.brightness);m_nightlight.time=500;
				}
				else if(m_nightlight.time==0 || APDS9960.whitelight>100)LED_Brightness(0);
			}
			break;
		case CLOSE_BODY_IR:
			break;
	    default:
			break;
	
	}
    
	
}
/*********************************************************************************************************
** Function name:       BodyDetection
** Descriptions:        �����⣬����Ӽ�ⲻ�����Զ��ػ���Ϣ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void BodyDetection_Sleep(void)
{

	if(bodydetection ==1)
	{
		Body_time=0;
	}
	else if(Body_time>18000)//30����û�м�⵽�����Զ�����
	{
		Body_time=0;
		control_sleep= true;
		sound_update= true;
		m_sound_type=STOP_SOUND;
	}

}


