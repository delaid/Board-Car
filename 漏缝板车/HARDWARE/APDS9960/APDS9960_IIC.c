#include "APDS9960_IIC.h"
#include "delay.h"

 
//��ʼ��IIC
void APDS9960_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_2); 	//PB1,PB2 �����
}
//����IIC��ʼ�ź�
void APDS9960_IIC_Start(void)
{
	SDA_OUT();     //sda�����
	APDS9960_IIC_SDA=1;	  	  
	APDS9960_IIC_SCL=1;
	delay_us(4);
 	APDS9960_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	APDS9960_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void APDS9960_IIC_Stop(void)
{
	SDA_OUT();//sda�����
	APDS9960_IIC_SCL=0;
	APDS9960_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	APDS9960_IIC_SCL=1; 
	APDS9960_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 APDS9960_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	APDS9960_IIC_SDA=1;delay_us(1);	   
	APDS9960_IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			APDS9960_IIC_Stop();
			return 1;
		}
	}
	APDS9960_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void APDS9960_IIC_Ack(void)
{
	APDS9960_IIC_SCL=0;
	SDA_OUT();
	APDS9960_IIC_SDA=0;
	delay_us(2);
	APDS9960_IIC_SCL=1;
	delay_us(2);
	APDS9960_IIC_SCL=0;
}
//������ACKӦ��		    
void APDS9960_IIC_NAck(void)
{
	APDS9960_IIC_SCL=0;
	SDA_OUT();
	APDS9960_IIC_SDA=1;
	delay_us(2);
	APDS9960_IIC_SCL=1;
	delay_us(2);
	APDS9960_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void APDS9960_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    APDS9960_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //APDS9960_IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			APDS9960_IIC_SDA=1;
		else
			APDS9960_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		APDS9960_IIC_SCL=1;
		delay_us(2); 
		APDS9960_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 APDS9960_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        APDS9960_IIC_SCL=0; 
        delay_us(2);
		APDS9960_IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        APDS9960_IIC_NAck();//����nACK
    else
        APDS9960_IIC_Ack(); //����ACK   
    return receive;
}



























