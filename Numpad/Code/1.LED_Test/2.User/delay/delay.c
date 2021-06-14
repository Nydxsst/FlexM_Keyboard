#include "delay.h"

/* �ڲ�ʹ�ò��� --------------------------------------------------------------*/
u8  Fac_Us = 0;							//us��ʱ������
u16 Fac_Ms = 0;							//ms��ʱ������

/**
	* @brief	��ʼ���ӳٺ���
	* @param  None
	* @retval None
	* @note		SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
	* @note   SYSCLK:ϵͳʱ��
	*/
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	Fac_Us = 9;										//ÿ1us���������ݼ�9������72M/8��
	Fac_Ms = (u16)Fac_Us * 1000;	//ÿ1ms���������ݼ�9000����
}

/**
	* @brief	us����ʱ
	* @param	nus:��ʱ��us��
	* @retval None
	* @note		��һ�������,usԽС,����Խ��
	*/		    
void Delay_Us(u32 Nus)
{
	SysTick->LOAD = Fac_Us * Nus;//�Զ���װ��ֵ
	SysTick->VAL = 0x00;//�����ʱ����ֵ
	SysTick->CTRL |= (1 << 0);//SysTickʹ�ܣ�ʹ�ܺ�ʱ����ʼ����
	while(!(SysTick->CTRL & (1 << 16)));//�ж��Ƿ����0������0ʱCTRL�ĵ�16λ����1����ȡ����Զ���0
}

/**
	* @brief	Ms����ʱ
	* @param	Nms:��ʱ��Ms��
	* @retval None
	* @note		SysTick->LOADΪ24λ�Ĵ���, ��ʱ��Us��/���� = ����Ĵ�������
	* 				��Nms * 1000 * SysTick_Clk(9M) <= 2^24, ��Nms <= 1864
	*/				    
void Delay_Ms(u16 Nms)
{
	SysTick->LOAD = Fac_Ms * Nms;//�Զ���װ��ֵ
	SysTick->VAL = 0x00;//�����ʱ����ֵ
	SysTick->CTRL |= (1 << 0);//SysTickʹ�ܣ�ʹ�ܺ�ʱ����ʼ����
	while(!(SysTick->CTRL & (1 << 16)));//�ж��Ƿ����0������0ʱCTRL�ĵ�16λ����1����ȡ����Զ���0
}
