#include "usart.h"		 
#include "system.h"
#include <stdlib.h>
#include "esp8266.h"
#include "SysTick.h"
/*******************************************************************************
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}
void USART2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

  //������GPIO������2��ʱ��
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE); 
  
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
	USART_InitStructure.USART_BaudRate = 9600;                      //���ô���2������Ϊ9600
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
void USART2_SendByte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
	USART_SendData(USART2, byte);
}
void USART2_SendStr(uint8_t *str)
{
  while(0!=*str)              //�жϷ�����Ч�ַ�
	{
    USART2_SendByte(*str);    //�����ַ�
    str++;
  }
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
    u8 r =USART_ReceiveData(USART2);//(USART1->DR);	//���յ�����,���ձ�ʶ����Ч
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
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);    //���USART2���жϴ�����λ
}
/*******************************************************************************
* �� �� ��         : USART1_IRQHandler
* ��������		   : USART1�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{ 
		u8 r =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //���յ�����
	{	 
		temp =USART_ReceiveData(USART3);
		WiFi_RX_BUF[WiFi_RxCounter]=temp;
		WiFi_RxCounter++;
		if(WiFi_RxCounter>=1024)
		{
			printf("wifi receive error");
			WiFi_RxCounter=0;                           //WiFi������������������                        
			memset(WiFi_RX_BUF,0,1024);      
		}			//����д�뵽������
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
				WiFi_RxCounter=0; 				//WiFi������������������                        
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
/*������������2���ͻ������е�����                  */
/*��  ����data������                               */
/*����ֵ����                                       */
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

  //������GPIO
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE); 
  
  //���õ�IO��PB10������3��TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  //���õ�IO��PB11������3��RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
     
  //���õ�IO��PE0��ESP8266��λ����RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           //�������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  //��ʼ״̬��ESP8266��λ�����ø�
  GPIO_SetBits(GPIOE,GPIO_Pin_0);
}

/**************************************************************************************
 * ��  �� : ��ʼ��USART3������USART3�ж����ȼ�
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART3_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

  //�򿪴���3��ʱ��
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3 , ENABLE); 
	
  //���ô���3��Ӳ������
	USART_DeInit(USART3);                                            //������USART3�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = 115200;                       //���ô���3������Ϊ9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;      //����һ��֡�д�������λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;           //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;              //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //���ͽ���ʹ��
	USART_Init(USART3, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;      //ʱ�ӵ͵�ƽ�
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;            //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;          //ʱ�ӵ�һ�����ؽ������ݲ���
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //���һλ���ݵ�ʱ�����岻��SCLK���
  USART_ClockInit(USART3, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                   //ʹ��USART3�����ж�
	USART_Cmd(USART3, ENABLE);                                       //ʹ��USART3����
	
  //���ô���3�ж����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                  //NVIC_Group:��ռ���ȼ�2λ�������ȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	               //����Ϊ����3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;          //��ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	             //�����ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //ʹ���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

