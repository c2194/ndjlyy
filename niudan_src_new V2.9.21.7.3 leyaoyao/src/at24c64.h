


//PC9 24C64 SCL 
//PC8 24C64 SDA

#define AT24C64_SDA_PORT GPIOC
#define AT24C64_SDA_PIN GPIO_Pin_8


#define AT24C64_SCL_PORT GPIOC
#define AT24C64_SCL_PIN GPIO_Pin_9


//“˝Ω≈≈‰÷√
#define I2C1_SCL_1()   (GPIO_SetBits(AT24C64_SCL_PORT,AT24C64_SCL_PIN))
#define I2C1_SCL_0()   (GPIO_ResetBits(AT24C64_SCL_PORT,AT24C64_SCL_PIN))

#define I2C1_SDA_1()   (GPIO_SetBits(AT24C64_SDA_PORT,AT24C64_SDA_PIN))
#define I2C1_SDA_0()   (GPIO_ResetBits(AT24C64_SDA_PORT,AT24C64_SDA_PIN))

void I2C1_SDA_OUT(void);
void I2C1_SDA_IN(void);
#define I2C1_READ_SDA()  (GPIO_ReadInputDataBit(AT24C64_SDA_PORT,AT24C64_SDA_PIN))



void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Ack(void);
void I2C1_NoAck(void);
unsigned char I2C1_WaitAck(void);
void I2C1_Write_Byte(unsigned char byte);
unsigned char I2C1_Read_Byte(void);
void I2C1_Pins_Init(void);




typedef struct
{
    unsigned char (*MultiRead)(unsigned int,unsigned char *,unsigned int);
    unsigned char (*ReadByte)(unsigned int);
    unsigned char (*CheckExist)(void);
    void (*PowerOn)(void);
    void (*PowerOff)(void);
    unsigned int (*MultiWrite)(unsigned int, unsigned int, unsigned int);
} __at24c64_struct_type; 

extern __at24c64_struct_type AT24C64;

void AT24C64_Power_On(void);
//unsigned char AT24C64_CheckExist(void);
unsigned char AT24C64_Read_Byte(unsigned int uiReadAddr);
void AT24C64_Write_Byte(unsigned int uiWriteAddr, unsigned char ucWriteData);
void AT24C64_Wirte_All_Byte(unsigned char ucWriteByte);
void AT24C64_Power_Off(void);
void AT24C64_Struct_Value_Init(void);
unsigned char AT24C64_Multi_Write(unsigned int uiStartAddress, unsigned int uiWriteData, unsigned int uiLength);


