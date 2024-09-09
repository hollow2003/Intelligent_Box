#include "stm32f10x.h"
#include "user_rtc.h"
#include "user_usart.h"

RTC_T     g_tRTC;

//枚举平年的每月天数表:1月，2月，3月，4月，5月，6月，7月，8月，9月，10月，11月，12月
const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*****************************************************************************************************
 * 描  述 : 设置RTC时钟函数
 * 入  参 : 无
 * 返回值 : ENABLE：成功      DISABLE：错误
 * 备  注 : 设置年份范围1970-2099
 *****************************************************************************************************/
FunctionalState RTC_WriteClock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
	uint16_t temp;
	uint32_t seccount=0;

	//判断设置的年份是否在有效范围内[1970,2099]
	if (year < 1970 || year > 2099)
	{
		return DISABLE;	    //年份不在[1970,2099]区间内，返回错误
	}		
	
	//把前面年份的秒钟相加（年份起始是1970年） 
	for (temp = 1970; temp < year; temp++) 
	{ 
		if (((0 == temp%4)&&(0 != temp%100)) ||(0 == temp %400))		// 判断是否为闰年
		{
			seccount += 31622400;	    //闰年的秒钟数 
		}
		else
		{
			seccount += 31536000; 	  //平年的秒钟数 
		}
	}

	
  //把前面月份的秒钟数相加
	month = month - 1;   //先把月份减一，因为month_table[0]对应1月份
	for (temp = 0; temp < month; temp++)        
	{
		seccount += (uint32_t)month_table[temp] * 86400;	// 月份秒钟数相加

		if ((((0 == year%4)&&(0 != year%100)) ||(0 == year %400)) && temp == 1)  //判断是否是闰年2月
		{
			seccount += 86400;	//如果是闰年2月则增加一天的秒钟数 
		}			
	}
  
	//把前面日期的秒钟数相加
	seccount += (uint32_t)(day - 1) * 86400;	

	//把前面小时的秒钟数相加
	seccount += (uint32_t)hour * 3600;	

	//把前面分钟的秒钟数相加
	seccount += (uint32_t)min * 60;	

	//把所剩的秒钟数加上
	seccount += sec;
																	
	PWR_BackupAccessCmd(ENABLE);                          //使能RTC和后备寄存器访问
  RTC_WaitForLastTask();                                //等待最近一次对RTC寄存器的写操作完成
	RTC_SetCounter(seccount);                             //设置RTC计数器的值
	RTC_WaitForLastTask();                                //等待最近一次对RTC寄存器的写操作完成

	return ENABLE;      
}

/*******************************************************************************************************
 * 描  述 : 得到当前时钟。
 * 入  参 : 无
 * 返回值 : 无
 * 备  注 : 本算法的核心是分两部分进行：
            1、通过读取RTC计数器值得到整天数计算出准确的年份、月份和日期；
            2、通过除去整天数剩余的秒数计算出准确的时、分和秒信息。
*******************************************************************************************************/
void RTC_ReadClock(void)
{
	static uint16_t daycnt = 0;
	uint32_t timecnt = 0; 
	uint16_t temp1 = 0;
	uint32_t temp2 = 0;

	timecnt = RTC_GetCounter();                            //获取RTC计数器的值     
	temp2 = timecnt / 86400;                               //得到天数

	if (daycnt != temp2)	//如果计算的天数不为0，即超过一天了
	{       
		daycnt = temp2;     //将得到的天数赋值给daycnt
		temp1 = 1970;       //从1970年开始 

		while (temp2 >= 365)           //在得到的天数大于364天的情况下
		{                          
			if (((0 == temp1%4)&&(0 != temp1%100)) ||(0 == temp1%400))	//判断是否是闰年 
			{
				if (temp2 >= 366)        	//是闰年，并且得到的天数大于365   
				{
					temp2 -= 366;		        //确定是闰年的话，递减366
				}
				else        	            //是闰年，但得到的天数小于366  
				{
					break;                	//错误，退出 
				}  
      }
			else 	                    //不是闰年，则是平年
			{
				temp2 -= 365;           //平年的话，递减365
			}
			temp1++;                  //从1970年开始递加 
		}   
		g_tRTC.Year = temp1;	      //将得到年份存放到全局变量g_tRTC中
		
		temp1 = 0;                  //临时变量清零
		while (temp2 >= 28)	        //在得到的天数去掉整年剩下的天数超过了28天
		{
			if((((0 == g_tRTC.Year%4)&&(0 != g_tRTC.Year%100)) ||(0 == g_tRTC.Year %400))&&temp1 == 1)	//判断当年是不是闰年的2月份
			{
				if (temp2 >= 29)        //是闰年的2月份，并且2月天数大于28  
				{
					temp2 -= 29;	        //确定是闰年2月份，递减29
				}
				else                    //是闰年的2月份，但2月天数小于29 
				{
					break;                //错误，退出 
				}
      }
      else 	                    //当年是平年，或者是闰年的非2月份
			{
				if (temp2 >= month_table[temp1])      //在得到的天数去掉整年剩下的天数依次与各月份天数比较（从1月到12月顺序）
				{
					temp2 -= month_table[temp1];	      //大于等于比较的月份天数则减去与之比较的月份天数
				}
				else 
				{
					break;                              //小于与之比较的月份天数则退出，说明找到了月份        
				}
			}
			temp1++;   
		}
		g_tRTC.Month = temp1 + 1;	      //将得到月份存放到全局变量g_tRTC中
		g_tRTC.Day = temp2 + 1;         //将得到日期存放到全局变量g_tRTC中
	}

	temp2 = timecnt % 86400;                //得到秒钟数
	g_tRTC.Hour = temp2 / 3600;             //将得到小时存放到全局变量g_tRTC中
	g_tRTC.Min = (temp2 % 3600) / 60;       //将得到分钟存放到全局变量g_tRTC中
	g_tRTC.Sec = (temp2 % 3600) % 60;       //将得到秒钟存放到全局变量g_tRTC中

}   

/**************************************************************************************
 * 描  述 : 初始化RTC
 * 入  参 : 待配置的时间信息（年份、月份、日期、时、分、秒）
 * 返回值 : 无
 **************************************************************************************/
void RTC_Init(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)	
{
	if(BKP_ReadBackupRegister(BKP_DR2) != 0xA587)      //读取BKP数据寄存器2中数据是不是0xA587  
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP , ENABLE);  //使能RTC和BKP时钟
		PWR_BackupAccessCmd(ENABLE);                          //使能RTC和后备寄存器访问
		BKP_DeInit();                                         //将外设BKP的全部寄存器重设为缺省值
		RCC_LSEConfig(RCC_LSE_ON);                            //开启LSE
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);     //等待LSE起振
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);               //选择LSE为RTC时钟源
		RCC_RTCCLKCmd(ENABLE);                                //使能RTC时钟
		RTC_WaitForSynchro();                                 //等待RTC寄存器与RTC的APB时钟同步完成
		RTC_WaitForLastTask();                                //等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC,ENABLE);                      //使能RTC秒中断
		RTC_WaitForLastTask();                                //等待最近一次对RTC寄存器的写操作完成
		RTC_SetPrescaler(32767);                              //设置RTC时钟分频值为32767
	  RTC_WaitForLastTask();                                //等待最近一次对RTC寄存器的写操作完成
		
		RTC_WriteClock(year, month, day, hour, min, sec);     //设置时间
		BKP_WriteBackupRegister(BKP_DR2, 0xA587);             //配置完成后，向BKP数据寄存器2中写特殊字符0xA587  
	}
  else                                                    //读取BKP数据寄存器2中数据是0xA587（说明后备寄存器一直有电）
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP , ENABLE);  //使能RTC和BKP时钟
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)      //检查是否掉电重启
		{
		  printf("\r\n\r\n Power On Reset occurred....");
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) //检查是否Reset管脚复位
		{
			printf("\r\n\r\n External Reset occurred....");
		}
    RCC_ClearFlag();                                      //清除RCC中复位标志 
		RTC_WaitForLastTask();                                //等待最近一次对RTC寄存器的写操作完成
	}		
}
