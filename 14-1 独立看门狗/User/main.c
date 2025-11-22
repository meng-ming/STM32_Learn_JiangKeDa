#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"

int main(void)
{
	OLED_Init();
	Key_Init(GPIOB,GPIO_Pin_0);
	
	OLED_ShowString(1,1,"IWDG_TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		OLED_ShowString(2,1,"IWDGRST");
		Delay_ms(100);
		OLED_ShowString(2,1,"       ");
		
		// 需要手动清除标志位
		RCC_ClearFlag();
	}else{
		OLED_ShowString(3,1,"RST");
		Delay_ms(100);
		OLED_ShowString(3,1,"   ");
	}
	
	// 1. 解除写保护，保证寄存器能够写入信息
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	// 喂狗周期为 1000ms ，使用计算公式计算
	// 1(s) = 1 / 40000（时钟源的频率） * PRS * (RL + 1);
	
	// 2. 设置 PSR 和 RL 寄存器的值
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499 - 1);  // 1000ms
	
	// 3. 执行一次喂狗，尽量保证时间精确
	IWDG_ReloadCounter();
	
	// 4. 开启独立看门狗
	IWDG_Enable();

	while(1)
	{
		// 使用获取按键值卡住循环，模仿主线程卡住情况
		Key_GetPressed_Num();
		
		// 执行喂狗
		IWDG_ReloadCounter();
		
		// 不断显示 Feed 表示正常喂狗
		OLED_ShowString(4,1,"Feed");
		Delay_ms(500);
		OLED_ShowString(4,1,"    ");
		Delay_ms(300);
	}
}
