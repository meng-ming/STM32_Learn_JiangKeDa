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
	
	// 待机模式需要使能 PWR 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	// 使用 WakeUp 功能主动退出待机模式（上升沿触发）
	PWR_WakeUpPinCmd(ENABLE);
	
	OLED_ShowString(1,1,"CNT:");
	OLED_ShowString(2,1,"ALR:");
	OLED_ShowString(3,1,"ALRF:");

	// 设置闹钟(10s 之后闹钟响)
	uint32_t Alarm_Value = RTC_GetCounter() + 5;
	RTC_SetAlarm(Alarm_Value);
	OLED_ShowNum(2,5,Alarm_Value,10);
	
	while(1)
	{
		OLED_ShowNum(1,5,RTC_GetCounter(),10);
		
		// 显示 Alarm 标志位
		OLED_ShowNum(3,6,RTC_GetFlagStatus(RTC_FLAG_ALR),1);
		
		// 便于查看现象，每次闹钟响时，会闪烁一下 Running
		OLED_ShowString(4,1,"Running");
		Delay_ms(300);
		OLED_ShowString(4,1,"       ");
		
		// 进入待机模式（待机模式重启是整个程序重新启动，而非当前指令所在位置）
		PWR_EnterSTANDBYMode();
	}
}
