
#include "stm32f10x.h"

void USART1_Init(uint32_t BAUDRATE);
void USART2_Init(uint32_t BAUDRATE);
void USART3_Init(uint32_t BAUDRATE);
void UART4_Init(uint32_t BAUDRATE);
void USART1_Send_Dec(unsigned int num,unsigned char ucNumCount);

void USART1_Send_Byte(unsigned char uc);
void USART2_Send_Byte(unsigned char uc);
void USART3_Send_Byte(unsigned char uc);
unsigned char USART1_Receive_Byte(void);
unsigned char USART2_Receive_Byte(void);
unsigned char USART3_Receive_Byte(void);

void USART1_Send_String(char *str);
void USART1_Send_Hex(unsigned char num);
void USART1_Send_Debug_Message(char *msg);


extern void USART2_Send_String(char *str);
extern void USART2_Send_B_S(char *str,char len);


void USART4_Init(uint32_t BAUDRATE);
void USART4_Send_Byte(unsigned char uc);


#define USART_REC_LEN  			2048  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

#define USART_REC_LEN  			2048  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART_RX_BUF4[2048]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA4;         		//����״̬���	


extern int Uart1_Long;
extern u8  Uart1_flag;

      
extern int Uart4_Long;
extern char  Uart4_flag;

extern int U4_RX;


//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void FlashFromat(void);


extern u32 FindADD(int32_t Add);


void USART_485_Pins_Init(void);
void USART_485_Enable(void);
void USART_485_Disable(void);



extern int Rxw[40][3] ;
extern int thisNum;
extern int prvNum;
extern u8  thisPos;

extern u8  upChar;

extern uint8_t USART2_R_B[700];

extern unsigned char USART2_Flag;
extern uint8_t  AUDIO_R_B[256];
extern uint8_t  AUDIO_R_B_Flag;
extern int USART2_R_B_L;
