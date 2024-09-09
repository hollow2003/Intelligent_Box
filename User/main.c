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
//定义温湿度变量用于设备控制
float temp=100;
float humi=100;
//设置温度上下限（第一行为上限，第二行为下限），对应五种模式
uint8_t settemp[2][5]={{24,2,49,8,81},{21,2,41,0,79}};
//设置湿度上下限（第一行为上限，第二行为下限），对应五种模式
uint8_t sethumi[2][5]={{59,84,59,59,61},{41,74,39,39,59}};
//设置箱是否为空判断标志
uint8_t empty_flag=1;
//按键开门信号接收变量
uint8_t open_flag=0;
//蓝牙开门信号接受变量
uint8_t blue=0;
//储存实时时间控制消毒时间
int count=300;
//wifi接收缓存区
char WiFi_RX_BUF[1024];
//wifi接收标志位
int WiFi_RxCounter=0;
//设置模式变量
u8 My_mode=0;
//Wifi连接成功标志位
u8 connection=0;
//server启动次数标志位
u8 retrytimes=0;
//中断计数器
u8 times = 0;
u8 reconnecttimes = 0;

int main()
{
//初始化TFTLCD2.8寸屏
	LCD_Init();	
  sw_delay_ms(50);
  //清屏为黑色背景
  LCD_Clear(0, 0, 240, 320, BLACK);	
  //指定位置显示温度指示信息
	LCD_DispStr(15, 10, (uint8_t *)"real-time temperature/Celsius:", WHITE);	
  //指定位置显示湿度指示信息
	LCD_DispStr(15, 100, (uint8_t *)"real-time humidity/Saturation:", WHITE);	
	//指定位置显示模式指示信息
	LCD_DispStr(15, 190, (uint8_t *)"real-time mode:", WHITE);	
	LCD_DispStr(15, 80, (uint8_t *)"real-time wifi state:", WHITE);	
	//初始化RS485
	RS485_Init();	
	RS485RX; 
	//使能中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//使能指示灯
	GLED_Init();
	ALED_Init();
	BLED_Init();
	FLED_Init();
	//使能系统时钟
	SysTick_Init(72);
	//使能按键
	KEY_Init();
	//定义中断函数
	My_EXTI_Init(); 
	//使能USART1串口
	USART1_Init(19200);
	//使能SH30
  IIC_GPIO_Init();  
	SHT3X_Init();
	//使能ESP8266
	TIM3_Init();
	esp8266_init();	
	USART3_Init();
	//使能USART2串口	
  USART2_Init();	
	//开启初始指示灯
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GLED_PORT,GPIO_Pin_7);
	//初始化RTC实时时钟
	RTC_Init(2020, 12, 31, 23, 59, 50); 

	while(retrytimes<=2&&connection==0)
		{
			WiFi_InitServer();
			//计数
			retrytimes++;
			//循环，初始化，建立服务器，直到成功
			delay_ms(200);              //延时
	  }

	while(1)
	{
		//显示当前模式
		LCD_DisNum(80, 220, My_mode, WHITE);
		//设置初始指示灯
		GPIO_ResetBits(GPIOF,GPIO_Pin_0);
		//根据当前模式更改指示灯状态
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
		//读取温湿度信息
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
		//控制温度湿度模块
    heat(temp,My_mode);
		damp(humi,My_mode);
		//检测按键开门信号（情况：箱空）
		if(empty_flag==1&&open_flag==1)
		{
			//开门
			opendoor();
			//清空箱空指示及按键开门指示
			empty_flag=0;
			open_flag=0;
		}
		//检测按键开门信号（情况：箱不空）
		if(empty_flag==0&&open_flag==1)
		{
			open_flag=0;
		}
		//检测蓝牙开门信号
		if(blue==1)
		{
			//蓝牙开门信号置零
			blue=0;
			//开门
			opendoor();
			delay_ms(10000);
			//读取记录实时时钟用于控制消毒时长
			RTC_ReadClock();
			//change 10 to change the duration time(min)
			count=(g_tRTC.Min+10)%60;
			//消毒
			LCD_DisNum(80, 220,4, WHITE);
			while(count!=g_tRTC.Min)
			{
				//更新时钟信息
				RTC_ReadClock();
				//更新温湿度信息
				SHT3X_TEST();
				heat(temp,4);
				damp(humi,4);
				delay_ms(2500);
				//中途检测蓝牙开门信号（防止未取完）
				if(blue==1)
				{
					//无需置零蓝牙开门信号，下一次循环开门后进入消毒
					break;
				}
			}
		  }
		//延时防止DHT11工作过于频繁
		;
	}
}	
