#ifndef __APDS9960_IIC_H
#define __APDS9960_IIC_H
#include "sys.h"


#define write_bit 		0x00
#define read_bit  		0x01
#define send_noack		0x00
#define send_ack  		0x01






//IO��������
 
#define SDA_IN()  {GPIOB->CRH&=0xFFFFFF0F;GPIOB->CRH|=(u32)8<<(1*4);}
#define SDA_OUT() {GPIOB->CRH&=0xFFFFFF0F;GPIOB->CRH|=(u32)3<<(1*4);}

//IO��������	 
#define APDS9960_IIC_SCL    PBout(8) //SCL
#define APDS9960_IIC_SDA    PBout(9) //SDA	 
#define READ_SDA   PBin(9)  //����SDA 

//IIC���в�������
void APDS9960_IIC_Init(void);                //��ʼ��IIC��IO��				 
void APDS9960_IIC_Start(void);				//����IIC��ʼ�ź�
void APDS9960_IIC_Stop(void);	  			//����IICֹͣ�ź�
void APDS9960_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 APDS9960_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 APDS9960_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void APDS9960_IIC_Ack(void);					//IIC����ACK�ź�
void APDS9960_IIC_NAck(void);				//IIC������ACK�ź�

void APDS9960_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 APDS9960_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















