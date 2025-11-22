#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM_Servo.h"

int main(void)
{
	OLED_Init();

	OLED_ShowString(1,3,"Hello!");
	
	// 舵机周期 20ms，高电平宽度 0.5ms,1ms,1.5ms,2ms,2.5ms
	
	int CRR[] = {500,1000,1500,2000,2500};
	
	PWM_Servo_Init();
	
	int idx = 0,direction = 1;
	
	while(1)
	{
		TIM_SetCompare2(TIM2,CRR[idx]);
		idx += direction;
		if(idx>= (sizeof(CRR) / sizeof(CRR[0]) - 1))
		{
			idx = (sizeof(CRR) / sizeof(CRR[0]) - 1);
			direction = - 1;
		}else if(idx <= 0){
			idx = 0;
			direction = 1;
		}
		Delay_ms(300);
	}
}
