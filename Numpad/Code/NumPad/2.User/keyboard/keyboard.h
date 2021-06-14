#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "stm32f10x.h"

/* Keyboard������ ------------------------------------------------------------*/
#define ROW_COUNT				4
#define COLUMN_COUNT    4

#define ROW1_GPIO				GPIOB
#define ROW1_PIN				GPIO_Pin_9
#define ROW1						PBin(9)

#define ROW2_GPIO				GPIOB
#define ROW2_PIN				GPIO_Pin_8
#define ROW2						PBin(8)

#define ROW3_GPIO				GPIOB
#define ROW3_PIN				GPIO_Pin_7
#define ROW3						PBin(7)

#define ROW4_GPIO				GPIOB
#define ROW4_PIN 				GPIO_Pin_6
#define ROW4						PBin(6)

#define COLUMN1_GPIO		GPIOB
#define COLUMN1_PIN			GPIO_Pin_12
#define COLUMN1					PBout(12)

#define COLUMN2_GPIO		GPIOB
#define COLUMN2_PIN			GPIO_Pin_13
#define COLUMN2					PBout(13)

#define COLUMN3_GPIO		GPIOB
#define COLUMN3_PIN			GPIO_Pin_14
#define COLUMN3					PBout(14)

#define COLUMN4_GPIO		GPIOB
#define COLUMN4_PIN			GPIO_Pin_15
#define COLUMN4					PBout(15)

#define RCC_KEYRECT			RCC_APB2Periph_GPIOB

/* ���������ӦӲ��ӳ��-------------------------------------------------------*/
#define KEY_SLASH_PRESS 0 // '/'
#define KEY_7_PRESS	1
#define KEY_4_PRESS	2
#define KEY_1_PRESS	3
#define KEY_STAR_PRESS	4 // '*'
#define KEY_8_PRESS	5
#define KEY_5_PRESS	6
#define KEY_0_PRESS	7
#define KEY_MINUS_PRESS	8 // '-'
#define KEY_9_PRESS	9
#define KEY_2_PRESS	10
#define KEY_DOT_PRESS	11 // '.'
#define KEY_PLUS_PRESS	12 // '+'
#define KEY_6_PRESS	13
#define KEY_3_PRESS	14
#define KEY_ENTER_PRESS	15 // 'Enter'

/* �豸����������-------------------------------------------------------------*/
#define Keyboard_a		4	// Keyboard a and A
#define Keyboard_b		5	// Keyboard b and B
#define Keyboard_c		6	// Keyboard c and C
#define Keyboard_d		7	// Keyboard d and D
#define Keyboard_e		8	// Keyboard e and E
#define Keyboard_f		9	// Keyboard f and F
#define Keyboard_g		10	// Keyboard g and G
#define Keyboard_h		11	// Keyboard h and H
#define Keyboard_i		12	// Keyboard i and I
#define Keyboard_j		13	// Keyboard j and J
#define Keyboard_k		14	// Keyboard k and K
#define Keyboard_l		15	// Keyboard l and L
#define Keyboard_m		16	// Keyboard m and M
#define Keyboard_n		17	// Keyboard n and N
#define Keyboard_o		18	// Keyboard o and O
#define Keyboard_p		19	// Keyboard p and P
#define Keyboard_q		20	// Keyboard q and Q
#define Keyboard_r		21	// Keyboard r and R
#define Keyboard_s		22	// Keyboard s and S
#define Keyboard_t		23	// Keyboard t and T
#define Keyboard_u		24	// Keyboard u and U
#define Keyboard_v		25	// Keyboard v and V
#define Keyboard_w		26	// Keyboard w and W
#define Keyboard_x		27	// Keyboard x and X
#define Keyboard_y		28	// Keyboard y and Y
#define Keyboard_z		29	// Keyboard z and Z
#define Keyboard_1		30	// Keyboard 1 and !
#define Keyboard_2		31	// Keyboard 2 and @
#define Keyboard_3		32	// Keyboard 3 and #
#define Keyboard_4		33	// Keyboard 4 and $
#define Keyboard_5		34	// Keyboard 5 and %
#define Keyboard_6		35	// Keyboard 6 and ^
#define Keyboard_7		36	// Keyboard 7 and &
#define Keyboard_8		37	// Keyboard 8 and *
#define Keyboard_9		38	// Keyboard 9 and (
#define Keyboard_0		39	// Keyboard 0 and )
#define Keyboard_ENTER		40	// Keyboard ENTER
#define Keyboard_ESCAPE		41	// Keyboard ESCAPE
#define Keyboard_Backspace		42	// Keyboard Backspace
#define Keyboard_Tab		43	// Keyboard Tab
#define Keyboard_KongGe		44	// Keyboard Spacebar
#define Keyboard_JianHao		45	// Keyboard - and _(underscore)
#define Keyboard_DengHao		46	// Keyboard = and +
#define Keyboard_ZuoZhongKuoHao		47	// Keyboard [ and {
#define Keyboard_YouZhongKuoHao		48	// Keyboard ] and }
#define Keyboard_FanXieGang		49	// Keyboard \ and |
#define Keyboard_FenHao		51	// Keyboard ; and :
#define Keyboard_DanYinHao		52	// Keyboard �� and ��
#define Keyboard_BoLangXian		53	// Keyboard `(Grave Accent) and ~(Tilde)
#define Keyboard_Douhao		54	// Keyboard, and <
#define Keyboard_JuHao		55	// Keyboard . and >
#define Keyboard_XieGang_WenHao		56	// Keyboard / and ?
#define Keyboard_CapsLock		57	// Keyboard Caps Lock
#define Keyboard_F1		58 //	Keyboard F1
#define Keyboard_F2		59 //	Keyboard F2
#define Keyboard_F3		60 //	Keyboard F3
#define Keyboard_F4		61 //	Keyboard F4
#define Keyboard_F5		62 //	Keyboard F5
#define Keyboard_F6		63 //	Keyboard F6
#define Keyboard_F7		64 //	Keyboard F7
#define Keyboard_F8		65 //	Keyboard F8
#define Keyboard_F9		66 //	Keyboard F9
#define Keyboard_F10		67	// Keyboard F10
#define Keyboard_F11		68 	// Keyboard F11
#define Keyboard_F12		69 	// Keyboard F12
#define Keyboard_PrintScreen		70	// Keyboard PrintScreen
#define Keyboard_ScrollLock		71	// Keyboard Scroll Lock
#define Keyboard_Pause		72	// Keyboard Pause
#define Keyboard_Insert		73	// Keyboard Insert
#define Keyboard_Home		74	// Keyboard Home
#define Keyboard_PageUp		75	// Keyboard PageUp
#define Keyboard_Delete		76	// Keyboard Delete
#define Keyboard_End		77	// Keyboard End
#define Keyboard_PageDown		78	// Keyboard PageDown
#define Keyboard_RightArrow		79	// Keyboard RightArrow
#define Keyboard_LeftArrow		80	// Keyboard LeftArrow
#define Keyboard_DownArrow		81	// Keyboard DownArrow
#define Keyboard_UpArrow		82	// Keyboard UpArrow
#define Keypad_NumLock		83	// Keypad Num Lock and Clear
#define Keypad_ChuHao		84	// Keypad /
#define Keypad_ChengHao		85	// Keypad *
#define Keypad_JianHao		86	// Keypad -
#define Keypad_JiaHao		87	// Keypad +
#define Keypad_ENTER		88	// Keypad ENTER
#define Keypad_1_and_End		89	// Keypad 1 and End
#define Keypad_2_and_DownArrow		90	// Keypad 2 and Down Arrow
#define Keypad_3_and_PageDn		91	// Keypad 3 and PageDn
#define Keypad_4_and_LeftArrow		92	// Keypad 4 and Left Arrow
#define Keypad_5		93	// Keypad 5
#define Keypad_6_and_RightArrow		94	// Keypad 6 and Right Arrow
#define Keypad_7_and_Home		95	// Keypad 7 and Home
#define Keypad_8_and_UpArrow		96	// Keypad 8 and Up Arrow
#define Keypad_9_and_PageUp		97	// Keypad 9 and PageUp
#define Keypad_0_and_Insert		98	// Keypad 0 and Insert
#define Keypad_Dian_and_Delete		99	// Keypad . and Delete
#define Keyboard_Application		101	// Keyboard Application
#define Keyboard_LeftControl		224
#define Keyboard_LeftShift		225
#define Keyboard_LeftAlt		226
#define Keyboard_LeftWindows		227
#define Keyboard_RightControl		228
#define Keyboard_RightShift		229
#define Keyboard_RightAlt		230
#define Keyboard_RightWindows		231

#define __delay(x) for(int i = x;i>0;i--){}
#define FLITER_TIME 4

void Keyboard_Init(void);
void Key_Press_detect(void);


#endif
