#include "stm32f10x.h"                  // Device header
#include "DC_Motor.h"
#include "PWM.h"

void DC_Motor_Init(void)
{
	Calcuelate_Param_Init(25000,1,0);
	
	PWM_Init();
	
	// 初始化GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Set_DC_Motor_Speed(int16_t speed)
{
	if(speed>0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		TIM_SetCompare3(TIM2,speed);
	}else{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		TIM_SetCompare3(TIM2,-speed);
	}
}
