#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// 定义一个宏定义，当两次计数中断间隔小于当前值时，不进行计数
#define DEBOUNCE_TIME_MS 20

// 阻挡次数
uint16_t volatile Count_Sensor_Count = 0;

// 消除系统一次性识别过多的阻挡，导致 count 计数跳变
// 通过一个 1ms 的系统中断函数
static volatile uint32_t Last_Trigger_Time = 0;

void Count_Sensor_Init(void)
{
	// 1. 初始化时钟 GPIO 和 AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStruture;
	GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruture.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&GPIO_InitStruture);

	// 3. AFIO 外部中断引脚选择配置(选择当前外设连接的 GPIOB 14 号引脚)
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

	// 4. 配置 EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 与引脚号对应
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	
	EXTI_Init(&EXTI_InitStructure);
	
	// 5. 配置 NVIC
	
	// 先分组，将该工程下的所有外设分为几个组 总共为4份
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // 终端外设引脚对应的分组
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;// 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

// 中断函数：固定格式，无参无返回值，得使用自己外设引脚对应的中断函数
void EXTI15_10_IRQHandler(void)
{
	// 由于当前函数不止一个引脚能进入该函数，判断是否是当前外设所在的引脚号
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		// 获取触发中断时系统时间
		uint32_t Current_Time = Get_Current_Time_ms();
		
		// 判断两次中断触发的时间间隔是否大于设定值
		if(Current_Time - Last_Trigger_Time > DEBOUNCE_TIME_MS)
		{
			// 有效计数
			Count_Sensor_Count++;
			
			// 更新中断触发时间
			Last_Trigger_Time = Current_Time;
		}
		// 执行完中断过程之后，需要消除中断，否则CPU会一直处理这个中断
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}	
