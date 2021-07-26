#include "stm32f10x.h"



#define H_VERSION 2
#define L_VERSION 8
//24 �����������ʾ����

//#define DEBUG  0

#define _RET_CMD_OK 0  //ָ����ȷ
#define _RET_HEART 1 //����������
#define _RET_CMD_ERROR 2 //ָ�����
#define _RET_CATCH_OK 3 //ץȡ�ɹ�
#define _RET_GAME_END 4 //��Ϸ����

#define SPEED1 300

#define SPEED2 700

#define SETUP_READ 0
#define SETUP_SAVE 1

/*
uint32_t PrizeSvaePage;//δ�ϴ��н���¼��ѭ��д��ҳ
uint32_t CoinSvaePage;//δ�ϴ�Ͷ�Ҽ�¼��ѭ��д��ҳ
uint32_t SetupSvaePage;//��������
uint32_t DiffSvaePage;//32���Ѷ�ѭ��
uint32_t PowerSvaePage;//צ��
*/

extern unsigned char ucCoinStatus;

#define WUMAHANG //������


extern int  Sky_LR_V[2];
extern int  Sky_FB_V[2];
extern int  Sky_UD_V[2];

extern int  PowerVar[10];

// wifi setup
extern unsigned char WiFiName[40];
extern unsigned char WiFiPass[20];
extern unsigned char WiFiServer[50];
extern unsigned char ApName[20];
extern unsigned char ApPass[20];
extern unsigned char WiFi_MODE;
extern char LinkNet;
extern unsigned int Wifi_Timeout;

extern char Server_Name[40];
extern int Server_Port; //�������˿�
extern int Music_Volume;

extern unsigned short size;

extern unsigned int DROP_TIME;
extern unsigned int DROP_ENABLE;
extern unsigned int WAWA_GET_OK;
extern unsigned int WAWA_DROP_CHECK_ENABLE;

//�ƶ����ٶ�����
extern unsigned int GAME_SETTING_LEFT_SPEED;
extern unsigned int GAME_SETTING_RIGHT_SPEED;
extern unsigned int GAME_SETTING_FRONT_SPEED;
extern unsigned int GAME_SETTING_BACK_SPEED;
extern unsigned int GAME_SETTING_UP_SPEED;
extern unsigned int GAME_SETTING_DOWN_SPEED;
extern unsigned int GAME_SETTING_CATCH_POWER;
extern unsigned int GAME_SETTING_LITTLE_CATCH_SPEED;
extern unsigned int GAME_SETTING_LEFT_AND_RIGHT_SPEED;
extern unsigned int GAME_SETTING_FRONT_AND_BACK_SPEED;
extern unsigned int GAME_SETTING_UP_AND_DOWN_SPEED;
extern unsigned int GAME_STOP;
extern unsigned int GAME_CHECK_BACK_STOP;
extern unsigned int GAME_CHECK_LEFT_STOP;
extern unsigned int GAME_CHECK_FRONT_STOP;
extern unsigned int SYSTICK_COUNT;
extern unsigned int HEART_ERROR;
extern unsigned int GAME_STATUS_CATCHING;
void Delay_Ms(unsigned long nCount);
void Device_Return_Cmd(unsigned char ucCmd);
void USB_Up_Power_Reset(void);
void USB_Down_Power_Reset(void);


extern unsigned int GAME_SETTING_BEN;

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

extern unsigned char gameTime;  //��Ϸʱ�� �뵥λ
extern unsigned char encCode;  // �Ƿ�������
extern unsigned char iName[10];  // ��������
extern unsigned char uartPasswd[4];  // 4λ���ڷ�������
extern unsigned char superUartPasswd[4];  // ����4λ���ڷ������룬��������id�л��
extern unsigned char Rn[20]; 
extern unsigned char gameRn[20];
extern unsigned char scanPwoer;       // ɨ�迪�� 

extern unsigned char idle_handler;



void System_Setting_Key_Factory(void);


extern unsigned char EGG_OUT;





void Delay_72M_S(unsigned long ulTime);
void Delay_72M_Ms(unsigned long ulTime);
void Delay_72M_Us(unsigned long ulTime);




extern unsigned char ucMusicEnable;


