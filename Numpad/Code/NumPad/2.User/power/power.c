#include "power.h"

/* 外部复用参量 --------------------------------------------------------------*/
volatile u8 OnOff_Dectect_Key = 0;//1为按键按下，0为松开

/* 内部使用参量 --------------------------------------------------------------*/
extern volatile u32 time_10us;//定时器中会一直递增该值，用于长按开关机的定时

/* 内部调用函数 --------------------------------------------------------------*/
void PWR_ONOFF_Init(void);
void OnOff_Dectect_Key_Init(void);
void Charge_Full_IO_Init(void);

/**
	* @brief	初始化LED的GPIO口
	* @param  None
	* @retval None
	*/
void Power_Init(void)
{
	PWR_ONOFF_Init();
	OnOff_Dectect_Key_Init();
	Charge_Full_IO_Init();
	Delay_Ms(PWRON_KEY_PRESS_TIME / 100);//等待通电1.8s
	PWR_ONOFF = 1;//开机
}

/**
	* @brief	初始化开关按键的GPIO口
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
  * @brief  初始化Num键的外部中断
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级
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
  * @brief  Num键外部中断函数
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	static u8 Boot_On = 0;//用来防止开机长按导致关机。开机长按时是识别不到按下操作的（因为按下后才供电），只能识别到松开操作
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		if(PAin(0) == 0)//按下按键，开始计时，标志开始
		{
			time_10us = 0;
			OnOff_Dectect_Key = 1;//Num键按下，供外部检测
			Boot_On = 1;
		}
		else//松开按键，判断按下的事件
		{
			if((time_10us > PWROFF_KEY_PRESS_TIME) && Boot_On == 1)//按键长按关机
			{
				PWR_ONOFF = 0;
			}
			else//按键短按并松开
			{
				//功能保留
				//SYSTEM_LED = !SYSTEM_LED;
			}
		}
    EXTI_ClearITPendingBit(EXTI_Line0);//Clear the EXTI line pending bit
  }
}

/**
  * @brief  电量充满检测引脚初始化
	*					电量充满会自动断电，故该引脚不需要配置中断，空闲时检测一下即可
	*					检测到充满电4个ws2812就同时亮绿灯
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
