#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "usart.h"
#include "game.h"
#include "timer.h"
#include "main.h"
#include "key.h"
#include "spiflash.h"
#include "music.h"
#include "protocol.h"
#include "led.h"
#include "switch_key.h"
#include "light_eye.h"
#include "clock.h"
#include "light.h"
#include "watch_dog.h"

unsigned char Data_Pack_Head[2] = {0xFC, 0x55}; //Э���ͷ

unsigned char SYSTEM_DEVICE_ID[12];
unsigned char SYSTEM_USART_BUFFER[SYSTEM_USART_BUFFER_LENGTH];
unsigned int SYSTEM_BUFFER_RECEVICE_COUNT = 0; //��ǰ���յ�����
unsigned char SYSTEM_RECEIVE_STATE = 0;        //���յ�״̬
unsigned int SYSTEM_BUFFER_RECEVICE_ERROR = 0;
uint64_t COIN_STATUS = 0;

//���ͷ���
unsigned char System_Send_Return(void)
{
    unsigned char i = 0;

    //���Ͱ�ͷ
    for (i = 0; i < 2; i++)
    {
        USART1_Send_Byte(Data_Pack_Head[i]);
    }

    return 0;
}

//debug �����Ϣ
void __Debug_Send_Recevie_Buffer(void)
{
    unsigned char i = 0;

    USART1_Send_String("Rec: ");

    for (i = 0; i < SYSTEM_BUFFER_RECEVICE_COUNT; i++)
    {
        USART1_Send_Hex(SYSTEM_USART_BUFFER[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    USART1_Send_String("Count: ");
    USART1_Send_Dec(SYSTEM_BUFFER_RECEVICE_COUNT, 4);
    USART1_Send_String("\r\n");

    Delay_72M_Ms(200);
}

//��ս��ջ�����
void System_Buffer_Clean(void)
{
    unsigned char i = 0;

    for (i = 0; i < SYSTEM_USART_BUFFER_LENGTH; i++)
    {
        SYSTEM_USART_BUFFER[i] = 0;
    }

    SYSTEM_BUFFER_RECEVICE_COUNT = 0;
    SYSTEM_RECEIVE_STATE = 0;
    SYSTEM_BUFFER_RECEVICE_ERROR = 0;

    Delay_72M_Ms(50);
}

//ϵͳ��λ
void System_Reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

void System_Send_Open_Status(unsigned char ucX, unsigned char ucY, unsigned char ucEnable)
{
    unsigned char ucHighSum = 0;
    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    //    unsigned char i = 0;

    //���㷢��У����
    uiSum = 4 + 1;
    uiSum = uiSum + ucX + ucY + ucEnable;

    //ȡ��
    uiSum = ~uiSum;

    //У����
    ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    USART1_Send_Byte(0xFC); //��ͷ
    USART1_Send_Byte(0x66);

    USART1_Send_Byte(4);    //����
    USART1_Send_Byte(0x01); //����ָ��

    USART1_Send_Byte(ucX); //X
    USART1_Send_Byte(ucY);
    USART1_Send_Byte(ucEnable); //״̬

    //�豸ID
    // for(i=0;i<12;i++)
    // {
    //     USART1_Send_Byte(SYSTEM_DEVICE_ID[i]);
    // }

    USART1_Send_Byte(ucHighSum);
    USART1_Send_Byte(ucLowSum);
}

void System_Send_Return_Status(unsigned char ucTargetID, unsigned char ucCommand, unsigned char ucStatus)
{

    //  unsigned char ucHighSum = 0;
    //  unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned char i = 0;
    unsigned char SEND_BUFFER[30];

    // Game_Status_Save();

    // if()

    //���㷢��У����
    // uiSum = 13 + 2;

    SEND_BUFFER[0] = 0x7F;           //��ͷ
    SEND_BUFFER[1] = ucTargetID;     //Ŀ��ID
    SEND_BUFFER[2] = GAME_STATUS.ID; //����ID
    SEND_BUFFER[3] = 2;              //����
    SEND_BUFFER[4] = ucCommand;      //��Ӧָ��
    SEND_BUFFER[5] = ucStatus;

    //����У����
    for (i = 1; i < 6; i++)
    {
        uiSum = uiSum + SEND_BUFFER[i];
    }

    //ȡ��
    uiSum = ~uiSum;

    //У����
    SEND_BUFFER[6] = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    SEND_BUFFER[7] = (unsigned char)uiSum & 0xff;          //��λ

#ifdef ENABLE_485

    USART_485_Enable();

#endif

    //�豸ID
    for (i = 0; i < 8; i++)
    {
        USART2_Send_Byte(SEND_BUFFER[i]);
    }

#ifdef ENABLE_485

    USART_485_Disable();

#endif

    // Delay_72M_Ms(50);

    // //У����
    // ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    // ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    // USART2_Send_Byte(ucHighSum);
    // USART2_Send_Byte(ucLowSum);
}

void System_Send_Return_Total_Days_Coin(unsigned char ucTargetID, unsigned char ucCommand, unsigned char ucStatus)
{

    //  unsigned char ucHighSum = 0;
    //  unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned char i = 0;
    unsigned char SEND_BUFFER[30];

    // Game_Status_Save();

    // if()

    //���㷢��У����
    // uiSum = 13 + 2;

    SEND_BUFFER[0] = 0x7F;           //��ͷ
    SEND_BUFFER[1] = ucTargetID;     //Ŀ��ID
    SEND_BUFFER[2] = GAME_STATUS.ID; //����ID
    SEND_BUFFER[3] = 2;              //����
    SEND_BUFFER[4] = ucCommand;      //��Ӧָ��

    SEND_BUFFER[5] = ucStatus;

    SEND_BUFFER[6] = (unsigned char)((GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 24) & 0xff); //��λ
    SEND_BUFFER[7] = (unsigned char)((GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 16) & 0xff); //��λ
    SEND_BUFFER[8] = (unsigned char)((GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 8) & 0xff);  //��λ
    SEND_BUFFER[9] = (unsigned char)GAME_STATUS.TOTAL_INSERT_COIN_COUNT & 0xff;           //��λ

    SEND_BUFFER[10] = (unsigned char)((GAME_STATUS.MACHINE_INSERT_COUNT >> 24) & 0xff); //��λ
    SEND_BUFFER[11] = (unsigned char)((GAME_STATUS.MACHINE_INSERT_COUNT >> 16) & 0xff); //��λ
    SEND_BUFFER[12] = (unsigned char)((GAME_STATUS.MACHINE_INSERT_COUNT >> 8) & 0xff);  //��λ
    SEND_BUFFER[13] = (unsigned char)GAME_STATUS.MACHINE_INSERT_COUNT & 0xff;           //��λ



    //����У����
    for (i = 1; i < 14; i++)
    {
        uiSum = uiSum + SEND_BUFFER[i];
    }

    //ȡ��
    uiSum = ~uiSum;

    //У����
    SEND_BUFFER[14] = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    SEND_BUFFER[15] = (unsigned char)uiSum & 0xff;          //��λ

#ifdef ENABLE_485

    USART_485_Enable();

#endif

    //�豸ID
    for (i = 0; i < 16; i++)
    {
        USART2_Send_Byte(SEND_BUFFER[i]);
    }

#ifdef ENABLE_485

    USART_485_Disable();

#endif

    // Delay_72M_Ms(50);

    // //У����
    // ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    // ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    // USART2_Send_Byte(ucHighSum);
    // USART2_Send_Byte(ucLowSum);
}

void System_Send_Return_Version(unsigned char ucTargetID, unsigned char ucCommand, unsigned char ucStatus)
{

    //  unsigned char ucHighSum = 0;
    //  unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned char i = 0;
    unsigned char SEND_BUFFER[30];

    // Game_Status_Save();

    // if()

    //���㷢��У����
    // uiSum = 13 + 2;

    SEND_BUFFER[0] = 0x7F;           //��ͷ
    SEND_BUFFER[1] = ucTargetID;     //Ŀ��ID
    SEND_BUFFER[2] = GAME_STATUS.ID; //����ID
    SEND_BUFFER[3] = 2;              //����
    SEND_BUFFER[4] = ucCommand;      //��Ӧָ��

    SEND_BUFFER[5] = ucStatus;

    SEND_BUFFER[6] = H_VERSION;
    SEND_BUFFER[7] = L_VERSION;

    //����У����
    for (i = 1; i < 8; i++)
    {
        uiSum = uiSum + SEND_BUFFER[i];
    }

    //ȡ��
    uiSum = ~uiSum;

    //У����
    SEND_BUFFER[8] = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    SEND_BUFFER[9] = (unsigned char)uiSum & 0xff;          //��λ

#ifdef ENABLE_485

    USART_485_Enable();

#endif

    //�豸ID
    for (i = 0; i < 10; i++)
    {
        USART2_Send_Byte(SEND_BUFFER[i]);
    }

#ifdef ENABLE_485

    USART_485_Disable();

#endif

    // Delay_72M_Ms(50);

    // //У����
    // ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    // ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    // USART2_Send_Byte(ucHighSum);
    // USART2_Send_Byte(ucLowSum);
}

void System_Send_Device_ID(void)
{
    unsigned char ucHighSum = 0;
    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned char i = 0;

    //���㷢��У����
    uiSum = 13 + 2;

    for (i = 0; i < 12; i++)
    {
        uiSum = uiSum + SYSTEM_DEVICE_ID[i];
    }

    //ȡ��
    uiSum = ~uiSum;

    USART2_Send_Byte(0xFC); //��ͷ
    USART2_Send_Byte(0x66);

    USART2_Send_Byte(13);   //���� ָ���� �� 12λiD һ��13λ
    USART2_Send_Byte(0x02); //ָ����

    //�豸ID
    for (i = 0; i < 12; i++)
    {
        USART2_Send_Byte(SYSTEM_DEVICE_ID[i]);
    }

    //У����
    ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    USART2_Send_Byte(ucHighSum);
    USART2_Send_Byte(ucLowSum);
}

void System_Setting_Count(unsigned int uiCoin_Count)
{
    // USART1_Send_Debug_Message("������Ϸ��\r\n");
    GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + uiCoin_Count;
    GAME_STATUS.TOTAL_INSERT_COIN_COUNT = GAME_STATUS.TOTAL_INSERT_COIN_COUNT + uiCoin_Count;

     Game_Status_Save();
     Game_Status_Read();

}

void System_Setting_Game_Coins(unsigned int uiCoin_Count)
{
    //   USART1_Send_Debug_Message("������Ϸ����:\r\n");
    GAME_STATUS.GAME_COINS = uiCoin_Count;
    //	Delay_72M_Ms(30);
    Game_Status_Save();
    //	Delay_72M_Ms(30);
}

void System_Setting_Game_Disable(unsigned char ucStatus)
{
    USART1_Send_Debug_Message("���ý�ֹ��Ϸ��״̬:");
    GAME_STATUS.DISABLE = ucStatus;

    switch (ucStatus)
    {
    case _SYSTEM_UNLOCK:
        GAME_STATUS.DISABLE = _SYSTEM_UNLOCK;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_UNLOCK;
        Game_Insert_Coin_Enable();

        USART1_Send_Debug_Message("δ����\r\n");

        break;

    case _SYSTEM_RESTART_LOCK:

        GAME_STATUS.DISABLE = _SYSTEM_RESTART_LOCK;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_RESTART_LOCK;
        Game_Insert_Coin_Disable();

        USART1_Send_Debug_Message("ϵͳ��λ����\r\n");

        break;

    case _SYSTEM_EGG_TIME_OUT_LOCK:

        GAME_STATUS.DISABLE = _SYSTEM_EGG_TIME_OUT_LOCK;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_EGG_TIME_OUT_LOCK;
        Game_Insert_Coin_Disable();

        USART1_Send_Debug_Message("������ʱ\r\n");

        break;

    case _SYSTEM_NET_CMD_LOCK:
        GAME_STATUS.DISABLE = _SYSTEM_NET_CMD_LOCK;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_NET_CMD_LOCK;
        Game_Insert_Coin_Disable();

        USART1_Send_Debug_Message("����ָ������\r\n");
        break;

    case _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE:
        GAME_STATUS.DISABLE = _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE;
        Game_Insert_Coin_Disable();

        USART1_Send_Debug_Message("����ָ������,����Ͷ������\r\n");
        break;

    case _SYSTEM_LEYAOYAO_LOCK:
        GAME_STATUS.DISABLE = _SYSTEM_LEYAOYAO_LOCK;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_LEYAOYAO_LOCK;
    //    Game_Insert_Coin_Enable();
        Game_Insert_Coin_Disable();

        USART1_Send_Debug_Message("��ҡҡ��������\r\n");
        break;


    default:

        GAME_STATUS.DISABLE = _SYSTEM_RESTART_LOCK;
        GAME_STATUS.CURRENT_LOCK_STATUS = _SYSTEM_RESTART_LOCK;
        Game_Insert_Coin_Disable();

        USART1_Send_Debug_Message("ϵͳ��λ������δ֪������\r\n");

        break;
    }
}

void System_Setting_Play_Music(unsigned int uiCoin_Count)
{
    //   USART1_Send_Debug_Message("��������:\r\n");
    VOICE_Play(uiCoin_Count);
    // Game_Status_Save();
}

void System_Setting_Light(unsigned int uiCoin_Count)
{
    //  USART1_Send_Debug_Message("���õƹ�\r\n");
    //VOICE_Play(uiCoin_Count);
    if (uiCoin_Count == 1)
    {
        Light1_On();
        Light2_On();
    }
    else
    {
        Light1_Off();
        Light2_Off();
    }

    // Game_Status_Save();
}

void System_Setting_Coin_Insert(unsigned int uiSettings)
{
    //  USART1_Send_Debug_Message("���õƹ�\r\n");
    //VOICE_Play(uiCoin_Count);
    if (uiSettings == 1)
    {
        Game_Insert_Coin_Enable();
    }
    else
    {
        Game_Insert_Coin_Disable();
    }

    // Game_Status_Save();
}

//����ID
void System_Setting_ID(unsigned char ucTargetID, unsigned char System_Setting_ID)
{
    // unsigned char ucHighSum = 0;
    // unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned char i = 0;
    unsigned char SEND_BUFFER[30];

    GAME_STATUS.ID = System_Setting_ID;

    Game_Status_Save();

    // if()

    //���㷢��У����
    // uiSum = 13 + 2;

    SEND_BUFFER[0] = 0x7F;                //��ͷ
    SEND_BUFFER[1] = ucTargetID;          //Ŀ��ID
    SEND_BUFFER[2] = GAME_STATUS.ID;      //����ID
    SEND_BUFFER[3] = 23;                  //����
    SEND_BUFFER[4] = 1;                   //��Ӧָ��
    SEND_BUFFER[5] = SYSTEM_DEVICE_ID[0]; //��Ϸ������λ
    SEND_BUFFER[6] = SYSTEM_DEVICE_ID[1]; //��Ϸ������λ
    SEND_BUFFER[7] = SYSTEM_DEVICE_ID[2];
    SEND_BUFFER[8] = SYSTEM_DEVICE_ID[3];
    SEND_BUFFER[9] = SYSTEM_DEVICE_ID[4];
    SEND_BUFFER[10] = SYSTEM_DEVICE_ID[5];
    SEND_BUFFER[11] = SYSTEM_DEVICE_ID[6];
    SEND_BUFFER[12] = SYSTEM_DEVICE_ID[7];
    SEND_BUFFER[13] = SYSTEM_DEVICE_ID[8];
    SEND_BUFFER[14] = SYSTEM_DEVICE_ID[9];
    SEND_BUFFER[15] = SYSTEM_DEVICE_ID[10];
    SEND_BUFFER[16] = SYSTEM_DEVICE_ID[11];

    SEND_BUFFER[17] = (unsigned char)((GAME_STATUS.GAME_COUNT >> 24) & 0xff); //��λ
    SEND_BUFFER[18] = (unsigned char)((GAME_STATUS.GAME_COUNT >> 16) & 0xff); //��λ
    SEND_BUFFER[19] = (unsigned char)((GAME_STATUS.GAME_COUNT >> 8) & 0xff);  //��λ
    SEND_BUFFER[20] = (unsigned char)GAME_STATUS.GAME_COUNT & 0xff;           //��λ

    SEND_BUFFER[21] = (unsigned char)((GAME_STATUS.GAME_EGG_OUT_COUNT >> 24) & 0xff); //��λ
    SEND_BUFFER[22] = (unsigned char)((GAME_STATUS.GAME_EGG_OUT_COUNT >> 16) & 0xff); //��λ
    SEND_BUFFER[23] = (unsigned char)((GAME_STATUS.GAME_EGG_OUT_COUNT >> 8) & 0xff);  //��λ
    SEND_BUFFER[24] = (unsigned char)GAME_STATUS.GAME_EGG_OUT_COUNT & 0xff;

    // SEND_BUFFER[25] = (unsigned char)((GAME_STATUS.GAME_COINS >> 8) & 0xff); //��λ
    SEND_BUFFER[25] = (unsigned char)GAME_STATUS.GAME_COINS & 0xff;
    //  =
    //  = (u8)((temp0 & 0x0000FF00) >> 8);
    //  = (u8)((temp0 & 0x00FF0000) >> 16);
    //  = (u8)((temp0 & 0xFF000000) >> 24);
    //  = (u8)(temp1 & 0x000000FF);
    // SYSTEM_DEVICE_ID[5] = (u8)((temp1 & 0x0000FF00) >> 8);
    // SYSTEM_DEVICE_ID[6] = (u8)((temp1 & 0x00FF0000) >> 16);
    // SYSTEM_DEVICE_ID[7] = (u8)((temp1 & 0xFF000000) >> 24);
    // SYSTEM_DEVICE_ID[8] = (u8)(temp2 & 0x000000FF);
    // SYSTEM_DEVICE_ID[9] = (u8)((temp2 & 0x0000FF00) >> 8);
    // SYSTEM_DEVICE_ID[10] = (u8)((temp2 & 0x00FF0000) >> 16);
    // SYSTEM_DEVICE_ID[11] = (u8)((temp2 & 0xFF000000) >> 24);

    //����У����
    for (i = 1; i < 26; i++)
    {
        uiSum = uiSum + SEND_BUFFER[i];
    }

    //ȡ��
    uiSum = ~uiSum;

    //У����
    SEND_BUFFER[26] = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    SEND_BUFFER[27] = (unsigned char)uiSum & 0xff;          //��λ

#ifdef ENABLE_485

    USART_485_Enable();

#endif

    //�豸ID
    for (i = 0; i < 28; i++)
    {
        USART2_Send_Byte(SEND_BUFFER[i]);
    }

#ifdef ENABLE_485

    USART_485_Disable();

#endif

    // Delay_72M_Ms(50);

    // //У����
    // ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    // ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    // USART2_Send_Byte(ucHighSum);
    // USART2_Send_Byte(ucLowSum);
}

//�����豸״̬
void System_Send_Status(unsigned char ucTargetID)
{
    //    unsigned char ucHighSum = 0;
    //    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned char i = 0;
    unsigned char SEND_BUFFER[90];

    //���㷢��У����
    // uiSum = 13 + 2;

    // USART1_Send_Debug_Message("���״̬\r\n");

    // GAME_STATUS.ERROR = GAME_STATUS.DISABLE;

    SEND_BUFFER[0] = 0x7F;           //��ͷ
    SEND_BUFFER[1] = ucTargetID;     //Ŀ��ID
    SEND_BUFFER[2] = GAME_STATUS.ID; //����ID
    SEND_BUFFER[3] = 20;             //����
    SEND_BUFFER[4] = 2;              //��Ӧָ��

    //��ǰ����Ϸ����
    SEND_BUFFER[5] = (unsigned char)((GAME_STATUS.COIN_COUNT >> 24) & 0xFF); //��Ϸ������λ
    SEND_BUFFER[6] = (unsigned char)((GAME_STATUS.COIN_COUNT >> 16) & 0xFF); //��Ϸ������λ
    SEND_BUFFER[7] = (unsigned char)((GAME_STATUS.COIN_COUNT >> 8) & 0xFF);  //��Ϸ������λ
    SEND_BUFFER[8] = (unsigned char)(GAME_STATUS.COIN_COUNT & 0xFF);         //��Ϸ������λ

    //ÿ����Ϸ����
    SEND_BUFFER[9] = GAME_STATUS.GAME_COINS; // ÿ�ֵı���

    //��ǰ���е��ܾ���
    SEND_BUFFER[10] = (unsigned char)((GAME_STATUS.GAME_COUNT >> 24) & 0xFF); //��Ϸ���� ��
    SEND_BUFFER[11] = (unsigned char)((GAME_STATUS.GAME_COUNT >> 16) & 0xFF); //��Ϸ���� ��
    SEND_BUFFER[12] = (unsigned char)((GAME_STATUS.GAME_COUNT >> 8) & 0xFF);  //��Ϸ���� ��
    SEND_BUFFER[13] = (unsigned char)(GAME_STATUS.GAME_COUNT & 0xFF);         //��Ϸ���� ��

    //��Ȼ��Ϸ���ܳ�����
    SEND_BUFFER[14] = (unsigned char)((GAME_STATUS.GAME_EGG_OUT_COUNT >> 24) & 0xFF); //��Ϸ���� ��
    SEND_BUFFER[15] = (unsigned char)((GAME_STATUS.GAME_EGG_OUT_COUNT >> 16) & 0xFF); //��Ϸ���� ��
    SEND_BUFFER[16] = (unsigned char)((GAME_STATUS.GAME_EGG_OUT_COUNT >> 8) & 0xFF);  //��Ϸ���� ��
    SEND_BUFFER[17] = (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF);         //��Ϸ���� ��

    //����״̬
    SEND_BUFFER[18] = GAME_STATUS.LIGHT_1;
    SEND_BUFFER[19] = GAME_STATUS.LIGHT_2;

    //��ǰ����״̬
    SEND_BUFFER[20] = GAME_STATUS.CURRENT_LOCK_STATUS;

    //Ͷ������Դ״̬
    SEND_BUFFER[21] = GAME_STATUS.COIN_MACHINE_STATUS;

    //LED�����״̬
    SEND_BUFFER[22] = GAME_STATUS.LED_1_STATUS;
    SEND_BUFFER[23] = GAME_STATUS.LED_2_STATUS;

    //����У����
    for (i = 1; i < 24; i++)
    {
        uiSum = uiSum + SEND_BUFFER[i];
    }

    //ȡ��
    uiSum = ~uiSum;

    //У����
    SEND_BUFFER[24] = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    SEND_BUFFER[25] = (unsigned char)uiSum & 0xff;          //��λ

#ifdef ENABLE_485

    USART_485_Enable();

#endif

    //�豸ID
    for (i = 0; i < 26; i++)
    {
        USART2_Send_Byte(SEND_BUFFER[i]);
    }

#ifdef ENABLE_485

    //Delay_72M_Ms(40);

    USART_485_Disable();

#endif

    //Delay_72M_Ms(10);
}

//���У��λ
unsigned char System_Check_Receive_Match(void)
{
    unsigned char ucHighSum = 0;
    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //��1��ʼУ��
    for (i = 1; i < SYSTEM_BUFFER_RECEVICE_COUNT - 2; i++)
    {
        uiSum = uiSum + SYSTEM_USART_BUFFER[i];
    }

    uiSum = ~uiSum;

    ucHighSum = (unsigned char)((uiSum >> 8) & 0xff); //��λ
    ucLowSum = (unsigned char)uiSum & 0xff;           //��λ

    // USART1_Send_String("У���λ:");

    //  USART1_Send_Hex(ucHighSum);

    //  USART1_Send_Hex(SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT - 2]);

    //   USART1_Send_String("У���λ:");

    //  USART1_Send_Hex(ucLowSum);

    //  USART1_Send_Hex(SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT-1]);

    //   USART1_Send_String("\r\n");

    if (ucHighSum == SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT - 2] && ucLowSum == SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT - 1])
    {
        // USART1_Send_Byte(0xEE);
        return 1;
    }

    return 0;
}

unsigned char System_Check_Receive(void)
{

    switch (SYSTEM_RECEIVE_STATE)
    {

    //�����ճ������ͷ
    case 0:

        if (SYSTEM_BUFFER_RECEVICE_COUNT >= 5)
        {

            Delay_72M_Ms(30);

            //  __Debug_Send_Recevie_Buffer();

            if (SYSTEM_USART_BUFFER[0] == 0x7E)
            {

                //���������ȷ,�����У��
                if (SYSTEM_BUFFER_RECEVICE_COUNT == (SYSTEM_USART_BUFFER[3] + 6))
                {
                    USART1_Send_String("������ȷ\r\n");

                    if (SYSTEM_USART_BUFFER[1] == GAME_STATUS.ID || SYSTEM_USART_BUFFER[1] == 0) //�ж�ID�Ƿ���ȷ
                    {
                        USART1_Send_String("ID��ȷ\r\n");

                        USART1_Send_Hex(GAME_STATUS.ID);
                        GAME_STATUS.TARGET_ID = SYSTEM_USART_BUFFER[2];
                        SYSTEM_RECEIVE_STATE = 1; //������һ�׶�

                        //			   USART1_Send_String("ID��ȷ\r\n");
                    }
                    else
                    {
                        USART1_Send_String("���ȴ���:\r\n");

                        // __Debug_Send_Recevie_Buffer();

                        // Delay_72M_Ms(50);

                        System_Buffer_Clean();
                        SYSTEM_RECEIVE_STATE = 0; //
                    }

                    return 1;
                }
                else
                {
                    SYSTEM_BUFFER_RECEVICE_ERROR++;

                    if (SYSTEM_BUFFER_RECEVICE_ERROR >= 300)
                    {
                        //  USART1_Send_Byte(0xbb);

                        USART1_Send_String("ָ�����\r\n");

                        //	 __Debug_Send_Recevie_Buffer();
                        //   Delay_72M_Ms(50);
                        System_Buffer_Clean();
                        SYSTEM_RECEIVE_STATE = 0; //
                        return 1;

                        //     return 0;
                    }
                } //         System_Buffer_Clean();

                //       USART1_Send_Byte(0xaa);

                //  USART2_Send_String("HELLO!!!!");
                // #ifdef DEBUG_USART

                // __Debug_Send_Recevie_Buffer();
                // #endif

                //  __Debug_Send_Recevie_Buffer();

                // USART1_Send_Byte(SYSTEM_BUFFER_RECEVICE_COUNT);
                //  System_Buffer_Clean();
            }
            else
            {
                USART1_Send_String("cuowuo");
                //   __Debug_Send_Recevie_Buffer();
                //    Delay_72M_Ms(50);
                System_Buffer_Clean();
                SYSTEM_RECEIVE_STATE = 0; //
                return 0;
            }
        }

        break;

        //����У����
    case 1:

        if (System_Check_Receive_Match())
        {
            USART1_Send_String("У��ɹ�\r\n");
            //	   	LED_Blue_On();
            SYSTEM_RECEIVE_STATE = 2; //

            //		__Debug_Send_Recevie_Buffer();
        }
        else
        {
            USART1_Send_String("У��ʧ��\r\n");

            System_Buffer_Clean();

            SYSTEM_RECEIVE_STATE = 0; //
        }
        // System_Buffer_Clean();

        // SYSTEM_RECEIVE_STATE = 0; //

        break;

    //ִ��ָ��
    case 2:

        USART1_Send_String("ִ��ָ��\r\n");

        //USART1_Send_Byte(0xAA);
        //USART1_Send_Byte(0xBB);
        //USART1_Send_Byte(0xCC);

        switch (SYSTEM_USART_BUFFER[4])
        {

        case 1: //����ID

            //USART1_Send_Hex(get_Setting_Key_Status());

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

                    System_Setting_ID(GAME_STATUS.TARGET_ID, SYSTEM_USART_BUFFER[5]);

                    while (get_Setting_Key_Status() != 0x01)
                        ;
                    LED_Blue_Off();

                    //***add
                    Light1_On();
                    Light2_On();
                    //***add
                }
            }

            System_Buffer_Clean();
            SYSTEM_RECEIVE_STATE = 0; //
            break;

        case 2: //��ò�Ʒ״̬

            System_Send_Status(GAME_STATUS.TARGET_ID);

            // 		 System_Buffer_Clean();
            // SYSTEM_RECEIVE_STATE = 0; //
            break;

        case 3: //Ͷ��

            Coin_Insert_Add(NET_ADD);
            break;

        case 4:

            if (SYSTEM_USART_BUFFER[5] != 0 && SYSTEM_USART_BUFFER[5] != 0xFF)
            {
                System_Setting_Game_Coins(SYSTEM_USART_BUFFER[5]);
                System_Send_Return_Status(GAME_STATUS.TARGET_ID, 4, 1); //3ָ���룬1 TRUE
            }
            else
            {
                System_Send_Return_Status(GAME_STATUS.TARGET_ID, 4, 0); //3ָ���룬1 TRUE
            }

            // System_Buffer_Clean();
            // SYSTEM_RECEIVE_STATE = 0; //
            break;

        case 5: //��ֹ��Ϸ

            USART1_Send_String("����ָ��: \r\n");
            USART1_Send_Hex(SYSTEM_USART_BUFFER[5]);
            USART1_Send_String("\r\n");

            switch (SYSTEM_USART_BUFFER[5])
            {
            case _SYSTEM_UNLOCK:
                System_Setting_Game_Disable(_SYSTEM_UNLOCK);
                System_Send_Return_Status(GAME_STATUS.TARGET_ID, 5, 1); //3ָ���룬1 TRUE

                break;

            case _SYSTEM_NET_CMD_LOCK:

                System_Setting_Game_Disable(_SYSTEM_NET_CMD_LOCK);
                System_Send_Return_Status(GAME_STATUS.TARGET_ID, 5, 1); //3ָ���룬1 TRUE
                break;

            case _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE:

                System_Setting_Game_Disable(_SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE);
                System_Send_Return_Status(GAME_STATUS.TARGET_ID, 5, 1); //3ָ���룬1 TRUE
                break;

            default:

                System_Send_Return_Status(GAME_STATUS.TARGET_ID, 5, 0); //3ָ���룬1 TRUE

                break;
            }

            break;

        case 6: //��������
            System_Setting_Play_Music(SYSTEM_USART_BUFFER[5]);
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 6, 1); //3ָ���룬1 TRUE

            break;

        case 7: //��������
            //System_Setting_Play_Music();
            System_Setting_Light(SYSTEM_USART_BUFFER[5]);
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 7, 1); //3ָ���룬1 TRUE

            break;

        case 8:
            System_Setting_Coin_Insert(SYSTEM_USART_BUFFER[5]);
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 8, 1); //3ָ���룬1 TRUE

            break;

        case 9: //��ð汾��

            System_Send_Return_Version(GAME_STATUS.TARGET_ID, 9, 1); //3ָ���룬1 TRUE

            break;

        case 10: //��õ�ͷ��Ͷ����

        System_Send_Return_Total_Days_Coin(GAME_STATUS.TARGET_ID, 10, 1);


            break;

        default:
            break;
        }

        //		LED_Blue_Off();
        System_Buffer_Clean();
        SYSTEM_RECEIVE_STATE = 0; //

        break;

    default:

        break;
    }

    return 0;
}
