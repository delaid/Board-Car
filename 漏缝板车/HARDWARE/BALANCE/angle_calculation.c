#include "angle_calculation.h"
/***********************************电机、轮子参数****************************************
磁极：26个磁极
传动比：1:48
轮子的半径：33mm
轮子周长：2*PI*33=207.34mm
电机编码器四倍频读数：26/2*48*4=2496
*****************************************************************************************/
//#define Motor_Pulse			2496			// 轮子转一圈生成的脉冲数
//#define Wheel_Radius		33				// 轮子直径约为 d=65mm 半径约为32.5mm
//#define Axial_Length		90.00f			// 车轴轴长为93mm

#define Motor_Pulse			2530.0f			// 轮子转一圈生成的脉冲数
#define Wheel_Radius		(32.5f)			// 轮子直径约为 d=65mm 半径约为32.5mm
#define Axial_Length		87.00f			// 车轴轴长为93mm

float Calculate_Angle = 0;					// 推算角度
/*********************************************************************************************************
** Function name:       Angle_Calculation
** Descriptions:        利用电机转速推算出小车转过的角度（注意编码值极性，车轮同向则同为正或同为负）,
                        向右转为正，向左转为负。
** input parameters:    encoder_left： 左轮编码值
						encoder_right：右轮编码值
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Angle_Calculation(int encoder_left,int encoder_right)
{
	/*
	  最终推导公式： 角度 = ((左轮转速 - 右轮转速) * 轮子半径/小车轴长) * 360
	*/
	Calculate_Angle += (((( encoder_right - encoder_left )/Motor_Pulse)*Wheel_Radius)/Axial_Length)*360;
}


float Mileage_MM;//里程计数值（单位：mm）
/*********************************************************************************************************
** Function name:       Mileage_Meter
** Descriptions:        轮式里程计
** input parameters:    encoder_left： 左轮编码值
						encoder_right：右轮编码值
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Mileage_Meter(int encoder_left,int encoder_right)
{
	/*
	  轮式里程计计算公式：移动距离 = （左轮距离+右轮距离）/2 
	                             = （左轮转动角度+右轮转动角度）/（360*2） * 周长
	*/
	//Mileage_MM += (0-(encoder_left+encoder_right))/(Motor_Pulse*2.0f)*198.86f;
	Mileage_MM += (0-(encoder_left+encoder_right))/(Motor_Pulse*2.0f)*207.34f;
}



void Call_Yaw(int encoder_left,int encoder_right)
{
	if(Control_flag.Stop == 0)
	{
		Angle_Calculation(encoder_left,encoder_right);
		//Mileage_Meter(encoder_left,encoder_right);
	}
	if(Control_flag.Stop == 1)
	{
		Calculate_Angle = 0;
		Gyro_Turn = 0;
	}
}



