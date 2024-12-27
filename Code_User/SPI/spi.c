#include "spi.h"

// SPI2��ʼ��
//---------------------------------------------------------------------------------------------------------------
/**
 * @brief ��ʼ��SPI2�ӿ�
 *
 * �ú������ڳ�ʼ��STM32΢��������SPI2�ӿڣ�����������Ϊ˫��˫��ȫ˫��ģʽ��
 * ��ΪSPI���������ͺͽ���8λ֡�ṹ�����ݣ�ʱ�����յͣ����ݲ����ڵ�1��ʱ���أ�
 * NSS�ź���������ƣ�������Ԥ��ƵֵΪ16�����ݴ����MSBλ��ʼ��CRCֵ����Ķ���ʽΪ7��
 */
void SPI2_Init_JX(void)
{
    // ����SPI��ʼ���ṹ��
    SPI_InitTypeDef  SPI_InitStructure;

    // ʹ��SPI2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    // SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // SPI����Ϊ˫��˫��ȫ˫��
    // SPI����
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// SPI����
    // ���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// ���ͽ���8λ֡�ṹ
    // ʱ�����յ�
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// ʱ�����յ�
    // ���ݲ����ڵ�1��ʱ����
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						// ���ݲ����ڵ�1��ʱ����
    // NSS�ź����������
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// NSS�ź����������
    // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
    // ���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// ���ݴ����MSBλ��ʼ
    // CRCֵ����Ķ���ʽ
    SPI_InitStructure.SPI_CRCPolynomial = 7;							// CRCֵ����Ķ���ʽ

    // ��ʼ��SPI2
    SPI_Init(SPI2, &SPI_InitStructure);

    // ʹ��SPI����
    SPI_Cmd(SPI2, ENABLE); 		// ʹ��SPI����

    // ��������
    //SPI2_ReadWriteByte(0xFF);	// ��������		 
}   
//---------------------------------------------------------------------------------------------------------------


// ����SPI�ٶ�
//---------------------------------------------------
// SPI_DivideFrequency_2	2��Ƶ�� 18MHz
// SPI_DivideFrequency_4	4��Ƶ�� 9MHz
// SPI_DivideFrequency_8	8��Ƶ�� 4.5MHz
// SPI_DivideFrequency_16	16��Ƶ��2.25MHz
/**
 * @brief ����SPI2���ٶ�
 *
 * ͨ���޸�SPI2��CR1�Ĵ��������λ������SPI2���ٶȡ�
 *
 * @param SPI_DivideFrequency SPI��Ƶֵ����������SPI2���ٶȡ�
 */
void SPI2_SetSpeed(u8 SPI_DivideFrequency)
{
	// ���SPI2��CR1�Ĵ��������λ��Ϊ�������ٶ���׼��
	SPI2->CR1 &= 0XFFC7;	// ���CR1�Ĵ��������λ

	// ����SPI2���ٶ�
	SPI2->CR1 |= SPI_DivideFrequency;	//����SPI2�ٶ� 

	// ����SPI2���ѱ�ע�͵���
	//SPI_Cmd(SPI2,ENABLE); 
} 
//---------------------------------------------------


// SPI��дһ���ֽ�
// TxData:Ҫд����ֽ�
// ����ֵ:��ȡ�����ֽ�
//-----------------------------------------------------------------------
/**
 * @brief ͨ��SPI2�ӿڶ�дһ���ֽڵ�����
 *
 * �ú���ͨ��SPI2�ӿڷ���һ���ֽڵ����ݣ������շ��ص��ֽ����ݡ�
 *
 * @param TxData Ҫ���͵������ֽ�
 *
 * @return ���ؽ��յ��������ֽ�
 *         ������ͻ���ճ�ʱ������250��ѭ�������򷵻�0��ʾʧ��
 */
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 TxWait = 0;
	u8 RxWait = 0;
	
	// �ȴ����ͻ���Ϊ��
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		TxWait++; // �ȴ����ͻ���Ϊ�ռ���
		if(TxWait>250)	// �ȴ�ʱ�������������ζ�д
			return 0;
	}	

	// SPI2дһ���ֽ�
	SPI_I2S_SendData(SPI2, TxData); // SPI2дһ���ֽ�
	
	// �ȴ����ջ���Ϊ��
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		RxWait++; // �ȴ����ջ���Ϊ�ռ���
		if(RxWait>250)	// �ȴ�ʱ�������������ζ�д
			return 0;
	}	  	
	
	// ���������ֽڷ���
	return SPI_I2S_ReceiveData(SPI2); // ���������ֽڷ���					    
}
//-----------------------------------------------------------------------
