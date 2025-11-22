#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"

uint16_t AD_0,AD_1,AD_2,AD_3;

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1,1,"AD0:");
	OLED_ShowString(2,1,"AD1:");
	OLED_ShowString(3,1,"AD2:");
	OLED_ShowString(4,1,"AD3:");
	
	while(1)
	{
		AD_0 = AD_Get_Value(ADC_Channel_0);
		AD_1 = AD_Get_Value(ADC_Channel_1);
		AD_2 = AD_Get_Value(ADC_Channel_2);
		AD_3 = AD_Get_Value(ADC_Channel_3);
		
		OLED_ShowNum(1,5,AD_0,4);
		OLED_ShowNum(2,5,AD_1,4);
		OLED_ShowNum(3,5,AD_2,4);
		OLED_ShowNum(4,5,AD_3,4);
		
		Delay_ms(100); 
	}
}
