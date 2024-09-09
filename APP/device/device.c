#include "device.h"
void heat(uint8_t temp,uint8_t My_mode)
{
	if(temp>=settemp[0][My_mode])
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
		GPIO_SetBits(GPIOF,GPIO_Pin_2);
	}			//turn the warm-up part off
	else if (temp<=settemp[1][My_mode])
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_2);
			GPIO_SetBits(GPIOF,GPIO_Pin_1);
	}//turn the warm-up part on
	else
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	}
}

void damp(uint8_t humi,uint8_t My_mode)
{
	if(humi>=sethumi[0][My_mode])
	 {
		 GPIO_ResetBits(GPIOF,GPIO_Pin_3);
	 }
		else if(humi<=sethumi[1][My_mode])
	 {
		 GPIO_SetBits(GPIOF,GPIO_Pin_3);
	 }
	 else
	 {
		 GPIO_ResetBits(GPIOF,GPIO_Pin_3);
	 }
}

void opendoor (void)
{
	GPIO_SetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);
	delay_ms(10);
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	delay_ms(50000);	
}
