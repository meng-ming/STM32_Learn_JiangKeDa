#include "Delay.h"
#include "LED.h"
#include "Key.h"

int main(void)
{
	Key_Pressed_Num_e key_val;

	LED_Init_All();
	Key_Init_All();
	
	while(1)
	{
		// 获取按下按下的按键序号
		key_val = Key_GetPressed_Num();
		
		if(key_val == Key_1)
		{
			LED_State_Toggle(LED_1);
		}

		if(key_val == Key_2)
			LED_State_Toggle(LED_2);
	}
}
