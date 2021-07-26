extern unsigned char ucSwitchKeyStatus;
extern unsigned char CURRENT_SWITCH_KEY_STATUS;
extern unsigned char LAST_SWITCH_KEY_STATUS;
extern unsigned char SWITCH_COUNT;

unsigned char ucGetSwitchKeyStatus(void);
void Switch_Key_Init(void);
void Check_Switch_Key_Status(void);
void Check_Switch_Key_Status_New(void);
