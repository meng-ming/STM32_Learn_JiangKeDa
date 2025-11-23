#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "DC_Motor.h"
#include "Key.h"

int main(void)
{
	OLED_Init();

	OLED_ShowString(1,1,"Speed:");
	
	DC_Motor_Init();
	
	Key_Init_All();
	
	int speed = 0,direction = 1;;
	
	while(1)
	{
		if(Key_GetPressed_Num() == Key_1)
		{
			speed += 10 * direction;
		}
		Set_DC_Motor_Speed(speed);
		if(speed >= 100)
			direction = -1;
		if(speed <= -100)
			direction = 1;
		OLED_ShowSignedNum(1,7,speed,4);
	}
}
