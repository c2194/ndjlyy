#include "global.h"

unsigned char LEYAOYAO_REC_BUFFER[LEYAOYAO_REC_BUFFER_COUNT];
unsigned int LEYAOYAO_REC_BUFFER_RECEVICE_COUNT = 0; //��ǰ���յ�����
unsigned char LEYAOYAO_RECEVICE_STATE = 0;           //���յ�״̬
unsigned int LEYAOYAO_RECEVICE_ERROR = 0;
unsigned char LAST_PAYMENT_ID[10];
unsigned char LAST_PAYMENT_COINS;

//debug �����Ϣ
void __Debug_Send_Leyaoyao_Recevie_Buffer(void)
{
    unsigned char i = 0;

    USART1_Send_String("Rec: ");

    for (i = 0; i < LEYAOYAO_REC_BUFFER_RECEVICE_COUNT; i++)
    {
        USART1_Send_Hex(LEYAOYAO_REC_BUFFER[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    USART1_Send_String("Count: ");
    USART1_Send_Dec(LEYAOYAO_REC_BUFFER_RECEVICE_COUNT, 4);
    USART1_Send_String("\r\n");

    Delay_72M_Ms(200);
}

void leyaoyao_return_yun_coins_pay_jieguo(unsigned char ucFlag)
{
    //���Ϸֽ������
    unsigned char ucSum = 0;
    unsigned char i = 0;
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 14;   //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x05; //������cmd
    leyaoyao_send_buffer[6] = 0x01; //������cmd

    leyaoyao_send_buffer[7] = LAST_PAYMENT_ID[0]; //֧��������
    leyaoyao_send_buffer[8] = LAST_PAYMENT_ID[1];
    leyaoyao_send_buffer[9] = LAST_PAYMENT_ID[2];
    leyaoyao_send_buffer[10] = LAST_PAYMENT_ID[3];
    leyaoyao_send_buffer[11] = LAST_PAYMENT_ID[4];
    leyaoyao_send_buffer[12] = LAST_PAYMENT_ID[5];

    if (ucFlag != 0)
    {
        leyaoyao_send_buffer[13] = LAST_PAYMENT_COINS; //���
    }
    else
    {
        leyaoyao_send_buffer[13] = 0;
    }

    // leyaoyao_send_buffer[13] = LEYAOYAO_REC_BUFFER[13];
    leyaoyao_send_buffer[14] = LEYAOYAO_REC_BUFFER[14];

    //  LAST_PAYMENT_ID[0] = LEYAOYAO_REC_BUFFER[7]; //֧��������
    // LAST_PAYMENT_ID[1] = LEYAOYAO_REC_BUFFER[8];
    // LAST_PAYMENT_ID[2] = LEYAOYAO_REC_BUFFER[9];
    // LAST_PAYMENT_ID[3] = LEYAOYAO_REC_BUFFER[10];
    // LAST_PAYMENT_ID[4] = LEYAOYAO_REC_BUFFER[11];
    // LAST_PAYMENT_ID[5] = LEYAOYAO_REC_BUFFER[12];
    // LAST_PAYMENT_ID[6] = LEYAOYAO_REC_BUFFER[13];

    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[16] = 0xDD;

    //leyaoyao_send_buffer[26] = 0xDD; //У����

    //��1��ʼУ��
    for (i = 1; i < 17 - 2; i++)
    {
        ucSum = ucSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[15] = ucSum;

    Delay_72M_Ms(500);

    for (i = 0; i < 17; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }

    USART1_Send_String("���Ϸֽ������(���巢): ");
    for (i = 0; i < 17; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    // USART1_Send_String("��ҡҡ֧��:\r\n");

    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    //  USART1_Send_String("\r\n");

    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    //  USART1_Send_String("\r\n");

    //����ָ��

    //GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + GAME_STATUS.GAME_COINS;

    System_Setting_Count(LEYAOYAO_REC_BUFFER[14]);
    //  System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 1); //3ָ���룬1 TRUE

    if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    {

        VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //���˿�ʼŤ����

        if (GAME_STATUS.EGG_OUT_ING != 1) //�����ǰ��ʧŤ����
        {
            GAME_STATUS.SWITCH_OUT_READEY = 1;
        }

        GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
    }

    //У�������

    // ucSum = ucSum ^ 13 ^ 0x02^0xCC ^ 0x1 ^ 0x03 ^ 0x01;

    // USART1_Send_String("aaaa:\r\n");
    // USART1_Send_Hex(ucSum);
    // USART1_Send_String("\r\n");

    // for(i=7;i<12;i++)
    // {
    //     ucSum = ucSum ^ LEYAOYAO_REC_BUFFER[i];
    //       USART1_Send_String("B:\r\n");
    // USART1_Send_Hex(ucSum);
    // USART1_Send_String("\r\n");
    // }

    // USART3_Send_Byte(0xAA);
    // USART3_Send_Byte(13);
    // USART3_Send_Byte(0x02);
    // USART3_Send_Byte(0xCC);
    // USART3_Send_Byte(0x01); //�豸ID
    // USART3_Send_Byte(0x03); //֧������Ӧ��
    // USART3_Send_Byte(0x1);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[7]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[8]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[9]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[10]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[11]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[12]);
    // USART3_Send_Byte(0x0); //��λ״̬
    //   USART3_Send_Byte(ucSum); //jУ��λ
    //   USART3_Send_Byte(0xDD);

    //       USART1_Send_Hex(0xAA);
    // USART1_Send_Hex(13);
    // USART1_Send_Hex(0x02);
    // USART1_Send_Hex(0xCC);
    // USART1_Send_Hex(0x01); //�豸ID
    // USART1_Send_Hex(0x03); //֧������Ӧ��
    // USART1_Send_Hex(0x1);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    // USART1_Send_Hex(0x0); //��λ״̬
    //   USART1_Send_Hex(ucSum); //jУ��λ
    //   USART1_Send_Hex(0xDD);
}

void leyaoyao_return_yun_coins_pay(void)
{
    unsigned char ucSum = 0;
    unsigned char i = 0;
    unsigned char leyaoyao_send_buffer[60];

    USART1_Send_String("���Ϸֹ���\r\n");

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    LAST_PAYMENT_ID[0] = LEYAOYAO_REC_BUFFER[7]; //֧��������
    LAST_PAYMENT_ID[1] = LEYAOYAO_REC_BUFFER[8];
    LAST_PAYMENT_ID[2] = LEYAOYAO_REC_BUFFER[9];
    LAST_PAYMENT_ID[3] = LEYAOYAO_REC_BUFFER[10];
    LAST_PAYMENT_ID[4] = LEYAOYAO_REC_BUFFER[11];
    LAST_PAYMENT_ID[5] = LEYAOYAO_REC_BUFFER[12];
    //LAST_PAYMENT_ID[6] = LEYAOYAO_REC_BUFFER[13];

    LAST_PAYMENT_COINS = LEYAOYAO_REC_BUFFER[13];

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 14;   //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x04; //������cmd
    leyaoyao_send_buffer[6] = 0x01; //������cmd

    leyaoyao_send_buffer[7] = LEYAOYAO_REC_BUFFER[7]; //֧��������
    leyaoyao_send_buffer[8] = LEYAOYAO_REC_BUFFER[8];
    leyaoyao_send_buffer[9] = LEYAOYAO_REC_BUFFER[9];
    leyaoyao_send_buffer[10] = LEYAOYAO_REC_BUFFER[10];
    leyaoyao_send_buffer[11] = LEYAOYAO_REC_BUFFER[11];
    leyaoyao_send_buffer[12] = LEYAOYAO_REC_BUFFER[12];
    leyaoyao_send_buffer[13] = LEYAOYAO_REC_BUFFER[13]; //CONIS;                       //CONIS
    leyaoyao_send_buffer[14] = LEYAOYAO_REC_BUFFER[14];

    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[16] = 0xDD;

    //leyaoyao_send_buffer[26] = 0xDD; //У����

    //��1��ʼУ��
    for (i = 1; i < 17 - 2; i++)
    {
        ucSum = ucSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[15] = ucSum;

    Delay_72M_Ms(500);

    for (i = 0; i < 17; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }

    USART1_Send_String("���Ϸֹ��ܷ���ָ��: ");

    for (i = 0; i < 17; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    // USART1_Send_String("��ҡҡ֧��:\r\n");

    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    //  USART1_Send_String("\r\n");

    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    //  USART1_Send_String("\r\n");

    //����ָ��

    //GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + GAME_STATUS.GAME_COINS;

    System_Setting_Count(LEYAOYAO_REC_BUFFER[13]);
    //  System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 1); //3ָ���룬1 TRUE

    if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    {

        VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //���˿�ʼŤ����

        if (GAME_STATUS.EGG_OUT_ING != 1) //�����ǰ��ʧŤ����
        {
            GAME_STATUS.SWITCH_OUT_READEY = 1;
        }

        GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
    }

    //У�������

    // ucSum = ucSum ^ 13 ^ 0x02^0xCC ^ 0x1 ^ 0x03 ^ 0x01;

    // USART1_Send_String("aaaa:\r\n");
    // USART1_Send_Hex(ucSum);
    // USART1_Send_String("\r\n");

    // for(i=7;i<12;i++)
    // {
    //     ucSum = ucSum ^ LEYAOYAO_REC_BUFFER[i];
    //       USART1_Send_String("B:\r\n");
    // USART1_Send_Hex(ucSum);
    // USART1_Send_String("\r\n");
    // }

    // USART3_Send_Byte(0xAA);
    // USART3_Send_Byte(13);
    // USART3_Send_Byte(0x02);
    // USART3_Send_Byte(0xCC);
    // USART3_Send_Byte(0x01); //�豸ID
    // USART3_Send_Byte(0x03); //֧������Ӧ��
    // USART3_Send_Byte(0x1);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[7]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[8]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[9]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[10]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[11]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[12]);
    // USART3_Send_Byte(0x0); //��λ״̬
    //   USART3_Send_Byte(ucSum); //jУ��λ
    //   USART3_Send_Byte(0xDD);

    //       USART1_Send_Hex(0xAA);
    // USART1_Send_Hex(13);
    // USART1_Send_Hex(0x02);
    // USART1_Send_Hex(0xCC);
    // USART1_Send_Hex(0x01); //�豸ID
    // USART1_Send_Hex(0x03); //֧������Ӧ��
    // USART1_Send_Hex(0x1);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    // USART1_Send_Hex(0x0); //��λ״̬
    //   USART1_Send_Hex(ucSum); //jУ��λ
    //   USART1_Send_Hex(0xDD);
}

void leyaoyao_return_pay_field(void)
{
    unsigned char ucSum = 0;
    unsigned char i = 0;
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 13;   //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x03; //������cmd
    leyaoyao_send_buffer[6] = 0x01; //������cmd

    leyaoyao_send_buffer[7] = LEYAOYAO_REC_BUFFER[7]; //֧��������
    leyaoyao_send_buffer[8] = LEYAOYAO_REC_BUFFER[8];
    leyaoyao_send_buffer[9] = LEYAOYAO_REC_BUFFER[9];
    leyaoyao_send_buffer[10] = LEYAOYAO_REC_BUFFER[10];
    leyaoyao_send_buffer[11] = LEYAOYAO_REC_BUFFER[11];
    leyaoyao_send_buffer[12] = LEYAOYAO_REC_BUFFER[12];

    leyaoyao_send_buffer[13] = 3;

    leyaoyao_send_buffer[14] = 0;
    leyaoyao_send_buffer[15] = 0xDD;

    //��1��ʼУ��
    for (i = 1; i < 16 - 2; i++)
    {
        ucSum = ucSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[14] = ucSum;

    Delay_72M_Ms(500);

    for (i = 0; i < 16; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }

    USART1_Send_String("֧�������쳣����ָ��(���巢): ");

    for (i = 0; i < 16; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");
}

void leyaoyao_return_pay(void)
{
    unsigned char ucSum = 0;
    unsigned char i = 0;
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 13;   //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x03; //������cmd
    leyaoyao_send_buffer[6] = 0x01; //������cmd

    leyaoyao_send_buffer[7] = LEYAOYAO_REC_BUFFER[7]; //֧��������
    leyaoyao_send_buffer[8] = LEYAOYAO_REC_BUFFER[8];
    leyaoyao_send_buffer[9] = LEYAOYAO_REC_BUFFER[9];
    leyaoyao_send_buffer[10] = LEYAOYAO_REC_BUFFER[10];
    leyaoyao_send_buffer[11] = LEYAOYAO_REC_BUFFER[11];
    leyaoyao_send_buffer[12] = LEYAOYAO_REC_BUFFER[12];

    leyaoyao_send_buffer[13] = 0;

    leyaoyao_send_buffer[14] = 0;
    leyaoyao_send_buffer[15] = 0xDD;

    //leyaoyao_send_buffer[26] = 0xDD; //У����

    //��1��ʼУ��
    for (i = 1; i < 16 - 2; i++)
    {
        ucSum = ucSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[14] = ucSum;

    Delay_72M_Ms(500);

    for (i = 0; i < 16; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }

    USART1_Send_String("֧�����󷵻�ָ��: ");
    for (i = 0; i < 16; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    // USART1_Send_String("��ҡҡ֧��:\r\n");

    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    //  USART1_Send_String("\r\n");

    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    //  USART1_Send_String("\r\n");
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    //  USART1_Send_String("\r\n");

    //����ָ��

    //GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + GAME_STATUS.GAME_COINS;

    // System_Setting_Count(GAME_STATUS.GAME_COINS);

    // if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    // {

    //     VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //���˿�ʼŤ����

    //     if (GAME_STATUS.EGG_OUT_ING != 1) //�����ǰ��ʧŤ����
    //     {
    //         GAME_STATUS.SWITCH_OUT_READEY = 1;
    //     }

    //     GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
    // }

    //У�������

    // ucSum = ucSum ^ 13 ^ 0x02^0xCC ^ 0x1 ^ 0x03 ^ 0x01;

    // USART1_Send_String("aaaa:\r\n");
    // USART1_Send_Hex(ucSum);
    // USART1_Send_String("\r\n");

    // for(i=7;i<12;i++)
    // {
    //     ucSum = ucSum ^ LEYAOYAO_REC_BUFFER[i];
    //       USART1_Send_String("B:\r\n");
    // USART1_Send_Hex(ucSum);
    // USART1_Send_String("\r\n");
    // }

    // USART3_Send_Byte(0xAA);
    // USART3_Send_Byte(13);
    // USART3_Send_Byte(0x02);
    // USART3_Send_Byte(0xCC);
    // USART3_Send_Byte(0x01); //�豸ID
    // USART3_Send_Byte(0x03); //֧������Ӧ��
    // USART3_Send_Byte(0x1);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[7]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[8]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[9]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[10]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[11]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[12]);
    // USART3_Send_Byte(0x0); //��λ״̬
    //   USART3_Send_Byte(ucSum); //jУ��λ
    //   USART3_Send_Byte(0xDD);

    //       USART1_Send_Hex(0xAA);
    // USART1_Send_Hex(13);
    // USART1_Send_Hex(0x02);
    // USART1_Send_Hex(0xCC);
    // USART1_Send_Hex(0x01); //�豸ID
    // USART1_Send_Hex(0x03); //֧������Ӧ��
    // USART1_Send_Hex(0x1);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    // USART1_Send_Hex(0x0); //��λ״̬
    //   USART1_Send_Hex(ucSum); //jУ��λ
    //   USART1_Send_Hex(0xDD);
}

void Send_System_Board_Status(void)
{

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    USART1_Send_String("���ز�λָ��\r\n");

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 0x18; //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x01; //������cmd

    leyaoyao_send_buffer[6] = SYSTEM_DEVICE_ID[0]; //�豸ID �̼��汾��
    leyaoyao_send_buffer[7] = SYSTEM_DEVICE_ID[1];
    leyaoyao_send_buffer[8] = SYSTEM_DEVICE_ID[2];
    leyaoyao_send_buffer[9] = SYSTEM_DEVICE_ID[3];

    leyaoyao_send_buffer[10] = SYSTEM_DEVICE_ID[4];
    leyaoyao_send_buffer[11] = SYSTEM_DEVICE_ID[5];
    leyaoyao_send_buffer[12] = SYSTEM_DEVICE_ID[6];
    leyaoyao_send_buffer[13] = SYSTEM_DEVICE_ID[7];
    leyaoyao_send_buffer[14] = SYSTEM_DEVICE_ID[8];

    leyaoyao_send_buffer[15] = SYSTEM_DEVICE_ID[9];
    leyaoyao_send_buffer[16] = SYSTEM_DEVICE_ID[10];
    leyaoyao_send_buffer[17] = SYSTEM_DEVICE_ID[11];
    leyaoyao_send_buffer[18] = 0x00;
    leyaoyao_send_buffer[19] = 0x01;

    leyaoyao_send_buffer[20] = 0x01;                   //�ܲ�λ��
    leyaoyao_send_buffer[21] = 0x01;                   //��λID
    leyaoyao_send_buffer[22] = 0x0;                    //����0 ������ 1
    leyaoyao_send_buffer[23] = GAME_STATUS.GAME_COINS; //ÿ�ֱ�����
    leyaoyao_send_buffer[24] = 0x45;                   //��Ϸʱ��
    leyaoyao_send_buffer[25] = 0;                      //У����

    leyaoyao_send_buffer[26] = 0xDD; //У����
		leyaoyao_send_buffer[27] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 28 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[25] = uiSum;

    for (i = 0; i < 28; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }

    for (i = 0; i < 28; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
    }
}



void Send_System_Board_Status2(void)
{

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    USART1_Send_String("���ز�λָ��\r\n");

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 0x18; //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x01; //������cmd

		 leyaoyao_send_buffer[6] = 0x37;
		leyaoyao_send_buffer[7] = 0xFF;
		leyaoyao_send_buffer[8] = 0xD9;
		leyaoyao_send_buffer[9] = 0x05;
		leyaoyao_send_buffer[10] = 0x42;
		leyaoyao_send_buffer[11] = 0x47;
		leyaoyao_send_buffer[12] = 0x31;
		leyaoyao_send_buffer[13] = 0x36;
		leyaoyao_send_buffer[14] = 0x07;
		leyaoyao_send_buffer[15] = 0x61;
		leyaoyao_send_buffer[16] = 0x09;
		leyaoyao_send_buffer[17] = 0x43;
		leyaoyao_send_buffer[18] = 0x00;
		leyaoyao_send_buffer[19] = 0x01;
		leyaoyao_send_buffer[20] = 0x01;
		leyaoyao_send_buffer[21] = 0x01;
		leyaoyao_send_buffer[22] = 0x00;
		leyaoyao_send_buffer[23] = 0x01;
		leyaoyao_send_buffer[24] = 0x45;
			leyaoyao_send_buffer[25] = 0xA9;
		
    leyaoyao_send_buffer[26] = 0xDD; //У����
		leyaoyao_send_buffer[27] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 28 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

   // leyaoyao_send_buffer[25] = uiSum;

    for (i = 0; i < 28; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }

    for (i = 0; i < 28; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
    }
}




void leyaoyao_test_connect(void)
{
    USART1_Send_String("��ҡҡ��������:\r\n");

    if (LEYAOYAO_REC_BUFFER[4] == 0 && LEYAOYAO_REC_BUFFER[5] == 0 && LEYAOYAO_REC_BUFFER[6] == 0 && LEYAOYAO_REC_BUFFER[7] == 0)
    {

        //__Debug_Send_Leyaoyao_Recevie_Buffer();

        USART3_Send_Byte(0xAA);
        USART3_Send_Byte(0x05);
        USART3_Send_Byte(0x02);
        USART3_Send_Byte(0x01);
        USART3_Send_Byte(0x30); //��Ӧ��ID ��
        USART3_Send_Byte(0x10); //��Ӧ��ID ��
        USART3_Send_Byte(0x26);
        USART3_Send_Byte(0xDD);
				USART3_Send_Byte(0x0A);

        Delay_72M_Ms(3000);

        Send_System_Board_Status2(); //����ϵͳ״̬


Delay_72M_Ms(1500);
        leyaoyao_return_system_lock(0); //����ϵͳ�쳣
    }
}

unsigned char Leyaoyao_Check_Receive_Match(void)
{
//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //��1��ʼУ��
    for (i = 1; i < LEYAOYAO_REC_BUFFER_RECEVICE_COUNT - 1; i++)
    {
        uiSum = uiSum ^ LEYAOYAO_REC_BUFFER[i];
    }

    if (uiSum == LEYAOYAO_REC_BUFFER[LEYAOYAO_REC_BUFFER_RECEVICE_COUNT])
    {
        // USART1_Send_Byte(0xEE);
        return 1;
    }

    return 0;
}

//�ϴ���Ŀ
void Leyaoyao_Retrun_Add_Coins(void)
{
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //�������
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    //COIN_STATUS = 0x12345678;

    leyaoyao_send_buffer[0] = 0xAA;                              //��ͷ
    leyaoyao_send_buffer[1] = 0x18;                              //�������д
    leyaoyao_send_buffer[2] = 0x02;                              //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC;                              //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01;                              //���豸ID
    leyaoyao_send_buffer[5] = 0x08;                              //������cmd
    leyaoyao_send_buffer[6] = 1;                                 //��λ1
    leyaoyao_send_buffer[7] = (unsigned char)COIN_STATUS & 0xff; //֧��������
    leyaoyao_send_buffer[8] = (unsigned char)(COIN_STATUS >> 8) & 0xff;
    leyaoyao_send_buffer[9] = (unsigned char)(COIN_STATUS >> 16) & 0xff; //֧��������
    leyaoyao_send_buffer[10] = (unsigned char)(COIN_STATUS >> 24) & 0xff;
    leyaoyao_send_buffer[11] = (unsigned char)(COIN_STATUS >> 32) & 0xff; //֧��������
    leyaoyao_send_buffer[12] = (unsigned char)(COIN_STATUS >> 48) & 0xff;

    leyaoyao_send_buffer[13] = 1;
    leyaoyao_send_buffer[14] = 0;

    //   leyaoyao_send_buffer[13] = GAME_STATUS.GAME_COINS;
    // leyaoyao_send_buffer[14] = (GAME_STATUS.GAME_COINS >> 8) & 0xFF;

    // leyaoyao_send_buffer[15] = GAME_STATUS.TOTAL_INSERT_COIN_COUNT & 0xFF;
    // leyaoyao_send_buffer[15] = (GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 8) & 0xFF;
    // leyaoyao_send_buffer[17] = (GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 16) & 0xFF;
    // leyaoyao_send_buffer[18] = (GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 24) & 0xFF;

    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[17] = 0;
    leyaoyao_send_buffer[18] = 0;

    leyaoyao_send_buffer[19] = 0; //����Ʒ��
    leyaoyao_send_buffer[20] = 0;

    // leyaoyao_send_buffer[21] = GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF; //�ܳ�����
    // leyaoyao_send_buffer[22] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 8) & 0xFF;
    // leyaoyao_send_buffer[23] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 16) & 0xFF;
    // leyaoyao_send_buffer[24] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 24) & 0xFF;

    leyaoyao_send_buffer[21] = 0; //�ܳ�����
    leyaoyao_send_buffer[22] = 0;
    leyaoyao_send_buffer[23] = 0;
    leyaoyao_send_buffer[24] = 0;

    leyaoyao_send_buffer[25] = 0;
    leyaoyao_send_buffer[26] = 0xDD;
		leyaoyao_send_buffer[27] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 28 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[25] = uiSum;

    for (i = 0; i < 28; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("�ϴ���Ŀ��Ϣ(���巢):\r\n");

    for (i = 0; i < 28; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    COIN_STATUS++;

}

//�ϴ���Ŀ
void Leyaoyao_Retrun_Coins_Details(void)
{
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //�������
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 0x18; //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x08; //������cmd
    leyaoyao_send_buffer[6] = 1;    //��λ1
                                    // leyaoyao_send_buffer[7] = LAST_PAYMENT_ID[0]; //֧��������
                                    // leyaoyao_send_buffer[8] = LAST_PAYMENT_ID[1];
                                    // leyaoyao_send_buffer[9] = LAST_PAYMENT_ID[2]; //֧��������
                                    // leyaoyao_send_buffer[10] = LAST_PAYMENT_ID[3];
                                    // leyaoyao_send_buffer[11] = LAST_PAYMENT_ID[4]; //֧��������
                                    // leyaoyao_send_buffer[12] = LAST_PAYMENT_ID[5];

    leyaoyao_send_buffer[7] = (unsigned char)COIN_STATUS & 0xff; //֧��������
    leyaoyao_send_buffer[8] = (unsigned char)(COIN_STATUS >> 8) & 0xff;
    leyaoyao_send_buffer[9] = (unsigned char)(COIN_STATUS >> 16) & 0xff; //֧��������
    leyaoyao_send_buffer[10] = (unsigned char)(COIN_STATUS >> 24) & 0xff;
    leyaoyao_send_buffer[11] = (unsigned char)(COIN_STATUS >> 32) & 0xff; //֧��������
    leyaoyao_send_buffer[12] = (unsigned char)(COIN_STATUS >> 48) & 0xff;

    leyaoyao_send_buffer[13] = 3;
    leyaoyao_send_buffer[14] = 0;

    //   leyaoyao_send_buffer[13] = GAME_STATUS.GAME_COINS;
    // leyaoyao_send_buffer[14] = (GAME_STATUS.GAME_COINS >> 8) & 0xFF;

    // leyaoyao_send_buffer[15] = GAME_STATUS.TOTAL_INSERT_COIN_COUNT & 0xFF;
    // leyaoyao_send_buffer[15] = (GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 8) & 0xFF;
    // leyaoyao_send_buffer[17] = (GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 16) & 0xFF;
    // leyaoyao_send_buffer[18] = (GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 24) & 0xFF;

    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[17] = 0;
    leyaoyao_send_buffer[18] = 0;

    leyaoyao_send_buffer[19] = 1; //����Ʒ��
    leyaoyao_send_buffer[20] = 0;
    // leyaoyao_send_buffer[21] = GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF; //�ܳ�����
    // leyaoyao_send_buffer[22] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 8) & 0xFF;
    // leyaoyao_send_buffer[23] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 16) & 0xFF;
    // leyaoyao_send_buffer[24] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 24) & 0xFF;

    leyaoyao_send_buffer[21] = 0; //�ܳ�����
    leyaoyao_send_buffer[22] = 0;
    leyaoyao_send_buffer[23] = 0;
    leyaoyao_send_buffer[24] = 0;

    leyaoyao_send_buffer[25] = 0;
    leyaoyao_send_buffer[26] = 0xDD;
		leyaoyao_send_buffer[27] = 0x0A;

    //��1��ʼУ��
    for (i = 1; i < 28 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[25] = uiSum;

    for (i = 0; i < 28; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("�ϴ���Ŀ��Ϣ(���巢):\r\n");

    for (i = 0; i < 28; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    COIN_STATUS++;

 //   return 0;
}

//��ս��ջ�����
void Leyaoyao_Buffer_Clean(void)
{
    unsigned char i = 0;

    for (i = 0; i < LEYAOYAO_REC_BUFFER_COUNT; i++)
    {
        LEYAOYAO_REC_BUFFER[i] = 0;
    }

    LEYAOYAO_REC_BUFFER_RECEVICE_COUNT = 0;
    LEYAOYAO_RECEVICE_STATE = 0;
    LEYAOYAO_RECEVICE_ERROR = 0;

    Delay_72M_Ms(50);
}

unsigned char leyaoyao_return_coins_settings(void)
{

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    USART1_Send_String("������Ϸ����:\r\n");

    __Debug_Send_Leyaoyao_Recevie_Buffer();

    if (LEYAOYAO_REC_BUFFER[6] == 1 && LEYAOYAO_REC_BUFFER[7] == 0)
    {
			
			if( LEYAOYAO_REC_BUFFER[8] ==1){
				leyaoyao_send_buffer[6] = 0;
				
			}else{

        GAME_STATUS.GAME_COINS = LEYAOYAO_REC_BUFFER[8];
        Game_Status_Save();
				leyaoyao_send_buffer[6] = 1;
			}


		}

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 6;    //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x07; //������cmd

    
    leyaoyao_send_buffer[7] = 0;
    leyaoyao_send_buffer[8] = 0xDD;
		leyaoyao_send_buffer[9] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 10 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[7] = uiSum;

    for (i = 0; i < 10; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("������Ϸ��������: ");

    for (i = 0; i < 10; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    return 0;
}

void leyaoyao_retrun_store_settings(void)
{

    //��������

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    //?���ò�λ
    USART1_Send_String("���ò�λ��Ϣָ��:\r\n");

    __Debug_Send_Leyaoyao_Recevie_Buffer();

    if (LEYAOYAO_REC_BUFFER[9] == 1)
    {
        System_Setting_Game_Disable(_SYSTEM_LEYAOYAO_LOCK);
    }
    else
    {
        System_Setting_Game_Disable(_SYSTEM_UNLOCK);
    }

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 6;    //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0xCC; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = 0x01; //���豸ID
    leyaoyao_send_buffer[5] = 0x09; //������cmd

    leyaoyao_send_buffer[6] = 1;
    leyaoyao_send_buffer[7] = 0;
    leyaoyao_send_buffer[8] = 0xDD;
		leyaoyao_send_buffer[9] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 10 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[7] = uiSum;

    for (i = 0; i < 11; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("��λ��Ϣ����: ");

    for (i = 0; i < 10; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");
}

void leyaoyao_return_system_lock(unsigned char ucCode)
{

      

        USART3_Send_Byte(ADD_CODE);
        USART3_Send_Byte(0);
         USART3_Send_Byte(0);
          USART3_Send_Byte(12);
           USART3_Send_Byte(12);
            USART3_Send_Byte(12);
             USART3_Send_Byte(12);

  
        

         //��������

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }




    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 0x0A;    //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0x13; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = ADD_CODE; //�ƹ����
    leyaoyao_send_buffer[5] = ucCode;
    leyaoyao_send_buffer[6] = 0;
    leyaoyao_send_buffer[7] = 0;
    leyaoyao_send_buffer[8] = 0;
    leyaoyao_send_buffer[9] = 0;
     leyaoyao_send_buffer[10] = 0;
     leyaoyao_send_buffer[11] = 0;
    leyaoyao_send_buffer[12] = 0xDD;
		leyaoyao_send_buffer[13] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 14 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[11] = uiSum;

    for (i = 0; i < 14; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("�ϱ����󣡣� ");

    for (i = 0; i < 14; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");


      if(ADD_CODE<0xFF)
        {
            ADD_CODE++;
        }
        else
        {
            ADD_CODE = 0;
        }
}

unsigned char leyaoyao_retrun_settings_value(void)
{

    //��������

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //��ͷ
    leyaoyao_send_buffer[1] = 5;    //�������д
    leyaoyao_send_buffer[2] = 0x02; //�����ʶ
    leyaoyao_send_buffer[3] = 0x05; //Ť�����̶�ָ��
    leyaoyao_send_buffer[4] = GAME_STATUS.LED_1_STATUS; //�ƹ����
    leyaoyao_send_buffer[5] = GAME_STATUS.COIN_MACHINE_STATUS;
    leyaoyao_send_buffer[6] = 0;
    leyaoyao_send_buffer[7] = 0xDD;
		leyaoyao_send_buffer[8] = 0x0A;//���з�

    //��1��ʼУ��
    for (i = 1; i < 9 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[6] = uiSum;

    for (i = 0; i < 9; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("������������: ");

    for (i = 0; i < 9; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");
		
		return 0;
}

unsigned char Leyaoyao_Check_Receive(void)
{

    switch (LEYAOYAO_RECEVICE_STATE)
    {

    //�����ճ������ͷ
    case 0:

        if (LEYAOYAO_REC_BUFFER_RECEVICE_COUNT >= 3)
        {

            Delay_72M_Ms(10);

            //    __Debug_Send_Leyaoyao_Recevie_Buffer();

            if (LEYAOYAO_REC_BUFFER[0] == 0xAA)
            {

                //���������ȷ,�����У��
                if (LEYAOYAO_REC_BUFFER_RECEVICE_COUNT == (LEYAOYAO_REC_BUFFER[1] + 3)) // ������ȴ�����
                {

                    LEYAOYAO_RECEVICE_STATE = 1; //������һ�׶�
                    return 1;
                }
                else
                {
                    LEYAOYAO_RECEVICE_ERROR++;

                    if (LEYAOYAO_RECEVICE_ERROR >= 100) // ���У����� ���� �෢��һ���ַ� �����ܳ���
                    {
                        USART1_Send_String("ָ�����\r\n");
                        __Debug_Send_Leyaoyao_Recevie_Buffer();
                        Leyaoyao_Buffer_Clean();
                        LEYAOYAO_RECEVICE_STATE = 0; //
											 
                        return 1;

                        //     return 0;
                    }
                } //         Leyaoyao_Buffer_Clean();
            }
            else
            {
                USART1_Send_String("cuowuo");
                Leyaoyao_Buffer_Clean();
                LEYAOYAO_RECEVICE_STATE = 0; //
							  
                return 0;
            }
        }
				
				
				
	
				

				

        break;

    case 1:
        //����У����
        if (Leyaoyao_Check_Receive_Match())
        {
            LEYAOYAO_RECEVICE_STATE = 2; //
        }
        else
        {
            Leyaoyao_Buffer_Clean();
            LEYAOYAO_RECEVICE_STATE = 0; //
        }

        break;

    //ִ��ָ��
    case 2:

        switch (LEYAOYAO_REC_BUFFER[3])
        {

        case 1:

            leyaoyao_test_connect(); //��ҡҡ��������
            Leyaoyao_Buffer_Clean();
            LEYAOYAO_RECEVICE_STATE = 0; //
            break;

        case 5:
            USART1_Send_String("��ѯ����\r\n");
            __Debug_Send_Leyaoyao_Recevie_Buffer();
            leyaoyao_retrun_settings_value();
            break;

        case 6:
            USART1_Send_String("���ò���: \r\n");
            __Debug_Send_Leyaoyao_Recevie_Buffer();
            //leyaoyao_retrun_settings_value();
            
          //   USART1_Send_String("�ƹ�ָ��: ");


            if (LEYAOYAO_REC_BUFFER[4] == 1)
            {
                Light1_On();
            }
            else
            {
                Light1_Off();
            }



                if (LEYAOYAO_REC_BUFFER[5] == 1)
            {
             //   Light1_On();
                Game_Insert_Coin_Enable();
            }
            else
            {
             //   Light1_Off();
                Game_Insert_Coin_Disable();
            }



            break;

        case 0x30:
             USART1_Send_String("�ָ��������� \r\n");
    
        USART3_Send_Byte(0xAA);
        USART3_Send_Byte(0x03);
        USART3_Send_Byte(0x02);
        USART3_Send_Byte(0x30);
        USART3_Send_Byte(0x31);
        USART3_Send_Byte(0xDD);
				USART3_Send_Byte(0x0A);

AT24C64_Write_Byte(0, 0xDD); //д�����������־λ
			Voice_Play_Stop();
			VOICE_Play(VOICE_OK_NIUDAN_ALREADY_OK);

            Delay_72M_Ms(5000);

//            System_Reset();

            break;

               case 0x31:
             USART1_Send_String("�������� \r\n");




//            System_Reset();

            break;

        case 0xCC: //Ť����״̬

            switch (LEYAOYAO_REC_BUFFER[5])
            {
            case 0x07: //���ò�λ����
                leyaoyao_return_coins_settings();
                break;

            case 0x09:
                leyaoyao_retrun_store_settings();
                break;

            case 0x03: // ����֧��ָ��
                USART1_Send_String("֧������ָ��:\r\n");
                __Debug_Send_Leyaoyao_Recevie_Buffer();

                if (GAME_STATUS.CURRENT_LOCK_STATUS == _SYSTEM_LEYAOYAO_LOCK)
                {
                    leyaoyao_return_pay_field();
                }
                else
                {
                    leyaoyao_return_pay();
                }

                break;

            case 0x04: //Զ������ ���Ϸ�ָ��
                USART1_Send_String("���Ϸ�ָ��:\r\n");
                __Debug_Send_Leyaoyao_Recevie_Buffer();
                leyaoyao_return_yun_coins_pay();
                break;
						
						case 0x01://��λ����
							break;

            default:
                break;
            }

            Leyaoyao_Buffer_Clean();
            LEYAOYAO_RECEVICE_STATE = 0; //
            break;

        default:
            __Debug_Send_Leyaoyao_Recevie_Buffer();
            break;
        }
        Leyaoyao_Buffer_Clean();
        LEYAOYAO_RECEVICE_STATE = 0; //

        break;

    default:

        break;
    }
		
		
		
		

    return 0;
}
