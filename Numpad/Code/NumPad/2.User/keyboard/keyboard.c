#include "keyboard.h"
#include "hw_config.h"

extern __IO uint8_t PrevXferComplete;

u8 KEY_Status[ROW_COUNT * COLUMN_COUNT] = {0};//记录按键触发次数
u8 KEY_Press_Array[ROW_COUNT * COLUMN_COUNT] = {0};//记录按键是否按下
u8 KEY_Press = 0;
u8 KEY_Release = 0;

void _KEYS_Scan(u8 COL);


void _KEYS_DOWN_TIME(u8 Phy_Num, u8 Is_Press)
{
	if(Is_Press == 1)
	{
//			printf("%d " ,Phy_Num);
		if(KEY_Status[Phy_Num] < FLITER_TIME)
		{
			KEY_Status[Phy_Num]++;

			if(KEY_Status[Phy_Num] == FLITER_TIME && KEY_Press_Array[Phy_Num] == 0)
			{
				KEY_Press_Array[Phy_Num] = 1;
				KEY_Press = 1;
			}
		}
	}
	else
	{
		if(KEY_Status[Phy_Num] > 0)
		{
			KEY_Status[Phy_Num]--;

			if(KEY_Status[Phy_Num] == 0 && KEY_Press_Array[Phy_Num] == 1)
			{
				KEY_Press_Array[Phy_Num] = 0;
				KEY_Release = 1;
			}
		}
	}
}




void Keyboard_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_KEYRECT, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_InitStructure.GPIO_Pin = COLUMN1_PIN;
    GPIO_Init(COLUMN1_GPIO, &GPIO_InitStructure);
    COLUMN1 = 0;
    GPIO_InitStructure.GPIO_Pin = COLUMN2_PIN;
    GPIO_Init(COLUMN2_GPIO, &GPIO_InitStructure);
    COLUMN2 = 0;
    GPIO_InitStructure.GPIO_Pin = COLUMN3_PIN;
    GPIO_Init(COLUMN3_GPIO, &GPIO_InitStructure);
    COLUMN3 = 0;
    GPIO_InitStructure.GPIO_Pin = COLUMN4_PIN;
    GPIO_Init(COLUMN4_GPIO, &GPIO_InitStructure);
    COLUMN4 = 0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = ROW1_PIN;
    GPIO_Init(ROW1_GPIO, &GPIO_InitStructure);
    ROW1 = 0;
    GPIO_InitStructure.GPIO_Pin = ROW2_PIN;
    GPIO_Init(ROW2_GPIO, &GPIO_InitStructure);
    ROW2 = 0;
    GPIO_InitStructure.GPIO_Pin = ROW3_PIN;
    GPIO_Init(ROW3_GPIO, &GPIO_InitStructure);
    ROW3 = 0;
    GPIO_InitStructure.GPIO_Pin = ROW4_PIN;
    GPIO_Init(ROW4_GPIO, &GPIO_InitStructure);
    ROW4 = 0;
}

void KEYS_Scan()
{
	for(int i = 0; i < COLUMN_COUNT; i++)
	{
		_KEYS_Scan(i);
	}
}

void _KEYS_Scan(u8 COL)
{
	u8 tmp = 0;

	if(COL == 0)
	{
		COLUMN4 = 0;
		__delay(3);
		COLUMN1 = 1;
		__delay(3);
	}

	if(COL == 1)
	{
		COLUMN1 = 0;
		__delay(3);
		COLUMN2 = 1;
		__delay(3);
	}

	if(COL == 2)
	{
		COLUMN2 = 0;
		__delay(3);
		COLUMN3 = 1;
		__delay(3);
	}

	if(COL == 3)
	{
		COLUMN3 = 0;
		__delay(3);
		COLUMN4 = 1;
		__delay(3);
	}

	__delay(3);
	tmp = ROW1;
	__delay(3);
	_KEYS_DOWN_TIME(COL * 4 + 0, tmp);

	tmp = ROW2;
	__delay(3);
	_KEYS_DOWN_TIME(COL * 4 + 1, tmp);

	tmp = ROW3;
	__delay(3);
	_KEYS_DOWN_TIME(COL * 4 + 2, tmp);

	tmp = ROW4;
	__delay(3);
	_KEYS_DOWN_TIME(COL * 4 + 3, tmp);

}

//u8 KEYS_CreatePacket(u8* Txbuf)
//{
//	u8 p = 2;
////  *Txbuf = 0;
////  *(Txbuf+1) = 0;
////  *(Txbuf+2) = 0;
////  *(Txbuf+3) = 0;
////  *(Txbuf+4) = 0;
////  *(Txbuf+5) = 0;
////  *(Txbuf+6) = 0;
////  *(Txbuf+7) = 0;
////
////
//	Txbuf[0] = 0;
//	Txbuf[1] = 0;
//	Txbuf[2] = 0;
//	Txbuf[3] = 0;
//	Txbuf[4] = 0;
//	Txbuf[5] = 0;
//	Txbuf[6] = 0;
//	Txbuf[7] = 0;

//	if(KEY_Press == 0)
//		return 0;

//	for(int i = 0; i < ROW_COUNT; i++)
//	{
//		for(int j = 0; j < COLUMN_COUNT; j++)
//		{
//			//printf("  %02X  ", KEY_Status[i][j]);
////          if(KEY_Status[i][j] == 5){
////              Txbuf[p] = KEY_MAP[i][j];
////              p++;
////          }
//			//if(p==7) goto end;
//		}

//		printf("\r\n");
//	}

//	printf("----------------\r\n");
//end:
//	KEY_Press = 0;
//	return 1;
//}

//u8 KEYS_CreateNRFPacket(u8* Txbuf)
//{
//	//生成包之后，KEY_Press = 0
//	u16 Key_Phy_Num = 0;
//	u8 i = 2;

//	if(KEY_Press == 0)
//		return 0;

//	for(Key_Phy_Num = 0; Key_Phy_Num < ROW_COUNT * COLUMN_COUNT; Key_Phy_Num++)
//	{
//		//if(KEY_MAP[Key_Phy_Num] == Shift Ctrl...)
//		if(KEY_Status[Key_Phy_Num] != 0)
//		{
//			printf("<<  %d  >>  ", KEY_MAP[Key_Phy_Num]);
//			*(Txbuf + i) = KEY_MAP[Key_Phy_Num];//KEY_MAP[Key_Phy_Num]是待发送的USB按件编码，KEY_Status[Key_Phy_Num]的Key_Phy_Num是物理上的按键编号，由ROW Column算出。这份代码图省事，没有填真实的KE_YMAP表。
//			i++;
//		}

//		if(i == 8)
//		{
//			break;
//		}
//	}

//	while(i < 8)
//	{
//		*(Txbuf + i) = 0; //clear txbuf
//		i++;
//	}

//	printf("\r\n");

//	for(int j = 0; j < ROW_COUNT * COLUMN_COUNT; j++)
//	{
//		printf("  %d  ", KEY_Status[j]);
//	}

//	printf("\r\n");
//	printf("----------------\r\n");
//	KEY_Press = 0;
//	return 1;
//}

u8 KEY_Press_Array_Old[ROW_COUNT * COLUMN_COUNT] = {0};
u8 usb_send = 0;
//Buf[0]为ctrl等功能键，Buf[1]保留，2-7为键值
void Key_Press_detect(void)
{
	u8 key_press_num = 0;//一次KEYS_Scan按下的第n个按键,用于防止Tx_Pack溢出
	uint8_t Mouse_Buffer[keyboard_PACKET_SIZE] = {0};

	for(u8 i = 0; i < ROW_COUNT * COLUMN_COUNT; i++)//保存上次的按键数据
	{
		KEY_Press_Array_Old[i] = KEY_Press_Array[i];
	}
		
	KEYS_Scan();//获取新的按键状态数据
	
	if(KEY_Press == 1)
	{
		KEY_Press = 0;
		
		for(u8 i = 0; i < ROW_COUNT * COLUMN_COUNT; i++)
		{
			if(KEY_Press_Array_Old[i] != KEY_Press_Array[i])
			{			
				if(i == KEY_SLASH_PRESS)
				{
					printf("KEY_SLASH_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_ChuHao;//第一个按键按下啊对应data[2]
				}
				else if(i == KEY_7_PRESS)
				{
					printf("KEY_7_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_7_and_Home;
				}
				else if(i == KEY_4_PRESS)
				{
					printf("KEY_4_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_4_and_LeftArrow;
				}
				else if(i == KEY_1_PRESS)
				{
					printf("KEY_1_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_1_and_End;
				}
				else if(i == KEY_STAR_PRESS)
				{
					printf("KEY_STAR_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_ChengHao;
				}
				else if(i == KEY_8_PRESS)
				{
					printf("KEY_8_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_8_and_UpArrow;
				}
				else if(i == KEY_5_PRESS)
				{
					printf("KEY_5_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_5;
				}
				else if(i == KEY_0_PRESS)
				{
					printf("KEY_0_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_0_and_Insert;
				}
				else if(i == KEY_MINUS_PRESS)
				{
					printf("KEY_MINUS_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_JianHao;
				}
				else if(i == KEY_9_PRESS)
				{
					printf("KEY_9_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_9_and_PageUp;
				}
				else if(i == KEY_2_PRESS)
				{
					printf("KEY_2_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_2_and_DownArrow;
				}
				else if(i == KEY_DOT_PRESS)
				{
					printf("KEY_DOT_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_Dian_and_Delete;
				}
				else if(i == KEY_PLUS_PRESS)
				{
					printf("KEY_PLUS_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_JiaHao;
				}
				else if(i == KEY_6_PRESS)
				{
					printf("KEY_6_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_6_and_RightArrow;
				}
				else if(i == KEY_3_PRESS)
				{
					printf("KEY_3_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_3_and_PageDn;
				}
				else if(i == KEY_ENTER_PRESS)
				{
					printf("KEY_ENTER_PRESS\r\n");
					if(key_press_num++ <= (keyboard_PACKET_SIZE - 2))Mouse_Buffer[key_press_num + 1] = Keypad_ENTER;
				}
			}
		}

		usb_send = 1;
		if(0 != Mouse_Buffer[2])
		{
			while(!PrevXferComplete){};
			/* prepare buffer to send */
			/* Reset the control token to inform upper layer that a transfer is ongoing */
			PrevXferComplete = 0;
			/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
			USB_SIL_Write(EP1_IN, Mouse_Buffer, keyboard_PACKET_SIZE);
			/* Enable endpoint for transmission */
			SetEPTxValid(ENDP1);
		}
	}
	else if(usb_send == 1)//没有新的按键按下，且上次按下按键后还没有取消长按
	{
		u8 KEY_Remain = 0;
		for(u8 i = 0; i < ROW_COUNT * COLUMN_COUNT; i++)//所有旧的按键松开
		{
			if(KEY_Press_Array[i] == 1)
			{
				KEY_Remain = 1;
				break;
			}
		}
		if(KEY_Remain == 0)
		{
			usb_send = 0;
			
			for(u8 i = 0; i < keyboard_PACKET_SIZE; i++)Mouse_Buffer[i] = 0;
			
			while(!PrevXferComplete){};
			/* prepare buffer to send */
			/* Reset the control token to inform upper layer that a transfer is ongoing */
			PrevXferComplete = 0;
			/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
			USB_SIL_Write(EP1_IN, Mouse_Buffer, keyboard_PACKET_SIZE);
			/* Enable endpoint for transmission */
			SetEPTxValid(ENDP1);
		}
	}
}

