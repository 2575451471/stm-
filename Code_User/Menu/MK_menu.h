#ifndef __MENU_H
#define	__MENU_H
#include "stm32f10x.h"
#include "string.h"

#define 	WKUP				0				// ���Ѽ�
#define 	KEY1				1				// ����1
#define 	KEY_UP				2				// ��
#define 	KEY_DOWN			3				// ��
#define 	KEY_LEFT			4				// ��
#define 	KEY_RIGHT			5				// ��
#define 	KEY_A				6				// A
#define 	KEY_AA				7				// A+
#define 	KEY_Rocker1			8				// ҡ��1�İ���
#define 	KEY_Rocker2			9				// ҡ��2�İ���

#define		control 0

/*�����ͼ��*/
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

/*������־λ*/
extern u8 F_KEY_Down[12];
extern u8 F_2G4_Send;

/*ҡ��*/
extern u16 AV_ADC_Channel1_Sample;
extern u16 AV_ADC_Channel3_Sample;

typedef struct
{
	u8 current;//��ǰ������
	u8 Num;//�ڵ�ǰ�˵�����ţ�=������
	u8 up;//���Ϸ�������
	u8 down;//���·�������
	u8 enter;//ȷ��������
  u8 back;//����������
	char* name;//��ʾ������
} key_table;

void show_hello_page(void);
void show_page(u8 num,u8 num_last);
void test_show(void);
void final_menu(void);
#endif
