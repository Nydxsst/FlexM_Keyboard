/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */
	
/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention ֻ��һ������ֻ��Ҫ������KEY�����߶���Ϊͬһ��IO����
  ******************************************************************************
  */

#ifndef __KEY_H
#define __KEY_H 

#include "stm32f10x.h"


/* �ⲿ���ã���Ķ���---------------------------------------------------------*/
#define KEY1_PRES 	1	//KEY0����
#define KEY2_PRES		2	//KEY1����
#define KEY3_PRES		3	//KEY2����

/* KEY������ -----------------------------------------------------------------*/
/* IO���Ŷ��� */
#define KEY1_CLK		RCC_APB2Periph_GPIOA
#define KEY1_PORT		GPIOA
#define KEY1_PIN		GPIO_Pin_0
#define KEY1_MODE		GPIO_Mode_IPU

#define KEY2_CLK		RCC_APB2Periph_GPIOA
#define KEY2_PORT		GPIOA
#define KEY2_PIN		GPIO_Pin_0
#define KEY2_MODE		GPIO_Mode_IPU

#define KEY3_CLK		RCC_APB2Periph_GPIOA
#define KEY3_PORT		GPIOA
#define KEY3_PIN		GPIO_Pin_0
#define KEY3_MODE		GPIO_Mode_IPU

#define KEY1_READ		GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN)
#define KEY2_READ		GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN)
#define KEY3_READ		GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN)


/* �ⲿ���ú��� --------------------------------------------------------------*/
void Key_Init(void);
u8 KEY_Scan(void);

#endif
