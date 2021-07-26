#include "global.h"


unsigned  char ucNumSwitch(unsigned char ucInput)
{
    unsigned char temp  =  0;
    switch(ucInput)
    {
  


        case 0:
         temp = 0x3F;
        break;

        case 1:
         temp = 0x06;
        break;

          case 2:
         temp = 0x5B;
        break;

          case 3:
         temp = 0x4F;
        break;

          case 4:
         temp = 0x66;
        break;

          case 5:
         temp = 0x6D;
        break;

          case 6:
         temp = 0x7D;
        break;

          case 7:
         temp = 0x07;
        break;

          case 8:
         temp = 0x7F;
        break;
          case 9:
         temp = 0x6F;
        break;

          default:
        // temp = 0x3F;
        break;
    }


    return  temp ;
}

void Digital_Tube_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DIGITAL_TUBE_SCL_PIN | DIGITAL_TUBE_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, DIGITAL_TUBE_SCL_PIN | DIGITAL_TUBE_SDA_PIN);


    USART1_Send_String("aaa");

      Write_DATA(0x48, 0x31);
      
        USART1_Send_String("CCC");
}


void Digital_Tube_Display(unsigned int uiNum)
{   
 
    Write_DATA(0x68,ucNumSwitch((uiNum % 10000) / 1000));
    Write_DATA(0x6A,ucNumSwitch((uiNum % 1000) / 100));
    Write_DATA(0x6C,ucNumSwitch((uiNum % 100) / 10));
    Write_DATA(0x6E,ucNumSwitch(uiNum % 10));

}


void Digital_Tube_Display_Error(void)
{   
 
    Write_DATA(0x68,0x79);
    Write_DATA(0x6A,0x79);
    Write_DATA(0x6C,0x79);
    Write_DATA(0x6E,0x79);

}




// void displayData(uint8_t iData, uint8_t tab)
// {
//     int i;
//     int itemp;
//     itemp = iData;
//     for (i = 2; i >= 0; i--)
//     {

//         {
//             Write_DATA(0x68 + 2 * i, tabForDisplay[itemp % 10] | 0x80);
//             ? //显示小数点
//         }
//         ?itemp = itemp / 10;
//     }
// }

void TM1650_start()
{

    DIGITAL_TUBE_SCL_1();
    DIGITAL_TUBE_SDA_1();
    Delay_72M_Us(5);
    DIGITAL_TUBE_SDA_0();
    Delay_72M_Us(5);
}

void TM1650_stop()
{

    DIGITAL_TUBE_SCL_1();
    DIGITAL_TUBE_SDA_0();
    Delay_72M_Us(5);
    DIGITAL_TUBE_SDA_1();
    Delay_72M_Us(5);
}
void TM1650_ACK()
{
    // ?GPIO_InitTypeDef GPIO_InitStruct;
    // ?{
    // ?? HAL_GPIO_WritePin(AnalonSCL_GPIO_Port, AnalonSCL_Pin, GPIO_PIN_SET);
    // ?? GPIO_InitStruct.Pin = AnalonSDA_Pin;
    // ??? GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    // ??? GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // ??? HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    DIGITAL_TUBE_SCL_1();


    DIGITAL_TUBE_SDA_IN();

    Delay_72M_Us(5);
    //?HAL_GPIO_WritePin(AnalonSCL_GPIO_Port, AnalonSCL_Pin, GPIO_PIN_RESET);
    DIGITAL_TUBE_SCL_0();

    Delay_72M_Us(5);

    DIGITAL_TUBE_SDA_OUT();

    // ??GPIO_InitStruct.Pin = AnalonSDA_Pin;
    // ??? GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    // ??? GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // ??? HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    Delay_72M_Us(5);
}
void TM1650_Write(unsigned char DATA) //写数据函数
{
    // ?GPIO_InitTypeDef GPIO_InitStruct;
    unsigned char i;
    // ? GPIO_InitStruct.Pin = AnalonSDA_Pin;
    // ? GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    // ? GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // ? HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // ? Delay_72M_Us(5);
    // (1);
    // ? HAL_GPIO_WritePin(AnalonSCL_GPIO_Port, AnalonSCL_Pin, GPIO_PIN_RESET);

    DIGITAL_TUBE_SDA_OUT();

    Delay_72M_Us(5);

    DIGITAL_TUBE_SCL_0();

    Delay_72M_Us(5);

    for (i = 0; i < 8; i++)
    {
        if (DATA & 0X80)
        {
            DIGITAL_TUBE_SDA_1();
        }
        else
        {
            DIGITAL_TUBE_SDA_0();
        }

        DATA <<= 1;

        DIGITAL_TUBE_SCL_0();
        Delay_72M_Us(5);
        DIGITAL_TUBE_SCL_1();
        Delay_72M_Us(5);

        DIGITAL_TUBE_SCL_0();

        Delay_72M_Us(5);
    }
}

void Write_DATA(unsigned char add, uint8_t DATA) //指定地址写入数据
{
    //  USART1_Send_String("AAA");

    DIGITAL_TUBE_Start();
     DIGITAL_TUBE_Write_Byte(add);

   //    USART1_Send_String("bbb");


    DIGITAL_TUBE_WaitAck(50);

 //   USART1_Send_String("fff");
    DIGITAL_TUBE_Write_Byte(DATA);

 //     USART1_Send_String("ccc");


    DIGITAL_TUBE_WaitAck(50);
    DIGITAL_TUBE_Stop();

  //    USART1_Send_String("eee");
}

/*
void StartDefaultTask(void)
{

    unsigned char i, j;

   // Write_DATA(0x48, 0x31);

    Write_DATA(0x68,ucNumSwitch(8));

    Write_DATA(0x6A,ucNumSwitch(9));

     Write_DATA(0x6C,ucNumSwitch(0));

      Write_DATA(0x6E,ucNumSwitch(7));

    while(1);
      


    



    // while (1)
    // { //根据发送来的数值在数码管显示
    //     for (i = 0; i < 16; i++)
    //     {
    //         for (j = 0; j < 4; j++)
    //         {
    //             Write_DATA(0x68 + 2 * j, tabForDisplay[(j + i) % 10] | 0x80);
    //         }
    //         Delay_72M_Us(500);
    //     }
    //     Delay_72M_Ms(1);
    // }
}
*/

// void Digital_Tube_Init(void)
// {

//     GPIO_InitTypeDef GPIO_InitStructure;
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

//     GPIO_InitStructure.GPIO_Pin = DIGITAL_TUBE_SCL_PIN | DIGITAL_TUBE_SDA_PIN;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOC, &GPIO_InitStructure);

//     GPIO_SetBits(GPIOC, DIGITAL_TUBE_SCL_PIN | DIGITAL_TUBE_SDA_PIN);
// }

//设置SDA为输出
void DIGITAL_TUBE_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DIGITAL_TUBE_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DIGITAL_TUBE_SDA_PORT, &GPIO_InitStructure);

    //  GPIO_SetBits(AT24C64_SDA_PORT,AT24C64_SDA_PIN);
}

void DIGITAL_TUBE_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DIGITAL_TUBE_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DIGITAL_TUBE_SDA_PORT, &GPIO_InitStructure);

    //  GPIO_SetBits(AT24C64_SDA_PORT,AT24C64_SDA_PIN);
}

//起始位
void DIGITAL_TUBE_Start(void)
{
    DIGITAL_TUBE_SDA_OUT(); //SDA 设置为输出

    DIGITAL_TUBE_SCL_1();

    Delay_72M_Us(5);

    DIGITAL_TUBE_SDA_1();

    Delay_72M_Us(5);

    DIGITAL_TUBE_SDA_0();
    Delay_72M_Us(5);
}

//结束位
void DIGITAL_TUBE_Stop(void)
{
    DIGITAL_TUBE_SDA_OUT(); //SDA 设置为输出

    // DIGITAL_TUBE_SCL_1();

    // DIGITAL_TUBE_SDA_0();

    // Delay_72M_Us(5);

    // DIGITAL_TUBE_SDA_1();
    // Delay_72M_Us(5);

     
    DIGITAL_TUBE_SDA_0();
     DIGITAL_TUBE_SCL_1();
     Delay_72M_Us(5);
    DIGITAL_TUBE_SDA_0();
    Delay_72M_Us(5);
    DIGITAL_TUBE_SDA_1();
    Delay_72M_Us(5);
}

// void DIGITAL_TUBE_Ack(void)
// {

// DIGITAL_TUBE_SCL_1();

//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SDA_1();

//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_0();
//     Delay_72M_Us(5);
// }

// void DIGITAL_TUBE_Write_1(void)
// {
//     DIGITAL_TUBE_SDA_1();
//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_0();
//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_1();

//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_0();

//     Delay_72M_Us(5);
// }

// void DIGITAL_TUBE_Write_0(void)
// {
//     DIGITAL_TUBE_SDA_0();
//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_0();
//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_1();

//     Delay_72M_Us(5);

//     DIGITAL_TUBE_SCL_0();

//     Delay_72M_Us(5);
// }

unsigned char DIGITAL_TUBE_WaitAck(unsigned int wait_time)
{
//    unsigned char ucErrTime = 0; //重试时间


  DIGITAL_TUBE_SDA_IN();   //改变方向///   
  DIGITAL_TUBE_SDA_1();
  Delay_72M_Us(5);	   
  DIGITAL_TUBE_SCL_1();
  Delay_72M_Us(5);	 

  if(DIGITAL_TUBE_READ_SDA()==0)
  {
      DIGITAL_TUBE_SCL_0();//时钟输出0 	   
  return 0;  //正常返回0 
  }
 
    return 1; //没有应答  
      
  
  
  

  

  
//   DIGITAL_TUBE_SDA_IN();   //改变方向///   
//   DIGITAL_TUBE_SDA_1();
//   Delay_72M_Us(5);	   
//   DIGITAL_TUBE_SCL_1();
//   Delay_72M_Us(5);	 
  
//   while(DIGITAL_TUBE_READ_SDA())
//   {
//     ucErrTime++;
//     if(ucErrTime>5000)
//     {
//       //  USART1_Send_String("ERROR");
//       DIGITAL_TUBE_Stop();
//       return 1; //没有应答
//     }
//   }
//   DIGITAL_TUBE_SCL_0();//时钟输出0 	   
//   return 0;  //正常返回0 


}

// void tm1650_writebyte(uint8_t DATA)
// {

//     unsigned char i;
// 	Delay_72M_Us(5);
// 	DIGITAL_TUBE_SCL_0();
// 	for(i=0;i<8;i++)
// 	{
// 		if(DATA&0X80)
// 			DIGITAL_TUBE_SDA_1();
// 		else
// 			DIGITAL_TUBE_SDA_0();
// 		DATA<<=1;
// 		DIGITAL_TUBE_SCL_0();
// 		Delay_72M_Us(5);
// 		DIGITAL_TUBE_SCL_1();
// 		Delay_72M_Us(5);
// 		DIGITAL_TUBE_SCL_0();
// 		Delay_72M_Us(5);
// 	}
// }

void DIGITAL_TUBE_Write_Byte(unsigned char byte)
{
    unsigned char i = 0;

    DIGITAL_TUBE_SDA_OUT(); //改变方向

    DIGITAL_TUBE_SCL_0();

    Delay_72M_Us(5);

    for (i = 0; i < 8; i++)
    {

        Delay_72M_Us(5);
        if (byte & 0x80)
        {
            DIGITAL_TUBE_SDA_1();
        }
        else
        {
            DIGITAL_TUBE_SDA_0();
        }

        Delay_72M_Us(5);
        DIGITAL_TUBE_SCL_1();
        Delay_72M_Us(5);
        DIGITAL_TUBE_SCL_0();
        byte <<= 1;
    }
    // DIGITAL_TUBE_SCL_0();
    // Delay_72M_Us(5);
}

// void Digital_Tube_Display(unsigned int uiNum)
// {

//     //USART1_Send_String("aa");
//     DIGITAL_TUBE_Start();
//     tm1650_writebyte(0x48);
//     DIGITAL_TUBE_Ack();
//     tm1650_writebyte(0x01);
//     DIGITAL_TUBE_Ack();

//     DIGITAL_TUBE_Stop();

//     Delay_72M_Ms(1000);

//     DIGITAL_TUBE_Start();
//     tm1650_writebyte(0x68);
//     DIGITAL_TUBE_Ack();
//     tm1650_writebyte(0x3F);
//      DIGITAL_TUBE_Ack();

//     DIGITAL_TUBE_Stop();

//     Delay_72M_Ms(1000);

//     //USART1_Send_String("aa");

//     DIGITAL_TUBE_Start();
//     tm1650_writebyte(0x6a);

// DIGITAL_TUBE_Ack();
//     tm1650_writebyte(0x3F);
//     DIGITAL_TUBE_Ack();

//     DIGITAL_TUBE_Stop();

//     Delay_72M_Ms(1000);

//     DIGITAL_TUBE_Start();
//     tm1650_writebyte(0x6c);
//     DIGITAL_TUBE_Ack();
//     tm1650_writebyte(0x3F);
//     DIGITAL_TUBE_Ack();

//     DIGITAL_TUBE_Stop();

//     Delay_72M_Ms(1000);

//     DIGITAL_TUBE_Start();
//     tm1650_writebyte(0x6e);
// DIGITAL_TUBE_Ack();
//     tm1650_writebyte(0x3F);
//     DIGITAL_TUBE_Ack();

//     DIGITAL_TUBE_Stop();

//     Delay_72M_Ms(1000);

//     // USART1_Send_String("aa");

//     // //USART1_Send_String("aa");
//     // DIGITAL_TUBE_Start();
//     // DIGITAL_TUBE_Write_Byte(0x48);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Write_Byte(0x11);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Stop();

//     // Delay_72M_Ms(1000);

//     // DIGITAL_TUBE_Start();
//     // DIGITAL_TUBE_Write_Byte(0x68);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Write_Byte(0x3F);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Stop();

//     // Delay_72M_Ms(1000);

//     // //USART1_Send_String("aa");

//     // DIGITAL_TUBE_Start();
//     // DIGITAL_TUBE_Write_Byte(0x6a);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Write_Byte(0x3F);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Stop();

//     // Delay_72M_Ms(1000);

//     // DIGITAL_TUBE_Start();
//     // DIGITAL_TUBE_Write_Byte(0x6c);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Write_Byte(0x3F);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Stop();

//     // Delay_72M_Ms(1000);

//     // DIGITAL_TUBE_Start();
//     // DIGITAL_TUBE_Write_Byte(0x6e);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Write_Byte(0x3F);
//     // DIGITAL_TUBE_WaitAck(50);

//     // DIGITAL_TUBE_Stop();

//     // Delay_72M_Ms(1000);

//     // // USART1_Send_String("aa");
// }


