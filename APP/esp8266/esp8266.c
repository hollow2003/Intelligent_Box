#include "esp8266.h"
#include "SysTick.h"
#include "usart.h"
/*-------------------------------------------------*/
/*函数名：WiFi复位                                 */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO();                                    //复位IO拉低电平
	delay_ms(500);                                  //延时500ms
	SET_IO();                                    //复位IO拉高电平	
	while(timeout--){                               //等待超时时间到0
		delay_ms(100);                              //延时100ms
		if(strstr(WiFi_RX_BUF,"ready"))             //如果接收到ready表示复位成功
		{break;}       						    //主动跳出while循环      
	}
	
	if(timeout<=0)
	{                            
		return 1;
	}                         //如果timeout<=0，说明超时时间到了，也没能收到ready，返回1
	else {                            
		return 0;
	} 		         				    //反之，表示正确，说明收到ready，通过break主动跳出while
}
 
/*-------------------------------------------------*/
/*函数名：WiFi发送设置指令                         */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	WiFi_printf("%s\r\n",cmd);                  //发送指令
	while(timeout--){                           //等待超时时间到0
		delay_ms(100);                          //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
	}
	printf("\r\n");                          //串口输出信息
	if(timeout<=0)
 {
		return 1;
 }                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}
 
/*-------------------------------------------------*/
/*函数名：WiFi加入路由器指令                       */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS); //发送指令	(wifi和密码)
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP\r\n\r\nOK")) //如果接收到WIFI GOT IP表示成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	printf("\r\n");                              //串口输出信息
	if(timeout<=0)
	{
		printf("connection error");
		connection =0;
		return 1;
	}		//如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	else 
	{
		connection =1;
	}
	return 0;                                       //正确，返回0
}
 
/*-------------------------------------------------*/
/*函数名：等待连接wifi，获取IP地址                 */
/*参  数：ip：保存IP的数组                         */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_GetIP(uint16_t timeout)
{
	char *presult1,*presult2;
	char ip[50];
	WiFi_printf("AT+CIFSR\r\n");                    //发送指令	
	while(timeout--){                               //等待超时时间到0
		delay_ms(100);                              //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))                //如果接收到OK表示成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	printf("\r\n");                              //串口输出信息
	if(timeout<=0)
	{
		connection =0;
		return 1;
	}		//如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
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
				printf("ESP8266 IP address：%s\r\n",ip);     //串口显示IP地址
				return 0;    //正确返回0
			}
			else 
			{
				connection =0;
				return 2;
			}  //未收到预期数据
		}
		else 
		{
			connection =0;
			return 3;
		}      //未收到预期数据	
	}
}
 
/*-------------------------------------------------*/
/*函数名：建立服务器初始化                         */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_InitServer(void)
{	
	printf("Ready to set STA mode\r\n");                  //串口提示数据
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //设置STA模式，100ms超时单位，总计5s超时时间
		printf("Failed to set STA mode, ready to restart\r\n");    //返回非0值，进入if，设置STA模式失败，准备重启
		return 2;                                      //返回2
	}else printf("Reset STA mode succeeded\r\n");            //串口提示数据
	
//	if(wifi_mode==0){
		//如果联网模式=0：SSID和密码写在程序里 
		printf("Ready to cancel automatic connection\r\n");                 //准备取消自动连接
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //取消自动连接，100ms超时单位，总计5s超时时间
			printf("Failed to cancel automatic connection, ready to restart\r\n");   //返回非0值，进入if，取消自动连接失败，准备重启
			return 3;                                      //返回3
		}else printf("succeeded\r\n");           //取消自动连接成功
				
		printf("Preparing to connect to the router\r\n");                   //准备连接路由器
		if(WiFi_JoinAP(15)){                               //连接路由器,1s超时单位，总计30s超时时间
			printf("Failed to connect the router, ready to restart\r\n");     //返回非0值，进入if，连接路由器失败，准备重启
			return 4;                                      //返回4	
		}else printf("succeeded\r\n");             //串口提示数据		
			printf("Preparing to get IP address\r\n");                   //准备获取IP地址
	if(WiFi_GetIP(25)){                                //准备获取IP地址，100ms超时单位，总计5s超时时间
		printf("Failed to obtain IP address, ready to restart\r\n");     //返回非0值，进入if，获取IP地址失败，准备重启
		return 10;                                     //返回10
	}else printf("ok\r\n");             //获取IP地址成功
	
	printf("Ready to set user and secret\r\n");                     //准备设置用户及密码
	if(WiFi_SendCmd("AT+MQTTUSERCFG=0,1,\"NULL\",\"62ef60e85f266f255bb9f8eb_esp8266_test0\",\"592d297f5dbd28fec238bf3cce6a7a74b9db760f220ae1b645d8d89adbd6cf12\",0,0,\"\"",50))
		{               //设置用户及密码，100ms超时单位，总计5s超时时间
		printf("false\r\n");       //返回非0值，进入if，串口提示数据
		return 11;                                     //返回11
		}
		else printf("ok\r\n");  		//设置用户及密码成功
		
		printf("Ready to set clientID\r\n");                     //准备设置Client
	if(WiFi_SendCmd("AT+MQTTCLIENTID=0,\"62ef60e85f266f255bb9f8eb_esp8266_test0_0_0_2022110113\"",50))
		{               //设置Client，100ms超时单位，总计5s超时时间
		printf("false\r\n");       //返回非0值，进入if，串口提示数据
		return 12;                                     //返回11
		}
		else printf("ok\r\n"); 
		
		printf("Ready to set adress and com\r\n");                     //准备设置地址及端口
	if(WiFi_SendCmd("AT+MQTTCONN=0,\"b23c9ff19f.iot-mqtts.cn-north-4.myhuaweicloud.com\",1883,1",50))
		{               //设置地址及端口透传，100ms超时单位，总计5s超时时间
		printf("false\r\n");       //返回非0值，进入if，串口提示数据
		return 13;                                     //返回11
		}
		else printf("ok\r\n"); 
	
		printf("Ready to set sub topic\r\n");                     //准备订阅话题
	if(WiFi_SendCmd("AT+MQTTSUB=0,\"$oc/devices/62ef60e85f266f255bb9f8eb_esp8266_test0/sys/properties/report\",1",50))
		{               //订阅话题，100ms超时单位，总计5s超时时间
		printf("false\r\n");       //返回非0值，进入if，串口提示数据
		return 14;                                     //返回11
		}
		else printf("ok\r\n"); 
		
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
	return 0;                                          //正确返回0
	
}
void clearwififlash()
{
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
}
