
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
#include "spiflash.h"
#include "music.h"
#include "protocol.h"
#include "led.h"
#include "switch_key.h"
#include "light_eye.h"
#include "clock.h"
#include "light.h"


void Light1_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_3);

}

void Light1_On(void)
{

GPIO_SetBits(GPIOC, GPIO_Pin_3);

GAME_STATUS.LED_1_STATUS = 1;

}

void Light1_Off(void)
{

GPIO_ResetBits(GPIOC, GPIO_Pin_3);
GAME_STATUS.LED_1_STATUS = 0;

}

void Light2_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);

}

void Light2_On(void)
{

GPIO_SetBits(GPIOC, GPIO_Pin_0);

GAME_STATUS.LED_2_STATUS = 1;

}

void Light2_Off(void)
{

GPIO_ResetBits(GPIOC, GPIO_Pin_0);
GAME_STATUS.LED_2_STATUS = 0;

}

