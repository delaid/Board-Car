#include "nightlight.h"

nightlight_type m_nightlight;
/*********************************************************************************************************
** Function name:       NightLight_Mode
** Descriptions:        夜光模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void NightLight_Mode(void)
{
	if(NIGHTLIGHT == Car_Mode)
	{
		Control_flag.Stop = 1;
		APDS9960.flag = APDS_LIGHT;//APDS9960开启环境光模式
	}
	
}

/*********************************************************************************************************
** Function name:       Set_NightLight
** Descriptions:        设置夜光模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_NightLight(void)
{
	
	if((USART3_RX_STA&0xFF)==8)
	{
		if(USART3_RX_BUF[4] == 3)
		{
			m_nightlight.mode = OPEN_LIGHT;//灯常开
			m_nightlight.brightness=50;
		}
		else if(USART3_RX_BUF[4] == 4)
		{
			m_nightlight.mode = CLOSE_LIGHT;//灯常关
		}
		else if(USART3_RX_BUF[4] == 1)
		{
			m_nightlight.mode = OPEN_BODY_IR;//自控灯开
			m_nightlight.brightness=50;
		}
		else if(USART3_RX_BUF[4] == 2)
		{
			m_nightlight.mode = CLOSE_BODY_IR;//自控灯关
		}
		//sound_update= true;m_sound_type=MODE_CHANGE_SOUND;//声音开关
	}
	else if((USART3_RX_STA&0xFF)==9 && USART3_RX_BUF[4] == 5)
		{
			m_nightlight.brightness = USART3_RX_BUF[5];
		}
		else{
			Send_ERROR();//接收到错误信息
		}
}


