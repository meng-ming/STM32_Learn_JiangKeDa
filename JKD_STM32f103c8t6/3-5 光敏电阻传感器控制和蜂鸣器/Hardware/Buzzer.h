#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

typedef enum
{
    Sound,
    No_Sound
}Buzzer_State_e;

typedef enum
{
	Buzzer_1,
}Buzzer_Num_e;

typedef struct
{
    GPIO_TypeDef* port;// 蜂鸣器使用的端口(例如GPIOA)
    uint16_t pin;// 蜂鸣器使用的引脚号
    Buzzer_State_e state;// 蜂鸣器状态
}Buzzer_Definition_t;

void Buzzer_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used);// 初始化蜂鸣器 GPIO

void Buzzer_Init_All(void);// 初始化所有的蜂鸣器

void Set_Buzzer_State(Buzzer_Num_e Buzzer_Num,Buzzer_State_e state); // 设置对应LED的状态

#endif
