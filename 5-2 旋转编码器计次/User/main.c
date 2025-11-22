#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"

int main(void)
{
	// 在所有硬件初始化之前，启动 Systick
	Delay_Init();
	
	Encoder_Init();
	
	OLED_Init();
	
	OLED_ShowString(1,1,"count:");

	while(1)
	{
		OLED_ShowSignedNum(1,7,Encoder_Count,5);
	}
}
