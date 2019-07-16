#include "adc.h"
/**************************************************************************
函数功能：ACD初始化电池电压检测
入口参数：无
返回  值：无
**************************************************************************/
void  Adc_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	//PA4 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	  /* DISABLE ADC1 DMA */  
    ADC_DMACmd(ADC1, DISABLE);    //关闭DMA请求 （不需要用到）  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_ResetCalibration(ADC1);	//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
}		

/**************************************************************************
函数功能：AD采样
入口参数：ADC1 的通道
返回  值：AD转换结果
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

void SortU16(uint16_t Buff[],uint16_t n)
{
		uint16_t i,j; 
		uint16_t tmp;

	for(i= n-1;i>=1;i--)
	{
	 	for(j=0;j<i;j++)
		{
			if(Buff[j] > Buff[j+1])
			{
		   		tmp = Buff[j+1];
				Buff[j+1] = Buff[j];
				Buff[j] = tmp;
			}
		}
	}
}


/**************************************************************************
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压 单位MV
**************************************************************************/
uint16_t Get_battery_volt(void)   
{ 
	static uint16_t Volt[10];//电池电压
    static uint16_t Volt_Value=8000;
	u32 Volt_Sum;
	static u8  Adc_count;
	
	Volt_Value=Get_Adc(ADC_Channel_11)*3300/4095*2.8;	//电阻分压，具体根据原理图简单分析可以得到	
	Adc_count++;
    Volt[Adc_count]=Volt_Value;
	if(Adc_count==10)
	{
		SortU16(Volt,10);
		Volt_Sum=(Volt[1]+Volt[2]+Volt[3]+Volt[4]+Volt[5]+Volt[6]+Volt[7]+Volt[8]);
		Volt_Value=Volt_Sum>>3;
		Adc_count=0;
	}
	 return Volt_Value;
}


/**************************************************************************
函数功能：读取插座电压 
入口参数：无
返回  值：插座电压 单位MV
**************************************************************************/
int Get_socket_volt(void)   
{ 
	static uint16_t Volt[10];//电池电压
    static uint16_t Volt_Value=8000;
	u32 Volt_Sum;
	static u8  Adc_count;
	
	Volt_Value=Get_Adc(ADC_Channel_10)*3300/4095*4.922;	//电阻分压，具体根据原理图简单分析可以得到	
	Adc_count++;
    Volt[Adc_count]=Volt_Value;
	if(Adc_count==10)
	{
		SortU16(Volt,10);
		Volt_Sum=(Volt[1]+Volt[2]+Volt[3]+Volt[4]+Volt[5]+Volt[6]+Volt[7]+Volt[8]);
		Volt_Value=Volt_Sum>>3;
		Adc_count=0;
	}
	 return Volt_Value;
}

