#ifndef __ADC_H
#define __ADC_H


#include "stm32f10x.h"


//------------------------------------------------------------
#define		C_ADC_Channel			4		// ADC通道数
#define		N_ADC_Channel_1			1		// ADC通道1
#define		N_ADC_Channel_2			2		// ADC通道2
#define		N_ADC_Channel_3			3		// ADC通道3
#define		N_ADC_Channel_6			6		// ADC通道6
#define 	C_Channel_Sample		5		// ADC采样次数
//------------------------------------------------------------

//--------------------------------------------------------------------------
extern u8   C_ADC_Sample ;					// ADC采样次数
	
extern u32  AM_ADC_Channel1_Sample ;		// ADC通道1采样总计
extern u32  AM_ADC_Channel2_Sample ;		// ADC通道2采样总计
extern u32  AM_ADC_Channel3_Sample ;		// ADC通道3采样总计
extern u32  AM_ADC_Channel6_Sample ;		// ADC通道6采样总计
	
extern u16  AV_ADC_Channel1_Sample ;		// ADC_CH1平均值
extern u16  AV_ADC_Channel2_Sample ;		// ADC_CH2平均值
extern u16  AV_ADC_Channel3_Sample ;		// ADC_CH3平均值
extern u16  AV_ADC_Channel6_Sample ;		// ADC_CH6平均值


extern u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel];	// DMA搬运数据存放处
//--------------------------------------------------------------------------

// 摇杆的ADC初始化
void ADC1_Rocker_Init(void);

// 获取ADC通道的平均值
void ADC1_Value_average(void);
void ADC1_Init(void);
void MYADC_Init(void);
uint16_t ADC_Getvalue(uint8_t ADC_Channel);


#endif	/* __ADC_H */

