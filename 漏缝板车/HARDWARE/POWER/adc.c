#include "adc.h"
/**************************************************************************
�������ܣ�ACD��ʼ����ص�ѹ���
��ڲ�������
����  ֵ����
**************************************************************************/
void  Adc_Init(void)
{    
 	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//PA4 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	  /* DISABLE ADC1 DMA */  
    ADC_DMACmd(ADC1, DISABLE);    //�ر�DMA���� ������Ҫ�õ���  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	
}		

/**************************************************************************
�������ܣ�AD����
��ڲ�����ADC1 ��ͨ��
����  ֵ��ADת�����
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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
�������ܣ���ȡ��ص�ѹ 
��ڲ�������
����  ֵ����ص�ѹ ��λMV
**************************************************************************/
uint16_t Get_battery_volt(void)   
{ 
	static uint16_t Volt[10];//��ص�ѹ
    static uint16_t Volt_Value=8000;
	u32 Volt_Sum;
	static u8  Adc_count;
	
	Volt_Value=Get_Adc(ADC_Channel_11)*3300/4095*2.8;	//�����ѹ���������ԭ��ͼ�򵥷������Եõ�	
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
�������ܣ���ȡ������ѹ 
��ڲ�������
����  ֵ��������ѹ ��λMV
**************************************************************************/
int Get_socket_volt(void)   
{ 
	static uint16_t Volt[10];//��ص�ѹ
    static uint16_t Volt_Value=8000;
	u32 Volt_Sum;
	static u8  Adc_count;
	
	Volt_Value=Get_Adc(ADC_Channel_10)*3300/4095*4.922;	//�����ѹ���������ԭ��ͼ�򵥷������Եõ�	
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

