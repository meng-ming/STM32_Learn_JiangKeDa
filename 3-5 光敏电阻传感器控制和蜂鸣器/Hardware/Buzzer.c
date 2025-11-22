#include "Buzzer.h"

// 定义包含所有 LED 的数组
static Buzzer_Definition_t Buzzers[] =
{
	{GPIOB,GPIO_Pin_12,No_Sound},
};

#define NUM_BUZZERS sizeof(Buzzers)/sizeof(Buzzers[0])

void Buzzer_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used)
{
	if (GPIOx == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Used;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);

	// 设置使用的引脚初始化为高电平，防止默认打开LED
	GPIO_SetBits(GPIOx,GPIO_Pin_Used);
}

void Buzzer_Init_All(void)
{
	for (int i = 0; i < NUM_BUZZERS; i++)
	{
		Buzzer_Init(Buzzers[i].port,Buzzers[i].pin);
	}
}

void Set_Buzzer_State(Buzzer_Num_e Buzzer_Num,Buzzer_State_e state)
{
	// 判断是否越界
	if(Buzzer_Num >= NUM_BUZZERS)
		return;

	// LED 默认低电平点亮，高电平熄灭
	if(state == Sound)
	{
		GPIO_ResetBits(Buzzers[Buzzer_Num].port,Buzzers[Buzzer_Num].pin);
		Buzzers[Buzzer_Num].state = Sound;
	}
	else
	{
		GPIO_SetBits(Buzzers[Buzzer_Num].port,Buzzers[Buzzer_Num].pin);
		Buzzers[Buzzer_Num].state = No_Sound;
	}
}
