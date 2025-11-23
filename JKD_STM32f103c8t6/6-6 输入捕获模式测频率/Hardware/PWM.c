#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void PWM_Init(void)
{

	// 1. 选择时钟源以及初始化 GPIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 2. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	// 3. 配置输出比较器
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);// 初始化所有的变量
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; // CCR
	
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	// 4. 使能定时器
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_Set_Compare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

void PWM_Set_Prescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Immediate);
}
