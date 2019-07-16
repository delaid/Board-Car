#ifndef __USART2_H
#define __USART2_H
#include "sys.h" 


#define USART2_REC_LEN  			100  	//定义最大接收字节数 200
/*******************************************************************************
协议格式：

0xFF + 0x01 +数据(1byte)+ 音效音量大小（0-31）

数据：01  对应    Pulcino Pio - El Pollito Pio (Radio Edit)1.mp3

02  对应    编程运动开始音 5.wav

03  对应    触摸后背 装可爱 6.wav

04  对应    触摸后背骄傲自满-7.wav

05  对应    触摸前部惊喜-8.wav

06  对应    触摸右翅膀紧张-9.wav

07  对应    触摸右翅膀开心-10.wav

08  对应    触摸右翅膀生气-11.wav

09 	对应    触摸左翅膀紧张-12.wav

10  对应    触摸左翅膀开心-13.wav

11  对应    触摸左翅膀生气-14.wav

12  对应    轨迹终点发音 4.wav

13  对应    蓝色多瑙河  慢舞蹈.mp3

14  对应    模式切换效果音 2.wav

15  对应    夜灯模式开启音 3.wav
********************************************************************************/
#define CHEICKET_SOUND			0x00
#define PROGRAME_SOUND 			0x01
#define BACK_LOVELY_SOUND		0x02
#define BACK_PRIDE_SOUND		0x03
#define FORN_SURPRISED_SOUND	0x04
#define RIGHT_NEVERS_SOUND		0x05
#define RIGHT_HAPPY_SOUND		0x06
#define RIGHT_ANGRY_SOUND 		0x07
#define LEFT_NEVERS_SOUND		0x08
#define LEFT_HAPPY_SOUND		0x09
#define LEFT_ANGERY_SOUND		0x0A
#define END_SOUND				0x0B
#define BLUE_RIVER_SOUND		0x0C
#define MODE_CHANGE_SOUND 		0x0D
#define NIGHT_SOUND				0x0E
#define STOP_SOUND            	0x0F
extern u8 m_sound_type;
extern bool sound_update; 
extern void Music_Sound(void);
//如果想串口中断接收，请不要注释以下宏定义
void usart2_init(u32 bound);

#endif


