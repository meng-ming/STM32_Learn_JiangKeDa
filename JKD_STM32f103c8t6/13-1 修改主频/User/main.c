#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
	OLED_Init();

	OLED_ShowString(1,1,"SysClk:");
	
	OLED_ShowNum(1,8,SystemCoreClock,8);

	while(1)
	{
		OLED_ShowString(2,1,"Running");
		Delay_ms(500);
		OLED_ShowString(2,1,"       ");
		Delay_ms(500);
	}
}
