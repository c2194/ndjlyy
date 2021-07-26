#include "stm32f10x.h"



#define ENABLE_485


#define HAND_IN_STOP_MOTO_TIME 





struct game_status
{
		unsigned char ID;  //�豸ID 0-255
		unsigned char DISABLE; //�豸�Ƿ��������
		uint32_t COIN_COUNT; //�Ѿ�Ͷ����
		uint32_t TOTAL_INSERT_COIN_COUNT; //ȫ��Ͷ����
		uint32_t MACHINE_INSERT_COUNT; //��ϷͶ����Ͷ����
		unsigned char GAME_COINS; //ÿ�ֱ�����
		uint32_t GAME_COUNT; //��Ϸ�Ѿ����еľ���
		uint32_t GAME_EGG_OUT_COUNT; //��Ϸ�ܳ�����
		unsigned char LIGHT_1; //����1״̬
		unsigned char LIGHT_2; //����2״̬
		unsigned char LIGHT_3; //����3״̬
		unsigned char OPEN_KEY; //����״̬
		unsigned char TARGET_ID; //���͹�����ID
		unsigned char ERROR; //����״̬
		unsigned char SWITCH_OUT_READEY; //��Ϸ��
		unsigned char EGG_OUT_WAIT_TIME; //��Ϸʱ��
		unsigned char MOTO_TIME_ENABLE;
		unsigned char CURRENT_LOCK_STATUS;

		unsigned char PLAYING_MUSIC;

		unsigned char COIN_MACHINE_STATUS;
		unsigned char LED_1_STATUS;
		unsigned char LED_2_STATUS;
	
	

	
	
		unsigned char MOTO_TIME; //���ת��ʱ��
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





#define SWITCH_OUT 1 //���س���
#define TIME_OVER_OUT 2     //��ʱ����



#define EGG_OUT_SUCCEED 1  //�����ɹ�
#define EGG_OUT_TIME_OUT 2 //������ʱ



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

