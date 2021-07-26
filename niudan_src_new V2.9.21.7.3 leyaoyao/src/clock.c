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
#include "light_eye.h"
#include "clock.h"


//设置系统时钟
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus; //外部高速时钟(HSE)的工作状态变量

	RCC_DeInit();								//将所有与时钟相关的寄存器设置为默认值
	RCC_HSEConfig(RCC_HSE_ON);					//启动外部高速时钟HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); //等待外部高速时钟(HSE)稳定

	if (SUCCESS == HSEStartUpStatus) //如果外部高速时钟已经稳定
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Flash设置
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		RCC_HCLKConfig(RCC_SYSCLK_Div1); //设置AHB时钟等于系统时钟(1分频)/72MHZ
		RCC_PCLK2Config(RCC_HCLK_Div1);  //设置APB2时钟和HCLK时钟相等/72MHz(最大为72MHz)
		RCC_PCLK1Config(RCC_HCLK_Div2);  //设置APB1时钟是HCLK时钟的2分频/36MHz(最大为36MHz)

#ifndef STM32F10X_CL										 //如果使用的不是STM32F107x或STM32F105x系列MCU,PLL以下配置
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLLCLK = 8MHz * 9 = 72 MHz
#else														 //如果使用的是STM32F107x或STM32F105x系列MCU,PLL以下配置
		/***** 配置PLLx *****/
		/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
		RCC_PREDIV2Config(RCC_PREDIV2_Div5);
		RCC_PLL2Config(RCC_PLL2Mul_8);

		RCC_PLL2Cmd(ENABLE); //使能PLL2
		while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
			; //等待PLL2稳定

		/* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

		RCC_PLLCmd(ENABLE); //使能PLL
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
			; //等待PLL稳定

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //设置系统时钟的时钟源为PLL

		while (RCC_GetSYSCLKSource() != 0x08)
			;								//检查系统的时钟源是否是PLL
		RCC_ClockSecuritySystemCmd(ENABLE); //使能系统安全时钟

		/* Enable peripheral clocks --------------------------------------------------*/
		/* Enable I2C1 and I2C1 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		/* Enable GPIOA GPIOB SPI1 and USART1 clocks */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	}
}


