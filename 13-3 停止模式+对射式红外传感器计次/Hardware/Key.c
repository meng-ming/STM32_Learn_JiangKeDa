#include "Key.h"

static const Key_Definition_t keys[] = 
{
    {GPIOB, GPIO_Pin_0},    // KEY_1 的定义
    {GPIOB, GPIO_Pin_1},    // KEY_2 的定义
};

#define NUM_KEYS (sizeof(keys)/sizeof(keys[0]))

void Key_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used)
{
	if(GPIOx == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(GPIOx == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	// 设置按键为上拉电阻模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Used;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOx,&GPIO_InitStructure);
}

void Key_Init_All(void)
{
	for(int i = 0;i< NUM_KEYS ;i++)
	{
		Key_Init(keys[i].port,keys[i].pin);
	}
}

bool Key_Pressed(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used)
{
	// 当前为上拉电阻模式，如果输入为零，则按键按下
	if(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_Used))
	{
		// 延时，消抖
		Delay_ms(20);
		// 确认是否真的被按下
		if(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_Used))
		{
			// 直到松开按键才退出
			while(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_Used));
			return true;
		}
	}
	return false;
}

Key_Pressed_Num_e Key_GetPressed_Num(void)
{
	for(int i = 0;i < NUM_KEYS;i++)
	{
		if(Key_Pressed(keys[i].port,keys[i].pin))
			return (Key_Pressed_Num_e)(i+1);
	}
	return KeyNone;
}
