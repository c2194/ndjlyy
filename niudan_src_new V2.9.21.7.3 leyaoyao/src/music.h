struct Voice
{
    
    void (*PowerOn)(void);
    void (*PowerOff)(void);
    void (*Play)(unsigned char);
    void (*CheckBusy)(void);
    void (*PlayDi)(void);
 
};

#define VOICE_Data_0() GPIO_ResetBits(VOICE_GPIO_PORT, VOICE_GPIO_PINS)
#define VOICE_Data_1() GPIO_SetBits(VOICE_GPIO_PORT, VOICE_GPIO_PINS)

#define VOICE_OK_PLEASE_START_PLAY 0 //ok 开始扭蛋吧
#define VOICE_OK_NIUDAN_ALREADY_OK 1 //OK 扭蛋机已经OK了 
#define VOICE_HAS_ERROR_CONNECT_US 2 
#define VOICE_INSERT_COIN_PLEASE 3
#define VOICE_EGG_OUT 4
#define VOICE_TIME_OUT 5
#define VOICE_I_HAS_SLEEP_CHANGE_OTHER 6
#define VOICE_DO_NOT_HAND_IN 7
#define VOICE_MUSIC_1 8
#define VOICE_COIN_MORE_PLEASE 9
//#define VOICE_ 10

void VOICE_Pins_Init(void);

void VOICE_Play(unsigned char SpeechData);

extern struct Voice VOICE;

//引脚设置 VOICE_DATA PG0
#define VOICE_GPIO_PORT  GPIOB
#define VOICE_GPIO_PINS  GPIO_Pin_8 
#define VOICE_GPIO_BUSY_PORT  GPIOB
#define VOICE_GPIO_BUSY_PINS GPIO_Pin_9

#define VOICE_GPIO_OUT_PORT  GPIOB
#define VOICE_GPIO_OUT_PIN  GPIO_Pin_5 







//void VOICE_Check_Busy_While(void);
unsigned char VOICE_Check_Busy(void);
void Voice_Play_Stop(void);
void VOICE_Out_Pins_Init(void);
void VOICE_Out_Pins_Enable(void);
void VOICE_Out_Pins_Disable(void);



