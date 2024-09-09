#include "stm32f10x.h"


#define HIGH  1
#define LOW   0

//�����궨�巽���û�ʹ������GPIO��Ϊ���������������ʱ�޸ĳ���
#define DHT11_CLK     RCC_APB2Periph_GPIOF
#define DHT11_PIN     GPIO_Pin_4                  
#define DHT11_PORT		GPIOF 

//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_4)
//��ȡ���ŵĵ�ƽ
#define  DHT11_DATA_IN()	 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)

typedef struct
{
	uint8_t  humi_int;		  //ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	  //�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
		                 
}DHT11_Data_TypeDef;

void DHT11_GPIO_Config(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

