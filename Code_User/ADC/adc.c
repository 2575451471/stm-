#include "adc.h"
#include "led.h"
#include "bit_band.h"
#include "delay.h"

//------------------------------------------------------------------------------
u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel] = {0}; // DMA�������ݴ�Ŵ�

u8 C_ADC_Sample = 0; // ADC��������

u32 AM_ADC_Channel1_Sample = 0; // ADCͨ��1�����ܼ�
u32 AM_ADC_Channel2_Sample = 0; // ADCͨ��2�����ܼ�
u32 AM_ADC_Channel3_Sample = 0; // ADCͨ��3�����ܼ�
u32 AM_ADC_Channel6_Sample = 0; // ADCͨ��6�����ܼ�

u16 AV_ADC_Channel1_Sample = 2047; // ADC_CH1ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
u16 AV_ADC_Channel2_Sample = 2047; // ADC_CH1ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
u16 AV_ADC_Channel3_Sample = 2047; // ADC_CH3ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
u16 AV_ADC_Channel6_Sample = 2047; // ADC_CH1ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
//------------------------------------------------------------------------------

/**
 * @brief ��ʼ��MYADC
 *
 * �ú������ڳ�ʼ��ADC1ģ�鼰��������š�
 *
 * @return �޷���ֵ
 */
void MYADC_Init(void)
{
	ADC_InitTypeDef ADC_Initstructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// ����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // ����ADC��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ��������GPIOAʱ��

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC��Ƶ������Ƶ  72/6=12hz

	// ����GPIOA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ���ýṹ��
	ADC_Initstructure.ADC_Mode = ADC_Mode_Independent;					// ģʽ���� ����ģʽ
	ADC_Initstructure.ADC_DataAlign = ADC_DataAlign_Right;				// ����ģʽ �Ҷ���
	ADC_Initstructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �������ƵĴ���Դ ����Ҫ�ⲿ����
	ADC_Initstructure.ADC_ContinuousConvMode = DISABLE;					// ����ģʽ enable ����   disable  ����
	ADC_Initstructure.ADC_ScanConvMode = DISABLE;						// ɨ��ģʽ enableɨ��ģʽ         disable��ɨ��
	ADC_Initstructure.ADC_NbrOfChannel = 1;								// ָ��������ͨ������Ŀ

	ADC_Init(ADC1, &ADC_Initstructure);

	// ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE); // ADC�ϵ�

	// У׼
	ADC_ResetCalibration(ADC1); // ��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET)
		;						// ���ظ�λУ׼��״̬  ���û��ɾ�һֱѭ�� ��λ�������
	ADC_StartCalibration(ADC1); // ��ʼУ׼
	while (ADC_GetCalibrationStatus(ADC1) == SET)
		; // У׼�Ƿ���ɣ����û��ɾ�һֱѭ��
}

/**
 * @brief ��ָ����ADCͨ����ȡֵ
 *
 * ��ָ����ADCͨ����ȡģ�⵽����ת����ֵ��
 *
 * @param ADC_Channel Ҫ��ȡֵ��ADCͨ��
 *
 * @return ���ش�ָ��ADCͨ����ȡ��16λֵ
 */
uint16_t ADC_Getvalue(uint8_t ADC_Channel) // ����ͨ����ȡֵ
{
	// ADCͨ������
	// ����ͨ�����Ի�ȡֵ
	// ADCͨ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_13Cycles5); // ������1д��ͨ��0  ʱ��55.5��ADCʱ��

	// �������ת��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // �������ת��

	// �ȴ�ת�����
	// ��ȡ��־λ״̬  ������ת����ɱ�־λ   0ת��δ���  1	ת�����
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		; // ��ȡ��־λ״̬  ������ת����ɱ�־λ   0ת��δ���  1	ת�����

	// ����ת��ֵ
	return ADC_GetConversionValue(ADC1);
}

// ��ȡ�ĸ�ADCͨ����ֵ
//-----------------------------------------------------------------------------------------------
/**
 * @brief ����ADC1��ͨ������ֵ��ƽ��ֵ
 *
 * �ú������ڶ�ADC1�Ķ��ͨ�����в�����������ÿ��ͨ������ֵ��ƽ��ֵ��
 * ��Ҫ����ҡ�˵ļ�⣬ͨ������ҡ���ڲ�ͬ�����ϵ�ƽ��ֵ���õ�ҡ�˵�λ�á�
 */
void ADC1_Value_average(void)
{
	// ҡ�˼��
	//------------------------------------------------------------------------
	// ���ʹ��ADC1ת��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// �ȴ�ת������
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;

	// ��ȡADCͨ��1�Ĳ���ֵ
	AV_ADC_Channel1_Sample = ADC_Getvalue(ADC_Channel_1);
	// ��ȡADCͨ��2�Ĳ���ֵ
	AV_ADC_Channel2_Sample = ADC_Getvalue(ADC_Channel_2);
	// ��ȡADCͨ��3�Ĳ���ֵ
	AV_ADC_Channel3_Sample = ADC_Getvalue(ADC_Channel_3);
	// ��ȡADCͨ��6�Ĳ���ֵ
	AV_ADC_Channel6_Sample = ADC_Getvalue(ADC_Channel_6);
	// �ٴλ�ȡADCͨ��1�Ĳ���ֵ�������������
	AV_ADC_Channel1_Sample = ADC_Getvalue(ADC_Channel_1);
}
//-----------------------------------------------------------------------------------------------
