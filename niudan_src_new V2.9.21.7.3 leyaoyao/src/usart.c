#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "usart.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "wifi.h"
#include "spiflash.h"
unsigned char HEX_TABLE[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

#define USART_1 USART1
#define USART_1_CLK RCC_APB2Periph_USART1

#define USART_1_RX_GPIO_PORT GPIOA
#define USART_1_RX_GPIO_CLK RCC_APB2Periph_GPIOA
#define USART_1_RX_PIN GPIO_Pin_10
#define USART_1_RX_AF GPIO_AF_USART1
#define USART_1_RX_SOURCE GPIO_PinSource10

#define USART_1_TX_GPIO_PORT GPIOA
#define USART_1_TX_GPIO_CLK RCC_APB2Periph_GPIOA
#define USART_1_TX_PIN GPIO_Pin_9
#define USART_1_TX_AF GPIO_AF_USART1
#define USART_1_TX_SOURCE GPIO_PinSource9

#define USART_2 USART2
#define USART_2_CLK RCC_APB1Periph_USART2

#define USART_2_RX_GPIO_PORT GPIOA
#define USART_2_RX_GPIO_CLK RCC_APB2Periph_GPIOA
#define USART_2_RX_PIN GPIO_Pin_3
#define USART_2_RX_AF GPIO_AF_USART2
#define USART_2_RX_SOURCE GPIO_PinSource3

#define USART_2_TX_GPIO_PORT GPIOA
#define USART_2_TX_GPIO_CLK RCC_APB2Periph_GPIOA
#define USART_2_TX_PIN GPIO_Pin_2
#define USART_2_TX_AF GPIO_AF_USART2
#define USART_2_TX_SOURCE GPIO_PinSource2

#define USART_3 USART3
#define USART_3_CLK RCC_APB1Periph_USART3

#define USART_3_RX_GPIO_PORT GPIOB
#define USART_3_RX_GPIO_CLK RCC_APB2Periph_GPIOB
#define USART_3_RX_PIN GPIO_Pin_11
#define USART_3_RX_AF GPIO_AF_USART3
#define USART_3_RX_SOURCE GPIO_PinSource11

#define USART_3_TX_GPIO_PORT GPIOB
#define USART_3_TX_GPIO_CLK RCC_APB2Periph_GPIOB
#define USART_3_TX_PIN GPIO_Pin_10
#define USART_3_TX_AF GPIO_AF_USART3
#define USART_3_TX_SOURCE GPIO_PinSource10

void USART1_Init(uint32_t BAUDRATE)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //初始化GPIOA.10

	//Usart1 NVIC 配置
	// NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级3
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	// NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = BAUDRATE;									//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口接受中断
	USART_Cmd(USART1, ENABLE);
}

void USART_485_Pins_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 //初始化GPIOA.9

	USART_485_Disable();
}

void USART_485_Enable(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void USART_485_Disable(void)
{
	Delay_72M_Ms(30);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void USART2_Init(uint32_t BAUDRATE)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	//接收寄存器不为空则进入中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART4_Init(uint32_t BAUDRATE)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);
	//接收寄存器不为空则进入中断
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART3_Init(uint32_t BAUDRATE)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(USART_3_RX_GPIO_CLK | USART_3_TX_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(USART_3_CLK, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_3_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_3_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_3_RX_PIN;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Init(USART_3_RX_GPIO_PORT, &GPIO_InitStructure);
	

	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_3, &USART_InitStructure);
	USART_Cmd(USART_3, ENABLE);


	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
}

void USART1_Send_Byte(unsigned char uc)
{

	USART_SendData(USART_1, uc);
	while (USART_GetFlagStatus(USART_1, USART_FLAG_TXE) == 0)
		;
}

void USART2_Send_Byte(unsigned char uc)
{

	USART_SendData(USART_2, uc);
	while (USART_GetFlagStatus(USART_2, USART_FLAG_TXE) == 0)
		;
}

void USART3_Send_Byte(unsigned char uc)
{

	USART_SendData(USART_3, uc);
	while (USART_GetFlagStatus(USART_3, USART_FLAG_TXE) == 0)
		;
}

//鍙戦�佸瓧绗︿覆
void USART1_Send_String(char *str)
{

	while ((*str) != '\0')
	{

		USART1_Send_Byte(*str);
		str++;
	}
}

void USART2_Send_String(char *str)
{

	while ((*str) != '\0')
	{

		USART2_Send_Byte(*str);
		str++;
	}
}

void USART2_Send_B_S(char *str, char len)
{
	int i = 0;

	for (i = 0; i < len; i++)
	{

		USART2_Send_Byte(*str);
		str++;
	}
}

void USART1_Send_Hex(unsigned char num)
{

	char disp_buffer[5];

	disp_buffer[4] = 0;
	disp_buffer[0] = '0';
	disp_buffer[1] = 'x';

	disp_buffer[2] = HEX_TABLE[(num >> 4) & 0xf];
	disp_buffer[3] = HEX_TABLE[num & 0xf];

	USART1_Send_String(disp_buffer);
}

void USART1_Send_Dec(unsigned int num, unsigned char ucNumCount)
{

	//字符串都是以0结尾，所以都需要补0，因为如果发送3个数字，实际上占用4个位置

	char disp_buffer[30];

	switch (ucNumCount)
	{
	case 3:

		disp_buffer[0] = HEX_TABLE[num / 100];
		disp_buffer[1] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[2] = HEX_TABLE[num % 10];
		disp_buffer[3] = 0;
		USART1_Send_String(disp_buffer);
		break;

	case 4:

		disp_buffer[0] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[1] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[2] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[3] = HEX_TABLE[num % 10];
		disp_buffer[4] = 0;

		USART1_Send_String(disp_buffer);
		break;

	case 5:
		disp_buffer[0] = HEX_TABLE[(num % 100000) / 10000];
		disp_buffer[1] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[2] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[3] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[4] = HEX_TABLE[num % 10];
		disp_buffer[5] = 0;

		USART1_Send_String(disp_buffer);
		break;

	case 6:

		disp_buffer[0] = HEX_TABLE[num / 100000];
		disp_buffer[1] = HEX_TABLE[(num % 100000) / 10000];
		disp_buffer[2] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[3] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[4] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[5] = HEX_TABLE[num % 10];
		disp_buffer[6] = 0;

		USART1_Send_String(disp_buffer);

		break;

	case 7:
		disp_buffer[0] = HEX_TABLE[num / 1000000];
		disp_buffer[1] = HEX_TABLE[(num % 1000000) / 100000];
		disp_buffer[2] = HEX_TABLE[(num % 100000) / 10000];
		disp_buffer[3] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[4] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[5] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[6] = HEX_TABLE[num % 10];
		disp_buffer[7] = 0;

		USART1_Send_String(disp_buffer);

		break;

	case 8:
		disp_buffer[0] = HEX_TABLE[num / 10000000];
		disp_buffer[1] = HEX_TABLE[(num % 10000000) / 1000000];
		disp_buffer[2] = HEX_TABLE[(num % 1000000) / 100000];
		disp_buffer[3] = HEX_TABLE[(num % 100000) / 10000];
		disp_buffer[4] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[5] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[6] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[7] = HEX_TABLE[num % 10];
		disp_buffer[8] = 0;

		USART1_Send_String(disp_buffer);
		break;

	case 9:

		disp_buffer[0] = HEX_TABLE[num / 100000000];
		disp_buffer[1] = HEX_TABLE[(num % 100000000) / 10000000];
		disp_buffer[2] = HEX_TABLE[(num % 10000000) / 1000000];
		disp_buffer[3] = HEX_TABLE[(num % 1000000) / 100000];
		disp_buffer[4] = HEX_TABLE[(num % 100000) / 10000];
		disp_buffer[5] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[6] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[7] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[8] = HEX_TABLE[num % 10];
		disp_buffer[9] = 0;

		USART1_Send_String(disp_buffer);

		break;

	case 10:

		disp_buffer[0] = HEX_TABLE[num / 1000000000];
		disp_buffer[1] = HEX_TABLE[(num % 1000000000) / 100000000];
		disp_buffer[2] = HEX_TABLE[(num % 100000000) / 10000000];
		disp_buffer[3] = HEX_TABLE[(num % 10000000) / 1000000];
		disp_buffer[4] = HEX_TABLE[(num % 1000000) / 100000];
		disp_buffer[5] = HEX_TABLE[(num % 100000) / 10000];
		disp_buffer[6] = HEX_TABLE[(num % 10000) / 1000];
		disp_buffer[7] = HEX_TABLE[(num % 1000) / 100];
		disp_buffer[8] = HEX_TABLE[(num % 100) / 10];
		disp_buffer[9] = HEX_TABLE[num % 10];
		disp_buffer[10] = 0;

		USART1_Send_String(disp_buffer);

		break;

	default:
		break;
	}
}

void USART1_Send_Debug_Message(char *msg)
{
#ifdef DEBUG
	USART1_Send_String(msg);
#endif
}

unsigned char USART3_Receive_Byte(void)
{
	unsigned char ucTemp = 0;

	while (USART_GetFlagStatus(USART_3, USART_FLAG_RXNE) != 1)
		;
	ucTemp = USART_ReceiveData(USART_3);

	return ucTemp;
}

unsigned char USART2_Receive_Byte(void)
{
	unsigned char ucTemp = 0;

	while (USART_GetFlagStatus(USART_2, USART_FLAG_RXNE) != 1)
		;
	ucTemp = USART_ReceiveData(USART_2);

	return ucTemp;
}

unsigned char USART1_Receive_Byte(void)
{
	unsigned char ucTemp = 0;

	while (USART_GetFlagStatus(USART_1, USART_FLAG_RXNE) != 1)
		; //绛夊緟鎺ユ敹鎺ユ敹鏍囧織浣嶄负1
	ucTemp = USART_ReceiveData(USART_1);

	return ucTemp;
}

void USART4_Send_Byte(unsigned char uc)
{

	USART_SendData(UART4, uc);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == 0)
		;
}
