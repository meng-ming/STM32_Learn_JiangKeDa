#ifndef __STORE_H
#define __STORE_H

#include "stm32f10x.h"

/* 公开宏定义，方便外部引用数组大小时使用 */
#define STORE_COUNT  512 

/**
 * @brief  SRAM 缓存数组
 * @note   修改此数组的数据后，调用 Store_Save() 即可保存到 Flash
 */
extern uint16_t Store_Data[STORE_COUNT];

/**
 * @brief  参数存储模块初始化
 * @note   上电时调用。逻辑如下：
 * 1. 检查 Flash 标志位。
 * 2. 若是第一次使用，格式化 Flash 并初始化为 0。
 * 3. 若非第一次，将 Flash 数据加载到 Store_Data[] 缓存中。
 * @retval None
 */
void Store_Init(void);

/**
 * @brief  将 SRAM 数组的数据保存到 Flash
 * @note   此函数会先擦除对应页，然后重新写入所有数据。
 * 建议仅在参数修改确定的时刻调用，避免频繁擦写损耗寿命。
 * @retval None
 */
void Store_Save(void);

/**
 * @brief  清空参数 (恢复出厂设置)
 * @note   将所有数据清零并保存。
 * @retval None
 */
void Store_Clear(void);

#endif
