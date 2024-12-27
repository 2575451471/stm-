#ifndef __RF2G4_H
#define __RF2G4_H	


#include "stm32f10x.h"
#include "bit_band.h"
#include "spi.h"
#include "key.h"
#include "adc.h"

//--------------------------------------
// 2G4ͨ�ŵ�ַ
//--------------------------------
extern const u8 RF2G4_ADDR_TX[5];

extern const u8 RF2G4_ADDR_RX[5];
//--------------------------------

// 2.4G�������ݻ�������
extern u8 RF2G4_Send_Data[32];

// 2.4G�������ݻ�������
extern u8 RF2G4_Receive_Data[32];
//--------------------------------------


//SI24R1�Ĵ�����������
//-----------------------------------------------------------------------------------
#define 	R_REGISTER    		0x00  	// ���Ĵ���,��5λΪ�Ĵ�����ַ
#define 	W_REGISTER   		0x20  	// д�Ĵ���,��5λΪ�Ĵ�����ַ
#define 	R_RX_PAYLOAD     	0x61  	// ��RX��Ч����,1~32�ֽ�
#define 	W_TX_PAYLOAD     	0xA0  	// дTX��Ч����,1~32�ֽ�
#define 	FLUSH_TX        	0xE1  	// ���TX FIFO�Ĵ���.����ģʽ����
#define 	FLUSH_RX        	0xE2  	// ���RX FIFO�Ĵ���.����ģʽ����
#define 	REUSE_TX_PL     	0xE3  	// ����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define 	NOP             	0xFF  	// �ղ���,����������״̬�Ĵ���	 
//-----------------------------------------------------------------------------------

//SPI(SI24R1)�Ĵ�����ַ
//----------------------------------------------------------------------------------------------------------------------
#define 	CONFIG          	0x00  	// ���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
										// bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define 	EN_AA           	0x01  	// ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define 	EN_RXADDR       	0x02  	// ���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define 	SETUP_AW        	0x03  	// ���õ�ַ����(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define 	SETUP_RETR      	0x04  	// �����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define 	RF_CH           	0x05  	// RFͨ��,bit6:0,����ͨ��Ƶ��;
#define 	RF_SETUP        	0x06  	// RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define 	STATUS          	0x07  	// ״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
										// bit5:���ݷ�������ж�;bit6:���������ж�;
										
#define 	OBSERVE_TX      	0x08  	// ���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define 	RSSI              	0x09  	// �ز����Ĵ���,bit0,�ز����;
#define 	RX_ADDR_P0      	0x0A  	// ����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define 	RX_ADDR_P1      	0x0B  	// ����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define 	RX_ADDR_P2      	0x0C  	// ����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define 	RX_ADDR_P3      	0x0D  	// ����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define 	RX_ADDR_P4      	0x0E  	// ����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define 	RX_ADDR_P5      	0x0F  	// ����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define 	TX_ADDR         	0x10  	// ���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define 	RX_PW_P0        	0x11  	// ��������ͨ��0��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define 	RX_PW_P1        	0x12  	// ��������ͨ��1��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define 	RX_PW_P2        	0x13  	// ��������ͨ��2��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define 	RX_PW_P3        	0x14  	// ��������ͨ��3��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define 	RX_PW_P4        	0x15  	// ��������ͨ��4��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define 	RX_PW_P5        	0x16  	// ��������ͨ��5��Ч���ݿ���(1~32�ֽ�),����Ϊ0��Ƿ�
#define 	FIFO_STATUS 		0x17  	// FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
										// bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
#define 	DYNPD			0x1C  //  ʹ�ܶ�̬���س���
#define 	FEATURE			0x1D  //  �����Ĵ���
										
#define 	TX_MAX  			0x10  	// �ﵽ����ʹ����ж�
#define 	TX_OK   			0x20  	// TX��������ж�
#define 	RX_OK   			0x40  	// ���յ������ж�


#define		TX_SUCCESS			0		// ���ͳɹ�
#define		TX_FAIL				1		// ����ʧ��
#define		TX_FAIL_MAX			2		// ����ʧ��
#define		RX_SUCCESS			0		// ���ճɹ�
#define		RX_FAIL				1		// ����ʧ��
//----------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------
//SI24R1����
#define 	RF2G4_CE   		PB_out(12) 	// SI24R1ѡ�ź�
#define 	RF2G4_CSN  		PB_out(10) 	// SPIƬѡ�ź�	   
#define 	RF2G4_IRQ  		PB_in(11)  	// IRQ������������

//SI24R1�շ����ݿ���
#define 	TX_ADR_WIDTH    5   		// �����ַ���ȣ�5�ֽڣ�
#define 	TX_PLOAD_WIDTH 	32  	// ����ͨ����Ч���ݿ���
#define 	RX_ADR_WIDTH    5   		// ���յ�ַ���ȣ�5�ֽڣ�
#define 	RX_MODE 0
#define 	TX_MODE 1
//---------------------------------------------------------------					   	   

//-------------------------------------------------------------------------------------
void RF2G4_Init(void);									// RF2G4��ʼ��
u8 RF2G4_Write_Reg(u8 Reg,u8 Write_Value);				// SPI��SI24R1�Ĵ�����д����
u8 RF2G4_Write_Cont(u8 Reg, u8* P_Data, u8 N_Data);		// ��Ĵ�����д��ָ�����ȵ�����
u8 RF2G4_Read_Reg(u8 Reg);								// SPI��SI24R1�Ĵ����Ķ�����
u8 RF2G4_Read_Cont(u8 Reg, u8* P_Data, u8 N_Data);		// �ӼĴ����ж���ָ�����ȵ�����

u8 RF2G4_Check(void);		// ���SI24R1�Ƿ�����

void RF2G4_RX_Mode(void);	// ��ʼ��SI24R1Ϊ����ģʽ
void RF2G4_TX_Mode(void);	// ��ʼ��SI24R1Ϊ����ģʽ
void RF2G4_Mode_NOACK(uint8_t Mode);

u8 RF2G4_Tx_Packet(u8* P_Data,u8 N_Data);	// SI24R1����һ֡����
u8 RF2G4_Rx_Packet(u8* P_Data,u8 N_Data);	// SI24R1����һ֡����
//-------------------------------------------------------------------------------------

#endif	/* __RF2G4_H */