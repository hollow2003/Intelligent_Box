#include "stm32f10x.h"
#include "user_rtc.h"
#include "user_usart.h"

RTC_T     g_tRTC;

//ö��ƽ���ÿ��������:1�£�2�£�3�£�4�£�5�£�6�£�7�£�8�£�9�£�10�£�11�£�12��
const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*****************************************************************************************************
 * ��  �� : ����RTCʱ�Ӻ���
 * ��  �� : ��
 * ����ֵ : ENABLE���ɹ�      DISABLE������
 * ��  ע : ������ݷ�Χ1970-2099
 *****************************************************************************************************/
FunctionalState RTC_WriteClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
	uint16_t temp;
	uint32_t seccount=0;

	//�ж����õ�����Ƿ�����Ч��Χ��[1970,2099]
	if (year < 1970 || year > 2099)
	{
		return DISABLE;	    //��ݲ���[1970,2099]�����ڣ����ش���
	}		
	
	//��ǰ����ݵ�������ӣ������ʼ��1970�꣩ 
	for (temp = 1970; temp < year; temp++) 
	{ 
		if (((0 == temp%4)&&(0 != temp%100)) ||(0 == temp %400))		// �ж��Ƿ�Ϊ����
		{
			seccount += 31622400;	    //����������� 
		}
		else
		{
			seccount += 31536000; 	  //ƽ��������� 
		}
	}

	
  //��ǰ���·ݵ����������
	month = month - 1;   //�Ȱ��·ݼ�һ����Ϊmonth_table[0]��Ӧ1�·�
	for (temp = 0; temp < month; temp++)        
	{
		seccount += (uint32_t)month_table[temp] * 86400;	// �·����������

		if ((((0 == year%4)&&(0 != year%100)) ||(0 == year %400)) && temp == 1)  //�ж��Ƿ�������2��
		{
			seccount += 86400;	//���������2��������һ��������� 
		}			
	}
  
	//��ǰ�����ڵ����������
	seccount += (uint32_t)(day - 1) * 86400;	

	//��ǰ��Сʱ�����������
	seccount += (uint32_t)hour * 3600;	

	//��ǰ����ӵ����������
	seccount += (uint32_t)min * 60;	

	//����ʣ������������
	seccount += sec;
																	
	PWR_BackupAccessCmd(ENABLE);                          //ʹ��RTC�ͺ󱸼Ĵ�������
  RTC_WaitForLastTask();                                //�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_SetCounter(seccount);                             //����RTC��������ֵ
	RTC_WaitForLastTask();                                //�ȴ����һ�ζ�RTC�Ĵ�����д�������

	return ENABLE;      
}

/*******************************************************************************************************
 * ��  �� : �õ���ǰʱ�ӡ�
 * ��  �� : ��
 * ����ֵ : ��
 * ��  ע : ���㷨�ĺ����Ƿ������ֽ��У�
            1��ͨ����ȡRTC������ֵ�õ������������׼ȷ����ݡ��·ݺ����ڣ�
            2��ͨ����ȥ������ʣ������������׼ȷ��ʱ���ֺ�����Ϣ��
*******************************************************************************************************/
void RTC_ReadClock(void)
{
	static uint16_t daycnt = 0;
	uint32_t timecnt = 0; 
	uint16_t temp1 = 0;
	uint32_t temp2 = 0;

	timecnt = RTC_GetCounter();                            //��ȡRTC��������ֵ     
	temp2 = timecnt / 86400;                               //�õ�����

	if (daycnt != temp2)	//��������������Ϊ0��������һ����
	{       
		daycnt = temp2;     //���õ���������ֵ��daycnt
		temp1 = 1970;       //��1970�꿪ʼ 

		while (temp2 >= 365)           //�ڵõ�����������364��������
		{                          
			if (((0 == temp1%4)&&(0 != temp1%100)) ||(0 == temp1%400))	//�ж��Ƿ������� 
			{
				if (temp2 >= 366)        	//�����꣬���ҵõ�����������365   
				{
					temp2 -= 366;		        //ȷ��������Ļ����ݼ�366
				}
				else        	            //�����꣬���õ�������С��366  
				{
					break;                	//�����˳� 
				}  
      }
			else 	                    //�������꣬����ƽ��
			{
				temp2 -= 365;           //ƽ��Ļ����ݼ�365
			}
			temp1++;                  //��1970�꿪ʼ�ݼ� 
		}   
		g_tRTC.Year = temp1;	      //���õ���ݴ�ŵ�ȫ�ֱ���g_tRTC��
		
		temp1 = 0;                  //��ʱ��������
		while (temp2 >= 28)	        //�ڵõ�������ȥ������ʣ�µ�����������28��
		{
			if((((0 == g_tRTC.Year%4)&&(0 != g_tRTC.Year%100)) ||(0 == g_tRTC.Year %400))&&temp1 == 1)	//�жϵ����ǲ��������2�·�
			{
				if (temp2 >= 29)        //�������2�·ݣ�����2����������28  
				{
					temp2 -= 29;	        //ȷ��������2�·ݣ��ݼ�29
				}
				else                    //�������2�·ݣ���2������С��29 
				{
					break;                //�����˳� 
				}
      }
      else 	                    //������ƽ�꣬����������ķ�2�·�
			{
				if (temp2 >= month_table[temp1])      //�ڵõ�������ȥ������ʣ�µ�������������·������Ƚϣ���1�µ�12��˳��
				{
					temp2 -= month_table[temp1];	      //���ڵ��ڱȽϵ��·��������ȥ��֮�Ƚϵ��·�����
				}
				else 
				{
					break;                              //С����֮�Ƚϵ��·��������˳���˵���ҵ����·�        
				}
			}
			temp1++;   
		}
		g_tRTC.Month = temp1 + 1;	      //���õ��·ݴ�ŵ�ȫ�ֱ���g_tRTC��
		g_tRTC.Day = temp2 + 1;         //���õ����ڴ�ŵ�ȫ�ֱ���g_tRTC��
	}

	temp2 = timecnt % 86400;                //�õ�������
	g_tRTC.Hour = temp2 / 3600;             //���õ�Сʱ��ŵ�ȫ�ֱ���g_tRTC��
	g_tRTC.Min = (temp2 % 3600) / 60;       //���õ����Ӵ�ŵ�ȫ�ֱ���g_tRTC��
	g_tRTC.Sec = (temp2 % 3600) % 60;       //���õ����Ӵ�ŵ�ȫ�ֱ���g_tRTC��

}   

/**************************************************************************************
 * ��  �� : ��ʼ��RTC
 * ��  �� : �����õ�ʱ����Ϣ����ݡ��·ݡ����ڡ�ʱ���֡��룩
 * ����ֵ : ��
 **************************************************************************************/
void RTC_Init(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)	
{
	if(BKP_ReadBackupRegister(BKP_DR2) != 0xA587)      //��ȡBKP���ݼĴ���2�������ǲ���0xA587  
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP , ENABLE);  //ʹ��RTC��BKPʱ��
		PWR_BackupAccessCmd(ENABLE);                          //ʹ��RTC�ͺ󱸼Ĵ�������
		BKP_DeInit();                                         //������BKP��ȫ���Ĵ�������Ϊȱʡֵ
		RCC_LSEConfig(RCC_LSE_ON);                            //����LSE
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);     //�ȴ�LSE����
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);               //ѡ��LSEΪRTCʱ��Դ
		RCC_RTCCLKCmd(ENABLE);                                //ʹ��RTCʱ��
		RTC_WaitForSynchro();                                 //�ȴ�RTC�Ĵ�����RTC��APBʱ��ͬ�����
		RTC_WaitForLastTask();                                //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC,ENABLE);                      //ʹ��RTC���ж�
		RTC_WaitForLastTask();                                //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetPrescaler(32767);                              //����RTCʱ�ӷ�ƵֵΪ32767
	  RTC_WaitForLastTask();                                //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_WriteClock(year, month, day, hour, min, sec);     //����ʱ��
		BKP_WriteBackupRegister(BKP_DR2, 0xA587);             //������ɺ���BKP���ݼĴ���2��д�����ַ�0xA587  
	}
  else                                                    //��ȡBKP���ݼĴ���2��������0xA587��˵���󱸼Ĵ���һֱ�е磩
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP , ENABLE);  //ʹ��RTC��BKPʱ��
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)      //����Ƿ��������
		{
		  printf("\r\n\r\n Power On Reset occurred....");
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) //����Ƿ�Reset�ܽŸ�λ
		{
			printf("\r\n\r\n External Reset occurred....");
		}
    RCC_ClearFlag();                                      //���RCC�и�λ��־ 
		RTC_WaitForLastTask();                                //�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}		
}
