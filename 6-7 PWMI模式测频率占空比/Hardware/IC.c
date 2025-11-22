#include "stm32f10x.h"                  // Device header
#include "IC.h"

static uint16_t PSC_Value = 72;
static TIM_TypeDef* TIM_Select = TIM3; 
static uint32_t IC_Clock_Freq_Hz = 72000000 / 72;

void IC_Init(void)
{
	// 使用 PWMI 通过两个通道，分别测量输入信号的频率以及占空比
	
	// 1. 使能 TIM 和 GPIO
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化GPIO
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	// 3. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC_Value - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM_Select,&TIM_TimeBaseInitStructure);
	
	// 4. 配置输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0x8;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	
	TIM_PWMIConfig(TIM_Select,&TIM_ICInitStructure);
	
	// 5. 触发源选择
	TIM_SelectInputTrigger(TIM_Select,TIM_TS_TI1FP1);
	
	// 6. 选择从模式
	TIM_SelectSlaveMode(TIM_Select,TIM_SlaveMode_Reset);
	
	// 7. 使能定时器
	TIM_Cmd(TIM_Select,ENABLE);
}

uint32_t IC_Get_Freq(void)
{
	// TIM_GetCapture1(TIM_Select) + 1: TIM3的硬件复位（Reset）优先级大于计数（Increment）
	// 当到达上升沿时，最后一个数还没加上，当前的CCR值就被锁存到 CCR1 寄存器
	return IC_Clock_Freq_Hz / (TIM_GetCapture1(TIM_Select) + 1);
}

int8_t IC_Get_Duty_Ratio(void)
{
	return (TIM_GetCapture2(TIM_Select) + 1) * 100 / (TIM_GetCapture1(TIM_Select) + 1);
}	
