#include "ws2812b.h"

/* �ⲿ���ò��� --------------------------------------------------------------*/
/* RGB��ɫֵ���� */
u32 WS2812B_ALL_COLOR[PIXEL_MAX];

/* �ڲ�ʹ�ò��� --------------------------------------------------------------*/
/* ͨѶ�ߵ͵�ƽʱ�� */
const u16 T0 = 22;
const u16 T1 = 68;
/* DMA��������жϱ�־ */
u8 WS2812B_DMA_Tx_Finish = 1;
/* ��ʱ����ֵ����ṹ�� */
typedef struct
{
	u16 data[24 * PIXEL_MAX];
	const u16 tail;//��ʱ����ֵ����,����͵�ƽ���ڽ�������
} WS2812B_Tx_frame;
/* ��ʱ����ֵ����(PWM�ߵ�ƽʱ��),����DMA���� */
WS2812B_Tx_frame frame = {.tail = 0};


/**
	* @brief	��ʼ��WS2812������
	* @param  None
	* @retval None
	*/
void WS2812_Init()
{
	GPIO_InitTypeDef				GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef				TIM_OCInitStruct;
	DMA_InitTypeDef					DMA_InitStruct;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* GPIO_Init */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(WS2812B_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = WS2812B_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WS2812B_GPIO_PORT, &GPIO_InitStruct);

	/* TIM_TimeBaseInit */
	WS2812B_TIM_RCC_CMD(WS2812B_TIM_CLK, ENABLE);
	WS2812B_TIM_REMAP;
	TIM_TimeBaseInitStruct.TIM_Period = 90 - 1; //(800khz, T=1.25us)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(WS2812B_TIM, &TIM_TimeBaseInitStruct);

	/* TIM_OCInit */
	/*PWMģʽ1:	���ϼ���,TIMx_CNT<TIMx_CCR1ʱ���Ϊ��Ч��ƽ,����Ϊ��Ч��ƽ
							���¼���,TIMx_CNT>TIMx_CCR1ʱ���Ϊ��Ч��ƽ,����Ϊ��Ч��ƽ*/
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		// ��Ч��ƽΪ�ߵ�ƽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;// ���ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;														// ���ó�ʼռ�ձ���ֵ
	WS2812B_TIM_OCINIT(WS2812B_TIM, &TIM_OCInitStruct);
	WS2812B_TIM_OC_PRE(WS2812B_TIM, TIM_OCPreload_Enable);

	/* NVIC_Init */
	NVIC_InitStructure.NVIC_IRQChannel = WS2812B_NVIC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = WS2812B_NVIC_PRE;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = WS2812B_NVIC_SUB;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);

	/* DMA_Init */
	WS2812B_DMA_RCC_CMD(WS2812B_DMA_CLK, ENABLE);
	DMA_DeInit(WS2812B_DMA_CNANNLE);
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;											// DMAģʽ��һ��ģʽ
	DMA_InitStruct.DMA_BufferSize = 0;															// �������ݸ���
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;									// ���򣺴��ڴ浽����
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;												// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&frame;						// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// �ڴ����ݵ�λ
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;						// �ڴ��ַ����
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&WS2812B_TIM->WS2812B_TIM_CCR;	// �����ַ
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// �������ݵ�λ
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// �����ַ����
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;								// ���ȼ�
	DMA_Init(WS2812B_DMA_CNANNLE, &DMA_InitStruct);

	DMA_ITConfig(WS2812B_DMA_CNANNLE, DMA_IT_TC, ENABLE);
	TIM_DMACmd(WS2812B_TIM, TIM_DMA_Update, ENABLE);
}
/**
	* @brief	�жϷ�����
	* @param  None
	* @retval None
	*/
void WS2812B_NVIC_IRQHandler(void)
{
	if(DMA_GetITStatus(WS2812B_DMA_FLAG) != RESET)
	{
		SYSTEM_LED = !SYSTEM_LED;
		//printf("WS2812B Intrrupt!\r\n");
		WS2812B_DMA_Tx_Finish = 1;
		DMA_Cmd(WS2812B_DMA_CNANNLE, DISABLE);
		TIM_Cmd(WS2812B_TIM, DISABLE);
		DMA_ClearITPendingBit(WS2812B_DMA_FLAG);
	}
}

/**
	* @brief	WS2812������ɫ, DMA����ʱ��Ϊ(1 + 24 * PIXEL_MAX) * T(1.25us)
	*					�����ϻ���Ҫ224���ڵĵ͵�ƽ���Reset
	*					�������bug�����ε��øú���֮��������ʱ224������
	* @param  None
	* @retval 1Ϊ���ͳɹ�,0Ϊ����ʧ��(��һ��DMA���仹û���)
	*/
u8 WS2812_Refresh(void)
{
	u8 tmp = 0, n = 0, i = 0;

	if(WS2812B_DMA_Tx_Finish == 0)return 0;//����ϴ�DMA�����Ƿ��Ѿ����
	WS2812B_DMA_Tx_Finish = 0;//��ɾͱ�־���㣬�ٽ�����һ�δ���

	for(n = 0; n < PIXEL_MAX; n++)
	{
		tmp = (u8)(WS2812B_ALL_COLOR[n] >> 8);
		for(i = 0; i < 8; i++)
		{
			frame.data[i + 24 * n] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
		tmp = (u8)(WS2812B_ALL_COLOR[n] >> 16);
		for(i = 0; i < 8; i++)
		{
			frame.data[8 + i + 24 * n] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
		tmp = (u8)WS2812B_ALL_COLOR[n];
		for(i = 0; i < 8; i++)
		{
			frame.data[16 + i + 24 * n] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
	}
	DMA_SetCurrDataCounter(WS2812B_DMA_CNANNLE, 24 * PIXEL_MAX + 1);
	DMA_Cmd(WS2812B_DMA_CNANNLE, ENABLE);
	TIM_Cmd(WS2812B_TIM, ENABLE);
	return 1;
}
