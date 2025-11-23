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

	while(1)
	{
		OLED_ShowNum(1,7,Count_Sensor_Count,5);
	}
}
