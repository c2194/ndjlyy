 #include "global.h"





//AT24C64 �ڲ���Ϊ256ҳ��ÿҳ32�ֽڣ���13λ��ַѰַ
#define AT24C64_WRITE_ADDRESS 0xA0 //�豸��ַҪ����Ӳ������ A0,1,2 �õ�
#define AT24C64_READ_ADDRESS 0xA1
#define AT24C64_MAX_PAGES 256
#define AT24C64_PAGE_BYTES 32

__at24c64_struct_type AT24C64;



// //���ģ��I2C
// //��ʼ��I2C����
// void I2C1_Pins_Init(void)
// {
//   //GPIO_Init(GPIOC,GPIO_Pin_1|GPIO_Pin_0,GPIO_Mode_Out_PP_High_Slow);
// }



//����SDAΪ���
void I2C1_SDA_OUT(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = AT24C64_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //  GPIO_SetBits(AT24C64_SDA_PORT,AT24C64_SDA_PIN);



}


void I2C1_SDA_IN(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = AT24C64_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //  GPIO_SetBits(AT24C64_SDA_PORT,AT24C64_SDA_PIN);



}


//��ʼλ
void I2C1_Start(void)
{         
  I2C1_SDA_OUT(); //SDA ����Ϊ���
  
  
  Delay_72M_Us(20);
  I2C1_SDA_1();
  I2C1_SCL_1();
  
  
  Delay_72M_Us(20);
  I2C1_SDA_0();
  Delay_72M_Us(20);
  I2C1_SCL_0();
  Delay_72M_Us(20);     
}

//����λ
void I2C1_Stop(void)
{       
  I2C1_SDA_OUT(); //SDA ����Ϊ���
  I2C1_SCL_0();		//1
  Delay_72M_Us(20);	// 2
  I2C1_SDA_0();		// 3. 1,2,3�����в���ȱ��
  Delay_72M_Us(20);
  I2C1_SCL_1();
  Delay_72M_Us(20);
  I2C1_SDA_1();
  Delay_72M_Us(20);
}

void I2C1_Ack(void)
{      
  I2C1_SDA_OUT();//�ı䷽��// 
  I2C1_SCL_0();
  Delay_72M_Us(20);
  I2C1_SDA_0();
  Delay_72M_Us(20);
  I2C1_SCL_1();
  Delay_72M_Us(20);
  I2C1_SCL_0();
  Delay_72M_Us(20);
}

void I2C1_NoAck(void)
{      
  I2C1_SDA_OUT();//�ı䷽��// 
  I2C1_SCL_0();
  Delay_72M_Us(20);
  I2C1_SDA_1();
  Delay_72M_Us(20);
  I2C1_SCL_1();
  Delay_72M_Us(20);
  I2C1_SCL_0();
  Delay_72M_Us(20);
}


unsigned char I2C1_WaitAck(void)
{ 	   
  unsigned char ucErrTime=0; //����ʱ��
  
  I2C1_SDA_IN();   //�ı䷽��///   
  I2C1_SDA_1();
  Delay_72M_Us(20);	   
  I2C1_SCL_1();
  Delay_72M_Us(20);	 
  
  while(I2C1_READ_SDA())
  {
    ucErrTime++;
    if(ucErrTime>5000)
    {
      I2C1_Stop();
      return 1; //û��Ӧ��
    }
  }
  
  I2C1_SCL_0();//ʱ�����0 	   
  return 0;  //��������0 


 // I2C1_WaitAck
  
}


/*
unsigned char I2C1_WaitAck(void)
{      
    IIC_SDA_IN_INT();      
    if(GT316L_SDA_READ())   
    {        
        IIC_SCL_L();     
        Voice_Delay_Us(IIC_bDELAY_CNT);      
        return IIC_NAK;   
    }           
    IIC_SCL_L();    
    Voice_Delay_Us(IIC_bWRSP_CNT);   
    return IIC_ACK; 
}
*/


void I2C1_Write_Byte(unsigned char byte)
{      	
  unsigned char i = 0;
  
  I2C1_SDA_OUT(); //�ı䷽�� 
  
  for(i = 0; i < 8; i++)
  {
    I2C1_SCL_0();
    Delay_72M_Us(20);
    if(byte & 0x80)
    {
      I2C1_SDA_1();
    }
    else
    {
      I2C1_SDA_0();
    }
    Delay_72M_Us(20);
    I2C1_SCL_1();
    Delay_72M_Us(20);
    byte<<=1;
  }
  I2C1_SCL_0();
  Delay_72M_Us(20);
}


//��1���ֽ����� 
unsigned char I2C1_Read_Byte(void)
{
  unsigned char i,ReadByte;
  I2C1_SDA_OUT();       
  I2C1_SDA_1();
  I2C1_SDA_IN();//�ı䷽��//
  
  
  for(i = 0; i < 8; i++)
  {	
    ReadByte <<= 1;
    I2C1_SCL_0();
    Delay_72M_Us(20);
    I2C1_SCL_1();
    Delay_72M_Us(20);
    
    if(I2C1_READ_SDA())
    {
      ReadByte |= 0x01;
    }
    else
    {
      ReadByte &= ~(0x01);	
    }
    
  }
  /*
  if (!ack)
    I2C1_NoAck();//����nACK
  else
    I2C1_Ack(); //����ACK   
  I2C1_SDA_OUT();//�ı䷽��//
  */
  
  return ReadByte;
}


void AT24C64_Struct_Value_Init(void)
{
 // AT24C64.CheckExist = AT24C64_CheckExist;
  AT24C64.PowerOn = AT24C64_Power_On;
  //AT24C64.PowerOff = AT24C64_Power_Off;
  //AT24C64 = AT24C64_Multi_Write;
}



// //�رյ�Դ
// // void AT24C64_Power_Off(void)
// // {
// //   //AT24C64 IIC1 �ӿ�
// //   GPIO_Init(GPIOC, GPIO_Pin_1 | GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow);
// //   GPIO_SetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_0);
// // }

// //���Դ洢���Ƿ����
// // unsigned char AT24C64_CheckExist(void)
// // {

// //   unsigned char ucReturn = FALSE;
// //   unsigned char ucTemp1 = 0;
// //   unsigned char ucTemp2 = 0;

// //   AT24C64.PowerOn();

// //   AT24C64_Write_Byte(0x1FFE, 0xCC);
// //   AT24C64_Write_Byte(0x1FFF, 0x55);

// //   ucTemp1 = AT24C64_Read_Byte(0x1FFE);
// //   ucTemp2 = AT24C64_Read_Byte(0x1FFF);

// //   USART1_Send_String("����1:");
// //   USART1_Send_Hex(ucTemp1);
// //   USART1_Send_String_New_Line();

// //   USART1_Send_String("����2:");
// //   USART1_Send_Hex(ucTemp2);
// //   USART1_Send_String_New_Line();
  
// //   if (ucTemp1 == 0xCC && ucTemp2 == 0x55)
// //   {


// //     AT24C64_Write_Byte(0x1FFE, 0xAA);
// //     AT24C64_Write_Byte(0x1FFF, 0xBB);

// //     ucTemp1 = AT24C64_Read_Byte(0x1FFE);
// //     ucTemp2 = AT24C64_Read_Byte(0x1FFF);

// //     if (ucTemp1 == 0xAA && ucTemp2 == 0xBB)
// //     {
// //       //USART1_Debug_Message("�洢��:[�ɹ�]\r\n");
// //       ucReturn = TRUE;
// //     }
// //     else
// //     {
// //       //USART1_Debug_Message("�洢��:[ʧ��]\r\n");
// //     }
// //   }
// //   else
// //   {
// //     //USART1_Debug_Message("�洢��:[ʧ��]\r\n");
// //   }

// //   return ucReturn;
// // }

void AT24C64_Power_On(void)
{
 

   GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC,GPIO_Pin_8| GPIO_Pin_9);



}




// unsigned char AT24C64_Multi_Write(unsigned int uiStartAddress, unsigned int uiWriteData, unsigned int uiLength)
// {
//   unsigned int i = 0;
//   unsigned char H_ADDR = 0;
//   unsigned char L_ADDR = 0;

//   // AT24C02_Init();

//   USART1_Send_String("���@�Yne\r\n");

//   H_ADDR = uiStartAddress / 256;
//   L_ADDR = uiStartAddress % 256;

//   I2C1_Start();

//   I2C1_Write_Byte(AT24C64_WRITE_ADDRESS); //����������ַAT24C02_ADDRESS,д����
//   I2C1_WaitAck();

//   I2C1_Write_Byte(H_ADDR); //�ߵ�ַ дλ��
//   I2C1_WaitAck();

//   I2C1_Write_Byte(L_ADDR); //���͵͵�ַ //��ҳ�����ĵ�ַ��255 ����256ȡ����
//   I2C1_WaitAck();

//   for (i = 0; i < uiLength; i++)
//   {
//     I2C1_Write_Byte(uiWriteData); //�����ֽ�
//     I2C1_WaitAck();
//   }

//   I2C1_Stop(); //����һ��ֹͣ����
//   Delay_72M_Ms(1); //�ȴ�д�����

//   return 0;
// }






// //������ֽ�
// // unsigned char AT24C64_Multi_Read(unsigned int uiStartAddress, unsigned int *uiReadAddress, unsigned int uiLength)
// // {
// //   unsigned char temp = 0;
// //   unsigned char H_ADDR = 0;
// //   unsigned char L_ADDR = 0;
// //   unsigned int i = 0;

// //   //���ݵ�ַѡ��ҳ��
// //   H_ADDR = uiStartAddress / 256;
// //   L_ADDR = uiStartAddress % 256;

// //   //��ʼд��
// //   I2C1_Start();
// //   I2C1_Write_Byte(AT24C64_WRITE_ADDRESS); //д�豸��ַ
// //   I2C1_WaitAck();

// //   I2C1_Write_Byte(H_ADDR); //�ߵ�ַ дλ��
// //   I2C1_WaitAck();

// //   I2C1_Write_Byte(L_ADDR); //���͵͵�ַ //��ҳ�����ĵ�ַ��255 ����256ȡ����
// //   I2C1_WaitAck();

// //   I2C1_Start();
// //   I2C1_Write_Byte(AT24C64_READ_ADDRESS); //д����ַ
// //   I2C1_WaitAck();

// //   for (i = 0; i < uiLength; i++)
// //   {
// //     (*uiReadAddress) = I2C1_Read_Byte();
// //     I2C1_WaitAck();
// //   }

// //   I2C1_Stop(); //����һ��ֹͣ����
// // }

unsigned char AT24C64_Read_Byte(unsigned int uiReadAddr)
{
  unsigned char temp = 0;
  unsigned char H_ADDR = 0;
  unsigned char L_ADDR = 0;

  //  AT24C02_Init();

  //���ݵ�ַѡ��ҳ��
  // ucDeviceAddr = AT24CXX_Select_Page_Address(uiReadAddr);
  H_ADDR = uiReadAddr / 256;
  L_ADDR = uiReadAddr % 256;

  //��ʼд��
  I2C1_Start();
  I2C1_Write_Byte(AT24C64_WRITE_ADDRESS); //д�豸��ַ
  I2C1_WaitAck();

  I2C1_Write_Byte(H_ADDR); //�ߵ�ַ дλ��
  I2C1_WaitAck();

  I2C1_Write_Byte(L_ADDR); //���͵͵�ַ //��ҳ�����ĵ�ַ��255 ����256ȡ����
  I2C1_WaitAck();

  I2C1_Start();
  I2C1_Write_Byte(AT24C64_READ_ADDRESS); //д����ַ
  I2C1_WaitAck();
  temp = I2C1_Read_Byte();
  I2C1_Stop(); //����һ��ֹͣ����

  return temp;
}

void AT24C64_Write_Byte(unsigned int uiWriteAddr, unsigned char ucWriteData)
{
  unsigned char H_ADDR = 0;
  unsigned char L_ADDR = 0;

  // AT24C02_Init();

  H_ADDR = uiWriteAddr / 256;
  L_ADDR = uiWriteAddr % 256;

  I2C1_Start();

  I2C1_Write_Byte(AT24C64_WRITE_ADDRESS); //����������ַAT24C02_ADDRESS,д����
  I2C1_WaitAck();

  I2C1_Write_Byte(H_ADDR); //�ߵ�ַ дλ��
  I2C1_WaitAck();

  I2C1_Write_Byte(L_ADDR); //���͵͵�ַ //��ҳ�����ĵ�ַ��255 ����256ȡ����
  I2C1_WaitAck();

  I2C1_Write_Byte(ucWriteData); //�����ֽ�
  I2C1_WaitAck();

  I2C1_Stop(); //����һ��ֹͣ����
  Delay_72M_Ms(10); //�ȴ�д�����

  //USART1_Send_String("0xee");
}

// //���ȫ������
// void AT24C64_Wirte_All_Byte(unsigned char ucWriteByte)
// {
//   unsigned int i;

//   // AT24C64.MultiWrite(0, 0x33, 0x1FFF);

//   //һ��1FFF
//   for (i = 0; i < 0x1FFF; i++)
//   {
//     AT24C64_Write_Byte(i, ucWriteByte);
//   }
// }



