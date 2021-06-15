/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention 
	* ������õ�PA13.PA14.PA15.PB3.PB4������Ҫ��jlink
	* RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	* GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  ******************************************************************************
  */
	
#include "stm32f10x.h"
#include "power.h"
#include "time.h"
#include "ws2812b.h"
#include "adc.h"
#include "hw_config.h"
#include "keyboard.h"
#include "si24r1.h"

/* WS2812 */
extern u32 WS2812B_ALL_COLOR[PIXEL_MAX];

/* USB������ɱ�־�����ж��в��ı䣩 */
__IO uint8_t PrevXferComplete = 1;

/* NRF24L01 */
extern u8 NRF24L_Rx_Buf[];
extern u8 NRF24L_Tx_Buf[];

int main(void)
{
//	u16 Get_Adc_Val = 0;
//	u8 Charge_Full_Flag = 0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	Usart_Debug_Init(115200);
	Led_Init();
	TIM_DELAY_Init();
	WS2812_Init();
	Adc_Init();
	Power_Init();
	NRF24L01_Init();
	
//	USB_Port_Set(0); 	//USB�ȶϿ�
//	Delay_Ms(100);
//	USB_Port_Set(1);	//USB�ٴ�����
//	USB_Interrupts_Config();
//	Set_USBClock();
//	USB_Init();
//	Keyboard_Init();

//	WS2812B_ALL_COLOR[0] = 0xFF00FF;
//	WS2812B_ALL_COLOR[1] = 0xFF0000;
//	WS2812B_ALL_COLOR[2] = 0x00FF00;
//	WS2812B_ALL_COLOR[3] = 0x0000FF;
//	WS2812_Refresh();
	
	printf("start\r\n");
	
	NRF24L_Rx_Buf[0] = 'n';//4λ���ַ���
	NRF24L_Rx_Buf[1] = 'o';//4λ���ַ���
	NRF24L_Rx_Buf[2] = 'n';//4λ���ַ���
	NRF24L_Rx_Buf[3] = 'a';//4λ���ַ���
	NRF24L_Rx_Buf[4] = '\0';//4λ���ַ���
	
	NRF24L_Tx_Buf[0] = 'b';//4λ���ַ���
	NRF24L_Tx_Buf[1] = 'a';//4λ���ַ���
	NRF24L_Tx_Buf[2] = 'c';//4λ���ַ���
	NRF24L_Tx_Buf[3] = 'k';//4λ���ַ���
	NRF24L_Tx_Buf[4] = '\0';//4λ���ַ���
	
	while(1)
	{
		//printf("receive: %s\r\n", NRF24L_Rx_Buf);
		/* ��ص�ѹ��ȡ */
//		Get_Adc_Val = Get_Adc_Val_Average(5);
//		printf("%d\r\n", Get_Adc_BAT_Percent(2));

		/* ����������ʾ */
//		Charge_Full_Flag = PWR_FULL_DECT;
//		printf("Charge_Full_Flag = %d\r\n", Charge_Full_Flag);
//		if(Charge_Full_Flag == 0)//�������δ�ӵ��
//		{
//			WS2812B_ALL_COLOR[0] = 0x000F00;
//			WS2812B_ALL_COLOR[1] = 0x000F00;
//			WS2812B_ALL_COLOR[2] = 0x000F00;
//			WS2812B_ALL_COLOR[3] = 0x000F00;
//			WS2812_Refresh();
//		} 
//		else//�����
//		{
//			WS2812B_ALL_COLOR[0] = 0x0F0000;
//			WS2812B_ALL_COLOR[1] = 0x0F0000;
//			WS2812B_ALL_COLOR[2] = 0x0F0000;
//			WS2812B_ALL_COLOR[3] = 0x0F0000;
//			WS2812_Refresh();
//		}

		/* ���̼���뷢�� */
//		if(bDeviceState == CONFIGURED) //USB���ӳɹ�
//		{
//			Key_Press_detect();
//		}
		
		Delay_Ms(1000);
		SYSTEM_LED = !SYSTEM_LED;
	}
}

