#ifndef __ADC_H
#define __ADC_H


#include "stm32f10x.h"


//------------------------------------------------------------
#define		C_ADC_Channel			4		// ADCͨ����
#define		N_ADC_Channel_1			1		// ADCͨ��1
#define		N_ADC_Channel_2			2		// ADCͨ��2
#define		N_ADC_Channel_3			3		// ADCͨ��3
#define		N_ADC_Channel_6			6		// ADCͨ��6
#define 	C_Channel_Sample		5		// ADC��������
//------------------------------------------------------------

//--------------------------------------------------------------------------
extern u8   C_ADC_Sample ;					// ADC��������
	
extern u32  AM_ADC_Channel1_Sample ;		// ADCͨ��1�����ܼ�
extern u32  AM_ADC_Channel2_Sample ;		// ADCͨ��2�����ܼ�
extern u32  AM_ADC_Channel3_Sample ;		// ADCͨ��3�����ܼ�
extern u32  AM_ADC_Channel6_Sample ;		// ADCͨ��6�����ܼ�
	
extern u16  AV_ADC_Channel1_Sample ;		// ADC_CH1ƽ��ֵ
extern u16  AV_ADC_Channel2_Sample ;		// ADC_CH2ƽ��ֵ
extern u16  AV_ADC_Channel3_Sample ;		// ADC_CH3ƽ��ֵ
extern u16  AV_ADC_Channel6_Sample ;		// ADC_CH6ƽ��ֵ


extern u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel];	// DMA�������ݴ�Ŵ�
//--------------------------------------------------------------------------

// ҡ�˵�ADC��ʼ��
void ADC1_Rocker_Init(void);

// ��ȡADCͨ����ƽ��ֵ
void ADC1_Value_average(void);
void ADC1_Init(void);
void MYADC_Init(void);
uint16_t ADC_Getvalue(uint8_t ADC_Channel);


#endif	/* __ADC_H */

