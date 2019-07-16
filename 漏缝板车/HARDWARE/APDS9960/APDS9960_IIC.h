#ifndef __APDS9960_IIC_H
#define __APDS9960_IIC_H
#include "sys.h"


#define write_bit 		0x00
#define read_bit  		0x01
#define send_noack		0x00
#define send_ack  		0x01






//IO方向设置
 
#define SDA_IN()  {GPIOB->CRH&=0xFFFFFF0F;GPIOB->CRH|=(u32)8<<(1*4);}
#define SDA_OUT() {GPIOB->CRH&=0xFFFFFF0F;GPIOB->CRH|=(u32)3<<(1*4);}

//IO操作函数	 
#define APDS9960_IIC_SCL    PBout(8) //SCL
#define APDS9960_IIC_SDA    PBout(9) //SDA	 
#define READ_SDA   PBin(9)  //输入SDA 

//IIC所有操作函数
void APDS9960_IIC_Init(void);                //初始化IIC的IO口				 
void APDS9960_IIC_Start(void);				//发送IIC开始信号
void APDS9960_IIC_Stop(void);	  			//发送IIC停止信号
void APDS9960_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 APDS9960_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 APDS9960_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void APDS9960_IIC_Ack(void);					//IIC发送ACK信号
void APDS9960_IIC_NAck(void);				//IIC不发送ACK信号

void APDS9960_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 APDS9960_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















