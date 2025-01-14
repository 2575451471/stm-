#include "bsp_si24r1.h"
#include "led.h"
#include "command.h"
#include "spi.h"

static __IO uint32_t  SPITimeout = SPIT_FLAG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

//uint8_t static TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};  // ����һ����̬���͵�ַ

// 2G4ͨ�ŵ�ַ
//------------------------------------------------------------------------------------------		
const u8 TX_ADDRESS[5] = {0x34,0x43,0x10,0x10,0x01 } ;		
const u8 RX_ADDRESS[5] = {0x34,0x43,0x10,0x10,0x01 } ;		
//------------------------------------------------------------------------------------------

u8 SI24R1_Send_Data[32] = { 0 };		// 2.4G�������ݻ�������
			 
u8 SI24R1_Receive_Data[32] = { 0 };	// 2.4G�������ݻ�������


static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t date;
	SPITimeout = SPIT_FLAG_TIMEOUT;
    /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
    }
	SPI_I2S_SendData(SI24R1_SPI, byte);
	/* �ȴ����ջ�����Ϊ�գ�RXNE�¼� */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
    }
	date = SPI_I2S_ReceiveData(SI24R1_SPI);
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
    }
	/* �ȴ������꣬BSY�¼� */
    while (SPI_I2S_GetFlagStatus(SI24R1_SPI, SPI_I2S_FLAG_BSY) == SET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(4);
    }
	return date;
}

/********************************************************
�������ܣ�SI24R1�Լ�
��ڲ�������
����  ֵ����
*********************************************************/
u8 SI2R4_Check(void)
{
	u8 Array_Test[5]={0X66,0X66,0X66,0X66,0X66};
	u8 C_Test = 0;
	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 			// SPI�ٶ�Ϊ9Mhz��SI24R1�����SPIʱ��Ϊ10Mhz��
	
	SI24R1_Write_Buf(W_REGISTER+TX_ADDR,Array_Test,5);	// д��5���ֽڵĵ�ַ.	
	
	SI24R1_Read_Buf(TX_ADDR,Array_Test,5); 				// ����д��ĵ�ַ  
	
	for(; C_Test<5; C_Test++)
	if(Array_Test[C_Test]!=0X66)
		break;
	
	if(C_Test!=5)return 1;		// SI24R1����	
	
	return 0;		 			// SI24R1��ȷ
}

/********************************************************
�������ܣ�SI24R1���ų�ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void SI24R1_Init(void)
{
    // GPIO���ýṹ���ʼ��
    GPIO_InitTypeDef  GPIO_InitStructure;
    // SPI���ýṹ���ʼ��
    SPI_InitTypeDef  SPI1_InitStructure;
    
    // ʹ��GPIO�˿�ʱ��
    RCC_APB2PeriphClockCmd(SI24R1_GPIO_RCC, ENABLE);		// ʹ��GPIO�˿�ʱ��
	//RCC_APB2PeriphClockCmd(SI24R1_SPI_RCC, ENABLE);			// ʹ��SPI�˿�ʱ��
	RCC_APB1PeriphClockCmd(SI24R1_SPI_RCC,ENABLE);			// ʹ��SPI�˿�ʱ��

    // ����MOSI��SCK��MISO����Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = SI24R1_MOSI|SI24R1_SCK|SI24R1_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			// �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// �ٶ�10MHz
    GPIO_Init(SI24R1_GPIOA, &GPIO_InitStructure);			// ��ʼ��GPIOA

    // ����CSN��CE����Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// �������
    GPIO_InitStructure.GPIO_Pin = SI24R1_CSN|SI24R1_CE;
    GPIO_Init(SI24R1_GPIOA, &GPIO_InitStructure);	  		// ��ʼ��GPIOA

    // ����IRQ����Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// ��������
    GPIO_InitStructure.GPIO_Pin = SI24R1_IRQ;
    GPIO_Init(SI24R1_GPIOB, &GPIO_InitStructure);	  		// ��ʼ��GPIOB

    // ����SPIͨ�Ų���
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI1_InitStructure.SPI_CRCPolynomial = 7;
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SI24R1_SPI, &SPI1_InitStructure);				// ��ʼ��SPI
	SPI_Cmd(SI24R1_SPI, ENABLE);								// ʹ��SPI

    // SPIʱ�������� SCK = 0
    SI24R1_SCK_Clr();	
    // CSN = 1
    SI24R1_CSN_Set();	
    // CE = 0
    SI24R1_CE_Clr();	
    // MOSI = 0
    SI24R1_MOSI_Clr();	
}

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
		 value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    // ����CSN�͵�ƽ�źţ�׼��д�����
    SI24R1_CSN_Clr();	//CSN = 0
    // ��SPI�ӿ�д��Ĵ�����ַ
    status = SPI_RW(reg);
    // ��SPI�ӿ�д��Ҫд���ֵ
    SPI_RW(value);
    // ����CSN�ߵ�ƽ�źţ�����д�����
    SI24R1_CSN_Set();	//CSN = 1
    // ����д�������״̬
    return(status);
}

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
		 pBuf:д�����׵�ַ
		 bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    // ��CSN�����õ͵�ƽ
    SI24R1_CSN_Clr();	//CSN = 0

    // ��SPIд��Ĵ�����ַ
    status = SPI_RW(reg);

    // д������
    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        SPI_RW(*pBuf++);

    // ��CSN�����øߵ�ƽ
    SI24R1_CSN_Set();	//CSN = 1

    return(status);
}

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
uint8_t SI24R1_Read_Reg(uint8_t reg)
{
    uint8_t value;

    // CSN = 0
    SI24R1_CSN_Clr();	//CSN = 0

    // ���ͼĴ�����ַ
    SPI_RW(reg);

    // ��ȡ�Ĵ�����ֵ
    value = SPI_RW(0);

    // CSN = 1
    SI24R1_CSN_Set();	//CSN = 1

    return(value);
}

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ��READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    // CSN = 0
    SI24R1_CSN_Clr();	//CSN = 0
    status = SPI_RW(reg);

    // ѭ����ȡ���ݣ����ֽ���ǰ
    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        pBuf[byte_ctr] = SPI_RW(0);	//��ȡ���ݣ����ֽ���ǰ

    // CSN = 1
    SI24R1_CSN_Set();	//CSN = 1
    return(status);
}

/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��
��ڲ�����SI24R1_InitTypeDef
		 Mode RX_MODE or TX_MODE
����  ֵ����
*********************************************************/
void SI24R1_Mode_NOACK(uint8_t Mode)
{
    SI24R1_InitTypeDef SI24R1_InitStruct;
	SI24R1_InitStruct.ADR_WIDTH = 0x03;			// 5λ���յ�ַ
    SI24R1_InitStruct.RX_ADDR_Px = RX_ADDR_P0;	// ����ͨ��0
    SI24R1_InitStruct.EN_RXADDR_Px = 0x01;		// ʹ�ܽ���ͨ��0
    SI24R1_InitStruct.RF_CHANNEL = 40;			// ��ƵƵ��ѡ��2440MHz
    SI24R1_InitStruct.RX_PW_Px = RX_PW_P0;		// ѡ�����ý���ͨ��0�������ݿ��
    SI24R1_InitStruct.RF_SET = 0x0f;			// �������ʹ������ã�2Mbps 7dbm
    SI24R1_InitStruct.RF_FEATURE = 0x01;		// ʹ��NOACK
	
	//CE = 0
    SI24R1_CE_Clr();
    // ���õ�ַ���Ϊ5bytes
    SI24R1_Write_Reg(W_REGISTER + SETUP_AW, SI24R1_InitStruct.ADR_WIDTH);
	if(Mode == RX_MODE)
	{
	// ����ʹ�ý���ͨ��0�ͽ��յ�ַ
    SI24R1_Write_Buf(W_REGISTER + SI24R1_InitStruct.RX_ADDR_Px, TX_ADDRESS, TX_ADR_WIDTH);
	// ʹ�ܽ���ͨ��0
    SI24R1_Write_Reg(W_REGISTER + EN_RXADDR, SI24R1_InitStruct.EN_RXADDR_Px);
	// ���ý���ͨ��0�������ݿ��
    SI24R1_Write_Reg(W_REGISTER + SI24R1_InitStruct.RX_PW_Px, TX_PLOAD_WIDTH);
	}
	else
	{
	// д�뷢�͵�ַ
    SI24R1_Write_Buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	// ʹ��NOACK	
    SI24R1_Write_Reg(W_REGISTER + FEATURE, SI24R1_InitStruct.RF_FEATURE);//
	}

    // ѡ����Ƶͨ��40
    SI24R1_Write_Reg(W_REGISTER + RF_CH, SI24R1_InitStruct.RF_CHANNEL);
    // ���ݴ�����2Mbps�����书��7dBm
    SI24R1_Write_Reg(W_REGISTER + RF_SETUP, SI24R1_InitStruct.RF_SET);

	if(Mode == RX_MODE)
	// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
    SI24R1_Write_Reg(W_REGISTER + CONFIG, (SI24R1_InitStruct.SI24R1_CONFIG)|0x01);
	else
	// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
    SI24R1_Write_Reg(W_REGISTER + CONFIG, (SI24R1_InitStruct.SI24R1_CONFIG)&0xfe);
    // ����CE���������豸
    SI24R1_CE_Set();
}


/********************************************************
�������ܣ���ȡ��������
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��1:���յ�����
         0:û�н��յ�����
*********************************************************/
uint8_t SI24R1_RxPacket(uint8_t *rxbuf)
{
    uint8_t state;
    uint8_t pload_width;
    state = SI24R1_Read_Reg(STATUS);  			      	// ��ȡ״̬�Ĵ�����ֵ
    SI24R1_Write_Reg(W_REGISTER + STATUS,state);        // ���RX_DR�жϱ�־
    if(state & RX_DR)								    // ���յ�����
    {
        pload_width = SI24R1_Read_Reg(R_RX_PL_WID);		// ��ȡ�յ��������ֽ���
		#if SI24R1_debug
			printf("\nSI24R1 RX:0x%X\n",state);
			printf("\nSI24R1 Readpload_width��%d\n",pload_width);
		#endif
        SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,pload_width);	// ��ȡ����
        SI24R1_Write_Reg(FLUSH_RX,0xff);				// ���RX FIFO�Ĵ���
        return 1;
    }
    return 0;                                         	// û�յ��κ�����
}

/********************************************************
�������ܣ�����һ�����ݰ�
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ��
         0x20:���ͳɹ�
         0xff:����ʧ��
*********************************************************/
uint8_t SI24R1_TxPacket(uint8_t *txbuf)
{
    uint8_t state;
    SI24R1_CE_Clr();										// CE���ͣ�ʹ��SI24R1����
    SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	// д���ݵ�TX FIFO,32���ֽ�
    SI24R1_CE_Set();										// CE�øߣ�ʹ�ܷ���
    SPITimeout = SPIT_FLAG_TIMEOUT;
    while(GPIO_ReadInputDataBit(SI24R1_GPIOB, SI24R1_IRQ))	// �ȴ��������
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
    }
    state = SI24R1_Read_Reg(STATUS);  						// ��ȡ״̬�Ĵ�����ֵ
    SI24R1_Write_Reg(W_REGISTER + STATUS, state); 			// ���TX_DS��MAX_RT�жϱ�־
    if(state & MAX_RT)										// �ﵽ����ط�����
    {
        SI24R1_Write_Reg(FLUSH_TX,0xff);					// ���TX FIFO�Ĵ���
#if SI24R1_debug
        printf("\nSI24R1 TX MAX:0x%X\n",MAX_RT);
#endif
        return MAX_RT;
    }
    if(state & TX_DS)										// �������
    {
#if SI24R1_debug
        printf("\nSI24R1 TX DS:0x%X\n",TX_DS);
#endif
        return TX_DS;
    }
#if SI24R1_debug
    printf("\nSI24R1 TX state:0x%X\n",state);
#endif
    return state;											//����ʧ��
}

/**
 * @brief �ر� SI24R1 ģ��
 *
 * �ú���ͨ���� SI24R1 �� CONFIG �Ĵ���д���ض�ֵ���ر� SI24R1 ģ�顣
 */
void SI24R1_Shutdown(void)
{
	SI24R1_Write_Reg(W_REGISTER + CONFIG, 0x0d);
}

/**
 * @brief ��SI24R1ģ�����ڴ���ģʽ
 *
 * ��SI24R1ģ���CONFIG�Ĵ�������Ϊ0x0f��ʹ��������ģʽ��
 */
void SI24R1_Standby(void)
{
	SI24R1_Write_Reg(W_REGISTER + CONFIG, 0x0f);
}

/**
 * @brief SPI��ʱ����û��ص�����
 *
 * ��SPIͨ�ŵȴ���ʱ�󣬻���ô˺�������ʱ�����
 *
 * @param errorCode ������룬��ʾ��ʱ��ԭ��
 *
 * @return ����ֵ����0����ʾ����ִ�гɹ���
 */
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* �ȴ���ʱ��Ĵ���,���������Ϣ */
    //SI24R1_ERROR("SI24R1 �ȴ���ʱ!errorCode = %d",errorCode);
    return 0;
}

/**
 * @brief �������ݺ���
 *
 * ���ݸ�����ԭʼ����origin������ֽ�Ϊ�����ֽڣ��ֱ�洢��data1��data2�С�
 *
 * @param data1 ָ��洢��λ���ݵ�ָ��
 * @param data2 ָ��洢��λ���ݵ�ָ��
 * @param origin ԭʼ���ݣ�����Ϊu16
 */
void data_deal(u8* data1,u8* data2,u16 origin)
{
    if (origin<100)
    {
        *data1=0;//��λ
        *data2=origin;//��λ
    }
    else if(origin<10000)
    {
        *data1=origin/100;//��λ
        *data2=origin-100*(*data1);//��λ
    }
    else
    {
        *data1 = (origin / 100) % 100; // ��λ
        *data2 = origin % 100; // ��λ
    }
}

/**
 * @brief �������ݣ���ԭʼ����ת��Ϊ����u8���͵�����
 *
 * �ú����������double����ԭʼ����originת��Ϊ����u8���͵����ݣ��洢��data1��data2�С�
 *
 * @param data1 ָ��洢��һ��u8�������ݵ�ָ��
 * @param data2 ָ��洢�ڶ���u8�������ݵ�ָ��
 * @param origin ��Ҫת����ԭʼdouble��������
 */
void double_data_deal(u8* data1,u8* data2,double origin)
{
		*data1=(u8)origin;
		*data2=(u8)((origin-*data1)*100);
}

/**
 * @brief ���ݴ��亯��
 *
 * �ú��������ڲ�ͬ��ģʽ�´������ݡ�
 *
 * @param change_num Ҫ���Ļ�������ݵ����
 * @param mode ����ģʽ����������ģʽΪ1���ӻ�����ģʽΪ0��
 * @param data_1 ��Ҫ����ĵ�һ��˫���ȸ���������
 * @param data_2 ��Ҫ����ĵڶ����޷���16λ��������
 * @param data_3 ��Ҫ����ĵ������޷���16λ��������
 */
void data_transfer(u8 change_num,u8 mode,double data_1,u16 data_2,u16 data_3)
{
	static u8 t_flag=0;
	u8 cnt=0;
	u8 i=4;
	SI24R1_Send_Data[0]=13;
	SI24R1_Send_Data[1]=16;//������ţ����������ֱ�
	
	SI24R1_Send_Data[2]=change_num;//���Ĳ��������/��Ҫ�ӻ����͵Ĳ��������
	
	SI24R1_Send_Data[3]=mode;//��������ģʽΪ1���ӻ�����ģʽΪ0
	
	if(mode==1)
	{
				SI24R1_Mode_NOACK(TX_MODE);//����ģʽ

				double_data_deal(&SI24R1_Send_Data[4],&SI24R1_Send_Data[5],data_1);
				data_deal(&SI24R1_Send_Data[6],&SI24R1_Send_Data[7],data_2);
				data_deal(&SI24R1_Send_Data[8],&SI24R1_Send_Data[9],data_3);//��Ҫ���͵���������6�����͵�λ��

				if(SI24R1_TxPacket(SI24R1_Send_Data) == 0x20)//�������ݰ����̵Ʒ�ת
				LED_Green=!LED_Green;

				/*for (i = 4; i < 10; i++)
				{
						SI24R1_Send_Data[i]=0;  //�������������0
				}*/
	}
    else
    {
        if(!t_flag)SI24R1_Mode_NOACK(TX_MODE);
				
				if(t_flag==0&&SI24R1_TxPacket(SI24R1_Send_Data)==0X20)//˳����������
				{
					LED_Green=!LED_Green;
					
					SI24R1_Mode_NOACK(RX_MODE);
					t_flag=1;//תΪ����ģʽ
				}
				
				if(t_flag&&SI24R1_RxPacket(SI24R1_Receive_Data))//˳�����ܴӻ�����
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
					double_data_deal(&SI24R1_Send_Data[4],&SI24R1_Send_Data[5],data_1);//�����ã�����+С��
					data_deal(&SI24R1_Send_Data[6],&SI24R1_Send_Data[7],data_2);//���μ�ң���ã���������λ+��������λ
					data_deal(&SI24R1_Send_Data[8],&SI24R1_Send_Data[9],data_3);//���μ�ң���ã���������λ+��������λ
				
					LED_Blue=!LED_Blue;
					t_flag=0;//˳�����յ����ݣ���־λ��Ϊ0
				}
    }
    
}
uint8_t tx_flag=0x00;
/*made by ���S*/
void test_tx(void)
{

	LCD_ShowIntNum(0, 16 * (2 + 2), tx_flag+0x50, 3, BLACK, WHITE, 16);
    //SI24R1_Mode_NOACK(TX_MODE); // ����SI24R1Ϊ��Ӧ�����ģʽ
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