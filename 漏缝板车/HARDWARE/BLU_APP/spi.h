#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//S24R1 spi2�ӿ�
#define   S24R1_CLK_PIN     GPIO_Pin_13 //PB13    GPIO_Pin_0
#define   S24R1_CLK_PORT		GPIOB

#define   S24R1_MISO_PIN    GPIO_Pin_14 //PB13    GPIO_Pin_0
#define   S24R1_MISO_PORT		GPIOB

#define   S24R1_MOSI_PIN    GPIO_Pin_15 //PB13    GPIO_Pin_0
#define   S24R1_MOSI_PORT		GPIOB

void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

