#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "W5500.h"
#include <string.h>
#include "usart.h"
#include "game.h"
#include "timer.h"
#include "main.h"
#include "key.h"
#include "wifi.h"
#include "spiflash.h"
#include "music.h"
#include "protocol.h"
#include "led.h"
#include "switch_key.h"

unsigned char ucSwitchKeyStatus;
unsigned char CURRENT_SWITCH_KEY_STATUS;
unsigned char LAST_SWITCH_KEY_STATUS;
unsigned char SWITCH_COUNT;

#define SWITCH_KEY_PINS_1 GPIO_Pin_2 //PD2
#define SWITCH_KEY_PINS_2 GPIO_Pin_3 //PB3
#define SWITCH_KEY_PINS_3 GPIO_Pin_4 //PB4

unsigned char ucGetSwitchKeyStatus(void)
{
    unsigned char temp = 0;

    temp = GPIO_ReadInputDataBit(GPIOD, SWITCH_KEY_PINS_1) + (GPIO_ReadInputDataBit(GPIOB, SWITCH_KEY_PINS_2) * 10) + (GPIO_ReadInputDataBit(GPIOB, SWITCH_KEY_PINS_3) * 100);

    //temp = GPIO_ReadInputDataBit(GPIOB, SWITCH_KEY_PINS_2);

    return temp;
}

void Switch_Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SWITCH_KEY_PINS_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SWITCH_KEY_PINS_2 | SWITCH_KEY_PINS_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus(); //读取旋钮的状态
}

// void Check_Switch_Key_Status_New(void)
// {
//     CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//     // USART1_Send_Dec(CURRENT_SWITCH_KEY_STATUS, 3);

//     // USART1_Send_Debug_Message("状态2\r\n");

//     switch (CURRENT_SWITCH_KEY_STATUS)
//     {
//         case 11:
//             USART1_Send_Debug_Message("状态3\r\n");
//             break;

//         default:
//             break;
//     }

// }

void Check_Switch_Key_Status(void)
{

    switch (ucSwitchKeyStatus)
    {

    case 0: //阶段一 没有安装旋钮

        // USART1_Send_Debug_Message("状态1\r\n");

        if (ucGetSwitchKeyStatus() != 111)
        {
            LAST_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
            ucSwitchKeyStatus = 1;
            SWITCH_COUNT = 0;
            //   ucSelect = 1; //停止计时
        }

        break;

    case 1:

        CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();

        if (CURRENT_SWITCH_KEY_STATUS != LAST_SWITCH_KEY_STATUS)
        {

            if (GAME_STATUS.DISABLE == _SYSTEM_UNLOCK || GAME_STATUS.DISABLE == _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE)
            {

                CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
                LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

                if (GAME_STATUS.COIN_COUNT < GAME_STATUS.GAME_COINS)
                {
                          //   USART1_Send_String("BBB");

                            if (VOICE_Check_Busy())
                            {
                                if (GAME_STATUS.COIN_COUNT > 0)
                                {
                                  //  USART1_Send_Debug_Message("请先对一个扭蛋币吧~");
                                    VOICE_Play(VOICE_COIN_MORE_PLEASE);
                                }
                                else
                                {
                                   // USART1_Send_Debug_Message("再投一个");

                                    VOICE_Play(VOICE_INSERT_COIN_PLEASE);
                                }
                            }
                }
                else
                {
                    USART1_Send_String("我要出蛋！！！！");
                    //没有正在出蛋的时候
                    if (GAME_STATUS.EGG_OUT_ING == 0)
                    {
                        Game_Egg_Out(SWITCH_OUT);
                    }

                    // if(EGG_OUT == 0) //如果当前没有出蛋的时候
                    // {
                    //    // Game_Egg_Out(); //出蛋
                    //    //出蛋位置旋钮
                    // }
                    //ucSwitchKeyStatus = 0;
                }

                ucSwitchKeyStatus = 0;
            }
            else
            {
                /* code */

            //    USART1_Send_String("CCCC");

                if (VOICE_Check_Busy() == 1) //sleep
                {
                    VOICE_Play(VOICE_I_HAS_SLEEP_CHANGE_OTHER);
                }

                ucSwitchKeyStatus = 0;
            }

            //  Delay_72M_Ms(50);

            //      USART1_Send_String("当前状态：");
            //   USART1_Send_Hex(CURRENT_SWITCH_KEY_STATUS);
            //   USART1_Send_String("\r\n");

            //   USART1_Send_String("LAST状态：");
            //   USART1_Send_Hex(LAST_SWITCH_KEY_STATUS);
            //   USART1_Send_String("\r\n");
        }

        //         CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();

        //         if (CURRENT_SWITCH_KEY_STATUS != LAST_SWITCH_KEY_STATUS)
        //         {
        //             USART1_Send_String("旋钮:");

        //             USART1_Send_String("当前状态:");
        // USART1_Send_Hex(GAME_STATUS.DISABLE );
        // USART1_Send_String("\r\n");

        //             LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

        //             if (GAME_STATUS.COIN_COUNT < GAME_STATUS.GAME_COINS)
        //             {
        //                  USART1_Send_String("AAAA");

        //                 if (GAME_STATUS.DISABLE == _SYSTEM_UNLOCK || GAME_STATUS.DISABLE == _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE )
        //                 {

        //                         USART1_Send_String("BBB");

        //                     if (VOICE_Check_Busy())
        //                     {
        //                         if (GAME_STATUS.COIN_COUNT > 0)
        //                         {
        //                           //  USART1_Send_Debug_Message("请先对一个扭蛋币吧~");
        //                             VOICE_Play(VOICE_COIN_MORE_PLEASE);
        //                         }
        //                         else
        //                         {
        //                            // USART1_Send_Debug_Message("再投一个");

        //                             VOICE_Play(VOICE_INSERT_COIN_PLEASE);
        //                         }
        //                     }
        //                 }
        //                 else
        //                 {
        //
        //                 }

        //                 ucSwitchKeyStatus = 0;
        //             }
        //             else
        //             {

        //
        //             }
        //         }

        break;

    default:
        break;
    }
}

//原来的

// void Check_Switch_Key_Status(void)
// {
//     switch (ucSwitchKeyStatus)
//     {
//     case 0: //阶段一 没有安装旋钮

//         USART1_Send_Debug_Message("状态1\r\n");

//         if (ucGetSwitchKeyStatus() != 111)
//         {
//             LAST_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//             ucSwitchKeyStatus = 1;
//             SWITCH_COUNT = 0;
//         }

//         break;

//     case 1:

//         CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//         // USART1_Send_Dec(CURRENT_SWITCH_KEY_STATUS, 3);

//         // USART1_Send_Debug_Message("状态2\r\n");

//         switch (LAST_SWITCH_KEY_STATUS)
//         {

//         case (11):

//          //   USART1_Send_Debug_Message("1111\r\n");
//             //  CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//             // LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

//             if(CURRENT_SWITCH_KEY_STATUS == 110)
//             {
//                 USART1_Send_Debug_Message("增加\r\n");

//                 LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

//                 if(GAME_STATUS.COIN_COUNT<GAME_STATUS.GAME_COINS)
//                 {

//                 }
//             }

//             if(CURRENT_SWITCH_KEY_STATUS == 101)
//             {
//                   USART1_Send_Debug_Message("减少\r\n");

//                 LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

//             }

//             break;

//         case (101):

//          //   USART1_Send_Debug_Message("2222\r\n");
//             //  CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//             // LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

//             if (CURRENT_SWITCH_KEY_STATUS == 11)
//             {
//                 USART1_Send_Debug_Message("增加\r\n");

//                     LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;
//             }

//             if (CURRENT_SWITCH_KEY_STATUS == 110)
//             {
//                 USART1_Send_Debug_Message("减少\r\n");

//                      LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;
//             }

//             break;

//         case (110):

//          //   USART1_Send_Debug_Message("3333\r\n");
//             //USART1_Send_Debug_Message("2222\r\n");
//             //  CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//             // LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;
//              if(CURRENT_SWITCH_KEY_STATUS == 101)
//             {
//                 USART1_Send_Debug_Message("增加\r\n");

//                LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

//             }

//             if(CURRENT_SWITCH_KEY_STATUS == 11)
//             {
//                   USART1_Send_Debug_Message("减少\r\n");

//                LAST_SWITCH_KEY_STATUS = CURRENT_SWITCH_KEY_STATUS;

//             }

//             break;

//         default:
//             // USART1_Send_Debug_Message("STATUS:");
//             // USART1_Send_Dec(CURRENT_SWITCH_KEY_STATUS, 3);
//             // USART1_Send_Debug_Message(" 4444\r\n");

//             break;
//         }

//         // CURRENT_SWITCH_KEY_STATUS = ucGetSwitchKeyStatus();
//         //  USART1_Send_Dec(CURRENT_SWITCH_KEY_STATUS,3);
//         // USART1_Send_Debug_Message("状态2\r\n");

//         break;

//         // case 1: //等待按键抬起

//         // while(GPIO_ReadInputDataBit(SWITCH_KEY_PORT,COIN_PINS)==0);
//         // 	ucCoinStatus = 2;

//         // break;

//         // case 2: //币子数+1

//         // GAME_STATUS.COIN_COUNT++;
//         // 	USART1_Send_Debug_Message("币子数：");
//         // 	USART1_Send_Dec(GAME_STATUS.COIN_COUNT,3);
//         // 	USART1_Send_Debug_Message("\r\n");

//         // 	if(GAME_STATUS.COIN_COUNT>= GAME_STATUS.GAME_COINS)
//         // 	{
//         // 		VOICE_Play(0); //好了开始扭蛋吧
//         // 	}
//         // 	else
//         // 	{
//         // 		VOICE_Play(4); //请再投一个币子
//         // 	}

//         // 	ucCoinStatus = 0;

//         // break;

//     default:
//         break;
//     }
// }
