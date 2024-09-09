#ifndef __WIFI_H
#define __WIFI_H
#include "stm32f10x.h"
#include "usart.h"	    //包含需要的头文件
#include "system.h"
#define WiFi_printf       u3_printf           //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //串口2控制 WiFi
#define RESET_IO()    GPIO_ResetBits(GPIOE, GPIO_Pin_0)  //PA4控制WiFi的复位
#define SET_IO()    GPIO_SetBits(GPIOE, GPIO_Pin_0)  //PA4控制WiFi的复位
#define SSID   "dd"                     //路由器SSID名称
#define PASS   "23333333"                 //路由器密码
extern u8 connection;
extern char WiFi_RX_BUF[1024]; //定义串口2 发送缓冲区大小 1024字节
extern int WiFi_RxCounter;
void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_GetIP(uint16_t timeout);
char WiFi_Get_LinkSta(void);
char WiFi_Get_Data(char *data, char *len, char *id);
char WiFi_SendData(char id, char *databuff, int data_len, int timeout);
char WiFi_InitServer(void);
int	Char_to_Hex(char *char_data, int len);
void clearwififlash(void);
#endif
