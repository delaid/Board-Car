#include "filter.h"

float angle;							// 角度
float angle_dot;						// 角度的微分（角速度）
static float K1 =0.02; 	
static float Q_angle=0.001;				// 过程噪声的协方差
static float Q_gyro=0.003;				// 0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
static float R_angle=0.5;				// 测量噪声的协方差 既测量偏差
static float dt=0.005;
static char  C_0 = 1;
static float Q_bias, Angle_err;
static float PCt_0, PCt_1, E;
static float K_0, K_1, t_0, t_1;
static float Pdot[4] ={0,0,0,0};
static float PP[2][2] = { { 1, 0 },{ 0, 1 } };

Kalman_Param   KP; //KP=KalmanParam
Kalman_Gesture KG; //KG=KalmanGesture

/************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
*************************************************************************/

void Kalman_Filter(float Accel,float Gyro)		
{
	angle+=(Gyro - Q_bias) * dt;				// 先验估计
	
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0];		// Pk-先验估计误差协方差的微分
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;					// Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;					// =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - angle;					// zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;						// 后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= 	K_0 * Angle_err;					// 后验估计
	Q_bias	+= 	K_1 * Angle_err;					// 后验估计
	angle_dot   = Gyro - Q_bias;				// 输出值(后验估计)的微分=角速度
	KG.KalPitch	= angle;
}

/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Yijielvbo(float angle_m, float gyro_m)
{
   angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.005f);
}

/*
********************************************************************************
** 函数名称	： KalmanParam_Init()
** 函数功能	： 预测MPU倾角
** 输    入	： Pitch：倾角测量值
               Gyro： 倾角角速度
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void KalmanParam_Init(void)
{
	KP.Count   = 0;
	KP.Q_angle = 0.001;
	KP.Q_gyro  = 0.003;
	KP.R_angle = 0.5;
	KP.dt      = 0.005;
	KP.C_0     = 1;
	KP.Q_bias  = 0;
	KP.Angle_err= 0;
	KP.PCt_0   = 0;
	KP.PCt_1   = 0;
	KP.E       = 0;
	KP.K_0     = 0;
	KP.K_1     = 0;
	KP.t_0     = 0;
	KP.t_1     = 0;
	KP.Pdot[0] = 0;
	KP.Pdot[1] = 0;
	KP.Pdot[2] = 0;
	KP.Pdot[3] = 0;
	KP.PP[0][0] = 1;
	KP.PP[0][1] = 0;
	KP.PP[1][0] = 0;
	KP.PP[1][1] = 1;
}

/*
********************************************************************************
** 函数名称	： Kalman_Pitch_Filter()
** 函数功能	： 预测MPU倾角
** 输    入	： Pitch：倾角测量值
               Gyro： 倾角角速度
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void Kalman_Roll_Filter(float Roll,float Gyro)		
{

		KG.KalRoll += (Gyro - KP.Q_bias) * KP.dt;					// 先验估计
		KP.Pdot[0]  = KP.Q_angle - KP.PP[0][1] - KP.PP[1][0];		// Pk-先验估计误差协方差的微分

		KP.Pdot[1] = -KP.PP[1][1];
		KP.Pdot[2] = -KP.PP[1][1];
		KP.Pdot[3] =  KP.Q_gyro;
		KP.PP[0][0] += KP.Pdot[0] * KP.dt;							// Pk-先验估计误差协方差微分的积分
		KP.PP[0][1] += KP.Pdot[1] * KP.dt;							// =先验估计误差协方差
		KP.PP[1][0] += KP.Pdot[2] * KP.dt;
		KP.PP[1][1] += KP.Pdot[3] * KP.dt;
			
		KP.Angle_err = Roll - KG.KalRoll;							// zk-先验估计
		
		KP.PCt_0 = KP.C_0 * KP.PP[0][0];
		KP.PCt_1 = KP.C_0 * KP.PP[1][0];
		
		KP.E = KP.R_angle + KP.C_0 * KP.PCt_0;
		
		KP.K_0 = KP.PCt_0 / KP.E;
		KP.K_1 = KP.PCt_1 / KP.E;
		
		KP.t_0 = KP.PCt_0;
		KP.t_1 = KP.C_0 * KP.PP[0][1];

		KP.PP[0][0] -= KP.K_0 * KP.t_0;								// 后验估计误差协方差
		KP.PP[0][1] -= KP.K_0 * KP.t_1;
		KP.PP[1][0] -= KP.K_1 * KP.t_0;
		KP.PP[1][1] -= KP.K_1 * KP.t_1;
			
		KG.KalRoll	+= KP.K_0 * KP.Angle_err;							// 后验估计
		KP.Q_bias	+= KP.K_1 * KP.Angle_err;							// 后验估计
		KG.KPPal= Gyro - KP.Q_bias;									// 输出值(后验估计)的微分=角速度

		KG.KalRoll += (Gyro - KP.Q_bias) * KP.dt;
		KP.Angle_err = Roll - KG.KalRoll;
		
		KG.KalRoll	+= KP.K_0 * KP.Angle_err;
		KP.Q_bias	+= KP.K_1 * KP.Angle_err;
		KG.KPPal= Gyro - KP.Q_bias;
	
}
















