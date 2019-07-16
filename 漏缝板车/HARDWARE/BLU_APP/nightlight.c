#include "nightlight.h"

nightlight_type m_nightlight;
/*********************************************************************************************************
** Function name:       NightLight_Mode
** Descriptions:        ҹ��ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void NightLight_Mode(void)
{
	if(NIGHTLIGHT == Car_Mode)
	{
		Control_flag.Stop = 1;
		APDS9960.flag = APDS_LIGHT;//APDS9960����������ģʽ
	}
	
}

/*********************************************************************************************************
** Function name:       Set_NightLight
** Descriptions:        ����ҹ��ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_NightLight(void)
{
	
	if((USART3_RX_STA&0xFF)==8)
	{
		if(USART3_RX_BUF[4] == 3)
		{
			m_nightlight.mode = OPEN_LIGHT;//�Ƴ���
			m_nightlight.brightness=50;
		}
		else if(USART3_RX_BUF[4] == 4)
		{
			m_nightlight.mode = CLOSE_LIGHT;//�Ƴ���
		}
		else if(USART3_RX_BUF[4] == 1)
		{
			m_nightlight.mode = OPEN_BODY_IR;//�Կصƿ�
			m_nightlight.brightness=50;
		}
		else if(USART3_RX_BUF[4] == 2)
		{
			m_nightlight.mode = CLOSE_BODY_IR;//�Կصƹ�
		}
		//sound_update= true;m_sound_type=MODE_CHANGE_SOUND;//��������
	}
	else if((USART3_RX_STA&0xFF)==9 && USART3_RX_BUF[4] == 5)
		{
			m_nightlight.brightness = USART3_RX_BUF[5];
		}
		else{
			Send_ERROR();//���յ�������Ϣ
		}
}


