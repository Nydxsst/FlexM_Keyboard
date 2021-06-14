#include "usart_debug.h"

/**
	* @brief	��ʼ��USART
	* @param  BaudRate
	* @retval None
	*/
void Usart_Debug_Init(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* Usart GPIO ���� */
	//USART_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Usart NVIC ���� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	/* USART ��ʼ������ */
	USART_InitStructure.USART_BaudRate = BaudRate;																		//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); 		//��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
}

/**
	* @brief	USART1�жϷ�����
	* @param  None
	* @retval None
	*/
void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART1);
		USART_SendData(USART1, ucTemp);
	}
}

/**
	* @brief	�ض���c�⺯����ʹ��printf.scanf.put.get����ʹ��
	* @param  None
	* @retval None
	*/
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE* f)
{
	while((USART1->SR & 0X40) == 0); //ѭ������,ֱ���������
	USART1->DR = (u8) ch;
	return ch;
}

