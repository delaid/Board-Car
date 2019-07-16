#include "smartcar_control.h"
int motor_left=0;                 
int motor_right=0;
int Left_encoder=0;
int Right_encoder=0;

#define PI 3.14159265

float Angle_Balance;		// ƽ�����  ��Pitch�ǣ�
float Gyro_Balance;			// ƽ�������ǣ�ƽ����ٶȣ�
float Gyro_Turn;			// ת�������ǣ�ת����ٶȣ�
float Acceleration_Z;		// Z����ٶȼ� 

/*********************************************************************************************************
** Function name:       Set_Pwm
** Descriptions:        ��ֵ��PWM�Ĵ���
** input parameters:    motor1������PWM
												motor2������PWM
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Set_Pwm(int motor1,int motor2)
{	
	if(motor1>0)			
	{
		//������ֵ��PWMֵС��0������������ת��
		  PWM_LEFT_SPEED = myabs(motor1);
		  MOTOR_LEFT_DR_FOWARD;
	}
	else{
		//������ֵ��PWMֵ���ڵ���0�������ַ���ת��
			PWM_LEFT_SPEED = myabs(motor1);
			MOTOR_LEFT_DR_BACKWARD;
	}

	if(motor2>0)	
	{
		//������ֵ��PWMֵ���ڵ���0�������ַ���ת��
			PWM_RIGHT_SPEED = myabs(motor2);
		  MOTOR_RIGHT_DR_FOWARD;
	}
	else{
		//������ֵ��PWMֵС��0������������ת��
			PWM_RIGHT_SPEED = myabs(motor2);
			MOTOR_RIGHT_DR_BACKWARD;//
	}
	
}

/*********************************************************************************************************
** Function name:       Limit_Pwm
** Descriptions:        ����PWM��ֵ 
** input parameters:    *motor1������PWM
						*motor2������PWM
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Limit_Pwm(int *motor1,int *motor2)
{	
	int Amplitude=3500;    //===PWM������9000
	if(*motor1 <-Amplitude)	*motor1 =-Amplitude;	
	if(*motor1 > Amplitude)	*motor1 = Amplitude;	
	if(*motor2 <-Amplitude)	*motor2 =-Amplitude;	
	if(*motor2 > Amplitude)	*motor2 = Amplitude;		
	
}


/*********************************************************************************************************
** Function name:       Get_Angle
** Descriptions:        ��ȡ�Ƕȣ�������̬��
** input parameters:    way����ȡ�Ƕȵ��㷨	[1��DMP]
											[2��������]
											[3�������˲�]
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Get_Angle(u8 way)
{ 
	float Accel_Y;		//MPU�� Y ����ٶ�
	float Accel_X;		//MPU�� X ����ٶ�
	float Accel_Z;		//MPU�� Z ����ٶ�
	float Gyro_X;		//MPU�� Y ����ٶ�
	float Gyro_Y;		//MPU�� Y ����ٶ�
	float Gyro_Z;		//MPU�� Z ����ٶ�
	
	if(way==1)                           //===DMP�Ķ�ȡ�����ݲɼ��ж����ѵ�ʱ���ϸ���ѭʱ��Ҫ��
	{	
		Read_DMP();                      //===��ȡ���ٶȡ����ٶȡ����
	}			
	else
	{
		Gyro_X	= (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H) <<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L));
		Gyro_Y  = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H) <<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L));		//��ȡ Y ��������
		Gyro_Z  = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H) <<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L));		//��ȡ Z ��������
		Accel_X = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L));	//��ȡ X ����ٶȼ�
		Accel_Y	= (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L));	//��ȡ Y ����ٶȼ�
		Accel_Z = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L));	//��ȡ Z ����ٶȼ�
		
		Gyro_Balance = -Gyro_Y;									// ����ƽ����ٶ�	
		
		if(way == 2)	Kalman_Filter((atan2(Accel_X,Accel_Z)*180/PI),(-Gyro_Y/16.4));			// �������˲�	���±���angle���Ƕȣ���angle_dot(���ٶ�)
		if(way == 3)	Yijielvbo(Accel_Y,-Gyro_Y);    			// �����˲�		���±���angle���Ƕȣ�
		Angle_Balance =angle;									// ����ƽ�����
		Gyro_Turn = Gyro_Z;										// ����ת����ٶ�
		Acceleration_Z = Accel_Z;								// ����Z����ٶȼ�	
		Kalman_Roll_Filter((atan2(Accel_Y,Accel_Z)*180/PI),(Gyro_X/16.4f));
	}
}

int laster_biass = 0;
int last_laster_biass = 0; 
float laster_pwm_buff=0;
void SmartCar_control(void)
{   
	 
	   static float Kp = 3.2;
	   static float  Ki = 0.001;

	   laster_biass = laster_forword-laster_backword;
	   
	   laster_pwm_buff += Kp*(laster_biass-last_laster_biass)+ Ki*laster_biass;
	   
			motor_left  =  1200 - (int)laster_pwm_buff ;
	    motor_right =  1200 + (int)laster_pwm_buff;
	
	    last_laster_biass = laster_biass;
		 
		
  	 Limit_Pwm(&motor_left,&motor_right);
	   Set_Pwm(motor_left,motor_right);
}


void SmartCar_control_init(void)
{
		MOTOR_LEFT_BK_DISEN;
		MOTOR_LEFT_EN_EN;
		MOTOR_RIGHT_BK_DISEN;
		MOTOR_RIGHT_EN_EN;

}
