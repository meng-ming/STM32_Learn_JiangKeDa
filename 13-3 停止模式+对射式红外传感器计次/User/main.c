#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
	// 在所有硬件初始化之前，启动 Systick
	Delay_Init();
	
	OLED_Init();
	
	OLED_ShowString(1,1,"count:");

	Count_Sensor_Init();

	// 使能 PWR 时钟，不然后续写入无效
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	while(1)
	{
		OLED_ShowNum(1,7,Count_Sensor_Count,5);
		
		OLED_ShowString(2,1,"Running");
		Delay_ms(200);
		OLED_ShowString(2,1,"       ");
		
		PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_Regulator_ON);
		
		// 从停止模式恢复之后，系统时钟默认使用 LSI ：8M 主频
		// 调用 SystemInit 函数，恢复时钟设置
		SystemInit();
	}
}
