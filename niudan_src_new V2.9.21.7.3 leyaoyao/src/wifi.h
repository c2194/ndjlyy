#ifndef __wifi_H
#define __wifi_H	 
#include "sys.h"
#include "delay.h"

extern u8 ICOMMAND;
extern u8 Internet_Linked ;
int Re_Str(char *InStr , char *reStr, char *chSrt,int lo);
int Re_Str2(char *InStr , char *reStr, char *chSrtSt ,char *chSrtEn);

int Re_Str3(unsigned char *InStr ,char *chSrtb);

void Wm_State(void);
extern uint8_t St_Now[10];
void WiFi_while(void);

void EXTI_PA0_Init(void);

extern unsigned char  Wifi_R_B[2048]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern int   Wifi_R_B_L;

extern uint8_t abc[1024];

void SendHtml(void);

extern u8 urAsio;

void LinkWifi(void);

extern u8 Wconnect; //????
extern u8 WconnFlag;

void my_delay(int time);

extern char STS[1024]; //发送缓冲 
extern u8 STS_LOCK;

void SendS_Step_1(unsigned char CH,unsigned char *data,char AorS);
void SendS_Step_2(void);

extern u8 A_L_Port;

extern u8 S_L_Port;
extern  int APPorWEB;

extern u8 AP_state;
extern void U4_Message(void);

extern void Run_Command(void);

extern int Audio_Par; //段落名称
extern int Audio_Rec_Flag; //是否接收状态
extern int A_Rec_Con; //接收计数器

extern int Heart_Time_here;
extern int Setup_MODE;
extern char STS[1024];

extern int ResetAdd;
extern u8 Check_STS;

extern char Send_finish;
extern char RequestSend[25];
#endif
