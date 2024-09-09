#include "stm32f10x.h"
#include "user_usart.h"
#include "SysTick.h"
/**************************************************************************************
 * 描  述 : 初始化RS485所用GPIO口及所用USART2配置
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void RS485_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

  //打开所用GPIO及串口2的时钟
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE); 
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE); 
  
  //配置的IO是PD7，RS485的使能控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
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
	USART_InitStructure.USART_BaudRate = 19200;                      //设置串口2波特率为19200
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

/**************************************************************************************
 * 描  述 : 重定向c库函数printf到USART2
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/


/**************************************************************************************
 * 描  述 : 重定向c库函数scanf到USART2
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART2 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/**************************************************************************************
 * 描  述 : 重定向c库函数scanf到USART2
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int fgetc(FILE *f)
{
		/* 等待串口2输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}




