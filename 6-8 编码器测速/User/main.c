#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Internal_Timer.h"
#include "Encoder.h"

int main(void)
{
	OLED_Init();
	Internal_Timer_Init();
	Encoder_Init();
	
	OLED_ShowString(1,1,"Speed:");

	while(1)
	{
		OLED_ShowSignedNum(1,7,Encoder_Speed,5);
	}
}
