#include "stm32f10x.h"
#include "sys.h" 
/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
#define START_STK_SIZE  				64//设置开始任务堆栈大小
OS_STK START_TASK_STK[START_STK_SIZE];  //开始任务堆栈	
void start_task(void *pdata);			//开始任务函数	
 			   
//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			5
//设置任务堆栈大小
#define MAIN_STK_SIZE  					128
//任务堆栈	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];



//任务函数
void main_task(void *pdata);
void System_Device_Init(void);


/*********主函数入口************/
 int main(void)
{	
//Program_Protection();
  System_Device_Init();//设备初始化
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//创建起始任务
	OSStart();	  	 
}
	  
//开始任务
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;	
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);
	
//  Key_task_Create();//创建按键任务
//	Ultra_task_Create();//创建超声波任务
//	Power_task_Create();//创建电源管理任务
//	Hand_action_task_Create();//创建手势任务
//	Led_task_Create();//创建LED任务
	  
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}

contorl_flag_type contorl_flag;
/*********************************************************************************************************
** Function name:       void main_task(void *pdata)
** Descriptions:        主任务运行
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void main_task(void *pdata)
{	
			
	

	SmartCar_control_init();
	smar_time_init();	
	uint32_t curtick  =	OSTimeGet();
	uint32_t sendtick = OSTimeGet();
	while(1)
	{		
		  if((OSTimeGet() - curtick) > 500)	
			{
				Joystick();
				
			}
			OSTimeDlyHMSM(0,0,0,20); 
	}

}

/*********************************************************************************************************
** Function name:       void System_Device_Init(void)
** Descriptions:        硬件设备初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void System_Device_Init(void)
{
	SystemInit();	    // 系统时钟初始化
	delay_init();	    //延时函数初始化	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级	
//delay_ms(500);//MPU6050需要启动时间大于250ms安全启动时间	
	Motor_PWM_Init(9000,1);	//=====初始化PWM 4KHZ，用于驱动电机 如需初始化电调接口 改为MiniBalance_PWM_Init(9999,35) 200HZ
	PS2_SetInit();
	usart3_init(9600);
  laster_forward_control_set();
	usart1_init(9600);
	laster_backward_control_set();
	Set_Pwm(0,0);

}


