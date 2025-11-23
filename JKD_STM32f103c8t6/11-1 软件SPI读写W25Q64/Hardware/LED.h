#ifndef __LED_H
#define __LED_H

typedef enum {
	OFF = 0, // 熄灭
	ON = 1 // 点亮
}LED_State_e;

typedef struct
{
    GPIO_TypeDef* port;// LED使用的端口(例如GPIOA)
    uint16_t pin;// LED使用的引脚号
	LED_State_e state;// LED 当前状态
}LED_Definition_t;

typedef enum
{
	LED_1,
	LED_2
}LED_Num_e;

void LED_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used);// 初始化LED GPIO

void LED_Init_All(void);// 初始化所有的LED

void Set_LED_State(LED_Num_e LED_Num,LED_State_e state); // 设置对应LED的状态

void LED_State_Toggle(LED_Num_e LED_Num);// 切换LED当前状态（例如点亮->熄灭）

#endif
