#include "command.h"

#define TX_MODE_1 1

// ȫ�ֱ���
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0; // TIM3�Ķ�ʱ�ƴ�

u8 One_Ms_Timing = 0; // 1msȫ��ʱ��

u8 F_2G4_Send = 0; // RF׼�������־λ

u8 Set_flag = 0;

UG_GUI my_ugui1;

u8 t_flag = 0;
//------------------------------------------------------------------

/**
 * @brief ������
 *
 * �ú����ǳ������ڵ㣬��Ҫ�����ʼ��Ӳ���ӿڡ������ж����ȼ���������Ƶͨ���Լ�LCD��ʾ�Ȳ�����
 */
int main(void)
{
	// ��������
	u8 i = 0;
	double n = 0, last_num = 0;
	// ע��������ʹ���ж�ʱ��NVIC��������Ӧ����λ�ڳ�����ʼ�������������ú�����Ҫ�ٸ���NVIC����
	//------------------------------------------------------------------------------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC����2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	// ��ʱ��ʼ��
	delay_Init(); // ��ʱ��ʼ����ע��������ʱ����֮ǰ�������ȵ���delay_Init()��SysTick��ʼ����

	// ADC��ʼ��
	MYADC_Init(); // ADC��ʼ����ҡ�ˣ�

	// ��ʼ��LEDӲ���ӿ�
	LED_Init_JX(); // ��ʼ��LEDӲ���ӿڣ�Green--PB4��Blue--PB5

	// ��ʱ��3��ʼ��
	TIM3_Time_Init(); // ��ʱ��3��ʼ��

	// ��ʼ������Ӳ���ӿ�
	KEY_Init_JX(); // ��ʼ������Ӳ���ӿ�

	// DMA��ʼ��
	DMA_Init_JX(); // DMA��ʼ��������ADC���ݣ�

	// SI24R1��ʼ��
	SI24R1_Init();

	// ��ʼ��LCD
	//-----------------------------------------
	UG_Init(&my_ugui1, LCD_DrawPoint, 240, 280); // ��ʼ��UGUI
	UG_SelectGUI(&my_ugui1);					 // ѡ��ǰʹ�õ�GUI
	LCD_Init();									 // LCD��ʼ��
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);		 // ���LCD����ɫΪ��ɫ
	
	//-----------------------------------------

	// �ж�SI24R1�Ƿ�����
	//-----------------------------------------
	if (SI2R4_Check())
	{
		LCD_ShowString(0, 0, (u8 *)"  ERROR 2.4G", BLACK, WHITE, 24, 0); // ��ʾ������Ϣ
	}
	else
	{
		LCD_ShowString(0, 0, (u8 *)"  2.4G YES", BLACK, WHITE, 24, 0); // ��ʾ������Ϣ
	}
	//-----------------------------------------
	SI24R1_Mode_NOACK(RX_MODE); // ����SI24R1Ϊ��Ӧ�����ģʽ
LCD_ShowString(0, 96, (u8 *)"ok???", BLACK, WHITE, 24, 0);
	while (1)
	{
		// ע�͵��Ĵ����
		// final_menu();
		// test_show();

		// 1msʱ��
		//----------------------------------
 		if (One_Ms_Timing == 1)
		{
			One_Ms_Timing = 0;
			//RF2G4_KEY_Scan_JX();  // ɨ��2.4G����
			//ADC1_Value_average(); // ����ADC1��ƽ��ֵ
		}
//------------------------------------------------------------------

// ��Ƶ����(50ms����һ֡����)
#if TX_MODE_1
//		final_menu(); // ����final_menu����
		test_tx();
		delay_ms(1);
		
#else
		test_show(); // ����test_show����
		if (!t_flag)
		{
			SI24R1_Mode_NOACK(RX_MODE); // ��־λΪ0������Ϊ����ģʽ
		}

		for (i = 0; i < 10; i++)
		{
			LCD_ShowIntNum(0, 16 * (i + 2), SI24R1_Receive_Data[i], 3, BLACK, WHITE, 16); // ��ʾ��������
			LCD_ShowIntNum(32, 16 * (i + 2), SI24R1_Send_Data[i], 3, BLACK, WHITE, 16);	  // ��ʾ��������
		} // �����ã���ʾ���ͺͽ���ֵ

		if (t_flag == 0 && SI24R1_RxPacket(SI24R1_Receive_Data)) // ��־Ϊ0��˳�����ܵ�����
		{
			if (!SI24R1_Receive_Data[3])
			{
				/*�������*/
				SI24R1_Send_Data[0] = 13;
				SI24R1_Send_Data[1] = 16;
				SI24R1_Send_Data[2] = SI24R1_Receive_Data[2];
				SI24R1_Send_Data[3] = 1;
				SI24R1_Send_Data[6] = 0;
				/*�����������Σ��������·���*/
				if (SI24R1_Receive_Data[2] != last_num)
				{
					n = 0;
				}
				last_num = SI24R1_Receive_Data[2];
				/*��Ҫ��Ĳ�ͬ�������ز�ͬ��ֵ*/
				if (SI24R1_Receive_Data[2] == 47)
				{
					SI24R1_Send_Data[7] = 80 * (sin(n / 12) + 1);
				}
				else if (SI24R1_Receive_Data[2] == 48)
				{
					SI24R1_Send_Data[7] = 2 * n;
				}
				else
				{
					SI24R1_Send_Data[7] = n;
				}

				if (n < 100)
				{
					n++;
				}
				else
				{
					n = 0;
				}

				LED_Blue = !LED_Blue;

				SI24R1_Mode_NOACK(TX_MODE); // ����Ϊ����ģʽ
				t_flag = 1;					// תΪ����ģʽ
			}
			else
			{
				continue;
			}
		}

		if (t_flag) // ����ģʽ�����ͳɹ�
		{
			while (SI24R1_TxPacket(SI24R1_Send_Data) != 0x20)
			{
				i++;
				if (i > 10)
				{
					i = 0;
					break; // ������Ͳ��ɹ����ظ�����10��
				}
			}
			SI24R1_Receive_Data[0] = 0;
			SI24R1_Receive_Data[1] = 0;
			SI24R1_Receive_Data[2] = 0;
			SI24R1_Receive_Data[3] = 0;
			SI24R1_Receive_Data[3] = 1;
			SI24R1_Receive_Data[6] = 0;

			LED_Green = !LED_Green;

			t_flag = 0; // ˳���������ݣ���־λ��Ϊ0�������������
		}
#endif
	}
}


