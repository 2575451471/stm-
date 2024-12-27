#include "spi.h"

// SPI2初始化
//---------------------------------------------------------------------------------------------------------------
/**
 * @brief 初始化SPI2接口
 *
 * 该函数用于初始化STM32微控制器的SPI2接口，并将其配置为双线双向全双工模式，
 * 作为SPI主机，发送和接收8位帧结构的数据，时钟悬空低，数据捕获于第1个时钟沿，
 * NSS信号由软件控制，波特率预分频值为16，数据传输从MSB位开始，CRC值计算的多项式为7。
 */
void SPI2_Init_JX(void)
{
    // 定义SPI初始化结构体
    SPI_InitTypeDef  SPI_InitStructure;

    // 使能SPI2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    // SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // SPI设置为双线双向全双工
    // SPI主机
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// SPI主机
    // 发送接收8位帧结构
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// 发送接收8位帧结构
    // 时钟悬空低
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// 时钟悬空低
    // 数据捕获于第1个时钟沿
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						// 数据捕获于第1个时钟沿
    // NSS信号由软件控制
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// NSS信号由软件控制
    // 定义波特率预分频的值:波特率预分频值为16
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	// 定义波特率预分频的值:波特率预分频值为16
    // 数据传输从MSB位开始
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// 数据传输从MSB位开始
    // CRC值计算的多项式
    SPI_InitStructure.SPI_CRCPolynomial = 7;							// CRC值计算的多项式

    // 初始化SPI2
    SPI_Init(SPI2, &SPI_InitStructure);

    // 使能SPI外设
    SPI_Cmd(SPI2, ENABLE); 		// 使能SPI外设

    // 启动传输
    //SPI2_ReadWriteByte(0xFF);	// 启动传输		 
}   
//---------------------------------------------------------------------------------------------------------------


// 设置SPI速度
//---------------------------------------------------
// SPI_DivideFrequency_2	2分频： 18MHz
// SPI_DivideFrequency_4	4分频： 9MHz
// SPI_DivideFrequency_8	8分频： 4.5MHz
// SPI_DivideFrequency_16	16分频：2.25MHz
/**
 * @brief 设置SPI2的速度
 *
 * 通过修改SPI2的CR1寄存器的相关位来设置SPI2的速度。
 *
 * @param SPI_DivideFrequency SPI分频值，用于设置SPI2的速度。
 */
void SPI2_SetSpeed(u8 SPI_DivideFrequency)
{
	// 清除SPI2的CR1寄存器的相关位，为设置新速度做准备
	SPI2->CR1 &= 0XFFC7;	// 清除CR1寄存器的相关位

	// 设置SPI2的速度
	SPI2->CR1 |= SPI_DivideFrequency;	//设置SPI2速度 

	// 启用SPI2（已被注释掉）
	//SPI_Cmd(SPI2,ENABLE); 
} 
//---------------------------------------------------


// SPI读写一个字节
// TxData:要写入的字节
// 返回值:读取到的字节
//-----------------------------------------------------------------------
/**
 * @brief 通过SPI2接口读写一个字节的数据
 *
 * 该函数通过SPI2接口发送一个字节的数据，并接收返回的字节数据。
 *
 * @param TxData 要发送的数据字节
 *
 * @return 返回接收到的数据字节
 *         如果发送或接收超时（超过250次循环），则返回0表示失败
 */
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 TxWait = 0;
	u8 RxWait = 0;
	
	// 等待发送缓存为空
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		TxWait++; // 等待发送缓存为空计数
		if(TxWait>250)	// 等待时间过长则放弃本次读写
			return 0;
	}	

	// SPI2写一个字节
	SPI_I2S_SendData(SPI2, TxData); // SPI2写一个字节
	
	// 等待接收缓存为空
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		RxWait++; // 等待接收缓存为空计数
		if(RxWait>250)	// 等待时间过长则放弃本次读写
			return 0;
	}	  	
	
	// 将读到的字节返回
	return SPI_I2S_ReceiveData(SPI2); // 将读到的字节返回					    
}
//-----------------------------------------------------------------------
