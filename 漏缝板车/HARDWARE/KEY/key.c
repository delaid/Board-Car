#include "key.h"

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
		
	// ʹ��IO��ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
	// ʹ��GPIO�˿�Aʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// ʹ��SWD ����JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;					// IO�ܽ�8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO���ٶ�Ϊ50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// �����趨������ʼ��GPIO
	OPEN_BLUE;
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY_LEFT����
//2��KEY_RIGHT����
//3��KEY_MOD���� 
//4��KEY3���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
//ÿ����20mSɨ��һ��
u8 key_scan(u8 mode)
{	 
	
	static u8 key_up=1;//�������ɿ���־
	static int key_mode_conut;//ģʽ�������� 
	static int key_fuction_conut;

	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_LEFT==0||KEY_RIGHT==0||KEY_MOD==0||KEY_BACK==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_LEFT==0)
			return KEY_LEFT_PRES;
		else if(KEY_RIGHT==0)
			return KEY_RIGHT_PRES;
		else if(KEY_MOD==0){
			    return KEY_FORWARD_PRES;
		    }
		else if(KEY_BACK==0)
			return KEY_BACK_PRES;
	}else if(KEY_LEFT==1&&KEY_RIGHT==1&&KEY_MOD==1&&KEY_BACK==1)key_up=1; 
   
    if(KEY_MOD==0){
		if(key_mode_conut++>150){//3S����
		key_mode_conut=0;
	    return KEY_MOD_PRES;
		} 
	}		
	else key_mode_conut=0;
	
	 if(KEY_BACK==0){
		if(key_fuction_conut++>150){//3S����
		key_fuction_conut=0;
	    return KEY_FOUNCTION_PRES;
		} 
	}		
	else key_fuction_conut=0;
	
	if(KEY_LEFT==0)
	{	
	    return KEY_LIGHT_ADD;
	}
	
	
	if(KEY_RIGHT==0)
	{	
	    return KEY_LIGHT_DECRE;	
	}
	
	
	
 	return KEY_NONE;// �ް�������
	
}






