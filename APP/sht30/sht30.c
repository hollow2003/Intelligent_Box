#include "stm32f10x.h"
#include "sht30.h"
#include "user_usart.h"
#include "user_iic.h"	
#include <string.h>
#include "delay.h"

 float temperatureC;
 float humidityRH;

 uint8_t buffer[6];

 /**************************************************************************************
 * ��  �� : ��SHT30д�Ĵ�������
 * ��  �� : cmd: ���ֽ�����
 * ����ֵ :   1---�ɹ�д��,     0---ʧ��
 **************************************************************************************/
FunctionalState SHT3X_WriteCMD(uint16_t cmd)
{
	if(!IIC_Start())   return DISABLE;
	IIC_Send_Byte(i2cAddWrite_8bit);
	if(!IIC_Wait_Ack()){IIC_Stop(); return DISABLE;}
	IIC_Send_Byte(cmd>>8);
	IIC_Wait_Ack();
	IIC_Send_Byte(cmd);
	IIC_Wait_Ack();
	IIC_Stop();
	return ENABLE;
}

 /**************************************************************************************
 * ��  �� : ���ò�������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void SHT3X_SetPeriodicMeasurement(void)
{
    SHT3X_WriteCMD(CMD_MEAS_PERI_2_H);
}

 /**************************************************************************************
 * ��  �� : ��ʼ��SHT3X������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void SHT3X_Init(void)
{
    sw_delay_ms(100); 
    SHT3X_SetPeriodicMeasurement();
    sw_delay_ms(100); 
}

 /**************************************************************************************
 * ��  �� : ��SHT30��ȡ״̬��Ϣ
 * ��  �� : temp: ��������ַ
 * ����ֵ :   1---�ɹ���ȡ,     0---ʧ��
 **************************************************************************************/
FunctionalState SHT3X_ReadState(uint8_t *temp)
{
   if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(i2cAddWrite_8bit);
	if(!IIC_Wait_Ack())		{IIC_Stop(); return DISABLE;}
	IIC_Send_Byte(0xf3);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X2d);
	IIC_Wait_Ack();
   IIC_Stop(); 
	sw_delay_ms(100);
  if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(i2cAddRead_8bit);
	IIC_Wait_Ack();
	temp[0] = IIC_Read_Byte(0x00);
	IIC_Ack();
	temp[1] = IIC_Read_Byte(0x00);
	IIC_Ack();
	temp[2] = IIC_Read_Byte(0x00);
	IIC_NAck(); 
  IIC_Stop(); 
	return ENABLE;
}

 /**************************************************************************************
 * ��  �� : ��SHT30��ȡ�������
 * ��  �� : cmd: ����������    p����������ַ
 * ����ֵ :   1---�ɹ���ȡ,     0---ʧ��
 **************************************************************************************/
FunctionalState SHX3X_ReadResults(uint16_t cmd,  uint8_t *p)
{
	if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(0x88);
	if(!IIC_Wait_Ack())		{IIC_Stop(); return DISABLE;}
	IIC_Send_Byte(cmd>>8);
	IIC_Wait_Ack();
	IIC_Send_Byte(cmd);
	IIC_Wait_Ack();
	IIC_Stop();
	
   if(!IIC_Start())return DISABLE;
	IIC_Send_Byte(0x89);
	if(!IIC_Wait_Ack())		{IIC_Stop(); return DISABLE;}

	p[0] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[1] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[2] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[3] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[4] = IIC_Read_Byte(0x00);
	IIC_Ack();
	p[5] =IIC_Read_Byte(0x00);
	IIC_NAck();
	IIC_Stop();
	return ENABLE;
}

 /**************************************************************************************
 * ��  �� : SHT30У�麯��
 * ��  �� : data[]: �ȴ����������   startByte: ��ʼ����ı��
            number: �������         checksum: ���յ���CRC����
 * ����ֵ :   1---�ɹ����,     0---ʧ��
 **************************************************************************************/
FunctionalState SHT3x_CheckCrc(uint8_t data[],uint8_t startBytes,uint8_t number, uint8_t checksum)
{
   uint8_t bit=0;
	uint8_t crc = 0xFF;	
   uint8_t byteCtr;

  for (byteCtr = startBytes; byteCtr < startBytes+number; byteCtr++)
  { crc ^= (data[byteCtr]);   //������Ҫ������������ָ����һ����
    for (bit = 8; bit > 0; --bit)
    { if (crc & 0x80)    //�ж����λ�Ƿ�Ϊ1  
       crc = (crc << 1) ^ POLYNOMIAL;  //���λΪ1������һλ�����ʽ���
      else 
       crc = (crc << 1);      //���λΪ0��ֱ������һλ
    }
  }
  if (crc ==checksum) 
	return ENABLE;
  else
	return DISABLE;
}

 /**************************************************************************************
 * ��  �� : �����¶Ⱥ���
 * ��  �� : dat����ȡ���¶�ԭʼֵ
 * ����ֵ : ��������¶�ֵ
 **************************************************************************************/
float SHT3X_CalcTemperature(uint16_t dat)
{
  float temp;
  temp = (175 * (float)dat / 65535 - 45) ;    //��λ [��]
  return temp;
}

 /**************************************************************************************
 * ��  �� : ����ʪ�Ⱥ���
 * ��  �� : dat����ȡ��ʪ��ԭʼֵ
 * ����ֵ : �������ʪ��ֵ
 **************************************************************************************/
float SHT3X_CalcRH(uint16_t dat)
{
  float temp;
  temp = (100 * (float)dat / 65535) ;  //��λ [%RH]
  return temp;
}

 /**************************************************************************************
 * ��  �� : SHT30���Ժ���
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void SHT3X_TEST(void)
{
   uint8_t p[6],Temp1,Temp2;
	 float cTemp,rhTemp;
   char temp_msg[200];
	 char humi_msg[200];
	 char mode_msg[200];
	 char use_msg[200];
   //��������p
   memset(p,0,6);
  
	 //����ģʽ������ʪ��ԭʼֵ����ԭʼֵ�������p ����������Ӧ��Ϣ��ʽ���£�
   //�¶ȸ�8λ + �¶ȵ�8λ + �¶�У��8λ + ʪ�ȸ�8λ + ʪ�ȵ�8λ + ʪ��У��8λ 
   if(SHX3X_ReadResults(CMD_FETCH_DATA, buffer))
	 {
     p[0] = buffer[0];
     p[1] = buffer[1];
     p[2] = buffer[2];
	   p[3] = buffer[3];
     p[4] = buffer[4];
     p[5] = buffer[5];
	 }
	 
   //У�����������
   Temp1=SHT3x_CheckCrc(p,0,2, p[2]);
	 Temp2=SHT3x_CheckCrc(p,3,2, p[5]);
	 
   //����У���������ϴ���������Ϣ
	 if(Temp1&&Temp2)
	 {
     Temp1=0;
     Temp2=0;
     //�����¶�ԭʼ��Ϣ���������㷨�����ʵ���¶�ֵ
     cTemp = (p[0] * 256.0) + p[1];
	   temp =SHT3X_CalcTemperature(cTemp);
	 
     //����ʪ��ԭʼ��Ϣ���������㷨�����ʵ��ʪ��ֵ
     rhTemp = (p[3] * 256.0) + p[4];
	   humi = SHT3X_CalcRH(rhTemp);
		 
     //����1�ϴ���ʪ����Ϣ
     printf("�¶ȣ�%f\r\n",temp);
     printf("ʪ�ȣ�%f%%\r\n",humi);
		 LCD_DisNum(80, 60,(int)temp, WHITE);
		 LCD_DisNum(80, 160, (int)humi, WHITE);
		 if(connection==1&&retrytimes <= 2)
		{
			sprintf(temp_msg,"AT+MQTTPUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"Dev_data\\\"\\,\\\"properties\\\":{\\\"temp\\\": %d}}]}\",0,0\r\n",(int)temp);
			sprintf(humi_msg,"AT+MQTTPUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"Dev_data\\\"\\,\\\"properties\\\":{\\\"humi\\\": %d}}]}\",0,0\r\n",(int)humi);
			sprintf(mode_msg,"AT+MQTTPUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"Dev_data\\\"\\,\\\"properties\\\":{\\\"mode\\\": %d}}]}\",0,0\r\n",My_mode);
			sprintf(use_msg,"AT+MQTTPUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"Dev_data\\\"\\,\\\"properties\\\":{\\\"occupied\\\": %d}}]}\",0,0\r\n",(int)empty_flag);
			WiFi_SendCmd(temp_msg,1);		
			WiFi_SendCmd(humi_msg,1);		
			WiFi_SendCmd(mode_msg,1);		
			WiFi_SendCmd(use_msg,1);	
			WiFi_RxCounter=0; 				//WiFi������������������                        
			memset(WiFi_RX_BUF,0,1024);
			WiFi_RxCounter=0; 				//WiFi������������������                        
			memset(WiFi_RX_BUF,0,1024);
			WiFi_RxCounter=0; 				//WiFi������������������                        
			memset(WiFi_RX_BUF,0,1024);
			LCD_DispStr(150, 80, (uint8_t *)"Connected   ", WHITE);
		}
		else
		{   
			//��ʾ����
			LCD_DispStr(150,  80, (uint8_t *)"Disconnected", RED);	
			//���ڱ���
			printf("Wifi ER\r\n");
		}
	}
	else
	{   
		//���ڱ���
    printf("Read DHT11 ERROR!\r\n");
	}
}
