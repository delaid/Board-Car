#include "MPU6050_IIC.h"

/*
********************************************************************************
** �������� �� Delay(vu32 nCount)
** �������� �� ��ʱ����
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
 void Delay_us(u32 nCount)
{
  for(; nCount != 0; nCount--);
}

/*
********************************************************************************
** �������� �� void Delayms(vu32 m)
** �������� �� ����ʱ����	 m=1,��ʱ1ms
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
 void Delayms(u32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}


//��ʼ��IIC
void MPU6050_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);//
}		   
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
int MPU6050_IIC_Start(void)
{
	MPU6050_SDA_OUT();     //MPU6050_SDA�����
	MPU6050_IIC_SDA=1;
	if(!MPU6050_READ_SDA)return 0;	
	MPU6050_IIC_SCL=1;
	Delay_us(1);
 	MPU6050_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	if(MPU6050_READ_SDA)return 0;
	Delay_us(1);
	MPU6050_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
void MPU6050_IIC_Stop(void)
{
	MPU6050_SDA_OUT();//MPU6050_SDA�����
	MPU6050_IIC_SCL=0;
	MPU6050_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_us(1);
	MPU6050_IIC_SCL=1; 
	MPU6050_IIC_SDA=1;//����I2C���߽����ź�
	Delay_us(1);							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 MPU6050_IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
u8 MPU6050_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU6050_SDA_IN();      //MPU6050_SDA����Ϊ����  
	MPU6050_IIC_SDA=1;
	Delay_us(1);	   
	MPU6050_IIC_SCL=1;
	Delay_us(1);	 
	while(MPU6050_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			MPU6050_IIC_Stop();
			return 0;
		}
	  Delay_us(1);
	}
	MPU6050_IIC_SCL=0;//ʱ�����0 	   
	return 1;  
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
void MPU6050_IIC_Ack(void)
{
	MPU6050_IIC_SCL=0;
	MPU6050_SDA_OUT();
	MPU6050_IIC_SDA=0;
	Delay_us(1);
	MPU6050_IIC_SCL=1;
	Delay_us(1);
	MPU6050_IIC_SCL=0;
}
	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
void MPU6050_IIC_NAck(void)
{
	MPU6050_IIC_SCL=0;
	MPU6050_SDA_OUT();
	MPU6050_IIC_SDA=1;
	Delay_us(1);
	MPU6050_IIC_SCL=1;
	Delay_us(1);
	MPU6050_IIC_SCL=0;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_IIC_Send_Byte(u8 txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/		  
void MPU6050_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MPU6050_SDA_OUT(); 	    
    MPU6050_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MPU6050_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay_us(1);   
		MPU6050_IIC_SCL=1;
		Delay_us(1); 
		MPU6050_IIC_SCL=0;	
		Delay_us(1);
    }	 
} 	 
  
/**************************ʵ�ֺ���********************************************
*����ԭ��:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
*��������:		
*******************************************************************************/
int MPU6050_i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
		int i;
    if (!MPU6050_IIC_Start())
        return 1;
    MPU6050_IIC_Send_Byte(addr << 1 );
    if (!MPU6050_IIC_Wait_Ack()) {
        MPU6050_IIC_Stop();
        return 1;
    }
    MPU6050_IIC_Send_Byte(reg);
    MPU6050_IIC_Wait_Ack();
		for (i = 0; i < len; i++) {
        MPU6050_IIC_Send_Byte(data[i]);
        if (!MPU6050_IIC_Wait_Ack()) {
            MPU6050_IIC_Stop();
            return 0;
        }
    }
    MPU6050_IIC_Stop();
    return 0;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
*��������:		
*******************************************************************************/
int MPU6050_i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!MPU6050_IIC_Start())
        return 1;
    MPU6050_IIC_Send_Byte(addr << 1);
    if (!MPU6050_IIC_Wait_Ack()) {
        MPU6050_IIC_Stop();
        return 1;
    }
    MPU6050_IIC_Send_Byte(reg);
    MPU6050_IIC_Wait_Ack();
    MPU6050_IIC_Start();
    MPU6050_IIC_Send_Byte((addr << 1)+1);
    MPU6050_IIC_Wait_Ack();
    while (len) {
        if (len == 1)
            *buf = MPU6050_IIC_Read_Byte(0);
        else
            *buf = MPU6050_IIC_Read_Byte(1);
        buf++;
        len--;
    }
    MPU6050_IIC_Stop();
    return 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 MPU6050_IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
u8 MPU6050_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU6050_SDA_IN();//MPU6050_SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MPU6050_IIC_SCL=0; 
        Delay_us(2);
		MPU6050_IIC_SCL=1;
        receive<<=1;
        if(MPU6050_READ_SDA)receive++;   
		Delay_us(2); 
    }					 
    if (ack)
        MPU6050_IIC_Ack(); //����ACK 
    else
        MPU6050_IIC_NAck();//����nACK  
    return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
unsigned char MPU6050_I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	MPU6050_IIC_Start();	
	MPU6050_IIC_Send_Byte(I2C_Addr);	   //����д����
	res++;
	MPU6050_IIC_Wait_Ack();
	MPU6050_IIC_Send_Byte(addr); res++;  //���͵�ַ
	MPU6050_IIC_Wait_Ack();	  
	//MPU6050_IIC_Stop();//����һ��ֹͣ����	
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(I2C_Addr+1); res++;          //�������ģʽ			   
	MPU6050_IIC_Wait_Ack();
	res=MPU6050_IIC_Read_Byte(0);	   
    MPU6050_IIC_Stop();//����һ��ֹͣ����

	return res;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/ 
u8 MPU6050_IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(dev);	   //����д����
	MPU6050_IIC_Wait_Ack();
	MPU6050_IIC_Send_Byte(reg);   //���͵�ַ
    MPU6050_IIC_Wait_Ack();	  
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(dev+1);  //�������ģʽ	
	MPU6050_IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=MPU6050_IIC_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=MPU6050_IIC_Read_Byte(0);	 //���һ���ֽ�NACK
	}
    MPU6050_IIC_Stop();//����һ��ֹͣ����
    return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
u8 MPU6050_IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(dev);	   //����д����
	MPU6050_IIC_Wait_Ack();
	MPU6050_IIC_Send_Byte(reg);   //���͵�ַ
    MPU6050_IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		MPU6050_IIC_Send_Byte(data[count]); 
		MPU6050_IIC_Wait_Ack(); 
	 }
	MPU6050_IIC_Stop();//����һ��ֹͣ����

    return 1; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
u8 MPU6050_IICreadByte(u8 dev, u8 reg, u8 *data){
	*data=MPU6050_I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/ 
unsigned char MPU6050_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return MPU6050_IICwriteBytes(dev, reg, 1, &data);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 MPU6050_IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (MPU6050_IICreadByte(dev, reg, &b) != 0) {
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return MPU6050_IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 MPU6050_IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    MPU6050_IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return MPU6050_IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------




