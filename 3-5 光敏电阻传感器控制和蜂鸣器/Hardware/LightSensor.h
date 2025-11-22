#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stm32f10x.h"                  // Device header

typedef enum
{
    Light,
    Dark
}Light_Sensor_State_e;

typedef enum
{
	Light_Sensor_1,
}Light_Sensor_Num_e;

typedef struct
{
    GPIO_TypeDef* port;// 光敏控制器使用的端口(例如GPIOA)
    uint16_t pin;// 光敏控制器使用的引脚号
    Light_Sensor_State_e state;// 光敏控制器状态
}Light_Sensor_Definition_t;

void Light_Sensor_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used);// 初始化光敏控制器 GPIO

void Light_Sensor_Init_All(void);// 初始化所有的光敏控制器

Light_Sensor_State_e Get_Light_Sensor_State(Light_Sensor_Num_e Light_Sensor_Num); // 获取对应光敏控制器的状态

#endif
