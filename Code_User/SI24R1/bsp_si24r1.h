#ifndef __bsp_si24r1_H
#define __bsp_si24r1_H

#include "stm32f10x.h"

#define SI24R1_debug				1
#define SPIT_FLAG_TIMEOUT			((uint32_t)0x10000)
#define SI24R1_INFO(fmt,arg...)		printf("<<-SI24R1-INFO->> "fmt"\n",##arg)
#define SI24R1_ERROR(fmt,arg...)	printf("<<-SI24R1-ERROR->> "fmt"\n",##arg)
#define SI24R1_DEBUG(fmt,arg...)	do{\
									if(SI24R1_debug)\
									printf("<<-SI24R1-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
									}while(0)

extern u8 SI24R1_Send_Data[32];		// 2.4G�������ݻ�������
			 
extern u8 SI24R1_Receive_Data[32];	// 2.4G�������ݻ�������

//********************************************************************************************************************//
//                                          FUNCTION's STRUCT                                                         //
//********************************************************************************************************************//
typedef struct
{
	uint8_t ADR_WIDTH;
	uint8_t RX_ADDR_Px;
	uint8_t EN_RXADDR_Px;
	uint8_t RF_CHANNEL;
	uint8_t RX_PW_Px;
	uint8_t RF_SET;
	uint8_t SI24R1_CONFIG;
	uint8_t RF_FEATURE;
}SI24R1_InitTypeDef;

//********************************************************************************************************************//								
//SI24R1 PIN DEFINITION

#define SI24R1_GPIO_RCC RCC_APB2Periph_GPIOB
#define SI24R1_SPI_RCC	RCC_APB1Periph_SPI2
#define SI24R1_SPI		SPI2
#define SI24R1_GPIOA 	GPIOB
#define SI24R1_GPIOB 	GPIOB
#define	SI24R1_MOSI		GPIO_Pin_15		// Master Out, Slave In pin (output)
#define	SI24R1_MISO  	GPIO_Pin_14    	// Master In, Slave Out pin (input)
#define	SI24R1_SCK     	GPIO_Pin_13    	// Serial Clock pin, (output)
#define	SI24R1_CSN     	GPIO_Pin_10    	// Slave Select pin, (output to CSN)
#define	SI24R1_CE    	GPIO_Pin_12    	// Chip Enable pin signal (output)
#define	SI24R1_IRQ    	GPIO_Pin_11   	// Interrupt signal, from nRF24L01 (input)

#define SI24R1_SCK_Clr() 	GPIO_ResetBits(SI24R1_GPIOA,SI24R1_SCK)//SCK
#define SI24R1_SCK_Set() 	GPIO_SetBits(SI24R1_GPIOA,SI24R1_SCK)

#define SI24R1_MOSI_Clr() 	GPIO_ResetBits(SI24R1_GPIOA,SI24R1_MOSI)//MOSI
#define SI24R1_MOSI_Set() 	GPIO_SetBits(SI24R1_GPIOA,SI24R1_MOSI)

#define SI24R1_MISO_Clr() 	GPIO_ResetBits(SI24R1_GPIOA,SI24R1_MISO)//MISO
#define SI24R1_MISO_Set() 	GPIO_SetBits(SI24R1_GPIOA,SI24R1_MISO)

#define SI24R1_CSN_Clr()  	GPIO_ResetBits(SI24R1_GPIOA,SI24R1_CSN)//CSN
#define SI24R1_CSN_Set()  	GPIO_SetBits(SI24R1_GPIOA,SI24R1_CSN)

#define SI24R1_CE_Clr()		GPIO_ResetBits(SI24R1_GPIOA,SI24R1_CE)//CE
#define SI24R1_CE_Set()  	GPIO_SetBits(SI24R1_GPIOA,SI24R1_CE)

#define SI24R1_IRQ_Clr()  	GPIO_ResetBits(SI24R1_GPIOB,SI24R1_IRQ)//IRQ
#define SI24R1_IRQ_Set()  	GPIO_SetBits(SI24R1_GPIOB,SI24R1_IRQ)

#define TX_ADR_WIDTH   	5  		// 5�ֽڿ�ȵķ���/���յ�ַ
#define TX_PLOAD_WIDTH 	32  	// ����ͨ����Ч���ݿ��
#define RX_MODE 0
#define TX_MODE 1

//********************************************************************************************************************//
// SPI(SI24R1) ����
#define R_REGISTER		0x00  // �����ȡ�Ĵ������� 000A AAAA
#define W_REGISTER     	0x20  // ����д��Ĵ������� 001A AAAA
#define RD_RX_PLOAD     0x61  // ����������ݸ��ؼĴ�����ַ
#define WR_TX_PLOAD     0xA0  // ���巢�����ݸ��ؼĴ�����ַ
#define FLUSH_TX        0xE1  // ������շ��ͼĴ�������
#define FLUSH_RX        0xE2  // ������ս��ռĴ�������
#define REUSE_TX_PL     0xE3  // �������÷������ݸ��ؼĴ�������
#define R_RX_PL_WID		0x60  // ������յ��������ֽ�������
#define NOP             0xFF  // �����޲������������ڶ�ȡ״̬�Ĵ���

//********************************************************************************************************************//
// SPI(SI24R1) �Ĵ�������ַ��
#define CONFIG          0x00  // '����' �Ĵ�����ַ
#define EN_AA           0x01  // '�����Զ�Ӧ��' �Ĵ�����ַ
#define EN_RXADDR       0x02  // '���ý��յ�ַ' �Ĵ�����ַ
#define SETUP_AW        0x03  // '���õ�ַ���' �Ĵ�����ַ
#define SETUP_RETR      0x04  // '�����Զ��ش�' �Ĵ�����ַ
#define RF_CH           0x05  // '��Ƶͨ��' �Ĵ�����ַ
#define RF_SETUP        0x06  // '��Ƶ����' �Ĵ�����ַ
#define STATUS          0x07  // '״̬' �Ĵ�����ַ
#define OBSERVE_TX      0x08  // '�۲췢��' �Ĵ�����ַ
#define RSSI            0x09  // '�����ź�ǿ��ָʾ��' �Ĵ�����ַ
#define RX_ADDR_P0      0x0A  // '���յ�ַ�ܵ�0' �Ĵ�����ַ
#define RX_ADDR_P1      0x0B  // '���յ�ַ�ܵ�1' �Ĵ�����ַ
#define RX_ADDR_P2      0x0C  // '���յ�ַ�ܵ�2' �Ĵ�����ַ
#define RX_ADDR_P3      0x0D  // '���յ�ַ�ܵ�3' �Ĵ�����ַ
#define RX_ADDR_P4      0x0E  // '���յ�ַ�ܵ�4' �Ĵ�����ַ
#define RX_ADDR_P5      0x0F  // '���յ�ַ�ܵ�5' �Ĵ�����ַ
#define TX_ADDR         0x10  // '���͵�ַ' �Ĵ�����ַ
#define RX_PW_P0        0x11  // '�������ݸ��ؿ�ȣ��ܵ�0' �Ĵ�����ַ
#define RX_PW_P1        0x12  // '�������ݸ��ؿ�ȣ��ܵ�1' �Ĵ�����ַ
#define RX_PW_P2        0x13  // '�������ݸ��ؿ�ȣ��ܵ�2' �Ĵ�����ַ
#define RX_PW_P3        0x14  // '�������ݸ��ؿ�ȣ��ܵ�3' �Ĵ�����ַ
#define RX_PW_P4        0x15  // '�������ݸ��ؿ�ȣ��ܵ�4' �Ĵ�����ַ
#define RX_PW_P5        0x16  // '�������ݸ��ؿ�ȣ��ܵ�5' �Ĵ�����ַ
#define FIFO_STATUS     0x17  // 'FIFO ״̬�Ĵ���' �Ĵ�����ַ
#define DYNPD			0x1C  // ʹ�ܶ�̬���س���
#define FEATURE			0x1D  // �����Ĵ���

//********************************************************************************************************************//
// STATUS Register
#define RX_DR			0x40  /* RX FIFO��ֵ��־λ��д��1�������*/
#define TX_DS			0x20  /* ����˷�������ж�λ�������ACKģʽ��
								 ���յ�ACKȷ���źź�TX_DSλ�á�1����д��1�������*/
#define MAX_RT			0x10  /* �ﵽ����ط������ж�λ��д��1�������*/

//********************************************************************************************************************//
//                                        FUNCTION's PROTOTYPES                                                       //
//********************************************************************************************************************//
//SI24R1 API Functions
void SI24R1_Init(void); 	//SI24R1 Pin Init
u8 SI2R4_Check(void);
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t value);
uint8_t SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes);
uint8_t SI24R1_Read_Reg(uint8_t reg);
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);

void SI24R1_Mode_NOACK(uint8_t Mode);
uint8_t SI24R1_RxPacket(uint8_t *rxbuf);
uint8_t SI24R1_TxPacket(uint8_t *txbuf);
void data_transfer(u8 change_num,u8 mode,double data_1,u16 data_2,u16 data_3);

void SI24R1_Shutdown(void);
void SI24R1_Standby(void);

/*made by ���S */
void test_tx(void);


//********************************************************************************************************************//
#endif /*__bsp_si24r1_H*/

