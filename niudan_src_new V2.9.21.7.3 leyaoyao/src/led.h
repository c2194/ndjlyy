#ifndef __LED_H
#define __LED_H	 





#define LED_RED_GPIO_PORT GPIOB
#define LED_RED_GPIO_PIN GPIO_Pin_6

#define LED_BLUE_GPIO_PORT GPIOB
#define LED_BLUE_GPIO_PIN GPIO_Pin_7

void LED_Gpio_Init(void);
void LED_Red_On(void);
void LED_Blue_On(void);
void LED_Red_Off(void);
void LED_Blue_Off(void);















/*********输出*************************************/
#define FRONT_DIR_1 GPIO_SetBits(GPIOC, GPIO_Pin_0)	
#define FRONT_DIR_0 GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define AFTER_DIR_1 GPIO_SetBits(GPIOC, GPIO_Pin_1)	
#define AFTER_DIR_0 GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define LEFT_DIR_1 GPIO_SetBits(GPIOC, GPIO_Pin_2)	
#define LEFT_DIR_0 GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define RIGHT_DIR_1 GPIO_SetBits(GPIOC, GPIO_Pin_3)	
#define RIGHT_DIR_0 GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define UP_DIR_1 GPIO_SetBits(GPIOB, GPIO_Pin_8)	
#define UP_DIR_0 GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define DOWN_DIR_1 GPIO_SetBits(GPIOB, GPIO_Pin_9)	
#define DOWN_DIR_0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)



#define BIG_DIR_1 GPIO_SetBits(GPIOA, GPIO_Pin_11)	
#define BIG_DIR_0 GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define SMALL_DIR_1 GPIO_SetBits(GPIOA, GPIO_Pin_12)	
#define SMALL_DIR_0 GPIO_ResetBits(GPIOA,GPIO_Pin_12)





/*****************输入***************************/
#define UP_OK     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)  //上到位检测点
#define DOWN_OK   GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)  //下到位检测点
#define LEFT_OK   GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)  //左到位检测点
#define FRONT_OK  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)  //前到位检测点
#define AFTER_OK  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)  //后到位检测点


void L6205_Init(void);//初始化
//void MotorMove_Front(void);
void MotorMove_Front_ON( void);
void MotorMove_Front_OFF( void);
void MotorMove_Back_REST(void);
//void MotorMove_Back(void);
void MotorMove_Back_ON(void);
void MotorMove_Back_OFF(void);
void MotorMove_FrontAndBack_STOP(void);
//void MotorMove_Left(void);
void MotorMove_Left_ON(void);
void MotorMove_Left_OFF(void);

void MotorMove_Left_REST(void); 
void MotorMove_Front_REST(void); 
//void MotorMove_Right(void);

void MotorMove_Front_Left_ON( void); 

void MotorMove_Front_Right_ON( void); 


void MotorMove_Back_Left_ON( void); 
void MotorMove_Back_Right_ON( void); 


void MotorMove_Right_ON(void);
void MotorMove_Right_OFF(void);
void MotorMove_LeftAndRight_STOP(void);
void MotorMove_Up(void);
//void MotorMove_Down(void);
void MotorMove_Down(int ms);
void MotorMove_UpAndDown_STOP(void);
void Right_After_Move(void); 
void Right_Front_Move(void);
void Left_Front_Move(void);
void Left_After_Move(void);
void Grab(void);
void Grab_small(void);
void Grab_STOP(void);
#endif
