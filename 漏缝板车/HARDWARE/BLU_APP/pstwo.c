#include "pstwo.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：PS2驱动程序
Author：pinggai    Version:1.1     Data:2015/10/20
Description: PS2驱动程序
             增加功能：
			 1、软件设置“红灯模式”、“绿灯模式”，并可以设置“保存”，通过手柄“模式按键”无法改变
			 2、设置手柄震动：通过数值的设置，改变左侧大震动电机震动频率。
			                  通过数值的设置，开关右侧小震动电机。
History:  
V1.0: 	2015/05/16
1、手柄解码，识别按键值，读取模拟值。       
**********************************************************/	 
#define DELAY_TIME  delay_us(3); 
unsigned short Handkey;	// 按键值读取，零时存储。
unsigned char Comd[2]={0x01,0x42};	//开始命令。请求数据
unsigned char Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
unsigned short MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明

//手柄接口初始化    输入  DI->PD11 
//                  输出  DO->PD12    CS->PD13  CLK->PD14
void PS2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);//
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOD, &GPIO_InitStructure);//
}

//向手柄发送命令
void PS2_Cmd(unsigned char CMD)
{
	volatile unsigned short ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //输出一位控制位
		}
		else DO_L;

		CLK_H;                        //时钟拉高
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}
//判断是否为红灯模式,0x41=模拟绿灯，0x73=模拟红灯
//返回值；0，红灯模式
//		  其他，其他模式
unsigned char PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile unsigned char byte=0;
	volatile unsigned short ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME;
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;
}

//对读出来的PS2的数据进行处理,只处理按键部分  
//只有一个按键按下时按下为0， 未按下为1
//unsigned char PS2_DataKey()
//{
//	unsigned char index;

//	PS2_ClearData();
//	PS2_ReadData();

//	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
//	
//	for(index=0;index<16;index++)
//	{	    
//		if((Handkey&(1<<(MASK[index]-1)))==0)
//		return index+1;
//	}
//	
//	return 0;          //没有任何按键按下
//}


unsigned short PS2_DataKey(void)
{
	//unsigned char index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1	
	
//	for(index=0;index<16;index++)
//	{	    
//		if((Handkey&(1<<(MASK[index]-1)))==0)
//		return index+1;
//	}
//	
	return ~Handkey;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
unsigned char PS2_AnologData(unsigned char button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	unsigned char a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function:    void PS2_Vibration(unsigned char motor1, unsigned char motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(unsigned char CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(unsigned char motor1, unsigned char motor2)
{
	CS_L;
	delay_us(16);
  PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
//进入配置
void PS2_EnterConfing(void)
{
  CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//发送模式设置
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	PS2_Cmd(0xEE); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				   //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//振动设置
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
//完成并保存配置
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
//手柄配置初始化
void PS2_SetInit(void)
{
	PS2_Init();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		//完成并保存配置
}


void Joystick_test(void){
#ifdef PSTWO_F
unsigned char key;  
	key=PS2_DataKey();
	if(key!=0)                   //有按键按下
	{
		switch (key)
		{
			case PSB_SELECT:
		    u1_printf("SELECT\r\n");
				break; 
			case PSB_L3:
			 u1_printf("L3\r\n");
				break;
			case PSB_R3:
			u1_printf("R3\r\n");
				break;
			case PSB_START:
			u1_printf("START\r\n");

				break;
			case PSB_PAD_UP:
			u1_printf("UP\r\n");

				break;
			case PSB_PAD_RIGHT:
			u1_printf("RIGHT\r\n");

				break;
			case PSB_PAD_DOWN:
			u1_printf("DOWN\r\n");

				break;
			case PSB_PAD_LEFT:
			u1_printf("LEFT\r\n");

				break;
			case PSB_L2:
			u1_printf("L2\r\n");
				break;
			case PSB_R2:
			u1_printf("R2\r\n");
				break;
			case PSB_L1:
		  u1_printf("L1\r\n");
				break;
			case PSB_R1:
		  u1_printf("R1\r\n");
				break;
			case PSB_TRIANGLE:
		  u1_printf("TRIANGLE\r\n");
				break;
			case PSB_CIRCLE:
		  u1_printf("CIRCLE\r\n");
				break;
			case PSB_CROSS:
		  u1_printf("CROSS\r\n");
				break;
			case PSB_SQUARE:
		  u1_printf("SQUARE\r\n");
				break;
			default:
		  u1_printf("UNKNOWN\r\n");
				break;
		}
	}
	u1_printf("(need to switch mode) %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY), PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY) );
#endif
}

bool Auto_car_mode=0;
bool  car_state_add = 1;
bool  car_state_minus = 1;
void Joystick(void){
#ifdef PSTWO_F
  unsigned short key; 
	key=PS2_DataKey();
	
	 if(key!=0XFFF)                   //有按键按下
	{	
		
		 if(key&PSB_GREEN_BIT){
			 Auto_car_mode =1;
			 POWER_LOW;
			 
		 }
		 else
		 {
			  POWER_HIGH;
				Auto_car_mode =0;
		 }

		
	   if(key&PSB_RED_BIT){
			 	Car_Stop();
		 }
		 
    if(Auto_car_mode==0)		
		{
					if(key&PSB_L1_BIT) //按住开启电锁
					{
						 Car_Start();
						 u1_printf("PSB_L1_BIT\r\n");		 
					}
					else
					{
							Car_Stop();//关闭电锁		  
					}

					if((PS2_AnologData(PSS_LX)>80 && PS2_AnologData(PSS_LX)<180) && PS2_AnologData(PSS_LY)<80)//向上 
					{
							Car_Forward();
					}
					else if((PS2_AnologData(PSS_LX)>80 && PS2_AnologData(PSS_LX)<180) && PS2_AnologData(PSS_LY)>180)//向下
					{
							Car_Backward();
					}
					else
					{	   
							Car_Break(); 
					}
						 
				 if(PS2_AnologData(PSS_RX)<80 && (PS2_AnologData(PSS_RY)>80 && PS2_AnologData(PSS_RY)<180))//向左
				 {
							 Car_Left();
				 }
				 else if (PS2_AnologData(PSS_RX)>180 && (PS2_AnologData(PSS_RY)>80 && PS2_AnologData(PSS_RY)<180))//向右
				 {
							 Car_Right();
				 }
		}		
		
		 if(key&PSB_START_BIT){
			 
			 u1_printf("PSB_START_BIT\r\n");
		 }
		 if(key&PSB_PAD_UP_BIT){
			 
			 
			 u1_printf("PSB_PAD_UP_BIT\r\n");
		 }
		 if(key&PSB_PAD_LEFT_BIT){
			 
			 u1_printf("PSB_PAD_LEFT_BIT\r\n");
		 }
		 if(key&PSB_PAD_RIGHT_BIT){
			 u1_printf("PSB_PAD_RIGHT_BIT\r\n");
		 }
		 if(key&PSB_PAD_DOWN_BIT){
			 u1_printf("PSB_PAD_DOWN_BIT\r\n");
		 }
		 	 
		 if(key&PSB_L2_BIT){
			 u1_printf("PSB_L2_BIT\r\n");
			 
		 }
		 if(key&PSB_L3_BIT){
			 u1_printf("PSB_L3_BIT\r\n");
		 }
		 
		 if(key&PSB_R1_BIT){
			   u1_printf("PSB_R1_BIT\r\n");
			   if(car_state_add ==1)
				 {
					  car_state_add = 0; 
						if(speed<3300)speed=speed+50;
						Dac1_Set_Vol(speed);
				 }
			   delay_ms(50);
		 }
		 else 
		 {
				car_state_add = 1;
		 }
		 
		 if(key&PSB_R2_BIT){
				  if(car_state_minus ==1)
				 {
					  car_state_minus = 0; 
						if(speed>1500)speed=speed-50; 
						Dac1_Set_Vol(speed);
				 }
				 delay_ms(50);
		 }
		 else
		 {
				car_state_minus = 1;
		 }
		 if(key&PSB_R3_BIT){
			  
				u1_printf("PSB_R3_BIT\r\n");
		 }
		 if(key&PSB_GREEN_BIT){
			 	u1_printf("PSB_GREEN_BIT\r\n");
		 }
		 if(key&PSB_RED_BIT){
			 	u1_printf("PSB_RED_BIT\r\n");
		 }
		 if(key&PSB_BLUE_BIT){
			 Car_Stop();
			 u1_printf("PSB_BLUE_BIT\r\n");
		 }
		 if(key&PSB_PINK_BIT){ 
			 u1_printf("PSB_PINK_BIT\r\n");
		 }	  
	}
	else
	{
			  Car_Break();
	}
		
	
		if(Auto_car_mode==1)
		 {
				SmartCar_control();
		 }	
 
#endif
}








