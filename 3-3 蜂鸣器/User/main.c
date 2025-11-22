#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	// 当前为低电平响
	int GPIO_Pin = GPIO_Pin_12;
	//1. 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	// 2.构造结构体，初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// 初始化PB12引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	while(1)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin);
		Delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin);
		Delay_ms(100);
		GPIO_ResetBits(GPIOB,GPIO_Pin);
		Delay_ms(700);
	}
}
