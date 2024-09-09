#ifndef _led_H
#define _led_H

#include "stm32f10x.h"

/*  LED时钟端口、引脚定义 */
#define GLED_PORT 			GPIOG   
#define GLED_PIN 			(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9)
#define GLED_PORT_RCC		RCC_APB2Periph_GPIOG
#define ALED_PORT 			GPIOA   
#define ALED_PIN 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9)
#define ALED_PORT_RCC	  RCC_APB2Periph_GPIOA
#define BLED_PORT 			GPIOB   
#define BLED_PIN 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9)
#define BLED_PORT_RCC		RCC_APB2Periph_GPIOB	
#define FLED_PORT 			GPIOF   
#define FLED_PIN 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9)
#define FLED_PORT_RCC		RCC_APB2Periph_GPIOF	
void GLED_Init(void);
void ALED_Init(void);
void BLED_Init(void);
void FLED_Init(void);
#endif
