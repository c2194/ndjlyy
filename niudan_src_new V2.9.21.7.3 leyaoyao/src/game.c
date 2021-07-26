#include "global.h"

struct game_status GAME_STATUS;

#define FLASH_SIZE 512 //��ѡMCU��FLASH������С(��λΪK)

#if FLASH_SIZE < 256
#define SECTOR_SIZE 1024 //�ֽ�
#else
#define SECTOR_SIZE 2048 //�ֽ�
#endif

#define FLASH_START_ADDRESS 0x8000000
#define FLASH_START_PAGE 100

#define MOTO_PORT GPIOA
#define MOTO_PIN GPIO_Pin_1

void Game_Struct_Init(void)
{

    //   unsigned char ID;         //�豸ID 0-255
    //   unsigned char ENBALE;     //�豸�Ƿ��������
    //   unsigned int COIN_COUNT;  //�Ѿ�Ͷ����
    //   unsigned char GAME_COINS; //ÿ�ֱ�����
    //   unsigned char LIGHT_1;    //����1״̬
    //   unsigned char LIGHT_2;    //����2״̬
    //   unsigned char LIGHT_3;    //����3״̬
    //  unsigned char OPEN_KEY;   //����״̬
    //  unsigned char MOTO_TIME;  //���ת��ʱ��

    GAME_STATUS.ID = 0;         //Ĭ����ID
    GAME_STATUS.GAME_COINS = 2; //Ĭ��3��1��

    GAME_STATUS.DISABLE = 0;

    GAME_STATUS.COIN_COUNT = 0;
    GAME_STATUS.TOTAL_INSERT_COIN_COUNT = 0;
    GAME_STATUS.MACHINE_INSERT_COUNT = 0;
    GAME_STATUS.GAME_COUNT = 0;
    GAME_STATUS.GAME_EGG_OUT_COUNT = 0;

    GAME_STATUS.EGG_OUT_WAIT_TIME = 0;
    GAME_STATUS.SWITCH_OUT_READEY = 0;
    GAME_STATUS.MOTO_TIME = 0;

    GAME_STATUS.EGG_OUT_ING = 0; //������

    //Ĭ�ϵ���Ϸ����

    GAME_STATUS.LIGHT_1 = 0;
    GAME_STATUS.LIGHT_2 = 0;
    GAME_STATUS.LIGHT_3 = 0;

    GAME_STATUS.COIN_MACHINE_STATUS = 0;
    GAME_STATUS.LED_1_STATUS = 0;
    GAME_STATUS.LED_2_STATUS = 0;

    GAME_STATUS.LIGHT_1_CURRENT = 0;
    GAME_STATUS.LIGHT_1_SAMPLE = 0;
    GAME_STATUS.LIGHT_2_CURRENT = 0;
    GAME_STATUS.LIGHT_2_SAMPLE = 0;
    GAME_STATUS.LIGHT_3_CURRENT = 0;
    GAME_STATUS.LIGHT_3_SAMPLE = 0;

    GAME_STATUS.DROP_ADC_COUNT = 0;
    GAME_STATUS.MOTO_TIME_ENABLE = 1;
    GAME_STATUS.COIN_COUNT = 0;

    GAME_STATUS.PLAYING_MUSIC = 0;
    GAME_STATUS.CURRENT_LOCK_STATUS = 1;

    // Game_Status_Read();

    // if (GAME_STATUS.ID == 0xFF)
    // {
    //     GAME_STATUS.ID = 0;
    //     Game_Status_Save();
    // }

    // Game_Status_Read(); //�ٶ�һ��
}

void Game_Moto_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = MOTO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTO_PORT, &GPIO_InitStructure);
    //GPIO_SetBits(MOTO_PORT, MOTO_PIN);

    GPIO_ResetBits(MOTO_PORT, MOTO_PIN);
}

void MOTO_On(void)
{
    USART4_Send_Byte(0xAA);
}

void MOTO_Off(void)
{
    USART4_Send_Byte(0xCC);
}

void Game_Status_Save(void)
{
    //  FLASH_Status FLASH_STATUS;

    //������Ϸϵͳ��������
    //0
    AT24C64_Write_Byte(1, GAME_STATUS.ID); //255
    Delay_72M_Ms(50);

    if (GAME_STATUS.GAME_COINS > 0) //�޶�ÿ����Ϸ��������Ϊ0
    {
        AT24C64_Write_Byte(2, GAME_STATUS.GAME_COINS); //255��
    }
    else
    {
        AT24C64_Write_Byte(2, 1); //255��
    }

    //д��һ������

    AT24C64_Write_Byte(3, (unsigned char)(GAME_STATUS.GAME_COUNT >> 24));
    AT24C64_Write_Byte(4, (unsigned char)(GAME_STATUS.GAME_COUNT >> 16));
    AT24C64_Write_Byte(5, (unsigned char)(GAME_STATUS.GAME_COUNT >> 8));
    AT24C64_Write_Byte(6, (unsigned char)(GAME_STATUS.GAME_COUNT & 0xFF));

    //д��һ��������
    AT24C64_Write_Byte(7, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT >> 24));
    AT24C64_Write_Byte(8, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT >> 16));
    AT24C64_Write_Byte(9, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT >> 8));
    AT24C64_Write_Byte(10, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF));

    //д��һ��Ͷ���������������£������ϵģ�
    AT24C64_Write_Byte(11, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 24));
    AT24C64_Write_Byte(12, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 16));
    AT24C64_Write_Byte(13, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 8));
    AT24C64_Write_Byte(14, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT & 0xFF));

    //д��һ��Ͷ���������������£������ϵģ�
    AT24C64_Write_Byte(15, (unsigned char)(GAME_STATUS.MACHINE_INSERT_COUNT >> 24));
    AT24C64_Write_Byte(16, (unsigned char)(GAME_STATUS.MACHINE_INSERT_COUNT >> 16));
    AT24C64_Write_Byte(17, (unsigned char)(GAME_STATUS.MACHINE_INSERT_COUNT >> 8));
    AT24C64_Write_Byte(18, (unsigned char)(GAME_STATUS.MACHINE_INSERT_COUNT & 0xFF));

    Delay_72M_Ms(50);
}

void Game_Status_Value_Init(void)
{
    uint8_t ucFlag = 0;

    ucFlag = AT24C64_Read_Byte(0);

    if (ucFlag == 0xCC)
    {
        //������ʼ��
        USART1_Send_String("��Ϸ����������ȡ\r\n");

        Game_Status_Read();
    }
    else
    {
        USART1_Send_String("��Ϸ������ʼ��\r\n");

        //������ʼ��
        GAME_STATUS.ID = 0;
        GAME_STATUS.GAME_COINS = 1;
        GAME_STATUS.GAME_COUNT = 0;
        GAME_STATUS.GAME_EGG_OUT_COUNT = 0;
        GAME_STATUS.TOTAL_INSERT_COIN_COUNT = 0;
        GAME_STATUS.MACHINE_INSERT_COUNT = 0;

        AT24C64_Write_Byte(0, 0xCC);
        Game_Status_Save();
        Game_Status_Read();
    }
}

void Game_Status_Read(void)
{
    uint32_t temp = 0;
    unsigned char temp_char = 0;

    GAME_STATUS.ID = AT24C64_Read_Byte(1);
    GAME_STATUS.GAME_COINS = AT24C64_Read_Byte(2);

    //��ȡ��Ϸ�ܾ���
    temp = 0;
    temp_char = 0;
    temp_char = AT24C64_Read_Byte(3);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 24);
    temp_char = AT24C64_Read_Byte(4);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 16);
    temp_char = AT24C64_Read_Byte(5);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 8);
    temp_char = AT24C64_Read_Byte(6);
    temp = (uint32_t)temp | (uint32_t)(temp_char & 0xFF);
    GAME_STATUS.GAME_COUNT = temp;

    //��ȡ��Ϸ�ܵ�����
    temp = 0;
    temp_char = 0;
    temp_char = AT24C64_Read_Byte(7);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 24);
    temp_char = AT24C64_Read_Byte(8);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 16);
    temp_char = AT24C64_Read_Byte(9);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 8);
    temp_char = AT24C64_Read_Byte(10);
    temp = (uint32_t)temp | (uint32_t)(temp_char & 0xFF);
    GAME_STATUS.GAME_EGG_OUT_COUNT = temp;

    //��ȡ��Ϸ��Ͷ����
    temp = 0;
    temp_char = 0;
    temp_char = AT24C64_Read_Byte(11);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 24);
    temp_char = AT24C64_Read_Byte(12);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 16);
    temp_char = AT24C64_Read_Byte(13);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 8);
    temp_char = AT24C64_Read_Byte(14);
    temp = (uint32_t)temp | (uint32_t)(temp_char & 0xFF);
    GAME_STATUS.TOTAL_INSERT_COIN_COUNT = temp;

    //��ȡ��Ϸ�ܵ�����
    temp = 0;
    temp_char = 0;
    temp_char = AT24C64_Read_Byte(15);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 24);
    temp_char = AT24C64_Read_Byte(16);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 16);
    temp_char = AT24C64_Read_Byte(17);
    temp = (uint32_t)temp | (uint32_t)(temp_char << 8);
    temp_char = AT24C64_Read_Byte(18);
    temp = (uint32_t)temp | (uint32_t)(temp_char & 0xFF);
    GAME_STATUS.MACHINE_INSERT_COUNT = temp;
}

void Get_ChipID(void)

{
    //    unsigned char i;
    //    unsigned char temp[12];

    u32 temp0, temp1, temp2;

    temp0 = *(__IO u32 *)(0x1FFFF7E8); //��ƷΨһ��ݱ�ʶ�Ĵ�����96λ��
    temp1 = *(__IO u32 *)(0x1FFFF7EC);
    temp2 = *(__IO u32 *)(0x1FFFF7F0);

    SYSTEM_DEVICE_ID[0] = (u8)(temp0 & 0x000000FF);
    SYSTEM_DEVICE_ID[1] = (u8)((temp0 & 0x0000FF00) >> 8);
    SYSTEM_DEVICE_ID[2] = (u8)((temp0 & 0x00FF0000) >> 16);
    SYSTEM_DEVICE_ID[3] = (u8)((temp0 & 0xFF000000) >> 24);
    SYSTEM_DEVICE_ID[4] = (u8)(temp1 & 0x000000FF);
    SYSTEM_DEVICE_ID[5] = (u8)((temp1 & 0x0000FF00) >> 8);
    SYSTEM_DEVICE_ID[6] = (u8)((temp1 & 0x00FF0000) >> 16);
    SYSTEM_DEVICE_ID[7] = (u8)((temp1 & 0xFF000000) >> 24);
    SYSTEM_DEVICE_ID[8] = (u8)(temp2 & 0x000000FF);
    SYSTEM_DEVICE_ID[9] = (u8)((temp2 & 0x0000FF00) >> 8);
    SYSTEM_DEVICE_ID[10] = (u8)((temp2 & 0x00FF0000) >> 16);
    SYSTEM_DEVICE_ID[11] = (u8)((temp2 & 0xFF000000) >> 24);

    /*   USART1_Send_Debug_Message("System ID:\r\n");

    for (i = 0; i < 12; i++)
    {
        USART1_Send_Hex(SYSTEM_DEVICE_ID[i]);
        USART1_Send_Debug_Message(" ");
    }

    USART1_Send_Debug_Message("\r\n");
*/
}

unsigned char ucCoinStatus;
unsigned int uiCoinTimeOut;

//Ͷ��������+1
void Coin_Insert_Add(unsigned char ucSelect)
{

    switch (ucSelect)
    {
    case COIN_MACHINE: //Ͷ��������

        GAME_STATUS.COIN_COUNT++;
        GAME_STATUS.TOTAL_INSERT_COIN_COUNT++;
        GAME_STATUS.MACHINE_INSERT_COUNT++;

        Game_Status_Save();
        Game_Status_Read();

        // USART1_Send_String("��������");
        // USART1_Send_Dec(GAME_STATUS.COIN_COUNT, 6);
        // USART1_Send_String("\r\n");

        if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
        {

            VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //���˿�ʼŤ����

            if (GAME_STATUS.EGG_OUT_ING != 1) //�����ǰ��ʧŤ����
            {
                GAME_STATUS.SWITCH_OUT_READEY = 1;
            }

            GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
        }
        else
        {
            VOICE_Play(VOICE_COIN_MORE_PLEASE); //����Ͷһ������
        }
        break;

    case NET_ADD: //��������

        if (SYSTEM_USART_BUFFER[5] != 0 && SYSTEM_USART_BUFFER[5] != 0xFF)
        {
            System_Setting_Count(SYSTEM_USART_BUFFER[5]);
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 1); //3ָ���룬1 TRUE
        }
        else
        {
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 0); //����ʧ����
        }

        if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
        {

            VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //���˿�ʼŤ����

            if (GAME_STATUS.EGG_OUT_ING != 1) //�����ǰ��ʧŤ����
            {
                GAME_STATUS.SWITCH_OUT_READEY = 1;
            }

            GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
        }

    default:
        break;
    }
}

void Check_Coin_Status(void)
{

    if (GAME_STATUS.COIN_ENABLE == 1)
    {
        switch (ucCoinStatus)
        {

        case 0: //û��Ͷ��
            if (GPIO_ReadInputDataBit(COIN_PORT, COIN_PINS) == 0)
            {
                Delay_72M_Ms(50);
                if (GPIO_ReadInputDataBit(COIN_PORT, COIN_PINS) == 0)
                {
                    ucCoinStatus = 1;
                }
                else
                {
                    ucCoinStatus = 0;
                }
            }
            break;

        case 1: //�ȴ�����̧��

            uiCoinTimeOut = 600;

            while (GPIO_ReadInputDataBit(COIN_PORT, COIN_PINS) == 0)
            {
                uiCoinTimeOut--;
                Delay_72M_Ms(1);
                if (uiCoinTimeOut <= 100)
                {
                    ucCoinStatus = 0;
                    uiCoinTimeOut = 0;
                    Game_Insert_Coin_Disable();
                    break;
                }
            }

            if (ucCoinStatus != 0)
            {
                ucCoinStatus = 2;
            }

            break;

        case 2: //������+1

            Coin_Insert_Add(COIN_MACHINE);
            Leyaoyao_Retrun_Add_Coins();
            ucCoinStatus = 0;

            break;

        default:
            break;
        }
    }
}

void Check_Coin_Add(void)
{
    if (GPIO_ReadInputDataBit(COIN_PORT, COIN_PINS) == 0)
    {
        Delay_72M_Ms(50);

        if (GPIO_ReadInputDataBit(COIN_PORT, COIN_PINS) == 0)
        {
            while (GPIO_ReadInputDataBit(COIN_PORT, COIN_PINS) == 0)
                ;
            //	VOICE.Play(0);
            // USART1_Send_Debug_Message("Ͷ��1��");
            VOICE_Play(2);
        }
    }
}

//}

unsigned char EGG_OUT_CHECK_STATUS = 0;

#define EGG_OUT_CHECK_TIME_IDEL 0
#define EGG_OUT_CHECK_TIME_START 1
#define EGG_OUT_CHECK_TIME_OVER 2

void Check_Game_Time_Out(void)
{

    switch (EGG_OUT_CHECK_STATUS)
    {
    case EGG_OUT_CHECK_TIME_IDEL: //������ʱ��

        // USART1_Send_String("State 1\r\n");

        //  USART1_Send_String("State 1 \r\n");
        if (GAME_STATUS.SWITCH_OUT_READEY == 1 && GAME_STATUS.EGG_OUT_ING != 1) //���������ʼ����,�Ҳ������ڳ���ʱ ��������ʱ
        {
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_START;
            GAME_STATUS.EGG_OUT_WAIT_TIME = 20; //���õ���ʱʱ��Ϊ20��
        }
        else
        {
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_IDEL;
        }

        break;

    case EGG_OUT_CHECK_TIME_START:

        // USART1_Send_String("State 2\r\n");

        if (GAME_STATUS.EGG_OUT_WAIT_TIME <= 10 && GAME_STATUS.EGG_OUT_WAIT_TIME != 0)
        {
            Delay_72M_Ms(10);
            if (VOICE_Check_Busy() == 1)
            {
                VOICE_Play(VOICE_TIME_OUT);
            }

            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_OVER;
        }

        break;

    case EGG_OUT_CHECK_TIME_OVER:

        // USART1_Send_String("State 3\r\n");

        //����ʱΪ0ʱ�����������������
        if (GAME_STATUS.EGG_OUT_WAIT_TIME == 0 && VOICE_Check_Busy() == 1)
        {
            Game_Egg_Out(TIME_OVER_OUT);
        }

        // if (GAME_STATUS.EGG_OUT_WAIT_TIME == 0)
        // {
        //     Game_Egg_Out(TIME_OVER_OUT);
        // }

        break;

    default:

        break;
    }
}
unsigned char EGG_OUT;
unsigned int EGG_OUT_TIME;
unsigned char EGG_OUT_ERROR;

//Game_Egg_Out

//
void Game_Egg_Out(unsigned char ucSelect)
{
    switch (ucSelect)
    {
    case SWITCH_OUT: //��ť����

        if (GAME_STATUS.EGG_OUT_ING == 0)
        {
            Voice_Play_Stop();
            VOICE_Play(VOICE_MUSIC_1);
            GAME_STATUS.PLAYING_MUSIC = 1;

            Game_Coin_Minus();

            USART1_Send_String("��ť����������������ģʽ2\r\n");

            GAME_STATUS.EGG_OUT_ING = 1; //����Ϊ���ڳ���
            MOTO_On();
            GAME_STATUS.MOTO_TIME = 20; //��ʱʱ��20��
            GAME_STATUS.MOTO_TIME_ENABLE = 1;
            EGG_OUT = 2; //��ʼ���ʱ���

            USART1_Send_String("��ֹ����ʱ����\r\n");
            GAME_STATUS.SWITCH_OUT_READEY = 0; //����
            GAME_STATUS.EGG_OUT_WAIT_TIME = 0;
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_IDEL;

            //�����ܵ�������
            if (GAME_STATUS.GAME_COUNT >= 0xFFFFFFFE)
            {
                GAME_STATUS.GAME_COUNT = 0;
            }

            GAME_STATUS.GAME_COUNT++;

            Game_Status_Save();
        }

        break;

    case TIME_OVER_OUT: //��ʱ����

        if (GAME_STATUS.EGG_OUT_ING == 0)
        {

            Voice_Play_Stop();
            VOICE_Play(VOICE_MUSIC_1);
            GAME_STATUS.PLAYING_MUSIC = 1;

            Game_Coin_Minus();

            USART1_Send_String("��ʱ����������������ģʽ2\r\n");

            GAME_STATUS.EGG_OUT_ING = 1; //����Ϊ���ڳ���
            MOTO_On();
            GAME_STATUS.MOTO_TIME = 20; //��ʱʱ��20��
            GAME_STATUS.MOTO_TIME_ENABLE = 1;
            EGG_OUT = 2; //��ʼ���ʱ���

            USART1_Send_String("��ֹ����ʱ����\r\n");
            GAME_STATUS.SWITCH_OUT_READEY = 0; //����
            GAME_STATUS.EGG_OUT_WAIT_TIME = 0;
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_IDEL;

            //�����ܵ�������
            if (GAME_STATUS.GAME_COUNT >= 0xFFFFFFFE)
            {
                GAME_STATUS.GAME_COUNT = 0;
            }

            GAME_STATUS.GAME_COUNT++;
            Game_Status_Save();
        }

        break;

    default:
        break;
    }
}

//������Ϸ��
void Game_Coin_Minus(void)
{

    if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    {

        GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT - GAME_STATUS.GAME_COINS; //��ȥ����
    }
    else
    {
        GAME_STATUS.COIN_COUNT = 0;
    }
}

void Game_Egg_Out_Stop(unsigned char ucSelect)
{
    switch (ucSelect)
    {
    case EGG_OUT_SUCCEED:
        /* code */

        USART1_Send_String("������\r\n");

        Voice_Play_Stop();
        VOICE_Play(VOICE_EGG_OUT);

        //Game_Coin_Minus();

        //�����ܵ�������
        if (GAME_STATUS.GAME_EGG_OUT_COUNT >= 0xFFFFFFFE)
        {
            GAME_STATUS.GAME_EGG_OUT_COUNT = 0;
        }

        GAME_STATUS.GAME_EGG_OUT_COUNT++;

        Game_Status_Save();

        //

        // USART1_Send_Debug_Message("��Ϸ����:");
        // USART1_Send_Dec(GAME_STATUS.GAME_COUNT, 6);

        // USART1_Send_Debug_Message("\r\n");

        //�����Ϸ����������Ȼ���ó�ʱŤ��
        if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
        {

            GAME_STATUS.SWITCH_OUT_READEY = 1;
        }
        else
        {
            GAME_STATUS.SWITCH_OUT_READEY = 0;
        }

        GAME_STATUS.EGG_OUT_ING = 0;

        EGG_OUT = 0;
        GAME_STATUS.DROP_EGG = 0;

        MOTO_Off();
        GAME_STATUS.MOTO_TIME = 0;
        GAME_STATUS.MOTO_TIME_ENABLE = 0;

        leyaoyao_return_yun_coins_pay_jieguo(1);
        Delay_72M_Ms(1500);
        Leyaoyao_Retrun_Coins_Details();

        break;

    case EGG_OUT_TIME_OUT:

        USART1_Send_String("����ʧ�ܳ�ʱδ��\r\n");

        Voice_Play_Stop();
        VOICE_Play(VOICE_HAS_ERROR_CONNECT_US);
        MOTO_Off();

        //  Game_Coin_Minus();

        //System_Setting_Game_Disable(_SYSTEM_EGG_TIME_OUT_LOCK); //������ʱ����

        System_Setting_Game_Disable(_SYSTEM_LEYAOYAO_LOCK);

        GAME_STATUS.SWITCH_OUT_READEY = 0;
        GAME_STATUS.EGG_OUT_ING = 0;
        EGG_OUT = 0;
        GAME_STATUS.DROP_EGG = 0;
        GAME_STATUS.MOTO_TIME = 0;
        GAME_STATUS.MOTO_TIME_ENABLE = 0;

        leyaoyao_return_yun_coins_pay_jieguo(0);

        Delay_72M_Ms(5000);

        leyaoyao_return_system_lock(120);
        Voice_Play_Stop();

    default:
        break;
    }
}

void Game_Egg_Switch_Out(void)
{
    switch (EGG_OUT)
    {
    case 0: //��׼ģʽ

        break;

    case 1: // ����

        Game_Egg_Out(SWITCH_OUT);
        EGG_OUT = 2;
        //   EGG_OUT_ERROR = 0;
        //   EGG_OUT_TIME = 50000;

        break;

    case 2: //���ʱ��
        //	USART1_Send_Debug_Message("ģʽ3");

        //  USART1_Send_Debug_Message("��ǰʱ��: ");
        //  USART1_Send_Dec(GAME_STATUS.MOTO_TIME, 3);
        //   USART1_Send_Debug_Message("\r\n");

        if (GAME_STATUS.DROP_EGG == 1)
        {
            EGG_OUT = 4; //�ɹ������˵�
        }

        if (GAME_STATUS.MOTO_TIME == 0) //��ʱ��
        {

            EGG_OUT = 3;
        }

        if (GAME_STATUS.LIGHT_2 == 1)
        {

            EGG_OUT = 5;
        }

        break;

    case 3: //��ʱ

        Game_Egg_Out_Stop(EGG_OUT_TIME_OUT);

        break;

    case 4: //�ɹ�����

        Game_Egg_Out_Stop(EGG_OUT_SUCCEED);
        break;

    case 5: //��ͣ

        MOTO_Off();

#ifdef HAND_IN_STOP_MOTO_TIME
        GAME_STATUS.MOTO_TIME_ENABLE = 0; //��ֹʱ�����
#endif

        EGG_OUT = 6;
        break;

    case 6: //�ȴ���������
        if (GAME_STATUS.LIGHT_2 == 0)
        {

            MOTO_On();
            GAME_STATUS.PLAYING_MUSIC = 1; //���²�������
            Voice_Play_Stop();
            VOICE_Play(VOICE_MUSIC_1);
            EGG_OUT = 2;

#ifdef HAND_IN_STOP_MOTO_TIME
            GAME_STATUS.MOTO_TIME_ENABLE = 1; //����ʱ�����
#endif
        }

        break;

    default:
        break;
    }
}
