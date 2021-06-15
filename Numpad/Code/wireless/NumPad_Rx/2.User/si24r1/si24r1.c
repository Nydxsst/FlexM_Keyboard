#include "si24r1.h"
#include "delay.h"


/* �ⲿ���ò��� --------------------------------------------------------------*/
u8 NRF24L_Tx_Rx_Sta = 0; 			//IRF24L01 �������ж�����
u8 NRF24L_Rx_Buf[RX_PLOAD_WIDTH]; //IRF24L01 ���ݽ��ջ�����
u8 NRF24L_Tx_Buf[TX_PLOAD_WIDTH];

/* �ڲ�ʹ�ò��� --------------------------------------------------------------*/
/* �շ���ַ */
const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x02};
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x02};

/* �ڲ����ú��� --------------------------------------------------------------*/
static void NRF24L01_Exti_Init(void);
u8 NRF24L01_Write_Buf(u8 reg, u8* pBuf, u8 u8s);//д������
u8 NRF24L01_Read_Buf(u8 reg, u8* pBuf, u8 u8s);	//��������
u8 NRF24L01_Read_Reg(u8 reg);										//���Ĵ���
u8 NRF24L01_Write_Reg(u8 reg, u8 value);				//д�Ĵ���


/**
	* @brief	��ʼ��24L01
	* @param  None
	* @retval None
	*/
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(NRF24L01_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN;
	GPIO_Init(NRF24L01_CE_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_PIN;
	GPIO_Init(NRF24L01_CSN_GPIO, &GPIO_InitStructure);
	NRF24L01_CE = 0; 		//NRFʹ��
	NRF24L01_CSN = 1;		//SPIƬѡȡ��
	
	NRF24L01_Exti_Init();//�ж���ʹ��
	NRF24L01_SPI_Init();//��ʼ��SPI

	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error\r\n");
		Delay_Ms(500);
	}
	printf("NRF24L01 OK\r\n");
	NRF24L01_RX_Mode();
	
	//NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_AW, 0x03); //����ͨ�ŵ�ַ�ĳ��ȣ�Ĭ��ֵʱ0x03,����ַ����Ϊ5�ֽ�
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH); //����RX�ڵ��ַ���Զ�Ӧ���ַ��,��ҪΪ��ʹ��ACK
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //ʹ��ͨ��0�Զ�Ӧ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40); //����RFͨ��Ϊ40hz(1-64Hz������)
	
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a); //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� 0x1A
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,����������ر� ��ע�⣺����������ر�/����ֱ��Ӱ��ͨ��,Ҫ������������Ҫ�رն��ر�0x0f��
	
	NRF24L01_RX_Mode(); //Ĭ��Ϊ����ģʽ
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��
u8 NRF24L01_Check(void)
{
	u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	u8 i;
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5); //д��5���ֽڵĵ�ַ.
	NRF24L01_Read_Buf(TX_ADDR, buf, 5); //����д��ĵ�ַ
	for(i = 0; i < 5; i++)if(buf[i] != 0XA5)break;
	if(i != 5)return 1; //���24L01����
	return 0;		 //��⵽24L01
}
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
	u8 status;
	NRF24L01_CSN = 0;               //ʹ��SPI����
	status = NRF24L01_SPI_ReadWriteByte(reg); //���ͼĴ�����
	NRF24L01_SPI_ReadWriteByte(value);      //д��Ĵ�����ֵ
	NRF24L01_CSN = 1;               //��ֹSPI����
	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
	NRF24L01_CSN = 0;          //ʹ��SPI����
	NRF24L01_SPI_ReadWriteByte(reg);   //���ͼĴ�����
	reg_val = NRF24L01_SPI_ReadWriteByte(0XFF); //��ȡ�Ĵ�������
	NRF24L01_CSN = 1;          //��ֹSPI����
	return(reg_val);           //����״ֵ̬
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Read_Buf(u8 reg, u8* pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;           //ʹ��SPI����
	status = NRF24L01_SPI_ReadWriteByte(reg); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr = 0; u8_ctr < len; u8_ctr++)pBuf[u8_ctr] = NRF24L01_SPI_ReadWriteByte(0XFF); //��������
	NRF24L01_CSN = 1;     //�ر�SPI����
	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8* pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;          //ʹ��SPI����
	status = NRF24L01_SPI_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr = 0; u8_ctr < len; u8_ctr++)NRF24L01_SPI_ReadWriteByte(*pBuf++); //д������
	NRF24L01_CSN = 1;       //�ر�SPI����
	return status;          //���ض�����״ֵ̬
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
void NRF24L01_TxPacket(u8* txbuf)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); //д���ݵ�TX BUF  32���ֽ�
	NRF24L01_CE = 1; //��������
	Delay_Us(10);  //CE�����ߵ�ƽ10us
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);
	NRF24L01_CE = 1; //CEΪ��,�������ģʽ
	Delay_Us(200);
}

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,����������.
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);
	NRF24L01_CE = 1; //CEΪ��,10us����������
}
/**
	* @brief	��ʼ���ⲿ�ж�
	* @param  None
	* @retval None
	*/
static void NRF24L01_Exti_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* GPIO_Init */
	RCC_APB2PeriphClockCmd(NRF24L01_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = NRF24L01_GPIO_MODE;
	GPIO_Init(NRF24L01_GPIO_PORT, &GPIO_InitStructure);
	/* NVIC_Init */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = NRF24L01_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NRF24L01_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NRF24L01_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	/* EXTI_Init */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	GPIO_EXTILineConfig(NRF24L01_EXTI_PORTSOURCE, NRF24L01_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = NRF24L01_EXTI_LINE;
	EXTI_InitStructure.EXTI_Trigger = NRF24L01_EXTI_Trigger;
	EXTI_Init(&EXTI_InitStructure);
}

/**
	* @brief	�жϷ�����
	* @param  None
	* @retval None
	*/
void NRF24L01_EXTI_IRQHandler(void)
{	
	u8 sta;
	static u8 time = 0;
	if(EXTI_GetITStatus(NRF24L01_EXTI_LINE) != RESET)
	{
		sta = NRF24L01_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����ֵ
		if(sta & MAX_TX) //�ﵽ����ط�����
		{
			printf("MAX_TX\r\n");
			NRF24L_Tx_Rx_Sta = MAX_TX;
			NRF24L01_Write_Reg(FLUSH_TX, 0xff); //���TX FIFO�Ĵ���
			if(time < 10)
			{
				NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //����жϱ�־
				EXTI_ClearITPendingBit(NRF24L01_EXTI_LINE);
				NRF24L01_TxPacket(NRF24L_Tx_Buf);
				time ++;
				return;
			}
			else 
			{
				NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //����жϱ�־
				EXTI_ClearITPendingBit(NRF24L01_EXTI_LINE);
				NRF24L01_RX_Mode();
				time = 0;
				return;
			}
		}
		else if(sta & TX_OK)
		{
			printf("TX_OK\r\n");
			NRF24L_Tx_Rx_Sta = TX_OK;
			NRF24L01_RX_Mode();
		}
		else if(sta & RX_OK)
		{
			printf("RX_OK\r\n");
			NRF24L_Tx_Rx_Sta = RX_OK;
			NRF24L01_Read_Buf(RD_RX_PLOAD, NRF24L_Rx_Buf, RX_PLOAD_WIDTH); //��ȡ����
			NRF24L01_Write_Reg(FLUSH_RX, 0xff); //���RX FIFO�Ĵ���
			//printf("Rx: %s\r\n", NRF24L_Rx_Buf);
//			printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", NRF24L_Rx_Buf[0],NRF24L_Rx_Buf[1],NRF24L_Rx_Buf[2],
//			NRF24L_Rx_Buf[3],NRF24L_Rx_Buf[4],NRF24L_Rx_Buf[5],NRF24L_Rx_Buf[6],NRF24L_Rx_Buf[7],NRF24L_Rx_Buf[8],
//			NRF24L_Rx_Buf[9],NRF24L_Rx_Buf[10]);
			NRF24L01_TX_Mode();
			//NRF24L01_TxPacket("Minifly back");
			NRF24L01_TxPacket(NRF24L_Tx_Buf);
			time = 0;
		}
		NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //����жϱ�־
		EXTI_ClearITPendingBit(NRF24L01_EXTI_LINE);
	}
}
