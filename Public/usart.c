#include "usart.h"		 
#include "system.h"
#include <stdlib.h>
#include "esp8266.h"
#include "SysTick.h"
/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}
void USART2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

  //打开所用GPIO及串口2的时钟
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE); 
  
  //配置的IO是PA2，串口2的TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //配置的IO是PA3，串口2的RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  //配置串口2的硬件参数
	USART_DeInit(USART2);                                            //将外设USART2寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = 9600;                      //设置串口2波特率为9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;      //设置一个帧中传输数据位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;           //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;              //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //发送接收使能
	USART_Init(USART2, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;      //时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;            //设置SCLK引脚上时钟输出的极性为低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;          //时钟第一个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //最后一位数据的时钟脉冲不从SCLK输出
  USART_ClockInit(USART2, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                   //使能USART2接收中断
	USART_Cmd(USART2, ENABLE);                                       //使能USART2外设
	
  //配置串口2中断优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                  //NVIC_Group:先占优先级2位，从优先级2位
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	               //配置为串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;          //先占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	             //从优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //使能中断通道
	NVIC_Init(&NVIC_InitStructure);
}
void USART2_SendByte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
	USART_SendData(USART2, byte);
}
void USART2_SendStr(uint8_t *str)
{
  while(0!=*str)              //判断发送有效字符
	{
    USART2_SendByte(*str);    //发送字符
    str++;
  }
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
    u8 r =USART_ReceiveData(USART2);//(USART1->DR);	//接收到数据,接收标识符有效
	  switch(r)
		{
			case 48:My_mode=0;
			       break;//mode 1
			case 49:My_mode=1;
				     break;//mode 2
			case 50:My_mode=2;
				     break;//mode 3
			case 51:My_mode=3;
				     break;//mode 4
			case 52:blue=1;
			       empty_flag=1;
			       break;
		}
	}
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);    //清除USART2的中断待处理位
}
/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{ 
		u8 r =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		switch(r)
		{
			case 0:My_mode=0;
			       break;//mode 1
			case 1:My_mode=1;
				     break;//mode 2
			case 2:My_mode=2;
				     break;//mode 3
			case 3:My_mode=3;
				     break;//mode 4
			case 4:blue=1;
			       empty_flag=1;
			       break;
		}
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
		USART_SendData(USART1,r);
	}
	USART_ClearFlag(USART1,USART_FLAG_TC);
} 	

 void USART3_IRQHandler(void)
{
	char temp;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //接收到数据
	{	 
		temp =USART_ReceiveData(USART3);
		WiFi_RX_BUF[WiFi_RxCounter]=temp;
		WiFi_RxCounter++;
		if(WiFi_RxCounter>=1024)
		{
			printf("wifi receive error");
			WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,1024);      
		}			//数据写入到缓冲区
		if(WiFi_RxCounter>=204)
		{
			int i;
			char *request=strstr(WiFi_RX_BUF,"request_id");
			char *requestmode=strstr(WiFi_RX_BUF,"test\"");
			char *end=strstr(WiFi_RX_BUF,"},\"ser");
			printf("Receive request successfully\n");
			if(request!=NULL&&requestmode!=NULL&&end!=NULL)
			{
				char mode= *(requestmode+6);
				char callback[40];
				char msg1[125];
				char request_id[40];
				char msg2[125];
				//printf("receive mode %s successfully\n",requestmode+6);
				printf("%s",request);
				for(i=0;i<=35;i++)
				{
					callback[i]=*(request+11+i);
				}
				printf("callback: %c           ",callback[35]);
				strcpy(request_id,callback);
				printf("request_id:%s     ",request_id);
				sprintf(msg1,"AT+MQTTSUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/commands/response/request_id=%36.36s\",1",callback);
				printf("%s",msg1);
				WiFi_SendCmd(msg1,1);
				delay_ms(50);
				sprintf(msg2,"AT+MQTTPUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/commands/response/request_id=%36.36s\",\"\",0,0",request_id);
				printf("%s",msg2);
				WiFi_SendCmd(msg2,2);
				My_mode=atoi(&mode);
				WiFi_RxCounter=0; 				//WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,1024); 
			}
		}
	}
	
}   

__align(8) char USART3_TxBuff[USART3_TXBUFF_SIZE];  
 
void u3_printf(char* fmt,...) 
{  
	unsigned int i,length;
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART3_TxBuff,fmt,ap);
	va_end(ap);	
	length=strlen((const char*)USART3_TxBuff);		
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = USART3_TxBuff[i];
		while((USART3->SR&0X40)==0);	
	}	
}
 
/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u3_TxData(unsigned char *data)
{
	int	i;	
	while((USART3->SR&0X40)==0);
	for(i = 1;i <= data[0];i ++){			
		USART3->DR = data[i];
		while((USART3->SR&0X40)==0);	
	}
}
void esp8266_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  //打开所用GPIO
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE); 
  
  //配置的IO是PB10，串口3的TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  //配置的IO是PB11，串口3的RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
     
  //配置的IO是PE0，ESP8266复位引脚RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           //推挽输出
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  //初始状态将ESP8266复位引脚置高
  GPIO_SetBits(GPIOE,GPIO_Pin_0);
}

/**************************************************************************************
 * 描  述 : 初始化USART3并配置USART3中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART3_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

  //打开串口3的时钟
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3 , ENABLE); 
	
  //配置串口3的硬件参数
	USART_DeInit(USART3);                                            //将外设USART3寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = 115200;                       //设置串口3波特率为9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;      //设置一个帧中传输数据位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;           //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;              //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //发送接收使能
	USART_Init(USART3, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;      //时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;            //设置SCLK引脚上时钟输出的极性为低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;          //时钟第一个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //最后一位数据的时钟脉冲不从SCLK输出
  USART_ClockInit(USART3, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                   //使能USART3接收中断
	USART_Cmd(USART3, ENABLE);                                       //使能USART3外设
	
  //配置串口3中断优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                  //NVIC_Group:先占优先级2位，从优先级2位
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	               //配置为串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;          //先占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	             //从优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //使能中断通道
	NVIC_Init(&NVIC_InitStructure);
}

