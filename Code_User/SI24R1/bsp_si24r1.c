#include "bsp_si24r1.h"
#include "led.h"
#include "command.h"
#include "spi.h"

static __IO uint32_t  SPITimeout = SPIT_FLAG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

//uint8_t static TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};  // 定义一个静态发送地址

// 2G4通信地址
//------------------------------------------------------------------------------------------		
const u8 TX_ADDRESS[5] = {0x34,0x43,0x10,0x10,0x01 } ;		
const u8 RX_ADDRESS[5] = {0x34,0x43,0x10,0x10,0x01 } ;		
//------------------------------------------------------------------------------------------

u8 SI24R1_Send_Data[32] = { 0 };		// 2.4G发射数据缓存数组
			 
u8 SI24R1_Receive_Data[32] = { 0 };	// 2.4G接收数据缓存数组


static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t date;
	SPITimeout = SPIT_FLAG_TIMEOUT;
    /* 等待发送缓冲区为空，TXE事件 */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
    }
	SPI_I2S_SendData(SI24R1_SPI, byte);
	/* 等待接收缓冲区为空，RXNE事件 */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
    }
	date = SPI_I2S_ReceiveData(SI24R1_SPI);
	/* 等待发送缓冲区为空，TXE事件 */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
    }
	/* 等待发送完，BSY事件 */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_BSY) == SET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(4);
    }
	return date;
}

/********************************************************
函数功能：SI24R1自检
入口参数：无
返回  值：无
*********************************************************/
u8 SI2R4_Check(void)
{
	u8 Array_Test[5]={0X66,0X66,0X66,0X66,0X66};
	u8 C_Test = 0;
	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 			// SPI速度为9Mhz（SI24R1的最大SPI时钟为10Mhz）
	
	SI24R1_Write_Buf(W_REGISTER+TX_ADDR,Array_Test,5);	// 写入5个字节的地址.	
	
	SI24R1_Read_Buf(TX_ADDR,Array_Test,5); 				// 读出写入的地址  
	
	for(; C_Test<5; C_Test++)
	if(Array_Test[C_Test]!=0X66)
		break;
	
	if(C_Test!=5)return 1;		// SI24R1错误	
	
	return 0;		 			// SI24R1正确
}

/********************************************************
函数功能：SI24R1引脚初始化
入口参数：无
返回  值：无
*********************************************************/
void SI24R1_Init(void)
{
    // GPIO配置结构体初始化
    GPIO_InitTypeDef  GPIO_InitStructure;
    // SPI配置结构体初始化
    SPI_InitTypeDef  SPI1_InitStructure;
    
    // 使能GPIO端口时钟
    RCC_APB2PeriphClockCmd(SI24R1_GPIO_RCC, ENABLE);		// 使能GPIO端口时钟
	//RCC_APB2PeriphClockCmd(SI24R1_SPI_RCC, ENABLE);			// 使能SPI端口时钟
	RCC_APB1PeriphClockCmd(SI24R1_SPI_RCC,ENABLE);			// 使能SPI端口时钟

    // 配置MOSI、SCK、MISO引脚为复用输出模式
    GPIO_InitStructure.GPIO_Pin = SI24R1_MOSI|SI24R1_SCK|SI24R1_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			// 复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// 速度10MHz
    GPIO_Init(SI24R1_GPIOA, &GPIO_InitStructure);			// 初始化GPIOA

    // 配置CSN、CE引脚为推挽输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// 推挽输出
    GPIO_InitStructure.GPIO_Pin = SI24R1_CSN|SI24R1_CE;
    GPIO_Init(SI24R1_GPIOA, &GPIO_InitStructure);	  		// 初始化GPIOA

    // 配置IRQ引脚为浮空输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// 浮空输入
    GPIO_InitStructure.GPIO_Pin = SI24R1_IRQ;
    GPIO_Init(SI24R1_GPIOB, &GPIO_InitStructure);	  		// 初始化GPIOB

    // 配置SPI通信参数
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI1_InitStructure.SPI_CRCPolynomial = 7;
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SI24R1_SPI, &SPI1_InitStructure);				// 初始化SPI
	SPI_Cmd(SI24R1_SPI, ENABLE);								// 使能SPI

    // SPI时钟线拉低 SCK = 0
    SI24R1_SCK_Clr();	
    // CSN = 1
    SI24R1_CSN_Set();	
    // CE = 0
    SI24R1_CE_Clr();	
    // MOSI = 0
    SI24R1_MOSI_Clr();	
}

/********************************************************
函数功能：写寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
		 value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    // 发送CSN低电平信号，准备写入操作
    SI24R1_CSN_Clr();	//CSN = 0
    // 向SPI接口写入寄存器地址
    status = SPI_RW(reg);
    // 向SPI接口写入要写入的值
    SPI_RW(value);
    // 发送CSN高电平信号，结束写入操作
    SI24R1_CSN_Set();	//CSN = 1
    // 返回写入操作的状态
    return(status);
}

/********************************************************
函数功能：写寄存器的值（多字节）
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
		 pBuf:写数据首地址
		 bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    // 将CSN引脚置低电平
    SI24R1_CSN_Clr();	//CSN = 0

    // 向SPI写入寄存器地址
    status = SPI_RW(reg);

    // 写入数据
    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        SPI_RW(*pBuf++);

    // 将CSN引脚置高电平
    SI24R1_CSN_Set();	//CSN = 1

    return(status);
}

/********************************************************
函数功能：读取寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
uint8_t SI24R1_Read_Reg(uint8_t reg)
{
    uint8_t value;

    // CSN = 0
    SI24R1_CSN_Clr();	//CSN = 0

    // 发送寄存器地址
    SPI_RW(reg);

    // 读取寄存器的值
    value = SPI_RW(0);

    // CSN = 1
    SI24R1_CSN_Set();	//CSN = 1

    return(value);
}

/********************************************************
函数功能：读取寄存器的值（多字节）
入口参数：reg:寄存器映射地址（READ_REG｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    // CSN = 0
    SI24R1_CSN_Clr();	//CSN = 0
    status = SPI_RW(reg);

    // 循环读取数据，低字节在前
    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        pBuf[byte_ctr] = SPI_RW(0);	//读取数据，低字节在前

    // CSN = 1
    SI24R1_CSN_Set();	//CSN = 1
    return(status);
}

/********************************************************
函数功能：SI24R1接收模式初始化
入口参数：SI24R1_InitTypeDef
		 Mode RX_MODE or TX_MODE
返回  值：无
*********************************************************/
void SI24R1_Mode_NOACK(uint8_t Mode)
{
    SI24R1_InitTypeDef SI24R1_InitStruct;
	SI24R1_InitStruct.ADR_WIDTH = 0x03;			// 5位接收地址
    SI24R1_InitStruct.RX_ADDR_Px = RX_ADDR_P0;	// 接收通道0
    SI24R1_InitStruct.EN_RXADDR_Px = 0x01;		// 使能接收通道0
    SI24R1_InitStruct.RF_CHANNEL = 40;			// 射频频道选择2440MHz
    SI24R1_InitStruct.RX_PW_Px = RX_PW_P0;		// 选择设置接收通道0负载数据宽度
    SI24R1_InitStruct.RF_SET = 0x0f;			// 传输速率功率设置，2Mbps 7dbm
    SI24R1_InitStruct.RF_FEATURE = 0x01;		// 使能NOACK
	
	//CE = 0
    SI24R1_CE_Clr();
    // 设置地址宽度为5bytes
    SI24R1_Write_Reg(W_REGISTER + SETUP_AW, SI24R1_InitStruct.ADR_WIDTH);
	if(Mode == RX_MODE)
	{
	// 接收使用接收通道0和接收地址
    SI24R1_Write_Buf(W_REGISTER + SI24R1_InitStruct.RX_ADDR_Px, TX_ADDRESS, TX_ADR_WIDTH);
	// 使能接收通道0
    SI24R1_Write_Reg(W_REGISTER + EN_RXADDR, SI24R1_InitStruct.EN_RXADDR_Px);
	// 设置接收通道0负载数据宽度
    SI24R1_Write_Reg(W_REGISTER + SI24R1_InitStruct.RX_PW_Px, TX_PLOAD_WIDTH);
	}
	else
	{
	// 写入发送地址
    SI24R1_Write_Buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	// 使能NOACK	
    SI24R1_Write_Reg(W_REGISTER + FEATURE, SI24R1_InitStruct.RF_FEATURE);//
	}

    // 选择射频通道40
    SI24R1_Write_Reg(W_REGISTER + RF_CH, SI24R1_InitStruct.RF_CHANNEL);
    // 数据传输率2Mbps，发射功率7dBm
    SI24R1_Write_Reg(W_REGISTER + RF_SETUP, SI24R1_InitStruct.RF_SET);

	if(Mode == RX_MODE)
	// CRC使能，16位CRC校验，上电，接收模式
    SI24R1_Write_Reg(W_REGISTER + CONFIG, (SI24R1_InitStruct.SI24R1_CONFIG)|0x01);
	else
	// CRC使能，16位CRC校验，上电，发射模式
    SI24R1_Write_Reg(W_REGISTER + CONFIG, (SI24R1_InitStruct.SI24R1_CONFIG)&0xfe);
    // 拉高CE启动接收设备
    SI24R1_CE_Set();
}


/********************************************************
函数功能：读取接收数据
入口参数：rxbuf:接收数据存放首地址
返回  值：1:接收到数据
         0:没有接收到数据
*********************************************************/
uint8_t SI24R1_RxPacket(uint8_t *rxbuf)
{
    uint8_t state;
    uint8_t pload_width;
    state = SI24R1_Read_Reg(STATUS);  			      	// 读取状态寄存器的值
    SI24R1_Write_Reg(W_REGISTER + STATUS,state);        // 清除RX_DR中断标志
    if(state & RX_DR)								    // 接收到数据
    {
        pload_width = SI24R1_Read_Reg(R_RX_PL_WID);		// 读取收到的数据字节数
		#if SI24R1_debug
			printf("\nSI24R1 RX:0x%X\n",state);
			printf("\nSI24R1 Readpload_width：%d\n",pload_width);
		#endif
        SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,pload_width);	// 读取数据
        SI24R1_Write_Reg(FLUSH_RX,0xff);				// 清除RX FIFO寄存器
        return 1;
    }
    return 0;                                         	// 没收到任何数据
}

/********************************************************
函数功能：发送一个数据包
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败
         0x20:发送成功
         0xff:发送失败
*********************************************************/
uint8_t SI24R1_TxPacket(uint8_t *txbuf)
{
    uint8_t state;
    SI24R1_CE_Clr();										// CE拉低，使能SI24R1配置
    SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	// 写数据到TX FIFO,32个字节
    SI24R1_CE_Set();										// CE置高，使能发送
    SPITimeout = SPIT_FLAG_TIMEOUT;
    while(GPIO_ReadInputDataBit(SI24R1_GPIOB, SI24R1_IRQ))	// 等待发送完成
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
    }
    state = SI24R1_Read_Reg(STATUS);  						// 读取状态寄存器的值
    SI24R1_Write_Reg(W_REGISTER + STATUS, state); 			// 清除TX_DS或MAX_RT中断标志
    if(state & MAX_RT)										// 达到最大重发次数
    {
        SI24R1_Write_Reg(FLUSH_TX,0xff);					// 清除TX FIFO寄存器
#if SI24R1_debug
        printf("\nSI24R1 TX MAX:0x%X\n",MAX_RT);
#endif
        return MAX_RT;
    }
    if(state & TX_DS)										// 发送完成
    {
#if SI24R1_debug
        printf("\nSI24R1 TX DS:0x%X\n",TX_DS);
#endif
        return TX_DS;
    }
#if SI24R1_debug
    printf("\nSI24R1 TX state:0x%X\n",state);
#endif
    return state;											//发送失败
}

/**
 * @brief 关闭 SI24R1 模块
 *
 * 该函数通过向 SI24R1 的 CONFIG 寄存器写入特定值来关闭 SI24R1 模块。
 */
void SI24R1_Shutdown(void)
{
	SI24R1_Write_Reg(W_REGISTER + CONFIG, 0x0d);
}

/**
 * @brief 将SI24R1模块置于待机模式
 *
 * 将SI24R1模块的CONFIG寄存器设置为0x0f，使其进入待机模式。
 */
void SI24R1_Standby(void)
{
	SI24R1_Write_Reg(W_REGISTER + CONFIG, 0x0f);
}

/**
 * @brief SPI超时后的用户回调函数
 *
 * 当SPI通信等待超时后，会调用此函数处理超时情况。
 *
 * @param errorCode 错误代码，表示超时的原因。
 *
 * @return 返回值总是0，表示函数执行成功。
 */
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* 等待超时后的处理,输出错误信息 */
    //SI24R1_ERROR("SI24R1 等待超时!errorCode = %d",errorCode);
    return 0;
}

/**
 * @brief 处理数据函数
 *
 * 根据给定的原始数据origin，将其分解为两个字节，分别存储在data1和data2中。
 *
 * @param data1 指向存储高位数据的指针
 * @param data2 指向存储低位数据的指针
 * @param origin 原始数据，类型为u16
 */
void data_deal(u8* data1,u8* data2,u16 origin)
{
    if (origin<100)
    {
        *data1=0;//高位
        *data2=origin;//低位
    }
    else if(origin<10000)
    {
        *data1=origin/100;//高位
        *data2=origin-100*(*data1);//低位
    }
    else
    {
        *data1 = (origin / 100) % 100; // 高位
        *data2 = origin % 100; // 低位
    }
}

/**
 * @brief 处理数据，将原始数据转换为两个u8类型的数据
 *
 * 该函数将输入的double类型原始数据origin转换为两个u8类型的数据，存储在data1和data2中。
 *
 * @param data1 指向存储第一个u8类型数据的指针
 * @param data2 指向存储第二个u8类型数据的指针
 * @param origin 需要转换的原始double类型数据
 */
void double_data_deal(u8* data1,u8* data2,double origin)
{
		*data1=(u8)origin;
		*data2=(u8)((origin-*data1)*100);
}

/**
 * @brief 数据传输函数
 *
 * 该函数用于在不同的模式下传输数据。
 *
 * @param change_num 要更改或传输的数据的序号
 * @param mode 传输模式（主机发送模式为1，从机发送模式为0）
 * @param data_1 需要传输的第一个双精度浮点数数据
 * @param data_2 需要传输的第二个无符号16位整数数据
 * @param data_3 需要传输的第三个无符号16位整数数据
 */
void data_transfer(u8 change_num,u8 mode,double data_1,u16 data_2,u16 data_3)
{
	static u8 t_flag=0;
	u8 cnt=0;
	u8 i=4;
	SI24R1_Send_Data[0]=13;
	SI24R1_Send_Data[1]=16;//主机序号，和其他车分别开
	
	SI24R1_Send_Data[2]=change_num;//更改参数的序号/需要从机发送的参数的序号
	
	SI24R1_Send_Data[3]=mode;//主机发送模式为1，从机发送模式为0
	
	if(mode==1)
	{
				SI24R1_Mode_NOACK(TX_MODE);//发送模式

				double_data_deal(&SI24R1_Send_Data[4],&SI24R1_Send_Data[5],data_1);
				data_deal(&SI24R1_Send_Data[6],&SI24R1_Send_Data[7],data_2);
				data_deal(&SI24R1_Send_Data[8],&SI24R1_Send_Data[9],data_3);//将要发送的数据填入6个发送单位中

				if(SI24R1_TxPacket(SI24R1_Send_Data) == 0x20)//发送数据包，绿灯翻转
				LED_Green=!LED_Green;

				/*for (i = 4; i < 10; i++)
				{
						SI24R1_Send_Data[i]=0;  //发送完后数据清0
				}*/
	}
    else
    {
        if(!t_flag)SI24R1_Mode_NOACK(TX_MODE);
				
				if(t_flag==0&&SI24R1_TxPacket(SI24R1_Send_Data)==0X20)//顺利发出数据
				{
					LED_Green=!LED_Green;
					
					SI24R1_Mode_NOACK(RX_MODE);
					t_flag=1;//转为接受模式
				}
				
				if(t_flag&&SI24R1_RxPacket(SI24R1_Receive_Data))//顺利接受从机数据
				{
				
					/*while(!SI24R1_RxPacket(SI24R1_Receive_Data))
					{
						cnt++;
						if(cnt>10)
						{
							break;
							cnt=0;
						}
					}*/
					SI24R1_Send_Data[0]=13;
					SI24R1_Send_Data[1]=16;
					SI24R1_Send_Data[2]=change_num;
					SI24R1_Send_Data[3]=0;
					double_data_deal(&SI24R1_Send_Data[4],&SI24R1_Send_Data[5],data_1);//参数用，整数+小数
					data_deal(&SI24R1_Send_Data[6],&SI24R1_Send_Data[7],data_2);//波形及遥控用，整数高两位+整数低两位
					data_deal(&SI24R1_Send_Data[8],&SI24R1_Send_Data[9],data_3);//波形及遥控用，整数高两位+整数低两位
				
					LED_Blue=!LED_Blue;
					t_flag=0;//顺利接收到数据，标志位置为0
				}
    }
    
}
uint8_t tx_flag=0x00;
/*made by 燊楽*/
void test_tx(void)
{

	LCD_ShowIntNum(0, 16 * (2 + 2), tx_flag+0x50, 3, BLACK, WHITE, 16);
    //SI24R1_Mode_NOACK(TX_MODE); // 设置SI24R1为无应答接收模式
    SI24R1_Send_Data[0]=1;
    SI24R1_Send_Data[1]=1;
    SI24R1_Send_Data[2]=1;
    SI24R1_Send_Data[3]=1;
    SI24R1_Send_Data[4]=1;
    SI24R1_Send_Data[5]=1;
    SI24R1_Send_Data[6]=1;
    SI24R1_Send_Data[7]=1;
    SI24R1_Send_Data[8]=1;
    SI24R1_Send_Data[9]=1;
    SI24R1_Send_Data[10]=1;
    SI24R1_Send_Data[11]=1;
    SI24R1_Send_Data[12]=1;
    SI24R1_Send_Data[13]=1;
    SI24R1_Send_Data[14]=1;
    SI24R1_Send_Data[15]=1;
    SI24R1_Send_Data[16]=1;
    SI24R1_Send_Data[17]=1;
    SI24R1_Send_Data[18]=1;
    SI24R1_Send_Data[19]=1;
    SI24R1_Send_Data[20]=1;
    SI24R1_Send_Data[21]=1;
    SI24R1_Send_Data[22]=1;
    SI24R1_Send_Data[23]=1;
    SI24R1_Send_Data[24]=1;
    SI24R1_Send_Data[25]=1;
    SI24R1_Send_Data[26]=1;
    SI24R1_Send_Data[27]=1;
    SI24R1_Send_Data[28]=1;
    SI24R1_Send_Data[29]=1;
    SI24R1_Send_Data[30]=1;
    SI24R1_Send_Data[31]=1;
    tx_flag=SI24R1_TxPacket(SI24R1_Send_Data);
	LCD_ShowIntNum(0, 16 * (2 + 2), tx_flag+0x60, 3, BLACK, WHITE, 16);
}