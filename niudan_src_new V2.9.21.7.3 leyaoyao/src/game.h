#include "stm32f10x.h"



#define ENABLE_485


#define HAND_IN_STOP_MOTO_TIME 





struct game_status
{
		unsigned char ID;  //设备ID 0-255
		unsigned char DISABLE; //设备是否允许操作
		uint32_t COIN_COUNT; //已经投币数
		uint32_t TOTAL_INSERT_COIN_COUNT; //全部投币数
		uint32_t MACHINE_INSERT_COUNT; //游戏投币器投币数
		unsigned char GAME_COINS; //每局币子数
		uint32_t GAME_COUNT; //游戏已经进行的局数
		uint32_t GAME_EGG_OUT_COUNT; //游戏总出蛋数
		unsigned char LIGHT_1; //光眼1状态
		unsigned char LIGHT_2; //光眼2状态
		unsigned char LIGHT_3; //光眼3状态
		unsigned char OPEN_KEY; //被打开状态
		unsigned char TARGET_ID; //发送过来的ID
		unsigned char ERROR; //错误状态
		unsigned char SWITCH_OUT_READEY; //游戏中
		unsigned char EGG_OUT_WAIT_TIME; //游戏时间
		unsigned char MOTO_TIME_ENABLE;
		unsigned char CURRENT_LOCK_STATUS;

		unsigned char PLAYING_MUSIC;

		unsigned char COIN_MACHINE_STATUS;
		unsigned char LED_1_STATUS;
		unsigned char LED_2_STATUS;
	
	

	
	
		unsigned char MOTO_TIME; //电机转动时间
		unsigned char COIN_ENABLE;

		unsigned int LIGHT_1_SAMPLE;
		unsigned int LIGHT_1_CURRENT;
	
		unsigned int LIGHT_2_SAMPLE;
		unsigned int LIGHT_2_CURRENT;

		unsigned int LIGHT_3_SAMPLE;
		unsigned int LIGHT_3_CURRENT;

		unsigned int DROP_DIFF;

		unsigned char DROP_EGG;
		unsigned int DROP_ADC_COUNT;
		
		
		unsigned char EGG_OUT_ING;
		


		

		// unsigned char HEART_ERROR_RESTART;
		// unsigned char GET_DROP;  //?????
		// unsigned char CATCH_CHECK_FLAG;
		// unsigned long MOVE_RIGHT_TIME_OUT;
		// unsigned int  CATCH_MOVE_DOWN_TIME_OUT;
  		// unsigned char START_POSITION;
		// unsigned char MOVE_ENABLE;
		// unsigned char MOVE;
		// unsigned char ARRIVED;
		// unsigned char G1; //[0-9]
		// unsigned char G2[10]; //
		// unsigned char G3[5]; //
		// unsigned char CATCH_POWER[10];
};



extern struct game_status GAME_STATUS;
extern struct game_cmd GAME_CMD;





#define SWITCH_OUT 1 //开关出蛋
#define TIME_OVER_OUT 2     //超时出蛋



#define EGG_OUT_SUCCEED 1  //出蛋成功
#define EGG_OUT_TIME_OUT 2 //出蛋超时



#define COIN_MACHINE 1
#define NET_ADD 2



void Game_Egg_Out(unsigned char ucSelect);


void Game_Egg_Out_Stop(unsigned char ucSelect);
void Game_Status_Value_Init(void);
void Game_Status_Save(void);
void Game_Status_Read(void);
void Game_Moto_Init(void);
void MOTO_On(void);
void MOTO_Off(void);
void Game_Struct_Init(void);
void Get_ChipID(void);
void Game_Egg_Switch_Out(void);
void Check_Coin_Status(void);
void Check_Game_Time_Out(void);
void Coin_Insert_Add(unsigned char ucSelect);
void Game_Coin_Minus(void);

extern unsigned int uiCoinTimeOut;


extern unsigned char EGG_OUT_CHECK_STATUS;


#define EGG_OUT_CHECK_TIME_IDEL 0
#define EGG_OUT_CHECK_TIME_START 1
#define EGG_OUT_CHECK_TIME_OVER 2


//  int obtainVol(unsigned char *C1, unsigned char *inBuf, unsigned char *outBuf);

