

#define LIGHT_EYE_1_SEND_PORT GPIOC
#define LIGHT_EYE_1_SEND_PIN GPIO_Pin_8
#define LIGHT_EYE_1_REC_PORT GPIOA //PA6 ADC6
#define LIGHT_EYE_1_REC_PIN GPIO_Pin_7

#define LIGHT_EYE_2_SEND_PORT GPIOA
#define LIGHT_EYE_2_SEND_PIN GPIO_Pin_15
#define LIGHT_EYE_2_REC_PORT GPIOC //PC4 ADC14
#define LIGHT_EYE_2_REC_PIN GPIO_Pin_4



#define ADC1_DR_Address ((u32)0x40012400+0x4c)

// #define ADC1_6 22PIN GOIO_PIN_6


// #deinf PC4 ADC1_14

// LIGHT_EYE_1_REC_PIN


unsigned int Get_ADC_1_DATA(void);
unsigned int Get_ADC_2_DATA(void);


void Light_Adc_Dma_Init(void);

void Light_Eye_Pins_Init(void);



void sub_Check_Light_Eye_1(void);
void sub_Check_Light_Eye_2(void);
void Check_Light_Eye_Status(void);

