#include "bodyinfrared.h"

bool Mode_Select_Flag = 0;//模式选择标志（1：进入了模式选择 0：退出了模式选择）
/*********************************************************************************************************
** Function name:       Bodyinfrared_Init
** Descriptions:        初始化人体红外检测模块的检测引脚
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Bodyinfrared_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);								//PORTD时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;											//PD0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 										//设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);												//初始化IO引脚	
}


/*********************************************************************************************************
** Function name:       BodyDetection
** Descriptions:        人体检测
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void BodyDetection(void)
{ 
	
	//处在夜光模式下，且退出了模式选择下夜灯有效，否则无效
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
** Descriptions:        人体检测，半个钟检测不到人自动关机休息
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void BodyDetection_Sleep(void)
{

	if(bodydetection ==1)
	{
		Body_time=0;
	}
	else if(Body_time>18000)//30分钟没有检测到人体自动休眠
	{
		Body_time=0;
		control_sleep= true;
		sound_update= true;
		m_sound_type=STOP_SOUND;
	}

}


