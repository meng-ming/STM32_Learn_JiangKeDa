#ifndef __MY_RTC_H
#define __MY_RTC_H

#include "stm32f10x.h"                  // Device header
#include <time.h>

typedef struct tm Time_s;

/**
* @brief 初始化实时时钟
* @param *Time_Structure: struct tm 结构体指针
* @note	1. 需要使能 BKP、PWR 时钟以及 允许访问备份域
* @note 2. 对 RTC 写入寄存器之前，需要确保 RTOFF 标志位为 1
* @note 3. 使用 LSE 外部低频晶振时，需要确保其起振，否则可以使用 LSI 内部晶振
*/
void My_RTC_Init(Time_s* Time_Structure);

/**
* @brief 通过时间结构体设置系统内置的CNT
* @param *Time_Structure: struct tm 结构体指针
*/
void My_RTC_Set_Time(Time_s *Time_Structure);

/**
* @brief 通过系统内置的CNT获取当前时间
* @param *Time_Structure: struct tm 结构体指针
*/
void My_RTC_Get_Time(Time_s* Time_Structure);

#endif
