#include "stm32f10x.h"
#include "sys.h" 
/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
#define START_STK_SIZE  				64//���ÿ�ʼ�����ջ��С
OS_STK START_TASK_STK[START_STK_SIZE];  //��ʼ�����ջ	
void start_task(void *pdata);			//��ʼ������	
 			   
//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			5
//���������ջ��С
#define MAIN_STK_SIZE  					128
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];



//������
void main_task(void *pdata);
void System_Device_Init(void);


/*********���������************/
 int main(void)
{	
//Program_Protection();
  System_Device_Init();//�豸��ʼ��
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//������ʼ����
	OSStart();	  	 
}
	  
//��ʼ����
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;	
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);
	
//  Key_task_Create();//������������
//	Ultra_task_Create();//��������������
//	Power_task_Create();//������Դ��������
//	Hand_action_task_Create();//������������
//	Led_task_Create();//����LED����
	  
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}

contorl_flag_type contorl_flag;
/*********************************************************************************************************
** Function name:       void main_task(void *pdata)
** Descriptions:        ����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        Ӳ���豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void System_Device_Init(void)
{
	SystemInit();	    // ϵͳʱ�ӳ�ʼ��
	delay_init();	    //��ʱ������ʼ��	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
//delay_ms(500);//MPU6050��Ҫ����ʱ�����250ms��ȫ����ʱ��	
	Motor_PWM_Init(9000,1);	//=====��ʼ��PWM 4KHZ������������� �����ʼ������ӿ� ��ΪMiniBalance_PWM_Init(9999,35) 200HZ
	PS2_SetInit();
	usart3_init(9600);
  laster_forward_control_set();
	usart1_init(9600);
	laster_backward_control_set();
	Set_Pwm(0,0);

}


