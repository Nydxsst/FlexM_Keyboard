/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/04/07
  ******************************************************************************
  * @attention
	* ����״̬�ǵ͵�ƽ
	* �ֲ��У�RES - 280us���ϵĵ͵�ƽ����˼��������װ�ض���2818��λ��
	*					�������2812ʱ��������������֮�䲻����RES
	*	������һ���Ա���ͬʱˢ������WS2812
  ******************************************************************************
  */

#ifndef __WS2812B_H
#define __WS2812B_H

#include "stm32f10x.h"


/* �ڲ����������������޸ģ� --------------------------------------------------*/
#define PIXEL_MAX 4 //the count of all the 2812 

/* WS2812B������ -------------------------------------------------------------*/
#define WS2812B_GPIO_CLK				RCC_APB2Periph_GPIOA
#define WS2812B_GPIO_PORT				GPIOA
#define WS2812B_GPIO_PIN				GPIO_Pin_15

#define WS2812B_TIM							TIM2
#define WS2812B_TIM_RCC_CMD			RCC_APB1PeriphClockCmd
#define WS2812B_TIM_CLK					RCC_APB1Periph_TIM2
#define WS2812B_TIM_REMAP				GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE)
#define WS2812B_TIM_OCINIT			TIM_OC1Init
#define WS2812B_TIM_OC_PRE			TIM_OC1PreloadConfig
#define WS2812B_TIM_CCR					CCR1

#define WS2812B_NVIC_IRQn				DMA1_Channel2_IRQn
#define WS2812B_NVIC_IRQHandler	DMA1_Channel2_IRQHandler
#define WS2812B_NVIC_PRE				2 //��ռ���ȼ�
#define WS2812B_NVIC_SUB				1 //�����ȼ�

#define WS2812B_DMA_RCC_CMD			RCC_AHBPeriphClockCmd
#define WS2812B_DMA_CLK					RCC_AHBPeriph_DMA1
#define WS2812B_DMA_CNANNLE			DMA1_Channel2
#define WS2812B_DMA_FLAG				DMA1_FLAG_TC2


/* �ⲿ���ú��� --------------------------------------------------------------*/
void WS2812_Init(void);
u8 WS2812_Refresh(void);


#endif
