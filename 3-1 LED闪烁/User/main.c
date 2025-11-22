#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	// 当前为低电平点亮
	//1. 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2.构造结构体，初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3.低电平点亮
	// GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	// GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
	
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);// 设置为高电平，不亮
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);// 设置为低电平，小灯亮
		Delay_ms(500);
	}
}
