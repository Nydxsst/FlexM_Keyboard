#include "spi1.h"

/**
	* @brief	��ʼ��SPI
	* @param  None
	* @retval None
	*/
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//PORTBʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);		//SPI1ʱ��ʹ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;												//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	//���岨����Ԥ��Ƶ��ֵ��APB1Ԥ��Ƶ������72M��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//ָ�����ݴ����MSBλ����LSBλ��ʼ:
	SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  																//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI1, ENABLE); 		//ʹ��SPI����
}

/**
	* @brief	����SPI���ٶ�
	* @param  SPI_BaudRatePrescaler��SPI_BaudRatePrescaler_2   2��Ƶ
																	 SPI_BaudRatePrescaler_8   8��Ƶ
																	 SPI_BaudRatePrescaler_16  16��Ƶ
																	 SPI_BaudRatePrescaler_256 256��Ƶ
	* @retval None
	*/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler;	//����SPI1�ٶ�
	SPI_Cmd(SPI1, ENABLE);
}

/**
	* @brief	����SPI���ٶ�
	* @param  TxData:Ҫд����ֽ�
	* @retval ��ȡ�����ֽ�
	*/
u8 SPI1_ReadWriteByte(u8 TxData)
{
	u8 retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)  //�ȴ�SPI�ɷ�������
	{
		retry++;
		if(retry > 200)return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry = 0;

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)  //�ȴ�SPI�ɽ�������
	{
		retry++;
		if(retry > 200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
}