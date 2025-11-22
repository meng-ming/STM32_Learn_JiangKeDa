#include "stm32f10x.h"

// 全局"心跳"变量，必须是 volatile
static volatile uint32_t g_ticks_ms = 0;

/**
 * @brief  初始化 SysTick 定时器，设置为 1ms 中断一次
 */
void Delay_Init(void)
{
    // 配置 SysTick 为 1ms 中断一次 (假设 SystemCoreClock 已被 SystemInit 设为 72MHz)
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        // 如果配置失败，进入死循环
        while (1);
    }
}


/**
 * @brief  微秒级延时 (粗略的 CPU 循环延时)
 */
void Delay_us(uint32_t xus)
{
    volatile uint32_t i;
    for(; xus > 0; xus--)
    {
        for(i = 0; i < 8; i++); // (72MHz 下 8 个循环约 1us)
    }
}

/**
 * @brief  毫秒级延时 (基于 SysTick 中断)
 */
void Delay_ms(uint32_t xms)
{
    uint32_t start_time = g_ticks_ms;
    
    // 利用无符号整数的回绕特性，安全地等待
    while (g_ticks_ms - start_time < xms);
}
 
/**
 * @brief  获取自系统启动以来的毫秒数
 */
uint32_t Get_Current_Time_ms(void)
{
    return g_ticks_ms;
}

/**
 * @brief  SysTick 中断服务函数 (硬件自动调用)
 * @note   这个函数必须是全局的，不能是 static
 */
void SysTick_Handler(void)
{
    // 心跳计数器自增
    g_ticks_ms++;
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
