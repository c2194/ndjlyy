#include "key.h"
#include "led.h"
#include "usart.h"
#include "game.h"
#include "stm32f10x.h"

extern u16 PWM_RL_speed, PWM_FA_speed, PWM_UD_speed, PWM_BS;
int power[16] = {0};

void LED1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

//按键输入
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				//使能PC PA端口时钟
	GPIO_InitStructure.GPIO_Pin = KEY_1_PINS | KEY_2_PINS | KEY_3_PINS; //PB5-PB7端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						//上拉输入
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

void Coin_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); //使能PC PA端口时钟
	GPIO_InitStructure.GPIO_Pin = COIN_PINS;									//PB5-PB7端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								//上拉输入
	GPIO_Init(COIN_PORT, &GPIO_InitStructure);

	//GPIO_SetBits(COIN_PORT, COIN_PINS);

	// GPIO_InitStructure.GPIO_Pin = COIN_INSERT_PIN;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// GPIO_Init(COIN_INSERT_PORT, &GPIO_InitStructure);

	// GPIO_SetBits(COIN_INSERT_PORT, COIN_INSERT_PIN);
}

void Game_Insert_Coin_Enable_Pins_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// RCC_APB2PeriphClockCmd( GPIOC , ENABLE); //使能PC PA端口时钟

	// GPIO_InitStructure.GPIO_Pin = COIN_INSERT_PIN;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// GPIO_Init(COIN_INSERT_PORT, &GPIO_InitStructure);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //开C口时钟，复用时钟。

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_SetBits(COIN_INSERT_PORT, COIN_INSERT_PIN);
}

void Game_Insert_Coin_Enable(void)
{
	GPIO_SetBits(COIN_INSERT_PORT, COIN_INSERT_PIN);
	GAME_STATUS.COIN_ENABLE = 1;


	GAME_STATUS.COIN_MACHINE_STATUS = 1;
}

void Game_Insert_Coin_Disable(void)
{
	GPIO_ResetBits(COIN_INSERT_PORT, COIN_INSERT_PIN);
	GAME_STATUS.COIN_ENABLE = 0;
	GAME_STATUS.COIN_MACHINE_STATUS = 0;
}

void Coin_Pin_Init_With_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); //使能PC PA端口时钟
	GPIO_InitStructure.GPIO_Pin = COIN_PINS;									//PB5-PB7端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								//上拉输入
	GPIO_Init(COIN_PORT, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //选择PB口的1号口为中断线路
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;					//外部中断线1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设置为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

unsigned char get_Setting_Key_Status(void)
{
	return (GPIO_ReadInputDataBit(KEY_PORT, KEY_1_PINS));
}

unsigned char readGameControlRight(void)
{
	return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15));
}
//读取游戏控制

unsigned char readGameControl(void)
{

	unsigned char ucTemp = 0;

	ucTemp = ((GPIO_ReadInputData(GPIOB) & 0xE0) >> 5); //先读取3个方向
	ucTemp = ucTemp | (readGameControlRight() << 3);	//再读取一个方向放入中间

	return ucTemp;

	/*
	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)  //摇杆往后移动
#define ROCKER_FRONT     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)  //摇杆往前移动
#define ROCKER_LEFT      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)  //摇杆往左移动
#define ROCKER_RIGHT     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)  //摇杆往右移动
	*/
}
