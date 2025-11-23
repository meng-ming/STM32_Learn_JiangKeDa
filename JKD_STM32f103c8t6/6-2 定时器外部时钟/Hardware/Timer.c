#include "stm32f10x.h"                  // Device header

volatile uint16_t Timer_Count = 0;

void Timer_Init(void)
{
	// 本次使用  TIM2 ，通用定时器，总线为 APB1，通过外设定时
	
	// 需要注意的是：  在 STM32F103 芯片上，TIM2_ETR 默认连接在 PA0 引脚
	
	// 1. 内部时钟以及GPIO时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化外设对应引脚的 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3. 配置对应时钟的参数
	// 0x08: 连续 8 次采样到的电平都保持不变时，TIM_GetCounter 才加 1（防止遮挡一次跳变 ）
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x08);
	
	// 4. 配置时基单元
	
	// 计时频率: CK_CNT_OV = CK_PSC / (PSC + 1) / (ARR + 1) ）
	// CK_PSC : 72MHz
	// PSC(TIM_Prescaler) : 预分频值
	// ARR(TIM_Period) : ARR自动重装值
	// 计数器计数频率：CK_CNT = CK_PSC / (PSC + 1)
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 只有高级定时器才有重复计数器
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	// 5. 配置输出中断控制，允许更新中断输出到 NVIC
	
	TIM_ClearFlag(TIM2,TIM_IT_Update); // 手动清除更新中断标志位，防止时基单元初始化之后立马进入更新中断导致单片机复位之后初始值为1
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// 6. 配置 NVIC
	
	// (1) 中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// (2) 初始化 NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	// 7. 使能时钟
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void)
{
	// 判断当前中断是否匹配
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		
		Timer_Count ++ ;
		
		// 清除中断标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
