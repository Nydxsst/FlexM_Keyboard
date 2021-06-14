#include "adc.h"

/**
	* @brief	��ʼ��ADC(��ʹ�ù���ͨ��)
	* @param  None
	* @retval None
	*/
void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO_Init */
	RCC_APB2PeriphClockCmd(ADC_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);
	
	/* ADC_Init */
	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   												//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	ADC_DeInit(ADCx);  																			//��λADC
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADCx, &ADC_InitStructure);											//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	ADC_Cmd(ADCx, ENABLE);											//ʹ��ָ����ADC1
	ADC_ResetCalibration(ADCx);									//ʹ�ܸ�λУ׼
	while(ADC_GetResetCalibrationStatus(ADCx));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADCx);									//����ADУ׼
	while(ADC_GetCalibrationStatus(ADCx));			//�ȴ�У׼����
//	ADC_SoftwareStartConvCmd(ADCx, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}

/**
	* @brief	���ADCֵ
	* @param  ch: adcͨ��
	* @retval None
	*/
u16 Get_Adc_val(void)
{
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_7Cycles5);	//ADC1,ADCͨ��,����ͨ��ʹ������,����ʱ��
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));	//�ȴ�ת������
	return ADC_GetConversionValue(ADCx);						//�������һ��ADC1�������ת�����
}

/**
	* @brief	���ƽ��ADCֵ
	* @param  ch: adcͨ��
						times: ����
	* @retval None 
	*/
u16 Get_Adc_Val_Average(u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0; t < times; t++)
	{
		temp_val += Get_Adc_val();
		Delay_Ms(5);
	}
	return temp_val / times;
}

/**
	* @brief	��ȡƽ����ص�����4.2V�򷵻�420��
	* @param  ch: adcͨ��
						times: ����
	* @retval None 
	*/
u8 Get_Adc_BAT_Percent(u8 times)
{
	u32 temp_val = 0;
	u16 BAT = 0;
	u8 t;
	for(t = 0; t < times; t++)
	{
		temp_val += Get_Adc_val();
//		Delay_Ms(5);
	}
	temp_val = (temp_val / times) * 330 * 2 / 4096;
	if(temp_val > 420)BAT = 99;
	else if(temp_val > 406)BAT = 90;
	else if(temp_val > 398)BAT = 80;
	else if(temp_val > 392)BAT = 70;
	else if(temp_val > 387)BAT = 60;
	else if(temp_val > 382)BAT = 50;
	else if(temp_val > 379)BAT = 40;
	else if(temp_val > 377)BAT = 30;
	else if(temp_val > 374)BAT = 20;
	else if(temp_val > 368)BAT = 10;
	else if(temp_val > 345)BAT = 5;
	else BAT = 0;
	return BAT;
}






