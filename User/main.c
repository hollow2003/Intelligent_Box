#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include <stdio.h>
#include "user_usart.h"
#include "stm32f10x.h"
#include "sht30.h"
#include "user_ili9341_lcd.h"
#include "device.h"
#include "user_rtc.h"
#include "esp8266.h"
#include "user_timer.h"
#include "user_iic.h"	
#include "delay.h"
uint32_t i;
//������ʪ�ȱ��������豸����
float temp=100;
float humi=100;
//�����¶������ޣ���һ��Ϊ���ޣ��ڶ���Ϊ���ޣ�����Ӧ����ģʽ
uint8_t settemp[2][5]={{24,2,49,8,81},{21,2,41,0,79}};
//����ʪ�������ޣ���һ��Ϊ���ޣ��ڶ���Ϊ���ޣ�����Ӧ����ģʽ
uint8_t sethumi[2][5]={{59,84,59,59,61},{41,74,39,39,59}};
//�������Ƿ�Ϊ���жϱ�־
uint8_t empty_flag=1;
//���������źŽ��ձ���
uint8_t open_flag=0;
//���������źŽ��ܱ���
uint8_t blue=0;
//����ʵʱʱ���������ʱ��
int count=300;
//wifi���ջ�����
char WiFi_RX_BUF[1024];
//wifi���ձ�־λ
int WiFi_RxCounter=0;
//����ģʽ����
u8 My_mode=0;
//Wifi���ӳɹ���־λ
u8 connection=0;
//server����������־λ
u8 retrytimes=0;
//�жϼ�����
u8 times = 0;
u8 reconnecttimes = 0;

int main()
{
//��ʼ��TFTLCD2.8����
	LCD_Init();	
  sw_delay_ms(50);
  //����Ϊ��ɫ����
  LCD_Clear(0, 0, 240, 320, BLACK);	
  //ָ��λ����ʾ�¶�ָʾ��Ϣ
	LCD_DispStr(15, 10, (uint8_t *)"real-time temperature/Celsius:", WHITE);	
  //ָ��λ����ʾʪ��ָʾ��Ϣ
	LCD_DispStr(15, 100, (uint8_t *)"real-time humidity/Saturation:", WHITE);	
	//ָ��λ����ʾģʽָʾ��Ϣ
	LCD_DispStr(15, 190, (uint8_t *)"real-time mode:", WHITE);	
	LCD_DispStr(15, 80, (uint8_t *)"real-time wifi state:", WHITE);	
	//��ʼ��RS485
	RS485_Init();	
	RS485RX; 
	//ʹ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//ʹ��ָʾ��
	GLED_Init();
	ALED_Init();
	BLED_Init();
	FLED_Init();
	//ʹ��ϵͳʱ��
	SysTick_Init(72);
	//ʹ�ܰ���
	KEY_Init();
	//�����жϺ���
	My_EXTI_Init(); 
	//ʹ��USART1����
	USART1_Init(19200);
	//ʹ��SH30
  IIC_GPIO_Init();  
	SHT3X_Init();
	//ʹ��ESP8266
	TIM3_Init();
	esp8266_init();	
	USART3_Init();
	//ʹ��USART2����	
  USART2_Init();	
	//������ʼָʾ��
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GLED_PORT,GPIO_Pin_7);
	//��ʼ��RTCʵʱʱ��
	RTC_Init(2020, 12, 31, 23, 59, 50); 

	while(retrytimes<=2&&connection==0)
		{
			WiFi_InitServer();
			//����
			retrytimes++;
			//ѭ������ʼ����������������ֱ���ɹ�
			delay_ms(200);              //��ʱ
	  }

	while(1)
	{
		//��ʾ��ǰģʽ
		LCD_DisNum(80, 220, My_mode, WHITE);
		//���ó�ʼָʾ��
		GPIO_ResetBits(GPIOF,GPIO_Pin_0);
		//���ݵ�ǰģʽ����ָʾ��״̬
		switch(My_mode)
		{
			case 0:GPIO_ResetBits(GLED_PORT,GPIO_Pin_6);//light1 on
			       GPIO_SetBits(ALED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(BLED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(BLED_PORT,GPIO_Pin_9);
			       break;//mode 1
			case 1:GPIO_ResetBits(ALED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(GLED_PORT,GPIO_Pin_6);
			       GPIO_SetBits(BLED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(BLED_PORT,GPIO_Pin_9);
				     break;//mode 2
			case 2:GPIO_ResetBits(BLED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(GLED_PORT,GPIO_Pin_6);
			       GPIO_SetBits(ALED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(BLED_PORT,GPIO_Pin_9);
				     break;//mode 3
			case 3:GPIO_ResetBits(BLED_PORT,GPIO_Pin_9);
			       GPIO_SetBits(GLED_PORT,GPIO_Pin_6);
			       GPIO_SetBits(ALED_PORT,GPIO_Pin_8);
			       GPIO_SetBits(BLED_PORT,GPIO_Pin_8);
				     break;//mode 4
		}
		//��ȡ��ʪ����Ϣ
		if(times>=2)
		{
			WiFi_GetIP(25);
			SHT3X_TEST();
			times=0;
		}
		if(connection == 0 && reconnecttimes>=30)
		{
			WiFi_JoinAP(15);
		}
		//�����¶�ʪ��ģ��
    heat(temp,My_mode);
		damp(humi,My_mode);
		//��ⰴ�������źţ��������գ�
		if(empty_flag==1&&open_flag==1)
		{
			//����
			opendoor();
			//������ָʾ����������ָʾ
			empty_flag=0;
			open_flag=0;
		}
		//��ⰴ�������źţ�������䲻�գ�
		if(empty_flag==0&&open_flag==1)
		{
			open_flag=0;
		}
		//������������ź�
		if(blue==1)
		{
			//���������ź�����
			blue=0;
			//����
			opendoor();
			delay_ms(10000);
			//��ȡ��¼ʵʱʱ�����ڿ�������ʱ��
			RTC_ReadClock();
			//change 10 to change the duration time(min)
			count=(g_tRTC.Min+10)%60;
			//����
			LCD_DisNum(80, 220,4, WHITE);
			while(count!=g_tRTC.Min)
			{
				//����ʱ����Ϣ
				RTC_ReadClock();
				//������ʪ����Ϣ
				SHT3X_TEST();
				heat(temp,4);
				damp(humi,4);
				delay_ms(2500);
				//��;������������źţ���ֹδȡ�꣩
				if(blue==1)
				{
					//�����������������źţ���һ��ѭ�����ź��������
					break;
				}
			}
		  }
		//��ʱ��ֹDHT11��������Ƶ��
		;
	}
}	
