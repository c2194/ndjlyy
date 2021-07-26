#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "W5500.h"
#include <string.h>
#include "usart.h"
#include "timer.h"
#include "main.h"
#include "game.h"
#include "music.h"



void TIM4_Int_Init(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //��ʱ�� TIM3 ʹ��
  //��ʱ�� TIM3 ��ʼ��
  TIM_TimeBaseStructure.TIM_Period = arr;                     //�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //�ڳ�ʼ�� TIM3
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);                  //����������ж�
  //�ж����ȼ� NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //TIM3 �ж�
 // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
 // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ� 3 ��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ ͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);                           //�ܳ�ʼ�� NVIC �Ĵ���
  TIM_Cmd(TIM4, ENABLE);                                    //��ʹ�� TIM3
}





