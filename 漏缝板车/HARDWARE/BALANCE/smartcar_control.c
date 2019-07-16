#include "smartcar_control.h"
int motor_left=0;                 
int motor_right=0;
int Left_encoder=0;
int Right_encoder=0;

#define PI 3.14159265

float Angle_Balance;		// 平衡倾角  （Pitch角）
float Gyro_Balance;			// 平衡陀螺仪（平衡角速度）
float Gyro_Turn;			// 转向陀螺仪（转向角速度）
float Acceleration_Z;		// Z轴加速度计 

/*********************************************************************************************************
** Function name:       Set_Pwm
** Descriptions:        赋值给PWM寄存器
** input parameters:    motor1：左轮PWM
												motor2：右轮PWM
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Set_Pwm(int motor1,int motor2)
{	
	if(motor1>0)			
	{
		//如果左轮电机PWM值小于0，则左轮正向转动
		  PWM_LEFT_SPEED = myabs(motor1);
		  MOTOR_LEFT_DR_FOWARD;
	}
	else{
		//如果左轮电机PWM值大于等于0，则左轮反向转动
			PWM_LEFT_SPEED = myabs(motor1);
			MOTOR_LEFT_DR_BACKWARD;
	}

	if(motor2>0)	
	{
		//如果右轮电机PWM值大于等于0，则右轮反向转动
			PWM_RIGHT_SPEED = myabs(motor2);
		  MOTOR_RIGHT_DR_FOWARD;
	}
	else{
		//如果右轮电机PWM值小于0，则右轮正向转动
			PWM_RIGHT_SPEED = myabs(motor2);
			MOTOR_RIGHT_DR_BACKWARD;//
	}
	
}

/*********************************************************************************************************
** Function name:       Limit_Pwm
** Descriptions:        限制PWM赋值 
** input parameters:    *motor1：左轮PWM
						*motor2：右轮PWM
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Limit_Pwm(int *motor1,int *motor2)
{	
	int Amplitude=3500;    //===PWM满幅是9000
	if(*motor1 <-Amplitude)	*motor1 =-Amplitude;	
	if(*motor1 > Amplitude)	*motor1 = Amplitude;	
	if(*motor2 <-Amplitude)	*motor2 =-Amplitude;	
	if(*motor2 > Amplitude)	*motor2 = Amplitude;		
	
}


/*********************************************************************************************************
** Function name:       Get_Angle
** Descriptions:        获取角度，更新姿态。
** input parameters:    way：获取角度的算法	[1：DMP]
											[2：卡尔曼]
											[3：互补滤波]
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Get_Angle(u8 way)
{ 
	float Accel_Y;		//MPU的 Y 轴加速度
	float Accel_X;		//MPU的 X 轴加速度
	float Accel_Z;		//MPU的 Z 轴加速度
	float Gyro_X;		//MPU的 Y 轴角速度
	float Gyro_Y;		//MPU的 Y 轴角速度
	float Gyro_Z;		//MPU的 Z 轴角速度
	
	if(way==1)                           //===DMP的读取在数据采集中断提醒的时候，严格遵循时序要求
	{	
		Read_DMP();                      //===读取加速度、角速度、倾角
	}			
	else
	{
		Gyro_X	= (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H) <<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L));
		Gyro_Y  = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H) <<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L));		//读取 Y 轴陀螺仪
		Gyro_Z  = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H) <<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L));		//读取 Z 轴陀螺仪
		Accel_X = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L));	//读取 X 轴加速度计
		Accel_Y	= (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L));	//读取 Y 轴加速度计
		Accel_Z = (short)((MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8) + MPU6050_I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L));	//读取 Z 轴加速度计
		
		Gyro_Balance = -Gyro_Y;									// 更新平衡角速度	
		
		if(way == 2)	Kalman_Filter((atan2(Accel_X,Accel_Z)*180/PI),(-Gyro_Y/16.4));			// 卡尔曼滤波	更新变量angle（角度）与angle_dot(角速度)
		if(way == 3)	Yijielvbo(Accel_Y,-Gyro_Y);    			// 互补滤波		更新变量angle（角度）
		Angle_Balance =angle;									// 更新平衡倾角
		Gyro_Turn = Gyro_Z;										// 更新转向角速度
		Acceleration_Z = Accel_Z;								// 更新Z轴加速度计	
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
