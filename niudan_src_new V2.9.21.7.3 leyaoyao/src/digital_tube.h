


void Digital_Tube_Init(void);
void Digital_Tube_Display(unsigned int uiNum);





//PC6 DIGITAL_TUBE SCL 
//PC7 DIGITAL_TUBE SDA

#define DIGITAL_TUBE_SDA_PORT GPIOC
#define DIGITAL_TUBE_SDA_PIN GPIO_Pin_7


#define DIGITAL_TUBE_SCL_PORT GPIOC
#define DIGITAL_TUBE_SCL_PIN GPIO_Pin_6


//引脚配置
#define DIGITAL_TUBE_SCL_1()   (GPIO_SetBits(DIGITAL_TUBE_SCL_PORT,DIGITAL_TUBE_SCL_PIN))
#define DIGITAL_TUBE_SCL_0()   (GPIO_ResetBits(DIGITAL_TUBE_SCL_PORT,DIGITAL_TUBE_SCL_PIN))

#define DIGITAL_TUBE_SDA_1()   (GPIO_SetBits(DIGITAL_TUBE_SDA_PORT,DIGITAL_TUBE_SDA_PIN))
#define DIGITAL_TUBE_SDA_0()   (GPIO_ResetBits(DIGITAL_TUBE_SDA_PORT,DIGITAL_TUBE_SDA_PIN))

void DIGITAL_TUBE_SDA_OUT(void);
void DIGITAL_TUBE_SDA_IN(void);
#define DIGITAL_TUBE_READ_SDA()  (GPIO_ReadInputDataBit(DIGITAL_TUBE_SDA_PORT,DIGITAL_TUBE_SDA_PIN))



void DIGITAL_TUBE_Start(void);
void DIGITAL_TUBE_Stop(void);
void DIGITAL_TUBE_Ack(void);
void DIGITAL_TUBE_NoAck(void);
unsigned char DIGITAL_TUBE_WaitAck(unsigned int wait_time);
void DIGITAL_TUBE_Write_Byte(unsigned char byte);
unsigned char DIGITAL_TUBE_Read_Byte(void);
void DIGITAL_TUBE_Pins_Init(void);


void Digital_Tube_Init(void);
void TM1650_start(void);
void TM1650_stop(void);
void TM1650_ACK(void);
void TM1650_Write(unsigned char DATA); //写数据函数
void Write_DATA(unsigned char add, unsigned char DATA); //指定地址写入数据
void StartDefaultTask(void);


void Digital_Tube_Display(unsigned int uiNum);
void Digital_Tube_Display_Error(void);

