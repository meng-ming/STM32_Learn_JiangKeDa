#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "My_RTC.h"

int main(void)
{
	Time_s Time_Structure,Time_Return_Structure;
	Time_Structure.tm_year = 2025 - 1900;
	Time_Structure.tm_mon = 11 - 1;
	Time_Structure.tm_mday = 21;
	Time_Structure.tm_hour = 15;
	Time_Structure.tm_min = 20;
	Time_Structure.tm_sec = 10;
	
	OLED_Init();
	My_RTC_Init(&Time_Structure);
	
	OLED_ShowString(1,1,"Date:xxxx-xx-xx");
	OLED_ShowString(2,1,"Time:xx:xx:xx");
	OLED_ShowString(3,1,"CNT:");
	OLED_ShowString(4,1,"DIV:");
	
	while(1)
	{
		My_RTC_Get_Time(&Time_Return_Structure);
		OLED_ShowNum(1,6,Time_Return_Structure.tm_year + 1900,4);
		OLED_ShowNum(1,11,Time_Return_Structure.tm_mon + 1,2);
		OLED_ShowNum(1,14,Time_Return_Structure.tm_mday,2);
		OLED_ShowNum(2,6,Time_Return_Structure.tm_hour,2);
		OLED_ShowNum(2,9,Time_Return_Structure.tm_min,2);
		OLED_ShowNum(2,12,Time_Return_Structure.tm_sec,2);
		
		OLED_ShowNum(3,5,RTC_GetCounter(),10);
		
		OLED_ShowNum(4,5,RTC_GetDivider(),10);
	}
}
