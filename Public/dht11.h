#include "stm32f10x.h"


#define HIGH  1
#define LOW   0

//这样宏定义方便用户使用其他GPIO作为传感器检测引脚用时修改程序
#define DHT11_CLK     RCC_APB2Periph_GPIOF
#define DHT11_PIN     GPIO_Pin_4                  
#define DHT11_PORT		GPIOF 

//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_4)
//读取引脚的电平
#define  DHT11_DATA_IN()	 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)

typedef struct
{
	uint8_t  humi_int;		  //湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	  //温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

void DHT11_GPIO_Config(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

