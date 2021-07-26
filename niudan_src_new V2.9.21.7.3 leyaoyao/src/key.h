#ifndef __key_H
#define __key_H	 
#include "sys.h"
#include "delay.h"


#define KEY_PORT GPIOB
#define KEY_1_PINS GPIO_Pin_12
#define KEY_2_PINS GPIO_Pin_13
#define KEY_3_PINS GPIO_Pin_14


#define COIN_PORT GPIOA
#define COIN_PINS GPIO_Pin_0

#define COIN_INSERT_PORT GPIOA
#define COIN_INSERT_PIN GPIO_Pin_5

void Key_Init(void);
unsigned char get_Setting_Key_Status(void);




//#define GAME_MOVE_LEFT 0x0B
//#define GAME_MOVE_UP 0x0D
//#define GAME_MOVE_RIGHT 0x07
//#define GAME_MOVE_DOWN 0x0E
//#define GAME_MOVE_UP_AND_RIGHT 0x05
//#define GAME_MOVE_DOWN_AND_RIGHT 0x06
//#define GAME_MOVE_DOWN_AND_LEFT 0x0A
//#define GAME_MOVE_UP_AND_LEFT 0x09

#define GAME_MOVE_LEFT 0x0D
#define GAME_MOVE_UP 0x07
#define GAME_MOVE_RIGHT 0x0E
#define GAME_MOVE_DOWN 0x0B

/*
#define GAME_MOVE_LEFT 0x07
#define GAME_MOVE_DOWN 0x0E
#define GAME_MOVE_RIGHT 0x07
#define GAME_MOVE_UP 0x0D
*/
#define GAME_MOVE_UP_AND_RIGHT 0x06
#define GAME_MOVE_DOWN_AND_RIGHT 0x0A
#define GAME_MOVE_DOWN_AND_LEFT 0x09
#define GAME_MOVE_UP_AND_LEFT 0x05



#define GAME_MOVE_NONE 0x0F


	#define GAME_ARRIVED_NONE 0x07
	#define GAME_ARRIVED_LEFT 0x06
	#define GAME_ARRIVED_UP 0X05
	#define GAME_ARRIVED_DOWN 0x03
	#define GAME_ARRIVED_LEFT_UP 0x04
	#define GAME_ARRIVED_LEFT_DOWN 0x02
	
	
	



unsigned char GameMoveCheckArrived(void);

/************************输入************************/
#define ROCKER_AFTER     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)  //摇杆往后移动
#define ROCKER_FRONT     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)  //摇杆往前移动
#define ROCKER_LEFT      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)  //摇杆往左移动
#define ROCKER_RIGHT     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)  //摇杆往右移动
#define KEY_GRAB         GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)  //抓取按钮

#define KEY_SETUP         GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  //抓取按钮


#define  LED_0           GPIO_SetBits(GPIOC, GPIO_Pin_13);
#define  LED_1           GPIO_ResetBits(GPIOC,GPIO_Pin_13);

#define  AFTER_MOVE_DOWN          1
#define  AFTER_MOVE_UP            2
#define  FRONT_MOVE_DOWN          3
#define  FRONT_MOVE_UP            4
#define  LEFT_MOVE_DOWN           5
#define  LEFT_MOVE_UP             6
#define  RIGHT_MOVE_DOWN          7
#define  RIGHT_MOVE_UP            8
#define  GRAB_MOVE_DOWN           9
#define  GRAB_MOVE_UP             10

void Key_Init(void);
unsigned char key_scan(void);
void Game_control(void);
void Game_control2(void);
void Game_control3(void);
void IntensitySet(int *power);
void Game_control4(void);
void Game_control5(void);
void LED1_init(void);



void GameMoveRightAndDown(void);
void GameMoveLeft(void);
void GameMoveRight(void);
void GameMoveDown(void);
void GameMoveStart(void);
void GameMoveUp(void);
void Coin_Pin_Init(void);

void Game_Insert_Coin_Enable(void);
void Game_Insert_Coin_Disable(void);


void Game_Insert_Coin_Enable_Pins_Init(void);
void Game_Insert_Coin_Enable(void);
void Game_Insert_Coin_Disable(void);


#endif

