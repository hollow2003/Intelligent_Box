#include "stm32f10x.h"
#include "user_usart.h"
#include "SysTick.h"
/**************************************************************************************
 * ��  �� : ��ʼ��RS485����GPIO�ڼ�����USART2����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void RS485_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

  //������GPIO������2��ʱ��
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE); 
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE); 
  
  //���õ�IO��PD7��RS485��ʹ�ܿ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //���õ�IO��PA2������2��TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //���õ�IO��PA3������2��RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  //���ô���2��Ӳ������
	USART_DeInit(USART2);                                            //������USART2�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = 19200;                      //���ô���2������Ϊ19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;      //����һ��֡�д�������λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;           //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;              //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //���ͽ���ʹ��
	USART_Init(USART2, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;      //ʱ�ӵ͵�ƽ�
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;            //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;          //ʱ�ӵ�һ�����ؽ������ݲ���
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //���һλ���ݵ�ʱ�����岻��SCLK���
  USART_ClockInit(USART2, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                   //ʹ��USART2�����ж�
	USART_Cmd(USART2, ENABLE);                                       //ʹ��USART2����
	
  //���ô���2�ж����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                  //NVIC_Group:��ռ���ȼ�2λ�������ȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	               //����Ϊ����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;          //��ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	             //�����ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //ʹ���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * ��  �� : �ض���c�⺯��printf��USART2
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/


/**************************************************************************************
 * ��  �� : �ض���c�⺯��scanf��USART2
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART2 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/**************************************************************************************
 * ��  �� : �ض���c�⺯��scanf��USART2
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int fgetc(FILE *f)
{
		/* �ȴ�����2�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}




