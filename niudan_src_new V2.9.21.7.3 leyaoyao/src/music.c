#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include <string.h>
#include "usart.h"
#include "game.h"
#include "timer.h"
#include "main.h"
#include "key.h"
#include "wifi.h"

#include "spiflash.h"
#include "music.h"

//NV180C VOICE CHIP
unsigned int TIMER1_DELAY = 0;

struct Voice VOICE;



//已经修正
void VOICE_Pins_Init(void)
{
  //GPIO_Init(VOICE_GPIO_PORT, VOICE_GPIO_PINS, GPIO_Mode_Out_PP_High_Slow);
  //GPIO_SetBits(VOICE_GPIO_PORT, VOICE_GPIO_PINS);

  //GPIO_Init(VOICE_GPIO_BUSY_PORT, VOICE_GPIO_BUSY_PINS, GPIO_Mode_In_FL_No_IT);
  //GPIO_SetBits(VOICE_GPIO_BUSY_PORT, VOICE_GPIO_BUSY_PINS);

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = VOICE_GPIO_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(VOICE_GPIO_PORT, &GPIO_InitStructure);
 // GPIO_SetBits(VOICE_GPIO_PORT, VOICE_GPIO_PINS);

  GPIO_InitStructure.GPIO_Pin = VOICE_GPIO_BUSY_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(VOICE_GPIO_PORT, &GPIO_InitStructure);
 // GPIO_SetBits(VOICE_GPIO_PORT, VOICE_GPIO_BUSY_PINS);
}


void VOICE_Out_Pins_Init(void)
{
	
	 GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = VOICE_GPIO_OUT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(VOICE_GPIO_OUT_PORT, &GPIO_InitStructure);

	VOICE_Out_Pins_Disable();

}


void  VOICE_Out_Pins_Enable(void)
{

if(ucMusicEnable==0)
{
    GPIO_SetBits(VOICE_GPIO_OUT_PORT,VOICE_GPIO_OUT_PIN);
    ucMusicEnable = 1;
}
	
}


void  VOICE_Out_Pins_Disable(void)
{
	GPIO_ResetBits(VOICE_GPIO_OUT_PORT,VOICE_GPIO_OUT_PIN);
}


//已经修正
void VOICE_Play(unsigned char SpeechData)
{

  unsigned char i;
 
  VOICE_Out_Pins_Enable();

  VOICE_Data_1();
  Delay_72M_Ms(10);

  VOICE_Data_0();

  Delay_72M_Us(2000);

  for (i = 0; i < 8; i++)
  {
    if (SpeechData & 0x01)
    {
      VOICE_Data_1();
      Delay_72M_Us(2400);
      VOICE_Data_0();
      Delay_72M_Us(800);
    }
    else
    {
      VOICE_Data_1();
      Delay_72M_Us(800);
      VOICE_Data_0();
      Delay_72M_Us(2400);
    }
    SpeechData >>= 1;
  }

  VOICE_Data_1();
}

//语音播放完毕检测
void VOICE_Check_Busy_While(void)
{

//  unsigned char ucTemp = 0;



  // GPIO_InitStructure.GPIO_Pin = VOICE_GPIO_BUSY_PINS;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // GPIO_Init(VOICE_GPIO_PORT, &GPIO_InitStructure);
  // GPIO_SetBits(VOICE_GPIO_PORT, VOICE_GPIO_BUSY_PINS);



  while (1)
  {
    // ucTemp = GPIO_ReadInputDataBit(VOICE_GPIO_PORT, VOICE_GPIO_BUSY_PINS);
    // USART1_Send_Hex(ucTemp);
    
    if (GPIO_ReadInputDataBit(VOICE_GPIO_PORT, VOICE_GPIO_BUSY_PINS) == 0x01)
    {
    //   USART1_Send_String("检测到语音结束了...\r\n");
      Delay_72M_Ms(5);
      break;
    }
  }
}

unsigned char VOICE_Check_Busy(void)
{
  if (GPIO_ReadInputDataBit(VOICE_GPIO_PORT, VOICE_GPIO_BUSY_PINS) == 0x01)
    {
     return 1;
    }
  else
  {
    return 0;
  }
  


}

//DI
void Voice_Play_Di(void)
{
  VOICE_Play(1);
}

//DI并且等待播放完毕
void Voice_Play_Di_Witch_Check(void)
{
  VOICE_Play(1);
  VOICE_Check_Busy();
}



void Voice_Play_Stop(void)
{
  VOICE_Play(0xFE); //停止播放当前语音
}

