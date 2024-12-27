#include "command.h"

#define TX_MODE_1 1

// 全局变量
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0; // TIM3的定时计次

u8 One_Ms_Timing = 0; // 1ms全局时基

u8 F_2G4_Send = 0; // RF准备发射标志位

u8 Set_flag = 0;

UG_GUI my_ugui1;

u8 t_flag = 0;
//------------------------------------------------------------------

/**
 * @brief 主函数
 *
 * 该函数是程序的入口点，主要负责初始化硬件接口、配置中断优先级、进行射频通信以及LCD显示等操作。
 */
int main(void)
{
	// 变量定义
	u8 i = 0;
	double n = 0, last_num = 0;
	// 注：程序中使用中断时，NVIC分组设置应尽量位于程序起始处，并且在设置后尽量不要再更改NVIC分组
	//------------------------------------------------------------------------------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC分组2：2位抢占优先级，2位响应优先级

	// 延时初始化
	delay_Init(); // 延时初始化（注：调用延时函数之前，必须先调用delay_Init()将SysTick初始化）

	// ADC初始化
	MYADC_Init(); // ADC初始化（摇杆）

	// 初始化LED硬件接口
	LED_Init_JX(); // 初始化LED硬件接口：Green--PB4、Blue--PB5

	// 定时器3初始化
	TIM3_Time_Init(); // 定时器3初始化

	// 初始化按键硬件接口
	KEY_Init_JX(); // 初始化按键硬件接口

	// DMA初始化
	DMA_Init_JX(); // DMA初始化（搬运ADC数据）

	// SI24R1初始化
	SI24R1_Init();

	// 初始化LCD
	//-----------------------------------------
	UG_Init(&my_ugui1, LCD_DrawPoint, 240, 280); // 初始化UGUI
	UG_SelectGUI(&my_ugui1);					 // 选择当前使用的GUI
	LCD_Init();									 // LCD初始化
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);		 // 填充LCD背景色为白色
	
	//-----------------------------------------

	// 判断SI24R1是否正常
	//-----------------------------------------
	if (SI2R4_Check())
	{
		LCD_ShowString(0, 0, (u8 *)"  ERROR 2.4G", BLACK, WHITE, 24, 0); // 显示错误信息
	}
	else
	{
		LCD_ShowString(0, 0, (u8 *)"  2.4G YES", BLACK, WHITE, 24, 0); // 显示正常信息
	}
	//-----------------------------------------
	SI24R1_Mode_NOACK(RX_MODE); // 设置SI24R1为无应答接收模式

	while (1)
	{
		// 注释掉的代码块
		// final_menu();
		// test_show();

		// 1ms时基
		//----------------------------------
 		if (One_Ms_Timing == 1)
		{
			One_Ms_Timing = 0;
			RF2G4_KEY_Scan_JX();  // 扫描2.4G按键
			ADC1_Value_average(); // 计算ADC1的平均值
		}
//------------------------------------------------------------------

// 射频发送(50ms发射一帧数据)
#if TX_MODE_1
		final_menu(); // 调用final_menu函数
//		test_tx();
//		delay_ms(1);
#else
		test_show(); // 调用test_show函数
		if (!t_flag)
		{
			SI24R1_Mode_NOACK(RX_MODE); // 标志位为0，设置为接受模式
		}

		for (i = 0; i < 10; i++)
		{
			LCD_ShowIntNum(0, 16 * (i + 2), SI24R1_Receive_Data[i], 3, BLACK, WHITE, 16); // 显示接收数据
			LCD_ShowIntNum(32, 16 * (i + 2), SI24R1_Send_Data[i], 3, BLACK, WHITE, 16);	  // 显示发送数据
		} // 调试用，显示发送和接收值

		if (t_flag == 0 && SI24R1_RxPacket(SI24R1_Receive_Data)) // 标志为0，顺利接受到数据
		{
			if (!SI24R1_Receive_Data[3])
			{
				/*填充数据*/
				SI24R1_Send_Data[0] = 13;
				SI24R1_Send_Data[1] = 16;
				SI24R1_Send_Data[2] = SI24R1_Receive_Data[2];
				SI24R1_Send_Data[3] = 1;
				SI24R1_Send_Data[6] = 0;
				/*进入其他波形，数据重新发送*/
				if (SI24R1_Receive_Data[2] != last_num)
				{
					n = 0;
				}
				last_num = SI24R1_Receive_Data[2];
				/*对要求的不同参数返回不同的值*/
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

				SI24R1_Mode_NOACK(TX_MODE); // 设置为发送模式
				t_flag = 1;					// 转为发送模式
			}
			else
			{
				continue;
			}
		}

		if (t_flag) // 发送模式，发送成功
		{
			while (SI24R1_TxPacket(SI24R1_Send_Data) != 0x20)
			{
				i++;
				if (i > 10)
				{
					i = 0;
					break; // 如果发送不成功，重复发送10次
				}
			}
			SI24R1_Receive_Data[0] = 0;
			SI24R1_Receive_Data[1] = 0;
			SI24R1_Receive_Data[2] = 0;
			SI24R1_Receive_Data[3] = 0;
			SI24R1_Receive_Data[3] = 1;
			SI24R1_Receive_Data[6] = 0;

			LED_Green = !LED_Green;

			t_flag = 0; // 顺利发送数据，标志位置为0，接受数据清空
		}
#endif
	}
}


