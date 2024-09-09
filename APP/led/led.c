#include "led.h"

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void FLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(FLED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=FLED_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(FLED_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_SetBits(GLED_PORT,GLED_PIN);   //将LED端口拉高，熄灭所有LED
}
void GLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(GLED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GLED_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GLED_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_SetBits(GLED_PORT,GLED_PIN);   //将LED端口拉高，熄灭所有LED
}
void ALED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(ALED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=ALED_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(ALED_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_SetBits(ALED_PORT,ALED_PIN);   //将LED端口拉高，熄灭所有LED
}
void BLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(BLED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=BLED_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(BLED_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_SetBits(BLED_PORT,BLED_PIN);   //将LED端口拉高，熄灭所有LED
}


