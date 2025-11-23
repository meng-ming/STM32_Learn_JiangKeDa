#include "stm32f10x.h"                  // Device header

int main(void)
{
	// 1.使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 2.配置端口模式
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	// 3.设置高低电平
	// GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	while(1)
	{
		
	}
}
