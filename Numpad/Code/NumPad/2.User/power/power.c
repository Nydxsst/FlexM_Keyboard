#include "power.h"

/* �ⲿ���ò��� --------------------------------------------------------------*/
volatile u8 OnOff_Dectect_Key = 0;//1Ϊ�������£�0Ϊ�ɿ�

/* �ڲ�ʹ�ò��� --------------------------------------------------------------*/
extern volatile u32 time_10us;//��ʱ���л�һֱ������ֵ�����ڳ������ػ��Ķ�ʱ

/* �ڲ����ú��� --------------------------------------------------------------*/
void PWR_ONOFF_Init(void);
void OnOff_Dectect_Key_Init(void);
void Charge_Full_IO_Init(void);

/**
	* @brief	��ʼ��LED��GPIO��
	* @param  None
	* @retval None
	*/
void Power_Init(void)
{
	PWR_ONOFF_Init();
	OnOff_Dectect_Key_Init();
	Charge_Full_IO_Init();
	Delay_Ms(PWRON_KEY_PRESS_TIME / 100);//�ȴ�ͨ��1.8s
	PWR_ONOFF = 1;//����
}

/**
	* @brief	��ʼ�����ذ�����GPIO��
	* @param  None
	* @retval None
	*/
void PWR_ONOFF_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  ��ʼ��Num�����ⲿ�ж�
  * @param  None
  * @retval None
  */
void OnOff_Dectect_Key_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
  /* GPIO_Init */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* NVIC_Init */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	/* Exti_Init */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStructure);
}


/**
  * @brief  Num���ⲿ�жϺ���
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	static u8 Boot_On = 0;//������ֹ�����������¹ػ�����������ʱ��ʶ�𲻵����²����ģ���Ϊ���º�Ź��磩��ֻ��ʶ���ɿ�����
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		if(PAin(0) == 0)//���°�������ʼ��ʱ����־��ʼ
		{
			time_10us = 0;
			OnOff_Dectect_Key = 1;//Num�����£����ⲿ���
			Boot_On = 1;
		}
		else//�ɿ��������жϰ��µ��¼�
		{
			if((time_10us > PWROFF_KEY_PRESS_TIME) && Boot_On == 1)//���������ػ�
			{
				PWR_ONOFF = 0;
			}
			else//�����̰����ɿ�
			{
				//���ܱ���
				//SYSTEM_LED = !SYSTEM_LED;
			}
		}
    EXTI_ClearITPendingBit(EXTI_Line0);//Clear the EXTI line pending bit
  }
}

/**
  * @brief  ��������������ų�ʼ��
	*					�����������Զ��ϵ磬�ʸ����Ų���Ҫ�����жϣ�����ʱ���һ�¼���
	*					��⵽������4��ws2812��ͬʱ���̵�
  * @param  None
  * @retval None
  */
void Charge_Full_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
