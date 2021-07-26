#include "global.h"

struct game_status GAME_STATUS;

#define FLASH_SIZE 512 //所选MCU的FLASH容量大小(单位为K)

#if FLASH_SIZE < 256
#define SECTOR_SIZE 1024 //字节
#else
#define SECTOR_SIZE 2048 //字节
#endif

#define FLASH_START_ADDRESS 0x8000000
#define FLASH_START_PAGE 100

#define MOTO_PORT GPIOA
#define MOTO_PIN GPIO_Pin_1

void Game_Struct_Init(void)
{

    //   unsigned char ID;         //设备ID 0-255
    //   unsigned char ENBALE;     //设备是否允许操作
    //   unsigned int COIN_COUNT;  //已经投币数
    //   unsigned char GAME_COINS; //每局币子数
    //   unsigned char LIGHT_1;    //光眼1状态
    //   unsigned char LIGHT_2;    //光眼2状态
    //   unsigned char LIGHT_3;    //光眼3状态
    //  unsigned char OPEN_KEY;   //被打开状态
    //  unsigned char MOTO_TIME;  //电机转动时间

    GAME_STATUS.ID = 0;         //默认无ID
    GAME_STATUS.GAME_COINS = 2; //默认3币1局

    GAME_STATUS.DISABLE = 0;

    GAME_STATUS.COIN_COUNT = 0;
    GAME_STATUS.TOTAL_INSERT_COIN_COUNT = 0;
    GAME_STATUS.MACHINE_INSERT_COUNT = 0;
    GAME_STATUS.GAME_COUNT = 0;
    GAME_STATUS.GAME_EGG_OUT_COUNT = 0;

    GAME_STATUS.EGG_OUT_WAIT_TIME = 0;
    GAME_STATUS.SWITCH_OUT_READEY = 0;
    GAME_STATUS.MOTO_TIME = 0;

    GAME_STATUS.EGG_OUT_ING = 0; //出蛋中

    //默认的游戏局数

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

    // Game_Status_Read(); //再读一遍
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

    //保存游戏系统参数数据
    //0
    AT24C64_Write_Byte(1, GAME_STATUS.ID); //255
    Delay_72M_Ms(50);

    if (GAME_STATUS.GAME_COINS > 0) //限定每局游戏币数不能为0
    {
        AT24C64_Write_Byte(2, GAME_STATUS.GAME_COINS); //255长
    }
    else
    {
        AT24C64_Write_Byte(2, 1); //255长
    }

    //写入一共局数

    AT24C64_Write_Byte(3, (unsigned char)(GAME_STATUS.GAME_COUNT >> 24));
    AT24C64_Write_Byte(4, (unsigned char)(GAME_STATUS.GAME_COUNT >> 16));
    AT24C64_Write_Byte(5, (unsigned char)(GAME_STATUS.GAME_COUNT >> 8));
    AT24C64_Write_Byte(6, (unsigned char)(GAME_STATUS.GAME_COUNT & 0xFF));

    //写入一共掉蛋数
    AT24C64_Write_Byte(7, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT >> 24));
    AT24C64_Write_Byte(8, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT >> 16));
    AT24C64_Write_Byte(9, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT >> 8));
    AT24C64_Write_Byte(10, (unsigned char)(GAME_STATUS.GAME_EGG_OUT_COUNT & 0xFF));

    //写入一共投币总数（包括线下，跟线上的）
    AT24C64_Write_Byte(11, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 24));
    AT24C64_Write_Byte(12, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 16));
    AT24C64_Write_Byte(13, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT >> 8));
    AT24C64_Write_Byte(14, (unsigned char)(GAME_STATUS.TOTAL_INSERT_COIN_COUNT & 0xFF));

    //写入一共投币总数（包括线下，跟线上的）
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
        //参数初始化
        USART1_Send_String("游戏参数正常读取\r\n");

        Game_Status_Read();
    }
    else
    {
        USART1_Send_String("游戏参数初始化\r\n");

        //参数初始化
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

    //读取游戏总局数
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

    //读取游戏总掉蛋数
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

    //读取游戏总投币数
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

    //读取游戏总掉蛋数
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

    temp0 = *(__IO u32 *)(0x1FFFF7E8); //产品唯一身份标识寄存器（96位）
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

//投币数增加+1
void Coin_Insert_Add(unsigned char ucSelect)
{

    switch (ucSelect)
    {
    case COIN_MACHINE: //投币器增加

        GAME_STATUS.COIN_COUNT++;
        GAME_STATUS.TOTAL_INSERT_COIN_COUNT++;
        GAME_STATUS.MACHINE_INSERT_COUNT++;

        Game_Status_Save();
        Game_Status_Read();

        // USART1_Send_String("币子数：");
        // USART1_Send_Dec(GAME_STATUS.COIN_COUNT, 6);
        // USART1_Send_String("\r\n");

        if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
        {

            VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //好了开始扭蛋吧

            if (GAME_STATUS.EGG_OUT_ING != 1) //如果当前不失扭蛋中
            {
                GAME_STATUS.SWITCH_OUT_READEY = 1;
            }

            GAME_STATUS.EGG_OUT_WAIT_TIME = 20;
        }
        else
        {
            VOICE_Play(VOICE_COIN_MORE_PLEASE); //请再投一个币子
        }
        break;

    case NET_ADD: //网络增加

        if (SYSTEM_USART_BUFFER[5] != 0 && SYSTEM_USART_BUFFER[5] != 0xFF)
        {
            System_Setting_Count(SYSTEM_USART_BUFFER[5]);
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 1); //3指令码，1 TRUE
        }
        else
        {
            System_Send_Return_Status(GAME_STATUS.TARGET_ID, 3, 0); //返回失败码
        }

        if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
        {

            VOICE_Play(VOICE_OK_PLEASE_START_PLAY); //好了开始扭蛋吧

            if (GAME_STATUS.EGG_OUT_ING != 1) //如果当前不失扭蛋中
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

        case 0: //没有投币
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

        case 1: //等待按键抬起

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

        case 2: //币子数+1

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
            // USART1_Send_Debug_Message("投币1个");
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
    case EGG_OUT_CHECK_TIME_IDEL: //不计算时间

        // USART1_Send_String("State 1\r\n");

        //  USART1_Send_String("State 1 \r\n");
        if (GAME_STATUS.SWITCH_OUT_READEY == 1 && GAME_STATUS.EGG_OUT_ING != 1) //如果即将开始出蛋,且不是正在出蛋时 才允许倒计时
        {
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_START;
            GAME_STATUS.EGG_OUT_WAIT_TIME = 20; //设置倒计时时间为20秒
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

        //倒计时为0时，并且语音播放完毕
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
    case SWITCH_OUT: //旋钮出蛋

        if (GAME_STATUS.EGG_OUT_ING == 0)
        {
            Voice_Play_Stop();
            VOICE_Play(VOICE_MUSIC_1);
            GAME_STATUS.PLAYING_MUSIC = 1;

            Game_Coin_Minus();

            USART1_Send_String("旋钮出蛋，进入出蛋检测模式2\r\n");

            GAME_STATUS.EGG_OUT_ING = 1; //设置为正在出蛋
            MOTO_On();
            GAME_STATUS.MOTO_TIME = 20; //超时时间20秒
            GAME_STATUS.MOTO_TIME_ENABLE = 1;
            EGG_OUT = 2; //开始检测时间等

            USART1_Send_String("禁止掉蛋时间检测\r\n");
            GAME_STATUS.SWITCH_OUT_READEY = 0; //设置
            GAME_STATUS.EGG_OUT_WAIT_TIME = 0;
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_IDEL;

            //计算总掉蛋个数
            if (GAME_STATUS.GAME_COUNT >= 0xFFFFFFFE)
            {
                GAME_STATUS.GAME_COUNT = 0;
            }

            GAME_STATUS.GAME_COUNT++;

            Game_Status_Save();
        }

        break;

    case TIME_OVER_OUT: //超时出蛋

        if (GAME_STATUS.EGG_OUT_ING == 0)
        {

            Voice_Play_Stop();
            VOICE_Play(VOICE_MUSIC_1);
            GAME_STATUS.PLAYING_MUSIC = 1;

            Game_Coin_Minus();

            USART1_Send_String("超时出蛋，进入出蛋检测模式2\r\n");

            GAME_STATUS.EGG_OUT_ING = 1; //设置为正在出蛋
            MOTO_On();
            GAME_STATUS.MOTO_TIME = 20; //超时时间20秒
            GAME_STATUS.MOTO_TIME_ENABLE = 1;
            EGG_OUT = 2; //开始检测时间等

            USART1_Send_String("禁止掉蛋时间检测\r\n");
            GAME_STATUS.SWITCH_OUT_READEY = 0; //设置
            GAME_STATUS.EGG_OUT_WAIT_TIME = 0;
            EGG_OUT_CHECK_STATUS = EGG_OUT_CHECK_TIME_IDEL;

            //计算总掉蛋个数
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

//减少游戏币
void Game_Coin_Minus(void)
{

    if (GAME_STATUS.COIN_COUNT >= GAME_STATUS.GAME_COINS)
    {

        GAME_STATUS.COIN_COUNT = GAME_STATUS.COIN_COUNT - GAME_STATUS.GAME_COINS; //减去币数
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

        USART1_Send_String("出蛋了\r\n");

        Voice_Play_Stop();
        VOICE_Play(VOICE_EGG_OUT);

        //Game_Coin_Minus();

        //计算总掉蛋个数
        if (GAME_STATUS.GAME_EGG_OUT_COUNT >= 0xFFFFFFFE)
        {
            GAME_STATUS.GAME_EGG_OUT_COUNT = 0;
        }

        GAME_STATUS.GAME_EGG_OUT_COUNT++;

        Game_Status_Save();

        //

        // USART1_Send_Debug_Message("游戏局数:");
        // USART1_Send_Dec(GAME_STATUS.GAME_COUNT, 6);

        // USART1_Send_Debug_Message("\r\n");

        //如果游戏币数大于依然设置超时扭蛋
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

        USART1_Send_String("出蛋失败超时未出\r\n");

        Voice_Play_Stop();
        VOICE_Play(VOICE_HAS_ERROR_CONNECT_US);
        MOTO_Off();

        //  Game_Coin_Minus();

        //System_Setting_Game_Disable(_SYSTEM_EGG_TIME_OUT_LOCK); //出蛋超时锁定

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
    case 0: //标准模式

        break;

    case 1: // 出蛋

        Game_Egg_Out(SWITCH_OUT);
        EGG_OUT = 2;
        //   EGG_OUT_ERROR = 0;
        //   EGG_OUT_TIME = 50000;

        break;

    case 2: //检测时间
        //	USART1_Send_Debug_Message("模式3");

        //  USART1_Send_Debug_Message("当前时间: ");
        //  USART1_Send_Dec(GAME_STATUS.MOTO_TIME, 3);
        //   USART1_Send_Debug_Message("\r\n");

        if (GAME_STATUS.DROP_EGG == 1)
        {
            EGG_OUT = 4; //成功掉落了蛋
        }

        if (GAME_STATUS.MOTO_TIME == 0) //超时了
        {

            EGG_OUT = 3;
        }

        if (GAME_STATUS.LIGHT_2 == 1)
        {

            EGG_OUT = 5;
        }

        break;

    case 3: //超时

        Game_Egg_Out_Stop(EGG_OUT_TIME_OUT);

        break;

    case 4: //成功掉蛋

        Game_Egg_Out_Stop(EGG_OUT_SUCCEED);
        break;

    case 5: //暂停

        MOTO_Off();

#ifdef HAND_IN_STOP_MOTO_TIME
        GAME_STATUS.MOTO_TIME_ENABLE = 0; //禁止时间减法
#endif

        EGG_OUT = 6;
        break;

    case 6: //等待光眼启动
        if (GAME_STATUS.LIGHT_2 == 0)
        {

            MOTO_On();
            GAME_STATUS.PLAYING_MUSIC = 1; //重新播放音乐
            Voice_Play_Stop();
            VOICE_Play(VOICE_MUSIC_1);
            EGG_OUT = 2;

#ifdef HAND_IN_STOP_MOTO_TIME
            GAME_STATUS.MOTO_TIME_ENABLE = 1; //允许时间减法
#endif
        }

        break;

    default:
        break;
    }
}
