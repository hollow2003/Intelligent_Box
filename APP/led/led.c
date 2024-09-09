#include "led.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void FLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(FLED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=FLED_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(FLED_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_SetBits(GLED_PORT,GLED_PIN);   //��LED�˿����ߣ�Ϩ������LED
}
void GLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(GLED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GLED_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GLED_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_SetBits(GLED_PORT,GLED_PIN);   //��LED�˿����ߣ�Ϩ������LED
}
void ALED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(ALED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=ALED_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(ALED_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_SetBits(ALED_PORT,ALED_PIN);   //��LED�˿����ߣ�Ϩ������LED
}
void BLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
	
	RCC_APB2PeriphClockCmd(BLED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=BLED_PIN;  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(BLED_PORT,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
	GPIO_SetBits(BLED_PORT,BLED_PIN);   //��LED�˿����ߣ�Ϩ������LED
}


