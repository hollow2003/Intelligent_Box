#ifndef __usart_H
#define __usart_H
#include "stdio.h"    
#include "stdarg.h"		
#include "string.h" 
#include "system.h" 
extern u8 My_mode;
extern u8 open_flag;
extern u8 empty_flag;
extern u8 blue;
extern char WiFi_RX_BUF[1024]; //定义串口3 发送缓冲区大小 1024字节
extern int WiFi_RxCounter; //定义串口3 发送缓冲区标志位
void USART1_Init(u32 bound);
void USART2_Init(void);
void USART2_SendByte(uint8_t byte);
void USART2_SendStr(uint8_t *str);
void u3_printf(char*,...) ;         //串口1 printf函数
void USART3_Init(void);
void esp8266_init(void);
#define USART1_TXBUFF_SIZE   256    //定义串口1 发送缓冲区大小 256字节
#define USART3_TXBUFF_SIZE   1024
#define USART3_RXBUFF_SIZE   1024 
#endif


