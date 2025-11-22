#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	// 当前为低电平点亮
	int delay_value = 500;
	//1. 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2.构造结构体，初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// 可以同时初始化多个引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	// 或者将某一类的引脚全部选中
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3.低电平点亮
	// GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	// GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
	
	while(1)
	{
		// 当前为低电平点亮，所以需要取反
		GPIO_Write(GPIOA,~0x0001);// 0000 0000 0000 0001 对应 PA0端口
		Delay_ms(delay_value);
		GPIO_Write(GPIOA,~0x0002);// 0000 0000 0000 0010 对应 PA2端口
		Delay_ms(delay_value);
		GPIO_Write(GPIOA,~0x0004);// 0000 0000 0000 0100 对应 PA3端口
		Delay_ms(delay_value);
		GPIO_Write(GPIOA,~0x0008);// 0000 0000 0000 1000 对应 PA4端口
		Delay_ms(delay_value);
		GPIO_Write(GPIOA,~0x0010);// 0000 0000 0001 0000 对应 PA5端口
		Delay_ms(delay_value);
		
	}
}
