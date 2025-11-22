#include "stm32f10x.h"                  // Device header

static TIM_TypeDef* TIM_Select = TIM3;

void Encoder_Init(void)
{
	// 使用 TIM3 的通道1和通道2 对编码器进行测速，对应的GPIO口为 PA6,PA7
	
	// 1. RCC 使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStruture;
	GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruture.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruture);
	
	// 3. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM_Select,&TIM_TimeBaseInitStructure);
	
	// 4. 配置 IC
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_ICFilter = 0x8;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM_Select,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_ICFilter = 0x8;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM_Select,&TIM_ICInitStructure);
	
	// 5. 配置编码器接口
	TIM_EncoderInterfaceConfig(TIM_Select,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);// Rising：不反转
	
	// 6. 使能时钟
	TIM_Cmd(TIM_Select,ENABLE);
}

int16_t Encoder_Get_CNT(void)
{
	// 返回两次调用该函数之间 TIM CNT 的值
	// 通过内置定时器中断设置中断频率，再在中断中调用该函数，则类似于求出编码器的速度
	int16_t temp = TIM_GetCounter(TIM_Select);
	TIM_SetCounter(TIM_Select,0);
	return temp;
}
