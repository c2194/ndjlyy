#include "global.h"

#define TIM6_CNT    (10000 - 1)
#define TIM6_PSC    (7200 - 1)


unsigned char ucMusicEnable;

unsigned char ADD_CODE;

unsigned char idle_handler =0;

//ϵͳ��ʱ���жϳ�ʼ��
/*
void SysTick_Init(void)
{

	// 72M / 1

	if (SysTick_Config(SystemCoreClock / 10000))
	{

		while (1)
		{
		}
	}
}*/

/*����Ƕ�������жϿ�����NVIC */
// static void NVIC_Configuration(void)
// {
// 	NVIC_InitTypeDef NVIC_InitStructure;

// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // �������ȼ�����

// 	/* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�Ϊ0
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// }

void GPIO_Set_All_Pins(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_All);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_All);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_All);
}






void SK_TIM6_NVICConifg(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
 
 

NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);
}
 
void SK_TIM6_Init(void)
{   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
    SK_TIM6_NVICConifg();
 
    /// Step 1 : Open TIM6&TIM7 Clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
 
    /// Step 2 : Set basic settings
    // Input CLOCK = 72MHz
    TIM_TimeBaseStructure.TIM_Period = TIM6_CNT;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM6_PSC;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
 
    TIM_ARRPreloadConfig(TIM6, ENABLE);
 
    TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Repetitive);
 
    TIM_UpdateRequestConfig(TIM6, TIM_UpdateSource_Global);
 
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
 
    TIM_Cmd(TIM6, ENABLE);
}


void System_Setting_Key_Factory(void)
{
	if (get_Setting_Key_Status() == 0)
	{
		LED_Blue_On();

		//***add
		Light1_Off(); //����ǰ�õƹ�������ʾ
		Light2_Off();
		//***add

		Delay_72M_Ms(1000);

		if (get_Setting_Key_Status() == 0)
		{

			//USART1_Send_Debug_Message("���յ�����IDָ��\r\n");

			AT24C64_Write_Byte(0, 0xDD); //д�����������־λ

			while (get_Setting_Key_Status() != 0x01)
			{
				Delay_72M_Ms(1000);
				//***add
				Light1_Off(); //����ǰ�õƹ�������ʾ
				Light2_Off();

				Delay_72M_Ms(1000);
				//***add
				Light1_On(); //����ǰ�õƹ�������ʾ
				Light2_On();
			}
			LED_Blue_Off();

			//***add
			Light1_On();
			Light2_On();
			Voice_Play_Stop();
			VOICE_Play(VOICE_OK_NIUDAN_ALREADY_OK);
			//***add
		}
	}

	System_Buffer_Clean();
	SYSTEM_RECEIVE_STATE = 0; //
	LEYAOYAO_RECEVICE_STATE = 0;
}

void System_Init(void)
{

	//unsigned char i = 0;
	
	idle_handler =0;

	VOICE_Out_Pins_Init(); //��ֹ��������
	GPIO_Set_All_Pins();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_Configuration(); //��ʼ��ʱ��

	LED_Gpio_Init();
	Game_Moto_Init();

	LED_Red_On();
	LED_Blue_On();

	Delay_72M_Ms(2000);
	USART1_Init(57600);
	USART2_Init(57600);
	
	USART4_Init(57600);

	USART1_Send_String("System Start\r\n");
USART3_Init(38400);
	// USART3_Send_Byte(0xcc);


	MOTO_Off();
	VOICE_Pins_Init();

	//USART4_Send_Byte(0xAA);

	Key_Init();
	Game_Struct_Init();

	//Ĭ��ID���� ������Ϊ0
	//GAME_STATUS.ID = 4;

	ucMusicEnable = 0;
	EGG_OUT = 0;
	EGG_OUT_ERROR = 0;
	EGG_OUT_TIME = 0;
	ucCoinStatus = 0;
	uiCoinTimeOut = 0;
	EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_IDEL;

	Switch_Key_Init();

	Get_ChipID();
	Coin_Pin_Init();
	Light1_Pin_Init();
	Light1_On();

	Light2_Pin_Init();
	Light2_On();

	Game_Insert_Coin_Enable_Pins_Init();
	AT24C64_Power_On(); //��ʼ��AT24C64

	Digital_Tube_Init();
	Digital_Tube_Display(GAME_STATUS.GAME_COINS);

	//������Ϸ
	//System_Setting_Game_Disable(_SYSTEM_RESTART_LOCK);
	System_Setting_Game_Disable(_SYSTEM_UNLOCK);
	//System_Setting_Game_Disable(_SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE);

	USART_485_Pins_Init();

	System_Setting_Key_Factory();

	Game_Status_Value_Init(); //��ʼ����Ϸ����

	USART1_Send_String("ID: ");
	USART1_Send_Dec(GAME_STATUS.ID, 3);
	USART1_Send_String("\r\n");
	USART1_Send_String("ÿ����Ϸ����: ");
	USART1_Send_Dec(GAME_STATUS.GAME_COINS, 3);
	USART1_Send_String("\r\n");

	USART1_Send_String("��Ϸ����: ");
	USART1_Send_Dec(GAME_STATUS.GAME_COUNT, 10);
	USART1_Send_String("\r\n");

	USART1_Send_String("��������: ");
	USART1_Send_Dec(GAME_STATUS.GAME_EGG_OUT_COUNT, 10);
	USART1_Send_String("\r\n");


	USART1_Send_String("��Ͷ����: ");
	USART1_Send_Dec(GAME_STATUS.TOTAL_INSERT_COIN_COUNT, 10);
	USART1_Send_String("\r\n");

	USART1_Send_String("����Ͷ����: ");
	USART1_Send_Dec(GAME_STATUS.MACHINE_INSERT_COUNT, 10);
	USART1_Send_String("\r\n");



	// GAME_STATUS.ID = 5;
	// GAME_STATUS.GAME_COINS = 10;
	// GAME_STATUS.GAME_COUNT = 1234567890;

	// Game_Status_Save();

	// Game_Status_Read();

	// USART1_Send_String("ID: ");
	// USART1_Send_Dec(GAME_STATUS.ID, 3);
	// USART1_Send_String("\r\n");

	// USART1_Send_String("ÿ����Ϸ����: ");
	// USART1_Send_Dec(GAME_STATUS.GAME_COINS, 3);
	// USART1_Send_String("\r\n");

	// USART1_Send_String("��Ϸ����: ");
	// USART1_Send_Dec(GAME_STATUS.GAME_COUNT, 10);
	// USART1_Send_String("\r\n");

	VOICE_Pins_Init();

	//IWDG_Init(IWDG_Prescaler_64 ,625);
	// USART_485_Enable();
	// USART2_Send_String("system start 222;\r\n");
	// USART_485_Disable();

	Delay_72M_Ms(2000);

	LED_Blue_Off();

	GAME_STATUS.COIN_ENABLE = 1;


	ADD_CODE = 0x1A +1;


	TIM4_Int_Init(9999, 7199); //��ʼ��TIMER4TIM4_Int_Init(9999, 7199); //��ʼ��TIMER4
	
	SK_TIM6_Init();




	// while(1)
	// {
	// 	i = USART3_Receive_Byte();
	// 	USART1_Send_Byte(i);
	// 	//	USART3_Send_Byte(++i);
	// }

	
}

void TIM2_Int_Init(u16 arr, u16 psc)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʱ�� TIM3 ʹ��
	//��ʱ�� TIM3 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;						//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//�ڳ�ʼ�� TIM3
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//����������ж�
	//�ж����ȼ� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3 �ж�
													// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
													// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ� 3 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);					//�ܳ�ʼ�� NVIC �Ĵ���
	TIM_Cmd(TIM2, ENABLE);							//��ʹ�� TIM3
													//USART1_Send_String("����������\r\n");
}

void LED_Blink(unsigned char uc)
{
	if (uc == 0)
	{
		LED_Red_On();
	}
	else
	{
		LED_Red_Off();
	}
}

int main(void)
{
	unsigned char ucLedStatus = 0;
	//unsigned char uclastvalue = 0;
	unsigned int uiLastCoin = 0;
	int iCoinTemp = 0;

	System_Init();
	


	while (1)
	{
		//IWDG_Feed();


		//led����˸
		ucLedStatus = ~ucLedStatus;
		LED_Blink(ucLedStatus);

		Check_Light_Eye_Status();
		Game_Egg_Switch_Out(); //����
		Check_Game_Time_Out(); //��Ϸʱ�䵹��
		Check_Coin_Status();

		// if (GAME_STATUS.EGG_OUT_WAIT_TIME != 0)
		// {
		// 	if (uclastvalue != GAME_STATUS.EGG_OUT_WAIT_TIME)
		// 	{
		// 		USART1_Send_String("��Ϸ����ʱ:");
		// 		USART1_Send_Dec(GAME_STATUS.EGG_OUT_WAIT_TIME, 3);
		// 		USART1_Send_String("\r\n");
		// 		uclastvalue = GAME_STATUS.EGG_OUT_WAIT_TIME;
		// 	}
		// }

		// if (GAME_STATUS.MOTO_TIME != 0)
		// {
		// 	if (uclastvalue != GAME_STATUS.MOTO_TIME)
		// 	{
		// 		USART1_Send_String("�������ʱ:");
		// 		USART1_Send_Dec(GAME_STATUS.MOTO_TIME, 3);
		// 		USART1_Send_String("\r\n");
		// 		uclastvalue = GAME_STATUS.MOTO_TIME;
		// 	}
		// }

		//��������ܱ�����ʾ
		if (GAME_STATUS.CURRENT_LOCK_STATUS == _SYSTEM_UNLOCK || GAME_STATUS.CURRENT_LOCK_STATUS == _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE)
		{
			// 	if(uiLastCoin!=GAME_STATUS.COIN_COUNT)
			// {
			// 		Digital_Tube_Display(GAME_STATUS.COIN_COUNT);
			// }
			iCoinTemp = GAME_STATUS.GAME_COINS - GAME_STATUS.COIN_COUNT;

			if (iCoinTemp < 0)
			{
				iCoinTemp = 0;
			}
			if (iCoinTemp != uiLastCoin)
			{
				Digital_Tube_Display(iCoinTemp);

				uiLastCoin = iCoinTemp;
			}

			//	Digital_Tube_Display(iCoinTemp);
		}
		else
		{
			Digital_Tube_Display_Error();
		}

		// USART1_Send_String("��ǰ״̬:");
		// USART1_Send_Hex(GAME_STATUS.DISABLE );
		// USART1_Send_String("\r\n");

		Check_Switch_Key_Status(); //�����ť
		System_Check_Receive();
		Leyaoyao_Check_Receive();
		
		
		
		if(idle_handler > 1){
			
			if(idle_handler > 2){
			  idle_handler =3;
			}else{
				if(LEYAOYAO_RECEVICE_STATE==0){
				Leyaoyao_Buffer_Clean();
				}			
			}
			
			
			
			
			
		}
		
		
		
	}
}





