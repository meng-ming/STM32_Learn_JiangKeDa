#include "stm32f10x.h"                  // Device header
#include "LED.h"

// 定义包含所有 LED 的数组
static LED_Definition_t LEDs[] =
{
	{GPIOA,GPIO_Pin_1,OFF},
	{GPIOA,GPIO_Pin_2,OFF}
};

#define NUM_LEDS sizeof(LEDs)/sizeof(LEDs[0])

void LED_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used)
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

void LED_Init_All(void)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		LED_Init(LEDs[i].port,LEDs[i].pin);
	}
}

void Set_LED_State(LED_Num_e LED_Num,LED_State_e state)
{
	// 判断是否越界
	if(LED_Num >= NUM_LEDS)
		return;

	// LED 默认低电平点亮，高电平熄灭
	if(state == ON)
	{
		GPIO_ResetBits(LEDs[LED_Num].port,LEDs[LED_Num].pin);
		LEDs[LED_Num].state = ON;
	}
	else
	{
		GPIO_SetBits(LEDs[LED_Num].port,LEDs[LED_Num].pin);
		LEDs[LED_Num].state = OFF;
	}
}

void LED_State_Toggle(LED_Num_e LED_Num)
{
	// 边界处理
	if(LED_Num >= NUM_LEDS)
		return;

	LED_State_e current_state = LEDs[LED_Num].state;

	// 读取对应 LED 引脚的输出或者直接读取当前 LED 的状态
	// 如果当前为高电平输出，表示当前 LED 熄灭
	// if(GPIO_ReadOutputDataBit(LEDs[LED_Num].port,LEDs[LED_Num].pin) == Bit_SET)
	if(current_state == OFF)
		Set_LED_State(LED_Num,ON);
	else
		Set_LED_State(LED_Num,OFF);
}
