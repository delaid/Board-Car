#ifndef __MPU6050_IIC_H
#define __MPU6050_IIC_H
#include "sys.h"



#define write_bit 		0x00
#define read_bit  		0x01
#define send_noack		0x00
#define send_ack  		0x01


//IO��������
 
#define MPU6050_SDA_IN()  {	GPIOE->CRH&=0XF0FFFFFF;GPIOE->CRH|=(u32)8<<(6*4);}
#define MPU6050_SDA_OUT() {	GPIOE->CRH&=0XF0FFFFFF;GPIOE->CRH|=(u32)3<<(6*4);}//P15 �������  

//IO��������	 
#define MPU6050_IIC_SCL    PEout(15) //SCL
#define MPU6050_IIC_SDA    PEout(14) //SDA	 
#define MPU6050_READ_SDA   PEin(14)  //����SDA 
 
//#define  MPU6050_READ_SDA       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15)
// 
//#define  MPU6050_IIC_SCL_HIGH   GPIO_SetBits(GPIOE,GPIO_Pin_14)
//#define  MPU6050_IIC_SCL_LOW    GPIO_ResetBits(GPIOE,GPIO_Pin_14)

//#define  MPU6050_IIC_SDA_HIGH   GPIO_SetBits(GPIOE,GPIO_Pin_15)
//#define  MPU6050_IIC_SDA_LOW    GPIO_ResetBits(GPIOE,GPIO_Pin_15)




//IIC���в�������
void MPU6050_IIC_Init(void);                	//��ʼ��IIC��IO��				 
int MPU6050_IIC_Start(void);					//����IIC��ʼ�ź�
void MPU6050_IIC_Stop(void);	  				//����IICֹͣ�ź�
void MPU6050_IIC_Send_Byte(u8 txd);				//IIC����һ���ֽ�
u8 MPU6050_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 MPU6050_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU6050_IIC_Ack(void);						//IIC����ACK�ź�
void MPU6050_IIC_NAck(void);					//IIC������ACK�ź�

extern  void Delayms(u32 m);
extern  void Delay_us(u32 nCount);
	
unsigned char MPU6050_I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
           u8 MPU6050_IICreadByte(u8 dev, u8 reg, u8 *data);
		   u8 MPU6050_IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);
unsigned char MPU6050_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
           u8 MPU6050_IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
		   u8 MPU6050_IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data);
           u8 MPU6050_IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
		   
int MPU6050_i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int MPU6050_i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


#endif

