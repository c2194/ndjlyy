#define LEYAOYAO_REC_BUFFER_COUNT 64
extern unsigned char LEYAOYAO_REC_BUFFER[LEYAOYAO_REC_BUFFER_COUNT];
extern unsigned int LEYAOYAO_REC_BUFFER_RECEVICE_COUNT; //当前接收的数量
extern unsigned char LEYAOYAO_RECEVICE_STATE;        //接收的状态
extern unsigned int LEYAOYAO_RECEVICE_ERROR;



void Leyaoyao_Buffer_Clean(void);
unsigned char Leyaoyao_Check_Receive(void);
void __Debug_Send_Leyaoyao_Recevie_Buffer(void);
void leyaoyao_return_pay(void);
void leyaoyao_return_yun_coins_pay(void);
void leyaoyao_return_yun_coins_pay_jieguo(unsigned char ucFlag);
void Leyaoyao_Retrun_Coins_Details(void);

void leyaoyao_return_pay_field(void);
void Leyaoyao_Retrun_Add_Coins(void);
void leyaoyao_return_system_lock(unsigned char ucCode);
