#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1,1,"AD_0:");
	OLED_ShowString(2,1,"AD_1:");
	OLED_ShowString(3,1,"AD_2:");
	OLED_ShowString(4,1,"AD_3:");
	
	while(1)
	{
		// AD_Get_Value(); // AD: 软件触发 + 单次转换 + 扫描模式 DMA：硬件触发 + 单次转运
		OLED_ShowNum(1,6,Memory_Data[0],4);
		OLED_ShowNum(2,6,Memory_Data[1],4);
		OLED_ShowNum(3,6,Memory_Data[2],4);
		OLED_ShowNum(4,6,Memory_Data[3],4);
		
		Delay_ms(100); 
	}
}
