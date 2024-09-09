#include "stm32f10x.h"
#include <stdio.h>


#define  RS485RX       GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define  RS485TX       GPIO_SetBits(GPIOD, GPIO_Pin_7)
#define	FALSE					0
#define	TRUE					1

void RS485_Init(void);



