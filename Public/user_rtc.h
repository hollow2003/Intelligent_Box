#include "stm32f10x.h"

typedef struct
{
	uint16_t Year;
	uint8_t Month;
	uint8_t Day;	
	uint8_t Hour;		
	uint8_t Min;				
	uint8_t Sec;					
}RTC_T;


extern RTC_T g_tRTC;
extern void RTC_Init(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
extern void RTC_ReadClock(void);

