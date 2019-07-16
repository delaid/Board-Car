#include "key.h"

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
		
	// 使能IO重映射时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  
	// 使能GPIO端口A时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 使能SWD 禁用JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;					// IO管脚8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// IO口速度为50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// 根据设定参数初始化GPIO
	OPEN_BLUE;
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY_LEFT按下
//2，KEY_RIGHT按下
//3，KEY_MOD按下 
//4，KEY3按下 
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
//每个隔20mS扫描一次
u8 key_scan(u8 mode)
{	 
	
	static u8 key_up=1;//按键按松开标志
	static int key_mode_conut;//模式长按计数 
	static int key_fuction_conut;

	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_LEFT==0||KEY_RIGHT==0||KEY_MOD==0||KEY_BACK==0))
	{
		delay_ms(10);//去抖动 
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
		if(key_mode_conut++>150){//3S以上
		key_mode_conut=0;
	    return KEY_MOD_PRES;
		} 
	}		
	else key_mode_conut=0;
	
	 if(KEY_BACK==0){
		if(key_fuction_conut++>150){//3S以上
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
	
	
	
 	return KEY_NONE;// 无按键按下
	
}






