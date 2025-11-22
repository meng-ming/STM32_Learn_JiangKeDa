#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM_LED.h"

int main(void)
{
	OLED_Init();

	OLED_ShowString(1,3,"HelloWorld!");
	
	Calcuelate_Param_Init(1000,1,0);
	
	PWM_LED_Init();
	
	uint16_t Duty_Ratio = 0; // 设置占空比
	
	int8_t Direction = 1; // 1：逐渐变量 -1：逐渐变暗
	
	while(1)
	{
		// 通过设置 CCR 寄存器的值，改变 PWM 占空比，实现呼吸灯效果
		TIM_SetCompare1(TIM2,Duty_Ratio);
		
		Duty_Ratio += Direction;
		
		if(Duty_Ratio >= 100)
		{
			Duty_Ratio = 100;
			Direction = -1;
		}else if (Duty_Ratio <= 0 ){
			Duty_Ratio = 0;
			Direction = 1;
		}
		Delay_ms(20);
	}
}
