#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init(GPIOB,GPIO_Pin_0);
	
	OLED_ShowString(1,1,"WWDG_TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		OLED_ShowString(2,1,"WWDG_RST");
		Delay_ms(10);
		OLED_ShowString(2,1,"        ");
		
		// 需要手动清除标志位
		RCC_ClearFlag();
	}else{
		OLED_ShowString(3,1,"RST");
		Delay_ms(10);
		OLED_ShowString(3,1,"   ");
	}
	
	// 1. 使能窗口看门狗的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	// 当前想设置 超时时间为 50ms，同时窗口时间为 30ms
	// 根据公式
	// 超时时间(s) = 1 / 36000000 * 4096 * PSC * (T[5:0] + 1);
	// 窗口时间(s) = 1 / 36000000 * 4096 * PSC * (T[5:0] - W[5:0]);
	// 2. 设置预分频以及窗口值
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40 | 21); // T6 | W[5:0] 30ms

	// 3. 开启 窗口看门狗并喂狗
	WWDG_Enable(0x40 | 54); // // T6 | T[5:0] 50ms

	while(1)
	{
		// 使用获取按键值卡住循环，模仿主线程卡住情况
		Key_GetPressed_Num();

		
		// 不断显示 Feed 表示正常喂狗
		OLED_ShowString(4,1,"Feed");
		Delay_ms(20);
		OLED_ShowString(4,1,"    ");
		Delay_ms(20);
		
		// 喂狗
		WWDG_SetCounter(0x40 | 54);
	}
}
