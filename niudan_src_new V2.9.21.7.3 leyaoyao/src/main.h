#include "stm32f10x.h"



#define H_VERSION 2
#define L_VERSION 8
//24 增加数码管显示问题

//#define DEBUG  0

#define _RET_CMD_OK 0  //指令正确
#define _RET_HEART 1 //发送心跳包
#define _RET_CMD_ERROR 2 //指令错误
#define _RET_CATCH_OK 3 //抓取成功
#define _RET_GAME_END 4 //游戏重新

#define SPEED1 300

#define SPEED2 700

#define SETUP_READ 0
#define SETUP_SAVE 1

/*
uint32_t PrizeSvaePage;//未上传中奖记录的循环写入页
uint32_t CoinSvaePage;//未上传投币记录的循环写入页
uint32_t SetupSvaePage;//各设置项
uint32_t DiffSvaePage;//32个难度循环
uint32_t PowerSvaePage;//爪力
*/

extern unsigned char ucCoinStatus;

#define WUMAHANG //五马行


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
extern int Server_Port; //服务器端口
extern int Music_Volume;

extern unsigned short size;

extern unsigned int DROP_TIME;
extern unsigned int DROP_ENABLE;
extern unsigned int WAWA_GET_OK;
extern unsigned int WAWA_DROP_CHECK_ENABLE;

//移动等速度设置
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

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

extern unsigned char gameTime;  //游戏时间 秒单位
extern unsigned char encCode;  // 是否开启加密
extern unsigned char iName[10];  // 机器名称
extern unsigned char uartPasswd[4];  // 4位串口访问密码
extern unsigned char superUartPasswd[4];  // 超级4位串口访问密码，倾向于在id中获得
extern unsigned char Rn[20]; 
extern unsigned char gameRn[20];
extern unsigned char scanPwoer;       // 扫描开关 

extern unsigned char idle_handler;



void System_Setting_Key_Factory(void);


extern unsigned char EGG_OUT;





void Delay_72M_S(unsigned long ulTime);
void Delay_72M_Ms(unsigned long ulTime);
void Delay_72M_Us(unsigned long ulTime);




extern unsigned char ucMusicEnable;


