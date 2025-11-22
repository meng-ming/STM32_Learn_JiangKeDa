#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

/**
 * @brief  初始化 SysTick 心跳，必须在所有程序前调用一次
 * @param  None
 * @retval None
 */
void Delay_Init(void);

/**
 * @brief  毫秒级延时 (基于 SysTick 心跳)
 * @param  xms 延时时长 (毫秒)
 * @retval None
 */
void Delay_ms(uint32_t xms);

/**
 * @brief  微秒级延时 (CPU 循环，不精确，但不会破坏中断)
 * @param  xus 延时时长 (微秒)
 * @retval None
 */
void Delay_us(uint32_t xus);

/**
 * @brief  获取自系统启动以来的毫秒数
 * @note   这是软件消抖的关键
 * @retval uint32_t: 当前的毫秒数
 */
uint32_t Get_Current_Time_ms(void);

#endif
