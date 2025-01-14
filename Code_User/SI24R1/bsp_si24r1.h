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

extern u8 SI24R1_Send_Data[32];		// 2.4G发射数据缓存数组
			 
extern u8 SI24R1_Receive_Data[32];	// 2.4G接收数据缓存数组

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

#define TX_ADR_WIDTH   	5  		// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 	32  	// 数据通道有效数据宽度
#define RX_MODE 0
#define TX_MODE 1

//********************************************************************************************************************//
// SPI(SI24R1) 命令
#define R_REGISTER		0x00  // 定义读取寄存器命令 000A AAAA
#define W_REGISTER     	0x20  // 定义写入寄存器命令 001A AAAA
#define RD_RX_PLOAD     0x61  // 定义接收数据负载寄存器地址
#define WR_TX_PLOAD     0xA0  // 定义发送数据负载寄存器地址
#define FLUSH_TX        0xE1  // 定义清空发送寄存器命令
#define FLUSH_RX        0xE2  // 定义清空接收寄存器命令
#define REUSE_TX_PL     0xE3  // 定义重用发送数据负载寄存器命令
#define R_RX_PL_WID		0x60  // 定义接收到的数据字节数命令
#define NOP             0xFF  // 定义无操作，可能用于读取状态寄存器

//********************************************************************************************************************//
// SPI(SI24R1) 寄存器（地址）
#define CONFIG          0x00  // '配置' 寄存器地址
#define EN_AA           0x01  // '启用自动应答' 寄存器地址
#define EN_RXADDR       0x02  // '启用接收地址' 寄存器地址
#define SETUP_AW        0x03  // '设置地址宽度' 寄存器地址
#define SETUP_RETR      0x04  // '设置自动重传' 寄存器地址
#define RF_CH           0x05  // '射频通道' 寄存器地址
#define RF_SETUP        0x06  // '射频设置' 寄存器地址
#define STATUS          0x07  // '状态' 寄存器地址
#define OBSERVE_TX      0x08  // '观察发送' 寄存器地址
#define RSSI            0x09  // '接收信号强度指示器' 寄存器地址
#define RX_ADDR_P0      0x0A  // '接收地址管道0' 寄存器地址
#define RX_ADDR_P1      0x0B  // '接收地址管道1' 寄存器地址
#define RX_ADDR_P2      0x0C  // '接收地址管道2' 寄存器地址
#define RX_ADDR_P3      0x0D  // '接收地址管道3' 寄存器地址
#define RX_ADDR_P4      0x0E  // '接收地址管道4' 寄存器地址
#define RX_ADDR_P5      0x0F  // '接收地址管道5' 寄存器地址
#define TX_ADDR         0x10  // '发送地址' 寄存器地址
#define RX_PW_P0        0x11  // '接收数据负载宽度，管道0' 寄存器地址
#define RX_PW_P1        0x12  // '接收数据负载宽度，管道1' 寄存器地址
#define RX_PW_P2        0x13  // '接收数据负载宽度，管道2' 寄存器地址
#define RX_PW_P3        0x14  // '接收数据负载宽度，管道3' 寄存器地址
#define RX_PW_P4        0x15  // '接收数据负载宽度，管道4' 寄存器地址
#define RX_PW_P5        0x16  // '接收数据负载宽度，管道5' 寄存器地址
#define FIFO_STATUS     0x17  // 'FIFO 状态寄存器' 寄存器地址
#define DYNPD			0x1C  // 使能动态负载长度
#define FEATURE			0x1D  // 特征寄存器

//********************************************************************************************************************//
// STATUS Register
#define RX_DR			0x40  /* RX FIFO有值标志位，写’1’清除。*/
#define TX_DS			0x20  /* 发射端发射完成中断位，如果是ACK模式，
								 则收到ACK确认信号后TX_DS位置’1’，写’1’清除。*/
#define MAX_RT			0x10  /* 达到最大重发次数中断位，写’1’清除。*/

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

/*made by 燊楽 */
void test_tx(void);


//********************************************************************************************************************//
#endif /*__bsp_si24r1_H*/

