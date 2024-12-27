#ifndef __MENU_H
#define	__MENU_H
#include "stm32f10x.h"
#include "string.h"

#define 	WKUP				0				// 唤醒键
#define 	KEY1				1				// 按键1
#define 	KEY_UP				2				// 上
#define 	KEY_DOWN			3				// 下
#define 	KEY_LEFT			4				// 左
#define 	KEY_RIGHT			5				// 右
#define 	KEY_A				6				// A
#define 	KEY_AA				7				// A+
#define 	KEY_Rocker1			8				// 摇杆1的按键
#define 	KEY_Rocker2			9				// 摇杆2的按键

#define		control 0

/*字体和图像*/
extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];
extern char Hzk[][32];
extern const unsigned char ascii_1206;
extern const unsigned char ascii_1608;
extern const unsigned char ascii_2412;
extern const unsigned char ascii_3216;
extern const struct typFNT_GB12 tfont12;
extern const struct typFNT_GB16 tfont16;
extern const struct typFNT_GB24 tfont24;
extern const struct typFNT_GB32 tfont32;
extern const unsigned char gImage_ouc[];

/*按键标志位*/
extern u8 F_KEY_Down[12];
extern u8 F_2G4_Send;

/*摇杆*/
extern u16 AV_ADC_Channel1_Sample;
extern u16 AV_ADC_Channel3_Sample;

typedef struct
{
	u8 current;//当前索引号
	u8 Num;//在当前菜单的序号！=索引号
	u8 up;//向上翻索引号
	u8 down;//向下翻索引号
	u8 enter;//确认索引号
  u8 back;//返回索引号
	char* name;//显示的名字
} key_table;

void show_hello_page(void);
void show_page(u8 num,u8 num_last);
void test_show(void);
void final_menu(void);
#endif
