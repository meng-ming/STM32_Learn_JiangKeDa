#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1,1,"Freq:00000Hz");
	
	OLED_ShowString(2,1,"Duty:00%");
	
	// 当前固定 ARR 为 99
	PWM_Set_Prescaler(7200 - 1); // Freq = 72M / (PSC + 1) / (ARR + 1)
	
	PWM_Set_Compare1(40); // Duty_Ratio = CRR / (ARR + 1)
	
	while(1)
	{
		OLED_ShowNum(1,6,IC_Get_Freq(),5);
		OLED_ShowNum(2,6,IC_Get_Duty_Ratio(),2);
	}
}
