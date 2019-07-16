#ifndef __USART2_H
#define __USART2_H
#include "sys.h" 


#define USART2_REC_LEN  			100  	//�����������ֽ��� 200
/*******************************************************************************
Э���ʽ��

0xFF + 0x01 +����(1byte)+ ��Ч������С��0-31��

���ݣ�01  ��Ӧ    Pulcino Pio - El Pollito Pio (Radio Edit)1.mp3

02  ��Ӧ    ����˶���ʼ�� 5.wav

03  ��Ӧ    ������ װ�ɰ� 6.wav

04  ��Ӧ    �����󱳽�������-7.wav

05  ��Ӧ    ����ǰ����ϲ-8.wav

06  ��Ӧ    �����ҳ�����-9.wav

07  ��Ӧ    �����ҳ����-10.wav

08  ��Ӧ    �����ҳ������-11.wav

09 	��Ӧ    �����������-12.wav

10  ��Ӧ    ����������-13.wav

11  ��Ӧ    ������������-14.wav

12  ��Ӧ    �켣�յ㷢�� 4.wav

13  ��Ӧ    ��ɫ��觺�  ���赸.mp3

14  ��Ӧ    ģʽ�л�Ч���� 2.wav

15  ��Ӧ    ҹ��ģʽ������ 3.wav
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
//����봮���жϽ��գ��벻Ҫע�����º궨��
void usart2_init(u32 bound);

#endif


