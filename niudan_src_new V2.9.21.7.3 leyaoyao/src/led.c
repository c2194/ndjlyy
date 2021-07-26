#include "main.h"
#include "led.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "game.h"




void LED_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_RED_GPIO_PIN | LED_BLUE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, LED_RED_GPIO_PIN | LED_BLUE_GPIO_PIN);
}

void LED_Red_On(void)
{
	GPIO_ResetBits(GPIOB, LED_RED_GPIO_PIN);
}

void LED_Blue_On(void)
{
	GPIO_ResetBits(GPIOB, LED_BLUE_GPIO_PIN);
}

void LED_Red_Off(void)
{
	GPIO_SetBits(GPIOB, LED_RED_GPIO_PIN);
}

void LED_Blue_Off(void)
{
	GPIO_SetBits(GPIOB, LED_BLUE_GPIO_PIN);
}

