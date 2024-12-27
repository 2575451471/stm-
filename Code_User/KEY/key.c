#include "key.h"

// �������¼�ʱ
// -------------------------
u8 WKUP_Cnt = 0;
u8 KEY1_Cnt = 0;
u8 KEY_UP_Cnt = 0;
u8 KEY_DOWN_Cnt = 0;
u8 KEY_LEFT_Cnt = 0;
u8 KEY_RIGHT_Cnt = 0;
u8 KEY_A_Cnt = 0;
u8 KEY_AA_Cnt = 0;
u8 KEY_B_Cnt = 0;
u8 KEY_BB_Cnt = 0;
u8 KEY_Rocker1_Cnt = 0;
u8 KEY_Rocker2_Cnt = 0;
// -------------------------

// 12��������״̬������Ϊ1���ɿ�Ϊ0
//-------------------------------
u8 F_KEY_Down[12] = {0};
//-------------------------------

// ������ʼ��
//---------------------
/**
 * @brief ��ʼ���������GPIO
 *
 * ���������ڳ�ʼ���밴����ص�GPIO���š�
 *
 * @return �޷���ֵ
 */
void KEY_Init_JX(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; // ����GPIO_InitTypeDef�ͽṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // JTAGʧ��

	// ʹ��GPIOA��GPIOC�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); // ʹ��GPIOʱ��

	// ������WKUP--PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	  // WKUP--PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ��������
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����ģʽ����Ҫ��˿��ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure); // �����趨������ʼ��PA0

	// ������KEY_LeftRocker--PA8��KEY_UP--PA11��KEY_DOWN--PA12��KEY_LEFT--PA15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  // �����趨������ʼ��

	// ������KEY_RIGHT--PB3��KEY_A--PB6��KEY_A+--PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);		  // �����趨������ʼ��

	// ������KEY_1--PC14��KEY_RightRocker--PC15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);		  // �����趨������ʼ��
}

// ����ɨ�躯��
//------------------------------------------
/**
 * @brief ɨ�谴��״̬
 *
 * �ú�������ɨ����������״̬������WKUP��KEY1��KEY_UP��KEY_DOWN��KEY_LEFT��KEY_RIGHT��KEY_A��KEY_AA��KEY_Rocker1��KEY_RightRocker��
 * ����״̬ͨ��ȫ������F_KEY_Down��¼��������������³���һ��ʱ�䣨�˴�Ϊ9��ѭ��������ÿ��ѭ������һ�ΰ���������ڣ������Ӧ����Ԫ�ر�����Ϊ1����ʾ���������¡�
 *
 */
void RF2G4_KEY_Scan_JX(void)
{
	// WKUP
	//------------------------------------
	// �ж�WKUP���Ƿ���
	if (!WKUP_in)
	{
		// ���WKUP��δ���£����ü����������Ϊδ����
		WKUP_Cnt = 0;
		F_KEY_Down[WKUP] = 0;
	}
	else
	{
		// ���WKUP�����£����Ӽ�����
		WKUP_Cnt++;
		// �������������8�����WKUP��Ϊ����
		if (WKUP_Cnt > 8)
		{
			F_KEY_Down[WKUP] = 1;
		}
	}
	//------------------------------------

	// KEY1
	//------------------------------------
	// �ж�KEY1���Ƿ���
	if (KEY1_in)
	{
		// ���KEY1��δ���£����ü����������Ϊδ����
		KEY1_Cnt = 0;
		F_KEY_Down[KEY1] = 0;
	}
	else
	{
		// ���KEY1�����£����Ӽ�����
		KEY1_Cnt++;
		// �������������8�����KEY1��Ϊ����
		if (KEY1_Cnt > 8)
		{
			F_KEY_Down[KEY1] = 1;
		}
	}
	//------------------------------------

	// KEY_UP
	//------------------------------------
	// �ж�KEY_UP���Ƿ���
	if (KEY_UP_in)
	{
		// ���KEY_UP��δ���£����ü����������Ϊδ����
		KEY_UP_Cnt = 0;
		F_KEY_Down[KEY_UP] = 0;
	}
	else
	{
		// ���KEY_UP�����£����Ӽ�����
		KEY_UP_Cnt++;
		// �������������8�����KEY_UP��Ϊ����
		if (KEY_UP_Cnt > 8)
		{
			F_KEY_Down[KEY_UP] = 1;
		}
	}
	//------------------------------------

	// KEY_DOWN
	//------------------------------------
	// �ж�KEY_DOWN���Ƿ���
	if (KEY_DOWN_in)
	{
		// ���KEY_DOWN��δ���£����ü����������Ϊδ����
		KEY_DOWN_Cnt = 0;
		F_KEY_Down[KEY_DOWN] = 0;
	}
	else
	{
		// ���KEY_DOWN�����£����Ӽ�����
		KEY_DOWN_Cnt++;
		// �������������8�����KEY_DOWN��Ϊ����
		if (KEY_DOWN_Cnt > 8)
		{
			F_KEY_Down[KEY_DOWN] = 1;
		}
	}
	//------------------------------------

	// KEY_LEFT
	//------------------------------------
	// �ж�KEY_LEFT���Ƿ���
	if (KEY_LEFT_in)
	{
		// ���KEY_LEFT��δ���£����ü����������Ϊδ����
		KEY_LEFT_Cnt = 0;
		F_KEY_Down[KEY_LEFT] = 0;
	}
	else
	{
		// ���KEY_LEFT�����£����Ӽ�����
		KEY_LEFT_Cnt++;
		// �������������8�����KEY_LEFT��Ϊ����
		if (KEY_LEFT_Cnt > 8)
		{
			F_KEY_Down[KEY_LEFT] = 1;
		}
	}
	//------------------------------------

	// KEY_RIGHT
	//------------------------------------
	// �ж�KEY_RIGHT���Ƿ���
	if (KEY_RIGHT_in)
	{
		// ���KEY_RIGHT��δ���£����ü����������Ϊδ����
		KEY_RIGHT_Cnt = 0;
		F_KEY_Down[KEY_RIGHT] = 0;
	}
	else
	{
		// ���KEY_RIGHT�����£����Ӽ�����
		KEY_RIGHT_Cnt++;
		// �������������8�����KEY_RIGHT��Ϊ����
		if (KEY_RIGHT_Cnt > 8)
		{
			F_KEY_Down[KEY_RIGHT] = 1;
		}
	}
	//------------------------------------

	// KEY_A
	//------------------------------------
	// �ж�KEY_A���Ƿ���
	if (KEY_A_in)
	{
		// ���KEY_A��δ���£����ü����������Ϊδ����
		KEY_A_Cnt = 0;
		F_KEY_Down[KEY_A] = 0;
	}
	else
	{
		// ���KEY_A�����£����Ӽ�����
		KEY_A_Cnt++;
		// �������������8�����KEY_A��Ϊ����
		if (KEY_A_Cnt > 8)
		{
			F_KEY_Down[KEY_A] = 1;
		}
	}
	//------------------------------------

	// KEY_AA
	//------------------------------------
	// �ж�KEY_AA���Ƿ���
	if (KEY_AA_in)
	{
		// ���KEY_AA��δ���£����ü����������Ϊδ����
		KEY_AA_Cnt = 0;
		F_KEY_Down[KEY_AA] = 0;
	}
	else
	{
		// ���KEY_AA�����£����Ӽ�����
		KEY_AA_Cnt++;
		// �������������8�����KEY_AA��Ϊ����
		if (KEY_AA_Cnt > 8)
		{
			F_KEY_Down[KEY_AA] = 1;
		}
	}
	//------------------------------------

	// KEY_Rocker1
	//------------------------------------
	// �ж�KEY_Rocker1���Ƿ���
	if (KEY_Rocker1_in)
	{
		// ���KEY_Rocker1��δ���£����ü����������Ϊδ����
		KEY_Rocker1_Cnt = 0;
		F_KEY_Down[KEY_Rocker1] = 0;
	}
	else
	{
		// ���KEY_Rocker1�����£����Ӽ�����
		KEY_Rocker1_Cnt++;
		// �������������8�����KEY_Rocker1��Ϊ����
		if (KEY_Rocker1_Cnt > 8)
		{
			F_KEY_Down[KEY_Rocker1] = 1;
		}
	}
	//------------------------------------

	// KEY_RightRocker
	//------------------------------------
	// �ж�KEY_Rocker2���Ƿ���
	if (KEY_Rocker2_in)
	{
		// ���KEY_Rocker2��δ���£����ü����������Ϊδ����
		KEY_Rocker2_Cnt = 0;
		F_KEY_Down[KEY_Rocker2] = 0;
	}
	else
	{
		// ���KEY_Rocker2�����£����Ӽ�����
		KEY_Rocker2_Cnt++;
		// �������������8�����KEY_Rocker2��Ϊ����
		if (KEY_Rocker2_Cnt > 8)
		{
			F_KEY_Down[KEY_Rocker2] = 1;
		}
	}
	//------------------------------------
}
//------------------------------------------
