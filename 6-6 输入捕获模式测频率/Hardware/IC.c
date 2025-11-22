#include "stm32f10x.h"                  // Device header
#include "IC.h"

static uint16_t PSC_Value = 72;
static TIM_TypeDef* TIM_Select = TIM3; 
static uint32_t IC_Clock_Freq_Hz = 72000000 / 72;

void IC_Init(void)
{
	// 使用 TIM_Select 的通道一进行测量输入信号的频率，对应的输入GPIO引脚为 PA6;
	
	// 1.RCC使能 TIM 和 GPIO 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 不分频，第一次触发时即为有效
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 上升计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1 ; // ARR ,设置为最大值，尽可能防止计数溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC_Value - 1 ; // 测周法的技术标准频率
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM_Select,&TIM_TimeBaseInitStructure);
	
	// 4. 配置输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 通道
	TIM_ICInitStructure.TIM_ICFilter = 0x8; // 滤波范围，防止输入信号抖动
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 每次上升沿都能触发
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 当前为直连通道
	
	TIM_ICInit(TIM_Select,&TIM_ICInitStructure);
	
	// 5. 触发源选择
	TIM_SelectInputTrigger(TIM_Select,TIM_TS_TI1FP1); //
	
	// 6. 设置从模式
	TIM_SelectSlaveMode(TIM_Select,TIM_SlaveMode_Reset); // 重置模式，将记录的 CRR 值清零
	
	
	// 7. 定时器使能
	TIM_Cmd(TIM_Select,ENABLE);
}

uint32_t IC_Get_Freq(void)
{
	// TIM_GetCapture1(TIM_Select) + 1: TIM3的硬件复位（Reset）优先级大于计数（Increment）
	// 当到达上升沿时，最后一个数还没加上，当前的CCR值就被锁存到 CCR1 寄存器
	return IC_Clock_Freq_Hz / (TIM_GetCapture1(TIM_Select) + 1);
}
