extern unsigned char Data_Pack_Head[2];
extern unsigned char SYSTEM_DEVICE_ID[12];

unsigned char System_Send_Return(void);
unsigned char System_Check_Receive(void);

void System_Buffer_Clean(void);

void System_Send_Device_ID(void);

extern unsigned int EGG_OUT_TIME;
extern unsigned char EGG_OUT_ERROR;


extern uint64_t COIN_STATUS;

#define _SYSTEM_UNLOCK 0
#define _SYSTEM_RESTART_LOCK 1
#define _SYSTEM_EGG_TIME_OUT_LOCK 2
#define _SYSTEM_NET_CMD_LOCK 3
#define _SYSTEM_NET_CMD_UNLOCK_COIN_DISABLE 4
#define _SYSTEM_LEYAOYAO_LOCK 5

#define SYSTEM_USART_BUFFER_LENGTH 64
#define SYSTEM_USART3_BUFFER_LENGTH 64

extern unsigned char SYSTEM_USART_BUFFER[SYSTEM_USART_BUFFER_LENGTH];
extern unsigned int SYSTEM_BUFFER_RECEVICE_COUNT;
extern unsigned char SYSTEM_RECEIVE_STATE;
extern unsigned char SYSTEM_DEVICE_ID[12];
void System_Send_Key_Status(void);
void System_Setting_ID(unsigned char ucTargetID, unsigned char System_Setting_ID);
void System_Setting_Count(unsigned int uiCoin_Count);
void System_Setting_Light(unsigned int uiCoin_Count);
void System_Setting_Game_Coins(unsigned int uiCoin_Count);
void System_Setting_Game_Disable(unsigned char ucStatus);
void System_Setting_Play_Music(unsigned int uiCoin_Count);
void System_Send_Return_Status(unsigned char ucTargetID, unsigned char ucCommand, unsigned char ucStatus);
void System_Setting_Coin_Insert(unsigned int uiSettings);
void System_Send_Return_Version(unsigned char ucTargetID, unsigned char ucCommand, unsigned char ucStatus);
void System_Send_Return_Total_Days_Coin(unsigned char ucTargetID, unsigned char ucCommand, unsigned char ucStatus);
