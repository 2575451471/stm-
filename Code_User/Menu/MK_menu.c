#include "MK_menu.h"
#include "oled.h"
#include "adc.h"
#include "bsp_si24r1.h"
#include "led.h"
#include "stdlib.h"
#include "math.h"

/*���ұ���ò�Ҫ�Ҹ��Ҽ��Ҽ��*/
key_table table[62] =
	{
		// ��0��
		{0, 1, 0, 0, 1, 0, "hello"}, // hello

		// ��1��
		{1, 1, 5, 2, 6, 0, "pid editor"},	   // pid����
		{2, 2, 1, 3, 8, 0, "oscillogram"},	   // ����ͼ
		{3, 3, 2, 4, 11, 0, "velocity plan"},  // �ٶȲ���
		{4, 4, 3, 5, 14, 0, "camera setting"}, // ����ͷ����
		{5, 5, 4, 1, 17, 0, "In my control"},  // ң��

		// ��2��
		{6, 1, 7, 7, 18, 1, "velocity"},  // �ٶȻ�
		{7, 2, 6, 6, 21, 1, "direction"}, // ����

		{8, 1, 10, 9, 24, 2, "velocity"}, // �ٶȲ���
		{9, 2, 8, 10, 26, 2, "demand"},	  // ��������
		{10, 3, 9, 8, 28, 2, "angle"},	  // ��̬�ǲ���

		{11, 1, 13, 12, 31, 3, "speed"},	 // �����ٶ�
		{12, 2, 11, 13, 32, 3, "k up&down"}, // �ٶ�ϵ��
		{13, 3, 12, 11, 34, 3, "area"},		 // ��������

		{14, 1, 16, 15, 36, 4, "bias area"},	   // ǰհ����
		{15, 2, 14, 16, 38, 4, "expo_t "},		   // �ع�ʱ��
		{16, 3, 15, 14, 39, 4, "inflection area"}, // �յ㷶Χ

		{17, 1, 17, 17, 17, 5, "show"}, // ң�ؽ���

		// ��3��
		{18, 1, 20, 19, 41, 6, "kp"}, // kp
		{19, 2, 18, 20, 42, 6, "ki"}, // ki
		{20, 3, 19, 18, 43, 6, "kd"}, // kd

		{21, 1, 23, 22, 44, 7, "kp"},	// kp
		{22, 2, 21, 23, 45, 7, "kp_2"}, // kp_2
		{23, 3, 22, 21, 46, 7, "kd"},	// kd

		{24, 1, 25, 25, 47, 8, "L"}, // L
		{25, 2, 24, 24, 48, 8, "R"}, // R

		{26, 1, 27, 27, 49, 9, "speed"}, // �����ٶ�
		{27, 2, 26, 26, 50, 9, "delta"}, // ��������

		{28, 1, 27, 29, 51, 10, "yaw"},	  // yaw
		{29, 2, 28, 30, 52, 10, "pitch"}, // pitch
		{30, 3, 29, 28, 53, 10, "roll"},  // roll

		{31, 1, 31, 31, 32, 11, "show"}, // �����ٶȽ���

		{32, 1, 33, 33, 54, 12, "k_up"},   // k_up
		{33, 2, 32, 32, 55, 12, "k_down"}, // k_down

		{34, 1, 35, 35, 56, 13, "up"},	 // area_up
		{35, 2, 34, 34, 57, 13, "down"}, // area_down

		{36, 1, 37, 37, 58, 14, "up"},	 // �ϱ���
		{37, 2, 36, 36, 59, 14, "down"}, // �±���

		{38, 1, 38, 38, 39, 15, "show"}, // �ع�ʱ�����

		{39, 1, 40, 40, 60, 16, "up"},	 // �Ͻ���
		{40, 2, 39, 39, 61, 16, "down"}, // �½���

		// ��4��
		{41, 1, 41, 41, 41, 18, "show"}, // kp����
		{42, 2, 42, 42, 42, 19, "show"}, // ki����
		{43, 3, 43, 43, 43, 20, "show"}, // kd����

		{44, 1, 44, 44, 44, 21, "show"}, // kp����
		{45, 2, 45, 45, 45, 22, "show"}, // kp_2����
		{46, 3, 46, 46, 46, 23, "show"}, // kd����

		{47, 1, 47, 47, 47, 24, "show"}, // L����
		{48, 2, 48, 48, 48, 25, "show"}, // R����

		{49, 1, 49, 49, 49, 26, "show"}, // �����ٶȽ���
		{50, 2, 50, 50, 50, 27, "show"}, // �������ٽ���

		{51, 1, 51, 51, 51, 28, "show"}, // yaw����
		{52, 2, 52, 52, 52, 29, "show"}, // pitch����
		{53, 3, 53, 53, 53, 30, "show"}, // roll����

		{54, 1, 54, 54, 54, 32, "show"}, // k_up����
		{55, 2, 55, 55, 55, 33, "show"}, // k_down����

		{56, 1, 56, 56, 56, 34, "show"}, // area_up����
		{57, 2, 57, 57, 57, 35, "show"}, // area_down����

		{58, 1, 58, 58, 58, 36, "show"}, // ǰհ�ϱ��ؽ���
		{59, 2, 59, 59, 59, 37, "show"}, // ǰհ�±��ؽ���

		{60, 1, 60, 60, 60, 39, "show"}, // �յ��ϱ��ؽ���
		{61, 2, 61, 61, 61, 40, "show"}, // �յ��±��ؽ���

};
/******************************************************************************
	  ����˵������ʼչʾ����
	  ������ݣ���
	  ����ֵ��  ��
******************************************************************************/
void show_hello_page()
{
	static u8 show_flag = 0;
	if (!show_flag)
	{
		// LCD_Fill(0,0,240,280,WHITE);
		LCD_ShowPicture(40, 30, 160, 214, gImage_ouc);
		LCD_ShowString(18, 30 + 214, (u8 *)"created by shenle", RED, WHITE, 24, 0);
		//LCD_ShowString(15, 40 + 133 + 16 + 20, (u8 *)"creater:shenle", RED, WHITE, 24, 0);
		show_flag = 1;
	}
}

/******************************************************************************
	  ����˵��������չʾ����
	  ������ݣ��������ݵ���Դֵ
	  ����ֵ��  ��
******************************************************************************/
void osc_page(u8 num)
{
	u16 i = 0;
	static double n = 0, high = 1;
	static u16 data[241] = {0}, old_data[241] = {0};
	static u8 cnt_x = 10, cnt_y = 10, flag = 0, now_num, wide = 1;

	// ��������
	data_transfer(num, 0, 0, 0, 0);

	/*���ο��*/
	// ��ADCͨ��1��ͨ��3�Ĳ���ֵ����3500ʱ������cnt_x
	if (AV_ADC_Channel1_Sample > 3500 || AV_ADC_Channel3_Sample > 3500)
		cnt_x++;
	// ��ADCͨ��1��ͨ��3�Ĳ���ֵС��700ʱ������cnt_x
	else if (AV_ADC_Channel1_Sample < 700 || AV_ADC_Channel3_Sample < 700)
		cnt_x--;
	else
		cnt_x = 10;

	// ��cnt_x���ڵ���20ʱ������cnt_x��wide
	if (cnt_x >= 20)
	{
		cnt_x = 15;
		if (wide < 4)
			wide += 1;
	}
	// ��cnt_x����0ʱ������cnt_x��wide
	else if (cnt_x == 0)
	{
		cnt_x = 5;
		if (wide > 1)
			wide -= 1;
	}

	/*���θ߶�*/
	// ��ADCͨ��2��ͨ��6�Ĳ���ֵ����3500ʱ������cnt_y
	if (AV_ADC_Channel2_Sample > 3500 || AV_ADC_Channel6_Sample > 3500)
		cnt_y++;
	// ��ADCͨ��2��ͨ��6�Ĳ���ֵС��700ʱ������cnt_y
	else if (AV_ADC_Channel2_Sample < 700 || AV_ADC_Channel6_Sample < 700)
		cnt_y--;
	else
		cnt_y = 10;
	// ��cnt_y���ڵ���20ʱ������cnt_y��high
	if (cnt_y >= 20)
	{
		cnt_y = 15;
		LCD_DrawLine(0, 160 - high * 2 * 40, 240, 160 - high * 2 * 40, WHITE);
		if (high < 2)
			high += 0.1;
	}
	// ��cnt_y����0ʱ������cnt_y��high
	else if (cnt_y == 0)
	{
		cnt_y = 5;
		LCD_DrawLine(0, 160 - high * 2 * 40, 240, 160 - high * 2 * 40, WHITE);
		if (high > 0.4)
			high -= 0.1;
	}

	/*��һ�ν��룬�����������0*/
	for (i = 0; i <= 240 && (!flag || now_num != num); i++, n++)
	{
		data[i] = 0; // 40*(sin(n/12.9)+1)*high
		old_data[i] = data[i];
		high = 1;
		wide = 1;
	}
	flag = 1;

	/*�Ѿ��������ݣ�ÿһλ��ǰ�ƶ�*/
	for (i = 0; i < 240 && flag; i++)
	{
		old_data[i] = data[i];
		if (i % wide == 0)
			data[i] = data[i + wide];
		else
			data[i] = 0;
	}

	/*�����һλ�����µ�����*/
	if (n >= 240)
		n = 0;
	n++;

	data_transfer(num, 0, 0, 0, 0);
	if (SI24R1_Receive_Data[0] == 13 && SI24R1_Receive_Data[1] == 16)
		data[240] = (SI24R1_Receive_Data[6] * 100 + SI24R1_Receive_Data[7]) * high;
	if (data[240] > 160)
		data[240] = 160;
	now_num = num;

	/****************************************/
	/*������ʾ*/
	LCD_ShowString(10, 170, (u8 *)"High:", LIGHTGREEN, WHITE, 32, 0);
	LCD_ShowFloatNum1(10 + 5 * 16, 170, high, 3, LIGHTGREEN, WHITE, 32);
	LCD_ShowString(10, 170 + 32, (u8 *)"Wide:", LIGHTGREEN, WHITE, 32, 0);
	LCD_ShowFloatNum1(10 + 5 * 16, 170 + 32, wide, 3, LIGHTGREEN, WHITE, 32);

	LCD_DrawLine(0, 160, 240, 160, BLACK);
	// LCD_DrawLine(0,160-high*80,240,160-high*80,YELLOW);
	for (i = 0; i < 240; i++)
	{
		LCD_DrawPoint(i, 160 - old_data[i], WHITE);
		LCD_DrawPoint(i, 160 - data[i], MAGENTA);
	}
}

/******************************************************************************
	  ����˵��������չʾ����
	  ������ݣ����Ĳ��������
	  ����ֵ��  ��
******************************************************************************/
void data_page(u8 num)
{
	static double x = 1, real = 0;
	static u16 cnt = 10;
	static u8 n_flag = 0, last_num = 0;
	static s8 show_x = 0, show_last_x = 0;
	static u8 channel = 0; // 0 1 2 3 ��Ӧ�ı�10 1 0.1 0.01

	LCD_ShowString(40, 0, (u8 *)table[table[num].back].name, BLACK, WHITE, 24, 0);
	LCD_ShowFloatNum1(40 + 12 * 6, 0, x, 5, LIGHTBLUE, WHITE, 24);
	if (x == real)
		LCD_ShowString(78, 26, (u8 *)" Saved ", YELLOW, WHITE, 24, 0);
	else
		LCD_ShowString(78, 26, (u8 *)"Unsaved", RED, WHITE, 24, 0);

	if (last_num != num) // ���ν��������ѡ�����
	{
		data_transfer(num, 0, 0, 0, 0);
		real = SI24R1_Receive_Data[4] + SI24R1_Receive_Data[5] * 0.01; // ��ȡ��ǰ����
		x = real;
	}
	last_num = num;

	/*������*/
	if (AV_ADC_Channel3_Sample > 3300 && cnt < 20)
		cnt++;
	else if (AV_ADC_Channel3_Sample < 900 && cnt > 0)
		cnt--;
	else if (AV_ADC_Channel3_Sample >= 900 && AV_ADC_Channel3_Sample <= 3300)
		cnt = 10;

	if (cnt == 20 && AV_ADC_Channel3_Sample > 3300)
	{
		cnt = 15;
		if (channel == 0 && x < 990)
			x += 10;
		else if (channel == 1 && x < 999)
			x += 1;
		else if (channel == 2 && x < 999.9)
			x += 0.1;
		else if (channel == 3 && x < 999.99)
			x += 0.01;
	}
	else if (cnt == 0 && AV_ADC_Channel3_Sample < 900)
	{
		cnt = 5;
		if (channel == 0 && x > 10)
			x -= 10;
		else if (channel == 1 && x > 1)
			x -= 1;
		else if (channel == 2 && x > 0.1)
			x -= 0.1;
		else if (channel == 3 && x > 0.01)
			x -= 0.01;
	}

	LCD_DrawLine(40, 100, 200, 100, BLACK);
	LCD_DrawLine(40, 101, 201, 100, BLACK);

	show_x = 80 * (AV_ADC_Channel3_Sample - 2100) / (3900 - 2100);
	LCD_DrawPoint(120 + show_x, 97, RED);
	LCD_DrawPoint(120 + show_x, 98, RED);
	LCD_DrawPoint(120 + show_x, 99, RED);
	LCD_DrawPoint(120 + show_x, 102, RED);
	LCD_DrawPoint(120 + show_x, 103, RED);
	LCD_DrawPoint(120 + show_x, 104, RED);
	if (show_last_x != show_x)
	{
		LCD_DrawPoint(120 + show_last_x, 97, WHITE);
		LCD_DrawPoint(120 + show_last_x, 98, WHITE);
		LCD_DrawPoint(120 + show_last_x, 99, WHITE);
		LCD_DrawPoint(120 + show_last_x, 102, WHITE);
		LCD_DrawPoint(120 + show_last_x, 103, WHITE);
		LCD_DrawPoint(120 + show_last_x, 104, WHITE);
	}
	show_last_x = show_x;

	/*�ı�λ��*/
	if (AV_ADC_Channel1_Sample > 3300 && !n_flag) // ���ֿ���λ��
	{
		if (channel < 3)
			channel += 1;
		else
			channel = 0;
		n_flag = 1;
	}
	else if (AV_ADC_Channel1_Sample < 900 && !n_flag)
	{
		if (channel > 0)
			channel -= 1;
		else
			channel = 3;
		n_flag = 1;
	}
	else if (AV_ADC_Channel1_Sample >= 900 && AV_ADC_Channel1_Sample <= 3300 && n_flag)
		n_flag = 0;

	switch (channel)
	{
	case 0:
		LCD_ShowChar(22, 200, 'A', WHITE, BLACK, 32, 0);			  // ����A
		LCD_ShowChar(22 * 2 + 32, 200, 'B', BLACK, WHITE, 32, 0);	  // B
		LCD_ShowChar(100, 200, '.', BLACK, WHITE, 32, 0);			  //.
		LCD_ShowChar(22 * 3 + 32 * 2, 200, 'C', BLACK, WHITE, 32, 0); // C
		LCD_ShowChar(22 * 4 + 32 * 3, 200, 'D', BLACK, WHITE, 32, 0); // D
		break;

	case 1:
		LCD_ShowChar(22, 200, 'A', BLACK, WHITE, 32, 0);			  // A
		LCD_ShowChar(22 * 2 + 32, 200, 'B', WHITE, BLACK, 32, 0);	  // ����B
		LCD_ShowChar(100, 200, '.', BLACK, WHITE, 32, 0);			  //.
		LCD_ShowChar(22 * 3 + 32 * 2, 200, 'C', BLACK, WHITE, 32, 0); // C
		LCD_ShowChar(22 * 4 + 32 * 3, 200, 'D', BLACK, WHITE, 32, 0); // D
		break;

	case 2:
		LCD_ShowChar(22, 200, 'A', BLACK, WHITE, 32, 0);			  // A
		LCD_ShowChar(22 * 2 + 32, 200, 'B', BLACK, WHITE, 32, 0);	  // B
		LCD_ShowChar(100, 200, '.', BLACK, WHITE, 32, 0);			  //.
		LCD_ShowChar(22 * 3 + 32 * 2, 200, 'C', WHITE, BLACK, 32, 0); // ����C
		LCD_ShowChar(22 * 4 + 32 * 3, 200, 'D', BLACK, WHITE, 32, 0); // D
		break;

	case 3:
		LCD_ShowChar(22, 200, 'A', BLACK, WHITE, 32, 0);			  // A
		LCD_ShowChar(22 * 2 + 32, 200, 'B', BLACK, WHITE, 32, 0);	  // B
		LCD_ShowChar(100, 200, '.', BLACK, WHITE, 32, 0);			  //.
		LCD_ShowChar(22 * 3 + 32 * 2, 200, 'C', BLACK, WHITE, 32, 0); // C
		LCD_ShowChar(22 * 4 + 32 * 3, 200, 'D', WHITE, BLACK, 32, 0); // ����D
		break;
	}

	/*���漰�ص�*/
	if (F_KEY_Down[KEY1])
	{
		real = x; // ����
		data_transfer(num, 1, real, 0, 0);
	}
	else if (F_KEY_Down[KEY_UP] || F_KEY_Down[KEY_DOWN])
		x = real; // �ص�
}
/******************************************************************************
	  ����˵������һ���ϴ�ĵ㣨�ڲ����ã�
	  ������ݣ��е�x,y����һ����x,y����ɫ
	  ����ֵ��  ��
******************************************************************************/
void LCD_drawbigpoint(u16 x, u16 y, u16 last_x, u16 last_y, u16 color)
{
	s16 dx = 0, dy = 0;

	dx = x - last_x;
	dy = y - last_y;

	LCD_DrawPoint(x, y + 1, color);
	LCD_DrawPoint(x, y - 1, color);
	LCD_DrawPoint(x - 1, y, color);
	LCD_DrawPoint(x + 1, y, color);

	if (abs(last_x - x) == 1 && abs(last_y - y) == 1) // б���ƶ����������ص�
	{
		if (dx < 0 && dy < 0)
		{
			LCD_DrawPoint(last_x, last_y + 1, WHITE);
			LCD_DrawPoint(last_x + 1, last_y, WHITE);
		}
		else if (dx < 0)
		{
			LCD_DrawPoint(last_x, last_y - 1, WHITE);
			LCD_DrawPoint(last_x + 1, last_y, WHITE);
		}
		else if (dy < 0)
		{
			LCD_DrawPoint(last_x, last_y + 1, WHITE);
			LCD_DrawPoint(last_x - 1, last_y, WHITE);
		}
		else
		{
			LCD_DrawPoint(last_x, last_y - 1, WHITE);
			LCD_DrawPoint(last_x - 1, last_y, WHITE);
		}
	}
	else if (abs(dx) == 2 && dy == 0) // ˮƽ�ƶ���һ���ص�
	{
		if (dx > 0)
		{
			LCD_DrawPoint(last_x, last_y + 1, WHITE);
			LCD_DrawPoint(last_x, last_y - 1, WHITE);
			LCD_DrawPoint(last_x - 1, last_y, WHITE);
		}
		else
		{
			LCD_DrawPoint(last_x, last_y + 1, WHITE);
			LCD_DrawPoint(last_x, last_y - 1, WHITE);
			LCD_DrawPoint(last_x + 1, last_y, WHITE);
		}
	}
	else if (abs(dy) == 2 && dx == 0) // ��ֱ�ƶ���һ���ص�
	{
		if (dy > 0)
		{
			LCD_DrawPoint(last_x + 1, last_y, WHITE);
			LCD_DrawPoint(last_x - 1, last_y, WHITE);
			LCD_DrawPoint(last_x, last_y - 1, WHITE);
		}
		else
		{
			LCD_DrawPoint(last_x + 1, last_y, WHITE);
			LCD_DrawPoint(last_x - 1, last_y, WHITE);
			LCD_DrawPoint(last_x, last_y + 1, WHITE);
		}
	}
	else if (dx != 0 || dy != 0) // ���ص��ƶ�
	{
		LCD_DrawPoint(last_x, last_y + 1, WHITE);
		LCD_DrawPoint(last_x, last_y - 1, WHITE);
		LCD_DrawPoint(last_x - 1, last_y, WHITE);
		LCD_DrawPoint(last_x + 1, last_y, WHITE);
	}
}

/******************************************************************************
	  ����˵����ң��չʾ����
	  ������ݣ���
	  ����ֵ��  ��
******************************************************************************/
void control_page(void)
{
	static u16 last_lx = 0, last_ly = 0, last_rx = 0, last_ry = 0;
	u16 L_x = 0, L_y = 0, R_x = 0, R_y = 0;
	u16 speed = 0;
	u16 dir = 0;

	/*�޷���������ֵ*/
	if (AV_ADC_Channel1_Sample >= 3900)
		AV_ADC_Channel1_Sample = 3899;
	else if (AV_ADC_Channel1_Sample <= 300)
		AV_ADC_Channel1_Sample = 301;
	if (AV_ADC_Channel2_Sample >= 3900)
		AV_ADC_Channel2_Sample = 3899;
	else if (AV_ADC_Channel2_Sample <= 300)
		AV_ADC_Channel2_Sample = 301;
	if (AV_ADC_Channel3_Sample >= 3900)
		AV_ADC_Channel3_Sample = 3899;
	else if (AV_ADC_Channel3_Sample <= 300)
		AV_ADC_Channel3_Sample = 301;
	if (AV_ADC_Channel6_Sample >= 3900)
		AV_ADC_Channel6_Sample = 3899;
	else if (AV_ADC_Channel6_Sample <= 300)
		AV_ADC_Channel6_Sample = 301;

	/*���źͶ������*/
	if (!F_KEY_Down[KEY_AA]) // �����Ҷ�
	{
		speed = 150 * (AV_ADC_Channel2_Sample - 300) / (3900 - 300);
		dir = 11 * (AV_ADC_Channel3_Sample - 300) / (3900 - 300);
	}
	else
	{
		speed = 150 * (AV_ADC_Channel6_Sample - 300) / (3900 - 300);
		dir = 11 * (AV_ADC_Channel1_Sample - 300) / (3900 - 300);
	}
	LCD_ShowString(10, 32 + 32 * 0, (u8 *)"Speed:", LIGHTBLUE, WHITE, 32, 0);
	LCD_ShowIntNum(10 + 6 * 16, 32 + 32 * 0, speed, 3, LIGHTGREEN, WHITE, 32);
	LCD_ShowString(10, 32 + 32 * 1, (u8 *)"DIR:", LIGHTBLUE, WHITE, 32, 0);
	LCD_ShowIntNum(10 + 4 * 16, 32 + 32 * 1, dir, 3, LIGHTGREEN, WHITE, 32);
	/*ҡ��ֱ����ʾ*/
	Draw_Circle(50, 230, 50, BLUE);
	Draw_Circle(190, 230, 50, BLUE);

	L_x = 50 + 50 * (AV_ADC_Channel1_Sample - 2100) / (3900 - 2100);
	L_y = 230 - sqrt(50 * 50 - (L_x - 50) * (L_x - 50)) * (AV_ADC_Channel2_Sample - 2100) / (3900 - 2100);
	R_x = 190 + 50 * (AV_ADC_Channel3_Sample - 2100) / (3900 - 2100);
	R_y = 230 - sqrt(50 * 50 - (R_x - 190) * (R_x - 190)) * (AV_ADC_Channel6_Sample - 2100) / (3900 - 2100);

	/*�޷��������ֵ*/
	if (L_x < 2)
		L_x = 2;
	else if (L_x > 98)
		L_x = 98;

	if (L_y < 182)
		L_y = 182;
	else if (L_y > 278)
		L_y = 278;

	if (R_x < 142)
		R_x = 142;
	else if (R_x > 238)
		R_x = 238;

	if (R_y < 182)
		R_y = 182;
	else if (R_y > 278)
		R_y = 278;

	LCD_drawbigpoint(L_x, L_y, last_lx, last_ly, RED);
	LCD_drawbigpoint(R_x, R_y, last_rx, last_ry, RED);

	last_lx = L_x;
	last_ly = L_y;
	last_rx = R_x;
	last_ry = R_y;

	if (F_2G4_Send == 1)
	{
		F_2G4_Send = 0;
		data_transfer(17, 1, 0, speed, dir);
	}
}

/******************************************************************************
	  ����˵�����˵�չʾ����
	  ������ݣ���
	  ����ֵ��  ��
******************************************************************************/
void show_page(u8 num, u8 last_num)
{
	static u8 show_flag = 0;
	u8 i, m = 0;
	u8 line = 0;
	// ���㵱ǰҳ������������
	line = num - table[table[num].back].enter;

	// ���������Ϊ���ҵ�ǰҳ��δ��ʾ������ʾ��������
	if (table[table[num].back].current != 0 && !show_flag)
		LCD_ShowString(8, 0, (u8 *)table[table[num].back].name, BLACK, WHITE, 32, 0); // ˵����������

	// �����ǰҳ�治����Ϊ"show"��ҳ�棨˵���ǲ˵����棩������ʾָ����ѡ����ļ�ͷ
	if (strcmp(table[num].name, "show") != 0)
		LCD_ShowString(12, 32 + 24 * line, (u8 *)"=>", RED, WHITE, 24, 0); // ָ����ѡ����

	i = table[table[table[num].back].enter].current;
	while (!show_flag && strcmp(table[num].name, "show") != 0)
	{
		// ��ʾ����ѡ��
		LCD_ShowString(2 * 24, 32 + 24 * m, (u8 *)table[i].name, RED, WHITE, 24, 0); // ��ʾѡ��
		if (table[i].down <= table[i].current)
		{
			show_flag = 1;
			break;
		}
		m++;
		i++;
	} // ��ʾ����ѡ��
	if (num != last_num)
		show_flag = 0;
}

/******************************************************************************
	  ����˵�������Խ���
	  ������ݣ���
	  ����ֵ��  ��
******************************************************************************/
void test_show()
{
	LCD_ShowIntNum(10, 12 * 16, F_KEY_Down[KEY1], 4, RED, WHITE, 16);
}

/******************************************************************************
	  ����˵��������չʾ����
	  ������ݣ���
	  ����ֵ��  ��
******************************************************************************/
void final_menu()
{
	static u8 num = 0, last_num = 0;
	static u8 key_flag = 0;

	// �����ǰ���治��"show"�����"hello"����
	if (strcmp(table[num].name, "show") != 0 && strcmp(table[num].name, "hello") != 0) // ����show����Ϳ�ʼ����
	{
		// �ϼ������»���ģ���ϼ���ADCֵ����⵽
		if ((F_KEY_Down[KEY_UP] == 1 || AV_ADC_Channel2_Sample > 3300 || AV_ADC_Channel6_Sample > 3300) &&
			AV_ADC_Channel1_Sample <= 3000 && AV_ADC_Channel1_Sample >= 1200 &&
			AV_ADC_Channel3_Sample <= 3000 && AV_ADC_Channel3_Sample >= 1200 && key_flag == 0) // ��
		{
			num = table[table[num].up].current;
			LCD_Fill(0, 32, 24 * 2, 32 + 24 * 5, WHITE);
			key_flag = 1;
		}
		// �¼������»���ģ���¼���ADCֵ����⵽
		else if ((F_KEY_Down[KEY_DOWN] == 1 || AV_ADC_Channel2_Sample < 900 || AV_ADC_Channel6_Sample < 900) &&
				 AV_ADC_Channel1_Sample <= 3000 && AV_ADC_Channel1_Sample >= 1200 &&
				 AV_ADC_Channel3_Sample <= 3000 && AV_ADC_Channel3_Sample >= 1200 && key_flag == 0) // ��
		{
			num = table[table[num].down].current;
			LCD_Fill(0, 32, 24 * 2, 32 + 24 * 5, WHITE);
			key_flag = 1;
		}
		// ��������»���ģ�������ADCֵ����⵽��ע������İ��������ܻ������⣩
		else if ((F_KEY_Down[KEY_RIGHT] == 1 || AV_ADC_Channel1_Sample < 900 || AV_ADC_Channel3_Sample < 900) &&
				 AV_ADC_Channel2_Sample <= 3000 && AV_ADC_Channel2_Sample >= 1200 &&
				 AV_ADC_Channel6_Sample <= 3000 && AV_ADC_Channel6_Sample >= 1200 && key_flag == 0) // �� ���أ���Ҫ�����������Ի��ˣ�
		{
			if (table[table[num].back].current != 0)
			{
				num = table[table[num].back].current;
				LCD_Fill(0, 0, 240, 280, WHITE);
			}
			key_flag = 1;
		}
		// �Ҽ������»���ģ���Ҽ���ADCֵ����⵽
		else if ((F_KEY_Down[KEY1] == 1 || AV_ADC_Channel1_Sample > 3300 || AV_ADC_Channel3_Sample > 3300) &&
				 AV_ADC_Channel2_Sample <= 3000 && AV_ADC_Channel2_Sample >= 1200 &&
				 AV_ADC_Channel6_Sample <= 3000 && AV_ADC_Channel6_Sample >= 1200 && key_flag == 0) // �� ����
		{
			num = table[table[num].enter].current;
			LCD_Fill(0, 0, 240, 280, WHITE);
			key_flag = 1;
		}

		// ���û�а�����������ADCֵ��������Χ�ڣ�����key_flag
		if (F_KEY_Down[KEY_UP] == 0 && F_KEY_Down[KEY_DOWN] == 0 && F_KEY_Down[KEY_RIGHT] == 0 && F_KEY_Down[KEY1] == 0 &&
			AV_ADC_Channel1_Sample <= 3000 && AV_ADC_Channel1_Sample >= 1200 &&
			AV_ADC_Channel2_Sample <= 3000 && AV_ADC_Channel2_Sample >= 1200 &&
			AV_ADC_Channel3_Sample <= 3000 && AV_ADC_Channel3_Sample >= 1200 &&
			AV_ADC_Channel6_Sample <= 3000 && AV_ADC_Channel6_Sample >= 1200 && key_flag == 1)
			key_flag = 0;

		// ��ʾ��ǰҳ��
		show_page(num, last_num);
	}

	// �����"hello"���棨��ʼ���棩
	else if (strcmp(table[num].name, "hello") == 0) // ��ʼ����
	{
		show_hello_page();
		// ����������������
		if (F_KEY_Down[KEY_UP] != 0 || F_KEY_Down[KEY_DOWN] != 0 || F_KEY_Down[KEY_RIGHT] != 0 || F_KEY_Down[KEY1] != 0 || F_KEY_Down[KEY_LEFT] != 0)
		{
			num = 1;
			key_flag = 1;
			LCD_Fill(0, 0, 240, 280, WHITE);
		}
	}
	// �����"show"����
	else // show����
	{
		if (table[num].current == 17) // ң�ؽ��棬����д
		{
			control_page();
		}
		else if (table[table[table[table[num].back].back].back].current == 2) // �ְֵİְֵİְ��ǲ���ͼ�����ν���
		{
			osc_page(num);
		}
		else
			data_page(num);

		// �����������£���Ҫ�����������Ի��ˣ�
		if (F_KEY_Down[KEY_RIGHT]) // �� ���أ���Ҫ�����������Ի��ˣ�
		{
			key_flag = 1;
			num = table[table[num].back].current;
			LCD_Fill(0, 0, 240, 280, WHITE);
		}
	}
	last_num = num;
}
