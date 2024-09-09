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
extern char WiFi_RX_BUF[1024]; //���崮��3 ���ͻ�������С 1024�ֽ�
extern int WiFi_RxCounter; //���崮��3 ���ͻ�������־λ
void USART1_Init(u32 bound);
void USART2_Init(void);
void USART2_SendByte(uint8_t byte);
void USART2_SendStr(uint8_t *str);
void u3_printf(char*,...) ;         //����1 printf����
void USART3_Init(void);
void esp8266_init(void);
#define USART1_TXBUFF_SIZE   256    //���崮��1 ���ͻ�������С 256�ֽ�
#define USART3_TXBUFF_SIZE   1024
#define USART3_RXBUFF_SIZE   1024 
#endif


