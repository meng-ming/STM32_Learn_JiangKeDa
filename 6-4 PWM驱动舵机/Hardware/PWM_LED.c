#include "stm32f10x.h"                  // Device header
#include "PWM_LED.h"

void PWM_LED_Init(void)
{
	// 使用 TIM2 通过 PWM 控制外设 LED 的亮度，达到呼吸灯的效果
	// 当前使用参数
	// 计时频率(也即PWM频率) CK_CNT_OV = CK_PSC / (PSC + 1) / (ARR + 1)
	// PWM 分辨率：1 / (ARR + 1)
	// PWM 占空比：CRR / (ARR + 1)
	
	// 1. 选择时钟源以及初始化 GPIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 2. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = PWM_LED_Param_Init.ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PWM_LED_Param_Init.PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	// 3. 配置输出比较器
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);// 初始化所有的变量
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 当前接线为 LED 正极接到 GPIOA Pin_0 引脚，负极接地，因此需要高电平有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM_LED_Param_Init.CCR; // CCR
	
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	// 4. 使能定时器
	TIM_Cmd(TIM2,ENABLE);
}
