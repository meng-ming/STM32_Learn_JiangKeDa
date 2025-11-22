#include "stm32f10x.h"  // Device header
#include "Encoder.h"
#include "Internal_Timer.h"

static TIM_TypeDef* TIM_Select = TIM2;
// 当前默认为 1Hz，即每 1s 进入一次中断
static uint16_t TIM_Period_Value = 10000 - 1;
static uint16_t TIM_Prescaler_Value = 7200 - 1;

volatile int16_t Encoder_Speed = 0;

void Internal_Timer_Init(void)
{
	// 使用 TIM2 进行内部定时器中断
	// 1. RCC 使能 TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	// 2. 选择时钟源（内部时钟）
	TIM_InternalClockConfig(TIM_Select);
	
	// 3. 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = TIM_Period_Value;
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_Prescaler_Value;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM_Select,&TIM_TimeBaseInitStructure);
	
	// 4. 允许中断输出到NVIC，使能更新标志位
	
	TIM_ClearFlag(TIM_Select,TIM_FLAG_Update); // 手动清除更新标志位，防止初始化不为0
	TIM_ITConfig(TIM_Select,TIM_IT_Update,ENABLE);
	
	// 5. 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// 6. 配置 NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	// 7. 使能定时器
	TIM_Cmd(TIM_Select,ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM_Select,TIM_IT_Update) == SET)
	{
		Encoder_Speed = Encoder_Get_CNT();
		TIM_ClearITPendingBit(TIM_Select,TIM_IT_Update);
	}
}
