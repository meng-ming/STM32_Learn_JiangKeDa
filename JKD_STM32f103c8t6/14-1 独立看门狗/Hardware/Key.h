#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdbool.h>

typedef struct
{
    GPIO_TypeDef* port;// 按键使用的端口(例如GPIOA)
    uint16_t pin;// 按键使用的引脚号
}Key_Definition_t;

typedef enum
{
    KeyNone,
    Key_1,
    Key_2
}Key_Pressed_Num_e;

void Key_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used);// 初始化按键所在的GPIO口

void Key_Init_All(void);// 初始化所有的按键

bool Key_Pressed(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used);// 判断按键是否被按下

Key_Pressed_Num_e Key_GetPressed_Num(void);// 读取当前是哪个按键按下的

#endif
