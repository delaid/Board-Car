#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//S24R1 spi2接口
#define   S24R1_CLK_PIN     GPIO_Pin_13 //PB13    GPIO_Pin_0
#define   S24R1_CLK_PORT		GPIOB

#define   S24R1_MISO_PIN    GPIO_Pin_14 //PB13    GPIO_Pin_0
#define   S24R1_MISO_PORT		GPIOB

#define   S24R1_MOSI_PIN    GPIO_Pin_15 //PB13    GPIO_Pin_0
#define   S24R1_MOSI_PORT		GPIOB

void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
#endif

