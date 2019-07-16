#include "Smar_time_task.h"

int  Left_encoder_Target=0;
int  Right_encoder_Target=0;

OS_TMR   * tmr_smar;			//软件定时器

void tmr_smar_callback(OS_TMR *ptmr,void *p_arg) //超声波读数
{
	static u16 IMU_Sample_count=0;
	static u16 Car_key_count=0;
  IMU_Sample_count++;
	
	if(IMU_Sample_count==1)
	{
		if(Auto_mode==1)
		SmartCar_control();
		IMU_Sample_count=0;
	}
	
	Car_key_count++;
	if(Car_key_count==100)
	{
		 Car_key_count = 0;
		
		 if(Auto_mode==0)
		 {
			 if(key==0)
			 {				
					Set_Pwm(0,0);
			 }
	   }	 
	}

	
}

//软件定时器1的回调函数	
//每5ms执行一次		
void smar_time_init(void)
{
	u8 err;
	tmr_smar=OSTmrCreate(4,1,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr_smar_callback,0,"tmr_smar",&err);
	OSTmrStart(tmr_smar,&err);  //10ms执行一次 
}


 
