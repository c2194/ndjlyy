#include "stm32f10x.h"

void TIM3_PWM_Init(void);
void TIM3_Mode_Config(u16 CCR1_Val, u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val);
static void TIM3_GPIO_Config(void);
void TIM3_Mode_Config_Catch_Pwm(unsigned int CCR4_Val);
void TIM4_Int_Init(u16 arr,u16 psc);

void sendHeart(void);
extern unsigned char unitsTime,tensTime; // ��λ��ʮλ
extern unsigned int uiHeartTime;
extern unsigned int UpID_Time ; // һֱ�����ۼ� ������ֹͣ ����������� �����������ע�⣬��������������� 126 �� ���ᵼ��ϵͳ����
extern unsigned int ConTime ; 

void TIMER7_Init(u16 arr, u16 psc);
void TIMER7_Start(void);
void TIMER7_Stop(void);
void TIM2_Int_Init(u16 arr, u16 psc);


void TIMER2_Start(void);

void TIMER2_Stop(void);






