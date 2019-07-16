#ifndef __DANCE_H
#define __DANCE_H
#include "sys.h"


#define DANCE_FAST 		0x01//������赸
#define DANCE_SLOW 		0x02//�������赸
#define DANCE_CUSTOM	0x03//�Զ����赸
#define DANCE_STOP		0x04
#define DANCE_SHAKE		0x05//ǰ��ҡ��
#define SHAKEMAXE		0x05//ҡ��������


extern volatile uint8_t Dance_Flag;
extern volatile uint8_t shakeparam;
void Dance_Clear(void);


void Dance_Mode(void);	//����ģʽ
void Set_Dance(void);	//��������ģʽ״̬
void Read_Dance(void);	//��ȡ��ǰ����ģʽ״̬
void Custom_Dance(void);//�Զ�������
void Fast_Dance(void);
void Slow_Dance(void);
void FrontBack_Shake(void);
uint8_t Dance_DP(void); //�Զ����������ݴ��� 

#endif
/********************************************END_DANCE_H_FILE********************************************/
