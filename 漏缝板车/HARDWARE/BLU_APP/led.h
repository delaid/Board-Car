#ifndef __LED_H
#define __LED_H
#include "sys.h"


#define Control_Led_Blue PCout(4)
#define Night_Led_Yellow PAout(6)
#define Dance_Led_Purple PAout(4)
#define Drawpath_Led_Green PAout(7)
#define Mod_Led_White 	 PAout(5)



#define CONTROL_LED_BLUE {Mod_Led_White = 0;Drawpath_Led_Green =1;Control_Led_Blue = 0;Dance_Led_Purple = 1;Night_Led_Yellow = 1;}
#define NIGHT_LED_YELLOW {Mod_Led_White = 0;Drawpath_Led_Green =1;Control_Led_Blue = 1;Dance_Led_Purple = 1;Night_Led_Yellow = 0;}
#define DANCE_LED_PURPLE {Mod_Led_White = 0;Drawpath_Led_Green =1;Control_Led_Blue = 1;Dance_Led_Purple = 0;Night_Led_Yellow = 1;}
#define DRAWPATH_LED_GREEN {Mod_Led_White = 0;Drawpath_Led_Green =0;Control_Led_Blue = 1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;}
#define LED_MOD_COLSE	{Mod_Led_White = 1;Drawpath_Led_Green =1;Control_Led_Blue = 1;Dance_Led_Purple = 1;Night_Led_Yellow = 1;}


#define RGB_BLUE			PAout(10)
#define RGB_GREEN			PAout(11)
#define RGB_RED				PAout(12)

#define RGB_LIGHT_BLUE		{RGB_BLUE = 0;RGB_GREEN = 1;RGB_RED = 1;}
#define RGB_LIGHT_GREEN		{RGB_BLUE = 1;RGB_GREEN = 0;RGB_RED = 1;}
#define RGB_LIGHT_RED		{RGB_BLUE = 1;RGB_GREEN = 1;RGB_RED = 0;}
#define RGB_LIGHT_PINK		{RGB_BLUE = 0;RGB_GREEN = 1;RGB_RED = 0;}
#define RGB_LIGHT_YELLOW	{RGB_BLUE = 1;RGB_GREEN = 0;RGB_RED = 0;}
#define RGB_LIGHT_CYAN		{RGB_BLUE = 0;RGB_GREEN = 0;RGB_RED = 1;}
#define RGB_LIGHT_WHITE		{RGB_BLUE = 0;RGB_GREEN = 0;RGB_RED = 0;}
#define RGB_LIGHT_BLACK		{RGB_BLUE = 1;RGB_GREEN = 1;RGB_RED = 1;}

#define LED_LEFT_TIME 125
#define LED_RIGHT_TIME 125
#define	LED_FLASH_TIME 125
#define LED_OK_TIME	250
#define LED_CLOSE               0x00
#define LEFT_LED_MODE           0x01
#define RIGHT_LED_MODE          0x02
#define UP_LED_MODE             0x03
#define DOWN_LED_MODE           0x04
#define FLASH_LED_MODE          0x05
#define OK_LED_MODE             0x06
#define CONTROL_LED_MODE        0x07
#define NIGHT_LIGHT_LED_MODE    0x08
#define DANCE_LIGHT_LED_MODE    0x09
#define DRAWPATH_LED_MODE       0x0A
#define DANCE_LED_FAST          0x0B
#define DANCE_LED_SLOW          0x0C
#define DANCE_LED_SHAKE         0x0D

extern volatile u8 Led_mode;
/*********************************************************************************************************
** Function name:       Led_Init
** Descriptions:        左右Led灯开关初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Night_Led_Init(void);
void Mod_Led_Init(void);
void LED_Brightness(u8 brightness);
void RGB_Led_Init(void);
void Led_task_Create(void);
#endif







