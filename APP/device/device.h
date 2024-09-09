#ifndef _device_H
#define _device_H

#include "SysTick.h"
#include "system.h"
#include "stm32f10x_gpio.h"

extern uint8_t settemp[2][5],sethumi[2][5];

void heat(uint8_t temp,uint8_t My_mode);
void damp(uint8_t humi,uint8_t My_mode);
void opendoor (void);
	
#endif
