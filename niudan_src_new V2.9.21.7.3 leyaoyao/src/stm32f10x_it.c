
#include "stm32f10x_it.h"
#include "usart.h"
#include "main.h"
#include "w5500.h"
#include "game.h"
#include "music.h"
#include "protocol.h"
#include "leyaoyao_protocol.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}


//系统时钟计时器
void SysTick_Handler(void)
{
}

//外部中断1
void EXTI1_IRQHandler(void)
{
 if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
  }
}

void EXTI0_IRQHandler(void)
{

 if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
  }

}

void EXTI4_IRQHandler(void)
{

  if (EXTI_GetITStatus(EXTI_Line4) != RESET)
  {
  }
}



void TIM4_IRQHandler(void) //TIM3 中断
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
  {

    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除 TIM3 更新中断标志

    if (GAME_STATUS.EGG_OUT_WAIT_TIME > 0 && GAME_STATUS.SWITCH_OUT_READEY == 1)
    {
      

        GAME_STATUS.EGG_OUT_WAIT_TIME--;
    }

    if(GAME_STATUS.MOTO_TIME >0 && GAME_STATUS.MOTO_TIME_ENABLE == 1)
    {
      GAME_STATUS.MOTO_TIME--;
    }
  }

//  USART1_Send_String("aaa\r\n");

   TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除 TIM3 更新中断标志
}



void TIM2_IRQHandler(void)
{


    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

}

void TIM7_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}



void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
	{
		
		idle_handler++;
	
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
}



void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
      //USART_SendData(USART1,USART_ReceiveData(USART1));
      SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT] = USART_ReceiveData(USART2);
      
     // USART1_Send_Hex(SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT]);
     // USART_SendData(USART1, SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT]);
      SYSTEM_BUFFER_RECEVICE_COUNT++;
  }
}

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
   // USART1_Send_String("aaa");
   LEYAOYAO_REC_BUFFER[LEYAOYAO_REC_BUFFER_RECEVICE_COUNT] = USART_ReceiveData(USART3);
    //  USART1_Send_Byte(0xCC);
   //   USART1_Send_Hex(LEYAOYAO_REC_BUFFER[LEYAOYAO_REC_BUFFER_COUNT]);
  //    // USART_SendData(USART1, SYSTEM_USART_BUFFER[SYSTEM_BUFFER_RECEVICE_COUNT]);
      LEYAOYAO_REC_BUFFER_RECEVICE_COUNT++;
		idle_handler=0;
  }
	
	


	
	
	
	
}

