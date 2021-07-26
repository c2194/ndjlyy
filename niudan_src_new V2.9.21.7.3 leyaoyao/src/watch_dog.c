#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
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
#include "watch_dog.h"


void IWDG_Init(uint8_t prv ,uint16_t rlv)
{    
    // ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
    IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
    
    // ����Ԥ��Ƶ��ֵ
    IWDG_SetPrescaler( prv );
    
    // ������װ�ؼĴ���ֵ
    IWDG_SetReload( rlv );
    
    // ����װ�ؼĴ�����ֵ�ŵ���������
    IWDG_ReloadCounter();
    
    // ʹ�� IWDG
    IWDG_Enable();    
}

void IWDG_Feed(void)
{
    // ����װ�ؼĴ�����ֵ�ŵ��������У�ι������ֹIWDG��λ
    // ����������ֵ����0��ʱ������ϵͳ��λ
    IWDG_ReloadCounter();
}


