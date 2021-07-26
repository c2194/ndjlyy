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


//����ϵͳʱ��
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus; //�ⲿ����ʱ��(HSE)�Ĺ���״̬����

	RCC_DeInit();								//��������ʱ����صļĴ�������ΪĬ��ֵ
	RCC_HSEConfig(RCC_HSE_ON);					//�����ⲿ����ʱ��HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); //�ȴ��ⲿ����ʱ��(HSE)�ȶ�

	if (SUCCESS == HSEStartUpStatus) //����ⲿ����ʱ���Ѿ��ȶ�
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Flash����
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		RCC_HCLKConfig(RCC_SYSCLK_Div1); //����AHBʱ�ӵ���ϵͳʱ��(1��Ƶ)/72MHZ
		RCC_PCLK2Config(RCC_HCLK_Div1);  //����APB2ʱ�Ӻ�HCLKʱ�����/72MHz(���Ϊ72MHz)
		RCC_PCLK1Config(RCC_HCLK_Div2);  //����APB1ʱ����HCLKʱ�ӵ�2��Ƶ/36MHz(���Ϊ36MHz)

#ifndef STM32F10X_CL										 //���ʹ�õĲ���STM32F107x��STM32F105xϵ��MCU,PLL��������
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLLCLK = 8MHz * 9 = 72 MHz
#else														 //���ʹ�õ���STM32F107x��STM32F105xϵ��MCU,PLL��������
		/***** ����PLLx *****/
		/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
		RCC_PREDIV2Config(RCC_PREDIV2_Div5);
		RCC_PLL2Config(RCC_PLL2Mul_8);

		RCC_PLL2Cmd(ENABLE); //ʹ��PLL2
		while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
			; //�ȴ�PLL2�ȶ�

		/* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

		RCC_PLLCmd(ENABLE); //ʹ��PLL
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
			; //�ȴ�PLL�ȶ�

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //����ϵͳʱ�ӵ�ʱ��ԴΪPLL

		while (RCC_GetSYSCLKSource() != 0x08)
			;								//���ϵͳ��ʱ��Դ�Ƿ���PLL
		RCC_ClockSecuritySystemCmd(ENABLE); //ʹ��ϵͳ��ȫʱ��

		/* Enable peripheral clocks --------------------------------------------------*/
		/* Enable I2C1 and I2C1 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		/* Enable GPIOA GPIOB SPI1 and USART1 clocks */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	}
}


