#include "key.h"

// 按键按下计时
// -------------------------
u8 WKUP_Cnt = 0;
u8 KEY1_Cnt = 0;
u8 KEY_UP_Cnt = 0;
u8 KEY_DOWN_Cnt = 0;
u8 KEY_LEFT_Cnt = 0;
u8 KEY_RIGHT_Cnt = 0;
u8 KEY_A_Cnt = 0;
u8 KEY_AA_Cnt = 0;
u8 KEY_B_Cnt = 0;
u8 KEY_BB_Cnt = 0;
u8 KEY_Rocker1_Cnt = 0;
u8 KEY_Rocker2_Cnt = 0;
// -------------------------

// 12个按键的状态：按下为1，松开为0
//-------------------------------
u8 F_KEY_Down[12] = {0};
//-------------------------------

// 按键初始化
//---------------------
/**
 * @brief 初始化按键相关GPIO
 *
 * 本函数用于初始化与按键相关的GPIO引脚。
 *
 * @return 无返回值
 */
void KEY_Init_JX(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO_InitTypeDef型结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // JTAG失能

	// 使能GPIOA、GPIOC端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); // 使能GPIO时钟

	// 按键：WKUP--PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	  // WKUP--PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 下拉输入
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 输入模式不需要设端口速度
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 根据设定参数初始化PA0

	// 按键：KEY_LeftRocker--PA8、KEY_UP--PA11、KEY_DOWN--PA12、KEY_LEFT--PA15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  // 根据设定参数初始化

	// 按键：KEY_RIGHT--PB3、KEY_A--PB6、KEY_A+--PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);		  // 根据设定参数初始化

	// 按键：KEY_1--PC14、KEY_RightRocker--PC15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);		  // 根据设定参数初始化
}

// 按键扫描函数
//------------------------------------------
/**
 * @brief 扫描按键状态
 *
 * 该函数用于扫描多个按键的状态，包括WKUP、KEY1、KEY_UP、KEY_DOWN、KEY_LEFT、KEY_RIGHT、KEY_A、KEY_AA、KEY_Rocker1和KEY_RightRocker。
 * 按键状态通过全局数组F_KEY_Down记录，如果按键被按下超过一定时间（此处为9次循环，假设每次循环代表一次按键检测周期），则对应数组元素被设置为1，表示按键被按下。
 *
 */
void RF2G4_KEY_Scan_JX(void)
{
	// WKUP
	//------------------------------------
	// 判断WKUP键是否按下
	if (!WKUP_in)
	{
		// 如果WKUP键未按下，重置计数器并标记为未按下
		WKUP_Cnt = 0;
		F_KEY_Down[WKUP] = 0;
	}
	else
	{
		// 如果WKUP键按下，增加计数器
		WKUP_Cnt++;
		// 如果计数器超过8，标记WKUP键为按下
		if (WKUP_Cnt > 8)
		{
			F_KEY_Down[WKUP] = 1;
		}
	}
	//------------------------------------

	// KEY1
	//------------------------------------
	// 判断KEY1键是否按下
	if (KEY1_in)
	{
		// 如果KEY1键未按下，重置计数器并标记为未按下
		KEY1_Cnt = 0;
		F_KEY_Down[KEY1] = 0;
	}
	else
	{
		// 如果KEY1键按下，增加计数器
		KEY1_Cnt++;
		// 如果计数器超过8，标记KEY1键为按下
		if (KEY1_Cnt > 8)
		{
			F_KEY_Down[KEY1] = 1;
		}
	}
	//------------------------------------

	// KEY_UP
	//------------------------------------
	// 判断KEY_UP键是否按下
	if (KEY_UP_in)
	{
		// 如果KEY_UP键未按下，重置计数器并标记为未按下
		KEY_UP_Cnt = 0;
		F_KEY_Down[KEY_UP] = 0;
	}
	else
	{
		// 如果KEY_UP键按下，增加计数器
		KEY_UP_Cnt++;
		// 如果计数器超过8，标记KEY_UP键为按下
		if (KEY_UP_Cnt > 8)
		{
			F_KEY_Down[KEY_UP] = 1;
		}
	}
	//------------------------------------

	// KEY_DOWN
	//------------------------------------
	// 判断KEY_DOWN键是否按下
	if (KEY_DOWN_in)
	{
		// 如果KEY_DOWN键未按下，重置计数器并标记为未按下
		KEY_DOWN_Cnt = 0;
		F_KEY_Down[KEY_DOWN] = 0;
	}
	else
	{
		// 如果KEY_DOWN键按下，增加计数器
		KEY_DOWN_Cnt++;
		// 如果计数器超过8，标记KEY_DOWN键为按下
		if (KEY_DOWN_Cnt > 8)
		{
			F_KEY_Down[KEY_DOWN] = 1;
		}
	}
	//------------------------------------

	// KEY_LEFT
	//------------------------------------
	// 判断KEY_LEFT键是否按下
	if (KEY_LEFT_in)
	{
		// 如果KEY_LEFT键未按下，重置计数器并标记为未按下
		KEY_LEFT_Cnt = 0;
		F_KEY_Down[KEY_LEFT] = 0;
	}
	else
	{
		// 如果KEY_LEFT键按下，增加计数器
		KEY_LEFT_Cnt++;
		// 如果计数器超过8，标记KEY_LEFT键为按下
		if (KEY_LEFT_Cnt > 8)
		{
			F_KEY_Down[KEY_LEFT] = 1;
		}
	}
	//------------------------------------

	// KEY_RIGHT
	//------------------------------------
	// 判断KEY_RIGHT键是否按下
	if (KEY_RIGHT_in)
	{
		// 如果KEY_RIGHT键未按下，重置计数器并标记为未按下
		KEY_RIGHT_Cnt = 0;
		F_KEY_Down[KEY_RIGHT] = 0;
	}
	else
	{
		// 如果KEY_RIGHT键按下，增加计数器
		KEY_RIGHT_Cnt++;
		// 如果计数器超过8，标记KEY_RIGHT键为按下
		if (KEY_RIGHT_Cnt > 8)
		{
			F_KEY_Down[KEY_RIGHT] = 1;
		}
	}
	//------------------------------------

	// KEY_A
	//------------------------------------
	// 判断KEY_A键是否按下
	if (KEY_A_in)
	{
		// 如果KEY_A键未按下，重置计数器并标记为未按下
		KEY_A_Cnt = 0;
		F_KEY_Down[KEY_A] = 0;
	}
	else
	{
		// 如果KEY_A键按下，增加计数器
		KEY_A_Cnt++;
		// 如果计数器超过8，标记KEY_A键为按下
		if (KEY_A_Cnt > 8)
		{
			F_KEY_Down[KEY_A] = 1;
		}
	}
	//------------------------------------

	// KEY_AA
	//------------------------------------
	// 判断KEY_AA键是否按下
	if (KEY_AA_in)
	{
		// 如果KEY_AA键未按下，重置计数器并标记为未按下
		KEY_AA_Cnt = 0;
		F_KEY_Down[KEY_AA] = 0;
	}
	else
	{
		// 如果KEY_AA键按下，增加计数器
		KEY_AA_Cnt++;
		// 如果计数器超过8，标记KEY_AA键为按下
		if (KEY_AA_Cnt > 8)
		{
			F_KEY_Down[KEY_AA] = 1;
		}
	}
	//------------------------------------

	// KEY_Rocker1
	//------------------------------------
	// 判断KEY_Rocker1键是否按下
	if (KEY_Rocker1_in)
	{
		// 如果KEY_Rocker1键未按下，重置计数器并标记为未按下
		KEY_Rocker1_Cnt = 0;
		F_KEY_Down[KEY_Rocker1] = 0;
	}
	else
	{
		// 如果KEY_Rocker1键按下，增加计数器
		KEY_Rocker1_Cnt++;
		// 如果计数器超过8，标记KEY_Rocker1键为按下
		if (KEY_Rocker1_Cnt > 8)
		{
			F_KEY_Down[KEY_Rocker1] = 1;
		}
	}
	//------------------------------------

	// KEY_RightRocker
	//------------------------------------
	// 判断KEY_Rocker2键是否按下
	if (KEY_Rocker2_in)
	{
		// 如果KEY_Rocker2键未按下，重置计数器并标记为未按下
		KEY_Rocker2_Cnt = 0;
		F_KEY_Down[KEY_Rocker2] = 0;
	}
	else
	{
		// 如果KEY_Rocker2键按下，增加计数器
		KEY_Rocker2_Cnt++;
		// 如果计数器超过8，标记KEY_Rocker2键为按下
		if (KEY_Rocker2_Cnt > 8)
		{
			F_KEY_Down[KEY_Rocker2] = 1;
		}
	}
	//------------------------------------
}
//------------------------------------------
