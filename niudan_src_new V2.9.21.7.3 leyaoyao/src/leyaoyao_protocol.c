#include "global.h"

unsigned char LEYAOYAO_REC_BUFFER[LEYAOYAO_REC_BUFFER_COUNT];
unsigned int LEYAOYAO_REC_BUFFER_RECEVICE_COUNT = 0; //当前接收的数量
unsigned char LEYAOYAO_RECEVICE_STATE = 0;           //接收的状态
unsigned int LEYAOYAO_RECEVICE_ERROR = 0;
unsigned char LAST_PAYMENT_ID[10];
unsigned char LAST_PAYMENT_COINS;

//debug 输出信息
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
    //云上分结果反馈
    unsigned char ucSum = 0;
    unsigned char i = 0;
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 14;   //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x05; //子命令cmd
    leyaoyao_send_buffer[6] = 0x01; //子命令cmd

    leyaoyao_send_buffer[7] = LAST_PAYMENT_ID[0]; //支付返回码
    leyaoyao_send_buffer[8] = LAST_PAYMENT_ID[1];
    leyaoyao_send_buffer[9] = LAST_PAYMENT_ID[2];
    leyaoyao_send_buffer[10] = LAST_PAYMENT_ID[3];
    leyaoyao_send_buffer[11] = LAST_PAYMENT_ID[4];
    leyaoyao_send_buffer[12] = LAST_PAYMENT_ID[5];

    if (ucFlag != 0)
    {
        leyaoyao_send_buffer[13] = LAST_PAYMENT_COINS; //结果
    }
    else
    {
        leyaoyao_send_buffer[13] = 0;
    }

    // leyaoyao_send_buffer[13] = LEYAOYAO_REC_BUFFER[13];
    leyaoyao_send_buffer[14] = LEYAOYAO_REC_BUFFER[14];

    //  LAST_PAYMENT_ID[0] = LEYAOYAO_REC_BUFFER[7]; //支付返回码
    // LAST_PAYMENT_ID[1] = LEYAOYAO_REC_BUFFER[8];
    // LAST_PAYMENT_ID[2] = LEYAOYAO_REC_BUFFER[9];
    // LAST_PAYMENT_ID[3] = LEYAOYAO_REC_BUFFER[10];
    // LAST_PAYMENT_ID[4] = LEYAOYAO_REC_BUFFER[11];
    // LAST_PAYMENT_ID[5] = LEYAOYAO_REC_BUFFER[12];
    // LAST_PAYMENT_ID[6] = LEYAOYAO_REC_BUFFER[13];

    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[16] = 0xDD;

    //leyaoyao_send_buffer[26] = 0xDD; //校验码

    //从1开始校验
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

    USART1_Send_String("云上分结果返回(主板发): ");
    for (i = 0; i < 17; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    // USART1_Send_String("乐摇摇支付:\r\n");

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

    //返回指令

    //GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + GAME_STATUS.GAME_COINS;

    System_Setting_Count(LEYAOYAO_REC_BUFFER[14]);
    //  System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 1); //3指令码，1 TRUE

    if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    {

        VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //好了开始扭蛋吧

        if (GAME_STATUS.EGG_OUT_ING != 1) //如果当前不失扭蛋中
        {
            GAME_STATUS.SWITCH_OUT_READEY = 1;
        }

        GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
    }

    //校验码计算

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
    // USART3_Send_Byte(0x01); //设备ID
    // USART3_Send_Byte(0x03); //支付申请应答
    // USART3_Send_Byte(0x1);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[7]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[8]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[9]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[10]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[11]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[12]);
    // USART3_Send_Byte(0x0); //仓位状态
    //   USART3_Send_Byte(ucSum); //j校验位
    //   USART3_Send_Byte(0xDD);

    //       USART1_Send_Hex(0xAA);
    // USART1_Send_Hex(13);
    // USART1_Send_Hex(0x02);
    // USART1_Send_Hex(0xCC);
    // USART1_Send_Hex(0x01); //设备ID
    // USART1_Send_Hex(0x03); //支付申请应答
    // USART1_Send_Hex(0x1);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    // USART1_Send_Hex(0x0); //仓位状态
    //   USART1_Send_Hex(ucSum); //j校验位
    //   USART1_Send_Hex(0xDD);
}

void leyaoyao_return_yun_coins_pay(void)
{
    unsigned char ucSum = 0;
    unsigned char i = 0;
    unsigned char leyaoyao_send_buffer[60];

    USART1_Send_String("云上分功能\r\n");

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    LAST_PAYMENT_ID[0] = LEYAOYAO_REC_BUFFER[7]; //支付返回码
    LAST_PAYMENT_ID[1] = LEYAOYAO_REC_BUFFER[8];
    LAST_PAYMENT_ID[2] = LEYAOYAO_REC_BUFFER[9];
    LAST_PAYMENT_ID[3] = LEYAOYAO_REC_BUFFER[10];
    LAST_PAYMENT_ID[4] = LEYAOYAO_REC_BUFFER[11];
    LAST_PAYMENT_ID[5] = LEYAOYAO_REC_BUFFER[12];
    //LAST_PAYMENT_ID[6] = LEYAOYAO_REC_BUFFER[13];

    LAST_PAYMENT_COINS = LEYAOYAO_REC_BUFFER[13];

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 14;   //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x04; //子命令cmd
    leyaoyao_send_buffer[6] = 0x01; //子命令cmd

    leyaoyao_send_buffer[7] = LEYAOYAO_REC_BUFFER[7]; //支付返回码
    leyaoyao_send_buffer[8] = LEYAOYAO_REC_BUFFER[8];
    leyaoyao_send_buffer[9] = LEYAOYAO_REC_BUFFER[9];
    leyaoyao_send_buffer[10] = LEYAOYAO_REC_BUFFER[10];
    leyaoyao_send_buffer[11] = LEYAOYAO_REC_BUFFER[11];
    leyaoyao_send_buffer[12] = LEYAOYAO_REC_BUFFER[12];
    leyaoyao_send_buffer[13] = LEYAOYAO_REC_BUFFER[13]; //CONIS;                       //CONIS
    leyaoyao_send_buffer[14] = LEYAOYAO_REC_BUFFER[14];

    leyaoyao_send_buffer[15] = 0;
    leyaoyao_send_buffer[16] = 0xDD;

    //leyaoyao_send_buffer[26] = 0xDD; //校验码

    //从1开始校验
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

    USART1_Send_String("云上分功能返回指令: ");

    for (i = 0; i < 17; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    // USART1_Send_String("乐摇摇支付:\r\n");

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

    //返回指令

    //GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + GAME_STATUS.GAME_COINS;

    System_Setting_Count(LEYAOYAO_REC_BUFFER[13]);
    //  System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 1); //3指令码，1 TRUE

    if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    {

        VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //好了开始扭蛋吧

        if (GAME_STATUS.EGG_OUT_ING != 1) //如果当前不失扭蛋中
        {
            GAME_STATUS.SWITCH_OUT_READEY = 1;
        }

        GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
    }

    //校验码计算

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
    // USART3_Send_Byte(0x01); //设备ID
    // USART3_Send_Byte(0x03); //支付申请应答
    // USART3_Send_Byte(0x1);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[7]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[8]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[9]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[10]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[11]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[12]);
    // USART3_Send_Byte(0x0); //仓位状态
    //   USART3_Send_Byte(ucSum); //j校验位
    //   USART3_Send_Byte(0xDD);

    //       USART1_Send_Hex(0xAA);
    // USART1_Send_Hex(13);
    // USART1_Send_Hex(0x02);
    // USART1_Send_Hex(0xCC);
    // USART1_Send_Hex(0x01); //设备ID
    // USART1_Send_Hex(0x03); //支付申请应答
    // USART1_Send_Hex(0x1);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    // USART1_Send_Hex(0x0); //仓位状态
    //   USART1_Send_Hex(ucSum); //j校验位
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

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 13;   //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x03; //子命令cmd
    leyaoyao_send_buffer[6] = 0x01; //子命令cmd

    leyaoyao_send_buffer[7] = LEYAOYAO_REC_BUFFER[7]; //支付返回码
    leyaoyao_send_buffer[8] = LEYAOYAO_REC_BUFFER[8];
    leyaoyao_send_buffer[9] = LEYAOYAO_REC_BUFFER[9];
    leyaoyao_send_buffer[10] = LEYAOYAO_REC_BUFFER[10];
    leyaoyao_send_buffer[11] = LEYAOYAO_REC_BUFFER[11];
    leyaoyao_send_buffer[12] = LEYAOYAO_REC_BUFFER[12];

    leyaoyao_send_buffer[13] = 3;

    leyaoyao_send_buffer[14] = 0;
    leyaoyao_send_buffer[15] = 0xDD;

    //从1开始校验
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

    USART1_Send_String("支付请求异常返回指令(主板发): ");

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

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 13;   //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x03; //子命令cmd
    leyaoyao_send_buffer[6] = 0x01; //子命令cmd

    leyaoyao_send_buffer[7] = LEYAOYAO_REC_BUFFER[7]; //支付返回码
    leyaoyao_send_buffer[8] = LEYAOYAO_REC_BUFFER[8];
    leyaoyao_send_buffer[9] = LEYAOYAO_REC_BUFFER[9];
    leyaoyao_send_buffer[10] = LEYAOYAO_REC_BUFFER[10];
    leyaoyao_send_buffer[11] = LEYAOYAO_REC_BUFFER[11];
    leyaoyao_send_buffer[12] = LEYAOYAO_REC_BUFFER[12];

    leyaoyao_send_buffer[13] = 0;

    leyaoyao_send_buffer[14] = 0;
    leyaoyao_send_buffer[15] = 0xDD;

    //leyaoyao_send_buffer[26] = 0xDD; //校验码

    //从1开始校验
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

    USART1_Send_String("支付请求返回指令: ");
    for (i = 0; i < 16; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    // USART1_Send_String("乐摇摇支付:\r\n");

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

    //返回指令

    //GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT + GAME_STATUS.GAME_COINS;

    // System_Setting_Count(GAME_STATUS.GAME_COINS);

    // if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    // {

    //     VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //好了开始扭蛋吧

    //     if (GAME_STATUS.EGG_OUT_ING != 1) //如果当前不失扭蛋中
    //     {
    //         GAME_STATUS.SWITCH_OUT_READEY = 1;
    //     }

    //     GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
    // }

    //校验码计算

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
    // USART3_Send_Byte(0x01); //设备ID
    // USART3_Send_Byte(0x03); //支付申请应答
    // USART3_Send_Byte(0x1);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[7]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[8]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[9]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[10]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[11]);
    // USART3_Send_Byte(LEYAOYAO_REC_BUFFER[12]);
    // USART3_Send_Byte(0x0); //仓位状态
    //   USART3_Send_Byte(ucSum); //j校验位
    //   USART3_Send_Byte(0xDD);

    //       USART1_Send_Hex(0xAA);
    // USART1_Send_Hex(13);
    // USART1_Send_Hex(0x02);
    // USART1_Send_Hex(0xCC);
    // USART1_Send_Hex(0x01); //设备ID
    // USART1_Send_Hex(0x03); //支付申请应答
    // USART1_Send_Hex(0x1);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[7]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[8]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[9]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[10]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[11]);
    // USART1_Send_Hex(LEYAOYAO_REC_BUFFER[12]);
    // USART1_Send_Hex(0x0); //仓位状态
    //   USART1_Send_Hex(ucSum); //j校验位
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

    USART1_Send_String("返回仓位指令\r\n");

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 0x18; //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x01; //子命令cmd

    leyaoyao_send_buffer[6] = SYSTEM_DEVICE_ID[0]; //设备ID 固件版本号
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

    leyaoyao_send_buffer[20] = 0x01;                   //总仓位数
    leyaoyao_send_buffer[21] = 0x01;                   //仓位ID
    leyaoyao_send_buffer[22] = 0x0;                    //在线0 不在线 1
    leyaoyao_send_buffer[23] = GAME_STATUS.GAME_COINS; //每局币子数
    leyaoyao_send_buffer[24] = 0x45;                   //游戏时间
    leyaoyao_send_buffer[25] = 0;                      //校验码

    leyaoyao_send_buffer[26] = 0xDD; //校验码
		leyaoyao_send_buffer[27] = 0x0A;//换行符

    //从1开始校验
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

    USART1_Send_String("返回仓位指令\r\n");

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 0x18; //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x01; //子命令cmd

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
		
    leyaoyao_send_buffer[26] = 0xDD; //校验码
		leyaoyao_send_buffer[27] = 0x0A;//换行符

    //从1开始校验
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
    USART1_Send_String("乐摇摇测试连接:\r\n");

    if (LEYAOYAO_REC_BUFFER[4] == 0 && LEYAOYAO_REC_BUFFER[5] == 0 && LEYAOYAO_REC_BUFFER[6] == 0 && LEYAOYAO_REC_BUFFER[7] == 0)
    {

        //__Debug_Send_Leyaoyao_Recevie_Buffer();

        USART3_Send_Byte(0xAA);
        USART3_Send_Byte(0x05);
        USART3_Send_Byte(0x02);
        USART3_Send_Byte(0x01);
        USART3_Send_Byte(0x30); //供应商ID 低
        USART3_Send_Byte(0x10); //供应商ID 高
        USART3_Send_Byte(0x26);
        USART3_Send_Byte(0xDD);
				USART3_Send_Byte(0x0A);

        Delay_72M_Ms(3000);

        Send_System_Board_Status2(); //发送系统状态


Delay_72M_Ms(1500);
        leyaoyao_return_system_lock(0); //修正系统异常
    }
}

unsigned char Leyaoyao_Check_Receive_Match(void)
{
//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //从1开始校验
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

//上传账目
void Leyaoyao_Retrun_Add_Coins(void)
{
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //清空数据
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    //COIN_STATUS = 0x12345678;

    leyaoyao_send_buffer[0] = 0xAA;                              //包头
    leyaoyao_send_buffer[1] = 0x18;                              //长度最后写
    leyaoyao_send_buffer[2] = 0x02;                              //主板标识
    leyaoyao_send_buffer[3] = 0xCC;                              //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01;                              //子设备ID
    leyaoyao_send_buffer[5] = 0x08;                              //子命令cmd
    leyaoyao_send_buffer[6] = 1;                                 //仓位1
    leyaoyao_send_buffer[7] = (unsigned char)COIN_STATUS & 0xff; //支付返回码
    leyaoyao_send_buffer[8] = (unsigned char)(COIN_STATUS >> 8) & 0xff;
    leyaoyao_send_buffer[9] = (unsigned char)(COIN_STATUS >> 16) & 0xff; //支付返回码
    leyaoyao_send_buffer[10] = (unsigned char)(COIN_STATUS >> 24) & 0xff;
    leyaoyao_send_buffer[11] = (unsigned char)(COIN_STATUS >> 32) & 0xff; //支付返回码
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

    leyaoyao_send_buffer[19] = 0; //出礼品数
    leyaoyao_send_buffer[20] = 0;

    // leyaoyao_send_buffer[21] = GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF; //总出蛋数
    // leyaoyao_send_buffer[22] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 8) & 0xFF;
    // leyaoyao_send_buffer[23] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 16) & 0xFF;
    // leyaoyao_send_buffer[24] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 24) & 0xFF;

    leyaoyao_send_buffer[21] = 0; //总出蛋数
    leyaoyao_send_buffer[22] = 0;
    leyaoyao_send_buffer[23] = 0;
    leyaoyao_send_buffer[24] = 0;

    leyaoyao_send_buffer[25] = 0;
    leyaoyao_send_buffer[26] = 0xDD;
		leyaoyao_send_buffer[27] = 0x0A;//换行符

    //从1开始校验
    for (i = 1; i < 28 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[25] = uiSum;

    for (i = 0; i < 28; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("上传账目信息(主板发):\r\n");

    for (i = 0; i < 28; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    COIN_STATUS++;

}

//上传账目
void Leyaoyao_Retrun_Coins_Details(void)
{
    unsigned int uiSum = 0;
    unsigned int i = 0;

    //清空数据
    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 0x18; //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x08; //子命令cmd
    leyaoyao_send_buffer[6] = 1;    //仓位1
                                    // leyaoyao_send_buffer[7] = LAST_PAYMENT_ID[0]; //支付返回码
                                    // leyaoyao_send_buffer[8] = LAST_PAYMENT_ID[1];
                                    // leyaoyao_send_buffer[9] = LAST_PAYMENT_ID[2]; //支付返回码
                                    // leyaoyao_send_buffer[10] = LAST_PAYMENT_ID[3];
                                    // leyaoyao_send_buffer[11] = LAST_PAYMENT_ID[4]; //支付返回码
                                    // leyaoyao_send_buffer[12] = LAST_PAYMENT_ID[5];

    leyaoyao_send_buffer[7] = (unsigned char)COIN_STATUS & 0xff; //支付返回码
    leyaoyao_send_buffer[8] = (unsigned char)(COIN_STATUS >> 8) & 0xff;
    leyaoyao_send_buffer[9] = (unsigned char)(COIN_STATUS >> 16) & 0xff; //支付返回码
    leyaoyao_send_buffer[10] = (unsigned char)(COIN_STATUS >> 24) & 0xff;
    leyaoyao_send_buffer[11] = (unsigned char)(COIN_STATUS >> 32) & 0xff; //支付返回码
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

    leyaoyao_send_buffer[19] = 1; //出礼品数
    leyaoyao_send_buffer[20] = 0;
    // leyaoyao_send_buffer[21] = GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF; //总出蛋数
    // leyaoyao_send_buffer[22] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 8) & 0xFF;
    // leyaoyao_send_buffer[23] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 16) & 0xFF;
    // leyaoyao_send_buffer[24] = (GAME_STATUS.GAME_EGG_OUT_COUNT >> 24) & 0xFF;

    leyaoyao_send_buffer[21] = 0; //总出蛋数
    leyaoyao_send_buffer[22] = 0;
    leyaoyao_send_buffer[23] = 0;
    leyaoyao_send_buffer[24] = 0;

    leyaoyao_send_buffer[25] = 0;
    leyaoyao_send_buffer[26] = 0xDD;
		leyaoyao_send_buffer[27] = 0x0A;

    //从1开始校验
    for (i = 1; i < 28 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[25] = uiSum;

    for (i = 0; i < 28; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("上传账目信息(主板发):\r\n");

    for (i = 0; i < 28; i++)
    {
        USART1_Send_Hex(leyaoyao_send_buffer[i]);
        USART1_Send_String(" ");
    }

    USART1_Send_String("\r\n");

    COIN_STATUS++;

 //   return 0;
}

//清空接收缓冲区
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

    USART1_Send_String("设置游戏币数:\r\n");

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

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 6;    //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x07; //子命令cmd

    
    leyaoyao_send_buffer[7] = 0;
    leyaoyao_send_buffer[8] = 0xDD;
		leyaoyao_send_buffer[9] = 0x0A;//换行符

    //从1开始校验
    for (i = 1; i < 10 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[7] = uiSum;

    for (i = 0; i < 10; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("设置游戏币数返回: ");

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

    //返回数据

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    //?设置仓位
    USART1_Send_String("设置仓位信息指令:\r\n");

    __Debug_Send_Leyaoyao_Recevie_Buffer();

    if (LEYAOYAO_REC_BUFFER[9] == 1)
    {
        System_Setting_Game_Disable(_SYSTEM_LEYAOYAO_LOCK);
    }
    else
    {
        System_Setting_Game_Disable(_SYSTEM_UNLOCK);
    }

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 6;    //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0xCC; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = 0x01; //子设备ID
    leyaoyao_send_buffer[5] = 0x09; //子命令cmd

    leyaoyao_send_buffer[6] = 1;
    leyaoyao_send_buffer[7] = 0;
    leyaoyao_send_buffer[8] = 0xDD;
		leyaoyao_send_buffer[9] = 0x0A;//换行符

    //从1开始校验
    for (i = 1; i < 10 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[7] = uiSum;

    for (i = 0; i < 11; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("仓位信息返回: ");

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

  
        

         //返回数据

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }




    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 0x0A;    //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0x13; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = ADD_CODE; //灯光控制
    leyaoyao_send_buffer[5] = ucCode;
    leyaoyao_send_buffer[6] = 0;
    leyaoyao_send_buffer[7] = 0;
    leyaoyao_send_buffer[8] = 0;
    leyaoyao_send_buffer[9] = 0;
     leyaoyao_send_buffer[10] = 0;
     leyaoyao_send_buffer[11] = 0;
    leyaoyao_send_buffer[12] = 0xDD;
		leyaoyao_send_buffer[13] = 0x0A;//换行符

    //从1开始校验
    for (i = 1; i < 14 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[11] = uiSum;

    for (i = 0; i < 14; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("上报错误！！ ");

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

    //返回数据

//    unsigned char ucHighSum = 0;
//    unsigned char ucLowSum = 0;
    unsigned int uiSum = 0;
    unsigned int i = 0;

    unsigned char leyaoyao_send_buffer[60];

    for (i = 0; i < 60; i++)
    {
        leyaoyao_send_buffer[i] = 0;
    }

    leyaoyao_send_buffer[0] = 0xAA; //包头
    leyaoyao_send_buffer[1] = 5;    //长度最后写
    leyaoyao_send_buffer[2] = 0x02; //主板标识
    leyaoyao_send_buffer[3] = 0x05; //扭蛋机固定指令
    leyaoyao_send_buffer[4] = GAME_STATUS.LED_1_STATUS; //灯光控制
    leyaoyao_send_buffer[5] = GAME_STATUS.COIN_MACHINE_STATUS;
    leyaoyao_send_buffer[6] = 0;
    leyaoyao_send_buffer[7] = 0xDD;
		leyaoyao_send_buffer[8] = 0x0A;//换行符

    //从1开始校验
    for (i = 1; i < 9 - 3; i++)
    {
        uiSum = uiSum ^ leyaoyao_send_buffer[i];
    }

    leyaoyao_send_buffer[6] = uiSum;

    for (i = 0; i < 9; i++)
    {
        USART3_Send_Byte(leyaoyao_send_buffer[i]);
    }
    USART1_Send_String("基础参数返回: ");

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

    //检查接收长度与包头
    case 0:

        if (LEYAOYAO_REC_BUFFER_RECEVICE_COUNT >= 3)
        {

            Delay_72M_Ms(10);

            //    __Debug_Send_Leyaoyao_Recevie_Buffer();

            if (LEYAOYAO_REC_BUFFER[0] == 0xAA)
            {

                //如果长度正确,则进行校验
                if (LEYAOYAO_REC_BUFFER_RECEVICE_COUNT == (LEYAOYAO_REC_BUFFER[1] + 3)) // 在这里等待长度
                {

                    LEYAOYAO_RECEVICE_STATE = 1; //进入下一阶段
                    return 1;
                }
                else
                {
                    LEYAOYAO_RECEVICE_ERROR++;

                    if (LEYAOYAO_RECEVICE_ERROR >= 100) // 如果校验错误 或者 多发了一个字符 都可能出错
                    {
                        USART1_Send_String("指令错误\r\n");
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
        //计算校验码
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

    //执行指令
    case 2:

        switch (LEYAOYAO_REC_BUFFER[3])
        {

        case 1:

            leyaoyao_test_connect(); //乐摇摇测试连接
            Leyaoyao_Buffer_Clean();
            LEYAOYAO_RECEVICE_STATE = 0; //
            break;

        case 5:
            USART1_Send_String("查询参数\r\n");
            __Debug_Send_Leyaoyao_Recevie_Buffer();
            leyaoyao_retrun_settings_value();
            break;

        case 6:
            USART1_Send_String("设置参数: \r\n");
            __Debug_Send_Leyaoyao_Recevie_Buffer();
            //leyaoyao_retrun_settings_value();
            
          //   USART1_Send_String("灯光指令: ");


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
             USART1_Send_String("恢复出厂设置 \r\n");
    
        USART3_Send_Byte(0xAA);
        USART3_Send_Byte(0x03);
        USART3_Send_Byte(0x02);
        USART3_Send_Byte(0x30);
        USART3_Send_Byte(0x31);
        USART3_Send_Byte(0xDD);
				USART3_Send_Byte(0x0A);

AT24C64_Write_Byte(0, 0xDD); //写入另外参数标志位
			Voice_Play_Stop();
			VOICE_Play(VOICE_OK_NIUDAN_ALREADY_OK);

            Delay_72M_Ms(5000);

//            System_Reset();

            break;

               case 0x31:
             USART1_Send_String("重新启动 \r\n");




//            System_Reset();

            break;

        case 0xCC: //扭蛋机状态

            switch (LEYAOYAO_REC_BUFFER[5])
            {
            case 0x07: //设置仓位数据
                leyaoyao_return_coins_settings();
                break;

            case 0x09:
                leyaoyao_retrun_store_settings();
                break;

            case 0x03: // 在线支付指令
                USART1_Send_String("支付请求指令:\r\n");
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

            case 0x04: //远程启动 云上分指令
                USART1_Send_String("云上分指令:\r\n");
                __Debug_Send_Leyaoyao_Recevie_Buffer();
                leyaoyao_return_yun_coins_pay();
                break;
						
						case 0x01://仓位返回
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
