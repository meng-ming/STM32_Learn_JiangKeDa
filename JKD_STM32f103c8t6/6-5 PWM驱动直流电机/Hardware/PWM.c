#include "stm32f10x.h"                  // Device header
#include "PWM.h"

PWM_Init_t PWM_Param_Init = {0};

void Calcuelate_Param_Init(uint16_t PWM_Frequency,uint16_t PWM_Resolution,int8_t PWM_Duty_Ratio)
{
	PWM_Param_Init.ARR = 100 / PWM_Resolution - 1;
	PWM_Param_Init.PSC = (72000000 / PWM_Frequency / (PWM_Param_Init.ARR + 1)) - 1;;
	PWM_Param_Init.CCR = PWM_Duty_Ratio * (PWM_Param_Init.ARR + 1) / 100;
}

void PWM_Init(void)
{

	// 1. 选择时钟源以及初始化 GPIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 2. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = PWM_Param_Init.ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PWM_Param_Init.PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	// 3. 配置输出比较器
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);// 初始化所有的变量
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 当前接线为 LED 正极接到 GPIOA Pin_0 引脚，负极接地，因此需要高电平有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM_Param_Init.CCR; // CCR
	
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	
	// 4. 使能定时器
	TIM_Cmd(TIM2,ENABLE);
}
