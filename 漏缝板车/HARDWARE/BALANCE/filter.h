#ifndef __FILTER_H
#define __FILTER_H
#include "sys.h"

extern float angle, angle_dot; 	
void Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);

typedef struct {
	float KalPitch;
	float KPPal;		//KPPal = KalmanPitchPalstance
	float KalRoll;
	float KRPal;		//KRPal = KalmanRollPalstance
}Kalman_Gesture;

typedef struct {
	u16   Count;
	float Q_angle;		// ����������Э����
	float Q_gyro;		// 0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
	float R_angle;		// ����������Э���� �Ȳ���ƫ��
	float dt;
	char  C_0;
	float Q_bias;
	float Angle_err;
	float PCt_0;
	float PCt_1;
	float E;
	float K_0;
	float K_1;
	float t_0;
	float t_1;
	float Pdot[4];
	float PP[2][2];
}Kalman_Param;

void Kalman_Roll_Filter(float Roll,float Gyro);	
void KalmanParam_Init(void);

extern Kalman_Param   KP; //KP=KalmanParam
extern Kalman_Gesture KG; //KG=KalmanGesture
#endif
