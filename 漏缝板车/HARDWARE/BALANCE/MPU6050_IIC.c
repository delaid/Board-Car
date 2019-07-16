#include "MPU6050_IIC.h"

/*
********************************************************************************
** 函数名称 ： Delay(vu32 nCount)
** 函数功能 ： 延时函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delay_us(u32 nCount)
{
  for(; nCount != 0; nCount--);
}

/*
********************************************************************************
** 函数名称 ： void Delayms(vu32 m)
** 函数功能 ： 长延时函数	 m=1,延时1ms
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delayms(u32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}


//初始化IIC
void MPU6050_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);//
}		   
/**************************实现函数********************************************
*函数原型:		void MPU6050_IIC_Start(void)
*功　　能:		产生IIC起始信号
*******************************************************************************/
int MPU6050_IIC_Start(void)
{
	MPU6050_SDA_OUT();     //MPU6050_SDA线输出
	MPU6050_IIC_SDA=1;
	if(!MPU6050_READ_SDA)return 0;	
	MPU6050_IIC_SCL=1;
	Delay_us(1);
 	MPU6050_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	if(MPU6050_READ_SDA)return 0;
	Delay_us(1);
	MPU6050_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	return 1;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_IIC_Stop(void)
*功　　能:	    //产生IIC停止信号
*******************************************************************************/	  
void MPU6050_IIC_Stop(void)
{
	MPU6050_SDA_OUT();//MPU6050_SDA线输出
	MPU6050_IIC_SCL=0;
	MPU6050_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_us(1);
	MPU6050_IIC_SCL=1; 
	MPU6050_IIC_SDA=1;//发送I2C总线结束信号
	Delay_us(1);							   	
}

/**************************实现函数********************************************
*函数原型:		u8 MPU6050_IIC_Wait_Ack(void)
*功　　能:	    等待应答信号到来 
//返回值：1，接收应答失败
//        0，接收应答成功
*******************************************************************************/
u8 MPU6050_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU6050_SDA_IN();      //MPU6050_SDA设置为输入  
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
	MPU6050_IIC_SCL=0;//时钟输出0 	   
	return 1;  
} 

/**************************实现函数********************************************
*函数原型:		void MPU6050_IIC_Ack(void)
*功　　能:	    产生ACK应答
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
	
/**************************实现函数********************************************
*函数原型:		void MPU6050_IIC_NAck(void)
*功　　能:	    产生NACK应答
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
/**************************实现函数********************************************
*函数原型:		void MPU6050_IIC_Send_Byte(u8 txd)
*功　　能:	    IIC发送一个字节
*******************************************************************************/		  
void MPU6050_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MPU6050_SDA_OUT(); 	    
    MPU6050_IIC_SCL=0;//拉低时钟开始数据传输
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
  
/**************************实现函数********************************************
*函数原型:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
*功　　能:		
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
/**************************实现函数********************************************
*函数原型:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
*功　　能:		
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


/**************************实现函数********************************************
*函数原型:		u8 MPU6050_IIC_Read_Byte(unsigned char ack)
*功　　能:	    //读1个字节，ack=1时，发送ACK，ack=0，发送nACK 
*******************************************************************************/  
u8 MPU6050_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU6050_SDA_IN();//MPU6050_SDA设置为输入
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
        MPU6050_IIC_Ack(); //发送ACK 
    else
        MPU6050_IIC_NAck();//发送nACK  
    return receive;
}

/**************************实现函数********************************************
*函数原型:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	I2C_Addr  目标设备地址
		addr	   寄存器地址
返回   读出来的值
*******************************************************************************/ 
unsigned char MPU6050_I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	MPU6050_IIC_Start();	
	MPU6050_IIC_Send_Byte(I2C_Addr);	   //发送写命令
	res++;
	MPU6050_IIC_Wait_Ack();
	MPU6050_IIC_Send_Byte(addr); res++;  //发送地址
	MPU6050_IIC_Wait_Ack();	  
	//MPU6050_IIC_Stop();//产生一个停止条件	
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(I2C_Addr+1); res++;          //进入接收模式			   
	MPU6050_IIC_Wait_Ack();
	res=MPU6050_IIC_Read_Byte(0);	   
    MPU6050_IIC_Stop();//产生一个停止条件

	return res;
}


/**************************实现函数********************************************
*函数原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要读的字节数
		*data  读出的数据将要存放的指针
返回   读出来的字节数量
*******************************************************************************/ 
u8 MPU6050_IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(dev);	   //发送写命令
	MPU6050_IIC_Wait_Ack();
	MPU6050_IIC_Send_Byte(reg);   //发送地址
    MPU6050_IIC_Wait_Ack();	  
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(dev+1);  //进入接收模式	
	MPU6050_IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=MPU6050_IIC_Read_Byte(1);  //带ACK的读数据
		 	else  data[count]=MPU6050_IIC_Read_Byte(0);	 //最后一个字节NACK
	}
    MPU6050_IIC_Stop();//产生一个停止条件
    return count;
}

/**************************实现函数********************************************
*函数原型:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	    将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要写的字节数
		*data  将要写的数据的首地址
返回   返回是否成功
*******************************************************************************/ 
u8 MPU6050_IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(dev);	   //发送写命令
	MPU6050_IIC_Wait_Ack();
	MPU6050_IIC_Send_Byte(reg);   //发送地址
    MPU6050_IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		MPU6050_IIC_Send_Byte(data[count]); 
		MPU6050_IIC_Wait_Ack(); 
	 }
	MPU6050_IIC_Stop();//产生一个停止条件

    return 1; //status == 0;
}

/**************************实现函数********************************************
*函数原型:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	dev  目标设备地址
		reg	   寄存器地址
		*data  读出的数据将要存放的地址
返回   1
*******************************************************************************/ 
u8 MPU6050_IICreadByte(u8 dev, u8 reg, u8 *data){
	*data=MPU6050_I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************实现函数********************************************
*函数原型:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*功　　能:	    写入指定设备 指定寄存器一个字节
输入	dev  目标设备地址
		reg	   寄存器地址
		data  将要写入的字节
返回   1
*******************************************************************************/ 
unsigned char MPU6050_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return MPU6050_IICwriteBytes(dev, reg, 1, &data);
}

/**************************实现函数********************************************
*函数原型:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入	dev  目标设备地址
		reg	   寄存器地址
		bitStart  目标字节的起始位
		length   位长度
		data    存放改变目标字节位的值
返回   成功 为1 
 		失败为0
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

/**************************实现函数********************************************
*函数原型:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
输入	dev  目标设备地址
		reg	   寄存器地址
		bitNum  要修改目标字节的bitNum位
		data  为0 时，目标位将被清0 否则将被置位
返回   成功 为1 
 		失败为0
*******************************************************************************/ 
u8 MPU6050_IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    MPU6050_IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return MPU6050_IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------




