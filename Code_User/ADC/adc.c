#include "adc.h"
#include "led.h"
#include "bit_band.h"
#include "delay.h"

//------------------------------------------------------------------------------
u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel] = {0}; // DMA搬运数据存放处

u8 C_ADC_Sample = 0; // ADC采样次数

u32 AM_ADC_Channel1_Sample = 0; // ADC通道1采样总计
u32 AM_ADC_Channel2_Sample = 0; // ADC通道2采样总计
u32 AM_ADC_Channel3_Sample = 0; // ADC通道3采样总计
u32 AM_ADC_Channel6_Sample = 0; // ADC通道6采样总计

u16 AV_ADC_Channel1_Sample = 2047; // ADC_CH1平均值（设置初值为中间位置）
u16 AV_ADC_Channel2_Sample = 2047; // ADC_CH1平均值（设置初值为中间位置）
u16 AV_ADC_Channel3_Sample = 2047; // ADC_CH3平均值（设置初值为中间位置）
u16 AV_ADC_Channel6_Sample = 2047; // ADC_CH1平均值（设置初值为中间位置）
//------------------------------------------------------------------------------

/**
 * @brief 初始化MYADC
 *
 * 该函数用于初始化ADC1模块及其相关引脚。
 *
 * @return 无返回值
 */
void MYADC_Init(void)
{
	ADC_InitTypeDef ADC_Initstructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// 配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 开启ADC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启引脚GPIOA时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC分频器六分频  72/6=12hz

	// 配置GPIOA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置结构体
	ADC_Initstructure.ADC_Mode = ADC_Mode_Independent;					// 模式配置 独立模式
	ADC_Initstructure.ADC_DataAlign = ADC_DataAlign_Right;				// 对齐模式 右对齐
	ADC_Initstructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 触发控制的触发源 不需要外部触发
	ADC_Initstructure.ADC_ContinuousConvMode = DISABLE;					// 连续模式 enable 连续   disable  单次
	ADC_Initstructure.ADC_ScanConvMode = DISABLE;						// 扫描模式 enable扫描模式         disable非扫描
	ADC_Initstructure.ADC_NbrOfChannel = 1;								// 指定规则组通道的数目

	ADC_Init(ADC1, &ADC_Initstructure);

	// ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE); // ADC上电

	// 校准
	ADC_ResetCalibration(ADC1); // 复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET)
		;						// 返回复位校准的状态  如果没完成就一直循环 复位后会清零
	ADC_StartCalibration(ADC1); // 开始校准
	while (ADC_GetCalibrationStatus(ADC1) == SET)
		; // 校准是否完成，如果没完成就一直循环
}

/**
 * @brief 从指定的ADC通道获取值
 *
 * 从指定的ADC通道获取模拟到数字转换的值。
 *
 * @param ADC_Channel 要读取值的ADC通道
 *
 * @return 返回从指定ADC通道获取的16位值
 */
uint16_t ADC_Getvalue(uint8_t ADC_Channel) // 输入通道获取值
{
	// ADC通道配置
	// 输入通道可以获取值
	// ADC通道配置
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_13Cycles5); // 在序列1写入通道0  时间55.5个ADC时间

	// 软件触发转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 软件触发转换

	// 等待转换完成
	// 获取标志位状态  规则组转换完成标志位   0转换未完成  1	转换完成
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		; // 获取标志位状态  规则组转换完成标志位   0转换未完成  1	转换完成

	// 返回转换值
	return ADC_GetConversionValue(ADC1);
}

// 获取四个ADC通道的值
//-----------------------------------------------------------------------------------------------
/**
 * @brief 计算ADC1各通道采样值的平均值
 *
 * 该函数用于对ADC1的多个通道进行采样，并计算每个通道采样值的平均值。
 * 主要用于摇杆的检测，通过计算摇杆在不同方向上的平均值，得到摇杆的位置。
 */
void ADC1_Value_average(void)
{
	// 摇杆检测
	//------------------------------------------------------------------------
	// 软件使能ADC1转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// 等待转换结束
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;

	// 获取ADC通道1的采样值
	AV_ADC_Channel1_Sample = ADC_Getvalue(ADC_Channel_1);
	// 获取ADC通道2的采样值
	AV_ADC_Channel2_Sample = ADC_Getvalue(ADC_Channel_2);
	// 获取ADC通道3的采样值
	AV_ADC_Channel3_Sample = ADC_Getvalue(ADC_Channel_3);
	// 获取ADC通道6的采样值
	AV_ADC_Channel6_Sample = ADC_Getvalue(ADC_Channel_6);
	// 再次获取ADC通道1的采样值，进行冗余采样
	AV_ADC_Channel1_Sample = ADC_Getvalue(ADC_Channel_1);
}
//-----------------------------------------------------------------------------------------------
