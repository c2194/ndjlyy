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

extern __IO uint16_t ADC_ConvertedValue;

unsigned int Get_ADC_1_DATA(void)
{

	// GPIO_InitStructure.GPIO_Pin = LIGHT_EYE_2_REC_PIN; //ADC14ͨ��
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  //��������
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(LIGHT_EYE_2_SEND_PORT, &GPIO_InitStructure);

	unsigned int REC = 0;
	REC = GPIO_ReadInputDataBit(LIGHT_EYE_1_REC_PORT, LIGHT_EYE_1_REC_PIN);

	return REC;

	//	unsigned int i = 0;
	// ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	// while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	// 	;
	// REC = ADC_GetConversionValue(ADC1);

	//ADC_InitTypeDef ADC_InitStructure;

	// ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����ģʽ
	// ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	// ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	// ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	// ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	// ADC_InitStructure.ADC_NbrOfChannel = 1;
	// ADC_Init(ADC1, &ADC_InitStructure);
	// RCC_ADCCLKConfig(RCC_PCLK2_Div8);											//9HZ
	// ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5); //�������� //ͨ��6
	// ADC_DMACmd(ADC1, ENABLE);
	// ADC_Cmd(ADC1, ENABLE);
	// ADC_ResetCalibration(ADC1);

	// while (ADC_GetResetCalibrationStatus(ADC1)) //У׼
	// 	;
	// ADC_StartCalibration(ADC1);

	// while (ADC_GetCalibrationStatus(ADC1)) //�ȴ�У׼���
	// 	;

	// // //��ȡ�ο���ѹ
	// // GAME_STATUS.LIGHT_1_SAMPLE = Get_ADC_1_DATA();

	// for (i = 8; i > 0; i--)
	// {

	// 	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// 	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
	// 		; //�ȴ�ת������
	// 	REC += ADC_GetConversionValue(ADC1);
	// 	// Voice_Delay_Ms(1);
	// }

	// // IWDG_FeedDog();

	// return (REC >> 3); //���س���8����

	// //	return REC;
}

unsigned int Get_ADC_2_DATA(void)
{
	return (GPIO_ReadInputDataBit(LIGHT_EYE_2_REC_PORT, LIGHT_EYE_2_REC_PIN));
}

void Light_Adc_Dma_Init(void)
{
	// 	/*

	// 	DMA_InitTypeDef DMA_InitStructure;
	// 	ADC_InitTypeDef ADC_InitStructure;

	// 	/* DMA channel1 configuration */
	// 	DMA_DeInit(DMA1_Channel1);
	// 	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		 /*ADC
	// ��ַ*/
	// 	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue; /*
	// �ڴ��ַ*/
	// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				 //����Ϊ����Դ
	// 	DMA_InitStructure.DMA_BufferSize = 1;
	// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			/*
	// �����ַ�̶�*/
	// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;					/*�ڴ��
	// ַ�̶�*/
	// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����
	// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //ѭ������
	// 	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// 	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	// 	/* Enable DMA channel1 */
	// 	DMA_Cmd(DMA1_Channel1, ENABLE);

	ADC_InitTypeDef ADC_InitStructure;

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);											//9HZ
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5); //�������� //ͨ��6
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);

	while (ADC_GetResetCalibrationStatus(ADC1)) //У׼
		;
	ADC_StartCalibration(ADC1);

	while (ADC_GetCalibrationStatus(ADC1)) //�ȴ�У׼���
		;

	//��ȡ�ο���ѹ
	GAME_STATUS.LIGHT_1_SAMPLE = Get_ADC_1_DATA();
}

void Light_Eye_Pins_Init(void)
{

	//����1
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);

	// //����1 ��������
	// GPIO_InitStructure.GPIO_Pin = LIGHT_EYE_1_SEND_PIN;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(LIGHT_EYE_1_SEND_PORT, &GPIO_InitStructure);

	//����1 ADC��������
	GPIO_InitStructure.GPIO_Pin = LIGHT_EYE_1_REC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LIGHT_EYE_1_REC_PORT, &GPIO_InitStructure);

	// //����2 ��������
	// GPIO_InitStructure.GPIO_Pin = LIGHT_EYE_2_SEND_PIN;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(LIGHT_EYE_2_SEND_PORT, &GPIO_InitStructure);

	//����2 ADC��������
	GPIO_InitStructure.GPIO_Pin = LIGHT_EYE_2_REC_PIN; //ADC14ͨ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LIGHT_EYE_2_SEND_PORT, &GPIO_InitStructure);

	//Light_Adc_Dma_Init();

	// //���÷�������Ϊ��
	// GPIO_ResetBits(LIGHT_EYE_1_SEND_PORT, LIGHT_EYE_1_SEND_PIN);
	// GPIO_ResetBits(LIGHT_EYE_2_SEND_PORT, LIGHT_EYE_2_SEND_PIN);
	// //	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void sub_Check_Light_Eye_1(void)
{

	GAME_STATUS.LIGHT_1_CURRENT = Get_ADC_1_DATA();

	if (GAME_STATUS.LIGHT_1_CURRENT == 1)
	{

		if (EGG_OUT == 2 && GAME_STATUS.DROP_EGG != 1)
		{

			GAME_STATUS.DROP_EGG = 1;
		}
		else
		{
			//USART1_Send_String("��Ҫ����A\r\n");
			Delay_72M_Ms(20);
			if (VOICE_Check_Busy() == 1)
			{
				VOICE_Play(VOICE_DO_NOT_HAND_IN);
			}
		}
	}

	GAME_STATUS.LIGHT_1 = GAME_STATUS.LIGHT_1_CURRENT;
}

void sub_Check_Light_Eye_2(void)
{

	GAME_STATUS.LIGHT_2_CURRENT = Get_ADC_2_DATA();

	if (GAME_STATUS.LIGHT_2_CURRENT == 1)
	{

		Delay_72M_Ms(10);

		//��������ڳ����е���Ϸ�����������
		if (GAME_STATUS.PLAYING_MUSIC == 1 && GAME_STATUS.EGG_OUT_ING == 1)
		{
			Voice_Play_Stop();
			USART1_Send_String("555555\r\n");
			VOICE_Play(VOICE_DO_NOT_HAND_IN);
			GAME_STATUS.PLAYING_MUSIC = 0;
		}
		else
		{
			if (VOICE_Check_Busy() == 1)
			{
				USART1_Send_String("666666\r\n");
				VOICE_Play(VOICE_DO_NOT_HAND_IN);
			}
		}
	}

	GAME_STATUS.LIGHT_2 = GAME_STATUS.LIGHT_2_CURRENT;
}

void Check_Light_Eye_Status(void)
{

	sub_Check_Light_Eye_2();
	sub_Check_Light_Eye_1();
}
