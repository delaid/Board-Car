#include  "Hand_action_task.h"

//APDS9960�������
#define APDS9960_TASK_PRIO				3					//�����������ȼ�
#define APDS9960_STK_SIZE				128					//���������ջ��С
	
OS_STK APDS9960_TASK_STK[APDS9960_STK_SIZE]; 	//�����ջ
void apds9960_task(void *pdata);							//������


void Hand_action_task_Create(void)
{
	OSTaskCreate(apds9960_task,(void *)0,(OS_STK*)&APDS9960_TASK_STK[APDS9960_STK_SIZE-1],APDS9960_TASK_PRIO);	//����APDS9960��������
}


/*********************************************************************************************************
** Function name:       void apds9960_task(void *pdata)
** Descriptions:        ���Ƽ����������ڶ�ȡ����������Ӧ���㷨�ϳ���������Ҫ�������趨һ����������
						ͨ�����ƴ�����
						���趨��Ӧ�ĵ�ѹֵ�����Ͷ�Ӧ�����źŸ��ƺ�APP�Լ����ؽ���ͨѶ��
** input parameters:    pdata: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void apds9960_task(void *pdata)
{
	while(1)
	{	
		APDS_SCAN();
		APDS_Mode();
		delay_ms(20);
	}
}





