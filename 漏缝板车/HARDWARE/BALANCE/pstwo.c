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
unsigned short Handkey_mode;
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
	
  Handkey_mode=Data[1];
	
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

bool Auto_mode = 0; 
unsigned char	Auto_flag=0;
uint8_t Up_Down_State=0;
unsigned short key;
unsigned short flog=1,flog2=1; 

void Joystick(void){
unsigned short i,j;
	//static uint16_t Left_speed=0,Right_speed=0;
	    key=PS2_DataKey();
	
	    if(Handkey_mode == 0x73)
	    {     
				Auto_mode	= 0;
				if( PS2_AnologData(PSS_LY)<80 && PS2_AnologData(PSS_RX)<80)//上左
				{					
					  Set_Pwm(2500,1500);
				}
				else if( PS2_AnologData(PSS_LY)<80 && PS2_AnologData(PSS_RX)>180)//右
				{
					  Set_Pwm(1500,2500);
				}
				else if(PS2_AnologData(PSS_LY)>180 && PS2_AnologData(PSS_RX)<80)
				{
					  Set_Pwm(-2500,-1500);
				}
				else if(PS2_AnologData(PSS_LY)>180 && PS2_AnologData(PSS_RX)>180 )
				{
						Set_Pwm(-1500,-2500);
				}
				else if((PS2_AnologData(PSS_LX)>80 && PS2_AnologData(PSS_LX)<180) && PS2_AnologData(PSS_LY)<80)//向上 
				{
						Set_Pwm(2500,2500);//Car_Forward();
				}
				else if((PS2_AnologData(PSS_LX)>80 && PS2_AnologData(PSS_LX)<180) && PS2_AnologData(PSS_LY)>180)//向下
				{
					  Set_Pwm(-2500,-2500);//Car_Backward();
				}

				else if(PS2_AnologData(PSS_RX)<80 && (PS2_AnologData(PSS_RY)>80 && PS2_AnologData(PSS_RY)<180))//向左
				{
					   Set_Pwm(2500,-2500);
				}
				else if (PS2_AnologData(PSS_RX)>180 && (PS2_AnologData(PSS_RY)>80 && PS2_AnologData(PSS_RY)<180))//向右
				{
					   Set_Pwm(-2500,2500);
				}
				else
				{			
					    Set_Pwm(0,0);
				}
				
			}	
			else if(Handkey_mode == 0x41)
	    {   
				 if(key&PSB_GREEN_BIT)
				 Auto_mode	= 1;	 	
			}
		  else
		  {
				 Auto_mode	= 0;
				 Set_Pwm(0,0);
		  }
			
			
		if(flog==1)
		{
			
			
				if(Limiter3 ==0 && Limiter1 ==0 )
				{	
					if(key&PSB_L2_BIT)
					{
							if(Up_Down_State!=1)
							{
									MOTOR_PULL_EN;
									MOTOR_PUSH_EN;
									Up_Down_State = 1;
									delay_ms(200);
							}
									MOTOR_PUSH_EN;
									MOTOR_PULL_DISEN;
								

						
				  }
				else if(key&PSB_R2_BIT)
				{
						if(Up_Down_State!=2)
						{
								MOTOR_PULL_EN;
								MOTOR_PUSH_EN;
								Up_Down_State = 2;
								delay_ms(200);
						}
						MOTOR_PUSH_DISEN;
						MOTOR_PULL_EN;
					 
				}
				else
				{
						MOTOR_PULL_EN;
						MOTOR_PUSH_EN;
						Up_Down_State = 0;
				}
	   }
		  else if( Limiter3 ==1 )
		  {
				if(key&PSB_L2_BIT)
				{
						if(Up_Down_State!=1)
						{
								MOTOR_PULL_EN;
								MOTOR_PUSH_EN;
								Up_Down_State = 1;
								delay_ms(200);
						}
						MOTOR_PUSH_EN;
						MOTOR_PULL_DISEN;

				}
			}
		 
		  else if(Limiter1 ==1 )	
			{
				if(key&PSB_R2_BIT)
				{
						if(Up_Down_State!=2)
						{
								MOTOR_PULL_EN;
								MOTOR_PUSH_EN;
								Up_Down_State = 2;
								delay_ms(200);
						}
						
						MOTOR_PUSH_DISEN;
						MOTOR_PULL_EN;

					 
				}
			}

				if(Limiter2 == 1 && flog2==1)
				{

					flog=0;
					MOTOR_PULL_EN;
					MOTOR_PUSH_EN;
					flog2 = 0; 


				}
			if(Limiter2 == 0 )
				  flog2= 1;
		}
	
				if(key&PSB_RED_BIT)
				{

					 
					flog=1;

				}
		
		
//#if 0						
//				if(Limiter3 ==0 && Limiter1 ==0 )
//				{
//					 if(key&PSB_L2_BIT)
//					 {
//						  delay_ms(50);
//							key1=1;
//							delay_ms(50);
//					 }
//					 else if(key&PSB_R2_BIT)
// 					 { 
//							delay_ms(50);
//						  key1=2;
//							delay_ms(50);
//					 }
//					
//				}
//				else if(Limiter1 ==0 && Limiter3 ==1)
//				{
//					 if(key&PSB_L2_BIT)
//					 {
//							key1=1;
//							delay_ms(50);
//					 }
//				}				
//				else if(Limiter1 ==1 && Limiter3 ==0)	
//				{
//					 if(key&PSB_R2_BIT)
//					 { 
//							key1=2;
//							delay_ms(50);
//					 }
//				}
//				else if(Limiter2 == 1)
//				{
//					  MOTOR_PULL_DISEN;
//					  MOTOR_PUSH_DISEN;
//						for(i=0;i<5;i++)
//						{					
//							delay_ms(500);
//						}
//				}
//        else
//			  {
//							key1=0;
//			  }				 
//		
//			
//		  }

//		
//			
//			
//			if(key1)
//			{						   
//				switch(key1)
//				{				 
//					case 1:
//							MOTOR_PUSH_EN;
//							MOTOR_PULL_DISEN;
//					    delay_ms(30);
//						 
//						break;
//					case 2:
//							MOTOR_PUSH_DISEN;
//							MOTOR_PULL_EN;
//							delay_ms(30);
//						break;
//					
//				}
//			}
//			else 
//			{
//					MOTOR_PULL_EN;
//					MOTOR_PUSH_EN;
//					delay_ms(100);
//			} 
//#endif		
//	
}




