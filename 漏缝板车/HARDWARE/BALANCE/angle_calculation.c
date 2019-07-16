#include "angle_calculation.h"
/***********************************��������Ӳ���****************************************
�ż���26���ż�
�����ȣ�1:48
���ӵİ뾶��33mm
�����ܳ���2*PI*33=207.34mm
����������ı�Ƶ������26/2*48*4=2496
*****************************************************************************************/
//#define Motor_Pulse			2496			// ����תһȦ���ɵ�������
//#define Wheel_Radius		33				// ����ֱ��ԼΪ d=65mm �뾶ԼΪ32.5mm
//#define Axial_Length		90.00f			// �����᳤Ϊ93mm

#define Motor_Pulse			2530.0f			// ����תһȦ���ɵ�������
#define Wheel_Radius		(32.5f)			// ����ֱ��ԼΪ d=65mm �뾶ԼΪ32.5mm
#define Axial_Length		87.00f			// �����᳤Ϊ93mm

float Calculate_Angle = 0;					// ����Ƕ�
/*********************************************************************************************************
** Function name:       Angle_Calculation
** Descriptions:        ���õ��ת�������С��ת���ĽǶȣ�ע�����ֵ���ԣ�����ͬ����ͬΪ����ͬΪ����,
                        ����תΪ��������תΪ����
** input parameters:    encoder_left�� ���ֱ���ֵ
						encoder_right�����ֱ���ֵ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Angle_Calculation(int encoder_left,int encoder_right)
{
	/*
	  �����Ƶ���ʽ�� �Ƕ� = ((����ת�� - ����ת��) * ���Ӱ뾶/С���᳤) * 360
	*/
	Calculate_Angle += (((( encoder_right - encoder_left )/Motor_Pulse)*Wheel_Radius)/Axial_Length)*360;
}


float Mileage_MM;//��̼���ֵ����λ��mm��
/*********************************************************************************************************
** Function name:       Mileage_Meter
** Descriptions:        ��ʽ��̼�
** input parameters:    encoder_left�� ���ֱ���ֵ
						encoder_right�����ֱ���ֵ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Mileage_Meter(int encoder_left,int encoder_right)
{
	/*
	  ��ʽ��̼Ƽ��㹫ʽ���ƶ����� = �����־���+���־��룩/2 
	                             = ������ת���Ƕ�+����ת���Ƕȣ�/��360*2�� * �ܳ�
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



