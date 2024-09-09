#ifndef __WIFI_H
#define __WIFI_H
#include "stm32f10x.h"
#include "usart.h"	    //������Ҫ��ͷ�ļ�
#include "system.h"
#define WiFi_printf       u3_printf           //����2���� WiFi
#define WiFi_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //����2���� WiFi
#define RESET_IO()    GPIO_ResetBits(GPIOE, GPIO_Pin_0)  //PA4����WiFi�ĸ�λ
#define SET_IO()    GPIO_SetBits(GPIOE, GPIO_Pin_0)  //PA4����WiFi�ĸ�λ
#define SSID   "dd"                     //·����SSID����
#define PASS   "23333333"                 //·��������
extern u8 connection;
extern char WiFi_RX_BUF[1024]; //���崮��2 ���ͻ�������С 1024�ֽ�
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
