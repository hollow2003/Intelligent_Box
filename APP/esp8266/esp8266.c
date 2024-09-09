#include "esp8266.h"
#include "SysTick.h"
#include "usart.h"
/*-------------------------------------------------*/
/*��������WiFi��λ                                 */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO();                                    //��λIO���͵�ƽ
	delay_ms(500);                                  //��ʱ500ms
	SET_IO();                                    //��λIO���ߵ�ƽ	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(100);                              //��ʱ100ms
		if(strstr(WiFi_RX_BUF,"ready"))             //������յ�ready��ʾ��λ�ɹ�
		{break;}       						    //��������whileѭ��      
	}
	
	if(timeout<=0)
	{                            
		return 1;
	}                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
	else {                            
		return 0;
	} 		         				    //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}
 
/*-------------------------------------------------*/
/*��������WiFi��������ָ��                         */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //���WiFi���ջ����� 
	WiFi_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		delay_ms(100);                          //��ʱ100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						//��������whileѭ��
	}
	printf("\r\n");                          //���������Ϣ
	if(timeout<=0)
 {
		return 1;
 }                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
 
/*-------------------------------------------------*/
/*��������WiFi����·����ָ��                       */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter=0;                               //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS); //����ָ��	(wifi������)
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP\r\n\r\nOK")) //������յ�WIFI GOT IP��ʾ�ɹ�
			break;       						    //��������whileѭ��
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                              //���������Ϣ
	if(timeout<=0)
	{
		printf("connection error");
		connection =0;
		return 1;
	}		//���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	else 
	{
		connection =1;
	}
	return 0;                                       //��ȷ������0
}
 
/*-------------------------------------------------*/
/*���������ȴ�����wifi����ȡIP��ַ                 */
/*��  ����ip������IP������                         */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_GetIP(uint16_t timeout)
{
	char *presult1,*presult2;
	char ip[50];
	WiFi_printf("AT+CIFSR\r\n");                    //����ָ��	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(100);                              //��ʱ100ms
		if(strstr(WiFi_RX_BUF,"OK"))                //������յ�OK��ʾ�ɹ�
			break;       						    //��������whileѭ��
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                              //���������Ϣ
	if(timeout<=0)
	{
		connection =0;
		return 1;
	}		//���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else{
		presult1 = strstr(WiFi_RX_BUF,"\"");
		if( presult1 != NULL ){
			presult2 = strstr(presult1+1,"\"");
			if( presult2 != NULL ){
				memcpy(ip,presult1+1,presult2-presult1-1);
				if(strcmp(ip,"0.0.0.0") == 0)
				{			
					connection =0;
				}
				printf("ESP8266 IP address��%s\r\n",ip);     //������ʾIP��ַ
				return 0;    //��ȷ����0
			}
			else 
			{
				connection =0;
				return 2;
			}  //δ�յ�Ԥ������
		}
		else 
		{
			connection =0;
			return 3;
		}      //δ�յ�Ԥ������	
	}
}
 
/*-------------------------------------------------*/
/*��������������������ʼ��                         */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_InitServer(void)
{	
	printf("Ready to set STA mode\r\n");                  //������ʾ����
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("Failed to set STA mode, ready to restart\r\n");    //���ط�0ֵ������if������STAģʽʧ�ܣ�׼������
		return 2;                                      //����2
	}else printf("Reset STA mode succeeded\r\n");            //������ʾ����
	
//	if(wifi_mode==0){
		//�������ģʽ=0��SSID������д�ڳ����� 
		printf("Ready to cancel automatic connection\r\n");                 //׼��ȡ���Զ�����
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //ȡ���Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
			printf("Failed to cancel automatic connection, ready to restart\r\n");   //���ط�0ֵ������if��ȡ���Զ�����ʧ�ܣ�׼������
			return 3;                                      //����3
		}else printf("succeeded\r\n");           //ȡ���Զ����ӳɹ�
				
		printf("Preparing to connect to the router\r\n");                   //׼������·����
		if(WiFi_JoinAP(15)){                               //����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
			printf("Failed to connect the router, ready to restart\r\n");     //���ط�0ֵ������if������·����ʧ�ܣ�׼������
			return 4;                                      //����4	
		}else printf("succeeded\r\n");             //������ʾ����		
			printf("Preparing to get IP address\r\n");                   //׼����ȡIP��ַ
	if(WiFi_GetIP(25)){                                //׼����ȡIP��ַ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("Failed to obtain IP address, ready to restart\r\n");     //���ط�0ֵ������if����ȡIP��ַʧ�ܣ�׼������
		return 10;                                     //����10
	}else printf("ok\r\n");             //��ȡIP��ַ�ɹ�
	
	printf("Ready to set user and secret\r\n");                     //׼�������û�������
	if(WiFi_SendCmd("AT+MQTTUSERCFG=0,1,\"NULL\",\"62ef60e85f266f255bb9f8eb_esp8266_test0\",\"592d297f5dbd28fec238bf3cce6a7a74b9db760f220ae1b645d8d89adbd6cf12\",0,0,\"\"",50))
		{               //�����û������룬100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("false\r\n");       //���ط�0ֵ������if��������ʾ����
		return 11;                                     //����11
		}
		else printf("ok\r\n");  		//�����û�������ɹ�
		
		printf("Ready to set clientID\r\n");                     //׼������Client
	if(WiFi_SendCmd("AT+MQTTCLIENTID=0,\"62ef60e85f266f255bb9f8eb_esp8266_test0_0_0_2022110113\"",50))
		{               //����Client��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("false\r\n");       //���ط�0ֵ������if��������ʾ����
		return 12;                                     //����11
		}
		else printf("ok\r\n"); 
		
		printf("Ready to set adress and com\r\n");                     //׼�����õ�ַ���˿�
	if(WiFi_SendCmd("AT+MQTTCONN=0,\"b23c9ff19f.iot-mqtts.cn-north-4.myhuaweicloud.com\",1883,1",50))
		{               //���õ�ַ���˿�͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("false\r\n");       //���ط�0ֵ������if��������ʾ����
		return 13;                                     //����11
		}
		else printf("ok\r\n"); 
	
		printf("Ready to set sub topic\r\n");                     //׼�����Ļ���
	if(WiFi_SendCmd("AT+MQTTSUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/properties/report\",1",50))
		{               //���Ļ��⣬100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("false\r\n");       //���ط�0ֵ������if��������ʾ����
		return 14;                                     //����11
		}
		else printf("ok\r\n"); 
		
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
	return 0;                                          //��ȷ����0
	
}
void clearwififlash()
{
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
}
