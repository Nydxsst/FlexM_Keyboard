/**
  ******************************************************************************
  * @author  
  * @version V1.0.1
  * @date    2020/04/07
  ******************************************************************************
  * @attention
	*
  * ---------------------------- ����ʹ��˵�� ---------------------------------*
  * ��Ҫ�õ������ţ�3��SPI,CE��CSN���ţ��궨�崦���޸ģ�,IRQ�ж�����
  *								 �������޸ĺ궨�廹Ҫ��exti.c�ļ��е��ⲿ�жϷ��������ƺͺ�
  *									 ��������жϱ�־���ж��ߣ�
  * �������̣�1.extern u8 Rxbuf[]�����ݽ��ջ�������
  *							extern u8 TX_RX_status
  *						 ���������ж����ͣ����ճɹ�TX_OK�����ͳɹ�RX_OK��
  *							����ʧ��MAX_TX�����ﵽ����ط�����������
  *						2.��ʼ��nrf2401��NRF24L01_Init�����ⲿ�жϣ�Exti_Init��
  *						3.ʹ�ܷ���ģʽ��NRF24L01_RX_Mode�������ģʽ��NRF24L01_TX_Mode��
  *						4.����ģʽ������NRF24L01_TxPacket����
  *							����ģʽ�����յ������ݽ���������Rxbuf[]
  *
  * ---------------------------- �������ܽ��� ---------------------------------*
  * ��126����ѡ��Ƶ��
  * ��ʵ��1��6��ͨѶ��1��6����ÿһ��ͨ����ַ��ͬ���ǹ���ͬ����ַ
  * ͨ��0��������40λ�����ַ��1~5����32λ��λ���õ�ַ
  *											��ͨ��1���ã���8λ�����ַ
  * ��߹�������2Mpbs
  * �����ڲ�CRC
  * ����Ӧ��
  * �ɵ���д�룬���ٷ���
  * �жϣ��ж�ʱ�����͵�ƽ����������ɲ��ҽ��յ�Ӧ���ź�
  *												���յ�����
  *												�ﵽ����ط�����
  * ����ģʽ��Enhanced ShockBurstTM:��Ӧ���źţ�֧���Զ��ط�
  *				ShockBurstTM				 :��Ӧ���źţ���֧���Զ��ط�
  * �������ݣ�����д��Ĵ���ʱ��CEλ�ø�����10us����
  * ͨ��Ƶ�ʣ�2400+RF_CH(0~125Mhz)
  * ʹ�ܽ���ģʽʱCEλ�ø�
  *
  * ͨ��0.1�����ݽ���ʹ�ܣ�2~5Ĭ��Ϊ0����ʹ�����ݽ���
  * ���͵�ַ����ʱ�ӵ�λд��
  *
  * ��δʵ�ֹ��ܣ�1��6ͨ��
  *								 DMA����
	*
  ******************************************************************************
  */

#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f10x.h"
#include "spi1.h"

/* �ⲿ���ã���Ķ���---------------------------------------------------------*/
#define MAX_TX  				0x10  //�ﵽ����ʹ����ж�
#define TX_OK   				0x20  //TX��������ж�
#define RX_OK   				0x40  //���յ������ж�				

/* ָ� --------------------------------------------------------------------*/
/* NRF24L01�Ĵ����������� */
#define NRF_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ�����ϼĴ�����ַ��Ϊ���յ�д���ַ��
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
/* SPI(NRF24L01)�Ĵ�����ַ */
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
															//bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
															//bit5:���ݷ�������ж�;bit6:���������ж�
#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
															//bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��

/* �ڲ����������������޸ģ� --------------------------------------------------*/
/* 24L01���ͽ������ݿ�ȶ��� */
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ�ȣ����32��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ�ȣ����32��
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���	

/* 24L01������ ---------------------------------------------------------------*/
/* SPIѡ�� */
#define NRF24L01_SPI_Init						SPI1_Init
#define NRF24L01_SPI_ReadWriteByte	SPI1_ReadWriteByte
/* IO���Ŷ��� */
#define NRF24L01_CLOCK					RCC_APB2Periph_GPIOA
#define NRF24L01_CE_PIN					GPIO_Pin_2
#define NRF24L01_CSN_PIN				GPIO_Pin_3
#define NRF24L01_CE_GPIO				GPIOA
#define NRF24L01_CSN_GPIO				GPIOA
#define NRF24L01_CE							PAout(2) //�����������ͻ���������
#define NRF24L01_CSN						PAout(3) //SPIƬѡ�ź�
/* �ж��������Ŷ��� */
#define NRF24L01_GPIO_CLK          		(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define NRF24L01_GPIO_PORT         		GPIOA
#define NRF24L01_GPIO_PIN          		GPIO_Pin_4
#define NRF24L01_GPIO_MODE						GPIO_Mode_IPU
#define NRF24L01_EXTI_PORTSOURCE   		GPIO_PortSourceGPIOA
#define NRF24L01_EXTI_PINSOURCE    		GPIO_PinSource4
#define NRF24L01_EXTI_LINE         		EXTI_Line4
#define NRF24L01_EXTI_Trigger					EXTI_Trigger_Falling
#define NRF24L01_EXTI_IRQ          		EXTI4_IRQn
#define NRF24L01_EXTI_IRQHandler			EXTI4_IRQHandler
#define NRF24L01_PreemptionPriority		1
#define NRF24L01_SubPriority					1


/* �ⲿ���ú��� --------------------------------------------------------------*/
void NRF24L01_Init(void);						//��ʼ��
u8 NRF24L01_Check(void);						//���24L01�Ƿ����
void NRF24L01_RX_Mode(void);				//����Ϊ����ģʽ
void NRF24L01_TX_Mode(void);				//����Ϊ����ģʽ
void NRF24L01_TxPacket(u8* txbuf);		//����һ����������


#endif
